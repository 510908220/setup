// MainDlg.cpp : implementation of the CLicenceDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "licence_dialog.h"
#include "install_util.h"
#include <shtypes.h>
#include <string>
#include "lang_string.h"
#include <algorithm>
#include "message_dialog.h"
//#include "setup_progress_dialog.h"
#include "movie_dialog.h"
#include "install_manager.h"
#include "install_tmpdir.h"
#include "res_filename.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
#include <ShlObj.h>

using namespace std;

CLicenceDialog::CLicenceDialog()
: m_licenceStaticCtrl(this, 1), m_expLinkStaticCtrl(this, 2)
, m_doInstallBtnCtrl(EXTEND_TO_FULL_PATH(INSTALL_NORMAL_FILE), EXTEND_TO_FULL_PATH(INSTALL_HOVER_FILE), EXTEND_TO_FULL_PATH(INSTALL_DOWN_FILE), EXTEND_TO_FULL_PATH(INSTALL_DISABLE_FILE))
, m_closeBtnCtrl(EXTEND_TO_FULL_PATH(CLOSE_NORMAL_FILE), EXTEND_TO_FULL_PATH(CLOSE_HOVER_FILE), EXTEND_TO_FULL_PATH(CLOSE_DOWN_FILE), EXTEND_TO_FULL_PATH(CLOSE_DISABLE_FILE))
, m_changePathBtnCtrl(EXTEND_TO_FULL_PATH(CHANGEPATH_NORMAL_FILE), EXTEND_TO_FULL_PATH(CHANGEPATH_HOVER_FILE), EXTEND_TO_FULL_PATH(CHANGEPATH_DOWN_FILE), EXTEND_TO_FULL_PATH(CHANGEPATH_DISABLE_FILE))
, m_backButtonCtrl(EXTEND_TO_FULL_PATH(PAGE_CUSTOM_BACK_NORMAL_FILE), EXTEND_TO_FULL_PATH(PAGE_CUSTOM_BACK_HOVER_FILE), EXTEND_TO_FULL_PATH(PAGE_CUSTOM_BACK_DOWN_FILE), EXTEND_TO_FULL_PATH(PAGE_CUSTOM_BACK_DISABLE_FILE))
, m_customButtonCtrl(EXTEND_TO_FULL_PATH(CUSTOM_HIDE_NORMAL_FILE), EXTEND_TO_FULL_PATH(CUSTOM_HIDE_HOVER_FILE), EXTEND_TO_FULL_PATH(CUSTOM_HIDE_DOWN_FILE), EXTEND_TO_FULL_PATH(CUSTOM_HIDE_DISABLE_FILE))
, m_acceptLicenceCheckBoxCtrl(EXTEND_TO_FULL_PATH(CHECK_NORMAL_FILE), EXTEND_TO_FULL_PATH(CHECK_HOVER_FILE), EXTEND_TO_FULL_PATH(CHECK_DWON_FILE), EXTEND_TO_FULL_PATH(CHECK_DISABLE_FILE), EXTEND_TO_FULL_PATH(UNCHECK_NORMAL_FILE), EXTEND_TO_FULL_PATH(UNCHECK_HOVER_FILE), EXTEND_TO_FULL_PATH(UNCHECK_DOWN_FILE), EXTEND_TO_FULL_PATH(UNCHECK_DISABLE_FILE))
, m_shortcutCheckBoxCtrl(EXTEND_TO_FULL_PATH(CHECK_NORMAL_FILE), EXTEND_TO_FULL_PATH(CHECK_HOVER_FILE), EXTEND_TO_FULL_PATH(CHECK_DWON_FILE), EXTEND_TO_FULL_PATH(CHECK_DISABLE_FILE), EXTEND_TO_FULL_PATH(UNCHECK_NORMAL_FILE), EXTEND_TO_FULL_PATH(UNCHECK_HOVER_FILE), EXTEND_TO_FULL_PATH(UNCHECK_DOWN_FILE), EXTEND_TO_FULL_PATH(UNCHECK_DISABLE_FILE))
, m_setDefaultCheckBoxCtrl(EXTEND_TO_FULL_PATH(CHECK_NORMAL_FILE), EXTEND_TO_FULL_PATH(CHECK_HOVER_FILE), EXTEND_TO_FULL_PATH(CHECK_DWON_FILE), EXTEND_TO_FULL_PATH(CHECK_DISABLE_FILE), EXTEND_TO_FULL_PATH(UNCHECK_NORMAL_FILE), EXTEND_TO_FULL_PATH(UNCHECK_HOVER_FILE), EXTEND_TO_FULL_PATH(UNCHECK_DOWN_FILE), EXTEND_TO_FULL_PATH(UNCHECK_DISABLE_FILE))
, m_joinExpCheckBoxCtrl(EXTEND_TO_FULL_PATH(CHECK_NORMAL_FILE), EXTEND_TO_FULL_PATH(CHECK_HOVER_FILE), EXTEND_TO_FULL_PATH(CHECK_DWON_FILE), EXTEND_TO_FULL_PATH(CHECK_DISABLE_FILE), EXTEND_TO_FULL_PATH(UNCHECK_NORMAL_FILE), EXTEND_TO_FULL_PATH(UNCHECK_HOVER_FILE), EXTEND_TO_FULL_PATH(UNCHECK_DOWN_FILE), EXTEND_TO_FULL_PATH(UNCHECK_DISABLE_FILE))
{
  m_pLicenceBkg = NULL;
  m_ImageBkg    = NULL;
  m_logoImageBkg = NULL;
}

CLicenceDialog::~CLicenceDialog()
{
  delete m_pLicenceBkg;
  delete m_ImageBkg;
  delete m_logoImageBkg;
}

