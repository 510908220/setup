#include "stdafx.h"
#include "install_manager.h"
#include "install_util.h"
#include "message_dialog.h"
#include "lang_string.h"
#include "resource.h"
#include "res_filename.h"
#include "install_tmpdir.h"
#include <map>
#include <ShlObj.h>
#include <shellapi.h>
#include <winuser.h>
#include <winbase.h>
#include <tlhelp32.h>
#include <wtsapi32.h>
#include <windows.h> 
#include <WinSafer.h> 
#include <comdef.h>
#include <comutil.h>
//Include Task header files - Included in Windows Vista Beta-2 SDK from MSDN
#include <taskschd.h>
#include <conio.h>
#include <iostream>
#include <time.h>
#include "image_pre_reader_win.h"
#include "yye_files_list.h"
#include "yygame/update/yg_update_define.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
//-----------������Ȩ���-----begin
#include <sdkddkver.h>
#define _WIN32_WINNT 0x0501 // XP and above
typedef NTSTATUS (NTAPI *NtQueryInformationProcessFn)( HANDLE process, ULONG infoClass, void* data,
  ULONG dataSize, ULONG* outSize );
static NtQueryInformationProcessFn NtQueryInformationProcess;
typedef NTSTATUS (NTAPI *NtSetInformationProcessFn)( HANDLE process, ULONG infoClass, void* data,
  ULONG dataSize );
static NtSetInformationProcessFn NtSetInformationProcess;
// these values determined by poking around in the debugger - use at your own risk!
const DWORD ProcessInformationMemoryPriority = 0x27;
const DWORD ProcessInformationIoPriority = 0x21;
const DWORD DefaultMemoryPriority = 5;
const DWORD LowMemoryPriority = 3;
const DWORD DefaultIoPriority = 2;
const DWORD LowIoPriority = 1;
//-----------������Ȩ���------end

// app id �����������������kAppListAppName��������һ��
#define YE_APP_ID (L"YYExplorer")

using namespace std;
# pragma comment(lib, "taskschd.lib")

CInstallManager& CInstallManager::GetInstance()
{
  static CInstallManager instance;
  return instance;
}

DWORD WINAPI UnZipThreadProc0(LPVOID lpParam)
{
  HRESULT hr = CoInitialize(NULL);
  if (FAILED(hr))
    return 0;
  CInstallManager* helper = (CInstallManager*)lpParam;
  helper->UnZipLevel0();
  CoUninitialize();
  return 0;
}

DWORD WINAPI UnZipThreadProc0_1(LPVOID lpParam)
{
  HRESULT hr = CoInitialize(NULL);
  if (FAILED(hr))
    return 0;
  CInstallManager* helper = (CInstallManager*)lpParam;
  helper->UnZipLevel0_1();
  CoUninitialize();
  return 0;
}

DWORD WINAPI UnZipThreadProc0_2(LPVOID lpParam)
{
  HRESULT hr = CoInitialize(NULL);
  if (FAILED(hr))
    return 0;
  CInstallManager* helper = (CInstallManager*)lpParam;
  helper->UnZipLevel0_2();
  CoUninitialize();
  return 0;
}

BOOL CInstallManager::UnZipLevel0()
{
  wchar_t tmpPath[MAX_PATH] = {0};
  GetTempPathW(MAX_PATH, tmpPath);
  wchar_t unZipPath[MAX_PATH + 1] = {0};
  swprintf_s(unZipPath, MAX_PATH, L"%s\\%S", tmpPath, STR_PRODUCT_VERSION);
  install_util::MakeSureDirectoryPathExists(unZipPath);
  UncompressHelper setupUncompress;
  setupUncompress.UnUncompress7zToDiskSync(_Module.GetModuleInstance(), IDR_MSI_SETUP, unZipPath, NULL);
  return TRUE;
}

BOOL CInstallManager::UnZipLevel0_1()
{
  wchar_t tmpPath[MAX_PATH] = {0};
  GetTempPathW(MAX_PATH, tmpPath);
  wchar_t unZipPath[MAX_PATH + 1] = {0};
  swprintf_s(unZipPath, MAX_PATH, L"%s\\%S\\%S\\chrome.7z", tmpPath, STR_PRODUCT_VERSION, STR_PRODUCT_VERSION);
  wchar_t tmpUnZipPath[MAX_PATH + 1] = {0};
  swprintf_s(tmpUnZipPath, MAX_PATH, L"%s\\%S", m_installPath.c_str(), STR_PRODUCT_VERSION);
  UncompressHelper setupUncompress;
  setupUncompress.UnUncompress7zToDiskSync(unZipPath, tmpUnZipPath, NULL);
  install_util::DeleteFile(unZipPath);
  return TRUE;
}

BOOL CInstallManager::UnZipLevel0_2()
{
  wchar_t tmpPath[MAX_PATH] = {0};
  GetTempPathW(MAX_PATH, tmpPath);
  wchar_t unZipPath[MAX_PATH + 1] = {0};
  swprintf_s(unZipPath, MAX_PATH, L"%s\\%S\\%S\\other.7z", tmpPath, STR_PRODUCT_VERSION, STR_PRODUCT_VERSION);
  wchar_t tmpUnZipPath[MAX_PATH + 1] = {0};
  swprintf_s(tmpUnZipPath, MAX_PATH, L"%s\\%S", m_installPath.c_str(), STR_PRODUCT_VERSION);
  UncompressHelper setupUncompress;
  setupUncompress.UnUncompress7zToDiskSync(unZipPath, tmpUnZipPath, NULL);
  install_util::DeleteFile(unZipPath);
  return TRUE;
}

BOOL CInstallManager::Init(bool isOnlyRegister)
{
  m_bAutoStart = TRUE;
  m_pInstallNotify = NULL;
  m_bIsCreateDesktopLnk = TRUE;
  m_bIsCreateStartupLnk = TRUE;
  m_enumCreateStartMenuLnk = CSMS_TRUE;
  m_bIsCreateUnistallList = TRUE;
  m_bYYIsDefaultBrowser = TRUE;
  m_bIsYYExplorerRunning = FALSE;
  m_oldVersion = L"";
  m_wstrRunYYEUrl = GetDefaultRunYYEUrl();

  // �ڡ���ע��������������£�����Ҫ��ѹ�ļ���TempĿ¼
  if (false == isOnlyRegister)  {
	  m_unZipHandle0 = CreateThread(NULL, 0, UnZipThreadProc0, (LPVOID)this, 0, NULL);
  }
 
  /*BOOL bRet  = IsInstallOlder();
  if (bRet)
    bRet = ((IDYES == MessageDlg::MessageBox(NULL, ALREADY_LARTEST_VERSION, UI_COMMON_UI_TITLE, MB_YESNO + MB_ICONINFORMATION)) ? FALSE : TRUE);
  return !bRet;*/
  DWORD val = 0;
  install_util::GetValueReg(HKEY_CURRENT_USER, L"SOFTWARE\\duowan\\yyesetup", _T("dbgOutputReport") ,REG_DWORD, (BYTE*)&val, sizeof(DWORD));
  m_bCanOptDbg = (val == 1);

  return TRUE;
}

BOOL CInstallManager::UnInit()
{
  return TRUE;
}

wstring CInstallManager::GetInitInstallPath()
{
  std::wstring strPathTemp = GetLastInstallPath();
  if (strPathTemp.empty())
  {
    strPathTemp = GetDefaultInstallPath();
  }
  return strPathTemp;
}

wstring CInstallManager::GetDefaultInstallPath()
{
  std::wstring strPathTemp;
  TCHAR tmp[MAX_PATH]={0};
  if (SHGetSpecialFolderPath(NULL, tmp, install_util::IsVistaSystem() ? CSIDL_APPDATA : CSIDL_PROGRAM_FILES, TRUE))
  {
    strPathTemp = tmp;
    strPathTemp += _TEXT("\\duowan\\YYExplorer");
  }
  return strPathTemp;
}

wstring CInstallManager::GetLastInstallPath()
{
  std::wstring strPathTemp;
  DWORD dwLen = MAX_PATH;
  _TCHAR szValue[MAX_PATH + 1] = {0};
  long lResult = install_util::GetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _TEXT("InstallDir"), REG_SZ, (LPBYTE)szValue, dwLen);
  if (ERROR_SUCCESS != lResult) 
    return strPathTemp;
  strPathTemp = szValue;
  return strPathTemp;
}

std::wstring CInstallManager::GetDefaultRunYYEUrl()
{
  //return L"http://ie.yy.com/s/desc/index.html";
  return L"";
}

void CInstallManager::SetAutoStartFlag(BOOL bAutoStart)
{
  m_bAutoStart = bAutoStart;
}

