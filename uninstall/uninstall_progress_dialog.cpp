#include "stdafx.h"
#include "uninstall_progress_dialog.h"
#include "uninstall_manager.h"
#include "res_pos.h"
#include "res_filename.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
#include <sstream>
#include "feedback_dialog.h"
#include "uninstall_util.h"

CUnInstallProgressDialog::CUnInstallProgressDialog():
m_closeBtnCtrl(IDB_PNG_CLOSE_NORMAL, IDB_PNG_CLOSE_HOVER, IDB_PNG_CLOSE_DOWN, IDB_PNG_CLOSE_NORMAL),
uninstall_complete(IDB_PNG_UNINSTALL_COMPLETE, IDB_PNG_UNINSTALL_COMPLETE_HOVER, IDB_PNG_UNINSTALL_COMPLETE_PRESSED, IDB_PNG_UNINSTALL_COMPLETE),
m_progress(IDB_PNG_PROGRESS, IDB_PNG_PROGERESS_BASE),
have_uninstalled_(false)
{
  m_WndX = 0;
  m_WndY = 0;
}

BOOL CUnInstallProgressDialog::OnCloseClick()
{
  OnUninstallComplete();
  return TRUE;
}

BOOL CUnInstallProgressDialog::OnUninstallComplete()
{
	CloseWindow();
  return TRUE;
}

LRESULT CUnInstallProgressDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // center the dialog on the screen
  CenterWindow();

  // set icons
  HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
  SetIcon(hIcon, TRUE);
  HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
  SetIcon(hIconSmall, FALSE);
  SetWindowText(UNINSTALL_TITLE);

  int w = UNINSTALL_PROGRESS_DLG_SIZE_W + 1;
  int h = UNINSTALL_PROGRESS_DLG_SIZE_H + 1;
  SetWindowPos(HWND_TOP, m_WndX, m_WndY, w, h, NULL);
  //HRGN hrgn = CreateRoundRectRgn(0, 0, w, h, 5, 5);// rounded rect w/50 pixel corners
  //SetWindowRgn(hrgn, TRUE);

  m_pDialogBkg = new CDialogBackground(m_hWnd);
  m_pDialogBkg->SetStatu(LS_Progress);

  BindResControl();

  SetForegroundWindow(m_hWnd);
  CUnInstallManager::GetInstance().UnInstallYYExplorer(this);
  //OnProgress(USP_RESETDEFAULT,100,100);//test
  //OnProgress(USP_DELETEUNINSTINFO,100,100);//test
  
  return TRUE;
}

void CUnInstallProgressDialog::SetWndPos(int x,int y)
{
  m_WndX = x;
  m_WndY = y;
}

LRESULT CUnInstallProgressDialog::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  RECT rect;
  GetWindowRect(&rect);
  BitBlt ((HDC)wParam, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_pDialogBkg->GetHDC(), 0, 0, SRCCOPY);
  return 0;
}

LRESULT CUnInstallProgressDialog::OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  HDC hdc =(HDC)wParam;
  HWND hwnd = (HWND)lParam;

  SetBkMode(hdc, TRANSPARENT);
  //SetTextColor(hdc, RGB(0, 0, 0));
  if(uninstall_util::IsVistaSystem())
    SetTextFontColorSize(hdc,PAGE_PROGRESS_DEFAULT_FONT_COLOR_SIZE_VISTA);
  else
    SetTextFontColorSize(hdc,PAGE_PROGRESS_DEFAULT_FONT_COLOR_SIZE_XP);

  if (hwnd == m_uninstallTitleStaticCtrl.m_hWnd)
  {
    SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
  }
  return (LRESULT)GetStockObject(HOLLOW_BRUSH);
}

void CUnInstallProgressDialog::SetTextFontColorSize(HDC hdc,LPTSTR szFont, COLORREF color,int size)
{
  HFONT hfont = CreateFont(size, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, szFont);
  SelectObject(hdc,hfont);
  SetTextColor(hdc, color);
}

