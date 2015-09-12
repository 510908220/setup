#ifndef INSTALL_MANAGER_H_
#define INSTALL_MANAGER_H_

#include <Windows.h>
#include <string>
#include <vector>
#include "7z_uncompress_helper.h"

using namespace std;

enum CreateStartupMenuLinkStatu{
  CSMS_TRUE,
  CSMS_FALSE,
  CSMS_NO_UNINST
};

class CInstallManager
{
public:
  static CInstallManager& GetInstance();
  BOOL Init(bool isOnlyRegister);
  BOOL UnInit();
public:
  void SetAutoStartFlag(BOOL bAutoStart);
  static std::wstring GetInitInstallPath();
  static std::wstring GetDefaultInstallPath();
  static std::wstring GetLastInstallPath();
  static std::wstring GetDefaultRunYYEUrl();
  void SetCreateDesktopLnk(BOOL bCreate);
  void SetCreateStartupLnk(BOOL bCreate);
  void SetYYIsDefauleBrowser(BOOL bDefault);
  void SetCreateStartMenuLnk(CreateStartupMenuLinkStatu enumCreate);
  void SetRunYYEUrl(std::wstring url);

  BOOL GetCreateDesktopLnk()    { return m_bIsCreateDesktopLnk; }
  BOOL GetCreateStartupLnk()    { return m_bIsCreateStartupLnk; }
  CreateStartupMenuLinkStatu GetCreateStartMenuLnk()  { return m_enumCreateStartMenuLnk; }
  BOOL GetYYIsDefauleBrowser()  { return m_bYYIsDefaultBrowser; }

  void SetCreateUninstallList(BOOL bCreate);
  void SetIsYYExplorerRunning(BOOL bRunning);
  void OutputDebugStrW(std::wstring str);
  void OutputDebugStrA(std::string str);
public:
  BOOL SetInstallPath(const std::wstring& installPath);
  std::wstring GetInstallPath() const;
  HANDLE GetInstallThreadHandle() const;
  HANDLE GetUnCompressThreadHandle() const;
  BOOL CleanMUICache();
  BOOL CreateDesktopLnk();
  BOOL CreateStartMenuLnk();
  BOOL CreateStartupLnk();
  BOOL DefaultBrowserYY();
  BOOL InstallYYExplorer(IInstallNotify* notify);
  BOOL UnRegisterNotify(IInstallNotify* notify);
  BOOL LaunchYYExplorer(bool isOnlyRegister = false);
  BOOL FindOldVersionDir();
  BOOL BackupVersion();
  BOOL RollbackVersion(bool needRollBack);
  BOOL RunExe();
  BOOL RunAsDesktopUser(const wchar_t * pPath, const wchar_t * pCmdLine);

  // 解压
  BOOL UnCompressYYExplorer();
  // 写注册表
  VOID RegisterYYExplorer();

private:
  BOOL SetCapabilitiesKeyVista();
  BOOL DefaultBrowserYYVista();
  BOOL DefaultBrowserYYXP();
  static BOOL IsInstallOlder();
  BOOL SetDefaultBrowserProgId();
  BOOL SetDefaultBrowserProtocalKeyXp(LPCTSTR lpszKey);
  BOOL SetDefaultBrowserFileKeyXp(LPCTSTR lpszKey);
  BOOL SetRegisteredApplicationsVista();
  BOOL SetCapabilitiesStartmenuVista();
  BOOL SetCapabilitiesProtocalKeyVista(LPCTSTR lpszKey);
  BOOL SetCapabilitiesFileKeyVista(LPCTSTR lpszKey);
  BOOL SetUninstallList();      
  bool MovePath(const std::wstring wsTmpSrcPath, const std::wstring wsTmpDesPath);
  BOOL MoveInstallerFile(std::wstring wsInstallerFilePath,std::wstring wsDstPath);
  BOOL MoveOldVersion(std::wstring wsSrcPath,std::wstring wsDstPath,bool onlyInstallerFile/* = true*/);
public:
  BOOL UnZipLevel0();
  BOOL UnZipLevel0_1();
  BOOL UnZipLevel0_2();
  BOOL DoInstallYYExplorer();
  BOOL WaitForUnzipComplete();
  BOOL CreateStartupLnk2();
  BOOL TellInstlauncherProgress(InstallStep step, int nTotle, int nCur);

  // 
  BOOL DoUnCompressYYExplorer();
  VOID AfterUnCompressYYExplorer();
public:
	void SetProductForm(std::wstring form);
	BOOL RemoveProductForm();
public:
  BOOL ModifyProcessPriority(DWORD pId);//对进程提权
  BOOL GetProcessIdsFromName(std::wstring mProcessName, std::vector<DWORD> &pIds);//根据进程名获取进程id

private:
  std::wstring        m_installPath;
  std::wstring        m_oldInstallPath;
  std::wstring        m_oldVersion;
  HANDLE              m_installHandle;

  HANDLE              m_unZipHandle0;
  HANDLE              m_unZipHandle1;
  HANDLE              m_unZipHandle2;

  BOOL                m_bAutoStart;
  BOOL                m_bIsCreateDesktopLnk;
  BOOL                m_bIsCreateStartupLnk;
  CreateStartupMenuLinkStatu                m_enumCreateStartMenuLnk;
  BOOL                m_bIsCreateUnistallList;
  BOOL                m_bYYIsDefaultBrowser;
  IInstallNotify*     m_pInstallNotify;
  BOOL                m_bIsYYExplorerRunning;
  std::wstring        m_wstrRunYYEUrl;

  bool                m_bCanOptDbg;
  std::wstring		  m_wstrProductForm;

private:
  HANDLE			  m_hUnCompressHandle;
};

#endif  //! #ifndef INSTALL_MANAGER_H_
