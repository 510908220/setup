#ifndef _INSTALL_UTIL_H_
#define _INSTALL_UTIL_H_

#include <string>

using namespace std;

#define YG_PROCESS_ERROR(condition)\
  do \
{\
  if (!(condition))\
{\
  goto Exit0;\
}\
} while (0)

namespace install_util
{
  BOOL Win7OrLater();
  BOOL IsVistaSystem();
  int  ReplaceString(std::wstring& src_str, const std::wstring& old_str, const std::wstring& new_str);
  LONG GetValueReg(HKEY hMainKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, LPBYTE lpBuf, DWORD dwBufLen);
  LONG SetValueReg(HKEY hMainKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, LPBYTE lpBuf, DWORD dwBufLen);
  int CompareVersionString(LPCWSTR szVerL, LPCWSTR szVerR);
  BOOL MakeSureDirectoryPathExists(LPCWSTR dir);
  BOOL CreateLnkPath(std::wstring wsSourceFilePath, std::wstring wsDestLnkPath, std::wstring wsArgument = L"", std::wstring wsAppId = L"");
  bool PathExists(const wstring& path);
  BOOL DeleteFile(LPCTSTR pszFile);
  bool IsFolder(const wstring& path);
  BOOL IsFolderEmpty(LPCTSTR pszFolder);
  BOOL DeleteFolder(LPCTSTR pszFolder);
  //BOOL IsProcessExist(std::wstring wsProcessName);
  int GetProcessCount(const TCHAR* szExeName);
  void NotifyProcessExit(const TCHAR* szExeName);

  // 进程退出则true，超时则false
  bool WaitProcessExit(const TCHAR* szExeName, int nMilliseconds);
  int KillProcess(const TCHAR* szExeName);//将关闭所有这个名字的进程
  std::wstring GetFileProductVersion(std::wstring wsFilePath);
  BOOL ForeGroundInstallWindow();
  bool CmdlineGetOpt(LPCTSTR argv, const std::wstring& opt_name, std::wstring* opt_value);
  bool CmdlineIs(LPCTSTR argv, const std::wstring& opt_name);
}
#endif  //! #ifndef _INSTALL_UTIL_H_