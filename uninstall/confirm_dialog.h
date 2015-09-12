#ifndef _LINSENCED_H_
#define _LINSENCED_H_

#include <atlcrack.h>
#include "gdiplus_facade.h"
#include "dialog_background.h"
#include "ui/image_button.h"
#include "ui/image_checkbox.h"

class CConfirmDialog : public CDialogImpl<CConfirmDialog>
{
public:
  CConfirmDialog();

public:
  enum { IDD = IDD_CONFIRM_DIALOG };

///////////////////////////////////////////////////////////////////////////////////////
  BEGIN_MSG_MAP_EX(CConfirmDialog)
//////////////////////////////////////////////////////////////////////////////////////
  MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
  MESSAGE_HANDLER(WM_ERASEBKGND, OnPaint)
  MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColor)
  MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
  MESSAGE_HANDLER(WM_CLOSE, OnDialogClose)

  REFLECT_NOTIFICATIONS()
  END_MSG_MAP()
//////////////////////////////////////////////////////////////////////////////////////
public:
  BOOL OnUnInstallClick();
  BOOL OnCloseClick();

  bool OnClickSlot(CImageButton* button, bool& handled);
  void SetWndPos(int x, int y);//左上角坐标点
private:
  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnDialogClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  
private:
  BOOL BindResControl();
private:
  CImageCheckBox              m_deleteProfileCheckBoxCtrl;
  CImageButton                m_doUnInstallBtnCtrl;
  CDialogBackground*          m_pLicenceBkg;
  CImageButton                m_closeBtnCtrl;
  CStatic                     m_YYExplorerRunningStaticCtrl;  //YY浏览器正在运行，点击“立刻安装”将关闭浏览器并安装
  int                     m_WndX;
  int                     m_WndY;
};

#endif  //! #ifndef _LINSENCED_H_
