#ifndef SETUP_PROGRESS_DIALOG_H_
#define SETUP_PROGRESS_DIALOG_H_

#include <atlcrack.h>
#include "gdiplus_facade.h"
#include "dialog_background.h"
#include "resource.h"
#include "setup_progress.h"
#include "uninstall_manager.h"
#include "ui/image_button.h"
#include "ui/image_checkbox.h"

#define TIMERID_PROGRESS_COMPLETE 1

class CUnInstallProgressDialog : public CDialogImpl<CUnInstallProgressDialog>, public IUnInstallNotify
{
public:
  CUnInstallProgressDialog();

public:
  enum { IDD = IDD_PROGRESS_DIALOG };

  ///////////////////////////////////////////////////////////////////////////////////////
  BEGIN_MSG_MAP_EX(CUnInstallProgressDialog)
  ///////////////////////////////////////////////////////////////////////////////////////
  MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
  MESSAGE_HANDLER(WM_ERASEBKGND, OnPaint)
  MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColor)
  MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
  MESSAGE_HANDLER(WM_CLOSE, OnDialogClose)
  MESSAGE_HANDLER(WM_TIMER, OnTimer)

  /////////////////////////////////////////////////////////////////////////////////////////
  REFLECT_NOTIFICATIONS()
  ///////////////////////////////////////////////////////////////////////////////////////
  END_MSG_MAP()
  //////////////////////////////////////////////////////////////////////////////////////
public:
  BOOL OnCloseClick();
  BOOL OnUninstallComplete();

  bool OnClickSlot(CImageButton* button, bool& handled);
public:
  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnDialogClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
  BOOL BindResControl();
  void SetTextFontColorSize(HDC hdc,LPTSTR szFont, COLORREF color,int size);
  void CloseWindow();
public:
  bool OnProgress(UnInstallStep step, int nTotle, int nCur);
public:
  void SetWndPos(int x,int y);//左上角坐标点
private:
  CDialogBackground*    m_pDialogBkg;
  CSetupProgress        m_progress;
  CImageButton          m_closeBtnCtrl;
  CStatic               m_uninstallTitleStaticCtrl;  //卸载YY浏览器 标题
  CStatic               m_uninstallingStaticCtrl;  //正在为您卸载YY浏览器n%
  CImageButton          uninstall_complete;      //卸载完成
  bool                  have_uninstalled_;             //已经卸载
  int                     m_WndX;
  int                     m_WndY;
};

#endif  //! #ifndef SETUP_PROGRESS_DIALOG_H_
