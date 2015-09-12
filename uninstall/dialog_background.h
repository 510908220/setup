#ifndef LICENCE_BKG_H_
#define LICENCE_BKG_H_

#include "ui/gdiplus_facade.h"
#include "ui/ygimage.h"

enum LicenceStatus
{
  LS_Invalid,
  LS_ChooseUninstall,
  LS_Confirm ,
  LS_Progress ,
  LS_Feedback ,
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