BOOL CLicenceDialog::OnInstallClick()
{
  CInstallManager::GetInstance().OutputDebugStrW(L"安装包OnInstallClick");
  WCHAR wsEditPath[1024];
  int char_count = GetDlgItemTextW(IDC_EDIT_PATH, wsEditPath, 1024);
  if(!CheckAndSetFilePathEditText(wsEditPath))
    return FALSE;

  BOOL startup_lnk = m_shortcutCheckBoxCtrl.GetCheckStatu();
  BOOL default_browser = m_setDefaultCheckBoxCtrl.GetCheckStatu();
  
  CInstallManager::GetInstance().SetCreateStartupLnk(startup_lnk);
  CInstallManager::GetInstance().SetYYIsDefauleBrowser(default_browser);

  if (startup_lnk)
    ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX EID_STARTUP_LNK, ESRC_UI_INSTALL);
  if (default_browser)
    ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX EID_DEFAULT_BROWSER, ESRC_UI_INSTALL);

  ShowWindow(SW_HIDE);

	
  CMovieDialog dlgSetupProgress;
	//CSetupProgressDialog dlgSetupProgress;
  //设置安装过程窗口左上角位置与本窗口相同
  RECT rect;
  GetWindowRect(&rect);
  dlgSetupProgress.SetWndPos(rect.left,rect.top);
  //
  dlgSetupProgress.DoModal();
  EndDialog(IDCLOSE);
	
  return TRUE;
}

BOOL CLicenceDialog::OnAcceptLicenceStatuChange(BOOL bCheck)
{
  m_doInstallBtnCtrl.EnableWindow(bCheck);
  m_customButtonCtrl.EnableWindow(bCheck);
  m_customButtonCtrl.SetBtnTextColor(bCheck ? RGB(55,55,55) : RGB(0xB3,0xB3,0xB3));
  return TRUE;
}

LRESULT CLicenceDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // set icons
  HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
  SetIcon(hIcon, TRUE);
  HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
  SetIcon(hIconSmall, FALSE);
  SetWindowText(INSTALL_TITLE);

  m_pLicenceBkg = new CDialogBackground(m_hWnd);

  //HWND hwndInstallLogo = GetDlgItem(IDC_LOGO);
  //m_logoImageBkg = new CImage(hwndInstallLogo);
  int w = BACKGROUND_SIZE_W + 1;// 460;
  int h = BACKGROUND_SIZE_HIDE_H + 1;//300;
  SetWindowPos(HWND_TOP, 0, 0, w, h, NULL);
  //HRGN hrgn = CreateRoundRectRgn(0, 0, w, h, 5, 5);// rounded rect w/50 pixel corners
  //SetWindowRgn(hrgn, TRUE);

  // center the dialog on the screen
  CenterWindow();

  InitFont();
  BindDefaultResControl();

  BindCustomOptionResControl();
  //ShowCustomOption(FALSE,FALSE);
  ShowPage(YYEXPLORER_INSTALL_UI_PAGE_INIT);
  //::UpdateWindow(hwndInstallLogo);

  wstring szInstallPath = CInstallManager::GetInitInstallPath();
  if(!CheckAndSetFilePathEditText(szInstallPath.c_str()))
    EndDialog(IDCLOSE);

  m_joinExpCheckBoxCtrl.SetCheckStatu(FALSE);

  //检测yy浏览器是否正在运行
  if(install_util::GetProcessCount(L"YYExplorer.exe"))
  {
    //m_YYExplorerRunningStaticCtrl.ShowWindow(SW_SHOW);
    CInstallManager::GetInstance().SetIsYYExplorerRunning(TRUE);
  }

  CInstallManager::GetInstance().OutputDebugStrW(L"安装包LicenceDlgShow");
  return TRUE;
}

LRESULT CLicenceDialog::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  RECT rect;
  GetWindowRect(&rect);
  BitBlt ((HDC)wParam, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_pLicenceBkg->GetHDC(), 0, 0, SRCCOPY);
  return 0;
}

LRESULT CLicenceDialog::OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  HDC hdc =(HDC)wParam;
  HWND hwnd = (HWND)lParam;

  SetBkMode(hdc, TRANSPARENT);
  SetTextFontColorSize(hdc, _PAGE_INIT_DEFAULT_FONT_COLOR_SIZE_VISTA, _PAGE_INIT_DEFAULT_FONT_COLOR_SIZE_XP);//这是下面的写法一种改进方案。其他的地方仍然是下面那种，暂时没改成这种。等资源释放出一套好的方案了一并修改
  //if(install_util::IsVistaSystem())
  //  SetTextFontColorSize(hdc,PAGE_INIT_DEFAULT_FONT_COLOR_SIZE_VISTA);
  //else
  //  SetTextFontColorSize(hdc,PAGE_INIT_DEFAULT_FONT_COLOR_SIZE_XP);

  if (hwnd == m_licenceStaticCtrl.m_hWnd || hwnd == m_expLinkStaticCtrl)
  {
    if(install_util::IsVistaSystem())
    {
      HFONT hfont = ::CreateFont(18, 0, 0, 0, FW_NORMAL, 0, TRUE, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _TEXT("Microsoft YaHei"));
      SelectObject(hdc,hfont);
      SetTextColor(hdc, RGB(0x06, 0x79, 0xD7));
    }
    else
    {
      HFONT hfont = ::CreateFont(12, 0, 0, 0, FW_NORMAL, 0, TRUE, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _TEXT("SimSun"));
      SelectObject(hdc,hfont);
      SetTextColor(hdc, RGB(0x06, 0x79, 0xD7));
    }

  }
  if (hwnd == m_YYExplorerRunningStaticCtrl.m_hWnd)
  {
    SetTextColor(hdc, RGB(0xFF, 0x00, 0x00));
  }
  
  if (hwnd == m_licenceTitleStaticCtrl.m_hWnd)
  {
      SetTextFontColorSize(hdc,LICENCE_TITLE_STATIC_FONT_COLOR_SIZE_VISTA, LICENCE_TITLE_STATIC_FONT_COLOR_SIZE_XP);
  }

	if (hwnd == m_installTitleStaticCtrl.m_hWnd || hwnd == m_installPathStaticCtrl.m_hWnd)
	{
			SetTextFontColorSize(hdc,INSTALL_TITLE_STATIC_FONT_COLOR_SIZE_VISTA, INSTALL_TITLE_STATIC_FONT_COLOR_SIZE_XP);
	}
/*
  RECT rect, rect2;
  ::GetWindowRect(hwnd, &rect);
  ::GetWindowRect(m_hWnd, &rect2);
  OffsetRect(&rect, -rect2.left, -rect2.top); //OffsetRect函数将指定的矩形移动到指定的位置

  BitBlt((HDC) wParam, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
    m_pLicenceBkg->GetHDC(), rect.left, rect.top, SRCCOPY);
*/
  return (LRESULT)GetStockObject(HOLLOW_BRUSH);
}

