#include "stdafx.h"
#include "install_util.h"
#include <assert.h>
#include <Windows.h>
#include <comdef.h>
#include <Shellapi.h>
//锁定到任务栏使用
#include <shlobj.h>
#include <comdef.h>
#include <atlbase.h>
#include <propkey.h>
#include <propvarutil.h>
//
#include <tlhelp32.h> //检测进程是否存在使用
#include "res_filename.h"
#include <Psapi.h>

BOOL install_util::Win7OrLater()
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi);
	return osvi.dwMajorVersion > 6 || (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion >= 1);
}

BOOL install_util::IsVistaSystem()
{
  OSVERSIONINFOEX osvi;
  BOOL bOsVersionInfoEx;

  ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi);
  return osvi.dwMajorVersion >= 6;
}

int  install_util::ReplaceString(std::wstring& src_str, const std::wstring& old_str, const std::wstring& new_str)
{
  int count = 0;
  std::wstring::size_type old_str_len = old_str.length(), new_str_len = new_str.length();
  std::wstring::size_type pos = 0;
  while ((pos = src_str.find(old_str, pos)) != std::wstring::npos)
  {   
    src_str.replace(pos, old_str_len, new_str);
    pos += new_str_len;
    ++count;
  }
  return count;
}

LONG install_util::GetValueReg(HKEY hMainKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, LPBYTE lpBuf, DWORD dwBufLen)
{
  if (NULL == lpSubKey || NULL == lpValueName || NULL == lpBuf || dwBufLen <= 0)
    return -1;

  HKEY hKey = NULL;
  LONG lRes = RegOpenKeyEx(hMainKey, lpSubKey, 0, KEY_READ, &hKey);
  if(lRes != ERROR_SUCCESS)
    return -1;

  DWORD nBytes = dwBufLen;
  lRes = ::RegQueryValueEx(hKey, lpValueName, NULL, &dwType, lpBuf, &nBytes);
  RegCloseKey(hKey);
  return lRes;
}

LONG install_util::SetValueReg(HKEY hMainKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, LPBYTE lpBuf, DWORD dwBufLen)
{
  if (NULL == lpSubKey || NULL == lpValueName || NULL == lpBuf || dwBufLen <= 0)
    return -1;

  HKEY hKey = NULL;
  if ((ERROR_SUCCESS != RegOpenKeyEx(hMainKey, lpSubKey, 0, KEY_WRITE, &hKey)) && 
    (ERROR_SUCCESS != RegCreateKey(hMainKey, lpSubKey, &hKey)))
    return -1;

  LONG lRes = RegSetValueEx(hKey, lpValueName, 0, dwType, (BYTE*)lpBuf, dwBufLen);
  RegCloseKey(hKey);
  return lRes;
}

UINT GetNextDotInt(LPCWSTR& p)
{
  UINT v = _tstol(p);
  p = wcschr(p, L'.');
  if (p)
    ++p;
  return v;
}

int install_util::CompareVersionString(LPCWSTR szVerL, LPCWSTR szVerR) 
{
  LPCWSTR pL = szVerL;
  LPCWSTR pR = szVerR;
  do
  {
    UINT vL = GetNextDotInt(pL);
    UINT vR = GetNextDotInt(pR);
    if (vL != vR)
      return vL - vR;
  } while(pL && pR);
  return 0; //相等
}

BOOL install_util::MakeSureDirectoryPathExists(LPCWSTR dir)
{
   DWORD ret = GetFileAttributes(dir);
  if (ret == INVALID_FILE_ATTRIBUTES)
  {
    int ret = ::SHCreateDirectoryExW(NULL,dir,NULL);
    if (ret == ERROR_SUCCESS || ret == ERROR_ALREADY_EXISTS || ret == ERROR_FILE_EXISTS)
      return TRUE;
    return FALSE;
  }
  return TRUE;
}

bool install_util::PathExists(const wstring& path) {
  return (GetFileAttributes(path.c_str()) != INVALID_FILE_ATTRIBUTES);
}