void CInstallManager::SetCreateDesktopLnk(BOOL bCreate)
{
  m_bIsCreateDesktopLnk = bCreate;
}

void CInstallManager::SetCreateStartupLnk(BOOL bCreate)
{
  m_bIsCreateStartupLnk = bCreate;
}

void CInstallManager::SetCreateStartMenuLnk(CreateStartupMenuLinkStatu enumCreate)
{
  m_enumCreateStartMenuLnk = enumCreate;
}

void CInstallManager::SetCreateUninstallList(BOOL bCreate)
{
  m_bIsCreateUnistallList = bCreate;
}

void CInstallManager::SetYYIsDefauleBrowser(BOOL bDefault)
{
  m_bYYIsDefaultBrowser = bDefault;
}

void CInstallManager::SetRunYYEUrl(std::wstring url)
{
  m_wstrRunYYEUrl = url + L' ' + GetDefaultRunYYEUrl();
}

void CInstallManager::SetIsYYExplorerRunning(BOOL bRunning)
{
  m_bIsYYExplorerRunning = bRunning;
}

void CInstallManager::OutputDebugStrW( std::wstring str )
{
  if(!m_bCanOptDbg)
    return;
  OutputDebugStringW(str.c_str());
}

void CInstallManager::OutputDebugStrA( std::string str )
{
  if(!m_bCanOptDbg)
    return;
  OutputDebugStringA(str.c_str());
}

BOOL CInstallManager::IsInstallOlder()
{
  TCHAR szQPlusOldVersion[MAX_PATH + 1] = {0};
  LONG lRtn = install_util::GetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("LastVer") ,REG_SZ, (LPBYTE)szQPlusOldVersion, MAX_PATH);

  if (ERROR_SUCCESS != lRtn)
    return FALSE;

  return install_util::CompareVersionString(szQPlusOldVersion, SETUP_VERSION) > 0;
}

DWORD WINAPI InstallThreadProc(LPVOID lpParam)
{
  HRESULT hr = CoInitialize(NULL);
  if (FAILED(hr))
    return 0;
  CInstallManager* helper = (CInstallManager*)lpParam;
  helper->DoInstallYYExplorer();
  CoUninitialize();
  return 0;
}

BOOL CInstallManager::InstallYYExplorer(IInstallNotify* notify)
{
  m_pInstallNotify = notify;
  m_installHandle = CreateThread(NULL, 0, InstallThreadProc, (LPVOID)this, 0, NULL);
  return TRUE;
}

BOOL CInstallManager::UnRegisterNotify(IInstallNotify* notify)
{
  if (m_pInstallNotify == notify)
    m_pInstallNotify = NULL;
  return TRUE;
}

BOOL CInstallManager::LaunchYYExplorer(bool isOnlyRegister/* = false*/)
{
  // �����ע������������������Ĭ��������ѳɹ���ѹ���˱���
  // ��˲�����Ҫ�ȴ���ѹ���
  if (false == isOnlyRegister)
  {
	WaitForUnzipComplete();
  }
 
  std::vector<std::wstring> m_prefetchFiles;
  wchar_t system_buffer[MAX_PATH];
  system_buffer[0] = 0;
  GetWindowsDirectory(system_buffer, MAX_PATH);
  wchar_t prefetch_buffer[MAX_PATH];
  prefetch_buffer[0] = 0;
  swprintf_s(prefetch_buffer, MAX_PATH, L"%s\\Prefetch\\*.*", system_buffer);
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  hFind = FindFirstFile(prefetch_buffer, &FindFileData);
  if (hFind != INVALID_HANDLE_VALUE) 
  {
    while (true)
    {
       if (FindFileData.cFileName[0] != _T('.'))
       {
          if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
          {
            if (wcsstr(FindFileData.cFileName, L"YYEXPLORER") || wcsstr(FindFileData.cFileName, L"NTOSBOOT"))
            {
              wchar_t szFind[MAX_PATH];
              _tcscpy_s(szFind, MAX_PATH, system_buffer);    
              _tcscat_s(szFind, _T("\\Prefetch\\"));    
              _tcscat_s(szFind, FindFileData.cFileName);
              m_prefetchFiles.push_back(szFind);
            }
          }
       }
       if (!::FindNextFile(hFind, &FindFileData))
         break;
    }
    
    FindClose(hFind);
  }

  for (std::vector<std::wstring>::iterator it = m_prefetchFiles.begin(); it != m_prefetchFiles.end(); ++it)
  {
    install_util::DeleteFile(it->c_str());
  }
  
  CreateStartupLnk2();

  if(m_bAutoStart)
  	RunExe();

  return TRUE;
}

BOOL CInstallManager::WaitForUnzipComplete()
{
  WaitForSingleObject(m_unZipHandle1, 30 * 1000);
  CloseHandle(m_unZipHandle1);
  WaitForSingleObject(m_unZipHandle2, 30 * 1000);
  CloseHandle(m_unZipHandle2);

  return TRUE;
}

BOOL CInstallManager::CreateStartupLnk2()
{
  if (m_bIsCreateStartupLnk)
  {
    wstring wsDestTmpExeLnkPath = CInstallTmpDir::GetInstance().GetTmpDir();
    wsDestTmpExeLnkPath += L"\\YY�����.lnk";

    reinterpret_cast<int>(ShellExecute(NULL, L"taskbarpin", wsDestTmpExeLnkPath.c_str(), NULL, NULL, SW_SHOW));
    return TRUE;
  }
  return TRUE;
}

BOOL CInstallManager::RunExe() 
{
  std::wstring wstrRunYYECmdLine(L"--run-source-type=YESetup --bring-to-top ");
  wstrRunYYECmdLine.append(m_wstrRunYYEUrl);
  
  TCHAR lpszModule[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszModule, MAX_PATH, _TRUNCATE, L"%s\\%S\\chrome.dll", m_installPath.c_str(), STR_PRODUCT_VERSION);
  ImagePreReader::PartialPreReadImage(lpszModule, 100, 1024 * 1014);
  
  _snwprintf_s(lpszModule, MAX_PATH, _TRUNCATE, L"%sYYExplorer.exe", m_installPath.c_str());
  OutputDebugStrW(L"��װ��RunYYE");
#ifdef UAC_SETUP
  if (install_util::IsVistaSystem())
  {
    if (RunAsDesktopUser(lpszModule, wstrRunYYECmdLine.c_str()))
    {
      return TRUE;
    }
  }
#endif
  SHELLEXECUTEINFO ShExecInfo = {0};
  ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
  ShExecInfo.fMask = 0;
  ShExecInfo.hwnd = NULL;
  ShExecInfo.lpVerb = NULL;
  
  ShExecInfo.lpFile = lpszModule;
  // ����ͳ����Ҫ֪����������Դ
  ShExecInfo.lpParameters = wstrRunYYECmdLine.c_str();
  ShExecInfo.lpDirectory = NULL;
  ShExecInfo.nShow = SW_SHOW;
  ShExecInfo.hInstApp = NULL;
  ShellExecuteEx(&ShExecInfo);

  return TRUE;
}

#define CLEANUP \
  pRootFolder->Release();\
  pTask->Release();\

