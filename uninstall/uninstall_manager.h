#ifndef UNINSTALL_MANAGER_H_
#define UNINSTALL_MANAGER_H_

#include <string>
#include <windows.h>

using namespace std;

enum UnInstallStep
{
  USP_BACKUP = 0,
  USP_DELETEFILE = 15,
  USP_DELETELNK = 80,
  USP_RESETDEFAULT = 90,
  USP_DELETEUNINSTINFO = 95,
};

class IUnInstallNotify
{
public:
  virtual bool OnProgress(UnInstallStep step, int nTotle, int nCur) = 0;
  virtual ~IUnInstallNotify() {}
};

class CUnInstallManager
{
public:
  static CUnInstallManager& GetInstance();
  BOOL Init();
  BOOL UnInit();
  std::wstring GetPromotionPlan();
public:
  BOOL DeleteDesktopLnk();
  //查找并删除飞机票快捷方式
  BOOL FindAndDeleteFlyLnk(std::wstring pwszDirectory);
  BOOL DeleteQuickTaskBarLnk();
  BOOL DeleteStartMenuLnk();
  BOOL ResetDefautBrowser();
  void ReportMidMd5();
  BOOL UnInstallYYExplorer(IUnInstallNotify* notify);
  BOOL BackupVersion();
  BOOL DeleteVersion();
  BOOL DeleteRegLastVer();
  BOOL RollbackVersion(bool needRollBack = false);
  BOOL SetDeleteProfile(BOOL bCreate);
  BOOL DeleteUninstFileSelf();
public:
  BOOL DoUnInstallYYExplorer();
private:
  BOOL ResetDefautBrowserXp(BOOL& bIsYYExplorerDefault);
  BOOL ResetDefautBrowserVista();
  BOOL DeleteYYExplorerProgId();
  BOOL DeleteProfile();
  wstring GetBrowserProtocalKeyXp(LPCTSTR lpszKey);
  BOOL ReSetDefaultBrowserProtocalKeyXp(LPCTSTR lpszKey);
  BOOL ReSetDefaultBrowserFileKeyXp(LPCTSTR lpszKey);

  BOOL ReSetRegisteredApplicationsVista();
  BOOL ReSetCapabilitiesStartmenuVista();

  BOOL DeleteUninstallList();         //程序和功能列表中移除
  bool MovePath(const std::wstring wsTmpSrcPath, const std::wstring wsTmpDesPath);
  BOOL MoveInstallerFile(std::wstring wsInstallerFilePath,std::wstring wsDstPath);
  BOOL MoveVersionDir(std::wstring wsSrcPath,std::wstring wsDstPath,bool bOnlyInstallerFile = true);
private:
  wstring           m_installPath;
  HANDLE            m_unInstallHandle;
  IUnInstallNotify* m_pUnInstallNotify;
  BOOL              m_bIsDeleteProfile;

  std::wstring      m_wstrPromotionPlan;
};

#endif  //! #ifndef UNINSTALL_MANAGER_H_
