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


/* Ĭ�ϵ���Ļˢ���� */
#define FREQUENCY_SCREEN_REFRESH	50
/* Ĭ�ϵ��ƶ�ˢ���� */
#define FREQUENCY_MOVE_REFRESH		50
/* Ĭ�ϵĹ���ˢ���� */
#define FREQUENCY_FUNCTION_REFRESH	200



/* lrWindowWiggle(): �𶯵����ڡ�ÿ�ı���ٴ�λ��У׼һ��ԭ��λ�ã��������̫�󣬻᳤ʱ���ֹ�����ƶ� */
#define lrWINDOWWIGGLE_CYCLE		5
#define lrCURSORWIGGLE_CYCLE		5


/* lrWindowAside(): �߿���ݲ�� */
#define lrWINDOWASIDE_JAMBALW		7


/* __int64 �����������󳤶� (����'\0') */
#define INT64_INPUT_MAX_LEN			(18 + 1)
#define INT64_OUTPUT_MAX_LEN		( 9 + 1)



/* lrCopyWindow() : Ĭ��������ǰ׺ */
#define lrCOPYWINDOW_NEWCLASSPREFIX	"_lr_copy_"



/*
* lrCreateBlankBox(): �հ׿�һЩĬ�����ԡ�
*/
/* ���� */
#define lrCLASSNAME_BLANKBOX		"lr_wdo_BlankBoxClass"
/* ���� */
#define lrTITLETEXT_BLANKBOX		"lrBlankBox"
/*
* lrCreateTextBox(): �ı���һЩĬ�����ԡ�
*/
/* ���� */
#define lrCLASSNAME_TEXTBOX			"lr_wdo_TextBoxClass"
/* ���� */
#define lrTITLETEXT_TEXTBOX			"lrTextBox"
/*
* lrCreateInfoBox(): ��Ϣ��һЩĬ�����ԡ�
*/
/* ���� */
#define lrCLASSNAME_INFOBOX			"lr_wdo_InfoBoxClass"
/* ���� */
#define lrTITLETEXT_INFOBOX			"lrInfoBox"
/*
* lrCreateInfoBox(): ��־��һЩĬ�����ԡ�
*/
/* ���� */
#define lrCLASSNAME_LOGBOX			"lr_wdo_LogBoxClass"
/* ���� */
#define lrTITLETEXT_LOGBOX			"lrLogBox"
/*
* lrCreateImageBox(): ͼƬ��һЩĬ�����ԡ�
*/
/* ���� */
#define lrCLASSNAME_IMAGEBOX		"lr_wdo_ImageWindowClass"
/*
* lrCreateBlueBox(): ������һЩĬ�����ԡ�
*/
/* ���� */
#define lrCLASSNAME_BLUEBOX			"lr_wdo_BlueBoxClass"
/* ���� */
#define lrTITLETEXT_BLUEBOX			"lrBlueBox"


/* Ϊֵ����˫���� */
#define lr_TO_STR(value)						#value


/* ��ͨ���û������ʮ��������ת���������� COLORREF��
* ��Ϊ COLORREF ����ԭɫ������ʵ�� BGR��
* ���Դ˺����ڽ� B �� R ����λ�á�
* 
* ���� lr_TO_RGB(0x112244)
* �õ��Ľ������ 0x442211
*/
#define lr_TO_RGB(color)						(COLORREF)(((color << 16) & 0xFFFFFF) | (color & 0x00FF00) | ((color & 0xFF0000) >> 16))

/* ��ʼ������� */
#define lr_srand()								srand((unsigned int)GetCurrentTime() + rand())


/* ������ֵ��val���ĳ����� ��Сֵ(MinVal) �� ���ֵ(MaxVal) ֮�� */
#define lr_LIMIT_VALSIZE(val,MinVal,MaxVal)		(val < MinVal ? val = MinVal : (val > MaxVal ? val = MaxVal : val))

/* �����ַ�����str�������� maxlen ���ڣ�������ض� */
#define lr_LIMIT_STRLEN(str,maxlen)				(strlen(str) > (size_t)maxlen ? str[maxlen - 1] = '\0' : 0)

/* ���������ģ�塣
* 
* ����:
*		cHeight    |  ����߶�
*		cWidth     |  ������
*		cWeight    |  �����ϸ
*		bItalic    |  ��б
*		bUnderline |  �»���
*		bStrikeOut |  ɾ����
*		pszFaceName|  ��������
* 
* ����ֵ:
*		���� HFONT ���͵���������
*/
#define lr_CreateFont(cHeight, cWidth, cWeight, bItalic, bUnderline, bStrikeOut, pszFaceName) \
												CreateFontA(-((int)cHeight), -((int)cWidth), 0, 0, (int)cWeight, (BOOL)bItalic, (BOOL)bUnderline, (BOOL)bStrikeOut, \
													DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH | FF_DONTCARE,\
													pszFaceName)

/* ���ݸ��ٲ����� lrLOGFONT ���ͣ�����Ҫȡ��ַ��(&) */
#define lr_CreateFontIndirect(logFont)			lr_CreateFont(logFont.lfHeight, logFont.lfWidth, logFont.lfWeight, logFont.lfItalic, logFont.lfUnderline, logFont.lfStrikeOut, logFont.lfFaceName)



typedef struct FSIZE
{
	FLOAT fw;
	FLOAT fh;
}FSIZE, * PFSIZE;

/* ������ lr_CreateFontIndirect() �� LOGFONT �ṹ�� */
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


