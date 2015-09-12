// YBSetup.cpp : main source file for YBSetup.exe
//

#include "stdafx.h"
#include "resource.h"
#include "licence_dialog.h"
#include "install_manager.h"
#include "7z/7zCrc.h"
#include "install_tmpdir.h"
#include "lang_string.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
#include "install_util.h"
#include <shellapi.h>

#include "resfile_struct.h"
#include "yg_eid_desc.h"

#define EXIT_ERROR_OK                         0
#define EXIT_ERROR_GET_OTHER_INSTANCE_WND     1
#define EXIT_ERROR_NOT_GET_OTHER_INSTANCE_WND 2

CAppModule _Module;

class SetupSwitchs
{
public:
  SetupSwitchs()
  {
    m_bSilent = false;

	m_bOnlyUnCompress = false;
	m_bOnlyRegister = false;;
  }
  void OnSwitch(const wchar_t* cmd)
  {
    if (install_util::CmdlineIs(cmd, L"silent"))
    {
      m_bSilent = true;
    }

	else if (install_util::CmdlineIs(cmd, L"only-uncompress")) {
		m_bOnlyUnCompress = true;
	}
	else if (install_util::CmdlineIs(cmd, L"only-register")) {
		m_bOnlyRegister = true;
	}

    else if (install_util::CmdlineIs(cmd, L"no-create-desktop-lnk"))//兼容以前版本
      m_wstrCreateDesktopLnk = L"false";
    else if (install_util::CmdlineGetOpt(cmd, L"create-desktop-lnk", &m_wstrCreateDesktopLnk))
      NULL;

    else if (install_util::CmdlineIs(cmd, L"no-create-startup-lnk"))//兼容以前版本
      m_wstrCreateStartupLnk = L"false";
    else if (install_util::CmdlineGetOpt(cmd, L"create-startup-lnk", &m_wstrCreateStartupLnk))
      NULL;

    else if (install_util::CmdlineIs(cmd, L"no-create-startup-menu-lnk"))//兼容以前版本
      m_wstrCreateStartupMenuLnk = L"false";
    else if (install_util::CmdlineGetOpt(cmd, L"create-startup-menu-lnk", &m_wstrCreateStartupMenuLnk))
      NULL;

    else if (install_util::CmdlineIs(cmd, L"no-auto-start"))//兼容以前版本
      m_wstrAutoStart = L"false";
    else if (install_util::CmdlineGetOpt(cmd, L"auto-start", &m_wstrAutoStart))
      NULL;

    else if (install_util::CmdlineGetOpt(cmd, L"url", &m_wstrStartUrl))
      NULL;
    else if (install_util::CmdlineGetOpt(cmd, L"install-path", &m_wstrInstallPath))
      NULL;
    else if (install_util::CmdlineIs(cmd, L"data-report-debug-string"))//兼容以前版本
      m_wstrDataReportDebugString = L"true";
    else if (install_util::CmdlineGetOpt(cmd, L"output-data-report-debug-string", &m_wstrDataReportDebugString))
      NULL;

    else if (install_util::CmdlineIs(cmd, L"set_default"))//兼容以前版本
      m_wstrSetDefault = L"true";
    else if (install_util::CmdlineGetOpt(cmd, L"default_browser", &m_wstrSetDefault))
      NULL;
	else if (install_util::CmdlineGetOpt(cmd, L"mark-product-form", &m_wstrProductForm))
		NULL;
  }
  void AfterSwitch()
  {
    //默认行为
    if(m_bSilent)
    {
      m_bSetDefault = false;
      m_bAutoStart = false;
      m_bCreateDesktopLnk = true;
      m_bCreateStartupLnk = true;
      m_enumCreateStartupMenuLnk = CSMS_TRUE;
      m_bDataReportDebugString = false;
    }
    else
    {
      m_bSetDefault = false;
      m_bAutoStart = true;
      m_bCreateDesktopLnk = true;
      m_bCreateStartupLnk = true;
      m_enumCreateStartupMenuLnk = CSMS_TRUE;
      m_bDataReportDebugString = false;
    }
    //根据命令行参数设置新行为
    SetCommand(m_wstrAutoStart, m_bAutoStart);
    SetCommand(m_wstrCreateDesktopLnk, m_bCreateDesktopLnk);
    SetCommand(m_wstrCreateStartupLnk, m_bCreateStartupLnk);
    SetCommand(m_wstrDataReportDebugString, m_bDataReportDebugString);
    SetCommand(m_wstrSetDefault, m_bSetDefault);

    if(m_wstrCreateStartupMenuLnk == L"true" || m_wstrCreateStartupMenuLnk == L"1")
      m_enumCreateStartupMenuLnk = CSMS_TRUE;
    else if(m_wstrCreateStartupMenuLnk == L"false" || m_wstrCreateStartupMenuLnk == L"0")
      m_enumCreateStartupMenuLnk = CSMS_FALSE;
    else if(m_wstrCreateStartupMenuLnk == L"no_uninst" || m_wstrCreateStartupMenuLnk == L"2")
      m_enumCreateStartupMenuLnk = CSMS_NO_UNINST;
  }
private:
  void SetCommand(const std::wstring &wsCmd, bool &bCmd)
  {
    if(wsCmd == L"true" || wsCmd == L"1")
      bCmd = true;
    else if(wsCmd == L"false" || wsCmd == L"0")
      bCmd = false;
  }
public:
  bool m_bSilent;

