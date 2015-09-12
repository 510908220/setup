#ifndef SETUP_PROGRESS_DIALOG_H_
#define SETUP_PROGRESS_DIALOG_H_

#include <atlcrack.h>
#include "ui/gdiplus_facade.h"
#include "dialog_background.h"
#include "resource.h"
#include "setup_progress.h"
#include "7z_uncompress_helper.h"
#include "image_button.h"

#define TIMERID_RUN_YYEXPLORER 1

class CSetupProgressDialog : public CDialogImpl<CSetupProgressDialog>, public IInstallNotify
{
public:
  CSetupProgressDialog();

public:
  enum { IDD = IDD_SETUP_PROGRESS };

  ///////////////////////////////////////////////////////////////////////////////////////
  BEGIN_MSG_MAP_EX(CSetupProgressDialog)
  ///////////////////////////////////////////////////////////////////////////////////////
  MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
  MESSAGE_HANDLER(WM_ERASEBKGND, OnPaint)
  MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColor)
  MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
  MESSAGE_HANDLER(WM_TIMER, OnTimer)
  
  /////////////////////////////////////////////////////////////////////////////////////////
  COMMAND_ID_HANDLER(IDC_MINSIZE, OnMinSize)
  REFLECT_NOTIFICATIONS()
  ///////////////////////////////////////////////////////////////////////////////////////
  END_MSG_MAP()
  //////////////////////////////////////////////////////////////////////////////////////
public:
  BOOL OnCloseClick();
public:
  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnRollLogoTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  LRESULT OnMinSize(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
  bool OnProgress(InstallStep step, int nTotle, int nCur);
public:
  void SetWndPos(int x,int y);//左上角坐标点
private:
  BOOL BindResControl();
private:
  CDialogBackground*      m_pLicenceBkg;
  CSetupProgress          m_progress;
  CImageButton            m_closeBtnCtrl;

  CStatic                 m_installingStaticCtrl;          //正在为您安装yy浏览器：n%

  int                     m_WndX;
  int                     m_WndY;
};

#endif  //! #ifndef SETUP_PROGRESS_DIALOG_H_
