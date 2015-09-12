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
  MESSAGE_HANDLER(WM_CLOSE, OnDialogClose)          //�Ҽ��ر�
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
  //page1:��ʼ���棬page2:Э�飬page3:�Զ��尲װ

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
  CContainedWindowT<CStatic>  m_licenceStaticCtrl; //<<�����û����Э��>> 
  CImageButton                m_doInstallBtnCtrl;  //������װ��ť
  CImageButton                m_closeBtnCtrl;			//�رհ�ť
  CImageButton                m_changePathBtnCtrl;//����·�����������󵯴�ѡ·���Ĺ��ܣ�
  YGImage*                    m_logoImageBkg;          //logo
  YGImage*                    m_ImageBkg;              //����ͼ��ÿ�θ���ҳ��ʱ���ư�ǰ����������渲�ǵ� //��취��m_pLicenceBkg��һ��

private:
  CEdit                       m_installPathEditCtrl;    //��װ·���༭��
  CStatic                     m_YYExplorerRunningStaticCtrl;  //YY������������У���������̰�װ�����ر����������װ
  CStatic                     m_licenceTitleStaticCtrl;  //�û����Э�� ����
	CStatic                     m_installTitleStaticCtrl;  //YY�����0.1.7beta�� ����
  CStatic                     m_installPathStaticCtrl;  //��װ����
  CStatic                     m_installNotChangeStaticCtrl;  //...���������·��..
  CButton                     m_installPathButtonCtrl;  //����·��������ť������ʾ���
  CImageButton                m_customButtonCtrl;        //�Զ��尲װ������ui��ʾ��
  CImageButton                m_backButtonCtrl;        //���أ�����ui��ʾ��
  CImageCheckBox              m_shortcutCheckBoxCtrl;   //������ݷ�ʽ
  CImageCheckBox              m_setDefaultCheckBoxCtrl; //��ΪĬ�������
  CImageCheckBox              m_joinExpCheckBoxCtrl;    //�����û�����ƻ�
  CContainedWindowT<CStatic>  m_expLinkStaticCtrl;      //�û�����ƻ�����

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