bool install_util::IsFolder(const wstring& path) {
  return PathExists(path) && ((GetFileAttributes(path.c_str()) & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
}

BOOL install_util::CreateLnkPath(std::wstring wsSourceFilePath, std::wstring wsDestLnkPath, std::wstring wsArgument, std::wstring wsAppId)
{
  IShellLink *pisl = NULL;
  HRESULT hr = CoCreateInstance(CLSID_ShellLink,
    NULL,
    CLSCTX_INPROC_SERVER,
    IID_IShellLink,
    (void**)&pisl);
  if (FAILED(hr))
  {
    return FALSE;
  }

  pisl->SetPath(wsSourceFilePath.c_str());
  pisl->SetArguments(wsArgument.c_str());
  int nStart = wsSourceFilePath.find_last_of(_T("/\\")); 
  pisl->SetWorkingDirectory(wsSourceFilePath.substr(0,nStart).c_str());
  IPersistFile *plPF = NULL; 
  hr = pisl->QueryInterface(IID_IPersistFile, (void**)&plPF);
  bool shortcut_existed = false;
  if (SUCCEEDED(hr))
  {
    if (PathExists(wsDestLnkPath))
    {
      shortcut_existed = true;
      install_util::DeleteFile(wsDestLnkPath.c_str());
    }
	if (Win7OrLater() && !wsAppId.empty() && wsAppId.length() < 64)
	{
		IPropertyStore *piPS = NULL;
		if (SUCCEEDED(pisl->QueryInterface(IID_IPropertyStore, (void**)&piPS)))
		{
			PROPVARIANT property_value;
			if (SUCCEEDED(InitPropVariantFromString(wsAppId.c_str(), &property_value)))
			{
				if (piPS->SetValue(PKEY_AppUserModel_ID, property_value) == S_OK)
					piPS->Commit();
				PropVariantClear(&property_value);
			}
			piPS->Release();
		}
	}

    hr = plPF->Save(wsDestLnkPath.c_str(), TRUE);
    plPF->Release();
  }

  pisl->Release();
  SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

  return SUCCEEDED(hr);
}
//锁定到任务栏
#define IF_FAILED_OR_NULL_BREAK(rv,ptr) {if (FAILED(rv) || ptr == NULL) break;}  

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }  

BOOL install_util::DeleteFile(LPCTSTR pszFile)
{
  if (::DeleteFile(pszFile))
    return TRUE;

  wstring tmpFile = pszFile;
  tmpFile += L"_yytmp";
  if (MoveFileEx(pszFile, tmpFile.c_str(), MOVEFILE_REPLACE_EXISTING))
    return MoveFileEx(tmpFile.c_str(),NULL,MOVEFILE_DELAY_UNTIL_REBOOT);

  return FALSE;
}

BOOL install_util::IsFolderEmpty(LPCTSTR pszFolder)
{
  BOOL bRet = TRUE;
  HANDLE hFind;
  WIN32_FIND_DATA FindFileData;
  std::wstring wsFilePath = pszFolder;
  if(wsFilePath[wsFilePath.size() - 1] != L'\\')
    wsFilePath.append(L"\\");
  wsFilePath.append(L"*");
  if((hFind=FindFirstFile(wsFilePath.c_str(), &FindFileData)) != INVALID_HANDLE_VALUE)
  {
    BOOL bFind = TRUE;   
    BOOL EmptyDirectory = TRUE;
    while(bFind)
    {
      if(wcscmp(FindFileData.cFileName, L".") == 0
        || wcscmp(FindFileData.cFileName, L"..") == 0)
      {
        bFind = FindNextFile(hFind, &FindFileData);
      }
      else
      {
        //有子文件夹也算非空
        EmptyDirectory = FALSE;
        break;
      }
    }
    if(EmptyDirectory)
    {
      bRet = TRUE;//Empty
    }
    else
    {
      bRet = FALSE;//Not Empty
    }
  }
  else
  {
    bRet = TRUE;//Path does not exist. Treat as Empty
  }
  FindClose(hFind);
  hFind = NULL;
  return bRet;
}

BOOL install_util::DeleteFolder(LPCTSTR pszFolder)
{
  if(IsFolderEmpty(pszFolder))
    return RemoveDirectory(pszFolder);

  //下面的实现据说有隐患。应改为递归删除所有子文件及文件夹
  _TCHAR szPath[MAX_PATH + 1] = {0};
  _sntprintf_s(szPath, _countof(szPath), sizeof(szPath), _TEXT("%s%c"), pszFolder, 0);

  SHFILEOPSTRUCT fos ;
  ZeroMemory(&fos, sizeof( fos)) ;
  fos.hwnd = HWND_DESKTOP;
  fos.wFunc = FO_DELETE ;
  fos.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
  fos.pFrom = szPath;

  // 删除文件夹及其内容
  if (0 == SHFileOperation(&fos))
    return TRUE;

  wstring tmpFile = szPath;
  tmpFile += L"_yytmp";
  if (MoveFileEx(szPath, tmpFile.c_str(), MOVEFILE_REPLACE_EXISTING))
    return MoveFileEx(tmpFile.c_str(),NULL,MOVEFILE_DELAY_UNTIL_REBOOT);

  return FALSE;
} 

int install_util::GetProcessCount(const TCHAR* szExeName)
{
  TCHAR sztarget[MAX_PATH] = {0};
  lstrcpy(sztarget, szExeName);
  CharLowerBuff(sztarget, MAX_PATH);

  int count = 0;
  PROCESSENTRY32 my = {0};
  HANDLE l = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (((int)l) != -1)
  {
    my.dwSize = sizeof(my);
    if (Process32First(l, &my))
    {
      do
      {
        CharLowerBuff(my.szExeFile, MAX_PATH);
        if (lstrcmp(sztarget, my.szExeFile) == 0)
        {
          count++;
        }
      }while (Process32Next(l, &my));
    }
    CloseHandle(l);
  }

  return count;
}

void install_util::NotifyProcessExit(const TCHAR* szExeName) {
  TCHAR sztarget[MAX_PATH] = {0};
  lstrcpy(sztarget, szExeName);
  CharLowerBuff(sztarget, MAX_PATH);

  BOOL bResult = TRUE;
  PROCESSENTRY32 my = {0};
  HANDLE l = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (((int)l) != -1) 
  {
    my.dwSize = sizeof(my);
    if (Process32First(l, &my))
    {
      do
      {
        CharLowerBuff(my.szExeFile, MAX_PATH);
        if (lstrcmp(sztarget, my.szExeFile) == 0)
        {
          HANDLE pProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, my.th32ProcessID);
          TCHAR lpszModule[MAX_PATH + 1] = {0};
          GetModuleFileNameEx(pProcess, NULL, lpszModule, MAX_PATH + 1);
          if (lpszModule[0]) 
          {
            std::wstring wstrRunYYECmdLine(L"quit");
            SHELLEXECUTEINFO ShExecInfo = {0};
            ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
            ShExecInfo.fMask = 0;
            ShExecInfo.hwnd = NULL;
            ShExecInfo.lpVerb = NULL;

            ShExecInfo.lpFile = lpszModule;
            // 数据统计需要知道启动的来源
            ShExecInfo.lpParameters = wstrRunYYECmdLine.c_str();
            ShExecInfo.lpDirectory = NULL;
            ShExecInfo.nShow = SW_SHOW;
            ShExecInfo.hInstApp = NULL;
            ShellExecuteEx(&ShExecInfo);
            break;
          }
        }
      }while (Process32Next(l, &my));
    }
    CloseHandle(l);
  }
}