HRESULT CreateMyTask(LPCWSTR wszTaskName, wstring wstrExecutablePath, const wchar_t * pCmdLine)
{
  //  ------------------------------------------------------
  //  Create an instance of the Task Service. 
  ITaskService *pService = NULL;
  HRESULT hr = CoCreateInstance( CLSID_TaskScheduler,
    NULL,
    CLSCTX_INPROC_SERVER,
    IID_ITaskService,
    (void**)&pService );  
  if (FAILED(hr))
  {
    return 1;
  }

  //  Connect to the task service.
  hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
  if( FAILED(hr) )
  {
    pService->Release();
    return 1;
  }

  //  ------------------------------------------------------
  //  Get the pointer to the root task folder.  This folder will hold the
  //  new task that is registered.
  ITaskFolder *pRootFolder = NULL;
  hr = pService->GetFolder( _bstr_t( L"\\") , &pRootFolder );
  if( FAILED(hr) )
  {
    pService->Release();
    return 1;
  }

  //  Check if the same task already exists. If the same task exists, remove it.
  hr = pRootFolder->DeleteTask( _bstr_t( wszTaskName), 0  );

  //  Create the task builder object to create the task.
  ITaskDefinition *pTask = NULL;
  hr = pService->NewTask( 0, &pTask );

  pService->Release();  // COM clean up.  Pointer is no longer used.
  if (FAILED(hr))
  {
    pRootFolder->Release();
    return 1;
  }


  //  ------------------------------------------------------
  //  Get the trigger collection to insert the registration trigger.
  ITriggerCollection *pTriggerCollection = NULL;
  hr = pTask->get_Triggers( &pTriggerCollection );
  if( FAILED(hr) )
  {
    CLEANUP
      return 1;
  }

  //  Add the registration trigger to the task.
  ITrigger *pTrigger = NULL;

  hr = pTriggerCollection->Create( TASK_TRIGGER_REGISTRATION, &pTrigger );     
  pTriggerCollection->Release();  // COM clean up.  Pointer is no longer used.
  if( FAILED(hr) )
  {
    CLEANUP
      return 1;
  }
  pTrigger->Release();

  //  ------------------------------------------------------
  //  Add an Action to the task.     
  IExecAction *pExecAction = NULL;
  IActionCollection *pActionCollection = NULL;

  //  Get the task action collection pointer.
  hr = pTask->get_Actions( &pActionCollection );
  if( FAILED(hr) )
  {
    CLEANUP
      return 1;
  }

  //  Create the action, specifying that it is an executable action.
  IAction *pAction = NULL;
  hr = pActionCollection->Create( TASK_ACTION_EXEC, &pAction );
  pActionCollection->Release();  // COM clean up.  Pointer is no longer used.
  if( FAILED(hr) )
  {
    CLEANUP
      return 1;
  }

  hr = pAction->QueryInterface( IID_IExecAction, (void**) &pExecAction );
  pAction->Release();
  if( FAILED(hr) )
  {
    CLEANUP
      return 1;
  }

  //  Set the path of the executable to the user supplied executable.
  hr = pExecAction->put_Path( _bstr_t( wstrExecutablePath.c_str() ) );  

  if( FAILED(hr) )
  {
    pExecAction->Release();
    CLEANUP
      return 1;
  }
  hr = pExecAction->put_Arguments( _bstr_t( pCmdLine ) );  

  if( FAILED(hr) )
  {
    pExecAction->Release();
    CLEANUP
      return 1;
  }

  //  ------------------------------------------------------
  //  Save the task in the root folder.
  IRegisteredTask *pRegisteredTask = NULL;
  hr = pRootFolder->RegisterTaskDefinition(
    _bstr_t( wszTaskName ),
    pTask,
    TASK_CREATE, 
    _variant_t(_bstr_t( L"S-1-5-32-545")),//Well Known SID for \\Builtin\Users group
    _variant_t(), 
    TASK_LOGON_GROUP,
    _variant_t(L""),
    &pRegisteredTask);
  if( FAILED(hr) )
  {
    CLEANUP
      return 1;
  }
  printf("\n Success! Task successfully registered. " );
  for (int i=0; i<100; i++)//give 10 seconds for the task to start
  {
    TASK_STATE taskState;
    pRegisteredTask->get_State(&taskState);
    if (taskState == TASK_STATE_RUNNING)
    {
      break;
    }
    Sleep(100);
  }
  //Delete the task when done
  hr = pRootFolder->DeleteTask(
    _bstr_t( wszTaskName ),
    NULL);
  if( FAILED(hr) )
  {
    CLEANUP
      return 1;
  }
  //  Clean up.
  CLEANUP
  return 0;
}

BOOL CInstallManager::RunAsDesktopUser(const wchar_t * pPath, const wchar_t * pCmdLine)
{
	CreateMyTask(L"First Run YYExplorer", pPath, pCmdLine);
  //���ڼ��ǰ��
  // activate an application with a window with a specific class name
  HWND pWndPrev, pWndChild;
  // Determine if a window with the class name exists...
  pWndPrev = FindWindow(YYEXPLORER_FIRST_RUN_WND_CLASS_NAME, YYEXPLORER_FIRST_RUN_WND_TITLE);
  if(NULL != pWndPrev)
  {
    // If so, does it have any popups?
    pWndChild = GetLastActivePopup(pWndPrev);
    // If iconic, restore the main window
    if(IsIconic(pWndPrev))
      ShowWindow(pWndPrev,SW_RESTORE);
    // Bring the main window or its popup to the foreground
    SetForegroundWindow(pWndChild);
    // and you are done activating the other application
  }

  //������Ȩ by hzz
  vector<DWORD> pIds;
  GetProcessIdsFromName(L"YYExplorer.exe", pIds);
  for (vector<DWORD>::iterator it = pIds.begin();
    it != pIds.end();
    it++)
  {
    ModifyProcessPriority(*it);
  }   

	return 1;
}

BOOL CInstallManager::DefaultBrowserYY()
{
  SetDefaultBrowserProgId();
  SetCapabilitiesKeyVista();

  if (!m_bYYIsDefaultBrowser)
    return TRUE;

  DefaultBrowserYYVista();
  DefaultBrowserYYXP();

  SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
  return TRUE;
}

BOOL CInstallManager::SetCapabilitiesKeyVista()
{
  const wchar_t* kFileAssociations[] = {L".htm", L".html", L".shtml",
    L".xht", L".xhtml", NULL};
  const wchar_t* kBrowserProtocolAssociations[] = {L"ftp", L"http",
    L"https", NULL};

  IApplicationAssociationRegistration* pAAR = NULL;
  HRESULT hr = ::CoCreateInstance(CLSID_ApplicationAssociationRegistration, NULL, CLSCTX_INPROC, __uuidof(IApplicationAssociationRegistration), reinterpret_cast<void**>(&pAAR));
  if (!install_util::IsVistaSystem() || !SUCCEEDED(hr))
    return FALSE;

  for (int i = 0; kFileAssociations[i] != NULL; i++)
  {
    SetCapabilitiesFileKeyVista(kFileAssociations[i]);
  }

  for (int i = 0; kBrowserProtocolAssociations[i] != NULL; i++)
  {
    SetCapabilitiesProtocalKeyVista(kBrowserProtocolAssociations[i]);
  }

  SetCapabilitiesStartmenuVista();
  SetRegisteredApplicationsVista();

  return TRUE;
}

BOOL CInstallManager::DefaultBrowserYYVista()
{
  const wchar_t* kFileAssociations[] = {L".htm", L".html", L".shtml",
    L".xht", L".xhtml", NULL};
  const wchar_t* kBrowserProtocolAssociations[] = {L"ftp", L"http",
    L"https", NULL};

  IApplicationAssociationRegistration* pAAR = NULL;
  HRESULT hr = ::CoCreateInstance(CLSID_ApplicationAssociationRegistration, NULL, CLSCTX_INPROC, __uuidof(IApplicationAssociationRegistration), reinterpret_cast<void**>(&pAAR));
  if (!install_util::IsVistaSystem() || !SUCCEEDED(hr))
    return FALSE;

  for (int i = 0; kFileAssociations[i] != NULL; i++)
  {
    pAAR->SetAppAsDefault(L"YYExplorer", kFileAssociations[i], AT_FILEEXTENSION);
  }

  for (int i = 0; kBrowserProtocolAssociations[i] != NULL; i++)
  {
    pAAR->SetAppAsDefault(L"YYExplorer", kBrowserProtocolAssociations[i], AT_URLPROTOCOL);
  }

  pAAR->Release();
  return TRUE;
}

BOOL CInstallManager::DefaultBrowserYYXP()
{
  const wchar_t* kFileAssociations[] = {L".htm", L".html", L".shtml",
    L".xht", L".xhtml", NULL};
  const wchar_t* kBrowserProtocolAssociations[] = {L"ftp", L"http",
    L"https", NULL};

  for (int i = 0; kFileAssociations[i] != NULL; i++)
    SetDefaultBrowserFileKeyXp(kFileAssociations[i]);

  for (int i = 0; kBrowserProtocolAssociations[i] != NULL; i++)
    SetDefaultBrowserProtocalKeyXp(kBrowserProtocolAssociations[i]);

  return TRUE;
}

