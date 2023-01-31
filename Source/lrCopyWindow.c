// lrCopyWindow.c -- Copy a window
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lrWindowDo.h"

#pragma warning (disable : 4312)

/* 目标窗口 */
static HWND* TargetWin= NULL;
/* 归属窗口 */
static HWND* HomeWin = NULL;
/* 窗口数量 */
static ULONG WinCount = 0;
/* 临界互斥 */
static HANDLE mutex = 0;

static LRESULT CALLBACK lrCopyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND lrCopyWindow(HWND _hwnd, const CHAR _newclass[MAXLEN_WIN_CLASS])
{
	if (IsWindow(_hwnd) == FALSE) return 0;


	WNDCLASS wndclass = { 0 };
	CWE cwe = { 0 };
	RECT rect;
	GetWindowRect(_hwnd, &rect);

	wndclass.cbClsExtra = 0;													// 窗口类扩展
	wndclass.cbWndExtra = 0;													// 窗口实例扩展

	wndclass.hbrBackground = (HBRUSH)GetClassLongA(_hwnd, GCLP_HBRBACKGROUND);	// 窗口背景颜色
	wndclass.hCursor = (HCURSOR)GetClassLongA(_hwnd, GCLP_HCURSOR);				// 窗口游标
	wndclass.hIcon = (HICON)GetClassLongA(_hwnd, GCLP_HICON);					// 窗口最小化图标

	wndclass.hInstance = 0;														// 窗口实例句柄

	wndclass.lpfnWndProc = lrCopyWindowProc;									// 窗口处理函数

	wndclass.lpszClassName = _newclass;											// 窗口类名
	wndclass.lpszMenuName = NULL;												// 窗口菜单
	wndclass.style = GetClassLongA(_hwnd, GCL_STYLE);							// 窗口样式

	/* 注册此类名 */
	RegisterClassA(&wndclass);
	
	
	cwe.dwExStyle = GetWindowLongA(_hwnd, GWL_EXSTYLE);

	BOOL IsLayered = FALSE;
	COLORREF color;
	BYTE byte;
	DWORD flags;

	/* 检查风格是否具有分层窗口 */
	DWORD TempExStyle = (cwe.dwExStyle & 0x000F0000) >> 16;
	if (   TempExStyle == 0x8
		|| TempExStyle == 0x9
		|| TempExStyle == 0xA
		|| TempExStyle == 0xC
		|| TempExStyle == 0xE) {

		IsLayered = TRUE;
		GetLayeredWindowAttributes(_hwnd, &color, &byte, &flags);
	}

	GetWindowTextA(_hwnd, cwe.lpWindowName, MAXLEN_WIN_TITLE);
	strcpy(cwe.lpClassName, _newclass);
	cwe.dwStyle = GetWindowLongA(_hwnd, GWL_STYLE);
	cwe.X = rect.left;
	cwe.Y = rect.top;
	cwe.nWidth = rect.right - rect.left;
	cwe.nHeight = rect.bottom - rect.top;
	cwe.hWndParent = 0;
	cwe.hMenu = 0;
	cwe.hInstance = 0;
	cwe.lpParam = 0;

	/* 设置并进入临界区 */
	if (!mutex) mutex = CreateMutexA(NULL, FALSE, NULL);
	if(mutex) WaitForSingleObject(mutex, INFINITE);

	/* 对内存进行分配 */
	if (WinCount == 0)
	{
		++WinCount;

		TargetWin = (HWND*)malloc(sizeof(HWND));
		if (TargetWin == NULL) return 0;
		else TargetWin[0] = _hwnd;

		HomeWin = (HWND*)malloc(sizeof(HWND));
		if (HomeWin == NULL) return 0;
		else HomeWin[0] = 0;
	}
	else
	{
		++WinCount;

#pragma warning (suppress : 6308)
		TargetWin = (HWND*)realloc(TargetWin, WinCount * sizeof(HWND));
		if (TargetWin == NULL) return 0;
		else TargetWin[WinCount - 1] = _hwnd;

#pragma warning (suppress : 6308)
		HomeWin = (HWND*)realloc(HomeWin, WinCount * sizeof(HWND));
		if (HomeWin == NULL) return 0;
		else HomeWin[WinCount - 1] = 0;
	}

	/* 退出临界区 */
	if (mutex) ReleaseMutex(mutex);

	/* 如果是分层窗口则窗口创建完成后设置其透明度 */
	if (IsLayered)
	{
		HWND hwnd = lrCreateWindow(&cwe);
		SetLayeredWindowAttributes(hwnd, color, byte, flags);
		return hwnd;
	}

	else return lrCreateWindow(&cwe);
}



