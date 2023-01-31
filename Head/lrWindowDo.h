// lrWindowDo.h -- To combine into lrWindowDo() function
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
// This project was written in 2022.01.18 - 19:12:36
// 
// 
// Abstract:
//     For window and cursor control, and create new window.
// 
#pragma once

#ifndef _LR_WindowDo_H_
#define _LR_WindowDo_H_

#include <stdio.h>
#include <Windows.h>
#include <windowsx.h>
#include <TlHelp32.h>
#include <io.h>
#include <string.h>
#include <time.h>
#include <psapi.h>
#include <mciapi.h>
#include <stdarg.h>
#include <direct.h>

#include "wdoio.h"
#include "lrMusic.h"

#pragma warning (disable : 4996)
#pragma warning (disable : 4090)
#pragma warning (disable : 28159)


/* Maximum length of window title */
#define MAXLEN_WIN_TITLE		201
/* Maximum length of window class */
#define MAXLEN_WIN_CLASS		101

#include "lrWindowAlias.h"


#if !defined(lrERRORTEXT_LANGUAGE)
#define lrERRORTEXT_LANGUAGE		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)
#endif


/* 默认的屏幕刷新率 */
#define FREQUENCY_SCREEN_REFRESH	50
/* 默认的移动刷新率 */
#define FREQUENCY_MOVE_REFRESH		50
/* 默认的功能刷新率 */
#define FREQUENCY_FUNCTION_REFRESH	200



/* lrWindowWiggle(): 震动的周期。每改变多少次位置校准一次原点位置，如果周期太大，会长时间禁止窗口移动 */
#define lrWINDOWWIGGLE_CYCLE		5
#define lrCURSORWIGGLE_CYCLE		5


/* lrWindowAside(): 边框的容差度 */
#define lrWINDOWASIDE_JAMBALW		7


/* __int64 输入输出的最大长度 (包括'\0') */
#define INT64_INPUT_MAX_LEN			(18 + 1)
#define INT64_OUTPUT_MAX_LEN		( 9 + 1)



/* lrCopyWindow() : 默认新类名前缀 */
#define lrCOPYWINDOW_NEWCLASSPREFIX	"_lr_copy_"



/*
* lrCreateBlankBox(): 空白框一些默认属性。
*/
/* 类名 */
#define lrCLASSNAME_BLANKBOX		"lr_wdo_BlankBoxClass"
/* 标题 */
#define lrTITLETEXT_BLANKBOX		"lrBlankBox"
/*
* lrCreateTextBox(): 文本框一些默认属性。
*/
/* 类名 */
#define lrCLASSNAME_TEXTBOX			"lr_wdo_TextBoxClass"
/* 标题 */
#define lrTITLETEXT_TEXTBOX			"lrTextBox"
/*
* lrCreateInfoBox(): 信息框一些默认属性。
*/
/* 类名 */
#define lrCLASSNAME_INFOBOX			"lr_wdo_InfoBoxClass"
/* 标题 */
#define lrTITLETEXT_INFOBOX			"lrInfoBox"
/*
* lrCreateInfoBox(): 日志框一些默认属性。
*/
/* 类名 */
#define lrCLASSNAME_LOGBOX			"lr_wdo_LogBoxClass"
/* 标题 */
#define lrTITLETEXT_LOGBOX			"lrLogBox"
/*
* lrCreateImageBox(): 图片框一些默认属性。
*/
/* 类名 */
#define lrCLASSNAME_IMAGEBOX		"lr_wdo_ImageWindowClass"
/*
* lrCreateBlueBox(): 蓝屏框一些默认属性。
*/
/* 类名 */
#define lrCLASSNAME_BLUEBOX			"lr_wdo_BlueBoxClass"
/* 标题 */
#define lrTITLETEXT_BLUEBOX			"lrBlueBox"


/* 为值加上双引号 */
#define lr_TO_STR(value)						#value


/* 将通过用户输入的十六进制数转化成真正的 COLORREF，
* 因为 COLORREF 的三原色排列其实是 BGR，
* 所以此宏用于将 B 和 R 交换位置。
* 
* 例： lr_TO_RGB(0x112244)
* 得到的结果将是 0x442211
*/
#define lr_TO_RGB(color)						(COLORREF)(((color << 16) & 0xFFFFFF) | (color & 0x00FF00) | ((color & 0xFF0000) >> 16))

