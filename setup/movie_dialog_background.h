#ifndef MOVIE_DLG_BKG_H_
#define MOVIE_DLG_BKG_H_

#include "ui/gdiplus_facade.h"
#include "ui/ygimage.h"

class CMovieDialogBackground
{
public:
  CMovieDialogBackground(HWND hWnd);
  ~CMovieDialogBackground();
public:
  BOOL SetFrame(int nFrame);
  HDC GetHDC();
private:
  void ResetResource();
private:
  YGImage*  m_pBackgroundImage;
  int     m_nFrame;
  HDC     m_hCacheDc;
  HBITMAP m_hCacheBitmap;
  HWND    m_hWnd;
};

#endif  //! #ifndef LICENCE_BKG_H_