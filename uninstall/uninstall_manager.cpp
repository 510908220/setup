#include "stdafx.h"
#include "uninstall_manager.h"
#include "uninstall_util.h"
#include "res_filename.h"
#include <windows.h>
#include <Shellapi.h>
#include <shlobj.h>
#include <winreg.h>
#include "lang_string.h"
#include "../setup/yye_files_list.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
#include "yygame/update/yg_update_define.h"

CUnInstallManager& CUnInstallManager::GetInstance()
{
  static CUnInstallManager instance;
  return instance;
}

BOOL CUnInstallManager::Init()
{
  DWORD dwLen = MAX_PATH;
  _TCHAR szValue[MAX_PATH + 1] = {0};
  long lResult = uninstall_util::GetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _TEXT("InstallDir"), REG_SZ, (LPBYTE)szValue, dwLen);
  if (ERROR_SUCCESS != lResult) 
    return FALSE;
  m_installPath = szValue;
  //
  lResult = uninstall_util::GetValueReg(HKEY_CURRENT_USER, _TEXT("SOFTWARE\\duowan\\yyesetup"), _TEXT("PromotionPlan"), REG_SZ, (LPBYTE)szValue, dwLen);
  if (ERROR_SUCCESS != lResult) 
    m_wstrPromotionPlan = L"";
  else
    m_wstrPromotionPlan = szValue;
  //
  m_bIsDeleteProfile = TRUE;
  return TRUE;
}

BOOL CUnInstallManager::UnInit()
{
  return TRUE;
}

std::wstring CUnInstallManager::GetPromotionPlan()
{
  return m_wstrPromotionPlan;
}

BOOL CUnInstallManager::FindAndDeleteFlyLnk(std::wstring wsDirectory)
{
  wstring wsDirectoryAllFiles = wsDirectory + L"*";
  WIN32_FIND_DATA fd;
  ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
  HANDLE hFindData = FindFirstFile(wsDirectoryAllFiles.c_str(), &fd);
  if (INVALID_HANDLE_VALUE == hFindData)
    return TRUE;

  BOOL bSucc;

  do 
  {
    if (wcsstr(fd.cFileName, L".lnk"))
    {
      std::wstring wsLnkPath = wsDirectory + fd.cFileName;
      std::wstring wsDstPath;
      uninstall_util::GetDesktopDstPath(fd.cFileName, wsDstPath);
      if (wcsstr(wsDstPath.c_str(), L"yy://yxdt-")
       && wcsstr(wsDstPath.c_str(), L"oid=20"))
      {
        bSucc &= DeleteFile(wsLnkPath.c_str());
      }
    }
  } while (FindNextFile(hFindData,&fd));

  FindClose(hFindData);
  hFindData = NULL;

  return bSucc;
}

BOOL CUnInstallManager::DeleteDesktopLnk()
{
  LPITEMIDLIST pIdl;
  if (NOERROR != SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &pIdl))
    return FALSE;

  TCHAR pwszDirectory[MAX_PATH + 1] = {0};
  if (!SHGetPathFromIDList(pIdl, pwszDirectory))
    return FALSE;

  wstring wsDestExeLnkPath = pwszDirectory;
  CoTaskMemFree(pIdl);

  wsDestExeLnkPath += L"\\YY浏览器.lnk";
  BOOL bRet = uninstall_util::DeleteFile(wsDestExeLnkPath.c_str());
  //YYBrowser Begin
  //删除网址导航的桌面快捷方式by zy. 2015-3-31 11:06.
  wstring wsDestURLLnkPath = pwszDirectory;
  wsDestURLLnkPath +=L"\\网址导航.lnk";
  bRet = uninstall_util::DeleteFile(wsDestURLLnkPath.c_str());
  //YYBrowser End
  wstring wsDesktopLnkDir = pwszDirectory;
  wsDesktopLnkDir += L"\\";
  return bRet & FindAndDeleteFlyLnk(wsDesktopLnkDir);
}

