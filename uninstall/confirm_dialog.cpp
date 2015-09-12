// MainDlg.cpp : implementation of the CLicenceDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "confirm_dialog.h"
#include <shtypes.h>
#include <string>
#include <algorithm>
#include "uninstall_progress_dialog.h"
#include "res_pos.h"
#include "res_filename.h"
#include "uninstall_util.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
#include "hakari/delegate.h"

using namespace std;

CConfirmDialog::CConfirmDialog():
m_closeBtnCtrl(IDB_PNG_CLOSE_NORMAL, IDB_PNG_CLOSE_HOVER, IDB_PNG_CLOSE_DOWN, IDB_PNG_CLOSE_NORMAL),
m_doUnInstallBtnCtrl(IDB_PNG_UNINSTALL_NORMAL, IDB_PNG_UNINSTALL_HOVER, IDB_PNG_UNINSTALL_DOWN, IDB_PNG_UNINSTALL_NORMAL),
m_deleteProfileCheckBoxCtrl(IDB_PNG_CHECK_NORMAL, IDB_PNG_CHECK_DOWN, IDB_PNG_CHECK_DOWN, IDB_PNG_CHECK_NORMAL
                            ,IDB_PNG_UNCHECK_NORMAL, IDB_PNG_UNCHECK_DOWN, IDB_PNG_UNCHECK_DOWN, IDB_PNG_UNCHECK_NORMAL),
							m_WndX(0),
							m_WndY(0)
{
  m_pLicenceBkg = NULL;
}

BOOL CConfirmDialog::OnUnInstallClick()
{
  BOOL delete_profile = m_deleteProfileCheckBoxCtrl.GetCheckStatu();
  ygdata_report::SendEventWithSrc("yeuninstall/newdata", "delete");
  if (delete_profile)
	  ygdata_report::SendEventWithSrc(YE_UNINSTALL_PREFIX "DeleteProfile", ESRC_UI_UNINSTALL);
  CUnInstallManager::GetInstance().SetDeleteProfile(delete_profile);
  ShowWindow(SW_HIDE);
  CUnInstallProgressDialog dlgUnInstallProgress;
  //设置卸载过程窗口左上角位置与本窗口相同
  RECT rect;
  GetWindowRect(&rect);
  dlgUnInstallProgress.SetWndPos(rect.left,rect.top);
  //
  dlgUnInstallProgress.DoModal();
  EndDialog(IDCLOSE);
  return TRUE;
}

BOOL CConfirmDialog::OnCloseClick()
{
  ygdata_report::SendEventWithSrc(YE_UNINSTALL_PREFIX EID_QUIT_UNINSTALL, "ClickQuitBtn");
  EndDialog(IDCLOSE);
  return TRUE;
}

bool CConfirmDialog::OnClickSlot( CImageButton* button, bool& handled )
{
  if(button == &m_doUnInstallBtnCtrl){
    OnUnInstallClick();
  }else if(button == &m_closeBtnCtrl){
    OnCloseClick();
  }else
    return false;
  return true;
}

void CConfirmDialog::SetWndPos(int x, int y){
	m_WndX = x;
	m_WndY = y;
}
LRESULT CConfirmDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // center the dialog on the screen

  // set icons
  HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
  SetIcon(hIcon, TRUE);
  HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
  SetIcon(hIconSmall, FALSE);
  SetWindowText(UNINSTALL_TITLE);

  int w = BACKGROUND_SIZE_W + 1;
  int h = BACKGROUND_SIZE_HIDE_H + 1;
  SetWindowPos(HWND_TOP, m_WndX, m_WndY, w, h, NULL);
  //HRGN hrgn = CreateRoundRectRgn(0, 0, w, h, 5, 5);// rounded rect w/50 pixel corners
  //SetWindowRgn(hrgn, TRUE);

  m_pLicenceBkg = new CDialogBackground(m_hWnd);
  m_pLicenceBkg->SetStatu(LS_Confirm);

  BindResControl();
 
  //检测yy浏览器是否正在运行
  if(uninstall_util::GetProcessCount(L"YYExplorer.exe"))
  {
    m_YYExplorerRunningStaticCtrl.ShowWindow(SW_HIDE);
  }

  return TRUE;
}

LRESULT CConfirmDialog::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  RECT rect;
  GetWindowRect(&rect);
  BitBlt ((HDC)wParam, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_pLicenceBkg->GetHDC(), 0, 0, SRCCOPY);
  return 0;
}