bool ProcessExists(const TCHAR* szExeName) {
  bool bResult = false;
  bool bRetCode = false;
  TCHAR sztarget[MAX_PATH] = {0};
  BOOL bFind   = FALSE;
  PROCESSENTRY32 my = {0};
  HANDLE l = NULL;

  lstrcpy(sztarget, szExeName);
  CharLowerBuff(sztarget, MAX_PATH);

  l = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  YG_PROCESS_ERROR(l != INVALID_HANDLE_VALUE);

  my.dwSize = sizeof(my);
  bRetCode = !!Process32First(l, &my);
  YG_PROCESS_ERROR(bRetCode);
  
  do
  {
    CharLowerBuff(my.szExeFile, MAX_PATH);
    if (lstrcmp(sztarget, my.szExeFile) == 0) {
      bResult = true;
      break;
    }
  }while (Process32Next(l, &my));

Exit0:
  if (l) {
    CloseHandle(l);
    l = NULL;
  }
  return bResult;
}

bool install_util::WaitProcessExit(const TCHAR* szExeName, int nMilliseconds) {
  if (nMilliseconds <= 0)
    return !ProcessExists(szExeName);

  DWORD dwStartTime = ::GetTickCount();
  do {
    if (!ProcessExists(szExeName))
      return true;

    if (::GetTickCount() - dwStartTime >= (size_t)nMilliseconds)
      return false;

    ::Sleep(1);
  } while (1);

  assert(false && "NOTREACHE");
  return false;
}

