#include "stdafx.h"
#include "uninstall_util.h"
#include <Windows.h>
#include <tlhelp32.h> //检测进程是否存在使用
//锁定到任务栏使用
#include <shlobj.h>
#include <comdef.h>
#include <atlbase.h>
//
#include <shellapi.h>
#include "yygame/ygdata_report/md5.h"
#include <Wininet.h> 
//
#include "lang_string.h"
#include "res_filename.h"

BOOL uninstall_util::IsVistaSystem()
{
  OSVERSIONINFOEX osvi;
  BOOL bOsVersionInfoEx;

  ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi);
  return osvi.dwMajorVersion >= 6;
}

int  uninstall_util::ReplaceString(std::wstring& src_str, const std::wstring& old_str, const std::wstring& new_str)
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

LONG uninstall_util::GetValueReg(HKEY hMainKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, LPBYTE lpBuf, DWORD dwBufLen)
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

LONG uninstall_util::SetValueReg(HKEY hMainKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, LPBYTE lpBuf, DWORD dwBufLen)
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

BOOL uninstall_util::GetDesktopDstPath( const std::wstring wsDesktopLnkFileName,std::wstring& wsDestPath )
{
  CoInitialize(NULL);
  wchar_t path[MAX_PATH + 1] = {0};
  wchar_t desktoppath[MAX_PATH + 1] = {0};
  SHGetSpecialFolderPathW(NULL, desktoppath, CSIDL_DESKTOP, FALSE);
  CComPtr<IPersistFile> pPF;
  CComPtr<IShellLink> pSL;
  HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)(&pSL));
  if(SUCCEEDED(hr))
  {
    hr = pSL->QueryInterface(IID_IPersistFile, (void**)(&pPF));
    if(SUCCEEDED(hr))
    {
      wcscat(desktoppath, L"\\");
      wcscat(desktoppath, wsDesktopLnkFileName.c_str());
      hr = pPF->Load(desktoppath, STGM_READ);
      if(SUCCEEDED(hr))
      {
        hr = pSL->Resolve(NULL, SLR_NO_UI);
        if (SUCCEEDED(hr)) {
          //hr = pSL->GetPath(path, MAX_PATH, NULL, 0);//飞机票获取不到
          LPITEMIDLIST idl;
          hr = pSL->GetIDList(&idl);
          //BOOL rt = SHGetPathFromIDListEx(idl, path, MAX_PATH, GPFIDL_DEFAULT  );//xp不能用
          BOOL rt = SHGetPathFromIDList(idl, path);//如果是飞机票，获取到的字符串是“\0y://yxdt-[...]”
        }
      }
    }
  }
  CoUninitialize();
  //将“\0y:/...”改为“yy:/...”
  if(path[1] == L'y' && path[2] == L':' && path[3] == L'/')
    path[0] = L'y';
  wsDestPath = path;
  return wsDestPath != L"";
}

BOOL uninstall_util::MakeSureDirectoryPathExists(LPCWSTR dir)
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

BOOL uninstall_util::DeleteFile(LPCTSTR pszFile)
{
  if (::DeleteFile(pszFile))
    return TRUE;

  wstring tmpFile = pszFile;
  tmpFile += L"_yytmp";
  if (MoveFileEx(pszFile, tmpFile.c_str(), MOVEFILE_REPLACE_EXISTING))
    return MoveFileEx(tmpFile.c_str(),NULL,MOVEFILE_DELAY_UNTIL_REBOOT);

  return FALSE;
}

bool uninstall_util::PathExists(const wstring& path) {
  return _waccess(path.c_str(), 0) == 0;
}

bool uninstall_util::IsFolder(const wstring& path) {
  return PathExists(path) && ((GetFileAttributes(path.c_str()) & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
}

BOOL uninstall_util::IsFolderEmpty( LPCTSTR pszFolder )
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

BOOL uninstall_util::DeleteFolder(LPCTSTR pszFolder)
{
  if(IsFolderEmpty(pszFolder))
    return RemoveDirectory(pszFolder);

  _TCHAR szPath[MAX_PATH + 1] = {0};
  _sntprintf_s(szPath, _countof(szPath), sizeof(szPath), _TEXT("%s%c"), pszFolder, 0);

  SHFILEOPSTRUCT fos ;
  ZeroMemory(&fos, sizeof( fos)) ;
  fos.hwnd = HWND_DESKTOP;
  fos.wFunc = FO_DELETE ;
  fos.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
  fos.pFrom = szPath;
  fos.pTo = NULL;

  // 删除文件夹及其内容
  if (0 == SHFileOperation(&fos))
    return TRUE;

  wstring tmpFile = szPath;
  tmpFile += L"_yytmp";
  if (MoveFileEx(szPath, tmpFile.c_str(), MOVEFILE_REPLACE_EXISTING))
    return MoveFileEx(tmpFile.c_str(),NULL,MOVEFILE_DELAY_UNTIL_REBOOT);

  return FALSE;
}

int uninstall_util::GetProcessCount(const TCHAR* szExeName)
{
  TCHAR sztarget[MAX_PATH];
  lstrcpy(sztarget, szExeName);
  CharLowerBuff(sztarget, MAX_PATH);

  int count = 0;
  PROCESSENTRY32 my;
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

int uninstall_util::CloseProcess(const TCHAR* szExeName)
{
  TCHAR sztarget[MAX_PATH];
  lstrcpy(sztarget, szExeName);
  CharLowerBuff(sztarget, MAX_PATH);

  BOOL bFind   = FALSE;
  BOOL bResult = TRUE;
  PROCESSENTRY32 my;
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
          bResult &= TerminateProcess(handLe,0);
        }
      }while (Process32Next(l, &my));
    }
    CloseHandle(l);
  }

  return bFind && bResult;//找到并关闭成功才返回TRUE
}


