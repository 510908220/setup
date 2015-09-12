#ifndef _INSTALL_UTIL_H_
#define _INSTALL_UTIL_H_

#include <string>

using namespace std;

namespace uninstall_util
{
  BOOL IsVistaSystem();
  int  ReplaceString(std::wstring& src_str, const std::wstring& old_str, const std::wstring& new_str);
  LONG GetValueReg(HKEY hMainKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, LPBYTE lpBuf, DWORD dwBufLen);
  LONG SetValueReg(HKEY hMainKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD dwType, LPBYTE lpBuf, DWORD dwBufLen);
  //wsDesktopLnkPath eg: L"\\XTP参考.lnk"
  BOOL GetDesktopDstPath(const std::wstring wsDesktopLnkPath,std::wstring& wsDestPath);
  BOOL MakeSureDirectoryPathExists(LPCWSTR dir);
  BOOL DeleteFile(LPCTSTR pszFile);
  bool PathExists(const wstring& path);
  bool IsFolder(const wstring& path);
  BOOL IsFolderEmpty(LPCTSTR pszFolder);
  BOOL DeleteFolder(LPCTSTR pszFolder);
  int GetProcessCount(const TCHAR* szExeName);
  int CloseProcess(const TCHAR* szExeName);//将关闭所有这个名字的进程
  BOOL SelfDel();
  bool md5(const char* str, int len, std::string& val);
  std::string HttpRequest(char * lpHostName,short sPort,char * lpUrl,char * lpMethod,char * lpPostData,int nPostDataLen);
}
#endif  //! #ifndef _INSTALL_UTIL_H_