/* 初始化随机数 */
#define lr_srand()								srand((unsigned int)GetCurrentTime() + rand())


/* 限制数值（val）的长度在 最小值(MinVal) 和 最大值(MaxVal) 之间 */
#define lr_LIMIT_VALSIZE(val,MinVal,MaxVal)		(val < MinVal ? val = MinVal : (val > MaxVal ? val = MaxVal : val))

/* 限制字符串（str）长度在 maxlen 以内，超过则截断 */
#define lr_LIMIT_STRLEN(str,maxlen)				(strlen(str) > (size_t)maxlen ? str[maxlen - 1] = '\0' : 0)

/* 创建字体的模板。
* 
* 参数:
*		cHeight    |  字体高度
*		cWidth     |  字体宽度
*		cWeight    |  字体粗细
*		bItalic    |  倾斜
*		bUnderline |  下划线
*		bStrikeOut |  删除线
*		pszFaceName|  字体名称
* 
* 返回值:
*		返回 HFONT 类型的字体句柄。
*/
#define lr_CreateFont(cHeight, cWidth, cWeight, bItalic, bUnderline, bStrikeOut, pszFaceName) \
												CreateFontA(-((int)cHeight), -((int)cWidth), 0, 0, (int)cWeight, (BOOL)bItalic, (BOOL)bUnderline, (BOOL)bStrikeOut, \
													DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH | FF_DONTCARE,\
													pszFaceName)

/* 兼容更少参数的 lrLOGFONT 类型，不需要取地址符(&) */
#define lr_CreateFontIndirect(logFont)			lr_CreateFont(logFont.lfHeight, logFont.lfWidth, logFont.lfWeight, logFont.lfItalic, logFont.lfUnderline, logFont.lfStrikeOut, logFont.lfFaceName)



typedef struct FSIZE
{
	FLOAT fw;
	FLOAT fh;
}FSIZE, * PFSIZE;

/* 适用于 lr_CreateFontIndirect() 的 LOGFONT 结构体 */
typedef struct lrLOGFONT
{
	LONG      lfHeight;
	LONG      lfWidth;
	LONG      lfWeight;
	BYTE      lfItalic;
	BYTE      lfUnderline;
	BYTE      lfStrikeOut;
	CHAR      lfFaceName[LF_FACESIZE];
}lrLOGFONT, * lrPLOGFONt;


/* lrWindowDo() : return value */
typedef struct WDO
{
	/* The window pointed to by the current lrWindowDo()
	*  Returns its address. It is static in the function */
	HWND* PtrWindow;

	/* If FALSE, lrWindowDo() error
	*  The error information will be output to the log window
	*  This default log window uses wdo_gethwnd(WDOGETHWND_OUTLOG) to obtain */
	BOOL  state;
}WDO, * PWDO;


/* lrCreateInfoBox(): 要显示的信息框窗口类型 */
typedef enum INFOBOX_WINDOWTYPE
{
	infobox_s_ct,				// 静态当前窗口
	infobox_s_par,				// 静态父窗口
	infobox_s_top,				// 静态顶层窗口
	infobox_s_desk,				// 静态桌面窗口
	infobox_s_cur,				// 静态游标窗口
	infobox_s_cp,				// 静态游标父窗口

	infobox_v_ct,				// 动态当前窗口
	infobox_v_par,				// 动态父窗口
	infobox_v_top,				// 动态顶层窗口
	infobox_v_desk,				// 动态桌面窗口
	infobox_v_cur,				// 动态游标窗口
	infobox_v_cp				// 动态游标父窗口
	
}INFOBOX_WINDOWTYPE;


typedef enum WINCODE { WINCODE_UNICODE, WINCODE_ASCII } WINCODE;

/* 窗口所有信息，使用 lrGetWindowAllInfo() 赋值 */
typedef struct WINDOWALLINFO
{
	HWND		ID;									// 窗口句柄
	DWORD		PID;								// 窗口所属的进程的PID
	CHAR		TITLE[MAXLEN_WIN_TITLE];			// 窗口标题文本
	CHAR		CLASS[MAXLEN_WIN_CLASS];			// 窗口类名
	CHAR		PPATH[MAX_PATH];					// 窗口所在的进程的文件路径
	HWND		PARENT;								// 窗口的父窗口
	WINCODE		CODE;								// 窗口的编码
	RECT		VERTEX;								// 窗口的顶点
	RECT		CVERTEX;							// 窗口的顶点（在父窗口中）
	SIZE		WINSIZE;							// 窗口的宽高
	FSIZE		DPIZOOM;							// 窗口的DPI缩放比例
	BOOL		SHOW;								// 窗口是否显示

	POINT		CURPOS;								// 游标坐标
	COLORREF	CURCOLOR;							// 游标坐标的颜色
	POINT		CURINWINPOS;						// 游标在客户区中的坐标

	LONG		STYLE;								// 窗口风格
	LONG		STYLEEX;							// 窗口扩展风格
	LONG		CLASSSTYLE;							// 窗口类风格
}WINDOWALLINFO, * PWINDOWALLINFO;


