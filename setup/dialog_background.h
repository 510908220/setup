#ifndef LICENCE_BKG_H_
#define LICENCE_BKG_H_

#include "ui/gdiplus_facade.h"
#include "ui/ygimage.h"

enum LicenceStatus
{
  LS_Invalid,
  LS_Normal,         //下面一条白
  LS_ShowLicence,
  LS_ShowCustom,
  LS_Advance,
  LS_NoBlank,         //无空白
  LS_Progress1,
  LS_Progress2,
  LS_Progress3
};

class CDialogBackground
{
public:
  CDialogBackground(HWND hWnd);
  ~CDialogBackground();
public:
  BOOL SetStatu(DWORD statu);
  HDC GetHDC();
private:
  void ResetResource();
private:
  YGImage*  m_pBackgroundImage;
  DWORD   m_dwStatu;
  HDC     m_hCacheDc;
  HBITMAP m_hCacheBitmap;
  HWND    m_hWnd;
};

#endif  //! #ifndef LICENCE_BKG_H_