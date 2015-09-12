#ifndef MESSAGE_DLG_
#define MESSAGE_DLG_

class MessageDlg
{
public:
  static int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
private:
};
#endif  //! #ifndef MESSAGE_DLG_
