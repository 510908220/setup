#include "stdafx.h"
#include "message_dialog.h"
#include <string>
#include "lang_string.h"
#include <algorithm>
#include "install_util.h"

using namespace std;

int MessageDlg::MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
  std::wstring szText = lpText;
  install_util::ReplaceString(szText, _TEXT("[ProductName]"), UI_PRODUCT_NAME);
  std::wstring szTitle = lpCaption;
  install_util::ReplaceString(szTitle, _TEXT("[ProductName]"), UI_PRODUCT_NAME);
  return ::MessageBox(hWnd, szText.c_str(), szTitle.c_str(), uType + MB_SYSTEMMODAL);
}