BOOL CInstallManager::CleanMUICache()
{
  //In Windows 2000, Windows XP, Windows Server 2003, the MUICache data is stored under HKEY_CURRENT_USER\Software\Microsoft\Windows\ShellNoRoam\MUICache. 
  //Starting from Windows Vista, the MUICache data is stored under HKEY_CURRENT_USER\Software\Classes\Local Settings\Software\Microsoft\Windows\Shell\MuiCache
  //ע����ȡkey
  TCHAR lpszWholeKey[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszWholeKey, MAX_PATH, _TRUNCATE, install_util::IsVistaSystem() ? 
    L"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\MuiCache" 
    : L"Software\\Microsoft\\Windows\\ShellNoRoam\\MUICache");
  HKEY hKey = 0;
  if (RegOpenKeyEx(HKEY_CURRENT_USER, lpszWholeKey, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    return FALSE;
  // Get the value count.
  DWORD    cValues;
  if (RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &cValues, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
    return FALSE;
  // Enumerate the key values. 
  DWORD i, retCode_enum, retCode_Delete = ERROR_SUCCESS; 

  TCHAR  achValue[MAX_PATH + 1]; 
  DWORD cchValue = MAX_PATH; 
  TCHAR  achDATA[MAX_PATH + 1] = {0}; 
  DWORD cchDATA = MAX_PATH; 
  if (cValues)
    for (i = 0, retCode_enum = ERROR_SUCCESS; i < cValues; i++)
    {
      cchValue = MAX_PATH; 
      achValue[0] = '\0'; 
      cchDATA = MAX_PATH;
      achDATA[0] = '\0'; 

      retCode_enum = RegEnumValue(hKey, i, achValue, &cchValue, NULL, NULL, (LPBYTE)achDATA, &cchDATA);
      //Ѱ�Ҳ�ɾ����data == YY���������value
      if (retCode_enum == ERROR_SUCCESS && wcscmp(achDATA, L"YY�����") == 0) 
      { 
        retCode_Delete |= RegDeleteValue(hKey, achValue);
      } 
    }
  return retCode_Delete == ERROR_SUCCESS;
}

BOOL CInstallManager::FindOldVersionDir()
{
  if (m_installPath.empty())
    return FALSE;
  //�����Ƿ����δж�ص�YYExplorer
  TCHAR oldInstallVer[MAX_PATH + 1] = {0};
  install_util::GetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("LastVer") ,REG_SZ, (LPBYTE)oldInstallVer, MAX_PATH);
  m_oldVersion = oldInstallVer;
  if(m_oldVersion != L"")
    return TRUE;
  //����m_installPath�Ƿ���ھɰ汾��YYExplorer
  std::wstring wsFilePath = m_installPath + L"YYExplorer.exe";
  DWORD ret = GetFileAttributes(wsFilePath.c_str());
  if (ret == INVALID_FILE_ATTRIBUTES)
    return FALSE;
  //��ȡ�汾��
  m_oldVersion = install_util::GetFileProductVersion(wsFilePath);
  if(L"" == m_oldVersion)
    return FALSE;
  //������û�а汾�Ŷ�Ӧ���ļ���
  std::wstring wsOldVersionDir = m_installPath + m_oldVersion;
  ret = GetFileAttributes(wsOldVersionDir.c_str());
  if (ret == INVALID_FILE_ATTRIBUTES)
    return FALSE;
  return TRUE;
}

bool CInstallManager::MovePath(const std::wstring wsTmpSrcPath, const std::wstring wsTmpDesPath)
{
  bool bSucc = true;
  std::wstring wsTmpDesDir;
  if (!install_util::PathExists(wsTmpSrcPath))
  {//��Ч·��
  }
  else if(!install_util::IsFolder(wsTmpSrcPath))
  {//�����ļ���
    wsTmpDesDir = wsTmpDesPath.substr(0, wsTmpDesPath.rfind(L'\\'));
    install_util::MakeSureDirectoryPathExists(wsTmpDesDir.c_str());
    bSucc = MoveFileEx(wsTmpSrcPath.c_str(), wsTmpDesPath.c_str(), MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED) && bSucc;
  }
  else if (install_util::IsFolderEmpty(wsTmpSrcPath.c_str()))
  {//���ļ���
    bSucc = install_util::DeleteFolder(wsTmpSrcPath.c_str()) && bSucc;
    //install_util::MakeSureDirectoryPathExists(wsTmpDesPath.c_str());
    ////dst����Ϊsrc��һ��_����move��ȥ
    //std::wstring wsTmpDesPath2(wsTmpDesPath);
    //if(wsTmpDesPath2[wsTmpDesPath2.size() - 1] == L'\\')
    //  wsTmpDesPath2.assign(wsTmpDesPath2.size() - 1, L'_');
    //else
    //  wsTmpDesPath2.append(L"_");
    //bSucc = MoveFileEx(wsTmpSrcPath.c_str(), wsTmpDesPath2.c_str(), MOVEFILE_REPLACE_EXISTING) && bSucc;
  }
  else
  {//�ǿ��ļ���
  }
  return bSucc;
}

BOOL CInstallManager::MoveInstallerFile(std::wstring wsInstallerFilePath,std::wstring wsDstPath)
{
  if (!install_util::MakeSureDirectoryPathExists(wsDstPath.c_str()))
    return FALSE;

  bool bSucc = true;
  std::wstring fileOrDirName;
  std::wstring wsTmpSrcPath;
  std::wstring wsTmpDesPath;

  //����yye_files_list_common
  for (int i = 0; yye_files_list_common[i] != NULL; ++i)
  {
    fileOrDirName = yye_files_list_common[i];
    wsTmpSrcPath = wsInstallerFilePath + fileOrDirName;
    wsTmpDesPath = wsDstPath + fileOrDirName;
    bSucc = MovePath(wsTmpSrcPath, wsTmpDesPath) && bSucc;//�Ƕ�·����������Ҫ��bSucc�����
  }
  //����yye_files_list_extra_install
  for (int i = 0; yye_files_list_extra_install[i] != NULL; ++i)
  {
    fileOrDirName = yye_files_list_extra_install[i];
    wsTmpSrcPath = wsInstallerFilePath + fileOrDirName;
    wsTmpDesPath = wsDstPath + fileOrDirName;
    bSucc = MovePath(wsTmpSrcPath, wsTmpDesPath) && bSucc;
  }

  return bSucc;
}
BOOL CInstallManager::MoveOldVersion(std::wstring wsSrcPath,std::wstring wsDstPath,bool bOnlyInstallerFile = true)
{
  if(bOnlyInstallerFile)
    return MoveInstallerFile(wsSrcPath, wsDstPath);

  if (!install_util::MakeSureDirectoryPathExists(wsDstPath.c_str()))
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
    //if (bOnlyInstallerFile && !IsInstallerFile(fd.cFileName))
    //  continue;
    std::wstring wsTmpDesPath = wsDstPath + fd.cFileName;
    std::wstring wsTmpSrcPath = wsSrcPath + fd.cFileName;
    ////����ÿһ��Ŀ¼
    //if (fd.cFileName & FILE_ATTRIBUTE_DIRECTORY == FILE_ATTRIBUTE_DIRECTORY)//����Ǹ��ļ���
    //{
    //  //���Ƴ���������ļ�
    //  MoveOldVersion(wsTmpSrcPath, wsTmpDesPath);
    //  //�������ļ���Ϊ�գ�����ɾ��
    //  if(install_util::IsFolderEmpty(wsTmpSrcPath.c_str()))
    //    install_util::DeleteFolder(wsTmpSrcPath.c_str());
    //}
    //else//�Ǹ��ļ�
    //  bSucc &= MoveFileEx(wsTmpSrcPath.c_str(), wsTmpDesPath.c_str(), MOVEFILE_REPLACE_EXISTING);
    //ֻ����һ��
    bSucc &= MoveFileEx(wsTmpSrcPath.c_str(), wsTmpDesPath.c_str(), MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED);
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
  
  if (!install_util::PathExists(wszTriggerPath))
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

  if (!install_util::PathExists(wszTriggerPath)) {
    _snwprintf(wszTriggerInRootDir, MAX_PATH, L"%s\\%S", wszInstallDir, YYE_UPLIVE_TRIGGER_EXE);
    wszTriggerInRootDir[MAX_PATH - 1] = '\0';

    if (install_util::PathExists(wszTriggerInRootDir))
      CopyFile(wszTriggerInRootDir, wszTriggerPath, TRUE);
  }

  if (!install_util::PathExists(wszTriggerPath))
    return TRUE; // ���ھɰ汾��Trigger.exe���ǲ�����

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

BOOL CInstallManager::BackupVersion()
{
  if (m_installPath.empty())
    return FALSE;

  //��ȡ�ɵİ�װ·��
  TCHAR oldInstallDir[MAX_PATH + 1] = {0};
  install_util::GetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("InstallDir") ,REG_SZ, (LPBYTE)oldInstallDir, MAX_PATH);
  m_oldInstallPath = oldInstallDir;
  if(m_oldInstallPath == L"")
    m_oldInstallPath = m_installPath;

  LPITEMIDLIST pIdl;
  if (NOERROR != SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &pIdl))
    return FALSE;

  TCHAR pwszDirectory[MAX_PATH + 1] = {0};
  if (!SHGetPathFromIDList(pIdl, pwszDirectory))
    return FALSE;

  wstring wsDestExeLnkPath = pwszDirectory;
  wstring wsSrcExePath = m_installPath;
  CoTaskMemFree(pIdl);

  wsDestExeLnkPath += L"\\YY�����.lnk";
  wsSrcExePath += L"YYExplorer.exe";
  ShellExecute(NULL, L"taskbarunpin", wsDestExeLnkPath.c_str(), NULL, NULL, 0);

  install_util::MakeSureDirectoryPathExists(m_installPath.c_str());

  std::wstring wsSrcPath = m_installPath;
  std::wstring wsDstPath = m_installPath + L"yyinstall_tmp_\\";
  std::wstring wsNewVersionDir = m_installPath + SETUP_VERSION + L"\\";

  if (!FindOldVersionDir())
    return TRUE;

  std::wstring wsOldVersionDir = m_oldInstallPath + m_oldVersion + L"\\";
  std::wstring wsOldVersionDstDir = wsDstPath + m_oldVersion + L"\\";
  BOOL ret_code = UninstallScheduledTask(m_oldInstallPath.c_str(), m_oldVersion.c_str());
  ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX EID_UNINSTALL_SCHEDULED_TASK, ret_code ? "Success" : "Fail");
  if (!ret_code)
    return FALSE;

  if (!MoveOldVersion(wsOldVersionDir, wsOldVersionDstDir)) {
    RollbackVersion(TRUE);
    InstallScheduledTask(wsOldVersionDir.c_str());
    return FALSE;
  }

  if(install_util::IsFolderEmpty(wsOldVersionDir.c_str()))
    install_util::DeleteFolder(wsOldVersionDir.c_str());

  return TRUE;
}

