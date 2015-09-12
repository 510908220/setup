// YBUnInstall.cpp : main source file for YBUnInstall.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <shellapi.h>

#include "resource.h"
#include "yg_eid_desc.h"
#include "confirm_dialog.h"
#include "uninstall_manager.h"
#include "res_filename.h"
#include "lang_string.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
#include "uninstall_util.h"
#include "yygame/uninstall/choose_uninstall_dialog.h"
#define EXIT_ERROR_OK                         0
#define EXIT_ERROR_GET_OTHER_INSTANCE_WND     1
#define EXIT_ERROR_NOT_GET_OTHER_INSTANCE_WND 2

CAppModule _Module;

class UniqueApp
{
public:
  UniqueApp();
  ~UniqueApp();

  BOOL IsUnique();

  // 限制其他进程实例执行
  BOOL Register();

  // 不再限制其他进程实例执行
  void Unregister();
private:
  HANDLE m_hMutex;
  char m_szKey[MAX_PATH];
};

#define LOCAL_PREFIX "Global\\"

#define SAFE_RELEASE_MUTEX(mutex)\
  do \
{\
  if (mutex)\
{\
  ReleaseMutex(mutex);\
  (mutex) = NULL;\
}\
} while (0)

UniqueApp::UniqueApp() : m_hMutex(NULL)
{
  strcpy(m_szKey, LOCAL_PREFIX);
  size_t uPrefixLen = strlen(LOCAL_PREFIX);
  char* szExecPath = m_szKey + uPrefixLen;
  GetModuleFileNameA(NULL, szExecPath, sizeof(m_szKey) - uPrefixLen);
  char* pChar = szExecPath;
  while (*pChar)
  {
    if (*pChar == '\\')
      *pChar = '/';
    pChar++;
  }
  CharLowerBuffA(szExecPath, strlen(szExecPath));
}
UniqueApp::~UniqueApp()
{
  Unregister();
}

BOOL UniqueApp::IsUnique()
{
  BOOL bRetCode = FALSE;
  BOOL bResult = FALSE;
  HANDLE hMutex = NULL;

  hMutex = OpenMutexA(READ_CONTROL, FALSE, m_szKey);
  YG_PROCESS_ERROR(!hMutex);

  bResult = TRUE;
Exit0:
  SAFE_RELEASE_MUTEX(hMutex);
  return bResult;
}

BOOL UniqueApp::Register()
{
  BOOL bRetCode = FALSE;
  BOOL bResult = FALSE;

  SetLastError(S_OK);
  m_hMutex = CreateMutexA(NULL, FALSE, m_szKey);
  YG_PROCESS_ERROR(m_hMutex);
  YG_PROCESS_ERROR(GetLastError() != ERROR_ALREADY_EXISTS);

  bResult = TRUE;
Exit0:
  if (!bResult)
  {
    SAFE_RELEASE_MUTEX(m_hMutex);
  }
  return bResult;
}

void UniqueApp::Unregister()
{
  SAFE_RELEASE_MUTEX(m_hMutex);
}

int CmdLineFindFlag(int argc, LPWSTR* argv, LPCWSTR flag)
{
	if (!argc || !argv)
		return -1;

	for (int i = 0; i < argc; ++i)
	{
		if (!wcscmp(argv[i], flag))
			return i;
	}
	return -1;
}

bool IsDebugStringValue(){
	int value = 0;
	int length = sizeof(int);
	uninstall_util::GetValueReg(HKEY_CURRENT_USER, _TEXT("SOFTWARE\\duowan\\yyesetup"), _TEXT("dbgOutputReport"), REG_DWORD, (LPBYTE)&value, length);
	if (value)
		return true;
	return false;
}
void InitDataReport(int argc, LPWSTR* argv)
{
  ygdata_report::GetMachineIdTimeout(10 * 1000);
  ygdata_report::AddEidDescList(kEidDescList);
	bool debug_string = CmdLineFindFlag(argc, argv, L"--data-report-debug-string") >= 0;
	//增加窗口
	debug_string = debug_string || IsDebugStringValue();

	ygdata_report::Init(L"http://stat.game.yy.com/data.do", "pas", "yyexplorer", 5 * 60 * 1000, NULL, L"", 0, debug_string);
	ygdata_report::Run();

	SessionInfo info;
	std::string screen_size("0.0");
	std::string screen_color("0");

	DEVMODEW dm;
	dm.dmSize = sizeof(dm);

	BOOL result = EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	if (result)
	{
		char number[32];

		screen_size.clear();
		screen_size.append(_itoa(dm.dmPelsWidth, number, 10));
		screen_size.append(".");
		screen_size.append(_itoa(dm.dmPelsHeight, number, 10));

		screen_color.clear();
		screen_color.append(_itoa(dm.dmBitsPerPel, number, 10));
	}

	info.cha = "yeuninstall";
	info.rso = "from_yeuninstall";
	info.ive = STR_PRODUCT_VERSION"."STR_FILE_VERSION;
	info.uve = info.ive;
	info.os = ygdata_report::GetOsPlatformVersion();
	info.lla = "zh_cn";
	info.sre = screen_size;
	info.sco = screen_color;
	ygdata_report::PostSessionInfo(info);

	ygdata_report::SendEventWithSrc("yeuninstall", "StartUp");
}