/* Near window jamb */
typedef enum NEARWINDOWJAMB
{
	NEARWINDOWJAMB_OUTSIDE = 0,				// If in window outside
	NEARWINDOWJAMB_LEFT,					// If near window left
	NEARWINDOWJAMB_TOP,						// If near window top
	NEARWINDOWJAMB_RIGHT,					// If near window right
	NEARWINDOWJAMB_BOTTOM					// If near window bottom

} NEARWINDOWJAMB;

typedef enum GWP_LR_ORIGINALPOINT
{
	GWP_LR_ORIGINALPOINT_SCREEN = 0,	// 以屏幕为原点
	GWP_LR_ORIGINALPOINT_PARENT = 1		// 以父窗口为原点（如果没有父窗口则以屏幕为原点）

} GWP_LR_ORIGINALPOINT;

typedef struct lrFIND_WINDOW_TYPE
{
	enum
	{
		LR__FIND_WINDOW_TYPE__WINDOWCLASS = 1,
		LR__FIND_WINDOW_TYPE__WINDOWTITLE,
		LR__FIND_WINDOW_TYPE__WINDOWPPATH,
	}type;

	union
	{
		const char* const WindowClass;
		const char* const WindowTitle;
		const char* const WindowPPath;
	}data;

}lrFIND_WINDOW_TYPE, * lrPFIND_WINDOW_TYPE;


/* 窗口和游标碰撞时发生的事件
* lrWindowAndCursorMoveAfterCollide()
*/
typedef enum lrWINDOW_AND_CURSOR_MOVE_AFTER_COLLIDE_MODE
{
	WINDOWMOTION_WINDOWANDCURSORCOLLIDE_PUSHMOVE = 0,	// 游标推进窗口移动
	WINDOWMOTION_WINDOWANDCURSORCOLLIDE_MOVEWINDOW,		// 游标击退窗口
	WINDOWMOTION_WINDOWANDCURSORCOLLIDE_MOVECURSOR,		// 窗口击退游标
	WINDOWMOTION_WINDOWANDCURSORCOLLIDE_REVERSEMOVE		// 窗口和游标反向移动

}lrWINDOW_AND_CURSOR_MOVE_AFTER_COLLIDE_MODE, * lrWINDOW_AND_CURSOR_MOVE_AFTER_COLLIDE_MODE_PTR;


/* lrCreateWindow() : 创建一个新窗口并管理消息的接收 */
typedef struct CWE
{
	/* 这两个参数保留内部使用 */

	volatile HWND hwnd;			// 窗口创建完成后返回的句柄
	volatile BOOL done;			// 窗口创建完成时会改为 true
	
	BOOL CencelShow;			// 如果为 TRUE 则取消窗口显示

	/* 同 CreateWindowEx() 参数
	* https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa?redirectedfrom=MSDN&f1url=%3FappId%3DDev16IDEF1%26l%3DZH-CN%26k%3Dk(CreateWindowEx)%3Bk(DevLang-C%252B%252B)%3Bk(TargetOS-Windows)%26rd%3Dtrue
	*/
	
	DWORD		dwExStyle;							// 扩展窗口风格 https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
	CHAR		lpClassName[MAXLEN_WIN_CLASS];		// 窗口类名
	CHAR		lpWindowName[MAXLEN_WIN_TITLE];		// 窗口标题
	DWORD		dwStyle;							// 窗口风格		https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles?redirectedfrom=MSDN&f1url=%3FappId%3DDev16IDEF1%26l%3DZH-CN%26k%3Dk(WINUSER%252FWS_CHILD)%3Bk(WS_CHILD)%3Bk(DevLang-C%252B%252B)%3Bk(TargetOS-Windows)%26rd%3Dtrue
	INT			X;									// 初始顶点坐标
	INT			Y;									// 初始顶点坐标
	INT			nWidth;								// 初始宽度
	INT			nHeight;							// 初始高度
	HWND		hWndParent;							// 父窗口句柄
	HMENU		hMenu;								// 菜单句柄
	HINSTANCE	hInstance;							// 模块实例句柄
	LPVOID		lpParam;							// 发送 WM_CREATE 时的 lParam 参数
}CWE, * PCWE;

