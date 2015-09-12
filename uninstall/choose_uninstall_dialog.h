/* -------------------------------------------------------------------------
//	FileName	��	choose_uninstall_dialog.h
//	Creator		��	����
//	CreateTime	��	2014/12/17 ������ 17:28:50
//	Description	�������û�ȷ��ж�����������
//
// -----------------------------------------------------------------------*/
#ifndef  __CHORME_YYGAME_UNINSTALL_DIALOG_H
#define __CHORME_YYGAME_UNINSTALL_DIALOG_H

#include "resource.h"
class CImageButton;
class CDialogBackground;
class CImageHandCursor;
class  CChooseUninstallDialog : public CDialogImpl<CChooseUninstallDialog>
{

public:
	enum{ IDD = IDD_DIALOG_CHOOSE_UNINSTALL };
	//////////////////////////////////////////////////////////////////////////
	//message_map
	///////////////////////////////////////////////////////////////////////////////////////
	BEGIN_MSG_MAP_EX(CChooseUninstallDialog)
		//////////////////////////////////////////////////////////////////////////////////////
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnPaint)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColor)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_CLOSE, OnDialogClose)
		MESSAGE_HANDLER(WM_NOTIFY, OnLinkClick)
		REFLECT_NOTIFICATIONS()
		END_MSG_MAP()
public:
	CChooseUninstallDialog();
	~CChooseUninstallDialog();
private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//���Ʊ���
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//����������ɫ
	LRESULT OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//�����϶�
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//�����ϱ�
	LRESULT OnDialogClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//������Ӧʱ
	LRESULT OnLinkClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	//���ڰ󶨰�ť����Ӧ�����Ĺ�ϵ
	void BindControl();
	bool OnSlotClick(CImageButton* button, bool& handled);
	bool OnClickClose();
	bool OnClickHoldYYExplore();
	bool OnClickAdvice();
	bool OnClickUninstall();
	CImageButton* CreateImageButton(int normal_id, int hover_id, int pressed_id,int disable_id, HWND parent_hwnd);
private:
	CImageButton* close_image_button_;
	CImageButton* hold_yyexplore_button_;
	CImageButton* advice_button_;
	CImageHandCursor* uninstall_button_;
	CDialogBackground*  background_;
	//TestLink* test_link_;
};
#endif // 
