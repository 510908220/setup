#include "stdafx.h"
#include "feedback_dialog.h"
#include "res_pos.h"
#include "res_filename.h"
#include "uninstall_manager.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
//#include "../yguicommon/yggdi.h"
//#include "yygame/yguicommon/yggdi/ygpen.h"
#include "uninstall_util.h"
#include "hakari/delegate.h"
#include <vector>
#include <algorithm>

CFeedbackDialog::CFeedbackDialog():
m_closeBtnCtrl(IDB_PNG_CLOSE_GRAY, IDB_PNG_CLOSE_GRAY_HOVER, IDB_PNG_CLOSE_GRAY_PRESSED, IDB_PNG_CLOSE_GRAY),
m_feedBackBtnCtrl(IDB_PNG_FEEDBACK_NORMAL, IDB_PNG_FEEDBACK_HOVER, IDB_PNG_FEEDBACK_DOWN, IDB_PNG_FEEDBACK_NORMAL)
, have_feedback_num_(true)
{
  m_WndX = 0;
  m_WndY = 0;
}

BOOL CFeedbackDialog::OnCloseClick()
{
  EndDialog(IDCLOSE);
  return TRUE;
}

const char* g_event_id_list[] = 
{
	"StartSlow",
	"UpdateAndReinstall",
	"TooManyAds",
	"OpenSomeSiteFailed",
	"NotStable",
	"LackOfFunction",
	"GetBlankPageWhenOpen",
	"NotRespond",
	"OtherReason",
};
BOOL CFeedbackDialog::OnFeedbackClick()
{



 

  wstring wstrOtherReason;
  wstrOtherReason.resize(1024);
  int char_count = GetDlgItemTextW(IDC_REASON_EDIT, &wstrOtherReason[0], wstrOtherReason.length());

  if (char_count <= 0){
	  //进行响应提示
	  drawRectBorder(m_otherReasonEditCtrl.m_hWnd, RGB(255, 0, 0));
	  //
	  m_feedBackBtnCtrl.SetFocus();
	  m_otherReasonStaticCtrl.ShowWindow(SW_HIDE);
	  SetDlgItemText(IDC_OTHER_REASON_STATIC, L"你还没有输入反馈意见哦...");
	  have_feedback_num_ = false;
	  m_otherReasonStaticCtrl.ShowWindow(SW_SHOWNORMAL);
	  return TRUE;
  }

  have_feedback_num_ = true;
 std::string eid = YE_UNINSTALL_PREFIX "OtherReasonContent";
 std::string content;
 if (!YGW2A(wstrOtherReason, content, CP_UTF8))
		 content.clear();
 ygdata_report::SendCustomEvent(eid, GetEidDesc(eid), content);
  EndDialog(IDCLOSE);
  return TRUE;
}

BOOL CFeedbackDialog::OnUnistallReasonStatuChange(BOOL bCheck)
{
  //m_otherReasonEditCtrl.EnableWindow(bCheck);
  return TRUE;
}

LRESULT CFeedbackDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // center the dialog on the screen
  CenterWindow();

  // set icons
  HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
  SetIcon(hIcon, TRUE);
  HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
  SetIcon(hIconSmall, FALSE);
  SetWindowText(UNINSTALL_TITLE);
  //窗口位置、尺寸和圆角
  int w = FEEDBACK_DLG_SIZE_W + 1;
  int h = FEEDBACK_DLG_SIZE_H + 1;
  //设置窗口大小 by zy
  SetWindowPos(HWND_TOP, m_WndX, m_WndY, w, h, NULL);
  //HRGN hrgn = CreateRoundRectRgn(0, 0, w, h, 5, 5);// rounded rect w/50 pixel corners
  //SetWindowRgn(hrgn, TRUE);

  m_pDialogBkg = new CDialogBackground(m_hWnd);
  m_pDialogBkg->SetStatu(LS_Feedback);

  BindResControl();
  SetForegroundWindow(m_hWnd);
  return 0;
}

void CFeedbackDialog::SetWndPos(int x,int y)
{
  m_WndX = x;
  m_WndY = y;
}