BOOL CUnInstallManager::DeleteQuickTaskBarLnk()
{
  if(uninstall_util::IsVistaSystem())//Win7
  {
    //从任务栏解锁
    std::wstring strTaskBarPath;
    TCHAR tmp[MAX_PATH]={0};
    if (SHGetSpecialFolderPath(NULL, tmp, CSIDL_APPDATA, TRUE))
    {
      strTaskBarPath = tmp;
      strTaskBarPath += _TEXT("\\Microsoft\\Internet Explorer\\Quick Launch\\User Pinned\\TaskBar");
      strTaskBarPath += L"\\YY浏览器.lnk";
      reinterpret_cast<int>(ShellExecute(NULL, L"taskbarunpin", strTaskBarPath.c_str(), NULL, NULL, SW_SHOW));
    }
    else
      return FALSE;
    return TRUE;
  }
  else//XP
  {
    //从快速启动栏删除
    LPITEMIDLIST pIdl;
    if (NOERROR != SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pIdl))
      return FALSE;

    TCHAR pwszDirectory[MAX_PATH + 1] = {0};
    if (!SHGetPathFromIDList(pIdl, pwszDirectory))
      return FALSE;

    wstring wsDestExeLnkPath = pwszDirectory;
    CoTaskMemFree(pIdl);

    wsDestExeLnkPath += L"\\Microsoft\\Internet Explorer\\Quick Launch\\YY浏览器.lnk";
    return uninstall_util::DeleteFile(wsDestExeLnkPath.c_str());
  }
}

BOOL CUnInstallManager::DeleteStartMenuLnk()
{
  LPITEMIDLIST pIdl;
  if (NOERROR != SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pIdl))
    return FALSE;

  TCHAR pwszDirectory[MAX_PATH + 1] = {0};
  if (!SHGetPathFromIDList(pIdl, pwszDirectory))
    return FALSE;

  wstring wsDestExeLnkPath = pwszDirectory;
  CoTaskMemFree(pIdl);

  wsDestExeLnkPath += L"\\多玩";
  wsDestExeLnkPath += L"\\YY浏览器";

  return uninstall_util::DeleteFolder(wsDestExeLnkPath.c_str());
}

DWORD WINAPI UnInstallThreadProc(LPVOID lpParam)
{
  HRESULT hr = CoInitialize(NULL);
  if (FAILED(hr))
    return 0;
  CUnInstallManager* helper = (CUnInstallManager*)lpParam;
  helper->DoUnInstallYYExplorer();
  CoUninitialize();
  return 0;
}

BOOL CUnInstallManager::DoUnInstallYYExplorer()
{
  // 重新检测一遍yy浏览器是否正在运行，并且强制关闭
  uninstall_util::CloseProcess(L"YYExplorer.exe");
  uninstall_util::CloseProcess(L"YYExplorerUplive.exe");//卸载执行杀掉升级程序

  // DeleteQuickTaskBarLnk()不能放后面，因为从任务栏脱离是用.exe右键菜单实现的，如果.exe都没了，这步就没办法执行了。
  DeleteQuickTaskBarLnk();
  
  m_pUnInstallNotify->OnProgress(USP_BACKUP, 100, 0);
  BackupVersion();
  m_pUnInstallNotify->OnProgress(USP_BACKUP, 100, 100);
  DeleteProfile();
  m_pUnInstallNotify->OnProgress(USP_DELETEFILE, 100, 50);
  DeleteVersion();
  m_pUnInstallNotify->OnProgress(USP_DELETEFILE, 100, 90);
  DeleteRegLastVer();
  m_pUnInstallNotify->OnProgress(USP_DELETEFILE, 100, 100);
  DeleteDesktopLnk();
  m_pUnInstallNotify->OnProgress(USP_DELETELNK, 100, 50);
  DeleteStartMenuLnk();
  m_pUnInstallNotify->OnProgress(USP_DELETELNK, 100, 100);
  ResetDefautBrowser();
  m_pUnInstallNotify->OnProgress(USP_RESETDEFAULT, 100, 100);
  DeleteUninstallList();
  m_pUnInstallNotify->OnProgress(USP_DELETEUNINSTINFO, 100, 100);
  return TRUE;
}

void CUnInstallManager::ReportMidMd5()
{
  std::string mid = ygdata_report::GetMachineIdTimeout(10 * 1000);
  std::string mid_key(mid);
  mid_key.append(MIDMD5_REPORT_KEY);
  std::string str_auto;
  uninstall_util::md5(mid_key.c_str(), mid_key.length(), str_auto);
  char dst_url[MAX_PATH];
  sprintf(dst_url, MIDMD5_REPORT_URL_MODE, mid.c_str(), str_auto.c_str());
  //ShellExecuteA(NULL,"open", dst_url, NULL, NULL, SW_HIDE ); 
  uninstall_util::HttpRequest("policy.game.yy.com", 80, dst_url, "GET", NULL, 0);  
}

