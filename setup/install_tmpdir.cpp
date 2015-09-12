#include "stdafx.h"
#include "install_tmpdir.h"
#include "install_util.h"

CInstallTmpDir& CInstallTmpDir::GetInstance()
{
  static CInstallTmpDir instance;
  return instance;
}

BOOL CInstallTmpDir::Init()
{
  wchar_t szTmpPath[MAX_PATH + 1] = {0};
  if (!GetTempPath(MAX_PATH, szTmpPath))
    return FALSE;
  m_tmpPath = szTmpPath;
  wchar_t endChar = m_tmpPath.at(m_tmpPath.length() - 1);
  if (endChar != L'\\' && endChar != L'/')
    m_tmpPath += L"\\";
  m_tmpPath += L"YYExplorer_InstallTmp\\";
  DWORD ret = GetFileAttributes(m_tmpPath.c_str());
  if (ret != INVALID_FILE_ATTRIBUTES)
    install_util::DeleteFolder(m_tmpPath.c_str());
  return CreateDirectory(m_tmpPath.c_str(), NULL);
}

BOOL CInstallTmpDir::UnInit()
{
  return install_util::DeleteFolder(m_tmpPath.c_str());
}

const wstring& CInstallTmpDir::GetTmpDir() const
{
  return m_tmpPath;
}

const wstring CInstallTmpDir::GetCombineFilePath(std::wstring wsFilePathAfterTmp) const
{
  return m_tmpPath + wsFilePathAfterTmp;
}