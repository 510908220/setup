#include "stdafx.h"
#include "movie_dialog_background.h"
#include "res_filename.h"
#include <string>
#include "install_tmpdir.h"

namespace CMovieDlgBkg{
  const int LEN_IMAGE_ARRAY = 67;
}

CMovieDialogBackground::CMovieDialogBackground(HWND hWnd)
{
  m_hWnd = hWnd;
  m_pBackgroundImage = NULL;
  m_hCacheDc = NULL;
  m_hCacheBitmap = NULL;
  m_nFrame = -1;
}

CMovieDialogBackground::~CMovieDialogBackground()
{
  if(m_pBackgroundImage)
  {
    delete m_pBackgroundImage;
    m_pBackgroundImage = NULL;
  }
}

void CMovieDialogBackground::ResetResource()
{
  if (m_hCacheDc)
  {
    DeleteObject(m_hCacheDc);
    m_hCacheDc = NULL;
  }
  if (m_hCacheBitmap)
  {
    DeleteObject(m_hCacheBitmap);
    m_hCacheBitmap = NULL;
  }
  if(m_pBackgroundImage)
  {
    delete m_pBackgroundImage;
    m_pBackgroundImage = NULL;
  }
}

BOOL CMovieDialogBackground::SetFrame(int nFrame)
{
  if (m_nFrame == nFrame)
    return TRUE;

  m_nFrame = nFrame % CMovieDlgBkg::LEN_IMAGE_ARRAY;

  ResetResource();

  int nX = 0;
  int nY = 0;
  RECT rect;
  GetWindowRect(m_hWnd, &rect);
  //int w = rect.right - rect.left;
  //int h = rect.bottom - rect.top;
  int w = MOVIE_SIZE_W;
  int h = MOVIE_SIZE_H;
  //std::wstring image = CMovieDlgBkg::image_array[m_nFrame];
  WCHAR image[MAX_PATH] = {0};
  wsprintf(image, L"movie\\%d.png",m_nFrame);
  
  HDC hdc = GetDC(m_hWnd);
  m_hCacheDc  = CreateCompatibleDC(hdc);
  m_hCacheBitmap = CreateCompatibleBitmap(hdc, w, h);
  SelectObject(m_hCacheDc, m_hCacheBitmap);

  if (!m_pBackgroundImage)
    m_pBackgroundImage = new YGImage();
  m_pBackgroundImage->LoadImage(EXTEND_TO_FULL_PATH(image));

  m_pBackgroundImage->Draw(m_hCacheDc, nX, nY);
	
	//ÉèÖÃÍ¸Ã÷´°¿Ú-------------------------------------------------
	CPoint DestPt(0,0);
	CSize psize(w,h);
	BLENDFUNCTION blendFunc32bpp;
	blendFunc32bpp.AlphaFormat = AC_SRC_ALPHA;
	blendFunc32bpp.BlendFlags = 0;
	blendFunc32bpp.BlendOp = AC_SRC_OVER;
	blendFunc32bpp.SourceConstantAlpha = 255;
	::UpdateLayeredWindow(m_hWnd,hdc,NULL,&psize,m_hCacheDc,&DestPt,0,&blendFunc32bpp,ULW_ALPHA);

  ReleaseDC(m_hWnd, hdc);
	return TRUE;
}

HDC CMovieDialogBackground::GetHDC()
{
  return m_hCacheDc;
}