BOOL CUnInstallManager::UnInstallYYExplorer(IUnInstallNotify* notify)
{
  m_pUnInstallNotify = notify;
  ReportMidMd5();
  m_unInstallHandle = CreateThread(NULL, 0, UnInstallThreadProc, (LPVOID)this, 0, NULL);
  
  return TRUE;
}

bool CUnInstallManager::MovePath(const std::wstring wsTmpSrcPath, const std::wstring wsTmpDesPath)
{
  bool bSucc = true;
  std::wstring wsTmpDesDir;
  if (!uninstall_util::PathExists(wsTmpSrcPath))
  {//无效路径
  }
  else if(!uninstall_util::IsFolder(wsTmpSrcPath))
  {//不是文件夹
    wsTmpDesDir = wsTmpDesPath.substr(0, wsTmpDesPath.rfind(L'\\'));
    uninstall_util::MakeSureDirectoryPathExists(wsTmpDesDir.c_str());
    bSucc = MoveFileEx(wsTmpSrcPath.c_str(), wsTmpDesPath.c_str(), MOVEFILE_REPLACE_EXISTING) && bSucc;
  }
  else if (uninstall_util::IsFolderEmpty(wsTmpSrcPath.c_str()))
  {//空文件夹
    bSucc = uninstall_util::DeleteFolder(wsTmpSrcPath.c_str()) && bSucc;
  }
  else
  {//非空文件夹
  }
  return bSucc;
}

BOOL CUnInstallManager::MoveInstallerFile(std::wstring wsInstallerFilePath,std::wstring wsDstPath)
{
  if (!uninstall_util::MakeSureDirectoryPathExists(wsDstPath.c_str()))
    return FALSE;

  bool bSucc = true;
  std::wstring fileOrDirName;
  std::wstring wsTmpSrcPath;
  std::wstring wsTmpDesPath;

  //先处理特殊情况
  if(m_bIsDeleteProfile)
    bSucc = MovePath(wsInstallerFilePath + L"..\\First Run", wsDstPath + L"..\\First Run") && bSucc;
  //遍历yye_files_list_common
  for (int i = 0; yye_files_list_common[i] != NULL; ++i)
  {
    fileOrDirName = yye_files_list_common[i];
    wsTmpSrcPath = wsInstallerFilePath + fileOrDirName;
    wsTmpDesPath = wsDstPath + fileOrDirName;
    bSucc = MovePath(wsTmpSrcPath, wsTmpDesPath) && bSucc;//是短路操作，所以要把bSucc放最后
  }
  //遍历yye_files_list_extra_uninstall
  for (int i = 0; yye_files_list_extra_uninstall[i] != NULL; ++i)
  {
    fileOrDirName = yye_files_list_extra_uninstall[i];
    wsTmpSrcPath = wsInstallerFilePath + fileOrDirName;
    wsTmpDesPath = wsDstPath + fileOrDirName;
    bSucc = MovePath(wsTmpSrcPath, wsTmpDesPath) && bSucc;
  }

  return bSucc;
}

BOOL CUnInstallManager::MoveVersionDir(std::wstring wsSrcPath,std::wstring wsDstPath,bool bOnlyInstallerFile)
{
  if(bOnlyInstallerFile)
    return MoveInstallerFile(wsSrcPath, wsDstPath);

  if (!uninstall_util::MakeSureDirectoryPathExists(wsDstPath.c_str()))
    return FALSE;

  std::wstring wsFilePath = wsSrcPath + L"*";
  WIN32_FIND_DATA fd;
  ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
  HANDLE hFindData = FindFirstFile(wsFilePath.c_str(), &fd);
  if (INVALID_HANDLE_VALUE == hFindData)
    return TRUE;

  BOOL bSucc = TRUE;
  do 
  {
    std::wstring wsTmpDesPath = wsDstPath + fd.cFileName;
    std::wstring wsTmpSrcPath = wsSrcPath + fd.cFileName;
    //只遍历一层
    bSucc &= MoveFileEx(wsTmpSrcPath.c_str(), wsTmpDesPath.c_str(), MOVEFILE_REPLACE_EXISTING);
  } while ((bSucc) && FindNextFile(hFindData,&fd));

  FindClose(hFindData);
  hFindData = NULL;

  return bSucc;
}

