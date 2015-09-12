#include "stdafx.h"
#include "choose_uninstall_dialog.h"
#include "ui/image_button.h"
#include "yygame/uninstall/dialog_background.h"
#include "hakari/delegate.h"
#include "yygame/uninstall/feedback_dialog.h"
#include "res_pos.h"
#include "res_filename.h"
#include "yygame/uninstall/confirm_dialog.h"
#include "yygame/uninstall/CImageHandCursors.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
CChooseUninstallDialog::CChooseUninstallDialog():
close_image_button_(NULL),
hold_yyexplore_button_(NULL),
advice_button_(NULL),
uninstall_button_(NULL),
background_(NULL){

}
CChooseUninstallDialog::~CChooseUninstallDialog(){
	if (close_image_button_){
		delete close_image_button_;
		close_image_button_ = NULL;
	}
	if (hold_yyexplore_button_){
		delete hold_yyexplore_button_;
		hold_yyexplore_button_ = NULL;
	}
	if (background_){
		delete background_;
		background_ = NULL;
	}
	if (advice_button_){
		delete advice_button_;
		advice_button_ = NULL;
	}
	if (uninstall_button_){
		delete uninstall_button_;
		uninstall_button_ = NULL;
	}

}
LRESULT CChooseUninstallDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/){
	
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);
	SetWindowText(UNINSTALL_TITLE);

	int w = BACKGROUND_SIZE_W + 1;
	int h = BACKGROUND_SIZE_HIDE_H + 1;
	SetWindowPos(HWND_TOP, 0, 0, w, h, SWP_NOMOVE);

	background_ = new CDialogBackground(m_hWnd);
	background_->SetStatu(LS_ChooseUninstall);
	//关闭按钮
	close_image_button_ = CreateImageButton(
		IDB_PNG_CLOSE_NORMAL
		, IDB_PNG_CLOSE_HOVER
		, IDB_PNG_CLOSE_DOWN
		, IDB_PNG_CLOSE_NORMAL
		, GetDlgItem(IDC_BUTTON_CLOSE));
	int close_button_h_interval = 10;
	int close_button_v_interval = 4;
	close_image_button_->SetWindowPos(HWND_TOP, w - close_button_h_interval - UNINSTALL_CLOSE_BTN_SIZE_W, close_button_v_interval, UNINSTALL_CLOSE_BTN_SIZE_W, UNINSTALL_CLOSE_BTN_SIZE_H, SWP_SHOWWINDOW);
	close_image_button_->SetBkgInfo(m_hWnd, background_->GetHDC());
	//建议按钮
	advice_button_ = CreateImageButton(
		 IDB_PNG_ADVICE
		 , IDB_PNG_ADVICE_HOVER
		 , IDB_PNG_ADVICE_PRESSED
		 , IDB_PNG_ADVICE
		 , GetDlgItem(IDC_BUTTON_ADVICE));
	int advice_button_h_interval = 124;
	int advice_button_v_interval = 46;
	int button_y = h - advice_button_v_interval - FEEDBACK_BTN_SIZE_H;
	advice_button_->SetWindowPos(HWND_BOTTOM
																	, w - advice_button_h_interval - FEEDBACK_BTN_SIZE_W
																	, button_y
																	, FEEDBACK_BTN_SIZE_W
																	, FEEDBACK_BTN_SIZE_H
																	, NULL);
	advice_button_->SetBkgInfo(m_hWnd, background_->GetHDC());

	 //保留按钮
	hold_yyexplore_button_ = CreateImageButton(
			IDB_PNG_HOLD_YYEXPLORE
		 , IDB_PNG_HOLD_YYEXPLORE_HOVER
		 , IDB_PNG_HOLD_YYEXPLORE_PRESSED
		 , IDB_PNG_HOLD_YYEXPLORE
		 , GetDlgItem(IDC_BUTTON_HOLD_EXPLORE));
	hold_yyexplore_button_->SetWindowPos(HWND_TOP
																					, advice_button_h_interval
																					, button_y
																					, FEEDBACK_BTN_SIZE_W
																					, FEEDBACK_BTN_SIZE_H
																					, SWP_SHOWWINDOW);
	hold_yyexplore_button_->SetBkgInfo(m_hWnd, background_->GetHDC());

	int uninstall_button_interval = 10;
	uninstall_button_ = new CImageHandCursor(IDB_PNG_LINK_BG, IDB_PNG_LINK_BG, IDB_PNG_LINK_BG, IDB_PNG_LINK_BG);
	uninstall_button_->SubclassWindow(GetDlgItem(IDC_CHOOSE_BUTTON_UNINSTALL));
	uninstall_button_->SetBkgInfo(m_hWnd, background_->GetHDC());
	uninstall_button_->SetBtnTextColor(RGB(0x33, 0x99, 0xff));
	HFONT m_hfont = ::CreateFont(14, 0, 0, 0, FW_NORMAL, 0, true, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"");
	uninstall_button_->SetFont(m_hfont);
	int uninstall_button_width = 80;
	int uninstall_button_height = 15;
	uninstall_button_->SetWindowPos(HWND_TOP
		, w - advice_button_h_interval - uninstall_button_width
		, button_y + FEEDBACK_BTN_SIZE_H + uninstall_button_interval
		, uninstall_button_width
		, uninstall_button_height
		, SWP_SHOWWINDOW);
	 BindControl();
	 return true;
}
CImageButton* CChooseUninstallDialog::CreateImageButton(int normal_id, int hover_id, int pressed_id, int disable_id, HWND parent_hwnd){
	CImageButton* image_button = new CImageButton(normal_id, hover_id, pressed_id, disable_id);
	image_button->SubclassWindow(parent_hwnd);
	return image_button;
}
//绘制背景
LRESULT CChooseUninstallDialog::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
	RECT rect;
	GetWindowRect(&rect);
	BitBlt((HDC)wParam, 0, 0, rect.right - rect.left, rect.bottom - rect.top, background_->GetHDC(), 0, 0, SRCCOPY);
	return 0;
}
//设置文字颜色
LRESULT CChooseUninstallDialog::OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
	//HWND hwnd = (HWND)lParam;

	//if (hwnd == GetDlgItem(IDC_SYSLINK_UNINSTALL)){

	//	HDC hdc =::GetDC(hwnd);
	//	SetTextColor(hdc, RGB(255, 0, 0));
	//}
	return true;
}
//用于拖动
LRESULT CChooseUninstallDialog::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
	ReleaseCapture();
	SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
	return 0;
}
//用于上报
LRESULT CChooseUninstallDialog::OnDialogClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){

	EndDialog(IDCLOSE);
	return true;
}