void CUnInstallProgressDialog::CloseWindow(){
	if (have_uninstalled_){
		CUnInstallManager::GetInstance().DeleteUninstFileSelf();
		EndDialog(IDCLOSE);
	}
	
}
BOOL CUnInstallProgressDialog::BindResControl()
{
  //image::DrawImage(LOGO_UNINSTALLING_POS_X,LOGO_UNINSTALLING_POS_Y,m_pDialogBkg->GetHDC(),IDB_PNG_LOGO);
	int width = UNINSTALL_PROGRESS_DLG_SIZE_W;
	int height = UNINSTALL_PROGRESS_DLG_SIZE_H;

	int x = width - UNINSTALL_CLOSE_BTN_SIZE_W - CLOSE_BTN_H_INTERVAL;
	int y = CLOSE_BTN_V_INTERVAL;
	//关闭按钮
	m_closeBtnCtrl.SubclassWindow(GetDlgItem(IDC_PROGRESS_CLOSE));
	m_closeBtnCtrl.SetBkgInfo(m_hWnd, m_pDialogBkg->GetHDC());
	m_closeBtnCtrl.SetWindowPos(HWND_BOTTOM, x,
		y, UNINSTALL_CLOSE_BTN_SIZE_W, UNINSTALL_CLOSE_BTN_SIZE_H, NULL);
	/*m_closeBtnCtrl.EnableWindow(FALSE);*/
	m_closeBtnCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CUnInstallProgressDialog::OnClickSlot));

	int uninstall_text_pos_y = 262;
	y = uninstall_text_pos_y;
	int uninstall_h_interval = 60;
	x = uninstall_h_interval;
	//正在为您卸载YY浏览器n%
	HWND hwndUnistallingStatic = GetDlgItem(IDC_UNINSTALLING_TEXT);
	SetDlgItemText(IDC_UNINSTALLING_TEXT, L"正在卸载中...");
	m_uninstallingStaticCtrl.Attach(hwndUnistallingStatic);
	m_uninstallingStaticCtrl.SetWindowPos(HWND_BOTTOM, x,
		y, UNINSTALLING_SIZE_W, UNINSTALLING_SIZE_H, NULL);

	//进度条
	int progress_bar_v_interval = 20;
	int font_size = 12;
	y += progress_bar_v_interval + font_size;
  m_progress.SubclassWindow(GetDlgItem(IDC_UNINSTALL_PROGRESS));
  m_progress.SetWindowPos(HWND_BOTTOM, x,
	  y, width - 2 * uninstall_h_interval, PROGRESS_SIZE_H, NULL);
  
  //卸载完成按钮
  y = 262;
  uninstall_complete.SubclassWindow(GetDlgItem(IDC_UNINSTALL_COMPLETE));
  uninstall_complete.SetWindowPos(HWND_BOTTOM, (width - UNINSTALL_BUTTON_SIZE_W) / 2, y,
	  UNINSTALL_BUTTON_SIZE_W, UNINSTALL_BUTTON_SIZE_H,NULL);
  uninstall_complete.ShowWindow(SW_HIDE);
  uninstall_complete.SetBkgInfo(m_hWnd, m_pDialogBkg->GetHDC());
  uninstall_complete.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CUnInstallProgressDialog::OnClickSlot));
  //卸载YY浏览器
  /* HWND hwndUnistallTitleStatic = GetDlgItem(IDC_PROGRESS_TITLE);
   m_uninstallTitleStaticCtrl.Attach(hwndUnistallTitleStatic);
   m_uninstallTitleStaticCtrl.SetWindowPos(HWND_BOTTOM, UNINSTALL_TITLE_POS_X,
   UNINSTALL_TITLE_POS_Y, UNINSTALL_TITLE_SIZE_W, UNINSTALL_TITLE_SIZE_H,NULL);
   m_uninstallTitleStaticCtrl.ShowWindow(SW_HIDE);*/
  
  //取消卸载按钮
  //m_cancelBtnCtrl.SubclassWindow(GetDlgItem(IDC_CANCEL));
  //m_cancelBtnCtrl.SetBkgInfo(m_hWnd, m_pDialogBkg->GetHDC());
  //if(uninstall_util::IsVistaSystem())
  //  m_cancelBtnCtrl.SetTextFontColorSize(CANCEL_UNINSTALL_BUTTON_FONT_COLOR_SIZE_VISTA);
  //else
  //  m_cancelBtnCtrl.SetTextFontColorSize(CANCEL_UNINSTALL_BUTTON_FONT_COLOR_SIZE_XP);
  ////m_cancelBtnCtrl.Set9SplitImageInfo(14,14,15,15,0,0,CANCEL_UNINSTALL_BTN_SIZE_W,CANCEL_UNINSTALL_BTN_SIZE_H);
  //m_cancelBtnCtrl.SetWindowPos(HWND_BOTTOM, CANCEL_UNINSTALL_BTN_POS_X,
  //  CANCEL_UNINSTALL_BTN_POS_Y, CANCEL_UNINSTALL_BTN_SIZE_W, CANCEL_UNINSTALL_BTN_SIZE_H,NULL);
  //SetDlgItemText(IDC_CANCEL, L"");
  //m_cancelBtnCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CUnInstallProgressDialog::OnClickSlot));
  //m_cancelBtnCtrl.EnableWindow(FALSE);

  return TRUE;
}