#define CMD_LINE_SIZE 1024
BOOL InstallScheduledTask(const wchar_t* wszVersionDir)
{
  BOOL bRet = FALSE;
  BOOL bResult = FALSE;
  DWORD dwWaitResult = 0;
  DWORD dwExitCode = 0;
  HANDLE hProcess = NULL;
  wchar_t wszTriggerPath[MAX_PATH];
  wchar_t wszCmdline[CMD_LINE_SIZE];
  STARTUPINFOW StartUpInfo = {0};
  PROCESS_INFORMATION ProcessInfo = {0};

  _snwprintf(wszTriggerPath, MAX_PATH, L"%s\\%S", wszVersionDir, YYE_UPLIVE_TRIGGER_EXE);
  wszTriggerPath[MAX_PATH - 1] = '\0';

  if (!uninstall_util::PathExists(wszTriggerPath))
    return TRUE;

  StartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
  StartUpInfo.wShowWindow = SW_HIDE;

  _snwprintf(wszCmdline, CMD_LINE_SIZE, L"\"%s\" --%S", wszTriggerPath, kSwitchKeyInstallTask);
  wszCmdline[CMD_LINE_SIZE - 1] = '\0';

  bRet = CreateProcessW(NULL, wszCmdline, NULL, NULL, FALSE, 
    0, NULL, NULL, &StartUpInfo, &ProcessInfo);
  YG_PROCESS_ERROR(bRet);

  dwWaitResult = WaitForSingleObject(ProcessInfo.hProcess, 1000 * 10);
  YG_PROCESS_ERROR(dwWaitResult == WAIT_OBJECT_0);

  bRet = GetExitCodeProcess(ProcessInfo.hProcess, &dwExitCode);
  YG_PROCESS_ERROR(bRet);
  YG_PROCESS_ERROR(dwExitCode == 0);

  bResult = TRUE;
Exit0:
  if (ProcessInfo.hThread) {
    CloseHandle(ProcessInfo.hThread);
    ProcessInfo.hThread = NULL;
  }
  if (ProcessInfo.hProcess) {
    CloseHandle(ProcessInfo.hProcess);
    ProcessInfo.hProcess = NULL;
  }
  return bRet;
}

BOOL UninstallScheduledTask(const wchar_t* wszInstallDir, const wchar_t* wszOldVersion) {
  BOOL bRet = FALSE;
  BOOL bResult = FALSE;
  DWORD dwWaitResult = 0;
  DWORD dwExitCode = 0;
  HANDLE hProcess = NULL;
  wchar_t wszTriggerPath[MAX_PATH];
  wchar_t wszTriggerInRootDir[MAX_PATH];
  wchar_t wszCmdline[CMD_LINE_SIZE];
  STARTUPINFOW StartUpInfo = {0};
  PROCESS_INFORMATION ProcessInfo = {0};

  _snwprintf(wszTriggerPath, MAX_PATH, L"%s\\%s\\%S", wszInstallDir, wszOldVersion, YYE_UPLIVE_TRIGGER_EXE);
  wszTriggerPath[MAX_PATH - 1] = '\0';

  if (!uninstall_util::PathExists(wszTriggerPath)) {
    _snwprintf(wszTriggerInRootDir, MAX_PATH, L"%s\\%S", wszInstallDir, YYE_UPLIVE_TRIGGER_EXE);
    wszTriggerInRootDir[MAX_PATH - 1] = '\0';

    if (uninstall_util::PathExists(wszTriggerInRootDir))
      CopyFile(wszTriggerInRootDir, wszTriggerPath, TRUE);
  }

  if (!uninstall_util::PathExists(wszTriggerPath))
    return TRUE; // 对于旧版本，Trigger.exe就是不存在

  StartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
  StartUpInfo.wShowWindow = SW_HIDE;

  _snwprintf(wszCmdline, CMD_LINE_SIZE, L"\"%s\" --%S", wszTriggerPath, kSwitchKeyUninstallTask);
  wszCmdline[CMD_LINE_SIZE - 1] = '\0';

  bRet = CreateProcessW(NULL, wszCmdline, NULL, NULL, FALSE, 
    0, NULL, NULL, &StartUpInfo, &ProcessInfo);
  YG_PROCESS_ERROR(bRet);

  dwWaitResult = WaitForSingleObject(ProcessInfo.hProcess, 1000 * 10);
  YG_PROCESS_ERROR(dwWaitResult == WAIT_OBJECT_0);

  bRet = GetExitCodeProcess(ProcessInfo.hProcess, &dwExitCode);
  YG_PROCESS_ERROR(bRet);
  YG_PROCESS_ERROR(dwExitCode == 0);

  bResult = TRUE;
Exit0:
  if (ProcessInfo.hThread) {
    CloseHandle(ProcessInfo.hThread);
    ProcessInfo.hThread = NULL;
  }
  if (ProcessInfo.hProcess) {
    CloseHandle(ProcessInfo.hProcess);
    ProcessInfo.hProcess = NULL;
  }
  return bRet;
}