static LRESULT CALLBACK lrCopyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* 以下数组是用测试代码测试得出，需要同步消息的消息值
	* 如果窗口收到以下任一消息，则会将消息同步至目标窗口
	* 在 switch (uMsg) default: 中使用 */
	const UINT CopySend[] =
	{
		// 0x0024, 0x0081, 0x0083, 0x0018, 0x0046, 0x001C, 0x0086, 0x0006, 0x0281, 0x0282,
		// 0x0047, 0x0005, 0x0003, 0x000D, 0xC0BC, 0x0085, 0x0014, 0xC0D8, 0x031F, 0x0020,
		// 0x00A0, 0x0101, 0x0164, 0x000E,

		0x0200, 0x02A2, 0x00A1, 0x0112, 0x0231, 0xC054, 0x0232, 0x0013, 0x0088, 0x0233,
		0x0201, 0x0202, 0x0204, 0x0205, 0x007B, 0x020A, 0x0207, 0x0208, 0x0021, 0x0100,
		0x0102, 0x0104, 0x0105, 0x0211, 0x0116, 0x011F, 0x0120, 0x0212, 0x004D, 0x0053,
		0x00A4, 0x0117, 0x0093, 0x0094, 0x0125, 0x0115, 0x0214, 0x020E, 0x0203, 0x0206,
		0x00A2, 0x0121
	};


	switch (uMsg)
	{
	case WM_CREATE:
		{
			WaitForSingleObject(mutex, INFINITE);

			HomeWin[WinCount - 1] = hWnd;

			ReleaseMutex(mutex);

			/* 设置定时器 */
			SetTimer(hWnd, (UINT_PTR)hWnd, FREQUENCY_SCREEN_REFRESH, NULL);

			return 0;
		}
	case WM_PAINT:
	case WM_TIMER:
		{
			ULONG WinNumber = 0;

			WaitForSingleObject(mutex, INFINITE);

			/* 搜索当前窗口 */
			for (ULONG i = 0; i < WinCount; ++i) {
				if (HomeWin[i] == hWnd) {
					WinNumber = i;
					break;
				}
			}
			
			if (IsWindow(TargetWin[WinNumber]))
			{
				/* 获取自身窗口DC */
				HDC hdc_owner = GetDC(hWnd);
				/* 获取目标窗口DC */
				HDC hdc_other = GetDC(TargetWin[WinNumber]);

				/* 自身窗口客户区 */
				RECT rect_owner;
				GetWindowRect(hWnd, &rect_owner);

				/* 目标窗口客户区 */
				RECT rect_other;
				GetWindowRect(TargetWin[WinNumber], &rect_other);

				/* 使无效区变有效区，避免重复接收 WM_PAINT */
				if (uMsg == WM_PAINT) ValidateRect(hWnd, &rect_owner);

				/* 改变图像复制模式，如果不设置，可能会失真严重 */
				SetStretchBltMode(hdc_owner, HALFTONE);
				SetStretchBltMode(hdc_other, HALFTONE);

				/* 复制图像 */
				StretchBlt(
					hdc_owner, 0, 0, 
					rect_owner.right - rect_owner.left,
					rect_owner.bottom - rect_owner.top,

					hdc_other, 0, 0,
					(int)((rect_other.right - rect_other.left) * lrGetWindowDpiZoom(TargetWin[WinNumber]).fw),
					(int)((rect_other.bottom - rect_other.top) * lrGetWindowDpiZoom(TargetWin[WinNumber]).fh),
					SRCCOPY
				);

				/* 释放两个DC */
				ReleaseDC(hWnd, hdc_owner);
				ReleaseDC(TargetWin[WinNumber], hdc_other);


				if (GetForegroundWindow() != hWnd)
				{
					/* 校准窗口标题 */
					char WinTitle[MAXLEN_WIN_TITLE];
					memset(WinTitle, '\0', MAXLEN_WIN_TITLE);
					GetWindowTextA(TargetWin[WinNumber], WinTitle, MAXLEN_WIN_TITLE);
					SetWindowTextA(HomeWin[WinNumber], WinTitle);

					/* 校准窗口大小 */
					RECT winWH;
					GetWindowRect(TargetWin[WinNumber], &winWH);
					SetWindowPos(hWnd, 0, 0, 0, winWH.right - winWH.left, winWH.bottom - winWH.top, SWP_NOMOVE | SWP_NOZORDER);

					/* 校准窗口风格 */
					LONG style = GetWindowLongA(TargetWin[WinNumber], GWL_STYLE);
					SetWindowLongA(hWnd, GWL_STYLE, style);
					style = GetWindowLongA(TargetWin[WinNumber], GWL_EXSTYLE);
					SetWindowLongA(hWnd, GWL_EXSTYLE, style);

					/* 检查风格是否具有分层窗口 */
					DWORD TempExStyle = (style & 0x000F0000) >> 16;
					if (TempExStyle == 0x8
						|| TempExStyle == 0x9
						|| TempExStyle == 0xA
						|| TempExStyle == 0xC
						|| TempExStyle == 0xE) {

						COLORREF color;
						BYTE byte;
						DWORD flags;

						GetLayeredWindowAttributes(TargetWin[WinNumber], &color, &byte, &flags);
						SetLayeredWindowAttributes(hWnd, color, byte, flags);
					}
				}
			}
			else
			{
				ReleaseMutex(mutex);
				SendMessageA(hWnd, WM_CLOSE, 0, 0);
				return 0;
			}

			ReleaseMutex(mutex);

			return 0;
		}
	case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			return 0;
		}
	case WM_DESTROY:
		{
			/* 删除计时器 */
			KillTimer(hWnd, (UINT_PTR)hWnd);

			WaitForSingleObject(mutex, INFINITE);
			/* 减少不需要的空间 */
			if (WinCount == 1)
			{
				free(TargetWin);
				TargetWin = NULL;

				free(HomeWin);
				HomeWin = NULL;

				WinCount = 0;

				CloseHandle(mutex);
				mutex = NULL;
			}
			else
			{
				ULONG WinNumber = 0;

				for (ULONG i = 0; i < WinCount; ++i) {
					if (HomeWin[i] == hWnd) {
						WinNumber = i;
						break;
					}
				}

				--WinCount;
				TargetWin[WinNumber] = TargetWin[WinCount];
				HomeWin[WinNumber] = HomeWin[WinCount];

				ReleaseMutex(mutex);
			}

			PostQuitMessage(0);
			return 0;
		}
	default:
		{
			/* 检查消息是否需要同步 */
			for (size_t i = 0; i < sizeof(CopySend) / sizeof(UINT); ++i)
			{
				if (CopySend[i] == uMsg)
				{
					ULONG WinNumber = 0;

					WaitForSingleObject(mutex, INFINITE);

					/* 搜索当前窗口 */
					for (ULONG i = 0; i < WinCount; ++i) {
						if (HomeWin[i] == hWnd) {
							WinNumber = i;
							break;
						}
					}
					HWND TempHwnd = TargetWin[WinNumber];
					ReleaseMutex(mutex);

					LRESULT lpesult = DefWindowProcA(hWnd, uMsg, wParam, lParam);

					SendMessageA(TempHwnd, uMsg, wParam, lParam);

					return lpesult;
				}
			}

			// 测试代码
			/*static UINT uint[250] = { 0 };
			static UINT size = 0;

			if (uMsg != 0xAE
				&& uMsg != 0x7F
				&& uMsg != 0x0C
				&& uMsg != 0x3D) {

				BOOL tf = 0;

				for (UINT i = 0; i < size; ++i) {
					if (uint[i] == uMsg) {
						tf = 1;
						break;
					}
				}
				if (tf == 0) {
					printf("%X, ", uMsg);
					uint[size++] = uMsg;
				}
			}*/

			return DefWindowProcA(hWnd, uMsg, wParam, lParam);
		}
	}
}