LRESULT CConfirmDialog::OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  HDC hdc =(HDC)wParam;
  HWND hwnd = (HWND)lParam;

  SetBkMode(hdc, TRANSPARENT);
  SetTextColor(hdc, RGB(0, 0, 0));

  if (hwnd == m_YYExplorerRunningStaticCtrl.m_hWnd)
  {
    SetTextColor(hdc, RGB(0xFF, 0x00, 0x00));
  }
  return (LRESULT)GetStockObject(HOLLOW_BRUSH);
}

LRESULT CConfirmDialog::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  ReleaseCapture();
  SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
  return 0;
}

LRESULT CConfirmDialog::OnDialogClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  ygdata_report::SendEventWithSrc(YE_UNINSTALL_PREFIX EID_QUIT_UNINSTALL, "CloseDialog");
  EndDialog(IDCLOSE);
  return 0;
}

BOOL CConfirmDialog::BindResControl()
{
  //image::DrawImage(LOGO_POS_X,LOGO_POS_Y,m_pLicenceBkg->GetHDC(),IDB_PNG_LOGO);

	int width = BACKGROUND_SIZE_W;
	int height = BACKGROUND_SIZE_HIDE_H;

	//关闭按钮
	int x = width - CLOSE_BTN_H_INTERVAL - UNINSTALL_CLOSE_BTN_SIZE_W;
	int y = CLOSE_BTN_V_INTERVAL;
	m_closeBtnCtrl.SubclassWindow(GetDlgItem(IDC_CONFIRM_CLOSE));
	m_closeBtnCtrl.SetWindowPos(HWND_BOTTOM, x,
		y, UNINSTALL_CLOSE_BTN_SIZE_W, UNINSTALL_CLOSE_BTN_SIZE_H, NULL);
	m_closeBtnCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
	m_closeBtnCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CConfirmDialog::OnClickSlot));

	//一键删除按钮
	y = 262;
	m_doUnInstallBtnCtrl.SubclassWindow(GetDlgItem(IDC_BUTTON_UNINSTALL));
	m_doUnInstallBtnCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
	if (uninstall_util::IsVistaSystem())
		m_doUnInstallBtnCtrl.SetTextFontColorSize(UNINSTALL_BUTTON_FONT_COLOR_SIZE_VISTA);
	else
		m_doUnInstallBtnCtrl.SetTextFontColorSize(UNINSTALL_BUTTON_FONT_COLOR_SIZE_XP);
	//m_doUnInstallBtnCtrl.Set9SplitImageInfo(20,20,19,19,0,0,UNINSTALL_BUTTON_SIZE_W,UNINSTALL_BUTTON_SIZE_H);
	m_doUnInstallBtnCtrl.SetWindowPos(HWND_BOTTOM, (width - UNINSTALL_BUTTON_SIZE_W) / 2,  
		y, UNINSTALL_BUTTON_SIZE_W, UNINSTALL_BUTTON_SIZE_H, NULL);
	SetDlgItemText(IDC_BUTTON_UNINSTALL, L"");
	m_doUnInstallBtnCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CConfirmDialog::OnClickSlot));

	//这个的布局
  m_deleteProfileCheckBoxCtrl.SubclassWindow(GetDlgItem(IDC_CHECK_DELETE_PROFILE));
  m_deleteProfileCheckBoxCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
  m_deleteProfileCheckBoxCtrl.SetCheckboxTextColor(RGB(0x3d,0x3d,0x3d));
  int delete_profile_h_interval = 20;
  int delete_profile_v_interval = 16;
  m_deleteProfileCheckBoxCtrl.SetWindowPos(HWND_BOTTOM, delete_profile_h_interval,
    height - delete_profile_h_interval - DELETE_PROFILE_CHECKBOX_SIZE_H, DELETE_PROFILE_CHECKBOX_SIZE_W, DELETE_PROFILE_CHECKBOX_SIZE_H,NULL);
  m_deleteProfileCheckBoxCtrl.SetCheckStatu(FALSE);

 

  
  
  //YY浏览器正在运行，点击“立刻安装”将关闭浏览器并安装?
  HWND hwndYYExplorerRunningStatic = GetDlgItem(IDC_YYEXPLORER_RUNNING_STATIC);
  m_YYExplorerRunningStaticCtrl.Attach(hwndYYExplorerRunningStatic);
  m_YYExplorerRunningStaticCtrl.SetWindowPos(HWND_BOTTOM, YYEXPLORER_RUNNING_STATIC_POS_X,
    YYEXPLORER_RUNNING_STATIC_POS_Y, YYEXPLORER_RUNNING_STATIC_SIZE_W, YYEXPLORER_RUNNING_STATIC_SIZE_H,NULL);
  m_YYExplorerRunningStaticCtrl.ShowWindow(SW_HIDE);

  return True;
}