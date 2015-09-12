#include "stdafx.h"
#include "dialog_background.h"
#include "res_filename.h"
#include <string>
#include "install_tmpdir.h"

int h_array[] = //下标用LicenceStatus的枚举值
{
  0,
  BACKGROUND_SIZE_HIDE_H,
  BACKGROUND_SIZE_SHOW_LISENCE_H,
  BACKGROUND_SIZE_SHOW_CUSTOM_H,
  BACKGROUND_SIZE_SHOW_H,
  BACKGROUND_SIZE_SHOW_H,
  BACKGROUND_SIZE_HIDE_H,
  BACKGROUND_SIZE_HIDE_H,
  BACKGROUND_SIZE_HIDE_H
};
std::wstring image_array[] = //下标用LicenceStatus的枚举值
{
  _TEXT(""),
  BACKGROUND_FILE,
  BACKGROUND_SHOW_LISENCE_FILE,
  BACKGROUND_SHOW_CUSTOM_FILE,
  BACKGROUND_SHOW_FILE,
  BACKGROUND_FILE_REAL,
  SETUP_PROGRESS_BACKGROUND_FILE1,
  SETUP_PROGRESS_BACKGROUND_FILE2,
  SETUP_PROGRESS_BACKGROUND_FILE3
};

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
  //int w = rect.right - rect.left;
  //int h = rect.bottom - rect.top;
  int w = BACKGROUND_SIZE_W;
  int h = h_array[statu];
  std::wstring image = image_array[statu];
  
  HDC hdc = GetDC(m_hWnd);
  m_hCacheDc  = CreateCompatibleDC(hdc);
  m_hCacheBitmap = CreateCompatibleBitmap(hdc, w, h);
  SelectObject(m_hCacheDc, m_hCacheBitmap);

  if (!m_pBackgroundImage)
    m_pBackgroundImage = new YGImage();
  m_pBackgroundImage->LoadImage(EXTEND_TO_FULL_PATH(image));

  m_pBackgroundImage->Draw(m_hCacheDc, nX, nY);

  ReleaseDC(m_hWnd, hdc);
  return TRUE;
}

HDC CDialogBackground::GetHDC()
{
  return m_hCacheDc;
}