void DeleteUninst(LPCWSTR wszProcess, LPCWSTR wszUninst)
{
	if (!wszProcess || !*wszProcess)
		return;

	if (!wszUninst || !*wszUninst)
		return;
    
	HANDLE hProcessOrig = (HANDLE)_wtoi(wszProcess);
	if (!hProcessOrig)
		return;

	WaitForSingleObject(hProcessOrig, INFINITE);
	CloseHandle(hProcessOrig);

  //YYBrowser Begin 将卸载程序所在目录的一些空目录删掉.add by hzz 2014-11-12 11:24:18
  WIN32_FIND_DATA fd;
  std::wstring wsTmp = wszUninst;
  int end_pos = wsTmp.find_last_of(L"\\");
  std::wstring wszUninstDir = wsTmp.substr(0, end_pos);
  wszUninstDir += L"\\*.*";
  HANDLE hFindData = FindFirstFile(wszUninstDir.c_str(), &fd);
  if (INVALID_HANDLE_VALUE == hFindData)
    return;
  do 
  {
    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      if (uninstall_util::IsFolderEmpty(fd.cFileName)) {
        uninstall_util::DeleteFolder(fd.cFileName);
      }
    }
  } while (FindNextFile(hFindData,&fd));
  FindClose(hFindData);
  //YYBrowsoeor End

	uninstall_util::DeleteFile(wszUninst);
   //YYBrowser Begin add by hzz 2014-11-12 11:36:24
  ////获取上一级文件夹
  //std::wstring wsUninst = wszUninst;
  //std::wstring wsVersionDir = wsUninst.substr(0, wsUninst.find_last_of(L'\\') + 1);
  ////删除上一级文件夹(如果为空)
  //if(uninstall_util::IsFolderEmpty(wsVersionDir.c_str()))
  //  uninstall_util::DeleteFolder(wsVersionDir.c_str());
  //YYBrowser End
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
  int argc = 0;
  LPWSTR* argv = CommandLineToArgvW(lpstrCmdLine, &argc);

  int nDelFlagPos = CmdLineFindFlag(argc, argv, L"-d");
  if (nDelFlagPos >= 0 && argc >= nDelFlagPos + 3)
  {
    DeleteUninst(argv[nDelFlagPos + 1], argv[nDelFlagPos + 2]);
    return 0;
  }
  
  //保证程序是唯一实例
  UniqueApp unique_uninst;
  if(!unique_uninst.Register())
  {
    //窗口激活，前置
    // activate an application with a window with a specific class name
    HWND pWndPrev, pWndChild;
    // Determine if a window with the class name exists...
    pWndPrev = FindWindow(_T("#32770"), UNINSTALL_TITLE);
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
      return EXIT_ERROR_NOT_GET_OTHER_INSTANCE_WND;
    }
    return EXIT_ERROR_GET_OTHER_INSTANCE_WND;
  }

  InitDataReport(argc, argv);
  HRESULT hRes = ::CoInitialize(NULL);

  GDIPlusFacade::Startup(hInstance);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

  int nRet = 0;
  {
    if (!CUnInstallManager::GetInstance().Init()) //安装前置条件
      goto EXIT_MAIN;
  }

	// BLOCK: Run application
	{
		CChooseUninstallDialog dlg_choose_uninstall;
		nRet = dlg_choose_uninstall.DoModal();
	}

  CUnInstallManager::GetInstance().UnInit();
EXIT_MAIN:
	_Module.Term();

  GDIPlusFacade::Shutdown();
	::CoUninitialize();

	ygdata_report::Exit(TRUE);
	if (argv)
	{
		LocalFree(argv);
		argv = NULL;
	}
	return nRet;
}