LRESULT CFeedbackDialog::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  RECT rect;
  GetWindowRect(&rect);
  BitBlt ((HDC)wParam, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_pDialogBkg->GetHDC(), 0, 0, SRCCOPY);
  //其他 编辑框背景涂白
  //BitBlt ((HDC)wParam, OTHER_REASON_EDIT_POS_X, OTHER_REASON_EDIT_POS_Y, OTHER_REASON_EDIT_SIZE_W, OTHER_REASON_EDIT_SIZE_H, m_pDialogBkg->GetHDC(), 0, 0, WHITENESS);
  COLORREF color;
  if (have_feedback_num_){
	  color = RGB(0xC0, 0xC0, 0xC0);
  }
  else{
	  color = RGB(255, 0, 0);
  }
  drawRectBorder(m_otherReasonEditCtrl.m_hWnd, color);
  return 0;
}

LRESULT CFeedbackDialog::OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  HDC hdc =(HDC)wParam;
  HWND hwnd = (HWND)lParam;

  SetBkMode(hdc, TRANSPARENT);
  if(uninstall_util::IsVistaSystem())
    SetTextFontColorSize(hdc,PAGE_FEEDBACK_DEFAULT_FONT_COLOR_SIZE_VISTA);
  else
    SetTextFontColorSize(hdc,PAGE_FEEDBACK_DEFAULT_FONT_COLOR_SIZE_XP);

  if (hwnd == m_sorryTitleStaticCtrl.m_hWnd)
  {
    if(uninstall_util::IsVistaSystem())
      SetTextFontColorSize(hdc,SORRY_TITLE_STATIC_FONT_COLOR_SIZE_VISTA);
    else
      SetTextFontColorSize(hdc,SORRY_TITLE_STATIC_FONT_COLOR_SIZE_XP);
  }

  //设置加入QQ群讨论的字体
  //
  //if (hwnd == imply_advice_static_.m_hWnd || hwnd == imply_join_QQ_group_.m_hWnd){
	 // if (uninstall_util::IsVistaSystem())
		//  SetTextFontColorSize(hdc, ADVICE_IMPLY_VISTA);
	 // else
		//  SetTextFontColorSize(hdc, ADVICE_IMPLY_XP);
  //}

  //if (hwnd == imply_advice_group_static_){
	 // if (uninstall_util::IsVistaSystem())
		//  SetTextFontColorSize(hdc, ADVICE_IMPLY_GROUP_VISTA);
	 // else
		//  SetTextFontColorSize(hdc, ADVICE_IMPLY_GROUP_XP);
  //}

  if ((hwnd == m_otherReasonStaticCtrl.m_hWnd))
  {
	  COLORREF color;
	  if (have_feedback_num_){
		  color = RGB(0xC0, 0xC0, 0xC0);
	  } else{
		  color = RGB(255, 0, 0);
	  }
	  SetTextColor(hdc, color);
  }


  return (LRESULT)GetStockObject(HOLLOW_BRUSH);
}

void CFeedbackDialog::SetTextFontColorSize(HDC hdc,LPTSTR szFont, COLORREF color,int size)
{
  HFONT hfont = CreateFont(size, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, szFont);
  SelectObject(hdc,hfont);
  SetTextColor(hdc, color);
}

LRESULT CFeedbackDialog::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  ReleaseCapture();
  SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
  return 0;
}
LRESULT CFeedbackDialog::OnDialogClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  EndDialog(IDCLOSE);
  return 0;
}

