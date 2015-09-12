#pragma once

#include <string>
#include "dwutility/ustring.h"

inline std::string app2net(const wchar_t * w, int cp = CP_UTF8)
{
    return DwUtility::text::toNarrowString(w, wcslen(w), cp);
}

inline std::string app2net(const std::wstring &w, int cp = CP_UTF8)
{
    return DwUtility::text::toNarrowString(w.c_str(), w.size(), cp);
}

inline std::wstring uitow10(unsigned int i)
{
	wchar_t buf[20];
    _ui64tow(i, buf, 10);
	return std::wstring(buf);
}

inline std::wstring itow10(int i)
{
	//fix the problem when sid > 0x80000000
	return uitow10((unsigned int)i);
}

inline std::wstring net2app(const std::string & net, int cp = CP_UTF8)
{
    return DwUtility::text::toWideString(net.c_str(), net.size(), cp);
}