LRESULT CChooseUninstallDialog::OnLinkClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
	switch (((LPNMHDR)lParam)->code)
	{
	case  NM_CLICK:
	case  NM_RETURN:{
						/*if (((LPNMHDR)lParam)->hwndFrom == GetDlgItem(IDC_SYSLINK_UNINSTALL)){
							ShowWindow(SW_HIDE);
							CConfirmDialog* dialog = new CConfirmDialog();
							dialog->DoModal();
							EndDialog(IDCLOSE);
						}*/
						break;
	}
	default:
		return FALSE;
	}

	return TRUE;
}
void CChooseUninstallDialog::BindControl(){
	if  (close_image_button_){
		close_image_button_->bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CChooseUninstallDialog::OnSlotClick));
	}

	if (hold_yyexplore_button_){
		hold_yyexplore_button_->bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CChooseUninstallDialog::OnSlotClick));
	}

	if (advice_button_){
		advice_button_->bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CChooseUninstallDialog::OnSlotClick));
	}
	if (uninstall_button_){
		uninstall_button_->bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CChooseUninstallDialog::OnSlotClick));

	}
}
bool CChooseUninstallDialog::OnSlotClick(CImageButton* button, bool& handled){
	if  (button == close_image_button_)
		return OnClickClose();

	if  (button == advice_button_)
		return OnClickAdvice();

	if  (button == hold_yyexplore_button_)
		return OnClickHoldYYExplore();
	if (button == uninstall_button_){
		return OnClickUninstall();
	}
	return false;
}
bool CChooseUninstallDialog::OnClickClose(){
	EndDialog(IDCLOSE);
	return true;
}
bool CChooseUninstallDialog::OnClickHoldYYExplore(){

	//取消卸载
	ygdata_report::SendEventWithSrc("yeuninstall/newdata", "cancel");
	EndDialog(IDCLOSE);
	return true;
}
bool CChooseUninstallDialog::OnClickAdvice(){
	ShowWindow(SW_HIDE);
	ygdata_report::SendEventWithSrc("yeuninstall/newdata", "talk");
	CFeedbackDialog* feed_back_dialog  = new CFeedbackDialog();
	RECT rect;
	GetWindowRect(&rect);
	feed_back_dialog->SetWndPos(rect.left,rect.top);
	feed_back_dialog->DoModal();
	EndDialog(IDCLOSE);
	return true;
}
bool CChooseUninstallDialog::OnClickUninstall(){
	ShowWindow(SW_HIDE);
	ygdata_report::SendEventWithSrc("yeuninstall/newdata", "getdelete");
	CConfirmDialog* dialog = new CConfirmDialog();
	RECT rect;
	GetWindowRect(&rect);
	dialog->SetWndPos(rect.left, rect.top);
	dialog->DoModal();
	EndDialog(IDCLOSE);
	return true;
}