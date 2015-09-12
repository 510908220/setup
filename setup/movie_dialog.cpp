#include "stdafx.h"
#include "movie_dialog.h"
#include "install_manager.h"
#include "res_filename.h"
#include "yygame/ygdata_report/ygdata_report_api.h"
#include <sstream>


CMovieDialog::CMovieDialog() :
  m_closeBtnCtrl(CLOSE_NORMAL_FILE, CLOSE_HOVER_FILE, CLOSE_DOWN_FILE, CLOSE_DISABLE_FILE),
  m_progress(SETUP_PROGRESS_FILE, SETUP_PROGRESSBKG_FILE)
{
  m_WndX = 0;
  m_WndY = 0;
	nCurrentFrame = 0;
	nCurrentFrameLimit = 0;
	nCurrentFrameWaitSum = 1;
	nCurrentFrameWaitCount = 1;
	bShouldRun = false;
}

LRESULT CMovieDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

  //为窗体添加WS_EX_LAYERED属性,该属性使窗体支持透明
  //ModifyStyleEx(0, WS_EX_LAYERED);
  DWORD dwExStyle=GetWindowLong(GWL_EXSTYLE);
  if((dwExStyle & 0x80000) != 0x80000 || (dwExStyle & WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW)
    SetWindowLong(GWL_EXSTYLE, dwExStyle | 0x80000 | WS_EX_TOOLWINDOW);

  // set icons
  HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
  SetIcon(hIcon, TRUE);
  HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
  SetIcon(hIconSmall, FALSE);
  //set title
  SetWindowText(INSTALL_TITLE);

  int w = BACKGROUND_SIZE_W + 1;// 460;
  int h = BACKGROUND_SIZE_HIDE_H + 1;//300;
  SetWindowPos(HWND_TOP, m_WndX, m_WndY, w, h, NULL);
  HRGN hrgn = CreateRoundRectRgn(0, 0, w, h, 5, 5);// rounded rect w/50 pixel corners
  SetWindowRgn(hrgn, TRUE);

	BindResControl();

	SetForegroundWindow(m_hWnd);
	nCurrentFrameWaitSum = 1;
	SetTimer(TIMERID_PLAY_MOVIE, 40);
  // center the dialog on the screen
  CenterWindow();

  CInstallManager::GetInstance().OutputDebugStrW(L"安装包MovieDlgShow");
  CInstallManager::GetInstance().InstallYYExplorer(this);
  //OnProgress(ISP_BACKUP, 100, 50);//test
  //OnProgress(ISP_SETDEFAULT, 100, 100);//test
  //OnProgress(ISP_UNCOMPRESS, 100, 90);//test
  
  return TRUE;
}

void CMovieDialog::SetWndPos(int x,int y)
{
  m_WndX = x;
  m_WndY = y;
}

LRESULT CMovieDialog::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  RECT rect;
  GetWindowRect(&rect);
  BitBlt ((HDC)wParam, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_pMovieDlgBkg->GetHDC(), 0, 0, SRCCOPY);
  return 0;
}

LRESULT CMovieDialog::OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  HDC hdc =(HDC)wParam;
  HWND hwnd = (HWND)lParam;

  SetBkMode(hdc, TRANSPARENT);
  SetTextColor(hdc, RGB(0, 0, 0));

  return (LRESULT)GetStockObject(HOLLOW_BRUSH);
}

LRESULT CMovieDialog::OnMinSize(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);
  return 0;
}

BOOL CMovieDialog::OnCloseClick()
{
  EndDialog(IDCLOSE);
  return TRUE;
}

BOOL CMovieDialog::BindResControl()
{
  //背景图
  m_pMovieDlgBkg = new CMovieDialogBackground(m_hWnd);
  m_pMovieDlgBkg->SetFrame(1);
  //安装文字提示
  HWND hwndInstallingStatic = GetDlgItem(IDC_INSTALLING_TEXT);
  m_installingStaticCtrl.Attach(hwndInstallingStatic);
  m_installingStaticCtrl.SetWindowPos(HWND_BOTTOM, INSTALLING_STATIC_POS_X,
    INSTALLING_STATIC_POS_Y, INSTALLING_STATIC_SIZE_W, INSTALLING_STATIC_SIZE_H, NULL);
  m_installingStaticCtrl.ShowWindow(SW_HIDE);
  //进度条
  m_progress.SubclassWindow(GetDlgItem(IDC_SETUP_PROGRESS));
  m_progress.SetWindowPos(HWND_BOTTOM, PROGRESS_POS_X,
    PROGRESS_POS_Y, PROGRESS_SIZE_W, PROGRESS_SIZE_H,NULL);
	m_progress.ShowWindow(SW_HIDE);
  //关闭按钮
  m_closeBtnCtrl.SubclassWindow(GetDlgItem(IDC_BUTTON_CLOSE));
  m_closeBtnCtrl.SetWindowPos(HWND_BOTTOM, CLOSE_BTN_POS_X,
    CLOSE_BTN_POS_Y, CLOSE_BTN_SIZE_W, CLOSE_BTN_SIZE_H,NULL);
  m_closeBtnCtrl.SetBkgInfo(m_hWnd, m_pMovieDlgBkg->GetHDC());
  m_closeBtnCtrl.EnableWindow(FALSE);
  return TRUE;
}

