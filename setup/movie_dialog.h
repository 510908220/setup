#ifndef MOVIE_DIALOG_H_
#define MOVIE_DIALOG_H_

#include "image_button.h"
#include <atlcrack.h>
#include "ui/gdiplus_facade.h"
#include "movie_dialog_background.h"
#include "resource.h"
#include "setup_progress.h"
#include "7z_uncompress_helper.h"

#define TIMERID_RUN_YYEXPLORER 1
#define TIMERID_PLAY_MOVIE     2

const int LEN_IMAGE_ARRAY = 67;//�����ظ�����һ�¡���������û�취

class CMovieDialog : public CDialogImpl<CMovieDialog>, public IInstallNotify
{
public:
  CMovieDialog();

public:
  enum { IDD = IDD_SETUP_PROGRESS };

  ///////////////////////////////////////////////////////////////////////////////////////
  BEGIN_MSG_MAP_EX(CMovieDialog)
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
  void SetWndPos(int x,int y);//���Ͻ������
private:
  BOOL BindResControl();
	void PlayNextFrame(int nWaitSum = 1,int limit = LEN_IMAGE_ARRAY);
private:
  CMovieDialogBackground*      m_pMovieDlgBkg;
  CSetupProgress          m_progress;
  CImageButton            m_closeBtnCtrl;

  CStatic                 m_installingStaticCtrl;          //����Ϊ����װyy�������n%

  int                     m_WndX;
  int                     m_WndY;
	int											nCurrentFrame;
	int											nCurrentFrameLimit;
	int											nCurrentFrameWaitSum;
	int											nCurrentFrameWaitCount;
	bool										bShouldRun;
};

#endif  //! #ifndef SETUP_PROGRESS_DIALOG_H_
