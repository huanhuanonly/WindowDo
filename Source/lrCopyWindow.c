// lrCopyWindow.c -- Copy a window
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lrWindowDo.h"

#pragma warning (disable : 4312)

/* Ŀ�괰�� */
static HWND* TargetWin= NULL;
/* �������� */
static HWND* HomeWin = NULL;
/* �������� */
static ULONG WinCount = 0;
/* �ٽ绥�� */
static HANDLE mutex = 0;

static LRESULT CALLBACK lrCopyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND lrCopyWindow(HWND _hwnd, const CHAR _newclass[MAXLEN_WIN_CLASS])
{
	if (IsWindow(_hwnd) == FALSE) return 0;


	WNDCLASS wndclass = { 0 };
	CWE cwe = { 0 };
	RECT rect;
	GetWindowRect(_hwnd, &rect);

	wndclass.cbClsExtra = 0;													// ��������չ
	wndclass.cbWndExtra = 0;													// ����ʵ����չ

	wndclass.hbrBackground = (HBRUSH)GetClassLongA(_hwnd, GCLP_HBRBACKGROUND);	// ���ڱ�����ɫ
	wndclass.hCursor = (HCURSOR)GetClassLongA(_hwnd, GCLP_HCURSOR);				// �����α�
	wndclass.hIcon = (HICON)GetClassLongA(_hwnd, GCLP_HICON);					// ������С��ͼ��

	wndclass.hInstance = 0;														// ����ʵ�����

	wndclass.lpfnWndProc = lrCopyWindowProc;									// ���ڴ�����

	wndclass.lpszClassName = _newclass;											// ��������
	wndclass.lpszMenuName = NULL;												// ���ڲ˵�
	wndclass.style = GetClassLongA(_hwnd, GCL_STYLE);							// ������ʽ

	/* ע������� */
	RegisterClassA(&wndclass);
	
	
	cwe.dwExStyle = GetWindowLongA(_hwnd, GWL_EXSTYLE);

	BOOL IsLayered = FALSE;
	COLORREF color;
	BYTE byte;
	DWORD flags;

	/* ������Ƿ���зֲ㴰�� */
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

	/* ���ò������ٽ��� */
	if (!mutex) mutex = CreateMutexA(NULL, FALSE, NULL);
	if(mutex) WaitForSingleObject(mutex, INFINITE);

	/* ���ڴ���з��� */
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

	/* �˳��ٽ��� */
	if (mutex) ReleaseMutex(mutex);

	/* ����Ƿֲ㴰���򴰿ڴ�����ɺ�������͸���� */
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
	/* �����������ò��Դ�����Եó�����Ҫͬ����Ϣ����Ϣֵ
	* ��������յ�������һ��Ϣ����Ὣ��Ϣͬ����Ŀ�괰��
	* �� switch (uMsg) default: ��ʹ�� */
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

			/* ���ö�ʱ�� */
			SetTimer(hWnd, (UINT_PTR)hWnd, FREQUENCY_SCREEN_REFRESH, NULL);

			return 0;
		}
	case WM_PAINT:
	case WM_TIMER:
		{
			ULONG WinNumber = 0;

			WaitForSingleObject(mutex, INFINITE);

			/* ������ǰ���� */
			for (ULONG i = 0; i < WinCount; ++i) {
				if (HomeWin[i] == hWnd) {
					WinNumber = i;
					break;
				}
			}
			
			if (IsWindow(TargetWin[WinNumber]))
			{
				/* ��ȡ������DC */
				HDC hdc_owner = GetDC(hWnd);
				/* ��ȡĿ�괰��DC */
				HDC hdc_other = GetDC(TargetWin[WinNumber]);

				/* �����ڿͻ��� */
				RECT rect_owner;
				GetWindowRect(hWnd, &rect_owner);

				/* Ŀ�괰�ڿͻ��� */
				RECT rect_other;
				GetWindowRect(TargetWin[WinNumber], &rect_other);

				/* ʹ��Ч������Ч���������ظ����� WM_PAINT */
				if (uMsg == WM_PAINT) ValidateRect(hWnd, &rect_owner);

				/* �ı�ͼ����ģʽ����������ã����ܻ�ʧ������ */
				SetStretchBltMode(hdc_owner, HALFTONE);
				SetStretchBltMode(hdc_other, HALFTONE);

				/* ����ͼ�� */
				StretchBlt(
					hdc_owner, 0, 0, 
					rect_owner.right - rect_owner.left,
					rect_owner.bottom - rect_owner.top,

					hdc_other, 0, 0,
					(int)((rect_other.right - rect_other.left) * lrGetWindowDpiZoom(TargetWin[WinNumber]).fw),
					(int)((rect_other.bottom - rect_other.top) * lrGetWindowDpiZoom(TargetWin[WinNumber]).fh),
					SRCCOPY
				);

				/* �ͷ�����DC */
				ReleaseDC(hWnd, hdc_owner);
				ReleaseDC(TargetWin[WinNumber], hdc_other);


				if (GetForegroundWindow() != hWnd)
				{
					/* У׼���ڱ��� */
					char WinTitle[MAXLEN_WIN_TITLE];
					memset(WinTitle, '\0', MAXLEN_WIN_TITLE);
					GetWindowTextA(TargetWin[WinNumber], WinTitle, MAXLEN_WIN_TITLE);
					SetWindowTextA(HomeWin[WinNumber], WinTitle);

					/* У׼���ڴ�С */
					RECT winWH;
					GetWindowRect(TargetWin[WinNumber], &winWH);
					SetWindowPos(hWnd, 0, 0, 0, winWH.right - winWH.left, winWH.bottom - winWH.top, SWP_NOMOVE | SWP_NOZORDER);

					/* У׼���ڷ�� */
					LONG style = GetWindowLongA(TargetWin[WinNumber], GWL_STYLE);
					SetWindowLongA(hWnd, GWL_STYLE, style);
					style = GetWindowLongA(TargetWin[WinNumber], GWL_EXSTYLE);
					SetWindowLongA(hWnd, GWL_EXSTYLE, style);

					/* ������Ƿ���зֲ㴰�� */
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
			/* ɾ����ʱ�� */
			KillTimer(hWnd, (UINT_PTR)hWnd);

			WaitForSingleObject(mutex, INFINITE);
			/* ���ٲ���Ҫ�Ŀռ� */
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
			/* �����Ϣ�Ƿ���Ҫͬ�� */
			for (size_t i = 0; i < sizeof(CopySend) / sizeof(UINT); ++i)
			{
				if (CopySend[i] == uMsg)
				{
					ULONG WinNumber = 0;

					WaitForSingleObject(mutex, INFINITE);

					/* ������ǰ���� */
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

			// ���Դ���
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
