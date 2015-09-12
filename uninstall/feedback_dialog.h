#ifndef _FEEDBACK_DIALOG_H_
#define _FEEDBACK_DIALOG_H_

#include <atlcrack.h>
#include "gdiplus_facade.h"
#include "dialog_background.h"
#include "ui/image_button.h"
#include "ui/image_checkbox.h"

#include "resource.h"

class CFeedbackDialog : public CDialogImpl<CFeedbackDialog>
{
public:
  CFeedbackDialog();
  
public:
  enum {IDD = IDD_FEEDBACK_DIALOG};
  ///////////////////////////////////////////////////////////////////////////////////////
  BEGIN_MSG_MAP_EX(CConfirmDialog)
  ///////////////////////////////////////////////////////////////////////////////////////
  MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
  MESSAGE_HANDLER(WM_ERASEBKGND, OnPaint)
  MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColor)
  MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
  MESSAGE_HANDLER(WM_CLOSE, OnDialogClose)
  COMMAND_HANDLER(IDC_REASON_EDIT, EN_SETFOCUS, OnEnGetFoucs)
  REFLECT_NOTIFICATIONS()
  END_MSG_MAP()
  //////////////////////////////////////////////////////////////////////////////////////
public:
  BOOL OnCloseClick();
  BOOL OnFeedbackClick();
  BOOL OnUnistallReasonStatuChange(BOOL bCheck);

  bool OnClickSlot( CImageButton* button, bool& handled );
private:
  BOOL BindResControl();
  void SetTextFontColorSize(HDC hdc,LPTSTR szFont, COLORREF color,int size);
private:
  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnDialogClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnEnGetFoucs(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
public:
  void SetWndPos(int x,int y);//���Ͻ������
private:
	void drawRectBorder(HWND window_hwnd, COLORREF color);
private:
  CDialogBackground*           m_pDialogBkg;
  CImageButton                 m_closeBtnCtrl;
  CImageButton                 m_feedBackBtnCtrl;
  CStatic                      m_sorryTitleStaticCtrl;   //�ǳ��ź�... ����
  CEdit                        m_otherReasonEditCtrl;    //���� �༭��
  CStatic                      m_otherReasonStaticCtrl;  //����ԭ��
//  CStatic                     imply_advice_static_;				//����Ⱥ�ŵĽ���
 // CStatic						imply_advice_group_static_;//qqȺ��
//  CStatic                    imply_join_QQ_group_; //����qqȺ
  bool                  have_feedback_num_; //�Ƿ�������
  int                     m_WndX;
  int                     m_WndY;
};

#endif