BOOL CUnInstallManager::BackupVersion()
{
  if (m_installPath.empty())
    return FALSE;

  if(!uninstall_util::PathExists(m_installPath.c_str()))
    return FALSE;

  std::wstring wsDstPath = m_installPath + L"yyinstall_tmp_\\" + SETUP_VERSION + L"\\";
  std::wstring wsCurVersionDir = m_installPath + SETUP_VERSION + L"\\";

  BOOL ret_code = UninstallScheduledTask(m_installPath.c_str(), SETUP_VERSION);
  ygdata_report::SendEventWithSrc(YE_UNINSTALL_PREFIX UNINSTALL_SCHEDULED_TASK, ret_code ? "Success" : "Fail");
  if (!ret_code)
    return FALSE;

  if (!MoveVersionDir(wsCurVersionDir,wsDstPath)) {
    RollbackVersion(TRUE);
    InstallScheduledTask((m_installPath + SETUP_VERSION).c_str());
    return FALSE;
  }

  return TRUE;
}

BOOL CUnInstallManager::RollbackVersion(bool needRollBack)
{
  if(!needRollBack)
    return TRUE;

  std::wstring wsSrcPath = m_installPath + L"yyinstall_tmp_\\";
  std::wstring wsDstPath = m_installPath;

  if(!MoveVersionDir(wsSrcPath,wsDstPath,false))
    return FALSE;

  return TRUE;
}

const wchar_t* kFileAssociations[] = {L".htm", L".html", L".shtml",
L".xht", L".xhtml", NULL};
const wchar_t* kBrowserProtocolAssociations[] = {L"ftp", L"http",
L"https", NULL};
const wchar_t* kBrowserProtocolAssociationsIE[] = {L"IE.FTP", L"IE.HTTP",
L"IE.HTTPS", NULL};

BOOL CUnInstallManager::ResetDefautBrowser()
{
  DeleteYYExplorerProgId();

  BOOL bIsYYExplorerDefault = TRUE;
  ResetDefautBrowserXp(bIsYYExplorerDefault);

  if (bIsYYExplorerDefault)
    ResetDefautBrowserVista();

  SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
  return TRUE;
}

BOOL CUnInstallManager::ResetDefautBrowserXp(BOOL& bIsYYExplorerDefault)
{
  for (int i = 0; kFileAssociations[i] != NULL; i++)
    bIsYYExplorerDefault = ReSetDefaultBrowserFileKeyXp(kFileAssociations[i]) && bIsYYExplorerDefault;

  for (int i = 0; kBrowserProtocolAssociations[i] != NULL; i++)
    ReSetDefaultBrowserProtocalKeyXp(kBrowserProtocolAssociations[i]);

  return TRUE;
}

BOOL CUnInstallManager::ResetDefautBrowserVista()
{
  const wchar_t* kFileAssociations[] = {L".htm", L".html", L".shtml",
    L".xht", L".xhtml", NULL};
  const wchar_t* kBrowserProtocolAssociations[] = {L"ftp", L"http",
    L"https", NULL};

  IApplicationAssociationRegistration* pAAR = NULL;
  HRESULT hr = ::CoCreateInstance(CLSID_ApplicationAssociationRegistration, NULL, CLSCTX_INPROC, __uuidof(IApplicationAssociationRegistration), reinterpret_cast<void**>(&pAAR));
  if (!uninstall_util::IsVistaSystem() || !SUCCEEDED(hr))
    return FALSE;

  ReSetCapabilitiesStartmenuVista();
  ReSetRegisteredApplicationsVista();

  for (int i = 0; kFileAssociations[i] != NULL; i++)
  {
    pAAR->SetAppAsDefault(L"Internet Explorer", kFileAssociations[i], AT_FILEEXTENSION);
  }

  for (int i = 0; kBrowserProtocolAssociations[i] != NULL; i++)
  {
    pAAR->SetAppAsDefault(L"Internet Explorer", kBrowserProtocolAssociations[i], AT_URLPROTOCOL);
  }

  return TRUE;
}