void CLicenceDialog::SetTextFontColorSize(HDC hdc,LPTSTR szFont, COLORREF color,int size)
{
  HFONT hfont = ::CreateFont(size, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, szFont);//注意：字体资源在这里create了并未释放。将来要想个好的方案把它释放掉，其他create字体地方亦如此
  SelectObject(hdc, hfont);
  SetTextColor(hdc, color);
}

void CLicenceDialog::SetTextFontColorSize(HDC hdc,FONT_COLOR_SIZE vista, FONT_COLOR_SIZE xp)
{
  if(install_util::IsVistaSystem())
    SetTextFontColorSize(hdc, vista.szFont, vista.color, vista.nHeight);
  else
    SetTextFontColorSize(hdc, xp.szFont, xp.color, xp.nHeight);
}

LRESULT CLicenceDialog::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  ReleaseCapture();
  SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
  return 0;
}

LRESULT CLicenceDialog::OnDialogClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX "QuitInstall", "CloseDialog");
  EndDialog(IDCLOSE);
  return 0;
}

void CLicenceDialog::InitFont()
{
  install_util::IsVistaSystem() ? InitVistaFont() : InitXpFont();
}

void CLicenceDialog::InitVistaFont()
{
  m_hFontText = ::CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _TEXT("Microsoft YaHei"));
}

void CLicenceDialog::InitXpFont()
{
  m_hFontText = ::CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _TEXT("SimSun"));
}

BOOL CLicenceDialog::BindDefaultResControl()
{
  //bkg
  m_ImageBkg = new YGImage();
  m_ImageBkg->LoadImage(EXTEND_TO_FULL_PATH(BACKGROUND_FILE));
  //logo
  m_logoImageBkg = new YGImage();
  m_logoImageBkg->LoadImage(EXTEND_TO_FULL_PATH(LOGO_FILE));
  //<<最终用户许可协议>>
  m_licenceStaticCtrl.SubclassWindow(GetDlgItem(IDC_LINSENCED_LINK));
  //m_licenceStaticCtrl.SetFont(m_hFontStaticBlue);
  //m_licenceStaticCtrl.MoveWindow(70, 295, 0, 0,TRUE);
  if(install_util::IsVistaSystem())
    m_licenceStaticCtrl.SetWindowPos(HWND_BOTTOM, LICENCE_STATIC_POS_X,
      LICENCE_STATIC_POS_Y, LICENCE_STATIC_SIZE_W, LICENCE_STATIC_SIZE_H, NULL);
  else
    m_licenceStaticCtrl.SetWindowPos(HWND_BOTTOM, LICENCE_STATIC_POS_X,
    LICENCE_STATIC_POS_Y + 2, LICENCE_STATIC_SIZE_W, LICENCE_STATIC_SIZE_H, NULL);

  m_doInstallBtnCtrl.SubclassWindow(GetDlgItem(IDC_INSTALL));
  m_doInstallBtnCtrl.SetWindowPos(HWND_BOTTOM, INSTALL_BUTTON_POS_X, 
    INSTALL_BUTTON_POS_Y, INSTALL_BUTTON_SIZE_W, INSTALL_BUTTON_SIZE_H, NULL);
  //m_doInstallBtnCtrl.SetTextFontColor(m_hFontText, RGB(255,255,255));
  m_doInstallBtnCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CLicenceDialog::OnClickSlot));
  m_doInstallBtnCtrl.bind(CImageButton::EVENT_OnMouseMove, ButtonDelegate(this, &CLicenceDialog::OnMouseMoveSlot));

  m_acceptLicenceCheckBoxCtrl.SubclassWindow(GetDlgItem(IDC_ACCEPT_LINSENCED));
  m_acceptLicenceCheckBoxCtrl.SetWindowPos(HWND_BOTTOM, LICENCE_CHECKBOX_POS_X,
    LICENCE_CHECKBOX_POS_Y, LICENCE_CHECKBOX_SIZE_W, LICENCE_CHECKBOX_SIZE_H,NULL);
  //m_acceptLicenceCheckBoxCtrl.SetFont(m_hFontStaticBlue);
  m_acceptLicenceCheckBoxCtrl.bind(CImageCheckBox::EVENT_OnCheckChange, CheckBoxDelegate(this, &CLicenceDialog::OnCheckSlot));

  m_closeBtnCtrl.SubclassWindow(GetDlgItem(IDC_CLOSE));
  m_closeBtnCtrl.SetWindowPos(HWND_BOTTOM, CLOSE_BTN_POS_X,
    CLOSE_BTN_POS_Y, CLOSE_BTN_SIZE_W, CLOSE_BTN_SIZE_H,NULL);
  m_closeBtnCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CLicenceDialog::OnClickSlot));

  m_customButtonCtrl.SubclassWindow(GetDlgItem(IDC_CUSTOM_OPTION));
  if (install_util::IsVistaSystem())
    m_customButtonCtrl.SetTextFontColorSize(CUSTOM_BUTTON_FONT_COLOR_SIZE_VISTA.szFont, CUSTOM_BUTTON_FONT_COLOR_SIZE_VISTA.color, CUSTOM_BUTTON_FONT_COLOR_SIZE_VISTA.nHeight);
  else
    m_customButtonCtrl.SetTextFontColorSize(CUSTOM_BUTTON_FONT_COLOR_SIZE_XP.szFont, CUSTOM_BUTTON_FONT_COLOR_SIZE_XP.color, CUSTOM_BUTTON_FONT_COLOR_SIZE_XP.nHeight);
  m_customButtonCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CLicenceDialog::OnClickSlot));

  m_customButtonCtrl.Set9SplitImageInfo(14,14,15,15,0,0,CUSTOM_BUTTON_SIZE_W,CUSTOM_BUTTON_SIZE_H);
  m_customButtonCtrl.SetDrawExtraImg(CUSTOM_RIGHT_ARROW_FILE, CUSTOM_RIGHT_ARROW_POS_X, CUSTOM_RIGHT_ARROW_POS_Y);
  m_customButtonCtrl.SetWindowPos(HWND_BOTTOM, CUSTOM_BUTTON_POS_X,
    CUSTOM_BUTTON_POS_Y, CUSTOM_BUTTON_SIZE_W, CUSTOM_BUTTON_SIZE_H,NULL);

  m_backButtonCtrl.SubclassWindow(GetDlgItem(IDC_BACK));
	SetDlgItemText(IDC_BACK,L"");
  m_backButtonCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CLicenceDialog::OnClickSlot));

  m_changePathBtnCtrl.SubclassWindow(GetDlgItem(IDC_CHANGE_PATH));
  m_changePathBtnCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CLicenceDialog::OnClickSlot));
	SetDlgItemText(IDC_CHANGE_PATH,L"");
  /*if (install_util::IsVistaSystem())
    m_changePathBtnCtrl.SetTextFontColorSize(CHANGE_PATH_BTN_FONT_COLOR_SIZE_VISTA);
  else
    m_changePathBtnCtrl.SetTextFontColorSize(CHANGE_PATH_BTN_FONT_COLOR_SIZE_XP);
  m_changePathBtnCtrl.Set9SplitImageInfo(14,14,13,13,0,0,CHANGE_PATH_BTN_SIZE_W,CHANGE_PATH_BTN_SIZE_H);*/

  return True;
}