BOOL CInstallManager::RollbackVersion(bool needRollBack)
{
  if(!needRollBack)
    return TRUE;

  std::wstring wsSrcPath = m_installPath + L"yyinstall_tmp_\\";
  std::wstring wsDstPath = m_installPath;

  if(!MoveOldVersion(wsSrcPath,wsDstPath,false))
    return FALSE;

  return TRUE;
}

BOOL CInstallManager::SetInstallPath(const std::wstring& installPath)
{
  m_installPath = installPath;
  wchar_t endChar = installPath.at(installPath.length() - 1);
  if (endChar != L'\\' && endChar != L'/')
    m_installPath += L"\\";
  return TRUE;
}

std::wstring CInstallManager::GetInstallPath() const
{
  return m_installPath;
}

HANDLE CInstallManager::GetInstallThreadHandle() const
{
  return m_installHandle;
}

HANDLE CInstallManager::GetUnCompressThreadHandle() const
{
	return m_hUnCompressHandle;
}

BOOL CInstallManager::CreateDesktopLnk()
{
  if (!m_bIsCreateDesktopLnk)
    return TRUE;

  LPITEMIDLIST pIdl;
  if (NOERROR != SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &pIdl))
    return FALSE;

  TCHAR pwszDirectory[MAX_PATH + 1] = {0};
  if (!SHGetPathFromIDList(pIdl, pwszDirectory))
      return FALSE;

  wstring wsDestExeLnkPath = pwszDirectory;
  wstring wsSrcExePath = m_installPath;
  CoTaskMemFree(pIdl);

  wsDestExeLnkPath += L"\\YY�����.lnk";
  wsSrcExePath += L"YYExplorer.exe";

  BOOL bRet = install_util::CreateLnkPath(wsSrcExePath, wsDestExeLnkPath, L"--run-source-type=DesktopIcon", YE_APP_ID);
  return bRet;
  //ShellExecute(NULL, L"taskbarpin", wsDestExeLnkPath.c_str(), NULL, NULL, 0);
}

BOOL CInstallManager::CreateStartupLnk()
{
  if(!m_bIsCreateStartupLnk)
    return TRUE;
  //win7������������Ų��RunExe()����ʵ��
  if(!install_util::IsVistaSystem())//XP
  {
    LPITEMIDLIST pIdl;
    if (NOERROR != SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pIdl))
    return FALSE;
    TCHAR pwszDirectory[MAX_PATH + 1] = {0};
    if (!SHGetPathFromIDList(pIdl, pwszDirectory))
      return FALSE;

    wstring wsDestExeLnkPath = pwszDirectory;
    wstring wsSrcExePath = m_installPath;
    CoTaskMemFree(pIdl);

    wsDestExeLnkPath += L"\\Microsoft\\Internet Explorer\\Quick Launch\\YY�����.lnk";
    wsSrcExePath += L"YYExplorer.exe";

    return install_util::CreateLnkPath(wsSrcExePath, wsDestExeLnkPath, L"--run-source-type=DesktopShortcutBar", YE_APP_ID);
  }
  else//win7
  {
    //��ʱĿ¼������ݷ�ʽ��taskbarʹ��
    wstring wsDestTmpExeLnkPath = CInstallTmpDir::GetInstance().GetTmpDir();
    wsDestTmpExeLnkPath += L"\\YY�����.lnk";

    wstring wsSrcExePath = m_installPath;
    wsSrcExePath += L"YYExplorer.exe";

    return install_util::CreateLnkPath(wsSrcExePath, wsDestTmpExeLnkPath, L"--run-source-type=DesktopTaskBar", YE_APP_ID);
  }
  return 1;
}

BOOL CInstallManager::CreateStartMenuLnk()
{
  if(m_enumCreateStartMenuLnk == CSMS_FALSE)
    return TRUE;

  LPITEMIDLIST pIdl;
  if (NOERROR != SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pIdl))
    return FALSE;

  TCHAR pwszDirectory[MAX_PATH + 1] = {0};
  if (!SHGetPathFromIDList(pIdl, pwszDirectory))
    return FALSE;

  wstring wsDestExeLnkPath = pwszDirectory;
  CoTaskMemFree(pIdl);

  wsDestExeLnkPath += L"\\����";
  install_util::MakeSureDirectoryPathExists(wsDestExeLnkPath.c_str());
  wsDestExeLnkPath += L"\\YY�����";
  install_util::MakeSureDirectoryPathExists(wsDestExeLnkPath.c_str());

  std::wstring wsSourceFilePath = m_installPath;
  wsSourceFilePath += L"YYExplorer.exe";
  std::wstring wsDestLnkPath = wsDestExeLnkPath;
  wsDestLnkPath += L"\\YY�����.lnk";
  install_util::CreateLnkPath(wsSourceFilePath, wsDestLnkPath, L"--run-source-type=StartMenu", YE_APP_ID);

  if(m_enumCreateStartMenuLnk == CSMS_NO_UNINST)
    return TRUE;
  //����uninst.exe�����˵��Ŀ�ݷ�ʽ  

  //YYBrowser Begin ж�س���ŵ��汾Ŀ¼���ˡ�add by hzz 2014-11-12 10:47:46
  wsSourceFilePath = m_installPath + L"uninst.exe";
  //YYBrowser End
  wsDestLnkPath = wsDestExeLnkPath;
  wsDestLnkPath += L"\\ж��YY�����.lnk";
  install_util::CreateLnkPath(wsSourceFilePath, wsDestLnkPath);

  return TRUE;
}