BOOL CUnInstallManager::DeleteVersion()
{
  if (m_installPath.empty())
    return FALSE;

  std::wstring wsTmpPath = m_installPath + L"yyinstall_tmp_";

  uninstall_util::DeleteFolder(wsTmpPath.c_str());
  return TRUE;
}

BOOL CUnInstallManager::DeleteRegLastVer()
{
  HKEY hKey,hFatherKey;
  if (RegOpenKeyEx(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    return FALSE;
  if (RegOpenKeyEx(HKEY_CURRENT_USER, YYEXPLORER_FATHER_REG_PATH, 0, KEY_ALL_ACCESS, &hFatherKey) != ERROR_SUCCESS)
    return FALSE;
  //TCHAR szQPlusOldVersion[MAX_PATH + 1] = {0};
  //LONG lRtn = uninstall_util::GetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("LastVer") ,REG_SZ, (LPBYTE)szQPlusOldVersion, MAX_PATH);
  BOOL bRet = RegDeleteValue(hKey, _T("LastVer"));
  bRet |= RegDeleteValue(hKey, _T("InstallDir"));
  bRet |= RegDeleteValue(hKey, NULL);
  bRet |= RegDeleteValue(hKey, _T("lastrun"));
  bRet |= RegDeleteValue(hKey, _T("InstallType"));
  bRet |= RegDeleteValue(hKey, _T("WhoCreateDesktopLnk"));
  bRet |= RegDeleteKey(hFatherKey,L"YYExplorer");
  //bRet |= uninstall_util::SetValueReg(HKEY_CURRENT_USER, GAMEPROXY_REG_PATH, _T("LaunchProduct") ,REG_SZ, (BYTE*) STR_YYGAME, _tcslen(STR_YYGAME) * sizeof(_TCHAR));
  return (ERROR_SUCCESS == bRet);
}

BOOL CUnInstallManager::SetDeleteProfile(BOOL bCreate)
{
  m_bIsDeleteProfile = bCreate;
  return TRUE;
}

BOOL CUnInstallManager::DeleteUninstFileSelf()
{
  return uninstall_util::SelfDel();
}

BOOL CUnInstallManager::ReSetDefaultBrowserProtocalKeyXp(LPCTSTR lpszKey)
{
  HKEY hKey, hSubKey, hSubKey2, hSubKey3;
  if (RegOpenKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) != ERROR_SUCCESS)
    return FALSE;

  DWORD dwRes = 0;
  RegCreateKeyEx(hKey, _T("shell"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey, &dwRes);
  RegCreateKeyEx(hSubKey, _T("open"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey2, &dwRes);
  RegCreateKeyEx(hSubKey2, _T("command"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey3, &dwRes);

  TCHAR lpszOldModule[_MAX_PATH + 1] = {0};
  DWORD nBytes = _MAX_PATH;
  DWORD dwType = REG_SZ;
  TCHAR lpszModule[_MAX_PATH + 1] = {0};
  RegQueryValueEx(hSubKey3, _T(""), NULL, &dwType, (LPBYTE)(lpszOldModule), &nBytes);

  _snwprintf_s(lpszModule, MAX_PATH, _TRUNCATE, L"\"%sYYExplorer.exe\" -- \"%%1\"", m_installPath.c_str());
  if (wcscmp(lpszOldModule, lpszModule) == 0)
  {
    wstring ieModule = GetBrowserProtocalKeyXp(L"htmlfile");
    RegSetValueEx(hSubKey3, _T(""), NULL, REG_SZ, (LPBYTE)(ieModule.c_str()), ((int)ieModule.length() + 1) * sizeof(TCHAR));
  }
  RegCloseKey(hSubKey3);
  RegCloseKey(hSubKey2);
  RegCloseKey(hSubKey);
  RegCloseKey(hKey);
  return TRUE;
}

wstring CUnInstallManager::GetBrowserProtocalKeyXp(LPCTSTR lpszKey)
{
  HKEY hKey, hSubKey, hSubKey2, hSubKey3;
  if (RegOpenKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) != ERROR_SUCCESS)
    return L"";

  DWORD dwRes = 0;
  RegCreateKeyEx(hKey, _T("shell"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey, &dwRes);
  RegCreateKeyEx(hSubKey, _T("open"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey2, &dwRes);
  RegCreateKeyEx(hSubKey2, _T("command"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey3, &dwRes);

  TCHAR lpszModule[_MAX_PATH + 1] = {0};
  DWORD nBytes = _MAX_PATH;
  DWORD dwType = REG_SZ;
  RegQueryValueEx(hSubKey3, _T(""), NULL, &dwType, (LPBYTE)(lpszModule), &nBytes);

  RegCloseKey(hSubKey3);
  RegCloseKey(hSubKey2);
  RegCloseKey(hSubKey);
  RegCloseKey(hKey);
  return lpszModule;
}

BOOL CUnInstallManager::ReSetDefaultBrowserFileKeyXp(LPCTSTR lpszKey)
{
  HKEY hKey;
  if (RegOpenKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) != ERROR_SUCCESS)
    return FALSE;

  TCHAR lpszOldValue[_MAX_PATH + 1] = {0};
  DWORD nBytes = _MAX_PATH;
  DWORD dwType = REG_SZ;
  if (RegQueryValueEx(hKey, _T(""), NULL, &dwType, (LPBYTE)lpszOldValue, &nBytes) != ERROR_SUCCESS)
    return FALSE;

  if (wcsstr(lpszOldValue, L"YYExplorerHTML") == 0)
    return FALSE;

  RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)(L"htmlfile"), ((int)_tcsclen(L"htmlfile") + 1) * sizeof(TCHAR));
  RegCloseKey(hKey);
  return TRUE;
}

BOOL CUnInstallManager::DeleteYYExplorerProgId()
{
  HKEY hKey;
  if (RegOpenKeyEx(HKEY_CLASSES_ROOT, L"", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    return FALSE;
  
  DWORD dwRes = 0;
  SHDeleteKey(hKey, _T("YYExplorerHTML"));
  RegCloseKey(hKey);
  return TRUE;
}

BOOL CUnInstallManager::DeleteProfile()
{
  if(!m_bIsDeleteProfile)
    return TRUE;
  //获取 appdata 目录中的profile
  _TCHAR appdataDir[_MAX_PATH];  
  SHGetSpecialFolderPathW(NULL,appdataDir,CSIDL_LOCAL_APPDATA,0);
  wstring wstrProfileDir = appdataDir;
  wstrProfileDir = wstrProfileDir + _T("\\YYExplorer");
  //删除文件夹及其内容
  return uninstall_util::DeleteFolder(wstrProfileDir.c_str());
}

BOOL CUnInstallManager::ReSetRegisteredApplicationsVista()
{
  TCHAR lpszWholeKey[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszWholeKey, MAX_PATH, _TRUNCATE, L"Software\\RegisteredApplications");
  HKEY hKey;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszWholeKey, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    return FALSE;

  SHDeleteKey(hKey, _T("YYExplorer"));
  RegCloseKey(hKey);
  return TRUE;
}

BOOL CUnInstallManager::ReSetCapabilitiesStartmenuVista()
{
  TCHAR lpszWholeKey[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszWholeKey, MAX_PATH, _TRUNCATE, L"Software\\Clients\\StartMenuInternet");
  HKEY hKey;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszWholeKey, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    return FALSE;

  SHDeleteKey(hKey, _T("YYExplorer"));

  RegCloseKey(hKey);
  return TRUE;
}

BOOL CUnInstallManager::DeleteUninstallList()
{
  BOOL bRet = FALSE;
  //注册表操作部分
  TCHAR lpszWholeKey[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszWholeKey, MAX_PATH, _TRUNCATE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
  HKEY hKey = 0;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszWholeKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)//XP
    bRet = TRUE;
  RegDeleteKey(hKey, L"YYExplorer");

  if (RegOpenKeyEx(HKEY_CURRENT_USER, lpszWholeKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)//Win7
    bRet = TRUE;
  RegDeleteKey(hKey, L"YYExplorer");

  RegCloseKey(hKey);
  ::SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, NULL, NULL, SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 2000, NULL);
  return bRet;
}