BOOL CFeedbackDialog::BindResControl()
{

	int width = FEEDBACK_DLG_SIZE_W;
	int height = FEEDBACK_DLG_SIZE_H;
	int x = width - UNINSTALL_CLOSE_BTN_SIZE_W - CLOSE_BTN_H_INTERVAL;
	int y = CLOSE_BTN_V_INTERVAL;
  //关闭按钮
  m_closeBtnCtrl.SubclassWindow(GetDlgItem(IDC_FEEDBACK_CLOSE));
  m_closeBtnCtrl.SetBkgInfo(m_hWnd, m_pDialogBkg->GetHDC());
  m_closeBtnCtrl.SetWindowPos(HWND_BOTTOM, x,
	  y, UNINSTALL_CLOSE_BTN_SIZE_W, UNINSTALL_CLOSE_BTN_SIZE_H, NULL);
  m_closeBtnCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CFeedbackDialog::OnClickSlot));
  //非常遗憾...static
  int h_interval = 40;
  int v_interval = 54;
  x = h_interval;
  y = v_interval;
  HWND hwndSorryTitleStatic = GetDlgItem(IDC_SORRY_STATIC);
  m_sorryTitleStaticCtrl.Attach(hwndSorryTitleStatic);
  m_sorryTitleStaticCtrl.SetWindowPos(HWND_BOTTOM, x,
  y, UNINSTALL_TITLE_SIZE_W, UNINSTALL_TITLE_SIZE_H,NULL);
  //其他 编辑框
  int reson_edit_v_interval = 12;
  y += UNINSTALL_TITLE_SIZE_H + reson_edit_v_interval;
  HWND hwndotherReasonEdit = GetDlgItem(IDC_REASON_EDIT);
  m_otherReasonEditCtrl.Attach(hwndotherReasonEdit);
  m_otherReasonEditCtrl.SetWindowPos(HWND_BOTTOM, x ,
	  y, OTHER_REASON_EDIT_SIZE_W, OTHER_REASON_EDIT_SIZE_H, NULL);
  //其他原因(编辑框内灰字)
  HWND hwndotherReasonStatic = GetDlgItem(IDC_OTHER_REASON_STATIC);
  m_otherReasonStaticCtrl.Attach(hwndotherReasonStatic);
  m_otherReasonStaticCtrl.SetWindowPos(HWND_BOTTOM, x + 5,
	  y , OTHER_REASON_STATIC_SIZE_W, OTHER_REASON_STATIC_SIZE_H, NULL);

  //提交反馈按钮
  m_feedBackBtnCtrl.SubclassWindow(GetDlgItem(IDC_FEEDBACK_BUTTON));
  m_feedBackBtnCtrl.SetBkgInfo(m_hWnd, m_pDialogBkg->GetHDC());
  int feed_back_interval = 25;
  y += OTHER_REASON_EDIT_SIZE_H + feed_back_interval;
  m_feedBackBtnCtrl.SetWindowPos(HWND_BOTTOM, (width - FEEDBACK_BTN_SIZE_W) / 2,
	  y, FEEDBACK_BTN_SIZE_W, FEEDBACK_BTN_SIZE_H, NULL);
  SetDlgItemText(IDC_FEEDBACK_BUTTON, L"");
  m_feedBackBtnCtrl.bind(CImageButton::EVENT_OnButtonClick, ButtonDelegate(this, &CFeedbackDialog::OnClickSlot));
  //
  //imply_join_QQ_group_.Attach(GetDlgItem(IDC_JOIN_QQ));
  //imply_advice_group_static_.Attach(GetDlgItem(IDC_QQ_GROUP_NUM));
  //imply_advice_static_.Attach(GetDlgItem(IDC_JOIN_ADVICE));

  return TRUE;
}

bool CFeedbackDialog::OnClickSlot( CImageButton* button, bool& handled )
{
  if (button == &m_feedBackBtnCtrl){
    OnFeedbackClick();
  }else if (button == &m_closeBtnCtrl){
    OnCloseClick();
  }else
    return false;
  return true;
}

LRESULT CFeedbackDialog::OnEnGetFoucs(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled){

	drawRectBorder(hWndCtl, RGB(0xC0, 0xC0, 0xC0));
	return true;
}
void CFeedbackDialog::drawRectBorder(HWND window_hwnd, COLORREF color){
	RECT edit_rect;
	::GetWindowRect(window_hwnd, &edit_rect);
	ScreenToClient(&edit_rect);
	HDC hdc = GetDC();
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HPEN hOldPen = (HPEN)::SelectObject(hdc, pen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	::Rectangle(hdc, edit_rect.left - 1, edit_rect.top - 1, edit_rect.right + 1, edit_rect.bottom + 1);
	::SelectObject(hdc, hOldPen);
	::SelectObject(hdc, hOldBrush);
	UpdateWindow();
}