BOOL CLicenceDialog::BindCustomOptionResControl()
{
  //安装路径编辑框
  HWND hwndInstallPathEdit = GetDlgItem(IDC_EDIT_PATH);
  m_installPathEditCtrl.Attach(hwndInstallPathEdit);
  m_installPathEditCtrl.SetWindowPos(HWND_BOTTOM, INSTALL_PATH_EDIT_POS_X,
    INSTALL_PATH_EDIT_POS_Y, INSTALL_PATH_EDIT_SIZE_W, INSTALL_PATH_EDIT_SIZE_H,NULL);
  m_installPathEditCtrl.EnableWindow(True);
  //去边框
  //long installPathEditStyle = m_installPathEditCtrl.GetWindowLong(GWL_EXSTYLE);
  //m_installPathEditCtrl.SetWindowLong(GWL_STYLE,installPathEditStyle);
  //m_installPathEditCtrl.EnableWindow(FALSE);

  //YY浏览器正在运行，点击“立刻安装”将关闭浏览器并安装
  HWND hwndYYExplorerRunningStatic = GetDlgItem(IDC_YYEXPLORER_RUNNING_STATIC);
  m_YYExplorerRunningStaticCtrl.Attach(hwndYYExplorerRunningStatic);
  m_YYExplorerRunningStaticCtrl.SetWindowPos(HWND_BOTTOM, YYEXPLORER_RUNNING_STATIC_POS_X,
    YYEXPLORER_RUNNING_STATIC_POS_Y, YYEXPLORER_RUNNING_STATIC_SIZE_W, YYEXPLORER_RUNNING_STATIC_SIZE_H,NULL);
  m_YYExplorerRunningStaticCtrl.ShowWindow(SW_HIDE);

  //用户许可协议 标题
  HWND hwndLicenceTitleStatic = GetDlgItem(IDC_LICENCE_TITLE_STATIC);
  m_licenceTitleStaticCtrl.Attach(hwndLicenceTitleStatic);
  m_licenceTitleStaticCtrl.SetWindowPos(HWND_BOTTOM, LICENCE_TITLE_STATIC_POS_X,
    LICENCE_TITLE_STATIC_POS_Y, LICENCE_TITLE_STATIC_SIZE_W, LICENCE_TITLE_STATIC_SIZE_H,NULL);

	//YY浏览器0.1.7beta版 标题
	HWND hwndInstallTitleStatic = GetDlgItem(IDC_TITLE_STATIC);
	m_installTitleStaticCtrl.Attach(hwndInstallTitleStatic);
	m_installTitleStaticCtrl.SetWindowPos(HWND_BOTTOM, INSTALL_TITLE_STATIC_POS_X,
		INSTALL_TITLE_STATIC_POS_Y, INSTALL_TITLE_STATIC_SIZE_W, INSTALL_TITLE_STATIC_SIZE_H,NULL);
  WCHAR szTitle[MAX_PATH];
  wsprintf(szTitle, L"YY浏览器 %s 版", _TEXT(STR_PRODUCT_VERSION));
  SetDlgItemText(IDC_TITLE_STATIC, szTitle);

  //安装到：
  HWND hwndInstallPathStatic = GetDlgItem(IDC_PATH_STATIC);
  m_installPathStaticCtrl.Attach(hwndInstallPathStatic);
  m_installPathStaticCtrl.SetWindowPos(HWND_BOTTOM, INSTALL_PATH_STATIC_POS_X,
    INSTALL_PATH_STATIC_POS_Y, INSTALL_PATH_STATIC_SIZE_W, INSTALL_PATH_STATIC_SIZE_H,NULL);

  //...不建议更改路径..
  HWND hwndNotChangeStatic = GetDlgItem(IDC_NOT_CHANGE_STATIC);
  m_installNotChangeStaticCtrl.Attach(hwndNotChangeStatic);
  m_installNotChangeStaticCtrl.SetWindowPos(HWND_BOTTOM, INSTALL_NOT_CHANGE_STATIC_POS_X,
    INSTALL_NOT_CHANGE_STATIC_POS_Y, INSTALL_NOT_CHANGE_STATIC_SIZE_W, INSTALL_NOT_CHANGE_STATIC_SIZE_H,NULL);
  m_installNotChangeStaticCtrl.ShowWindow(FALSE);

  //设为默认浏览器
  m_setDefaultCheckBoxCtrl.SubclassWindow(GetDlgItem(IDC_SET_DEFAULT));
  m_setDefaultCheckBoxCtrl.SetWindowPos(HWND_BOTTOM, SET_DEFAULT_CHECKBOX_POS_X,
    SET_DEFAULT_CHECKBOX_POS_Y, SET_DEFAULT_CHECKBOX_SIZE_W, SET_DEFAULT_CHECKBOX_SIZE_H,NULL);
  m_setDefaultCheckBoxCtrl.SetCheckStatu(CInstallManager::GetInstance().GetYYIsDefauleBrowser());

  //更改路径
  HWND hwndInstallPathButton = GetDlgItem(IDC_CHANGE_PATH);
  m_installPathButtonCtrl.Attach(hwndInstallPathButton);
  m_installPathButtonCtrl.SetWindowPos(HWND_BOTTOM, CHANGE_PATH_BTN_POS_X,
    CHANGE_PATH_BTN_POS_Y, CHANGE_PATH_BTN_SIZE_W, CHANGE_PATH_BTN_SIZE_H,NULL);

  //加入用户体验计划
  m_joinExpCheckBoxCtrl.SubclassWindow(GetDlgItem(IDC_JOIN_EXP));
  m_joinExpCheckBoxCtrl.SetWindowPos(HWND_BOTTOM, JOIN_EXP_CHECKBOX_POS_X,
    JOIN_EXP_CHECKBOX_POS_Y, JOIN_EXP_CHECKBOX_SIZE_W, JOIN_EXP_CHECKBOX_SIZE_H,NULL);
  m_joinExpCheckBoxCtrl.ShowWindow(SW_HIDE);

  //用户体验计划链接
  m_expLinkStaticCtrl.SubclassWindow(GetDlgItem(IDC_EXP_LINK));
  //m_expLinkStaticCtrl.SetFont(m_hFontStaticBlue);
  m_expLinkStaticCtrl.SetWindowPos(HWND_BOTTOM, JOIN_EXP_STATIC_POS_X,
    JOIN_EXP_STATIC_POS_Y, JOIN_EXP_STATIC_SIZE_W, JOIN_EXP_STATIC_SIZE_H,NULL);
  m_expLinkStaticCtrl.ShowWindow(SW_HIDE);

  //创建快捷方式
  m_shortcutCheckBoxCtrl.SubclassWindow(GetDlgItem(IDC_CREATE_SHORTCAT));
  m_shortcutCheckBoxCtrl.SetWindowPos(HWND_BOTTOM, SHORTCUT_CHECKBOX_POS_X,
    SHORTCUT_CHECKBOX_POS_Y, SHORTCUT_CHECKBOX_SIZE_W, SHORTCUT_CHECKBOX_SIZE_H,NULL);

  m_shortcutCheckBoxCtrl.SetCheckStatu(CInstallManager::GetInstance().GetCreateStartupLnk());
  
  RECT rc = {LICENCE_BROWSER_POS_X, LICENCE_BROWSER_POS_Y, LICENCE_BROWSER_SIZE_W, LICENCE_BROWSER_SIZE_H};
  m_licenceBrowserCtrl.Create(m_hWnd, &rc);
  
  return TRUE;
}

