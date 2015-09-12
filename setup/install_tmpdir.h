#ifndef _INSTALL_TMP_DIR_
#define _INSTALL_TMP_DIR_

#include <string>

using namespace std;

class CInstallTmpDir
{
public:
  static CInstallTmpDir& GetInstance();
public:
  BOOL Init();
  BOOL UnInit();
public:
  const wstring& GetTmpDir() const;
  const wstring GetCombineFilePath(std::wstring wsFilePathAfterTmp) const;
private:
  wstring m_tmpPath;
};

#define EXTEND_TO_FULL_PATH(fpath) CInstallTmpDir::GetInstance().GetCombineFilePath(fpath).c_str()
//#define EXTEND_TO_FULL_PATH(fpath) (CInstallTmpDir::GetInstance().GetTmpDir()+fpath).c_str()

#endif  //! #ifndef _INSTALL_TMP_DIR_