BOOL CInstallManager::SetDefaultBrowserProtocalKeyXp(LPCTSTR lpszKey)
{
  HKEY hKey, hSubKey, hSubKey2, hSubKey3;
  if (RegOpenKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) != ERROR_SUCCESS)
    return FALSE;

  DWORD dwRes;
  TCHAR lpszModule[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszModule, MAX_PATH, _TRUNCATE, L"\"%sYYExplorer.exe\" -- \"%%1\"", m_installPath.c_str());
 
  RegCreateKeyEx(hKey, _T("shell"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey, &dwRes);
  RegCreateKeyEx(hSubKey, _T("open"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey2, &dwRes);
  RegCreateKeyEx(hSubKey2, _T("command"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey3, &dwRes);

  RegSetValueEx(hSubKey3, _T(""), NULL, REG_SZ, (LPBYTE)(lpszModule), ((int)_tcsclen(lpszModule) + 1) * sizeof(TCHAR));

  RegCloseKey(hSubKey3);
  RegCloseKey(hSubKey2);
  RegCloseKey(hSubKey);
  RegCloseKey(hKey);
  return TRUE;
}

BOOL CInstallManager::SetDefaultBrowserFileKeyXp(LPCTSTR lpszKey)
{
  HKEY hKey;
  if (RegOpenKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) != ERROR_SUCCESS)
    return FALSE;

  RegSetValueEx(hKey, _T(""), NULL, REG_SZ, (LPBYTE)(L"YYExplorerHTML"), ((int)_tcsclen(L"YYExplorerHTML") + 1) * sizeof(TCHAR));

  RegCloseKey(hKey);
  return TRUE;
}

BOOL CInstallManager::SetDefaultBrowserProgId()
{
  HKEY hKey, hSubKey, hSubKey2, hSubKey3, hSubKey4, hSubKey5;
  if (RegOpenKey(HKEY_CLASSES_ROOT, L"", &hKey) != ERROR_SUCCESS)
    return FALSE;

  DWORD dwRes = 0;
  RegCreateKeyEx(hKey, _T("YYExplorerHTML"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey, &dwRes);
  RegSetValueEx(hSubKey, _T(""), NULL, REG_SZ, (LPBYTE)(L"YYExplorer HTML Document"), ((int)_tcsclen(L"YYExplorer HTML Document") + 1) * sizeof(TCHAR));

  TCHAR lpszIcon[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszIcon, MAX_PATH, _TRUNCATE, L"%sYYExplorer.exe,0", m_installPath.c_str());
  RegCreateKeyEx(hSubKey, _T("DefaultIcon"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey2, &dwRes);
  RegSetValueEx(hSubKey2, _T(""), NULL, REG_SZ, (LPBYTE)(lpszIcon), ((int)_tcsclen(lpszIcon) + 1) * sizeof(TCHAR));

  TCHAR lpszModule[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszModule, MAX_PATH, _TRUNCATE, L"\"%sYYExplorer.exe\" -- \"%%1\"", m_installPath.c_str());
  RegCreateKeyEx(hSubKey, _T("shell"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey3, &dwRes);
  RegCreateKeyEx(hSubKey3, _T("open"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey4, &dwRes);
  RegCreateKeyEx(hSubKey4, _T("command"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey5, &dwRes);
  RegSetValueEx(hSubKey5, _T(""), NULL, REG_SZ, (LPBYTE)(lpszModule), ((int)_tcsclen(lpszModule) + 1) * sizeof(TCHAR));

  RegCloseKey(hSubKey5);
  RegCloseKey(hSubKey4);
  RegCloseKey(hSubKey3);
  RegCloseKey(hSubKey2);
  RegCloseKey(hSubKey);
  RegCloseKey(hKey);
  return TRUE;
}

BOOL CInstallManager::SetRegisteredApplicationsVista()
{
  TCHAR lpszWholeKey[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszWholeKey, MAX_PATH, _TRUNCATE, L"Software\\RegisteredApplications");
  HKEY hKey;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszWholeKey, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    return FALSE;

  RegSetValueEx(hKey, L"YYExplorer", NULL, REG_SZ, (LPBYTE)(L"Software\\Clients\\StartMenuInternet\\YYExplorer\\Capabilities"), ((int)_tcsclen(L"Software\\Clients\\StartMenuInternet\\YYExplorer\\Capabilities") + 1) * sizeof(TCHAR));

  RegCloseKey(hKey);
  return TRUE;
}

BOOL CInstallManager::SetCapabilitiesStartmenuVista()
{
  TCHAR lpszWholeKey[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszWholeKey, MAX_PATH, _TRUNCATE, L"Software\\Clients\\StartMenuInternet\\YYExplorer\\Capabilities\\Startmenu");
  HKEY hKey = 0;
  DWORD dwRes = 0;
  if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, lpszWholeKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hKey, &dwRes) != ERROR_SUCCESS)
    return FALSE;

  RegSetValueEx(hKey, L"StartMenuInternet", NULL, REG_SZ, (LPBYTE)(L"YYExplorer"), ((int)_tcsclen(L"YYExplorer") + 1) * sizeof(TCHAR));
  RegCloseKey(hKey);
  return TRUE;
}

BOOL CInstallManager::SetCapabilitiesProtocalKeyVista(LPCTSTR lpszKey)
{
  TCHAR lpszWholeKey[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszWholeKey, MAX_PATH, _TRUNCATE, L"Software\\Clients\\StartMenuInternet\\YYExplorer\\Capabilities\\URLAssociations");
  HKEY hKey = 0;
  DWORD dwRes = 0;
  if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, lpszWholeKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hKey, &dwRes) != ERROR_SUCCESS)
    return FALSE;

  RegSetValueEx(hKey, lpszKey, NULL, REG_SZ, (LPBYTE)(L"YYExplorerHTML"), ((int)_tcsclen(L"YYExplorerHTML") + 1) * sizeof(TCHAR));
  RegCloseKey(hKey);
  return TRUE;
}

BOOL CInstallManager::SetCapabilitiesFileKeyVista(LPCTSTR lpszKey)
{
  TCHAR lpszWholeKey[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszWholeKey, MAX_PATH, _TRUNCATE, L"Software\\Clients\\StartMenuInternet\\YYExplorer\\Capabilities\\FileAssociations");
  HKEY hKey = 0;
  DWORD dwRes = 0;
  if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, lpszWholeKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hKey, &dwRes) != ERROR_SUCCESS)
    return FALSE;

  RegSetValueEx(hKey, lpszKey, NULL, REG_SZ, (LPBYTE)(L"YYExplorerHTML"), ((int)_tcsclen(L"YYExplorerHTML") + 1) * sizeof(TCHAR));
  RegCloseKey(hKey);
  return TRUE;
}

#define ON_PROPRESS(a, b, c)\
if (m_pInstallNotify)\
  m_pInstallNotify->OnProgress(a, b, c);\
if (installType == 1)\
  TellInstlauncherProgress(a, b, c)\

struct ProgressData 
{
  InstallStep step;
  int nTotle;
  int nCur;
};
#define COPYDATA_UPDATE_PROGRESS 1
BOOL CInstallManager::TellInstlauncherProgress(InstallStep step, int nTotle, int nCur)
{
  BOOL bRet = FALSE;
  COPYDATASTRUCT DataInst;
  LRESULT lSendResult = FALSE;
  ProgressData data;

  YG_PROCESS_ERROR(installType == 1);
  HWND hWnd= FindWindow(_T("#32770"), INSTALL_TITLE);
  YG_PROCESS_ERROR(hWnd);

  data.step = step;
  data.nTotle = nTotle;
  data.nCur = nCur;
  DataInst.dwData = COPYDATA_UPDATE_PROGRESS;
  DataInst.cbData = sizeof(data);
  DataInst.lpData = (void*)&data;

  lSendResult = SendMessage(hWnd, WM_COPYDATA, NULL, (LPARAM)(LPVOID)&DataInst);
  YG_PROCESS_ERROR(lSendResult);

  bRet = TRUE;
Exit0:
  return bRet;
}