BOOL CLicenceDialog::OnChangePathClick()
{
  ShowBrowserDirDlg();
  return TRUE;
}

BOOL CLicenceDialog::OnCloseClick()
{
  if(nCurrentPage == YYEXPLORER_INSTALL_UI_PAGE_LISENCE)
  {
    ShowPage(YYEXPLORER_INSTALL_UI_PAGE_INIT);
    return TRUE;
  }
  // TODO: Add validation code 
  ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX "QuitInstall", "ClickQuitBtn");
  EndDialog(IDCLOSE);
  return TRUE;
}

BOOL CLicenceDialog::OnCustomClick()
{
  //ShowCustomOption(m_licenseShowStatus,!m_customOptionShowStatus);
  ShowPage(YYEXPLORER_INSTALL_UI_PAGE_CUSTOM);
  return TRUE;
}

bool CLicenceDialog::OnClickSlot( CImageButton* button, bool& handled )
{
  if (button == &m_doInstallBtnCtrl) {
    OnInstallClick();
  } else if (button == &m_closeBtnCtrl) {
    OnCloseClick();
  } else if (button == &m_changePathBtnCtrl) {
    OnChangePathClick();
  } else if (button == &m_customButtonCtrl) {
    OnCustomClick();
  } else if (button == &m_backButtonCtrl) {
    OnBackClick();
  }
  return true;
}

bool CLicenceDialog::OnMouseMoveSlot(CImageButton* button, bool& handled) {
  if (button == &m_doInstallBtnCtrl) {
    static HCURSOR hcur = LoadCursor(NULL, IDC_HAND);
    SetCursor(hcur);
  } 
  
  return true;
}

bool CLicenceDialog::OnCheckSlot(CImageCheckBox* checkbox, bool& handled) {
  if (checkbox == &m_acceptLicenceCheckBoxCtrl) {
    OnAcceptLicenceStatuChange(checkbox->GetCheckStatu());
  }
  return true;
}

BOOL CLicenceDialog::OnBackClick()
{
  //ShowCustomOption(m_licenseShowStatus,!m_customOptionShowStatus);
  ShowPage(YYEXPLORER_INSTALL_UI_PAGE_INIT);
  return TRUE;
}

LRESULT CLicenceDialog::OnMinSize(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);
  return 0;
}

LRESULT CLicenceDialog::OnLinsencedLink(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  //ShowCustomOption(!m_licenseShowStatus,m_customOptionShowStatus);
  ShowPage(YYEXPLORER_INSTALL_UI_PAGE_LISENCE);
  return 0;
}

