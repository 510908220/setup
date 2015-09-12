#include "stdafx.h"
#include "setup_progress_dialog.h"
#include "install_manager.h"
#include "res_filename.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
#include <sstream>

CSetupProgressDialog::CSetupProgressDialog() :
  m_closeBtnCtrl(CLOSE_NORMAL_FILE, CLOSE_HOVER_FILE, CLOSE_DOWN_FILE, CLOSE_DISABLE_FILE),
  m_progress(SETUP_PROGRESS_FILE, SETUP_PROGRESSBKG_FILE)
{
  m_WndX = 0;
  m_WndY = 0;
}

LRESULT CSetupProgressDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // center the dialog on the screen
  //CenterWindow();

  // set icons
  HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
  SetIcon(hIcon, TRUE);
  HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
  SetIcon(hIconSmall, FALSE);
  //set title
  SetWindowText(INSTALL_TITLE);

  int w = BACKGROUND_SIZE_W + 1;// 460;
  int h = BACKGROUND_SIZE_HIDE_H + 1;//300;
  SetWindowPos(HWND_TOP, m_WndX, m_WndY, w, h, NULL);
  HRGN hrgn = CreateRoundRectRgn(0, 0, w, h, 5, 5);// rounded rect w/50 pixel corners
  SetWindowRgn(hrgn, TRUE);

  BindResControl();

  SetForegroundWindow(m_hWnd);
  CInstallManager::GetInstance().InstallYYExplorer(this);
  //OnProgress(ISP_BACKUP, 100, 50);//test
  //OnProgress(ISP_SETDEFAULT, 100, 100);//test
  //OnProgress(ISP_UNCOMPRESS, 100, 90);//test
  
  return TRUE;
}

void CSetupProgressDialog::SetWndPos(int x,int y)
{
  m_WndX = x;
  m_WndY = y;
}

LRESULT CSetupProgressDialog::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  RECT rect;
  GetWindowRect(&rect);
  BitBlt ((HDC)wParam, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_pLicenceBkg->GetHDC(), 0, 0, SRCCOPY);
  return 0;
}

LRESULT CSetupProgressDialog::OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  HDC hdc =(HDC)wParam;
  HWND hwnd = (HWND)lParam;

  SetBkMode(hdc, TRANSPARENT);
  SetTextColor(hdc, RGB(0, 0, 0));

  return (LRESULT)GetStockObject(HOLLOW_BRUSH);
}

LRESULT CSetupProgressDialog::OnMinSize(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);
  return 0;
}

BOOL CSetupProgressDialog::OnCloseClick()
{
  EndDialog(IDCLOSE);
  return TRUE;
}

BOOL CSetupProgressDialog::BindResControl()
{
  //背景图
  m_pLicenceBkg = new CDialogBackground(m_hWnd);
  m_pLicenceBkg->SetStatu(LS_Progress2);
  //安装文字提示
  HWND hwndInstallingStatic = GetDlgItem(IDC_INSTALLING_TEXT);
  m_installingStaticCtrl.Attach(hwndInstallingStatic);
  m_installingStaticCtrl.SetWindowPos(HWND_BOTTOM, INSTALLING_STATIC_POS_X,
    INSTALLING_STATIC_POS_Y, INSTALLING_STATIC_SIZE_W, INSTALLING_STATIC_SIZE_H, NULL);
  m_installingStaticCtrl.ShowWindow(SW_HIDE);
  //进度条
  m_progress.SubclassWindow(GetDlgItem(IDC_SETUP_PROGRESS));
  m_progress.SetWindowPos(HWND_BOTTOM, PROGRESS_POS_X,
    PROGRESS_POS_Y, PROGRESS_SIZE_W, PROGRESS_SIZE_H,NULL);
  //关闭按钮
  m_closeBtnCtrl.SubclassWindow(GetDlgItem(IDC_BUTTON_CLOSE));
  m_closeBtnCtrl.SetWindowPos(HWND_BOTTOM, CLOSE_BTN_POS_X,
    CLOSE_BTN_POS_Y, CLOSE_BTN_SIZE_W, CLOSE_BTN_SIZE_H,NULL);
  m_closeBtnCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
  m_closeBtnCtrl.EnableWindow(FALSE);
  return TRUE;
}

LRESULT CSetupProgressDialog::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  ReleaseCapture();
  SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
  return 0;
}

bool CSetupProgressDialog::OnProgress(InstallStep step, int nTotle, int nCur)
{
  int preProgress = 0;
  int thisStep = 0;
  preProgress = step;
  switch (step)
  {
  case ISP_BACKUP:
    thisStep = ISP_UNCOMPRESS - ISP_BACKUP; break;
  case ISP_SETDEFAULT:
    thisStep = ISP_CREATELNK - ISP_SETDEFAULT; break;
  case ISP_CREATELNK:
    thisStep = ISP_UNCOMPRESS - ISP_CREATELNK; break;
  case ISP_UNCOMPRESS:
    thisStep = 100 - ISP_UNCOMPRESS; break;
  }

  /*
  //换背景图
  if (step == ISP_SETDEFAULT && nTotle == 100 && nCur == 100)//ON_PROPRESS(ISP_SETDEFAULT, 100, 100);
  {
    m_pLicenceBkg->SetStatu(LS_Progress2);
    m_closeBtnCtrl.SetBkgInfo(m_hWnd,m_pLicenceBkg->GetHDC());
    Invalidate();
  }
  if (step == ISP_UNCOMPRESS && nTotle == 100 && nCur == 90)//ON_PROPRESS(ISP_UNCOMPRESS, 100, 90);
  {
    m_pLicenceBkg->SetStatu(LS_Progress3);
    m_closeBtnCtrl.SetBkgInfo(m_hWnd,m_pLicenceBkg->GetHDC());
    Invalidate();
  }
  */
  //进度条上面的文字
  int currPos = preProgress + (nCur * thisStep) / nTotle;
  //看看进度是不是比之前的小
  int oldPos = m_progress.GetPos();
  if (oldPos > currPos)
    return false;
  //
  std::wstringstream ss;
  ss << INSTALLING_STATIC_MODE << _TEXT(" ") << currPos << _TEXT("%");
  std::wstring installing_str = ss.str();
  SetDlgItemText(IDC_INSTALLING_TEXT,installing_str.c_str());
  //解决背景透明时文字重叠的问题
  RECT rect = {INSTALLING_NUMBER_STATIC_POS_X, INSTALLING_NUMBER_STATIC_POS_Y,
    INSTALLING_NUMBER_STATIC_POS_X + INSTALLING_NUMBER_STATIC_SIZE_W, INSTALLING_NUMBER_STATIC_POS_Y + INSTALLING_NUMBER_STATIC_SIZE_H};
  InvalidateRect(&rect,TRUE);
  //进度条
  m_progress.SetPos(currPos);
  
  if (currPos == 100)
  {
    ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX EID_SUCCEED, ESRC_UI_INSTALL);
    SetTimer(TIMERID_RUN_YYEXPLORER, 300);
  }
  return true;
}

LRESULT CSetupProgressDialog::OnRollLogoTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  return 0;
}

LRESULT CSetupProgressDialog::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  switch(wParam)
  {
    case TIMERID_RUN_YYEXPLORER:
    {
      KillTimer(TIMERID_RUN_YYEXPLORER);
      ShowWindow(SW_HIDE);
      //
      CInstallManager::GetInstance().LaunchYYExplorer();
      //
      EndDialog(IDCLOSE);
      break;
    }
  }
  return 0;
}