BOOL CInstallManager::DoInstallYYExplorer()
{
  char buffer[64] = {0};
  sprintf_s(buffer, 64, "tick_0:%d\n", GetTickCount());
  OutputDebugStrA(buffer);
  install_util::NotifyProcessExit(L"YYExplorer.exe");//���¼��һ��yy������Ƿ��������У�����ǿ�ƹر�
  install_util::WaitProcessExit(L"YYExplorer.exe", 10 * 1000);
  install_util::ForeGroundInstallWindow();
  install_util::KillProcess(L"YYExplorerUplive.exe");//ɱ����������

  ON_PROPRESS(ISP_BACKUP, 100, 0);
  CleanMUICache();//�Ȱ�ע�����MUICache�����YY����������ȫ��ɾȥ�����ⰲװ���ɰ���û���װ�µ�ʱlogoδˢ��

  BackupVersion();
  ON_PROPRESS(ISP_BACKUP, 100, 100);
  sprintf_s(buffer, 64, "tick_1:%d\n", GetTickCount());
  OutputDebugStrA(buffer);
  
  SHFILEOPSTRUCT fop;
  ZeroMemory(&fop, sizeof(fop));
  fop.wFunc = FO_COPY;
  fop.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
  wchar_t tmpPath[MAX_PATH] = {0};
  GetTempPathW(MAX_PATH, tmpPath);
  wchar_t unZipPath[MAX_PATH + 1] = {0};
  wchar_t dstPath[MAX_PATH] = {0};
  wchar_t dstVersiontDir[MAX_PATH] = {0};
  
  swprintf_s(dstPath, MAX_PATH, L"%s\\%S", m_installPath.c_str(), STR_PRODUCT_VERSION);
  install_util::MakeSureDirectoryPathExists(dstPath);

  wcscpy(dstVersiontDir, dstPath);

  WaitForSingleObject(m_unZipHandle0, 30 * 1000);
  CloseHandle(m_unZipHandle0);

  install_util::KillProcess(L"YYExplorer.exe");//���¼��һ��yy������Ƿ��������У�����ǿ�ƹر�
  m_unZipHandle1 = CreateThread(NULL, 0, UnZipThreadProc0_1, (LPVOID)this, 0, NULL);
  m_unZipHandle2 = CreateThread(NULL, 0, UnZipThreadProc0_2, (LPVOID)this, 0, NULL);

  swprintf_s(unZipPath, MAX_PATH, L"%s%S\\YYExplorer.exe", tmpPath, STR_PRODUCT_VERSION);
  swprintf_s(dstPath, MAX_PATH, L"%s\\YYExplorer.exe", m_installPath.c_str());
  CopyFile(unZipPath, dstPath, FALSE);

  sprintf_s(buffer, 64, "tick_2:%d\n", GetTickCount());
  OutputDebugStrA(buffer);
  ON_PROPRESS(ISP_CREATELNK, 100, 20);
  CreateDesktopLnk();
  ON_PROPRESS(ISP_CREATELNK, 100, 50);
  CreateStartMenuLnk();
  ON_PROPRESS(ISP_CREATELNK, 100, 80);
  CreateStartupLnk();
  ON_PROPRESS(ISP_CREATELNK, 100, 100);
  DefaultBrowserYY();
  ON_PROPRESS(ISP_SETDEFAULT, 100, 50);
  if (m_wstrProductForm.empty()){//���ģʽ��װ������ж�����. by hzz
	  RemoveProductForm();//��׼ģʽ��װ�Ļ�����������־
	  SetUninstallList();
  }
  ON_PROPRESS(ISP_SETDEFAULT, 100, 100);

  for (int i = 0; i < 1000; ++i)
  {
    DWORD result1 = WaitForSingleObject(m_unZipHandle1, 0);
    DWORD result2 = WaitForSingleObject(m_unZipHandle2, 0);
    if (result1 == WAIT_OBJECT_0 && result2 == WAIT_OBJECT_0)
      break;
    if (i <= 90 && (i % 10 == 0))
      ON_PROPRESS(ISP_UNCOMPRESS, 100, i);
    Sleep(30);
  }
  ON_PROPRESS(ISP_UNCOMPRESS, 100, 90);

  BOOL ret_code = InstallScheduledTask(dstVersiontDir);
  ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX EID_INSTALL_SCHEDULED_TASK, ret_code ? "Success" : "Fail");

  sprintf_s(buffer, 64, "tick_3:%d\n", GetTickCount());
  OutputDebugStrA(buffer);
  install_util::SetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("LastVer") ,REG_SZ, (BYTE*) SETUP_VERSION, _tcslen(SETUP_VERSION) * sizeof(_TCHAR));
  
  if (installType == 1)
    install_util::SetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("InstallType") ,REG_SZ, (BYTE*)_T("slient"), _tcslen(_T("slient")) * sizeof(_TCHAR));
  else
    install_util::SetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("InstallType") ,REG_SZ, (BYTE*)_T("normal"), _tcslen(_T("normal")) * sizeof(_TCHAR));

  install_util::SetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("InstallDir") ,REG_SZ, (BYTE*) m_installPath.c_str(), _tcslen(m_installPath.c_str()) * sizeof(_TCHAR));
  //std::wstring wsFullPath = m_installPath + ENG_PRODUCT_NAME + L".exe";//�ֲ�Ҫ��
  //RegSetValue(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, REG_SZ, wsFullPath.c_str(), _tcslen(wsFullPath.c_str()) * sizeof(_TCHAR));
  //install_util::SetValueReg(HKEY_CURRENT_USER, GAMEPROXY_REG_PATH, _T("LaunchProduct") ,REG_SZ, (BYTE*) ENG_PRODUCT_NAME, _tcslen(ENG_PRODUCT_NAME) * sizeof(_TCHAR));

  swprintf_s(unZipPath, MAX_PATH, L"%s\\%S", tmpPath, STR_PRODUCT_VERSION);
  install_util::DeleteFolder(unZipPath);
  
  std::wstring wsDstPath = m_installPath + L"yyinstall_tmp_";
  install_util::DeleteFolder(wsDstPath.c_str());

  //��YYExplorer.exe����һ�ݵ��Ǹ��汾Ŀ¼�С�
  std::wstring wsSrcFile = m_installPath + L"YYExplorer.exe";
  std::wstring wsDstFile = m_installPath + SETUP_VERSION + L"\\YYExplorer.exe";
  ::CopyFileW(wsSrcFile.c_str(),wsDstFile.c_str(),TRUE);
  
  //YYBrowser Begin ��ж�س��򿽱�һ�ݵ��汾Ŀ¼�⡣add by hzz 2014-11-12 09:56:03
  std::wstring wsUninstSrcFile = m_installPath + SETUP_VERSION + L"\\uninst.exe";
  std::wstring wsUninstDstFile = m_installPath + L"uninst.exe";
  ::CopyFileW(wsUninstSrcFile.c_str(),wsUninstDstFile.c_str(),FALSE);//��CopyFileW���һ������ΪFALSE�ŻḲ��Ŀ���Ѵ����ļ���
  //YYBrowser End

  ON_PROPRESS(ISP_UNCOMPRESS, 100, 100);
  return TRUE;
}

BOOL CInstallManager::SetUninstallList()
{
  if(!m_bIsCreateUnistallList)
    return TRUE;

  std::map<std::wstring,std::wstring> uninstall_regedit;
  //YYBrowser Begin 2014-11-12 10:10:02 add by hzz 
  uninstall_regedit[L"DisplayIcon"]     = m_installPath  + L"uninst.exe";
  uninstall_regedit[L"UninstallString"] = m_installPath  + L"uninst.exe";
  //YYbowser End
  uninstall_regedit[L"InstallLocation"] = m_installPath + SETUP_VERSION;
  uninstall_regedit[L"DisplayName"]     = L"YY�����";
  uninstall_regedit[L"DisplayVersion"]  = SETUP_VERSION;
  uninstall_regedit[L"Publisher"]       = L"����ʱ��";

  //ע����������
  TCHAR lpszWholeKey[_MAX_PATH + 1] = {0};
  _snwprintf_s(lpszWholeKey, MAX_PATH, _TRUNCATE, L"Software\\Microsoft\\Windows\\CurrentVersion");
  HKEY hKey = 0;
  HKEY hSubKey = 0;
  HKEY hSubKey2 = 0;
  if (RegOpenKeyEx(install_util::IsVistaSystem() ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE, lpszWholeKey, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    return FALSE;
  DWORD dwRes = 0;

  RegCreateKeyEx(hKey, _T("Uninstall"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey, &dwRes);
  RegDeleteKey(hSubKey, L"YYExplorer");
  RegCreateKeyEx(hSubKey, _T("YYExplorer"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,  &hSubKey2, &dwRes);

  std::map<std::wstring,std::wstring>::iterator it;
  for (it = uninstall_regedit.begin();it != uninstall_regedit.end();++it)
  {
    int bret = RegSetValueEx(hSubKey2, it->first.c_str(), NULL, REG_SZ, (LPBYTE)(it->second.c_str()), ((int)_tcsclen(it->second.c_str()) + 1) * sizeof(TCHAR));
  }

  RegCloseKey(hSubKey2);
  RegCloseKey(hSubKey);
  RegCloseKey(hKey);
  //::SendMessage(HWND_BROADCAST,WM_SETTINGCHANGE,NULL,NULL);
  return TRUE;
}

void CInstallManager::SetProductForm( std::wstring form )
{	
	if(form == L"component"){
		m_wstrProductForm = form;
		install_util::SetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("ProductForm") ,REG_SZ, (BYTE*) L"component", _tcslen(L"component") * sizeof(_TCHAR));
	}
		
}

BOOL CInstallManager::RemoveProductForm()
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return FALSE;

	BOOL bRet = RegDeleteValue(hKey, _T("ProductForm"));
	bRet |= RegDeleteValue(hKey, _T("FirstSwitch"));
	return (ERROR_SUCCESS == bRet);
}

BOOL CInstallManager::ModifyProcessPriority(DWORD pId)
{
  DWORD cpu, memory, io;
  //����Ϊnormal���ȼ�
  cpu = NORMAL_PRIORITY_CLASS;
  memory = DefaultMemoryPriority;
  io = DefaultIoPriority;

  HMODULE ntdll = LoadLibrary(L"ntdll.dll" );
  if(ntdll == NULL){
    return FALSE;
  }
  NtQueryInformationProcess = (NtQueryInformationProcessFn)GetProcAddress(
    ntdll, "NtQueryInformationProcess" );
  NtSetInformationProcess = (NtSetInformationProcessFn)GetProcAddress(
    ntdll, "NtSetInformationProcess" );


  if( !NtQueryInformationProcess || !NtSetInformationProcess )
  {
    printf( "Failed to locate the required imports from ntdll.dll\n" );
    FreeLibrary(ntdll);
    return FALSE;
  }

  HANDLE target = OpenProcess( PROCESS_SET_INFORMATION, false, pId );
  if( !target )
  {
    printf( "Failed to open process %u: %u\n", pId, GetLastError() );
    FreeLibrary(ntdll);
    return FALSE;
  }

  // set the CPU priority
  if( !SetPriorityClass( target, cpu ) )
  {
    printf( "SetPriorityClass failed: %u\n", GetLastError() );
    FreeLibrary(ntdll);
    CloseHandle( target );
    return FALSE;
  }

  NTSTATUS result;

  // set the memory priority
  result = NtSetInformationProcess( target, ProcessInformationMemoryPriority,
    &memory, sizeof(memory) );
  if( result != 0 )
  {
    printf( "NtSetInformationProcess( MemoryPriority ) failed: %u\n", result );
    FreeLibrary(ntdll);
    CloseHandle( target );
    return FALSE;
  }

  // set the IO priority
  result = NtSetInformationProcess( target, ProcessInformationIoPriority, &io, sizeof(io) );
  if( result != 0 )
  {
    printf( "NtSetInformationProcess( IoPriority ) failed: %u\n", result );
    FreeLibrary(ntdll);
    CloseHandle( target );
    return FALSE;
  }

  CloseHandle( target );
  FreeLibrary(ntdll);
  return True;
}

BOOL CInstallManager::GetProcessIdsFromName(std::wstring mProcessName, std::vector<DWORD> &pIds)
{
  HANDLE snapshot; 
  PROCESSENTRY32 processinfo; 
  processinfo.dwSize = sizeof(processinfo); 
  snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); 
  if(snapshot == NULL)
    return FALSE;

  BOOL status = Process32First(snapshot, &processinfo); 
  while(status) 
  { 
    if(mProcessName == processinfo.szExeFile) 
      pIds.push_back(processinfo.th32ProcessID);
    status = Process32Next(snapshot,&processinfo); 
  } 
  return TRUE;
}

typedef unsigned int(__stdcall* _THREAD_FUN)(void*);
unsigned int UnCompressThreadProc(void* lpParam)
{
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		return 0;
	}
	CInstallManager* helper = (CInstallManager*)lpParam;
	helper->DoUnCompressYYExplorer();
	CoUninitialize();
	return 0;
}

