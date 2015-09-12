#include "yg_7z_uncompress_helper.h"
#include "7z\7zFile.h"
#include "7z\7zCrc.h"
#include "7z\7zAlloc.h"
#include "npyygameutils/common/coder/yg_coder.h"
#include <shlobj.h>

YG7zUncompressHelper::YG7zUncompressHelper()
{

}

bool YG7zUncompressHelper::UnUncompress7zToDiskSync(const wstring& szPak, 
	const wstring& szDstFolder, IInstallNotify* notify)
{
  int nRet = 0;  
	m_dstFolder = szDstFolder;
	wchar_t endChar = m_dstFolder.at(m_dstFolder.length() - 1);
	if (endChar != L'\\' && endChar != L'/')
		m_dstFolder += L"\\";

  CFileInStream archiveStream = { 0 };  
  CLookToRead* lookStream = new CLookToRead();
  CSzArEx db = { 0 };  
  UInt16 *temp = NULL;  
  size_t tempSize = 0;  

  ////////////////////////////////////////////////////////////  
  // init  

  ISzAlloc alloc;
  alloc.Alloc = SzAlloc;
  alloc.Free = SzFree;
  ISzAlloc allocTemp;
  allocTemp.Alloc = SzAllocTemp;
  allocTemp.Free = SzFreeTemp;
	std::string szPakA = YGCoder::WStringToUtf8(szPak);
  nRet = InFile_Open(&archiveStream.file, szPakA.c_str());  
  if (0 != nRet)
  {
    delete lookStream;
    return false;
  }

  FileInStream_CreateVTable(&archiveStream);  
  LookToRead_CreateVTable(lookStream, False);  

  lookStream->realStream = &archiveStream.s;  
  LookToRead_Init(lookStream);  
  
  //by kxy
  CrcGenerateTable();

  SzArEx_Init(&db);
  SRes res = SzArEx_Open(&db, &lookStream->s, &alloc, &allocTemp);
  if (res != SZ_OK)
  {
    File_Close(&archiveStream.file);
    delete lookStream;
    return false;
  }

  UInt32 blockIndex = 0xFFFFFFFF; 
  Byte *outBuffer = 0; 
  size_t outBufferSize = 0;

  bool bRet = true;
  for (UInt32 i = 0; i < db.db.NumFiles; ++i)
  {
    if (notify)
      notify->OnProgress(ISP_UNCOMPRESS, db.db.NumFiles, i);
    const CSzFileItem *f = db.db.Files + i;
    bRet &= (f->IsDir == 0 ? UncompressFile(i, lookStream, &db, &blockIndex, &outBuffer, &outBufferSize, &alloc, &allocTemp) : UncompressDir(i, lookStream, &db));
  }

  if (notify)
    notify->OnProgress(ISP_UNCOMPRESS, db.db.NumFiles, db.db.NumFiles);

  IAlloc_Free(&alloc, outBuffer);
  SzArEx_Free(&db, &alloc);
  File_Close(&archiveStream.file);
  delete lookStream;
  return bRet;
}

bool YG7zUncompressHelper::DoUncompress7z(CFileInResource *archiveStream, IInstallNotify* notify)
{
  ISzAlloc alloc;
  alloc.Alloc = SzAlloc;
  alloc.Free = SzFree;
  ISzAlloc allocTemp;
  allocTemp.Alloc = SzAllocTemp;
  allocTemp.Free = SzFreeTemp;

  CLookToRead* lookStream = new CLookToRead();
  LookToRead_CreateVTable(lookStream, TRUE);
  lookStream->realStream = &(archiveStream->s);
  LookToRead_Init(lookStream);

  CSzArEx db;
  SzArEx_Init(&db);
  SRes res = SzArEx_Open(&db, &lookStream->s, &alloc, &allocTemp);
  if (res != SZ_OK)
  {
    delete lookStream;
    return false;
  }

  UInt32 blockIndex = 0xFFFFFFFF; 
  Byte *outBuffer = 0; 
  size_t outBufferSize = 0;

  bool bRet = true;
  for (UInt32 i = 0; i < db.db.NumFiles; ++i)
  {
    if (notify)
      notify->OnProgress(ISP_UNCOMPRESS, db.db.NumFiles, i);
    const CSzFileItem *f = db.db.Files + i;
    bRet &= (f->IsDir == 0 ? UncompressFile(i, lookStream, &db, &blockIndex, &outBuffer, &outBufferSize, &alloc, &allocTemp) : UncompressDir(i, lookStream, &db));
  }

  if (notify)
    notify->OnProgress(ISP_UNCOMPRESS, db.db.NumFiles, db.db.NumFiles);

  IAlloc_Free(&alloc, outBuffer);
  SzArEx_Free(&db, &alloc);
  delete lookStream;
  return bRet;
}

bool YG7zUncompressHelper::UncompressDir(UInt32 index, CLookToRead* lookStream, CSzArEx* db)
{
  UInt16 fileName[MAX_PATH] = {0};
  SzArEx_GetFileNameUtf16(db, index, fileName);
  if (!MakePathDirectory((wchar_t*)fileName))
    return false;

  std::wstring fullPath = m_dstFolder + std::wstring((wchar_t*)fileName);

  return !!MakeSureDirectoryPathExists(fullPath.c_str());
}

bool YG7zUncompressHelper::UncompressFile(UInt32 index, CLookToRead* lookStream, CSzArEx* db, UInt32* blockIndex, Byte** outBuffer, size_t* outBufferSize, ISzAlloc *allocMain, ISzAlloc *allocTemp)
{
  UInt16 fileName[MAX_PATH] = {0};
  SzArEx_GetFileNameUtf16(db, index, fileName);
  MakePathDirectory((wchar_t*)fileName);

  size_t offset = 0;
  size_t outSizeProcessed = 0;
  SRes res = SzArEx_Extract(db, &(lookStream->s), index, blockIndex, outBuffer, outBufferSize, &offset, &outSizeProcessed, allocMain, allocTemp);
  if (res != SZ_OK)
    return false;

  CSzFile outFile;
  std::wstring fullPath = m_dstFolder + wstring((wchar_t*)fileName);
  OutFile_OpenW(&outFile, fullPath.c_str());
  size_t processedSize = outSizeProcessed;

  bool bOk = (File_Write(&outFile, (*outBuffer) + offset, &processedSize) == 0 && processedSize == outSizeProcessed);
  File_Close(&outFile);
  return bOk;
}

bool YG7zUncompressHelper::MakePathDirectory(wchar_t* szFileName)
{
  for (size_t j = 0; szFileName[j] != 0; j++)
  {
    if (szFileName[j] != L'/' && szFileName[j] != L'\\')
      continue;
    wchar_t separator = szFileName[j];
    szFileName[j] = 0;
    std::wstring fullPath = m_dstFolder + szFileName;
    if (!MakeSureDirectoryPathExists(fullPath.c_str()))
      return false;
    szFileName[j] = separator;
  }
  return true;
}

bool YG7zUncompressHelper::MakeSureDirectoryPathExists(LPCWSTR dir)
{
	DWORD ret = GetFileAttributes(dir);
	if (ret == INVALID_FILE_ATTRIBUTES)
	{
		int ret = ::SHCreateDirectoryExW(NULL,dir,NULL);
		if (ret == ERROR_SUCCESS || ret == ERROR_ALREADY_EXISTS || ret == ERROR_FILE_EXISTS)
			return true;
		return false;
	}
	return true;
}

