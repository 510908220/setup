#ifndef _RES_STRUCT_H_
#define _RES_STRUCT_H_
#include "wtypes.h"

enum SKIN
{
  SKIN_NONE = 0,
  SKIN_1,
  SKIN_2,
  SKIN_3,
  SKIN_4,
  SKIN_GAMECHANNEL = 1000,
  SKIN_CHANGYOU,
  SKIN_YYEURL = 9999,
};

typedef struct __SPECIAL_HANDING_
{
  bool skip_startdlg;
  bool skip_licencedlg;
}SPECIAL_HANDING;
//movie
typedef struct __MOVIE_FRAME_
{
  int start_init;//最初的那帧
  int start_begin;//循环帧开始
  int hover_begin;
  int drag_begin;
  int install_begin;
  int install_end;
  TCHAR* mode;
  int duration_time;
  int startw;
  int starth;
  int installw;
  int installh;
  bool b_start_reverse;
  bool b_hover_loop_play;
} MOVIE_FRAME;
//flash
typedef struct __FLASH_INFO_
{
  TCHAR* szStartFlashFile;
  TCHAR* szDragFlashFile;
  TCHAR* szInstallFlashFile;
} FLASH_INFO;
//1
typedef struct  __FONT_COLOR_SIZE_
{
  TCHAR* szFont;
  COLORREF color;
  int nHeight;
} FONT_COLOR_SIZE_;

//2
typedef struct __WND_CONTROL
{
  TCHAR* szNormalFile;
  TCHAR* szHoverFile;
  TCHAR* szDownFile;
  TCHAR* szDisableFile;
  int x;
  int y;
  int w;
  int h;
  TCHAR* text;
  FONT_COLOR_SIZE_ font_color_size_xp;
  FONT_COLOR_SIZE_ font_color_size_vista;
} WND_CONTROL,WND_CONTROL_BTN;

typedef struct __WND_CONTROL_CHECKBOX
{
  TCHAR* szCheckNormalFile;
  TCHAR* szCheckHoverFile;
  TCHAR* szCheckDownFile;
  TCHAR* szCheckDisableFile;
  TCHAR* szUnCheckNormalFile;
  TCHAR* szUnCheckHoverFile;
  TCHAR* szUnCheckDownFile;
  TCHAR* szUnCheckDisableFile;
  int x;
  int y;
  int w;
  int h;
  TCHAR* text;
  FONT_COLOR_SIZE_ font_color_size_xp;
  FONT_COLOR_SIZE_ font_color_size_vista;
} WND_CONTROL_CHECKBOX;

typedef struct __WND_CONTROL_STATIC
{
  int x;
  int y;
  int w;
  int h;
  TCHAR* text;
  FONT_COLOR_SIZE_ font_color_size_xp;
  FONT_COLOR_SIZE_ font_color_size_vista;
} WND_CONTROL_STATIC,WND_CONTROL_TEXT;

typedef struct __WND_CONTROL_PROGRESS
{
  TCHAR* szProgressNormalFile;
  TCHAR* szProgressBkgFile;
  int x;
  int y;
  int w;
  int h;
} WND_CONTROL_PROGRESS;

#endif