typedef enum WDOECHO { WDOECHO_FALSE = FALSE, WDOECHO_TRUE = TRUE, WDOECHO_DEF} WDOECHO;


#ifdef __cplusplus
extern "C"
{
#endif



// In file lrWindowDo.c



/* Perform WDO command string */
WDO									// See struct WDO
lrWindowDo(
	const char* const _command,		// Command string
	WDOECHO _echo_state				// Set state of echo, See enum WDOECHO
);


/* Perform WDO script file (.wdo) */
errno_t
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  0  |  No error;                   *
*  1  |  Unrecognized suffix name;   *
*  2  |  File open failed;           *
-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
lrWindowDoScript(
	const char* const _ScriptPath,	// Script file path
	BOOL _RestoreTitle
);


// In file lrCreateWindow.c
// 
// 
// After the new windows created by these functions, I call them boxes.
// 
// These function used to create the box has an ALIAS_BOX type parameter at the end,
// this parameter can set some settings of the box to be created, such as class name of the window box.
// 
// If it is NULL, the internal will call lrGetAliasDef() to get the default data.
// Later, you can call lrSetBoxAlias() to change the current alias settings.
// 
// 
// About pointers and resources in ALIAS_BOX:
// 
//     When the window box is destroyed, the corresponding alias will be released by calling lrClearAliasEx();
// 
//     If lrSetBoxAlias() is called midway, lrClearAliasEx() will be called first to release the original alias,
//     the new alias will be released by calling lrClearAliasEx() when the window box is destroyed.
// 
// 
// About ALIAS, please go to the <lrWindowAlias.h> file
//


/* Determine whether it is a window box */
BOOL
lrIsWindowBox(
	HWND hWnd						// Window handle
);


/* Create a window, and show text, text from number of GetLastError() */
VOID
lrCreateErrorTextBox();



/* Create a new window and thread,
*  new thread for receiving and send window messages,
*  if you close the window, the thread will be closed. */
HWND								// If created done, return a HWND of new created window. else return NULL.
lrCreateWindow(
	PCWE _pcwe						// A pointer to the CWE struct, See CWE.
);



/* Create a blank box, no repaint */
HWND
lrCreateBlankBox(
	int _x,							// X-axis coordinate of new window.
	int _y,							// Y-axis coordinate of new window.
	int _w,							// Width of new window.
	int _h,							// Height of new window.
	const ALIAS_BOX* _BoxAlias		// Alias settings used.
);



/* Create a text box, and show text in the text box */
HWND
lrCreateTextBox(
	const char* const _text,		// For the window show text, allow use linebreak('\n').
	int _x,							// X-axis coordinate of new window.
	int _y,							// Y-axis coordinate of new window.
	const ALIAS_BOX* _BoxAlias		// Alias settings used.
);



/* Create a info box, and show these window info in the info box */
HWND
lrCreateInfoBox(
	volatile HWND* volatile _hwnd,	// A pointer to HWND, according to _type decision how to use _hwnd.
	INFOBOX_WINDOWTYPE _type,		// Type of the _hwnd, See struct INFOBOX_WINDOWTYPE.
	int _x,							// X-axis coordinate of new window.
	int _y,							// Y-axis coordinate of new window.
	const ALIAS_BOX* _BoxAlias		// Alias settings used.
);



/* Create a log box, and show log in the log box */
HWND
lrCreateLogBox(
	int _x,							// X-axis coordinate of new window.
	int _y,							// Y-axis coordinate of new window.
	int _w,							// Width of new window.
	int _h,							// Height of new window.
	const char* const _LogText,		// Linebreak mode is '\r\n', use lrStringLinebreakMode() to convert the linebreak mode.
	const ALIAS_BOX* _BoxAlias		// Alias settings used.
);



/* Create a image box, and show the image in the image box.
* 
*  In the image box, click two or more coordinate points, polyline will be drawn;
*  Input characters can be printed in the image box;
*  Right click to repaint the image box, and restore size. */
HWND
lrCreateImageBox(
	const char* const _imagepath,	// The image path (.bmp)
	int _x,							// X-axis coordinate of new window.
	int _y,							// Y-axis coordinate of new window.
	const ALIAS_BOX* _BoxAlias		// Alias settings used.
);



/* Create a blue box, the blue box is full screen and topmost */
HWND
lrCreateBlueBox(
	const char* const _text,		// Show text
	const ALIAS_BOX* _BoxAlias		// Alias settings used
);




// In file lrGetWindowInfo.c




/* Determine whether the cursor is in the window */
BOOL								// Return TRUE if is, else return FALSE.
InWindow(
	HWND _hwnd						// Window handle
);



/* Determine whether the cursor is in the jamb of window */
BOOL
InWindowJamb(
	HWND _hwnd,						// Window handle
	UINT _x,						// Length of x-asix
	UINT _y							// Length of y-asix
);



/* Determine the window border closet to the cursor */
NEARWINDOWJAMB						// See enum NEARWINDOWJAMB
lrCursorNearWindowJamb(
	HWND _hwnd,						// Window handle
	BOOL _ReturnOutside				// Allow return NEARWINDOWJAMB_OUTSIDE if is TRUE
);



/* Get window dpi zoom */
FSIZE								// See struct FSIZE, return width and height of window dpi, no return error.
lrGetWindowDpiZoom(
	HWND _hwnd						// Window handle, return screen dpi if is NULL.
);



/* Get cursor in window position */
POINT								// Subtract some pixels of the form according to the window style,
									// Will be compatible with the window dpi.
lrGetCursorInWindowPosition(
	HWND hWnd						// Window handle, return NULL if the hWnd is NULL or invalid.
);



/* Get the path from process of the window */
char*								// Return _ProcessPath
lrGetWindowProcessPath(
	HWND _hWnd,						// Window handle
	char* _ProcessPath,				// Receiving the process path (.exe)
	size_t _PathMaxLen				// Limit maximum length of _ProcessPath
);



/* Get all window handle of the process */
HWND								// Return _AllWindow[0] if not NULL
lrFindProcessPathWindow(
	const char* const _ProcessPath,	// The process path
	HWND** _AllWindow,				// Get all window handle, need free
	UINT* _WindowQuan				// Get quantity of all window handle
);


/* Get command line of process */
BOOL
lrGetCommandLine(
	DWORD pId,						// The process id
	CHAR* pCmdLine,					// Get command line parameter, use CommandLineToArgvW() to argv
	DWORD dwBufLen					// Limit maximum length of pCmdLine
);



/* Get all window info */
void
lrGetWindowAllInfo(
	HWND hwnd,						// The window handle
	PWINDOWALLINFO p				// Get all window info, See struct WINDOWALLINFO
);



/* Get all handles of parent window */
HWND*								// Return all parent window handles obtained, need free
lrGetWindowHwndSets(
	size_t* _WinQuantity			// Receiving quantity of window handle
);



/* Get all child window handles from the parent window */
HWND*								// Return these child window handles of the parent window, need free
lrGetChildWindowHwndSets(
	HWND _ParentWindow,				// The parent window handle
	size_t* _WinQuantity			// Receiving quantity of child window handles
);



/* Get all parent window handles and all child window handles */
HWND*								// Return window handles, need free
lrGetParentAndChildWindowHwndSets(
	size_t* _wq						// Receiving quantity of all parent window and child window
);



// In file lrFindWindow.c


/* Find a window that meets the conditions, can include child window */
HWND								// Return first window
lrFindWindow(
	lrFIND_WINDOW_TYPE _WindowType,	// See struct lrFIND_WINDOW_TYPE
	BOOL _IncludeChildWindow		// If the parameter is TRUE, include child window
);



/* Find a window for meet all requirement, (the parent window is preferred) */
HWND								// Return first window
lrFindWindowEx(
	const char* const _WindowClass,	// Window class name, can be NULL
	const char* const _WindowTitle,	// Window title text, can be NULL
	const char* const _WindowPath,	// Path of process, can be NULL
	BOOL _IncludeChildWindow		// If the parameter is TRUE, include child window
);


// In file lrCopyWindow.c


/* Copy a window */
HWND								// Return to the new window created
lrCopyWindow(
	HWND _hwnd,						// Window to be copied
	const CHAR _newclass[MAXLEN_WIN_CLASS]	// New class name of the new window, can be NULL
);



// In file lrWindowMotion.c



/* Move the window (Increase on the original), compatible child window */
BOOL
lrMoveWindow(
	HWND _hwnd,
	int _x,
	int _y
);



/* Zoom the window (Increase on the original), compatible child window */
BOOL
lrZoomWindow(
	HWND _hwnd,
	int _w,
	int _h
);



/* Get the window rect, compatible child window */
BOOL
lrGetWindowPos(
	HWND _hwnd,
	int* _x,
	int* _y,
	int* _w,
	int* _h,
	GWP_LR_ORIGINALPOINT _OriginalPoint
);



/* Set vertex coordinates of the window, compatible child window */
BOOL
lrSetWindowPos(
	HWND _hwnd,
	int _x,
	int _y,
	GWP_LR_ORIGINALPOINT _OriginalPoint
);



/* Print text and image to the window. */
BOOL
lrWindowPrint(
	HWND			 _hwnd,			// Window handle
	int				 _x,			// X-coordinate in the window
	int				 _y,			// Y-coordinate in the window
	LPCSTR			 _Text,			// String of text. If NULL, do not print text
	const ALIAS_OUT* _OutAlias		// See ALIAS_OUT. If NULL, call the lrGetAliasDef() function to obtain default settings
);

BOOL
lrWindowDrift(
	HWND _hwnd,
	int _x,
	int _y,
	BYTE _speed
);

BOOL
lrWindowDrifts(
	HWND _hwnd,
	size_t _quantity,
	int _x[],
	int _y[],
	BYTE _speed[]
);

BOOL
lrWindowFollowCursor(
	HWND _hwnd,
	int _x,
	int _y,
	BOOL _switch
);

BOOL
lrWindowWiggle(
	HWND _hwnd,
	UINT _range,
	UINT _freq,
	BOOL _switch
);

BOOL
lrWindowAside(
	HWND _hwnd,
	BYTE _speed,
	UINT _retainX,
	UINT _retainY,
	BOOL _switch
);
	

/* Move window and cursor after collision */
BOOL
lrWindowAndCursorMoveAfterCollide(
	HWND _hwnd,											// The window handle
	lrWINDOW_AND_CURSOR_MOVE_AFTER_COLLIDE_MODE _mode,	// Set mode of move
	UINT _distance,										// Knockback distance after collision
	BOOL _SpringBack,									// If is TRUE, it will automatically rebound when crossing the screen boundary
	UINT _SpringBackPixel,								// Number of extra pixels when rebounding
	BOOL _switch										// Stop if is FALSE, else start runing
);


// In file lrCursorMotion.c


BOOL
lrCursorDrift(
	int _x,
	int _y,
	BYTE _speed
);

BOOL
lrCursorDrifts(
	size_t _quantity,
	int* _x,
	int* _y,
	BYTE _speed[]
);

BOOL
lrCursorWiggle(
	UINT _range,
	UINT _freq,
	BOOL _switch
);



// In file lrSendString.c


/* Send a word message to window
* 
*  Parameter 2: ANSI consecutive double-byte characters
*  If ANSI double-byte characters appear, use the following writing method:
* 
*  char Str[3] = {0xC1, 0xB5, '\0'};
*  if (lrSendWordA(hWnd, (short)((((short)(Str[0]) & 0x00ff) << 8) | (short)(Str[1]) & 0x00ff)))
*  {
*      ...
*  }
* 
*  Determine whether a character is a double-byte character:
* 
*  if ((Str[i] & 0x80) && (Str[i + 1] & 0x80)) { }
*/
BOOL
lrSendWordA(
	HWND  hWnd,						// Window handle
	short nWord						// A word message
);

/* wchar_t version of lrSendWordA */
BOOL
lrSendWordW(
	HWND hWnd,
	wchar_t wcWord
);

/* Send a text message to window */
BOOL
lrSendStringA(
	HWND hWnd,						// Window handle
	const char* const lpText		// A text message
);

/* wchar_t version of lrSendStringA */
BOOL
lrSendStringW(
	HWND hWnd,
	const wchar_t* const lpwText
);



#ifdef UNICODE
#define lrSendWord					lrSendWordW
#define lrSendString				lrSendStringW
#else
#define lrSendWord					lrSendWordA
#define lrSendString				lrSendStringA
#endif

	

	

	


#ifdef __cplusplus
}
#endif


#endif				// _LR_WindowDo_H_