LRESULT CLicenceDialog::OnCustomOption(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{//改成ImageButton后，这个貌似没用了...如果连续点两下按钮，还是会执行这里。。。
  //ShowCustomOption(!m_customOptionShowStatus);
  return 0;
}

LRESULT CLicenceDialog::OnLicenceLinkSetCursor(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg)
{
  static HCURSOR hcur = LoadCursor(NULL, IDC_HAND);
  if (NULL != hcur)
  {
    SetCursor(hcur);
    return 1;
  }
  SetMsgHandled(false);
  return 0;
}

LRESULT CLicenceDialog::OnExpLinkSetCursor(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg)
{
  static HCURSOR hcur = LoadCursor(NULL, IDC_HAND);
  if (NULL != hcur)
  {
    SetCursor(hcur);
    return 1;
  }
  SetMsgHandled(false);
  return 0;
}

void CLicenceDialog::ShowPage(int nPage)
{
  switch(nPage)
  {
  case YYEXPLORER_INSTALL_UI_PAGE_INIT:
    nCurrentPage = YYEXPLORER_INSTALL_UI_PAGE_INIT;
    //logo
    m_ImageBkg->Draw(m_pLicenceBkg->GetHDC(),0,0);
		m_pLicenceBkg->SetStatu(LS_Normal);
    SetControlsBkgInfo();
    m_logoImageBkg->Draw(m_pLicenceBkg->GetHDC(),PAGE_INIT_LOGO_POS_X,PAGE_INIT_LOGO_POS_Y);
    //一键安装按钮
		SetDlgItemText(IDC_INSTALL,L"");
    /*SetDlgItemText(IDC_INSTALL,L"一键安装");
		if (install_util::IsVistaSystem())
		m_doInstallBtnCtrl.SetTextFontColorSize(INSTALL_BUTTON_FONT_COLOR_SIZE_VISTA);
		else
		m_doInstallBtnCtrl.SetTextFontColorSize(INSTALL_BUTTON_FONT_COLOR_SIZE_XP);
		m_doInstallBtnCtrl.Set9SplitImageInfo(20,20,19,19,0,0,INSTALL_BUTTON_SIZE_W,INSTALL_BUTTON_SIZE_H);*/
    m_doInstallBtnCtrl.SetWindowPos(HWND_BOTTOM, INSTALL_BUTTON_POS_X, 
      INSTALL_BUTTON_POS_Y, INSTALL_BUTTON_SIZE_W, INSTALL_BUTTON_SIZE_H, NULL);
    m_doInstallBtnCtrl.ShowWindow(SW_NORMAL);
    //我已阅读并接受checkbox用户许可协议link
    m_acceptLicenceCheckBoxCtrl.ShowWindow(SW_NORMAL);
    m_licenceStaticCtrl.ShowWindow(SW_NORMAL);
    //自定义安装按钮
    m_customButtonCtrl.ShowWindow(SW_HIDE);

    //标题
		m_installTitleStaticCtrl.ShowWindow(SW_NORMAL);
    m_licenceTitleStaticCtrl.ShowWindow(SW_HIDE);
    //协议内容
    m_licenceBrowserCtrl.ShowWindow(SW_HIDE);
    //返回按钮
    m_backButtonCtrl.ShowWindow(SW_HIDE);

    //更改目录
    m_changePathBtnCtrl.ShowWindow(SW_NORMAL);
    //安装路径编辑框
		m_installPathStaticCtrl.ShowWindow(SW_NORMAL);
    m_installPathEditCtrl.ShowWindow(SW_NORMAL);
    //创建快速启动栏快捷方式checkbox
    m_shortcutCheckBoxCtrl.ShowWindow(SW_HIDE);
    //设置为默认浏览器checkbox
    m_setDefaultCheckBoxCtrl.ShowWindow(SW_HIDE);
    break;
  case YYEXPLORER_INSTALL_UI_PAGE_LISENCE:
  {
    nCurrentPage = YYEXPLORER_INSTALL_UI_PAGE_LISENCE;
    //logo
    m_ImageBkg->Draw(m_pLicenceBkg->GetHDC(),0,0);
		m_pLicenceBkg->SetStatu(LS_ShowLicence);
    SetControlsBkgInfo();
    //一键安装按钮
    m_doInstallBtnCtrl.ShowWindow(SW_HIDE);
    //我已阅读并接受checkbox用户许可协议link
    m_acceptLicenceCheckBoxCtrl.ShowWindow(SW_HIDE);
    m_licenceStaticCtrl.ShowWindow(SW_HIDE);
    //自定义安装按钮
    m_customButtonCtrl.ShowWindow(SW_HIDE);

    //标题
		m_installTitleStaticCtrl.ShowWindow(SW_HIDE);
    m_licenceTitleStaticCtrl.ShowWindow(SW_NORMAL);
    //协议内容
    m_licenceBrowserCtrl.ShowWindow(SW_NORMAL);
    m_licenceBrowserCtrl.SetWindowPos(HWND_BOTTOM, LICENCE_BROWSER_POS_X,
      LICENCE_BROWSER_POS_Y, LICENCE_BROWSER_SIZE_W, LICENCE_BROWSER_SIZE_H,NULL);
    wstring tmpDir = CInstallTmpDir::GetInstance().GetTmpDir();
    wchar_t szFileName[MAX_PATH + 1] = {0};
    _snwprintf_s(szFileName, MAX_PATH, _TRUNCATE, L"%slicence.html", tmpDir.c_str());
    m_licenceBrowserCtrl.Navigate(szFileName, 0, NULL, NULL);
    //返回按钮
		/*
    if (install_util::IsVistaSystem())
      m_backButtonCtrl.SetTextFontColorSize(BACK_BUTTON_PAGE_LICENCE_FONT_COLOR_SIZE_VISTA);
    else
      m_backButtonCtrl.SetTextFontColorSize(BACK_BUTTON_PAGE_LICENCE_FONT_COLOR_SIZE_XP);
    m_backButtonCtrl.Set9SplitImageInfo(14,14,15,15,0,0,BACK_BUTTON_PAGE_LICENCE_SIZE_W,BACK_BUTTON_PAGE_LICENCE_SIZE_H);
    m_backButtonCtrl.ChangeSkin(TRUE);//licence
    m_backButtonCtrl.SetIsDrawExtraImg(FALSE);
		*/
		m_backButtonCtrl.SetWindowPos(HWND_BOTTOM, BACK_BUTTON_PAGE_LICENCE_POS_X,
			BACK_BUTTON_PAGE_LICENCE_POS_Y, BACK_BUTTON_PAGE_LICENCE_SIZE_W, BACK_BUTTON_PAGE_LICENCE_SIZE_H,NULL);
    m_backButtonCtrl.ShowWindow(SW_NORMAL);

    //更改目录
    m_changePathBtnCtrl.ShowWindow(SW_HIDE);
    //安装路径编辑框
		m_installPathStaticCtrl.ShowWindow(SW_HIDE);
    m_installPathEditCtrl.ShowWindow(SW_HIDE);
    //创建快速启动栏快捷方式checkbox
    m_shortcutCheckBoxCtrl.ShowWindow(SW_HIDE);
    //设置为默认浏览器checkbox
    m_setDefaultCheckBoxCtrl.ShowWindow(SW_HIDE);
    break;
  }
  case YYEXPLORER_INSTALL_UI_PAGE_CUSTOM:
    nCurrentPage = YYEXPLORER_INSTALL_UI_PAGE_CUSTOM;
    //logo
    m_ImageBkg->Draw(m_pLicenceBkg->GetHDC(),0,0);
		m_pLicenceBkg->SetStatu(LS_Normal);
    SetControlsBkgInfo();
    m_logoImageBkg->Draw(m_pLicenceBkg->GetHDC(),PAGE_CUSTOM_LOGO_POS_X,PAGE_CUSTOM_LOGO_POS_Y);
    //一键安装按钮
    SetDlgItemText(IDC_INSTALL,L"立即安装");

    if (install_util::IsVistaSystem())
      m_doInstallBtnCtrl.SetTextFontColorSize(PAGE_CUSTOM_INSTALL_BUTTON_FONT_COLOR_SIZE_VISTA.szFont, PAGE_CUSTOM_INSTALL_BUTTON_FONT_COLOR_SIZE_VISTA.color, PAGE_CUSTOM_INSTALL_BUTTON_FONT_COLOR_SIZE_VISTA.nHeight);
    else
      m_doInstallBtnCtrl.SetTextFontColorSize(PAGE_CUSTOM_INSTALL_BUTTON_FONT_COLOR_SIZE_XP.szFont, PAGE_CUSTOM_INSTALL_BUTTON_FONT_COLOR_SIZE_XP.color, PAGE_CUSTOM_INSTALL_BUTTON_FONT_COLOR_SIZE_XP.nHeight);

    m_doInstallBtnCtrl.Set9SplitImageInfo(24,24,19,19,0,0,PAGE_CUSTOM_INSTALL_BUTTON_SIZE_W,PAGE_CUSTOM_INSTALL_BUTTON_SIZE_H);
    m_doInstallBtnCtrl.SetWindowPos(HWND_BOTTOM, PAGE_CUSTOM_INSTALL_BUTTON_POS_X, 
      PAGE_CUSTOM_INSTALL_BUTTON_POS_Y, PAGE_CUSTOM_INSTALL_BUTTON_SIZE_W, PAGE_CUSTOM_INSTALL_BUTTON_SIZE_H, NULL);
    m_doInstallBtnCtrl.ShowWindow(SW_NORMAL);
    //我已阅读并接受checkbox用户许可协议link
    m_acceptLicenceCheckBoxCtrl.ShowWindow(SW_HIDE);
    m_licenceStaticCtrl.ShowWindow(SW_HIDE);
    //自定义安装按钮
    m_customButtonCtrl.ShowWindow(SW_HIDE);

    //用户许可协议 标题
    m_licenceTitleStaticCtrl.ShowWindow(SW_HIDE);
    //协议内容
    m_licenceBrowserCtrl.ShowWindow(SW_HIDE);
    //返回按钮
    SetDlgItemText(IDC_BACK,L"   返回");
    if (install_util::IsVistaSystem())
      m_backButtonCtrl.SetTextFontColorSize(BACK_BUTTON_PAGE_CUSTOM_FONT_COLOR_SIZE_VISTA.szFont, BACK_BUTTON_PAGE_CUSTOM_FONT_COLOR_SIZE_VISTA.color, BACK_BUTTON_PAGE_CUSTOM_FONT_COLOR_SIZE_VISTA.nHeight);
    else
      m_backButtonCtrl.SetTextFontColorSize(BACK_BUTTON_PAGE_CUSTOM_FONT_COLOR_SIZE_XP.szFont, BACK_BUTTON_PAGE_CUSTOM_FONT_COLOR_SIZE_XP.color, BACK_BUTTON_PAGE_CUSTOM_FONT_COLOR_SIZE_XP.nHeight);

    m_backButtonCtrl.Set9SplitImageInfo(14,14,15,15,0,0,BACK_BUTTON_PAGE_CUSTOM_SIZE_W,BACK_BUTTON_PAGE_CUSTOM_SIZE_H);
    m_backButtonCtrl.SetDrawExtraImg(BACK_LEFT_ARROW_FILE, BACK_LEFT_ARROW_POS_X, BACK_LEFT_ARROW_POS_Y);
    m_backButtonCtrl.SetWindowPos(HWND_BOTTOM, BACK_BUTTON_PAGE_CUSTOM_POS_X,
      BACK_BUTTON_PAGE_CUSTOM_POS_Y, BACK_BUTTON_PAGE_CUSTOM_SIZE_W, BACK_BUTTON_PAGE_CUSTOM_SIZE_H,NULL);
    m_backButtonCtrl.ChangeImage(PAGE_CUSTOM_BACK_NORMAL_FILE, PAGE_CUSTOM_BACK_HOVER_FILE, PAGE_CUSTOM_BACK_DOWN_FILE, PAGE_CUSTOM_BACK_DISABLE_FILE);
    m_backButtonCtrl.SetIsDrawExtraImg(TRUE);
    m_backButtonCtrl.ShowWindow(SW_NORMAL);

    //更改目录
    m_changePathBtnCtrl.ShowWindow(SW_NORMAL);
    //安装路径编辑框
    m_installPathEditCtrl.ShowWindow(SW_NORMAL);
    //创建快速启动栏快捷方式checkbox
    m_shortcutCheckBoxCtrl.ShowWindow(SW_NORMAL);
    //设置为默认浏览器checkbox
    m_setDefaultCheckBoxCtrl.ShowWindow(SW_NORMAL);
    break;
  }
  Invalidate();
}

void CLicenceDialog::SetControlsBkgInfo()
{
  //修改背景信息
  m_changePathBtnCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
  m_closeBtnCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
  m_customButtonCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
  m_backButtonCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
  m_doInstallBtnCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
  m_acceptLicenceCheckBoxCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
  m_shortcutCheckBoxCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
  m_setDefaultCheckBoxCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
  m_joinExpCheckBoxCtrl.SetBkgInfo(m_hWnd, m_pLicenceBkg->GetHDC());
}

static int CALLBACK BrowserProc(HWND hwnd, UINT msg, LPARAM lParam, LPARAM lpData) 
{
  switch(msg)
  {   
  case BFFM_INITIALIZED: 
    SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)L"");
    break;
  }   
  return   0;
}

