
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#ifdef _WINDOWS
#include <windows.h>
#endif
#include <string>
#include <vector>

namespace DwUtility
{
	namespace text
	{
		std::wstring    toWideString(const char* pStr, int len,UINT CodePage = CP_ACP );

		std::string    toNarrowString(const wchar_t* pStr, int len, UINT CodePage = CP_ACP);

		void    splitWString(const std::wstring &stringOrg, const std::wstring & delimiter, std::vector<std::wstring> &vectorResult);

		std::string    getHashString( const char *hashData, int length );

		std::wstring    Char2String(unsigned char c);

		std::wstring    BinToHex(const std::string& bin);

		std::string    URLEncode2(const std::string& src);
		std::string    URLEncode2(const std::wstring& str);
		std::wstring    URLEncode(const std::wstring& src);

        bool    isUtf8Text(const char *text, size_t size);

		std::string    toBase64String(const void *data, size_t size);
	}
}
