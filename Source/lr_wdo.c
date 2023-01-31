// lr_wdo.c -- Set up the running environment of wdo
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include <locale.h>

#include "../Head/lrWindowDo.h"
#include "../Head/lr_wdo.h"
#include "../Head/wdo_instructions.h"

#pragma warning (disable : 6308)
#pragma warning (disable : 6387)

#pragma warning (disable : 4477)
#pragma warning (disable : 4313)
#pragma warning (disable : 6273)

#define ID_WINDOWCONTROL_INPUT		0
#define ID_WINDOWCONTROL_OUTPUT		1
#define ID_WINDOWCONTROL_BUTTON		2

static BOOL wdoAssociatedSuffix();			// 关联后缀名，设置脚本文件打开方式

static LRESULT CALLBACK wdoControlExitThreadWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK wdoControlThreadWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


static HWND wdoWindowCommandHwnd;			// 命令框窗口
static HWND wdoWindowOutlogHwnd;			// 日志框窗口
static HWND wdoWindowControlHwnd;			// 控制窗口
static HWND wdoWindowExitControlHwnd;		// 退出控制窗口

static BOOL wdoOutlogSwitch = TRUE;			// 如果为 FALSE，日志框禁止输出

static int  wdoExitCode = 0;
static BOOL wdoExiting = FALSE;


HWND wdo_gethwnd(WDOGETHWND _type)
{
	switch (_type)
	{
	case WDOGETHWND_OUTLOG:			return wdoWindowOutlogHwnd;
	case WDOGETHWND_COMMAND:		return wdoWindowCommandHwnd;
	case WDOGETHWND_CONTROL:		return wdoWindowControlHwnd;
	case WDOGETHWND_EXITCONTROL:	return wdoWindowExitControlHwnd;
	default:						return NULL;
	}
}


BOOL wdo_add_window(HWND _window)
{
	if (wdoWindowControlHwnd && IsWindow(_window))
	{
 		return (BOOL)PostMessageA(wdoWindowControlHwnd, WM_WDO_ADDNEWWINDOW, (WPARAM)_window, 0);
	}
	else return FALSE;
}

BOOL wdo_del_window(HWND _window)
{
	if (wdoWindowControlHwnd && IsWindow(_window))
	{
		return (BOOL)PostMessageA(wdoWindowControlHwnd, WM_WDO_DESTROYWINDOW, (WPARAM)_window, 0);
	}
	else return FALSE;
}

BOOL wdo_add_thread(DWORD _thread)
{
	if (wdoWindowControlHwnd)
	{
		return (BOOL)PostMessageA(wdoWindowControlHwnd, WM_WDO_ADDNEWTHREAD, (WPARAM)_thread, 0);
	}
	else return FALSE;
}

BOOL wdo_del_thread(DWORD _thread)
{
	if (wdoWindowControlHwnd)
	{
		return (BOOL)PostMessageA(wdoWindowControlHwnd, WM_WDO_DESTROYTHREAD, (WPARAM)_thread, 0);
	}
	else return FALSE;
}


void wdo_error(const unsigned __int64 _Line, const unsigned char* const _File, const unsigned char* const _Func)
{
	unsigned char err[500] = "\0";

#if WDO_LANGUAGE
	sprintf(err,
		"There is an error in the current program, which may be due to insufficient memory or other reasons,\n"
		"The pop-up box indicates that there may be no solution to this error for the time being,\n"
		"Or it is not recommended to continue running,\n"
		"If you continue, it may lead to memory leakage or other unknown problems.\n"
		"\n"
		"File : %s\n"
		"Func : %s()\n"
		"Line : %llu\n"
		, _File
		, _Func
		, _Line
	);
#else
	sprintf(err,
		"当前程序发生错误，可能是内存不足或其他原因，\n"
		"弹出此框表示可能暂时并没有对此错误的解决方案，\n"
		"或者不推荐继续运行，\n"
		"如果继续，可能会导致内存泄漏，或者其他未知问题。\n"
		"\n"
		"FILE:  %s\n"
		"FUNC:  %s()\n"
		"LINE:  %llu"
		, _File
		, _Func
		, _Line
	);
#endif

	while (TRUE)
	{
		switch (MessageBoxA(NULL, err, "wdoError", MB_ABORTRETRYIGNORE | MB_ICONSTOP | MB_DEFBUTTON1 | MB_SYSTEMMODAL))
		{
		case IDABORT:	/* 中止 */
			wdo_exit(WDO_EXIT_CODE__ERROR);
			return;

		case IDRETRY:	/* 重试 */
			continue;

		case IDIGNORE:	/* 忽略 */
			return;
		}
	}
	return;

	/*LPVOID lpMsgBuf = NULL;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	MessageBoxA(wdoWindowControlHwnd, lpMsgBuf, 0, 0);*/
}


void wdo_exit(int _ExitCode)
{
	if (wdoExiting == FALSE)
	{
		wdoExiting = TRUE;
		wdoExitCode = _ExitCode;
		SendMessageA(wdoWindowExitControlHwnd, WM_CLOSE, 0, 0);
		SendMessageA(wdoWindowControlHwnd, WM_CLOSE, TRUE, 0);

		exit(_ExitCode);
		return;
	}
	return;
}


/* 请求退出当前进程
* 
* 参数:
*		_ExitCode     |  退出码
*		_AskMode      |  退出模式
* 
* 返回值:
*		TRUE          |  请求成功
*		FALSE         |  请求失败
* 
* 言论:
*		如果请求成功，则不会返回，进程将立即退出。
*/
BOOL wdo_exit_ask(int _ExitCode, WDOEXITASKMODE _AskMode)
{
	BOOL ret = (BOOL)SendMessageA(wdoWindowControlHwnd, WM_WDO_EXITASK, (WPARAM)_ExitCode, (LPARAM)_AskMode);

	if (_AskMode = WDOEXITASK_WAITEXIT) Sleep(INFINITE);

	return ret;
}

BOOL wdo_set_hotkey(const char* const _HotkeyString, size_t _Count, const char** const _Instructions)
{
	WDOINSTRUCTIONS wdoInstructions = { _Count,_Instructions };
	DWORD ErrorCode;
	SetLastError(ErrorCode = (DWORD)SendMessageA(wdoWindowControlHwnd, WM_WDO_SETHOTKEY, (WPARAM)_HotkeyString, (LPARAM)&wdoInstructions));
	return !ErrorCode;
}

/* 设置日志的输出开关
*  返回原来的输出开关
*/
BOOL wdo_outlog_switch(BOOL _Switch)
{
	BOOL OldSwitch = wdoOutlogSwitch;

	wdoOutlogSwitch = _Switch;

	return OldSwitch;
}

void wdo_puts(const char* const _String)
{
	if (wdoOutlogSwitch == FALSE) return;

	/*SendMessageA(wdoWindowOutlogHwnd, EM_SETSEL, -2, -1);
	SendMessageA(wdoWindowOutlogHwnd, EM_REPLACESEL, FALSE, (LPARAM)_String);
	SendMessageA(wdoWindowOutlogHwnd, WM_VSCROLL, SB_BOTTOM, 0);*/
	
	if (_String && _String[0])
	{
		wchar_t* newText = (wchar_t*)malloc((strlen(_String) + 1) * sizeof(wchar_t));
		if (newText)
		{
			size_t wcsize = mbstowcs(newText, _String, strlen(_String) + 1);

			for (size_t i = 0; i < wcsize; ++i)
			{
#pragma warning (suppress : 6385)
				SendMessageW(wdoWindowOutlogHwnd, WM_CHAR, (WPARAM)newText[i], 0);
			}
			free(newText);
		}
	}
}

void wdo_puts_upper(const char* const _String)
{
	if (wdoOutlogSwitch == FALSE) return;

	if (_String && _String[0])
	{
		wchar_t* newText = (wchar_t*)malloc((strlen(_String) + 1) * sizeof(wchar_t));
		if (newText)
		{
			size_t wcsize = mbstowcs(newText, _String, strlen(_String) + 1);

			for (size_t i = 0; i < wcsize; ++i)
			{
#pragma warning (suppress : 6385)
				SendMessageW(wdoWindowOutlogHwnd, WM_CHAR, (WPARAM)towupper(newText[i]), 0);
			}
			free(newText);
		}
	}
}