  // 安装过程可拆分为“解压缩”和“写注册表”两个步骤
  // m_wstrOnlyUnCompress控制在安装过程中仅对安装包进行解压操作
  // m_wstrOnlyRegister控制在安装过程中仅做写注册表操作,但前提是必须完成解压缩操作
  bool	m_bOnlyUnCompress;
  bool	m_bOnlyRegister;
  // 

  bool m_bAutoStart;
  bool m_bCreateDesktopLnk;
  bool m_bCreateStartupLnk;
  CreateStartupMenuLinkStatu m_enumCreateStartupMenuLnk;
  bool m_bDataReportDebugString;
  bool m_bSetDefault;
private:
  std::wstring m_wstrAutoStart;
  std::wstring m_wstrCreateDesktopLnk;
  std::wstring m_wstrCreateStartupLnk;
  std::wstring m_wstrCreateStartupMenuLnk;
  std::wstring m_wstrDataReportDebugString;
  std::wstring m_wstrSetDefault;

public:
  std::wstring m_wstrStartUrl;
  std::wstring m_wstrInstallPath;
   std::wstring m_wstrProductForm;
};

void InitDataReport(const SetupSwitchs& setupSwitch)
{
  ygdata_report::GetMachineIdTimeout(10 * 1000);
  ygdata_report::AddEidDescList(kEidDescList);
	ygdata_report::Init(L"http://stat.game.yy.com/data.do", "pas", "yyexplorer", 5 * 60 * 1000, NULL, L"", 0, setupSwitch.m_bDataReportDebugString);
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

	info.cha = "yesetup";
	info.rso = "from_yesetup";
	info.ive = STR_PRODUCT_VERSION"."STR_FILE_VERSION;
	info.uve = info.ive;
	info.os = ygdata_report::GetOsPlatformVersion();
	info.lla = "zh_cn";
	info.sre = screen_size;
	info.sco = screen_color;
	ygdata_report::PostSessionInfo(info);

	ygdata_report::SendEventWithSrc("yesetup", "StartUp");
  ygdata_report::SendEventWithSrc("yesetup", "FromyyeSite");
}

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


