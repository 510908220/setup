#include "stdafx.h"
#include "dialog_background.h"
#include "res_filename.h"
#include "resource.h"
#include "res_pos.h"

CDialogBackground::CDialogBackground(HWND hWnd)
{
  m_hWnd = hWnd;
  m_pBackgroundImage = NULL;
  m_hCacheDc = NULL;
  m_hCacheBitmap = NULL;
  m_dwStatu = 0;
}

CDialogBackground::~CDialogBackground()
{
  if(m_pBackgroundImage)
  {
    delete m_pBackgroundImage;
    m_pBackgroundImage = NULL;
  }
}

void CDialogBackground::ResetResource()
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

BOOL CDialogBackground::SetStatu(DWORD statu)
{
  if (m_dwStatu == statu)
    return TRUE;

  m_dwStatu = statu;

  ResetResource();

  int nX = 0;
  int nY = 0;
  RECT rect;
  GetWindowRect(m_hWnd, &rect);
  int w = rect.right - rect.left - 1;
  int h = rect.bottom - rect.top - 1;
  
  HDC hdc = GetDC(m_hWnd);
  m_hCacheDc	= CreateCompatibleDC(hdc);
  m_hCacheBitmap = CreateCompatibleBitmap(hdc, w, h);
  SelectObject(m_hCacheDc, m_hCacheBitmap);
  int image_id = 0;
  switch (m_dwStatu)
  {
  case  LS_ChooseUninstall:{
							   image_id = IDB_PNG_CHOOSE_UNINSTALL_BACKGROUND;
							   break;
  }
  case  LS_Confirm:{
					   image_id = IDB_PNG_UNINSTALL_BG;
					   break;
  }
  case  LS_Progress:{
						image_id = IDB_PNG_UNINSTALLING_BG;
						break;
  }
  case  LS_Feedback:{
						image_id = IDB_PNG_BKG_FEEDBACK;
						break;

  }
  default:
	  return FALSE;
  }
  if (!m_pBackgroundImage)
  	m_pBackgroundImage = new YGImage();
  m_pBackgroundImage->LoadImage(image_id);

  m_pBackgroundImage->Draw(m_hCacheDc, nX, nY);

  ReleaseDC(m_hWnd, hdc);
  return TRUE;
}

HDC CDialogBackground::GetHDC()
{
  return m_hCacheDc;
}