/* lrCreateInfoBox(): Ҫ��ʾ����Ϣ�򴰿����� */
typedef enum INFOBOX_WINDOWTYPE
{
	infobox_s_ct,				// ��̬��ǰ����
	infobox_s_par,				// ��̬������
	infobox_s_top,				// ��̬���㴰��
	infobox_s_desk,				// ��̬���洰��
	infobox_s_cur,				// ��̬�α괰��
	infobox_s_cp,				// ��̬�α길����

	infobox_v_ct,				// ��̬��ǰ����
	infobox_v_par,				// ��̬������
	infobox_v_top,				// ��̬���㴰��
	infobox_v_desk,				// ��̬���洰��
	infobox_v_cur,				// ��̬�α괰��
	infobox_v_cp				// ��̬�α길����
	
}INFOBOX_WINDOWTYPE;


typedef enum WINCODE { WINCODE_UNICODE, WINCODE_ASCII } WINCODE;

/* ����������Ϣ��ʹ�� lrGetWindowAllInfo() ��ֵ */
typedef struct WINDOWALLINFO
{
	HWND		ID;									// ���ھ��
	DWORD		PID;								// ���������Ľ��̵�PID
	CHAR		TITLE[MAXLEN_WIN_TITLE];			// ���ڱ����ı�
	CHAR		CLASS[MAXLEN_WIN_CLASS];			// ��������
	CHAR		PPATH[MAX_PATH];					// �������ڵĽ��̵��ļ�·��
	HWND		PARENT;								// ���ڵĸ�����
	WINCODE		CODE;								// ���ڵı���
	RECT		VERTEX;								// ���ڵĶ���
	RECT		CVERTEX;							// ���ڵĶ��㣨�ڸ������У�
	SIZE		WINSIZE;							// ���ڵĿ��
	FSIZE		DPIZOOM;							// ���ڵ�DPI���ű���
	BOOL		SHOW;								// �����Ƿ���ʾ

	POINT		CURPOS;								// �α�����
	COLORREF	CURCOLOR;							// �α��������ɫ
	POINT		CURINWINPOS;						// �α��ڿͻ����е�����

	LONG		STYLE;								// ���ڷ��
	LONG		STYLEEX;							// ������չ���
	LONG		CLASSSTYLE;							// ��������
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
	GWP_LR_ORIGINALPOINT_SCREEN = 0,	// ����ĻΪԭ��
	GWP_LR_ORIGINALPOINT_PARENT = 1		// �Ը�����Ϊԭ�㣨���û�и�����������ĻΪԭ�㣩

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


/* ���ں��α���ײʱ�������¼�
* lrWindowAndCursorMoveAfterCollide()
*/
typedef enum lrWINDOW_AND_CURSOR_MOVE_AFTER_COLLIDE_MODE
{
	WINDOWMOTION_WINDOWANDCURSORCOLLIDE_PUSHMOVE = 0,	// �α��ƽ������ƶ�
	WINDOWMOTION_WINDOWANDCURSORCOLLIDE_MOVEWINDOW,		// �α���˴���
	WINDOWMOTION_WINDOWANDCURSORCOLLIDE_MOVECURSOR,		// ���ڻ����α�
	WINDOWMOTION_WINDOWANDCURSORCOLLIDE_REVERSEMOVE		// ���ں��α귴���ƶ�

}lrWINDOW_AND_CURSOR_MOVE_AFTER_COLLIDE_MODE, * lrWINDOW_AND_CURSOR_MOVE_AFTER_COLLIDE_MODE_PTR;


/* lrCreateWindow() : ����һ���´��ڲ�������Ϣ�Ľ��� */
typedef struct CWE
{
	/* ���������������ڲ�ʹ�� */

	volatile HWND hwnd;			// ���ڴ�����ɺ󷵻صľ��
	volatile BOOL done;			// ���ڴ������ʱ���Ϊ true
	
	BOOL CencelShow;			// ���Ϊ TRUE ��ȡ��������ʾ

	/* ͬ CreateWindowEx() ����
	* https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa?redirectedfrom=MSDN&f1url=%3FappId%3DDev16IDEF1%26l%3DZH-CN%26k%3Dk(CreateWindowEx)%3Bk(DevLang-C%252B%252B)%3Bk(TargetOS-Windows)%26rd%3Dtrue
	*/
	
	DWORD		dwExStyle;							// ��չ���ڷ�� https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
	CHAR		lpClassName[MAXLEN_WIN_CLASS];		// ��������
	CHAR		lpWindowName[MAXLEN_WIN_TITLE];		// ���ڱ���
	DWORD		dwStyle;							// ���ڷ��		https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles?redirectedfrom=MSDN&f1url=%3FappId%3DDev16IDEF1%26l%3DZH-CN%26k%3Dk(WINUSER%252FWS_CHILD)%3Bk(WS_CHILD)%3Bk(DevLang-C%252B%252B)%3Bk(TargetOS-Windows)%26rd%3Dtrue
	INT			X;									// ��ʼ��������
	INT			Y;									// ��ʼ��������
	INT			nWidth;								// ��ʼ���
	INT			nHeight;							// ��ʼ�߶�
	HWND		hWndParent;							// �����ھ��
	HMENU		hMenu;								// �˵����
	HINSTANCE	hInstance;							// ģ��ʵ�����
	LPVOID		lpParam;							// ���� WM_CREATE ʱ�� lParam ����
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