BOOL CLicenceDialog::ShowBrowserDirDlg()
{
  LPITEMIDLIST pidlDesktop = NULL;
  if (SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidlDesktop) != NOERROR)
    return FALSE;

  BROWSEINFO bi = {0};
  bi.hwndOwner = m_hWnd;
  bi.pidlRoot = pidlDesktop;
  _TCHAR wchDisplayName[MAX_PATH] = {0}; 
  bi.pszDisplayName = wchDisplayName;
  bi.lpszTitle = UI_FILE_DIR_INFO;
  bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE; 
  bi.lpfn = BrowserProc;
  bi.lParam = 0;
  bi.iImage = 0;

  _TCHAR wchFileName[MAX_PATH + 1] = {0};
  LPITEMIDLIST pidlCurrent = SHBrowseForFolder(&bi);
  if (pidlCurrent == NULL)
    return FALSE;

  SHGetPathFromIDList(pidlCurrent, wchFileName);
  std::wstring wstrFilePath = wchFileName; 
  if (wstrFilePath.empty())
    return FALSE;

  std::wstring wstrLowercasePath = wstrFilePath;
  //把路径转成小写才能比较，因为微软的路径是不分大小写的，感觉有点别扭
  std::transform(wstrLowercasePath.begin(), wstrLowercasePath.end(), wstrLowercasePath.begin(), tolower);

  if (wstrFilePath.length() < wcslen(L"\\YYExplorer") || wstrLowercasePath.compare(wstrLowercasePath.length() - wcslen(L"\\yyexplorer"), wcslen(L"\\yyexplorer"), L"\\yyexplorer") != 0)
  {
    if (wstrFilePath[wstrFilePath.length() - 1] != L'\\')
      wstrFilePath += L"\\";
    wstrFilePath += L"YYExplorer";
  }

  if (wstrFilePath.length() <= 100)
  {
    CheckAndSetFilePathEditText(wstrFilePath.c_str());
  }
  else
  {
    MessageDlg::MessageBox(m_hWnd, UI_FILE_ERR_PATH_TOOLONG, UI_COMMON_UI_TITLE, MB_OK + MB_ICONERROR);  
    return FALSE;
  }
  return TRUE;
}