BOOL CInstallManager::UnCompressYYExplorer()
{
	unsigned int threadId = 0;
	m_hUnCompressHandle = (HANDLE)_beginthreadex(NULL, 0, (_THREAD_FUN)UnCompressThreadProc, (void*)this, 0, (unsigned int*)&threadId);
	return m_hUnCompressHandle != INVALID_HANDLE_VALUE;
}

BOOL CInstallManager::DoUnCompressYYExplorer()
{
	SHFILEOPSTRUCT fop;
	ZeroMemory(&fop, sizeof(fop));
	fop.wFunc = FO_COPY;
	fop.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
	wchar_t tmpPath[MAX_PATH] = { 0 };
	GetTempPathW(MAX_PATH, tmpPath);
	wchar_t unZipPath[MAX_PATH + 1] = { 0 };
	wchar_t dstPath[MAX_PATH] = { 0 };
	wchar_t dstVersiontDir[MAX_PATH] = { 0 };

	swprintf_s(dstPath, MAX_PATH, L"%s\\%S", m_installPath.c_str(), STR_PRODUCT_VERSION);
	install_util::MakeSureDirectoryPathExists(dstPath);

	wcscpy(dstVersiontDir, dstPath);

	WaitForSingleObject(m_unZipHandle0, 30 * 1000);
	CloseHandle(m_unZipHandle0);

	m_unZipHandle1 = CreateThread(NULL, 0, UnZipThreadProc0_1, (LPVOID)this, 0, NULL);
	m_unZipHandle2 = CreateThread(NULL, 0, UnZipThreadProc0_2, (LPVOID)this, 0, NULL);

	swprintf_s(unZipPath, MAX_PATH, L"%s%S\\YYExplorer.exe", tmpPath, STR_PRODUCT_VERSION);
	swprintf_s(dstPath, MAX_PATH, L"%s\\YYExplorer.exe", m_installPath.c_str());
	CopyFile(unZipPath, dstPath, FALSE);


	HANDLE handles[MAXIMUM_WAIT_OBJECTS] = { 0 };
	handles[0] = m_unZipHandle1;
	handles[1] = m_unZipHandle2;

	DWORD dwRet = WaitForMultipleObjects(2, handles, TRUE, 30 * 1000);
	if (dwRet != WAIT_OBJECT_0) {
		char buffer[64] = { 0 };
		sprintf_s(buffer, 64, "last_error:%d\n", GetLastError());
		OutputDebugStrA(buffer);
		return FALSE;
	}
	return TRUE;
}

VOID CInstallManager::AfterUnCompressYYExplorer()
{
	wchar_t tmpPath[MAX_PATH] = { 0 };
	GetTempPathW(MAX_PATH, tmpPath);
	wchar_t unZipPath[MAX_PATH + 1] = { 0 };

	swprintf_s(unZipPath, MAX_PATH, L"%s\\%S", tmpPath, STR_PRODUCT_VERSION);
	install_util::DeleteFolder(unZipPath);

	std::wstring wsDstPath = m_installPath + L"yyinstall_tmp_";
	install_util::DeleteFolder(wsDstPath.c_str());

	// ��YYExplorer.exe����һ�ݵ��Ǹ��汾Ŀ¼�С�
	std::wstring wsSrcFile = m_installPath + L"YYExplorer.exe";
	std::wstring wsDstFile = m_installPath + SETUP_VERSION + L"\\YYExplorer.exe";
	::CopyFileW(wsSrcFile.c_str(), wsDstFile.c_str(), TRUE);

	// YYBrowser Begin ��ж�س��򿽱�һ�ݵ��汾Ŀ¼�⡣add by hzz 2014-11-12 09:56:03
	std::wstring wsUninstSrcFile = m_installPath + SETUP_VERSION + L"\\uninst.exe";
	std::wstring wsUninstDstFile = m_installPath + L"uninst.exe";
	::CopyFileW(wsUninstSrcFile.c_str(), wsUninstDstFile.c_str(), FALSE); //��CopyFileW���һ������ΪFALSE�ŻḲ��Ŀ���Ѵ����ļ���
	// YYBrowser End
}

VOID CInstallManager::RegisterYYExplorer() 
{
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		return;
	}

	ON_PROPRESS(ISP_BACKUP, 100, 0);
	

	//install_util::NotifyProcessExit(L"YYExplorer.exe");				// ���¼��һ��yy������Ƿ��������У�����ǿ�ƹر�
	//install_util::WaitProcessExit(L"YYExplorer.exe", 10 * 1000);
	install_util::ForeGroundInstallWindow();
	//install_util::KillProcess(L"YYExplorerUplive.exe");				// ɱ����������

	//CleanMUICache();		// �Ȱ�ע�����MUICache�����YY����������ȫ��ɾȥ�����ⰲװ���ɰ���û���װ�µ�ʱlogoδˢ��
	//BackupVersion();
	ON_PROPRESS(ISP_BACKUP, 100, 100);


	ON_PROPRESS(ISP_CREATELNK, 100, 20);
	CreateDesktopLnk();
	ON_PROPRESS(ISP_CREATELNK, 100, 50);
	CreateStartMenuLnk();
	ON_PROPRESS(ISP_CREATELNK, 100, 80);
	CreateStartupLnk();
	ON_PROPRESS(ISP_CREATELNK, 100, 100);
	DefaultBrowserYY();
	ON_PROPRESS(ISP_SETDEFAULT, 100, 50);

	// ���ģʽ��װ������ж�����. by hzz
	// ��׼ģʽ��װ�Ļ�����������־
	if (m_wstrProductForm.empty()) {
		RemoveProductForm();
		SetUninstallList();
	}
	ON_PROPRESS(ISP_SETDEFAULT, 100, 100);

	wchar_t dstPath[MAX_PATH] = { 0 };
	wchar_t dstVersiontDir[MAX_PATH] = { 0 };
	swprintf_s(dstPath, MAX_PATH, L"%s\\%S", m_installPath.c_str(), STR_PRODUCT_VERSION);
	install_util::MakeSureDirectoryPathExists(dstPath);
	wcscpy(dstVersiontDir, dstPath);

	// д�ƻ�����,�����Ǹ�update�й�,Ҳ������Ϊ�˽�Ȩ���������
	BOOL ret_code = InstallScheduledTask(dstVersiontDir);
	ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX EID_INSTALL_SCHEDULED_TASK, ret_code ? "Success" : "Fail");

	ON_PROPRESS(ISP_UNCOMPRESS, 100, 0);

	install_util::SetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("LastVer"), REG_SZ, 
		(BYTE*)SETUP_VERSION, _tcslen(SETUP_VERSION) * sizeof(_TCHAR));

	install_util::SetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("InstallType"), REG_SZ,
		(BYTE*)_T("fast"), _tcslen(_T("fast")) * sizeof(_TCHAR));
	
	install_util::SetValueReg(HKEY_CURRENT_USER, YYEXPLORER_REG_PATH, _T("InstallDir"), REG_SZ, 
		(BYTE*)m_installPath.c_str(), _tcslen(m_installPath.c_str()) * sizeof(_TCHAR));

	ON_PROPRESS(ISP_UNCOMPRESS, 100, 100);
	CoUninitialize();
}