void wdo_puts_lower(const char* const _String)
{
	if (wdoOutlogSwitch == FALSE) return;

	if (_String && _String[0])
	{
		wchar_t* newText = (wchar_t*)malloc((strlen(_String) + 1) * sizeof(wchar_t));
		if (newText)
		{
			size_t wcsize = mbstowcs(newText, _String, strlen(_String) + 1);

			for (size_t i = 0; i < wcsize; ++i)
			{
#pragma warning (suppress : 6385)
				SendMessageW(wdoWindowOutlogHwnd, WM_CHAR, (WPARAM)towlower(newText[i]), 0);
			}
			free(newText);
		}
	}
}

void wdo_putc(int c)
{
	if (wdoOutlogSwitch == FALSE) return;

	wchar_t newWord = L'\0';
	mbtowc(&newWord, &(char)c, MB_CUR_MAX);
	SendMessageW(wdoWindowOutlogHwnd, WM_CHAR, newWord, 0);
}

void wdo_cls()
{
	SetWindowTextA(wdoWindowOutlogHwnd, "");
	UpdateWindow(wdoWindowOutlogHwnd);
}

void wdo_setcolor(COLORREF color)
{
	SendMessageA(wdoWindowControlHwnd, WM_WDO_SETCOLOR, (WPARAM)color, 0);
}


HWND wdo_create()
{
	/* 检查接收退出快捷键的窗口是否已存在，如果不存在则创建 */
	if (!FindWindowA(WDO_WINDOW_CLASS_WDOEXIT, WDO_WINDOW_CLASS_WDOEXIT))
	{
		WNDCLASS wndclass = { 0 };

		wndclass.lpszClassName = WDO_WINDOW_CLASS_WDOEXIT;
		wndclass.hbrBackground = NULL;
		wndclass.hCursor = IA_ALIAS_HCURSOR;
		wndclass.hIcon = IA_ALIAS_HICON;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = 0;
		wndclass.lpfnWndProc = wdoControlExitThreadWindowProc;
		wndclass.lpszMenuName = NULL;
		wndclass.style = CS_NOCLOSE;

		RegisterClassA(&wndclass);

		CWE cwe = { 0 };
		cwe.dwExStyle = WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_NOREDIRECTIONBITMAP;
		strcpy(cwe.lpClassName, WDO_WINDOW_CLASS_WDOEXIT);
		strcpy(cwe.lpWindowName, WDO_WINDOW_CLASS_WDOEXIT);
		cwe.dwStyle = WS_MINIMIZE;
		cwe.X = 0;
		cwe.Y = 0;
		cwe.nWidth = 0;
		cwe.nHeight = 0;
		cwe.hWndParent = 0;
		cwe.hMenu = 0;
		cwe.hInstance = 0;
		cwe.lpParam = 0;
		cwe.CencelShow = TRUE;

		wdoWindowExitControlHwnd = lrCreateWindow(&cwe);
	}

	/* 创建主控制（背景）窗口 */
	WNDCLASS wndclass = { 0 };

	wndclass.lpszClassName = WDO_WINDOW_CLASS_WDOCONTROL;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = LoadImageA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDC_CURSOR_CONSOLE), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR);
	wndclass.hIcon = IA_ALIAS_HICON;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = (HINSTANCE)GetModuleHandleA(NULL);
	wndclass.lpfnWndProc = wdoControlThreadWindowProc;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	RegisterClassA(&wndclass);

	CWE cwe = { 0 };
	cwe.dwExStyle = WS_EX_ACCEPTFILES;
	strcpy(cwe.lpClassName, WDO_WINDOW_CLASS_WDOCONTROL);
	strcpy(cwe.lpWindowName, WDO_CONSOLE_TITLE);
	cwe.dwStyle = WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_VISIBLE;
	cwe.X = (GetSystemMetrics(SM_CXSCREEN) / 2) - WDOWINDOWSIZE_WIDTH / 2;
	cwe.Y = (GetSystemMetrics(SM_CYSCREEN) / 2) - WDOWINDOWSIZE_HEIGHT / 2;
	cwe.nWidth = WDOWINDOWSIZE_WIDTH + WDOWINDOWSIZE_JAMB;
	cwe.nHeight = WDOWINDOWSIZE_HEIGHT;
	cwe.hWndParent = 0;
	cwe.hMenu = 0;
	cwe.hInstance = 0;
	cwe.lpParam = 0;
	
	wdoWindowControlHwnd = lrCreateWindow(&cwe);
	InvalidateRect(wdoWindowCommandHwnd, NULL, FALSE);
	InvalidateRect(wdoWindowOutlogHwnd, NULL, FALSE);

	return wdoWindowControlHwnd;
}



static LRESULT CALLBACK wdoControlExitThreadWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* 记录所有要关闭的窗口 */
	static HWND* CurrentWindowHwndSet = NULL;
	/* 窗口的数量 */
	static UINT CurrentWindowCount = 0;

	switch (uMsg)
	{
	case WM_CREATE:

		/* 设置快捷键 */
		if (RegisterHotKey(hWnd, 1, WDO_EXIT_HOTKEY_MOD, WDO_EXIT_HOTKEY_VK) == FALSE) return -1;

		/* 隐藏游标 */
		ShowCursor(FALSE);

		if (CurrentWindowHwndSet = (HWND*)malloc(sizeof(HWND))) CurrentWindowHwndSet[0] = NULL;

		return 0;

	case WM_HOTKEY:

		for (size_t i = 0; i < CurrentWindowCount; ++i)
		{
			DWORD pid;
			GetWindowThreadProcessId(CurrentWindowHwndSet[i], &pid);
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
			TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
		}
		
		free(CurrentWindowHwndSet);
		CurrentWindowHwndSet = NULL;
		wdo_exit(WDO_EXIT_CODE__HKEY);
		return 0;

	case WM_WDO_ADDEXITWINDOW:

		if (CurrentWindowHwndSet && (HWND)wParam != wdoWindowControlHwnd)
		{
			CurrentWindowHwndSet[CurrentWindowCount++] = (HWND)wParam;
			CurrentWindowHwndSet = (HWND*)realloc(CurrentWindowHwndSet, (size_t)(CurrentWindowCount + 1) * sizeof(HWND));
		}
		return 0;

	case WM_DESTROY:
	{
		/* 注销快捷键 */
		UnregisterHotKey(hWnd, 1);

		if (CurrentWindowCount)
		{
			/* 新的退出窗口 */
			HWND newExitControlWindow = (HWND)SendMessageA(CurrentWindowHwndSet[0], WM_WDO_CREATEEXIT, 0, 0);
			if (newExitControlWindow)
			{
				for (UINT i = 0; i < CurrentWindowCount; ++i)
					SendMessageA(newExitControlWindow, WM_WDO_ADDEXITWINDOW, (WPARAM)CurrentWindowHwndSet[i], 0);
			}
		}
		if (CurrentWindowHwndSet) free(CurrentWindowHwndSet);
		PostQuitMessage(0);
		return 0;
	}
	default:

		return DefWindowProcA(hWnd, uMsg, wParam, lParam);
	}
}