BOOL CLicenceDialog::CheckAndSetFilePathEditText(LPCWSTR szText)
{
  if(!CheckTmpDirFreeSpaceAvailable())
  {
    MessageDlg::MessageBox(m_hWnd, UI_FILE_ERR_SPACE_NOT_ENOUGH, UI_COMMON_UI_TITLE, MB_OK + MB_ICONERROR);  
    m_installPathEditCtrl.SetWindowText(CInstallManager::GetInstance().GetInstallPath().c_str());
    return FALSE;
  }
  if(!CheckInstallPathAvailable(szText))
  {
    MessageDlg::MessageBox(m_hWnd, UI_FILE_ERR_INVALID_PATH, UI_COMMON_UI_TITLE, MB_OK + MB_ICONERROR);  
    m_installPathEditCtrl.SetWindowText(CInstallManager::GetInstance().GetInstallPath().c_str());
    return FALSE;
  }
  if(!CheckInstallPathFreeSpaceAvailable(szText))
  {
    MessageDlg::MessageBox(m_hWnd, UI_FILE_ERR_INSTALL_SPACE_NOT_ENOUGH, UI_COMMON_UI_TITLE, MB_OK + MB_ICONERROR);  
    m_installPathEditCtrl.SetWindowText(CInstallManager::GetInstance().GetInstallPath().c_str());
    return FALSE;
  }
  if(!IsSystemAllowedInstallPath(szText))
  {
    MessageDlg::MessageBox(m_hWnd, UI_FILE_ERR_SYS_FORBID_PATH, UI_COMMON_UI_TITLE, MB_OK + MB_ICONERROR);  
    m_installPathEditCtrl.SetWindowText(CInstallManager::GetInstance().GetInstallPath().c_str());
    return FALSE;
  }
  if (wcslen(szText)==0)
  {
    MessageDlg::MessageBox(m_hWnd, UI_FILE_ERR_VOID_PATH, UI_COMMON_UI_TITLE, MB_OK + MB_ICONERROR);  
    m_installPathEditCtrl.SetWindowText(CInstallManager::GetInstance().GetInstallPath().c_str());
    return FALSE;
  }
  wstring wstrFilePath = szText;
  if (wstrFilePath.length() > 100)
  {
    MessageDlg::MessageBox(m_hWnd, UI_FILE_ERR_PATH_TOOLONG, UI_COMMON_UI_TITLE, MB_OK + MB_ICONERROR);  
    return FALSE;
  }
  m_installPathEditCtrl.SetWindowText(szText);
  CInstallManager::GetInstance().SetInstallPath(szText);
  return TRUE;
}
BOOL CLicenceDialog::CheckTmpDirFreeSpaceAvailable()
{
  ULARGE_INTEGER FreeSpace;
  BOOL bRet = ::GetDiskFreeSpaceEx(CInstallTmpDir::GetInstance().GetTmpDir().c_str(), &FreeSpace, NULL, NULL);
  assert(bRet);
  return ((FreeSpace.QuadPart / (ULONGLONG)(1024 * 1024)) > 100) ? TRUE : FALSE;
}
BOOL CLicenceDialog::CheckInstallPathFreeSpaceAvailable(LPCWSTR szUserSetInstallPathText)
{
  ULARGE_INTEGER FreeSpace;
  BOOL bRet = ::GetDiskFreeSpaceEx(szUserSetInstallPathText, &FreeSpace, NULL, NULL);
  assert(bRet);
  return ((FreeSpace.QuadPart / (ULONGLONG)(1024 * 1024)) > 100) ? TRUE : FALSE;
}
BOOL CLicenceDialog::CheckInstallPathAvailable(LPCWSTR szText)
{
  return install_util::MakeSureDirectoryPathExists(szText);
}
BOOL CLicenceDialog::IsSystemAllowedInstallPath(LPCWSTR szText)
{
  if(!install_util::IsVistaSystem())
    return TRUE;

  std::wstring wstrLowercaseText = szText;
  //把路径转成小写才能比较，因为微软的路径是不分大小写的，感觉有点别扭
  std::transform(wstrLowercaseText.begin(), wstrLowercaseText.end(), wstrLowercaseText.begin(), tolower);

  //·Program Files以及子文件夹；
  //·64位系统中的\Program Files (x86)；
  if(wcsstr(wstrLowercaseText.c_str(), L"c:\\program files"))
    return FALSE;

  //·Windows及其子文件夹，包括System32；
  if(wcsstr(wstrLowercaseText.c_str(), L"c:\\windows"))
    return FALSE;
  return TRUE;
}

LRESULT CLicenceDialog::OnShowWnd( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
  ::SetFocus(m_doInstallBtnCtrl.m_hWnd);
  return TRUE;
}
