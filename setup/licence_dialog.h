#ifndef _LINSENCED_H_
#define _LINSENCED_H_

#include <atlcrack.h>
#include "ui/gdiplus_facade.h"
#include "ui/ygimage.h"
#include "dialog_background.h"
#include "image_button.h"
#include "ie_browser.h"
#include "image_checkbox.h"
#include "res_filename.h"

const int YYEXPLORER_INSTALL_UI_PAGE_INIT    = 1;
const int YYEXPLORER_INSTALL_UI_PAGE_LISENCE = 2;
const int YYEXPLORER_INSTALL_UI_PAGE_CUSTOM  = 3;

class CLicenceDialog : public CDialogImpl<CLicenceDialog>
{
public:
  CLicenceDialog();
  ~CLicenceDialog();

public:
  enum { IDD = IDD_LINSENCED };

///////////////////////////////////////////////////////////////////////////////////////
  BEGIN_MSG_MAP_EX(CLicenceDialog)
///////////////////////////////////////////////////////////////////////////////////////
  MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
  MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWnd)
  MESSAGE_HANDLER(WM_ERASEBKGND, OnPaint)
  MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnColor)
  MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
  MESSAGE_HANDLER(WM_CLOSE, OnDialogClose)          //右键关闭
/////////////////////////////////////////////////////////////////////////////////////////
  COMMAND_ID_HANDLER(IDC_MINSIZE, OnMinSize)
  COMMAND_ID_HANDLER(IDC_CUSTOM_OPTION, OnCustomOption)
  COMMAND_ID_HANDLER(IDC_LINSENCED_LINK, OnLinsencedLink)
  REFLECT_NOTIFICATIONS()
////////////////////////////////////////////////////////////////////////////////////////
  ALT_MSG_MAP(1)
  MSG_WM_SETCURSOR(OnLicenceLinkSetCursor)
  ALT_MSG_MAP(2)
  MSG_WM_SETCURSOR(OnExpLinkSetCursor)
///////////////////////////////////////////////////////////////////////////////////////
  END_MSG_MAP()
//////////////////////////////////////////////////////////////////////////////////////
public:
  BOOL OnInstallClick();
  BOOL OnAcceptLicenceStatuChange(BOOL bCheck);
  BOOL OnCloseClick();
  BOOL OnChangePathClick();
  BOOL OnCustomClick();
  BOOL OnBackClick();
private:
  bool OnClickSlot(CImageButton* button, bool& handled);
  bool OnMouseMoveSlot(CImageButton* button, bool& handled);
  bool OnCheckSlot(CImageCheckBox* checkbox, bool& handled);
private:
  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnDialogClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnShowWnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  
  LRESULT OnMinSize(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnLinsencedLink(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnCustomOption(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
 
  LRESULT OnLicenceLinkSetCursor(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg);
  LRESULT OnExpLinkSetCursor(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg);
private:
  void InitFont();
  void InitVistaFont();
  void InitXpFont();

  BOOL BindDefaultResControl();
  BOOL BindCustomOptionResControl();
  void ShowPage(int nPage);
  //page1:初始界面，page2:协议，page3:自定义安装

  void SetControlsBkgInfo();
  BOOL ShowBrowserDirDlg();
  BOOL CheckAndSetFilePathEditText(LPCWSTR szText);
  BOOL IsSystemAllowedInstallPath(LPCWSTR szText);
  BOOL CheckInstallPathAvailable(LPCWSTR szText);
  BOOL CheckTmpDirFreeSpaceAvailable();
  BOOL CheckInstallPathFreeSpaceAvailable(LPCWSTR szUserSetInstallPathText);
  void SetTextFontColorSize(HDC hdc, LPTSTR szFont, COLORREF color,int size);
  void SetTextFontColorSize(HDC hdc, FONT_COLOR_SIZE vista, FONT_COLOR_SIZE xp);
private:
  CImageCheckBox              m_acceptLicenceCheckBoxCtrl;
  CContainedWindowT<CStatic>  m_licenceStaticCtrl; //<<最终用户许可协议>> 
  CImageButton                m_doInstallBtnCtrl;  //立即安装按钮
  CImageButton                m_closeBtnCtrl;			//关闭按钮
  CImageButton                m_changePathBtnCtrl;//更改路径（负责点击后弹窗选路径的功能）
  YGImage*                    m_logoImageBkg;          //logo
  YGImage*                    m_ImageBkg;              //背景图，每次更换页面时绘制把前面的遗留界面覆盖掉 //想办法跟m_pLicenceBkg合一块

private:
  CEdit                       m_installPathEditCtrl;    //安装路径编辑框
  CStatic                     m_YYExplorerRunningStaticCtrl;  //YY浏览器正在运行，点击“立刻安装”将关闭浏览器并安装
  CStatic                     m_licenceTitleStaticCtrl;  //用户许可协议 标题
	CStatic                     m_installTitleStaticCtrl;  //YY浏览器0.1.7beta版 标题
  CStatic                     m_installPathStaticCtrl;  //安装到：
  CStatic                     m_installNotChangeStaticCtrl;  //...不建议更改路径..
  CButton                     m_installPathButtonCtrl;  //更改路径（负责按钮本身显示与否）
  CImageButton                m_customButtonCtrl;        //自定义安装（负责ui显示）
  CImageButton                m_backButtonCtrl;        //返回（负责ui显示）
  CImageCheckBox              m_shortcutCheckBoxCtrl;   //创建快捷方式
  CImageCheckBox              m_setDefaultCheckBoxCtrl; //设为默认浏览器
  CImageCheckBox              m_joinExpCheckBoxCtrl;    //加入用户体验计划
  CContainedWindowT<CStatic>  m_expLinkStaticCtrl;      //用户体验计划链接

  CIEBrowser                  m_licenceBrowserCtrl;
private:
  int                         nCurrentPage;

private:
  BOOL                        m_customOptionShowStatus;
  BOOL                        m_licenseShowStatus;
  CDialogBackground*          m_pLicenceBkg;
  HFONT                       m_hFontText;
};

#endif  //! #ifndef _LINSENCED_H_