LRESULT CUnInstallProgressDialog::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  ReleaseCapture();
  SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
  return 0;
}

bool CUnInstallProgressDialog::OnProgress(UnInstallStep step, int nTotle, int nCur)
{
  int preProgress = 0;
  int thisStep = 0;
  preProgress = step;
  switch (step)
  {
  case USP_BACKUP:
    thisStep = USP_DELETEFILE - USP_BACKUP; break;
  case USP_DELETEFILE:
    thisStep = USP_DELETELNK - USP_DELETEFILE; break;
  case USP_DELETELNK:
    thisStep = USP_RESETDEFAULT - USP_DELETELNK; break;
  case USP_RESETDEFAULT:
    thisStep = USP_DELETEUNINSTINFO - USP_RESETDEFAULT; break;
  case USP_DELETEUNINSTINFO:
    thisStep = 100 - USP_DELETEUNINSTINFO; break;
  }

  int currPos = preProgress + (nCur * thisStep) / nTotle;
  //看看进度是不是比之前的小
  int oldPos = m_progress.GetPos();//这个有问题，获取到的值老是0
  if (oldPos > currPos)
    return false;
  //进度条上面的文字
  std::wstringstream ss;
  ss << UNINSTALLING_STATIC_MODE << _TEXT(" ") << currPos << _TEXT("%");
  std::wstring installing_str = ss.str();
  //SetDlgItemText(IDC_UNINSTALLING_TEXT,installing_str.c_str());
  //GetDlgItem(IDC_UNINSTALLING_TEXT).GetParent().RedrawWindow();//解决背景透明时文字重叠的问题
  //进度条
  m_progress.SetPos(currPos);
  if (currPos == 100)
  {
    char event_name[MAX_PATH] = {0};
    std::string strPromotionPlan;
    YGW2A(CUnInstallManager::GetInstance().GetPromotionPlan(), strPromotionPlan, CP_UTF8);
    sprintf(event_name, YE_UNINSTALL_PREFIX "Succeed/%s", strPromotionPlan.c_str());
	
	m_uninstallingStaticCtrl.ShowWindow(SW_HIDE);
	m_progress.ShowWindow(SW_HIDE);
	uninstall_complete.ShowWindow(SW_SHOW);
	ygdata_report::SendEventWithSrc(event_name, ESRC_UI_UNINSTALL);
	have_uninstalled_ = true;
    //ygdata_report::SendEventWithSrc(YE_UNINSTALL_PREFIX "Succeed", ESRC_UI_UNINSTALL);
    SetTimer(TIMERID_PROGRESS_COMPLETE, 2000);
  }
  return true;
}

LRESULT CUnInstallProgressDialog::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  KillTimer(TIMERID_PROGRESS_COMPLETE);

  ShowWindow(SW_HIDE);
  //////直接弹url
  //////ShellExecute(NULL,L"open",FEEDBACK_URL,NULL,NULL,SW_SHOWNORMAL);
  ////
  CloseWindow();
  return 0;
}

LRESULT CUnInstallProgressDialog::OnDialogClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){

	CloseWindow();
	return TRUE;
}
bool CUnInstallProgressDialog::OnClickSlot( CImageButton* button, bool& handled )
{
  if (button == &uninstall_complete){
    OnUninstallComplete();
  }else if (button == &m_closeBtnCtrl){
    OnCloseClick();
  }else
    return false;
  return true;
}