int install_util::KillProcess(const TCHAR* szExeName)
{
  TCHAR sztarget[MAX_PATH] = {0};
  lstrcpy(sztarget, szExeName);
  CharLowerBuff(sztarget, MAX_PATH);

  BOOL bFind   = FALSE;
  BOOL bResult = TRUE;
  PROCESSENTRY32 my = {0};
  HANDLE l = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (((int)l) != -1)
  {
    my.dwSize = sizeof(my);
    if (Process32First(l, &my))
    {
      do
      {
        CharLowerBuff(my.szExeFile, MAX_PATH);
        if (lstrcmp(sztarget, my.szExeFile) == 0)
        {
          bFind = TRUE;
          HANDLE handLe =  OpenProcess(PROCESS_TERMINATE , FALSE, my.th32ProcessID);
          bResult &= ::TerminateProcess(handLe, 0);
        }
      }while (Process32Next(l, &my));
    }
    CloseHandle(l);
  }

  return bFind && bResult;//找到并关闭成功才返回TRUE
}

std::wstring install_util::GetFileProductVersion(std::wstring wsFilePath)
{
  DWORD dwLen = 0;
  WCHAR* lpData=NULL;
  BOOL bSuccess = FALSE;

  dwLen = GetFileVersionInfoSize(wsFilePath.c_str(), 0);
  if (0 == dwLen)
    return L"";

  lpData =new WCHAR [dwLen+1];
  bSuccess = GetFileVersionInfo(wsFilePath.c_str(), 0, dwLen, lpData);
  if (!bSuccess)
  {
    delete lpData;
    return L"";
  }

  LPVOID lpBuffer = NULL;
  UINT uLen = 0;
  bSuccess = VerQueryValue(lpData,
                           _TEXT("\\StringFileInfo\\040904b0\\ProductVersion"),
                           &lpBuffer, 
                           &uLen);  
  if (!bSuccess)
  {
    delete lpData;
    return L"";
  }

  std::wstring wsRet = (LPCTSTR)lpBuffer;

  delete [] lpData;//此处不需要释放lpBuffer指向的空间,因为lpBuffer和lpData指向同一块内存空间
  return wsRet;
}

BOOL install_util::ForeGroundInstallWindow()
{
  HWND hWnd= FindWindow(_T("#32770"), INSTALL_TITLE);
  if(NULL == hWnd)
    return FALSE;
  HWND hForeWnd =  GetForegroundWindow(); 
  DWORD dwCurID =  GetCurrentThreadId(); 
  DWORD dwForeID =  GetWindowThreadProcessId(hForeWnd, NULL); 
  AttachThreadInput(dwCurID, dwForeID, TRUE); 
  ShowWindow( hWnd, SW_SHOWNORMAL ); 
  SetWindowPos( hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
  SetWindowPos( hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
  SetForegroundWindow( hWnd ); 
  AttachThreadInput(dwCurID, dwForeID, FALSE);
  return TRUE;
}

bool install_util::CmdlineGetOpt(LPCTSTR argv, const std::wstring& opt_name, std::wstring* opt_value)
{
  bool bRetCode = false;
  bool bResult = false;
  std::wstring find_opt(L"--");
  std::wstring inner_value;
  bool bFindFlag = false;

  YG_PROCESS_ERROR(argv);
  YG_PROCESS_ERROR(!opt_name.empty());
  YG_PROCESS_ERROR(opt_value);

  find_opt.append(opt_name + L"=");
  bRetCode = !!wcsncmp(find_opt.c_str(), argv, find_opt.length());
  YG_PROCESS_ERROR (!bRetCode);

  inner_value = argv + find_opt.length();
  bFindFlag = true;
  YG_PROCESS_ERROR(bFindFlag);

  *opt_value = inner_value;
  bResult = true;
Exit0:
  return bResult;
}

bool install_util::CmdlineIs(LPCTSTR argv, const std::wstring& opt_name)
{
  bool bRetCode = false;
  bool bResult = false;
  std::wstring find_opt(L"--");
  std::wstring inner_value;

  YG_PROCESS_ERROR(argv);
  YG_PROCESS_ERROR(!opt_name.empty());

  find_opt.append(opt_name);
  bRetCode = !!wcsncmp(find_opt.c_str(), argv, find_opt.length());
  YG_PROCESS_ERROR (!bRetCode);
  YG_PROCESS_ERROR (*(argv + find_opt.length()) == L'\0');

  bResult = true;
Exit0:
  return bResult;
}