static LRESULT CALLBACK wdoControlThreadWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc = NULL;

	static unsigned __int64 LineNumber = 1;
	static char Command[WDO_COMMAND_MAXLEN];
	static HWND* CurrentPtrhwnd = NULL;
	
	static FSIZE dpi = { 1.00f,1.00f };
	static HANDLE mutex = NULL;

	/* 所有按钮的集合 */
	static HWND buttons[6] = { 0 };

	/* 字体 */
	static HFONT font_command = NULL;
	static HFONT font_outlog = NULL;
	static HFONT font_button = NULL;
	static HFONT font_head = NULL;
	static HFONT font_text = NULL;

	/* 背景图片的编码集合 */
	static LPSTR BackgroundImage_CodeSets[] = {
		MAKEINTRESOURCE(IDC_BITMAP__BACKGROUND_CONSOLE_0),
		MAKEINTRESOURCE(IDC_BITMAP__BACKGROUND_CONSOLE_1),
		MAKEINTRESOURCE(IDC_BITMAP__BACKGROUND_CONSOLE_2)
	};
	/* 背景图片的当前编号 */
	static UINT BackgroundImage_CurrentNumber = 0;
	/* 背景图片是否显示 */
	static BYTE BackgroundImage_ShowMode = TRUE;
	/* 如果背景颜色不显示则使用该颜色填充 */
	static COLORREF BackgroundImage_Color = RGB(0x00, 0x00, 0x00);


	/* 编辑框的画笔 */
	static HBRUSH EditTextboxBrush = NULL;

	static COLORREF InputColor = RGB(0x00, 0xff, 0x00);
	static COLORREF OutputColor = RGB(0x00, 0xff, 0x00);

	/* 绘画坐标相交线的信息 */
	static struct LAST_POINTS_INFO
	{
		SIZE size;
		POINT pos;
	}lastPointsColorInfo = { { 0,0 },{ 0,0 } };

	/* 保存所有的该进程窗口 */
	static HWND* wdoWindowSet = NULL;
	static ULONG wdoWindowCount = 0;
	
	/* 保存所有的该进程线程 */
	static DWORD* wdoThreadSet = NULL;
	static ULONG wdoThreadCount = 0;

	/* 快捷键管理 */
	static UINT HotkeyCount = 0;
	static WDOINSTRUCTIONS* HotkeyInstructions = NULL;
	static char** HotkeyString = NULL;

	static BOOL ExitAskSwitch = FALSE;
	static BOOL DestroyingNoExit = FALSE;


	enum { INEXTERNAL = 0/*在窗口外*/, INCONSOLE/*在控制台内*/, INCONTROL/*在wdo控制窗口内*/ } CURPOSIN;

	POINT CurrentCursorPosition;
	GetCursorPos(&CurrentCursorPosition);

	RECT CurrentWindowRect;
	GetWindowRect(hWnd, &CurrentWindowRect);

	if (CurrentCursorPosition.y > CurrentWindowRect.top && CurrentCursorPosition.y < CurrentWindowRect.bottom)
	{
		if (CurrentCursorPosition.x > CurrentWindowRect.left && CurrentCursorPosition.x < CurrentWindowRect.right)
		{
			if (CurrentCursorPosition.x > CurrentWindowRect.right - WDOWINDOWSIZE_JAMB)
				CURPOSIN = INCONTROL;
			else
				CURPOSIN = INCONSOLE;
		}
		else CURPOSIN = INEXTERNAL;
	}
	else CURPOSIN = INEXTERNAL;

	

	switch (uMsg)
	{
	case WM_CREATE:
	{
		SendMessageA(FindWindowA(WDO_WINDOW_CLASS_WDOEXIT, WDO_WINDOW_CLASS_WDOEXIT), WM_WDO_ADDEXITWINDOW, (WPARAM)(wdoWindowControlHwnd = hWnd), 0);

		hdc = GetDC(hWnd);
		dpi = lrGetWindowDpiZoom(hWnd);
		mutex = CreateMutexA(NULL, FALSE, NULL);

		HMODULE hInstance = GetModuleHandleA(NULL);

		char winstr[20];
		sprintf(winstr, WDOINS_PTR " -I %X", hWnd);
		CurrentPtrhwnd = lrWindowDo(winstr, WDOECHO_DEF).PtrWindow;

		const char* Text[] = {
			WDOBUTTONNAME_OPEN_CLS,
			WDOBUTTONNAME_OPEN_RESTORE,
			WDOBUTTONNAME_OPEN_SETTOP,
			WDOBUTTONNAME_OPEN_BKMODE,
			WDOBUTTONNAME_OPEN_CHANGEBK,
			WDOBUTTONNAME_OPEN_SCROLLBAR
		};

		font_command = lr_CreateFont(20, 0, 400, FALSE, FALSE, FALSE, "Cascadia Code");
		font_outlog = lr_CreateFont(15, 0, 300, FALSE, FALSE, FALSE, "Cascadia Mono");
		font_button = lr_CreateFont(14, 0, 200, FALSE, FALSE, FALSE, "Courier new");
		font_head = lr_CreateFont(15, 0, 200, FALSE, FALSE, FALSE, "Cascadia Code");
		font_text = lr_CreateFont(13, 0, 400, FALSE, FALSE, FALSE, "Cascadia Mono");
		
		EditTextboxBrush = CreateSolidBrush(BackgroundImage_Color);
		
		wdoWindowCommandHwnd = CreateWindowExA(WS_EX_LAYERED
			, "Edit"
			, NULL
			, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL
			, WDOWINDOWSIZE_INPUT_POSX, WDOWINDOWSIZE_INPUT_POSY
			, WDOWINDOWSIZE_WIDTH - WDOWINDOWSIZE_INPUT_POSX - WDOWINDOWSIZE_INPUT_ENDX, WDOWINDOWSIZE_INPUT_HEIGHT
			, hWnd
			, ID_WINDOWCONTROL_INPUT
			, hInstance
			, NULL
		);
		SendMessageA(wdoWindowCommandHwnd, WM_SETFONT, (WPARAM)font_command, 0);
		SetLayeredWindowAttributes(wdoWindowCommandHwnd, RGB(0x00, 0x00, 0x00), 0xC0, LWA_ALPHA);
		Edit_LimitText(wdoWindowCommandHwnd, sizeof(Command));
		ShowWindow(wdoWindowCommandHwnd, SW_SHOW);
		UpdateWindow(wdoWindowCommandHwnd);
		
		wdoWindowOutlogHwnd = CreateWindowExA(WS_EX_ACCEPTFILES | WS_EX_LAYERED
			, "Edit"
			, NULL
			, (WS_CHILD | WS_VISIBLE /*| WS_VSCROLL | WS_HSCROLL*/) | (ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_WANTRETURN)
			, WDOWINDOWSIZE_OUTPUT_POSX, WDOWINDOWSIZE_INPUT_POSY + WDOWINDOWSIZE_INPUT_HEIGHT + WDOWINDOWSIZE_IO_SPACE
			, WDOWINDOWSIZE_WIDTH, WDOWINDOWSIZE_HEIGHT - WDOWINDOWSIZE_INPUT_HEIGHT - WDOWINDOWSIZE_IO_SPACE - WDOWINDOWSIZE_INPUT_POSY
			, hWnd
			, (HMENU)ID_WINDOWCONTROL_OUTPUT
			, hInstance
			, NULL
		);
		SendMessageA(wdoWindowOutlogHwnd, WM_SETFONT, (WPARAM)font_outlog, 0);
		SetLayeredWindowAttributes(wdoWindowOutlogHwnd, BackgroundImage_Color, 0xff, LWA_COLORKEY);
		Edit_LimitText(wdoWindowOutlogHwnd, ULLONG_MAX);
		ShowWindow(wdoWindowCommandHwnd, SW_SHOW);
		UpdateWindow(wdoWindowCommandHwnd);

		/* 循环创建按钮控件 */
		for (size_t i = 0; i < sizeof(buttons) / sizeof(*buttons); ++i)
		{
			buttons[i] = CreateWindowExA(0L
				, "Button"
				, (LPCSTR)Text[i]
				, (WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_GROUP)
				| (BS_CENTER/*文本居中对齐*/ | BS_MULTILINE/*自动换行*/ | BS_VCENTER/*文本垂直居中*/ | BS_DEFPUSHBUTTON)
				, WDOWINDOWSIZE_WIDTH + WDOWINDOW_BUTTON_POSX
				, WDOWINDOW_BUTTON_POSY + WDOWINDOW_BUTTON_JAMB * (int)i
				, WDOWINDOW_BUTTON_WIDTH
				, WDOWINDOW_BUTTON_HEIGHT
				, hWnd
				, (HMENU)(ID_WINDOWCONTROL_BUTTON + i)
				, hInstance
				, NULL
			);
			ShowWindow(buttons[i], SW_SHOW);
			UpdateWindow(buttons[i]);

			SendMessageA(buttons[i], WM_SETFONT, (WPARAM)font_button, 0);
		}

		/* 用于刷新窗口右下角的颜色和信息框 */
		SetTimer(hWnd, (UINT_PTR)hWnd, 30, NULL);

		lastPointsColorInfo.size.cx = WDOWINDOWSIZE_WIDTH;
		lastPointsColorInfo.size.cy = WDOWINDOWSIZE_HEIGHT;
		
		lastPointsColorInfo.pos = lrGetCursorInWindowPosition(hWnd);

		return 0;
	}
	case WM_PAINT:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		ValidateRect(hWnd, &rect);
		
		
		/* 绘制背景图片 */
		if (BackgroundImage_ShowMode)
		{
			HDC bitmapDC = CreateCompatibleDC(hdc);
			HBITMAP ConsoleBackground = LoadImageA(GetModuleHandleA(NULL), BackgroundImage_CodeSets[BackgroundImage_CurrentNumber], IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
			SelectObject(bitmapDC, ConsoleBackground);
			SetStretchBltMode(hdc, HALFTONE);

			if ((rect.right - rect.left - WDOWINDOWSIZE_JAMB) <= WDOWINDOW_BK_WIDTH && rect.bottom - rect.top <= WDOWINDOW_BK_HEIGHT)
			{
				BitBlt(hdc, 0, 0, rect.right - rect.left - WDOWINDOWSIZE_JAMB + 16, rect.bottom - rect.top, bitmapDC, 0, 0, SRCCOPY);
			}
			else
			{
				StretchBlt(hdc
					, rect.left
					, rect.top
					, rect.right - rect.left - WDOWINDOWSIZE_JAMB + 16
					, rect.bottom - rect.top
					, bitmapDC
					, 0
					, 0
					, WDOWINDOW_BK_WIDTH
					, WDOWINDOW_BK_HEIGHT
					, SRCCOPY);
			}
			DeleteObject(ConsoleBackground);
			DeleteDC(bitmapDC);

		}
		else
		{
			HBRUSH ConsoleBackBrush = CreateSolidBrush(BackgroundImage_Color);
			SelectObject(hdc, ConsoleBackBrush);
			PatBlt(hdc, rect.left, rect.top, rect.right - rect.left - WDOWINDOWSIZE_JAMB + 16, rect.bottom - rect.top, PATCOPY);
			DeleteObject(ConsoleBackBrush);
		}

		HBRUSH RightBackBrush = CreateSolidBrush(WDOWINDOWCOLOR);
		SelectObject(hdc, RightBackBrush);
		PatBlt(hdc, (rect.right - WDOWINDOWSIZE_JAMB) + 16, rect.top, WDOWINDOWSIZE_JAMB, rect.bottom - rect.top, PATCOPY);
		DeleteObject(RightBackBrush);

		if (BackgroundImage_Color == RGB(0x00, 0x00, 0x00))
		{
			SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
		}
		else
		{
			SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
		}
		char str[30] = "";
		SelectObject(hdc, font_head);
		SetBkMode(hdc, TRANSPARENT);
		TextOutA(hdc, WDOWINDOWSIZE_OUTPUT_POSX, WDOWINDOWSIZE_INPUT_POSY + 2, str, sprintf(str, "#%2llu | _%X_ > ", LineNumber, CurrentPtrhwnd ? *CurrentPtrhwnd : 0));

		UpdateWindow(wdoWindowOutlogHwnd);


		/* 重绘按钮控件 */
		for (size_t i = 0; i < sizeof(buttons) / sizeof(buttons[0]); ++i)
		{
			InvalidateRect(buttons[i], NULL, FALSE);
		}

		return 0;
	}
	case WM_COMMAND:
	{
		unsigned int wmId = LOWORD(wParam);
		unsigned int wmEvent = HIWORD(wParam);
		
		switch (wmId)
		{
		case ID_WINDOWCONTROL_INPUT:
		{
			switch (wmEvent)
			{
			case EN_CHANGE:
			{
				int LineCount = (int)SendMessageA(wdoWindowCommandHwnd, EM_GETLINECOUNT, 0, 0);

				if (LineCount > 1)
				{
					int len = GetWindowTextA(wdoWindowCommandHwnd, Command, sizeof(Command));
					if (len = sizeof(Command) - 1) Command[sizeof(Command) - 1] = '\0';

					if (LineCount == 2 && Command[len - 1] != '\n')
					{
						BOOL found = FALSE;
						for (int i = 0; i < len; ++i)
						{
							if (found)
							{
								Command[i] = Command[i + 2];
							}
							else if (Command[i] == '\r')
							{
								found = TRUE;
								Command[i] = Command[i + 2];
							}
						}
					}

					SetWindowTextA(wdoWindowCommandHwnd, "");
					UpdateWindow(wdoWindowCommandHwnd);
					wdo_cls();

					lrWindowDo(Command, WDOECHO_DEF);
					++LineNumber;

					PostMessageA(hWnd, WM_PAINT, 0, 0);
				}
				break;
			}
			case EN_MAXTEXT:
			{
				wdo_puts(wdo_language("~Error: The character count in the command area has reached the upper limit!", "~错误：命令区的字符计数已达上限！") "\n");
				break;
			}
			default:
				DefWindowProcA(hWnd, uMsg, wParam, lParam);
				break;
			}
			break;
		}
		case ID_WINDOWCONTROL_OUTPUT:
		{
			switch (wmEvent)
			{
			default:
				DefWindowProcA(hWnd, uMsg, wParam, lParam);
				break;
			}
			break;
		}
		case ID_WINDOWCONTROL_BUTTON:
		{
			SendMessageA(hWnd, WM_PAINT, 0, 0);
			wdo_cls();

			wdo_log(NULL, OUTLOG_NORMAL, "LoveRain WindowDoSctipt.exe [VER 1.0] Hello Mr.%s!\n", getenv("UserName"));

			wdo_log(NULL, OUTLOG_NORMAL, "Screen resolution is (%d,%d), DPI = [%.2fW, %.2fH]\n"
				, GetDeviceCaps(hdc, DESKTOPHORZRES)
				, GetDeviceCaps(hdc, DESKTOPVERTRES)
				, lrGetWindowDpiZoom(wdo_gethwnd(WDOGETHWND_CONTROL)).fw
				, lrGetWindowDpiZoom(wdo_gethwnd(WDOGETHWND_CONTROL)).fh);

			wdo_log(NULL, OUTLOG_NORMAL, "LANGUAGE = %s, IP = %s\n\n", setlocale(LC_ALL, ""), lrGetMyIPv4(NULL));

			wdo_log(NULL, OUTLOG_NORMAL,
				"Use cmd.exe to bring in the path of the (" WDO_SCRIPT_SUFFIX ") file to run the script\n"
				"Use the global shortcut <" WDO_EXIT_HOTKEY_STR "> to exit all running " WDO_EXE_FILE " processes\n"
				"\n"
				"Use the help instruction to obtain the usage mode\n");
			wdo_log(NULL, OUTLOG_NORMAL,
				"\n\n"
				"This project was written in 2022.01.18 - 19:12:36\n"
				"Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.\n"
			);
			SetFocus(wdoWindowCommandHwnd);
			break;
		}
		case ID_WINDOWCONTROL_BUTTON + 1:
		{
			MoveWindow(hWnd
				, (GetSystemMetrics(SM_CXSCREEN) / 2) - WDOWINDOWSIZE_WIDTH / 2
				, (GetSystemMetrics(SM_CYSCREEN) / 2) - WDOWINDOWSIZE_HEIGHT / 2
				, WDOWINDOWSIZE_WIDTH + WDOWINDOWSIZE_JAMB
				, WDOWINDOWSIZE_HEIGHT
				, TRUE);
			SetFocus(wdoWindowCommandHwnd);
			break;
		}
		case ID_WINDOWCONTROL_BUTTON + 2:
		{
			/* 判断是否顶置（如果是） */
			if (GetWindowLongA(hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST)
			{
				SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				SetWindowTextA(buttons[2], WDOBUTTONNAME_OPEN_SETTOP);
			}
			else
			{
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				SetWindowTextA(buttons[2], WDOBUTTONNAME_CLOSE_SETTOP);
			}
			SetFocus(wdoWindowCommandHwnd);
			break;
		}
		case ID_WINDOWCONTROL_BUTTON + 3:
		{
			if (BackgroundImage_ShowMode)
			{
				BackgroundImage_ShowMode = FALSE;
				SetWindowTextA(buttons[3], WDOBUTTONNAME_CLOSE_BKMODE);
			}
			else
			{
				BackgroundImage_ShowMode = TRUE;
				SetWindowTextA(buttons[3], WDOBUTTONNAME_OPEN_BKMODE);
			}
			SendMessageA(hWnd, WM_PAINT, 0, 0);
			SetFocus(wdoWindowCommandHwnd);
			break;
		}
		case ID_WINDOWCONTROL_BUTTON + 4:
		{
			if (BackgroundImage_ShowMode)
			{
				/* 更改彩色背景 */

				lr_srand();
				while (TRUE)
				{
					UINT iR = (UINT)rand() % (UINT)(sizeof(BackgroundImage_CodeSets) / sizeof(BackgroundImage_CodeSets[0]));
					if (iR != BackgroundImage_CurrentNumber)
					{
						BackgroundImage_CurrentNumber = iR;
						break;
					}
				}
			}
			else
			{
				/* 更改纯色背景 */

				if (BackgroundImage_Color == RGB(0x00, 0x00, 0x00))
				{
					BackgroundImage_Color = RGB(0xff, 0xff, 0xff);
				}
				else
				{
					BackgroundImage_Color = RGB(0x00, 0x00, 0x00);
				}
				DeleteObject(EditTextboxBrush);
				EditTextboxBrush = CreateSolidBrush(BackgroundImage_Color);
				SetLayeredWindowAttributes(wdoWindowOutlogHwnd, BackgroundImage_Color, 0xff, LWA_COLORKEY);
				InvalidateRect(wdoWindowOutlogHwnd, NULL, TRUE);
				UpdateWindow(wdoWindowOutlogHwnd);
			}
			SendMessageA(hWnd, WM_PAINT, 0, 0);
			SetFocus(wdoWindowCommandHwnd);
			break;
		}
		case ID_WINDOWCONTROL_BUTTON + 5:
		{
			long int style = GetWindowLongA(hWnd, GWL_STYLE);

			if (style & WS_VSCROLL)
			{
				SetWindowLongA(hWnd, GWL_STYLE, style & ~WS_VSCROLL);
				SetWindowTextA(buttons[5], WDOBUTTONNAME_OPEN_SCROLLBAR);
			}
			else
			{
				SetWindowLongA(hWnd, GWL_STYLE, style | WS_VSCROLL);
				SetWindowTextA(buttons[5], WDOBUTTONNAME_CLOSE_SCROLLBAR);

				SetScrollPos(hWnd, SB_VERT, 45, TRUE);
			}

			UpdateWindow(wdoWindowOutlogHwnd);
			SetFocus(wdoWindowCommandHwnd);
			break;
		}
		default:
		{
			DefWindowProcA(hWnd, uMsg, wParam, lParam);
			break;
		}
		}
		
		return 0;
	}
	case WM_TIMER:
	{
		HDC PointDC = GetDC(NULL);
		
		RECT rect;
		GetClientRect(hWnd, &rect);

		COLORREF PointColor = GetPixel(PointDC, (int)((float)CurrentCursorPosition.x * dpi.fw), (int)((float)CurrentCursorPosition.y * dpi.fh));

		/* 获取颜色值，创建笔刷 */
		HBRUSH hBrush = CreateSolidBrush(PointColor);
		SelectObject(hdc, hBrush);
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0x00, 0xff, 0x00));

		SelectObject(hdc, hPen);
		SelectObject(hdc, font_text);

		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, WDOWINDOWCOLOR);

		char OutputText[20] = "\0";

		long int x = rect.right > WDOWINDOWSIZE_JAMB ? rect.right - WDOWINDOWSIZE_JAMB + 25 : 0;
		long int y = rect.bottom - 100;

		SetTextColor(hdc, RGB(0x00, 0xff, 0xff));
		TextOutA(hdc, x, y - 95, OutputText, sprintf(OutputText, wdo_language("Windows #%3u", "窗口 #%3u"), wdoWindowCount));
		TextOutA(hdc, x, y - 75, OutputText, sprintf(OutputText, wdo_language("Threads #%3u", "线程 #%3u"), wdoThreadCount));

		/* 绘制颜色框 */
		Rectangle(hdc, x, y - 55, x + 50, y - 5);

		SetTextColor(hdc, RGB(0xff, 0x00, 0x00));
		TextOutA(hdc, x + 55, y - 55, OutputText, sprintf(OutputText, "%02X", GetRValue(PointColor)));

		SetTextColor(hdc, RGB(0x00, 0xff, 0x00));
		TextOutA(hdc, x + 55, y - 35, OutputText, sprintf(OutputText, "%02X", GetGValue(PointColor)));

		SetTextColor(hdc, RGB(0x00, 0x00, 0xff));
		TextOutA(hdc, x + 55, y - 15, OutputText, sprintf(OutputText, "%02X", GetBValue(PointColor)));


		SetTextColor(hdc, PointColor);
		TextOutA(hdc, x, y, OutputText, sprintf(OutputText, "X- %3d ", CurrentCursorPosition.x));
		TextOutA(hdc, x, y += 15, OutputText, sprintf(OutputText, "Y- %3d ", CurrentCursorPosition.y));

		SetTextColor(hdc, RGB(0xff, 0x00, 0x00));
		TextOutA(hdc, x, y += 15 + 15, OutputText, sprintf(OutputText, "R- %03u", GetRValue(PointColor)));

		SetTextColor(hdc, RGB(0x00, 0xff, 0x00));
		TextOutA(hdc, x, y += 15, OutputText, sprintf(OutputText, "G- %03u", GetGValue(PointColor)));

		SetTextColor(hdc, RGB(0x00, 0x00, 0xff));
		TextOutA(hdc, x , y += 15, OutputText, sprintf(OutputText, "B- %03u", GetBValue(PointColor)));

		DeleteObject(hBrush);
		DeleteObject(hPen);

		ReleaseDC(NULL, PointDC);


		/* 如果游标在窗口内则绘制相交坐标线 */
		POINT curpos = CurrentCursorPosition;
		ScreenToClient(hWnd, &curpos);

		/* 如果游标位置改变，与上次不一样时 */
		if (curpos.x != lastPointsColorInfo.pos.x || curpos.y != lastPointsColorInfo.pos.y)
		{
			/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			* 二维数组转一维数组下标
			* 已知 宽（W） 高（H），坐标（X, Y）
			* 一维下标公式为
			*		index = Y * W + X
			-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
			/*for (LONG x = 0; x < lastPointsColorInfo.size.cx; ++x)
			{
				SetPixel(hdc, x, lastPointsColorInfo.pos.y, lastPointsColorInfo.colors[lastPointsColorInfo.pos.y * lastPointsColorInfo.size.cx + x]);
			}
			for (LONG y = 0; y < lastPointsColorInfo.size.cy; ++y)
			{
				SetPixel(hdc, lastPointsColorInfo.pos.x, y, lastPointsColorInfo.colors[y * lastPointsColorInfo.size.cx + lastPointsColorInfo.pos.x]);
			}*/

			/* 重绘背景图片 */
			if (BackgroundImage_ShowMode)
			{
				HDC bitmapDC = CreateCompatibleDC(hdc);
				HBITMAP ConsoleBackground = LoadImageA(GetModuleHandleA(NULL), BackgroundImage_CodeSets[BackgroundImage_CurrentNumber], IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
				SelectObject(bitmapDC, ConsoleBackground);
				SetStretchBltMode(hdc, HALFTONE);

				if ((rect.right - rect.left - WDOWINDOWSIZE_JAMB) <= WDOWINDOW_BK_WIDTH && rect.bottom - rect.top <= WDOWINDOW_BK_HEIGHT)
				{
					BitBlt(hdc, 0, 0, rect.right - rect.left - WDOWINDOWSIZE_JAMB + 16, rect.bottom - rect.top, bitmapDC, 0, 0, SRCCOPY);
				}
				else
				{
					StretchBlt(hdc
						, rect.left
						, rect.top
						, rect.right - rect.left - WDOWINDOWSIZE_JAMB + 16
						, rect.bottom - rect.top
						, bitmapDC
						, 0
						, 0
						, WDOWINDOW_BK_WIDTH
						, WDOWINDOW_BK_HEIGHT
						, SRCCOPY);
				}
				DeleteObject(ConsoleBackground);
				DeleteDC(bitmapDC);

			}
			else
			{
				HBRUSH ConsoleBackBrush = CreateSolidBrush(BackgroundImage_Color);
				SelectObject(hdc, ConsoleBackBrush);
				PatBlt(hdc, rect.left, rect.top, rect.right - rect.left - WDOWINDOWSIZE_JAMB + 16, rect.bottom - rect.top, PATCOPY);
				DeleteObject(ConsoleBackBrush);
			}
			

			/* 更新坐标 */
			lastPointsColorInfo.pos = curpos;

			/* 绘制当前相交线条 */
			HPEN hPen = CreatePen(PS_DASHDOT, 1, (BackgroundImage_ShowMode == TRUE || (BackgroundImage_Color == RGB(0x00, 0x00, 0x00))) ? WDOWINDOWPOSLINECOLOR1 : WDOWINDOWPOSLINECOLOR2);
			SelectObject(hdc, hPen);

			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, (BackgroundImage_ShowMode == TRUE || (BackgroundImage_Color == RGB(0x00, 0x00, 0x00))) ? WDOWINDOWPOSLINECOLOR1 : WDOWINDOWPOSLINECOLOR2);
			
			char carrier_x[7] = "\0";
			char carrier_y[7] = "\0";

			
			MoveToEx(hdc, 0, lastPointsColorInfo.pos.y, NULL);
			LineTo(hdc, lastPointsColorInfo.size.cx, lastPointsColorInfo.pos.y);

			if (lastPointsColorInfo.pos.x < lastPointsColorInfo.size.cx)
			{
				MoveToEx(hdc, lastPointsColorInfo.pos.x, 0, NULL);
				LineTo(hdc, lastPointsColorInfo.pos.x, lastPointsColorInfo.size.cy);
			}

			const enum { RECTANGLE_RANGE = 3 };

			MoveToEx(hdc
				, min(lastPointsColorInfo.pos.x - (lastPointsColorInfo.pos.y / RECTANGLE_RANGE), lastPointsColorInfo.size.cx)
				, lastPointsColorInfo.pos.y - (lastPointsColorInfo.pos.y / RECTANGLE_RANGE)
				, NULL);
			LineTo(hdc
				, min(lastPointsColorInfo.pos.x + (lastPointsColorInfo.pos.y / RECTANGLE_RANGE), lastPointsColorInfo.size.cx)
				, lastPointsColorInfo.pos.y - (lastPointsColorInfo.pos.y / RECTANGLE_RANGE)
			);
			LineTo(hdc
				, min(lastPointsColorInfo.pos.x + (lastPointsColorInfo.pos.y / RECTANGLE_RANGE), lastPointsColorInfo.size.cx)
				, lastPointsColorInfo.pos.y + (lastPointsColorInfo.pos.y / RECTANGLE_RANGE)
			);
			LineTo(hdc
				, min(lastPointsColorInfo.pos.x - (lastPointsColorInfo.pos.y / RECTANGLE_RANGE), lastPointsColorInfo.size.cx)
				, lastPointsColorInfo.pos.y + (lastPointsColorInfo.pos.y / RECTANGLE_RANGE)
			);
			LineTo(hdc
				, min(lastPointsColorInfo.pos.x - (lastPointsColorInfo.pos.y / RECTANGLE_RANGE), lastPointsColorInfo.size.cx)
				, lastPointsColorInfo.pos.y - (lastPointsColorInfo.pos.y / RECTANGLE_RANGE)
			);

			sprintf(carrier_x, "%d"
				, lastPointsColorInfo.pos.y);
			sprintf(carrier_y, "%d"
				, lastPointsColorInfo.pos.x);

			TextOutA(hdc
				, lastPointsColorInfo.size.cx - 40
				, lastPointsColorInfo.pos.y - 20
				, carrier_x, (int)strlen(carrier_x));

			/* 不可越过控制台区 */
			if (lastPointsColorInfo.pos.x < CurrentWindowRect.right - CurrentWindowRect.left - WDOWINDOWSIZE_JAMB - 35)
			{
				TextOutA(hdc
					, lastPointsColorInfo.pos.x + 5
					, lastPointsColorInfo.size.cy - 40
					, carrier_y, (int)strlen(carrier_y));
			}
			
			DeleteObject(hPen);

			if (BackgroundImage_Color == RGB(0x00, 0x00, 0x00))
			{
				SetTextColor(hdc, RGB(0xff, 0xff, 0xff));
			}
			else
			{
				SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
			}
			SelectObject(hdc, font_head);
			char str[30] = "";
			TextOutA(hdc, WDOWINDOWSIZE_OUTPUT_POSX, WDOWINDOWSIZE_INPUT_POSY + 2, str, sprintf(str, "#%2llu | _%X_ > ", LineNumber, CurrentPtrhwnd ? *CurrentPtrhwnd : 0));
		}


		return 0;
	}
	case WM_CTLCOLOREDIT:	// 更改编辑框（Edit）颜色
	{
		/*
		* wParam 为 Edit 控件的 DC 句柄
		* lParam 为 Edit 的控件句柄
		*
		* 返回背景画笔
		*/
		if (lParam == (LPARAM)wdoWindowCommandHwnd)
		{
			SetTextColor((HDC)wParam, InputColor);
		}
		else if (lParam == (LPARAM)wdoWindowOutlogHwnd)
		{
			SetTextColor((HDC)wParam, OutputColor);
		}
		SetBkColor((HDC)wParam, BackgroundImage_Color);
		SetBkMode((HDC)wParam, OPAQUE);

		return (LRESULT)EditTextboxBrush;
	}
	case WM_MOUSEWHEEL:		// 上下滚轮
	{
		if ((short)HIWORD(wParam) > 0)
		{
			return SendMessageA(wdoWindowOutlogHwnd, WM_VSCROLL, SB_LINEUP, 0);
		}
		else if ((short)HIWORD(wParam) < 0)
		{
			return SendMessageA(wdoWindowOutlogHwnd, WM_VSCROLL, SB_LINEDOWN, 0);
		}
		return 0;
	}
	case WM_MOUSEHWHEEL:	// 左右滚轮
	{
		if ((short)HIWORD(wParam) > 0)
		{
			return SendMessageA(wdoWindowOutlogHwnd, WM_HSCROLL, SB_LINERIGHT, 0);
		}
		else if ((short)HIWORD(wParam) < 0)
		{
			return SendMessageA(wdoWindowOutlogHwnd, WM_HSCROLL, SB_LINELEFT, 0);
		}
		return 0;
	}
	case WM_CHAR:			// 字符
	{
		SetFocus(wdoWindowCommandHwnd);
		return SendMessageA(wdoWindowCommandHwnd, uMsg, wParam, lParam);
	}
	case WM_LBUTTONDOWN:	// 左键
	{
		SetFocus(wdoWindowCommandHwnd);
		return SendMessageA(wdoWindowCommandHwnd, uMsg, wParam, lParam);
	}
	case WM_RBUTTONDOWN:	// 右键
	{
		if (CurrentCursorPosition.y > CurrentWindowRect.top + WDOWINDOWSIZE_INPUT_HEIGHT + WDOWINDOWSIZE_INPUT_POSY + WDOWINDOWSIZE_IO_SPACE
			&& CurrentCursorPosition.x < CurrentWindowRect.right - WDOWINDOWSIZE_JAMB)
		{
			SetFocus(wdoWindowOutlogHwnd);
			SendMessageA(wdoWindowOutlogHwnd, EM_SETSEL, 0, -1);
		}
		return 0;
	}
	case WM_LBUTTONDBLCLK:	// 双击（左）
	{
		SendMessageA(wdoWindowOutlogHwnd, WM_LBUTTONDOWN, wParam, lParam);
		return SendMessageA(wdoWindowOutlogHwnd, WM_LBUTTONDBLCLK, wParam, lParam);
	}
	case WM_DROPFILES:		// 文件拖拽
	{
		HDROP hDrop = (HDROP)wParam;

		char filename[MAX_PATH];

		/* 打开拖拽的第一个(下标为0)文件 */
		DragQueryFileA(hDrop, 0, filename, sizeof(filename));

		lrSendStringA(wdoWindowCommandHwnd, filename);

		/* 完成拖入文件操作，系统释放缓冲区 */
		DragFinish(hDrop);

		SetFocus(wdoWindowCommandHwnd);
		return 0;
	}
	case WM_VSCROLL:		// 垂直滚动条
	{
		return SendMessageA(wdoWindowOutlogHwnd, WM_VSCROLL, wParam, lParam);
	}
	case WM_SIZE:			// 大小更改
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		/* 更改编辑框大小 */

		MoveWindow(wdoWindowCommandHwnd
			, WDOWINDOWSIZE_INPUT_POSX
			, WDOWINDOWSIZE_INPUT_POSY
			, x > (WDOWINDOWSIZE_JAMB + WDOWINDOWSIZE_INPUT_POSX + WDOWINDOWSIZE_INPUT_ENDX) ? x - WDOWINDOWSIZE_JAMB - WDOWINDOWSIZE_INPUT_POSX - WDOWINDOWSIZE_INPUT_ENDX + 16 : 0
			, WDOWINDOWSIZE_INPUT_HEIGHT
			, TRUE);
		InvalidateRect(wdoWindowCommandHwnd, NULL, FALSE);

		MoveWindow(wdoWindowOutlogHwnd
			, WDOWINDOWSIZE_OUTPUT_POSX
			, WDOWINDOWSIZE_INPUT_HEIGHT + WDOWINDOWSIZE_INPUT_POSY + WDOWINDOWSIZE_IO_SPACE
			, x > WDOWINDOWSIZE_JAMB ? x - WDOWINDOWSIZE_JAMB + 16 : 0
			, y - WDOWINDOWSIZE_INPUT_HEIGHT - WDOWINDOWSIZE_IO_SPACE
			, TRUE);
		InvalidateRect(wdoWindowOutlogHwnd, NULL, FALSE);

		/* 更改游标坐标相交线长宽 */
		lastPointsColorInfo.size.cx = x > WDOWINDOWSIZE_JAMB ? x - WDOWINDOWSIZE_JAMB + 16 : 0;
		lastPointsColorInfo.size.cy = y;
		
		/* 更改按钮位置 */
		for (size_t i = 0; i < sizeof(buttons) / sizeof(buttons[0]); ++i)
		{
			MoveWindow(buttons[i]
				, (CurrentWindowRect.right - CurrentWindowRect.left - WDOWINDOWSIZE_JAMB) + WDOWINDOW_BUTTON_POSX
				, WDOWINDOW_BUTTON_POSY + WDOWINDOW_BUTTON_JAMB * (int)i
				, WDOWINDOW_BUTTON_WIDTH
				, WDOWINDOW_BUTTON_HEIGHT
				, TRUE);
		}

		for (size_t i = 0; i < sizeof(buttons) / sizeof(buttons[0]); ++i)
		{
			InvalidateRect(buttons[i], NULL, FALSE);
		}
		return 0;
	}
	case WM_HOTKEY:			// 按下快捷键
	{
		if (HotkeyCount && wParam < HotkeyCount)
		{
			for (UINT i = 0; i < HotkeyInstructions[wParam].Count; ++i)
			{
				lrWindowDo(HotkeyInstructions[wParam].Instructions[i], WDOECHO_DEF);
			}
		}
		return 0;
	}
	case WM_WDO_CREATEEXIT:
	{
		WNDCLASS wndclass = { 0 };

		wndclass.lpszClassName = WDO_WINDOW_CLASS_WDOEXIT;
		wndclass.hbrBackground = NULL;
		wndclass.hCursor = IA_ALIAS_HCURSOR;
		wndclass.hIcon = IA_ALIAS_HICON;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = 0;
		wndclass.lpfnWndProc = wdoControlExitThreadWindowProc;
		wndclass.lpszMenuName = NULL;
		wndclass.style = CS_NOCLOSE;

		RegisterClassA(&wndclass);

		CWE cwe = { 0 };
		cwe.dwExStyle = WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_NOREDIRECTIONBITMAP;
		strcpy(cwe.lpClassName, WDO_WINDOW_CLASS_WDOEXIT);
		strcpy(cwe.lpWindowName, WDO_WINDOW_CLASS_WDOEXIT);
		cwe.dwStyle = WS_MINIMIZE;
		cwe.X = 0;
		cwe.Y = 0;
		cwe.nWidth = 0;
		cwe.nHeight = 0;
		cwe.hWndParent = 0;
		cwe.hMenu = 0;
		cwe.hInstance = 0;
		cwe.lpParam = 0;
		cwe.CencelShow = TRUE;

		return (LRESULT)(wdoWindowExitControlHwnd = lrCreateWindow(&cwe));
	}
	case WM_WDO_ADDNEWWINDOW:
	{
		if (IsWindow((HWND)wParam) == FALSE) return FALSE;

		WaitForSingleObject(mutex, 5000);

		if (wdoWindowCount)
		{
			++wdoWindowCount;
			HWND* temp = (HWND*)realloc(wdoWindowSet, wdoWindowCount * sizeof(HWND));

			if (temp)
			{
				wdoWindowSet = temp;
			}
			else {
				wdo_error_ex();
				ReleaseMutex(mutex);
				--wdoWindowCount;
				return FALSE;
			}
		}
		else
		{
			free(wdoWindowSet);
			++wdoWindowCount;
			wdoWindowSet = (HWND*)malloc(wdoWindowCount * sizeof(HWND));

			if (wdoWindowSet == NULL) {
				wdo_error_ex();
				ReleaseMutex(mutex);
				--wdoWindowCount;
				return FALSE;
			}
		}

		wdoWindowSet[wdoWindowCount - 1] = (HWND)wParam;

		ReleaseMutex(mutex);
		return TRUE;
	}
	case WM_WDO_DESTROYWINDOW:
	{
		WaitForSingleObject(mutex, 5000);

		if (wdoWindowSet && wdoWindowCount)
		{
			ULONG wdoWindowNumber = 0;
			BOOL wdoWindowHave = FALSE;

			for (ULONG i = 0; i < wdoWindowCount; ++i)
			{
				if (wdoWindowSet[i] == (HWND)wParam)
				{
					wdoWindowNumber = i;
					wdoWindowHave = TRUE;
				}
				else continue;
			}
			if (wdoWindowHave == FALSE) {
				ReleaseMutex(mutex);
				return FALSE;
			}

			if (--wdoWindowCount)
				wdoWindowSet[wdoWindowNumber] = wdoWindowSet[wdoWindowCount];

			if (ExitAskSwitch)
				if (wdoWindowCount == 0 && wdoThreadCount == 0)
					wdo_exit(wdoExitCode);

			ReleaseMutex(mutex);
			return TRUE;
		}
		else {
			ReleaseMutex(mutex);
			return FALSE;
		}
	}
	case WM_WDO_ADDNEWTHREAD:
	{
		WaitForSingleObject(mutex, 5000);

		if (wdoThreadCount)
		{
			++wdoThreadCount;
			DWORD* handle = (DWORD*)realloc(wdoThreadSet, wdoThreadCount * sizeof(DWORD));

			if (handle)
			{
				wdoThreadSet = handle;
			}
			else
			{
				wdo_error_ex();
				ReleaseMutex(mutex);
				--wdoThreadCount;
				return FALSE;
			}
		}
		else
		{
			++wdoThreadCount;

			wdoThreadSet = (DWORD*)malloc(wdoThreadCount * sizeof(DWORD));

			if (wdoThreadSet == NULL)
			{
				wdo_error_ex();
				ReleaseMutex(mutex);
				--wdoThreadCount;
				return FALSE;
			}
		}
		wdoThreadSet[wdoThreadCount - 1] = (DWORD)wParam;

		ReleaseMutex(mutex);
		return TRUE;
	}
	case WM_WDO_DESTROYTHREAD:
	{
		WaitForSingleObject(mutex, 5000);

		if (wdoWindowSet && wdoThreadCount)
		{
			ULONG wdoThreadNumber = 0;
			BOOL wdoThreadHave = FALSE;

			for (ULONG i = 0; i < wdoThreadCount; ++i)
			{
				if (wdoThreadSet[i] == (DWORD)wParam)
				{
					wdoThreadNumber = i;
					wdoThreadHave = TRUE;
				}
				else continue;
			}
			if (wdoThreadHave == FALSE) {
				ReleaseMutex(mutex);
				return FALSE;
			}

			if(--wdoThreadCount)
				wdoThreadSet[wdoThreadNumber] = wdoThreadSet[wdoThreadCount];

			if (ExitAskSwitch)
				if (wdoWindowCount == 0 && wdoThreadCount == 0)
					wdo_exit(wdoExitCode);

			ReleaseMutex(mutex);
			return TRUE;
		}
		else {
			ReleaseMutex(mutex);
			return FALSE;
		}
	}
	case WM_WDO_EXITASK:
	{
		wdoExitCode = (int)wParam;

		if (wdoWindowCount == 0 && wdoThreadCount == 0) wdo_exit(wdoExitCode);

		switch (lParam)
		{
		case WDOEXITASK_RETURN:
			if (wdoWindowCount || wdoThreadCount) return FALSE;
			else return TRUE;

		case WDOEXITASK_ENDEDEXIT:
			ExitAskSwitch = TRUE;

			if (wdoWindowCount || wdoThreadCount) return FALSE;
			else return TRUE;

		case WDOEXITASK_WAITEXIT:
			ExitAskSwitch = TRUE;

			if (wdoWindowCount || wdoThreadCount) return FALSE;
			else return TRUE;

		default: return FALSE;
		}
	}
	case WM_WDO_SETCOLOR:
	{
		if (OutputColor != (COLORREF)wParam)
		{
			OutputColor = (COLORREF)wParam;
			InvalidateRect(wdoWindowOutlogHwnd, NULL, FALSE);
			UpdateWindow(wdoWindowOutlogHwnd);
		}
		return 0;
	}
	case WM_WDO_SETHOTKEY:
	{
		/* 删除快捷键 */
		if (lParam == 0 || ((WDOINSTRUCTIONS*)lParam)->Count == 0 || ((WDOINSTRUCTIONS*)lParam)->Instructions == NULL)
		{
			if (wParam)
			{
				for (UINT i = 0; i < HotkeyCount; ++i)
				{
					if (!stricmp((const char*)wParam, HotkeyString[i]))
					{
						--HotkeyCount;

						if (HotkeyCount == 0)
						{
							lrfrees(&(HotkeyInstructions->Instructions), HotkeyInstructions->Count);

							free(HotkeyString[0]);
							free(HotkeyString);
						}
						else
						{
							free(HotkeyString[i]);
							HotkeyString[i] = HotkeyString[HotkeyCount];

							lrfrees(&(HotkeyInstructions[i].Instructions), HotkeyInstructions[i].Count);
							HotkeyInstructions[i] = HotkeyInstructions[HotkeyCount];
						}

						UnregisterHotKey(hWnd, i);
						return GetLastError();
					}
				}
				return ERROR_HOTKEY_NOT_REGISTERED;
			}
			else return ERROR_INVALID_PARAMETER;
		}
		/* 添加快捷键 */
		else
		{
			UINT Modifiers = 0;
			UINT VirtKey = 0;

			if (wdo_string_to_hotkey((char*)wParam, &Modifiers, &VirtKey))
			{
				if (HotkeyCount == 0)
				{
					if (HotkeyInstructions = (WDOINSTRUCTIONS*)malloc(sizeof(WDOINSTRUCTIONS)))
					{
						memcpy(HotkeyInstructions, (WDOINSTRUCTIONS*)lParam, sizeof(WDOINSTRUCTIONS));
					}
					else return ERROR_OUTOFMEMORY;

					if (HotkeyString = (char**)malloc(sizeof(char*)))
					{
						if (HotkeyString[0] = (char*)malloc(strlen((char*)wParam) + 1))
						{
							strcpy(HotkeyString[0], (char*)wParam);
						}
						else
						{
							free(HotkeyInstructions);
							free(HotkeyString);
							return ERROR_OUTOFMEMORY;
						}
					}
					else
					{
						free(HotkeyInstructions);
						return ERROR_OUTOFMEMORY;
					}

					++HotkeyCount;
				}
				else
				{
					++HotkeyCount;

					if (HotkeyInstructions = (WDOINSTRUCTIONS*)realloc(HotkeyInstructions, HotkeyCount * sizeof(WDOINSTRUCTIONS)))
					{
						memcpy(&(HotkeyInstructions[HotkeyCount - 1]), (WDOINSTRUCTIONS*)lParam, sizeof(WDOINSTRUCTIONS));
					}
					else
					{
						--HotkeyCount;
						return ERROR_OUTOFMEMORY;
					}

					if (HotkeyString = (char**)realloc(HotkeyString, HotkeyCount * sizeof(char*)))
					{
						if (HotkeyString[HotkeyCount - 1] = (char*)malloc(strlen((char*)wParam) + 1))
						{
							strcpy(HotkeyString[HotkeyCount - 1], (char*)wParam);
						}
						else
						{
							--HotkeyCount;
							return ERROR_OUTOFMEMORY;
						}
					}
					else
					{
						--HotkeyCount;
						return ERROR_OUTOFMEMORY;
					}
				}

				if (RegisterHotKey(hWnd, (int)HotkeyCount - 1, Modifiers, VirtKey) == FALSE)
				{
					--HotkeyCount;

					if (HotkeyCount == 0)
					{
						free(HotkeyString[0]);
						free(HotkeyString);
					}
					else free(HotkeyString[HotkeyCount]);

					return GetLastError();
				}
				else return ERROR_SUCCESS;
			}
			else return ERROR_INVALID_PARAMETER;
		}
	}
	case WM_CLOSE:
	{
		DestroyingNoExit = (BOOL)wParam;

		DestroyWindow(hWnd);
		return 0;
	}
	case WM_DESTROY:
	{
		KillTimer(hWnd, (UINT_PTR)hWnd);

		ReleaseDC(hWnd, hdc);
		CloseHandle(mutex);

		DeleteObject(font_command);
		DeleteObject(font_outlog);
		DeleteObject(font_button);
		DeleteObject(font_head);
		DeleteObject(font_text);

		SendMessageA(wdoWindowExitControlHwnd, WM_CLOSE, 0, 0);

		/* 关闭按钮控件 */
		for (size_t i = 0; i < sizeof(buttons) / sizeof(*buttons); ++i)
		{
			SendMessageA(buttons[i], WM_CLOSE, 0, 0);
		}
		SendMessageA(wdoWindowOutlogHwnd, WM_CLOSE, 0, 0);
		SendMessageA(wdoWindowCommandHwnd, WM_CLOSE, 0, 0);

		/* 销毁附属窗口及线程 */
		if (wdoWindowCount)
			for (ULONG i = 0; i < wdoWindowCount; ++i)
			{
				SendMessageA(wdoWindowSet[i], WM_CLOSE, 0, 0);
			}
		if (wdoWindowSet && wdoWindowCount) free(wdoWindowSet);

		if (wdoThreadSet && wdoWindowCount) free(wdoThreadSet);

		for (UINT i = 0; i < HotkeyCount; ++i)
		{
			UnregisterHotKey(hWnd, i);
			lrfrees(&(HotkeyInstructions[i].Instructions), HotkeyInstructions[i].Count);
		}
		if (HotkeyCount) lrfrees(&HotkeyString, HotkeyCount);


		PostQuitMessage(wdoExitCode);

		if (!DestroyingNoExit) exit(wdoExitCode);
		return 0;
	}
	}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}