#define YG_PROCESS_ERROR(condition)\
  do \
{\
  if (!(condition))\
{\
  goto Exit0;\
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

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
  //保证程序是唯一实例
  UniqueApp unique_uninst;
  if(!unique_uninst.Register())
  {
    //窗口激活，前置
    return install_util::ForeGroundInstallWindow() ? EXIT_ERROR_GET_OTHER_INSTANCE_WND : EXIT_ERROR_NOT_GET_OTHER_INSTANCE_WND;
  }

  SetupSwitchs setupSwitch;
  int argc = 0;
  LPWSTR* argv = CommandLineToArgvW(lpstrCmdLine, &argc);
  for (int i = 0; i < argc; ++i)
  {
    setupSwitch.OnSwitch(argv[i]);
  }
  setupSwitch.AfterSwitch();
  InitDataReport(setupSwitch);
  CInstallTmpDir::GetInstance().Init();
  CrcGenerateTable();
  
  {
	  if (!CInstallManager::GetInstance().Init(setupSwitch.m_bOnlyRegister)) //安装前置条件
      goto EXIT_MAIN;
  }
  CInstallManager::GetInstance().OutputDebugStrW(L"安装包StartUp");
  // BLOCK: Run application
  CInstallManager::GetInstance().SetCreateDesktopLnk(setupSwitch.m_bCreateDesktopLnk ? TRUE : FALSE);
  CInstallManager::GetInstance().SetCreateStartupLnk(setupSwitch.m_bCreateStartupLnk ? TRUE : FALSE);
  CInstallManager::GetInstance().SetYYIsDefauleBrowser(setupSwitch.m_bSetDefault ? TRUE : FALSE);
  CInstallManager::GetInstance().SetCreateStartMenuLnk(setupSwitch.m_enumCreateStartupMenuLnk);
  CInstallManager::GetInstance().SetCreateUninstallList(TRUE);
  CInstallManager::GetInstance().SetInstallPath(setupSwitch.m_wstrInstallPath.empty() ? CInstallManager::GetInitInstallPath() : setupSwitch.m_wstrInstallPath);
  if(!setupSwitch.m_wstrStartUrl.empty())CInstallManager::GetInstance().SetRunYYEUrl(setupSwitch.m_wstrStartUrl);
  CInstallManager::GetInstance().SetAutoStartFlag(setupSwitch.m_bAutoStart ? TRUE : FALSE);
  CInstallManager::GetInstance().OutputDebugStrW(L"安装包Set一堆东西完毕");
  if(!setupSwitch.m_wstrProductForm.empty()){
	  CInstallManager::GetInstance().SetProductForm(setupSwitch.m_wstrProductForm);
  }

  int nRet = 0;
  
  if (setupSwitch.m_bOnlyUnCompress)
  {
	  CInstallManager::GetInstance().UnCompressYYExplorer();
	  WaitForSingleObject(CInstallManager::GetInstance().GetUnCompressThreadHandle(), INFINITE);
	  CInstallManager::GetInstance().AfterUnCompressYYExplorer();
  }
  else if (setupSwitch.m_bOnlyRegister) 
  {
	  installType = 1;
	  CInstallManager::GetInstance().RegisterYYExplorer();

	  CInstallManager::GetInstance().CreateStartupLnk2();
	  ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX EID_SUCCEED, ESRC_SILENT_INSTALL);
	  CInstallManager::GetInstance().LaunchYYExplorer(setupSwitch.m_bOnlyRegister);
  } 
  else {
	  if (!setupSwitch.m_bSilent)
	  {
		  GDIPlusFacade::Startup(hInstance);
		  //OutputDebugString(L"安装包 GDIPlusFacade初始化");

		  HRESULT hRes = ::CoInitialize(NULL);
		  // If you are running on NT 4.0 or higher you can use the following call instead to 
		  // make the EXE free threaded. This means that calls come in on a random RPC thread.
		  //  HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		  ATLASSERT(SUCCEEDED(hRes));

		  // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
		  ::DefWindowProc(NULL, 0, 0, 0L);

		  AtlInitCommonControls(ICC_BAR_CLASSES);  // add flags to support other controls

		  hRes = _Module.Init(NULL, hInstance);
		  ATLASSERT(SUCCEEDED(hRes));

		  const wstring& tmpdir = CInstallTmpDir::GetInstance().GetTmpDir();
		  {
			  UncompressHelper helper;
			  if (!helper.UnUncompress7zToDiskSync(_Module.GetModuleInstance(), IDR_MSI_IMAGES, tmpdir.c_str(), NULL))
				  goto EXIT_MAIN;
		  }

		  {
			  CLicenceDialog dlgMain;
			  nRet = dlgMain.DoModal();
		  }

	  EXIT_MAIN:
		  _Module.Term();
		  ::CoUninitialize();

		  GDIPlusFacade::Shutdown();
	  }
	  else//静默安装
	  {
		  installType = 1;

		  ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX EID_STARTUP_LNK, ESRC_SILENT_INSTALL);

		  CInstallManager::GetInstance().InstallYYExplorer(NULL);
		  CInstallManager::GetInstance().WaitForUnzipComplete();
		  WaitForSingleObject(CInstallManager::GetInstance().GetInstallThreadHandle(), INFINITE);
		  CInstallManager::GetInstance().CreateStartupLnk2();
		  ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX EID_SUCCEED, ESRC_SILENT_INSTALL);
		  CInstallManager::GetInstance().LaunchYYExplorer();
	  }
  }

  CInstallManager::GetInstance().UnInit();

  CInstallTmpDir::GetInstance().UnInit();
  ygdata_report::Exit(true);
  if (argv)
  {
	  LocalFree(argv);
	  argv = NULL;
  }
  CInstallManager::GetInstance().OutputDebugStrW(L"安装包EndUp");
  return nRet;
}