BOOL uninstall_util::SelfDel() 
{
	TCHAR szPathOrig[MAX_PATH];
	TCHAR szTempPath[MAX_PATH];
	TCHAR szPathClone[MAX_PATH];
	TCHAR szCmdLine[MAX_PATH * 3]; 
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	DWORD dwRetCode = 0;
	HANDLE hProcessOrig = NULL;
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	si.cb = sizeof(si);

	dwRetCode = GetModuleFileName(NULL, szPathOrig, MAX_PATH);
	YG_PROCESS_ERROR(dwRetCode > 0);

	dwRetCode = GetTempPath(MAX_PATH, szTempPath);
	YG_PROCESS_ERROR(dwRetCode > 0);

	dwRetCode = GetTempFileName(szTempPath, __TEXT("Del"), 0, szPathClone);
	YG_PROCESS_ERROR(dwRetCode > 0);

	bRetCode = CopyFile(szPathOrig, szPathClone, FALSE);
	YG_PROCESS_ERROR(bRetCode);

	hProcessOrig = OpenProcess(SYNCHRONIZE, TRUE, GetCurrentProcessId());
	YG_PROCESS_ERROR(hProcessOrig);

	wsprintf(szCmdLine, __TEXT("%s -d %d \"%s\""), szPathClone, hProcessOrig, szPathOrig);
	bRetCode = CreateProcess(NULL, szCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	YG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
Exit0:
	if (pi.hThread)
	{
		CloseHandle(pi.hThread);
		pi.hThread = NULL;
	}

	if (pi.hProcess)
	{
		CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
	}

	if (hProcessOrig)
	{
		CloseHandle(hProcessOrig);
		hProcessOrig = NULL;
	}
	return bResult;
}

bool uninstall_util::md5(const char* str, int len, std::string& val)
{
  if(NULL == str || len == 0)
    return false;

  const int md5_len = 16;
  unsigned char result[md5_len];
  char hex[33];
  MD5Context ctx;
  MD5Init(&ctx);
  MD5Update(&ctx, (const unsigned char*)str, len);
  MD5Final(result, &ctx);

  for (int i = 0; i < md5_len; i++)
    sprintf(hex + 2 * i, "%02x", result[i]);
  val.assign(hex, 32);

  return true;
}

//模拟浏览器发送HTTP请求函数  
std::string uninstall_util::HttpRequest(char * lpHostName,short sPort,char * lpUrl,char * lpMethod,char * lpPostData,int nPostDataLen)  
{  
  HINTERNET hInternet = NULL;
  HINTERNET hConnect = NULL;
  HINTERNET hRequest = NULL;

  BOOL bRet = TRUE;  

  std::string strResponse;  

  hInternet = (HINSTANCE)InternetOpenA("User-Agent",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);  
  if(!hInternet)  
    goto Ret0;  

  hConnect = (HINSTANCE)InternetConnectA(hInternet,lpHostName,sPort,NULL,"HTTP/1.1",INTERNET_SERVICE_HTTP,0,0);  
  if(!hConnect)  
    goto Ret0;  

  hRequest = (HINSTANCE)HttpOpenRequestA(hConnect,lpMethod,lpUrl,"HTTP/1.1",NULL,NULL,INTERNET_FLAG_RELOAD,0);  
  if(!hRequest)  
    goto Ret0;  

  //bRet = HttpAddRequestHeaders(hRequest,"Content-Type: application/x-www-form-urlencoded",Len(FORMHEADERS),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);  
  //if(!bRet)  
  //goto Ret0;  
  DWORD a = GetLastError();
  bRet = HttpSendRequestA(hRequest,NULL,0,lpPostData,nPostDataLen);  
  a = GetLastError();
  char statu_code[MAX_PATH];
  DWORD statue_code_len;
  BOOL  bQuery=::HttpQueryInfoA(hRequest,
    HTTP_QUERY_STATUS_CODE,
    statu_code,
    &statue_code_len,
    NULL);

  while(TRUE)  
  {  
    char cReadBuffer[4096];  
    unsigned long lNumberOfBytesRead;  
    bRet = InternetReadFile(hRequest,cReadBuffer,sizeof(cReadBuffer) - 1,&lNumberOfBytesRead);  
    if(!bRet || !lNumberOfBytesRead)  
      break;  
    cReadBuffer[lNumberOfBytesRead] = 0;  
    strResponse = strResponse + cReadBuffer;  
  }  

Ret0:  
  if(hRequest)  
    InternetCloseHandle(hRequest);  
  if(hConnect)  
    InternetCloseHandle(hConnect);  
  if(hInternet)  
    InternetCloseHandle(hInternet);  

  return strResponse;  
}  