LRESULT CMovieDialog::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  ReleaseCapture();
  SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
  return 0;
}

bool CMovieDialog::OnProgress(InstallStep step, int nTotle, int nCur)
{
  int preProgress = 0;
  int thisStep = 0;
  preProgress = step;
  switch (step)
  {
  case ISP_BACKUP:
		nCurrentFrameLimit = 28;
    thisStep = ISP_UNCOMPRESS - ISP_BACKUP; break;
	case ISP_CREATELNK:
		nCurrentFrameLimit = 36;
		thisStep = ISP_SETDEFAULT - ISP_CREATELNK; break;
  case ISP_SETDEFAULT:
		nCurrentFrameLimit = 49;
    thisStep = ISP_UNCOMPRESS - ISP_SETDEFAULT; break;
  case ISP_UNCOMPRESS:
		nCurrentFrameLimit = 66;
    thisStep = 100 - ISP_UNCOMPRESS; break;
  }

  /*
  //换背景图
  if (step == ISP_SETDEFAULT && nTotle == 100 && nCur == 100)//ON_PROPRESS(ISP_SETDEFAULT, 100, 100);
  {
    m_pLicenceBkg->SetStatu(LS_Progress2);
    m_closeBtnCtrl.SetBkgInfo(m_hWnd,m_pLicenceBkg->GetHDC());
    Invalidate();
  }
  if (step == ISP_UNCOMPRESS && nTotle == 100 && nCur == 90)//ON_PROPRESS(ISP_UNCOMPRESS, 100, 90);
  {
    m_pLicenceBkg->SetStatu(LS_Progress3);
    m_closeBtnCtrl.SetBkgInfo(m_hWnd,m_pLicenceBkg->GetHDC());
    Invalidate();
  }
  */
  //进度条上面的文字
  int currPos = preProgress + (nCur * thisStep) / nTotle;
  //看看进度是不是比之前的小
  int oldPos = m_progress.GetPos();
  if (oldPos > currPos)
    return false;
  //
  std::wstringstream ss;
  ss << INSTALLING_STATIC_MODE << _TEXT(" ") << currPos << _TEXT("%");
  std::wstring installing_str = ss.str();
  SetDlgItemText(IDC_INSTALLING_TEXT,installing_str.c_str());
  //解决背景透明时文字重叠的问题
  RECT rect = {INSTALLING_NUMBER_STATIC_POS_X, INSTALLING_NUMBER_STATIC_POS_Y,
    INSTALLING_NUMBER_STATIC_POS_X + INSTALLING_NUMBER_STATIC_SIZE_W, INSTALLING_NUMBER_STATIC_POS_Y + INSTALLING_NUMBER_STATIC_SIZE_H};
  InvalidateRect(&rect,TRUE);
  //进度条
  m_progress.SetPos(currPos);
  
  if (currPos == 100)
  {
		nCurrentFrameLimit = LEN_IMAGE_ARRAY;
    ygdata_report::SendEventWithSrc(YE_SETUP_PREFIX EID_SUCCEED, ESRC_UI_INSTALL);
    SetTimer(TIMERID_RUN_YYEXPLORER, 300);
  }
  return true;
}

LRESULT CMovieDialog::OnRollLogoTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  return 0;
}

void CMovieDialog::PlayNextFrame(int nWaitSum/*=1*/,int limit /*= LEN_IMAGE_ARRAY*/)
{
	//播放nCurrentFrame（限制在limit帧之前）
	if(nCurrentFrame < limit)
	{
		if(nCurrentFrameWaitCount >= nWaitSum)
		{
			nCurrentFrameWaitCount = 1;
			nCurrentFrame++;
		}
		else
			nCurrentFrameWaitCount++;
	}
	else
		nCurrentFrame = limit;
	//
	m_pMovieDlgBkg->SetFrame(nCurrentFrame);
}

LRESULT CMovieDialog::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  switch(wParam)
  {
    case TIMERID_RUN_YYEXPLORER:
    {
			bShouldRun = true;
			nCurrentFrameWaitSum = 1;
      KillTimer(TIMERID_RUN_YYEXPLORER);
      break;
    }
		case TIMERID_PLAY_MOVIE:
		{
			PlayNextFrame(nCurrentFrameWaitSum, nCurrentFrameLimit);
			if(nCurrentFrame >= LEN_IMAGE_ARRAY - 1)
			{
				if (bShouldRun)
				{
					KillTimer(TIMERID_PLAY_MOVIE);
					ShowWindow(SW_HIDE);
					//
				  CInstallManager::GetInstance().LaunchYYExplorer();
					//
					EndDialog(IDCLOSE);
				}
				nCurrentFrame = LEN_IMAGE_ARRAY - 1;
			}
		}
  }
  return 0;
}