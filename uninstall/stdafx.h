// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
//#define WINVER		0x0500
//#define _WIN32_WINNT	0x0501
#define _WIN32_IE	_WIN32_IE_IE60
#define _RICHEDIT_VER	0x0200
//#define NTDDI_VERSION   0x06010000

#ifndef max
#define max(a,b)	(((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif
// wtl
#include <atlstr.h>
#define _WTL_NO_CSTRING
#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlframe.h>

extern CAppModule _Module;

#define YG_PROCESS_ERROR(condition)\
	do \
{\
	if (!(condition))\
{\
	goto Exit0;\
}\
} while (0)

#define YE_UNINSTALL_PREFIX "yeuninstall/"
#define EID_QUIT_UNINSTALL "QuitUninstall"
#define ESRC_UI_UNINSTALL "UIUninstall"
#define UNINSTALL_SCHEDULED_TASK "UninstallScheduledTask"