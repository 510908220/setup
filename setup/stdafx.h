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

#define YE_SETUP_PREFIX "yesetup/"
#define EID_SUCCEED "Succeed"
#define EID_STARTUP_LNK "StartupLnk"
#define EID_DEFAULT_BROWSER "DefaultBrowser"
#define ESRC_SILENT_INSTALL "SilentInstall"
#define ESRC_UI_INSTALL "UIInstall"
#define EID_INSTALL_SCHEDULED_TASK "InstallScheduledTask"
#define EID_UNINSTALL_SCHEDULED_TASK "UninstallScheduledTask"

extern DWORD installType;