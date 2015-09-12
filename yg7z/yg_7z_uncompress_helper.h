#ifndef _7Z_UNCOMPRESS_HELPER_H_
#define _7Z_UNCOMPRESS_HELPER_H_

#include <string>
#include "7z/7zFile.h"
#include "7z/Types.h"
#include "7z/7z.h"

using namespace std;

enum InstallStep
{
  ISP_BACKUP = 0,
  ISP_CREATELNK = 8,
  ISP_SETDEFAULT = 16,
  ISP_UNCOMPRESS = 25,
};

class IInstallNotify
{
public:
  virtual bool OnProgress(InstallStep step, int nTotle, int nCur) = 0;
  virtual ~IInstallNotify() {}
};

class YG7zUncompressHelper
{
public:
  YG7zUncompressHelper();
public:
  bool UnUncompress7zToDiskSync(const wstring& szPak, const wstring& szDstFolder, IInstallNotify* notify);
private:
  bool DoUncompress7z(CFileInResource *archiveStream, IInstallNotify* notify);
  bool UncompressDir(UInt32 index, CLookToRead* lookStream, CSzArEx* db);
  bool UncompressFile(UInt32 index, CLookToRead* lookStream, CSzArEx* db, UInt32* blockIndex, Byte** outBuffer, size_t* outBufferSize, ISzAlloc *allocMain, ISzAlloc *allocTemp);
private:
  bool MakePathDirectory(wchar_t* szFileName);
	bool MakeSureDirectoryPathExists(LPCWSTR dir);
private:
  wstring             m_dstFolder;
};

#endif  //! #ifndef _7Z_UNCOMPRESS_HELPER_H_