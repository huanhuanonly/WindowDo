// lrGetWindowInfo.c -- Get handle and info of all window
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lrWindowDo.h"

#pragma warning (disable : 6308)
#pragma warning (disable : 28182)
#pragma warning (disable : 26451)

#pragma warning (disable : 4311; disable : 4312; disable : 4133)

typedef struct ALLHWND
{
	HWND* hwnd;
	size_t quan;
}ALLHWND, * PALLHWND;


static BOOL CALLBACK lrGetWindowHwndSetsProc(HWND hwnd, PALLHWND AllHwnd);


/* 判断游标是否在窗口中
* 
* 参数:
*		_hwnd         |  窗口句柄
* 
* 返回值:
*		TRUE          |  在窗口(_hwnd)中
*		FALSE         |  不在窗口(_hwnd)中
*/
BOOL InWindow(HWND _hwnd)
{
	if (!IsWindow(_hwnd)) return FALSE;

	RECT rect;
	GetWindowRect(_hwnd, &rect);

	POINT curpos;
	GetCursorPos(&curpos);

	if (curpos.x > rect.left
		&& curpos.x < rect.right
		&& curpos.y > rect.top
		&& curpos.y < rect.bottom)
	{
		return TRUE;
	}
	else return FALSE;
}

/* 判断游标是否在窗口边框上
* 
* 参数:
*		_hwnd         |  窗口句柄
*		_x            |  X 轴容差
*		_y            |  Y 轴容差
* 
* 返回值:
*		TRUE          |  在窗口(_hwnd)边框上
*		FALSE         |  不在窗口(_hwnd)边框上
*/
BOOL InWindowJamb(HWND _hwnd, UINT _x, UINT _y)
{
	if (!IsWindow(_hwnd)) return FALSE;

	RECT rect;
	GetWindowRect(_hwnd, &rect);

	POINT curpos;
	GetCursorPos(&curpos);

	if (((curpos.x >= rect.left - (LONG)_x) && (curpos.x <= rect.left + (LONG)_x))
		|| ((curpos.x >= rect.right - (LONG)_x) && (curpos.x <= rect.right + (LONG)_x))
		|| ((curpos.y >= rect.top - (LONG)_y) && (curpos.y <= rect.top + (LONG)_y))
		|| ((curpos.y >= rect.bottom - (LONG)_y) && (curpos.y <= rect.bottom + (LONG)_y))
		)
		return TRUE;

	else return FALSE;
}


/* 判断游标最接近窗口的哪条边
* 
* 参数:
*		_hwnd          |  窗口句柄
*		_ReturnOutside |  如果为 TRUE 则允许返回 NEARWINDOWJAMB_OUTSIDE
* 
* 返回值:
*		返回最接近的窗口边；
*		如果游标在窗口外且 _ReturnOutside 为 TRUE 时返回 NEARWINDOWJAMB_OUTSIDE；
*		否则正常判断。
*/
NEARWINDOWJAMB lrCursorNearWindowJamb(HWND _hwnd, BOOL _ReturnOutside)
{
	POINT curpos;
	GetCursorPos(&curpos);

	RECT rect;
	GetWindowRect(_hwnd, &rect);

	if (_ReturnOutside) if (!(curpos.x > rect.left && curpos.x < rect.right && curpos.y > rect.top && curpos.y < rect.bottom)) return NEARWINDOWJAMB_OUTSIDE;

	UINT iLeft = abs(curpos.x - rect.left);
	UINT iRight = abs(curpos.x - rect.right);
	UINT iTop = abs(curpos.y - rect.top);
	UINT iBottom = abs(curpos.y - rect.bottom);

	if (iLeft   <= iRight && iLeft   <= iTop   && iLeft   <= iBottom) return NEARWINDOWJAMB_LEFT;
	if (iRight  <= iLeft  && iRight  <= iTop   && iRight  <= iBottom) return NEARWINDOWJAMB_RIGHT;
	if (iTop    <= iLeft  && iTop    <= iRight && iTop    <= iBottom) return NEARWINDOWJAMB_TOP;
	if (iBottom <= iLeft  && iBottom <= iRight && iBottom <= iTop)    return NEARWINDOWJAMB_BOTTOM;

	return NEARWINDOWJAMB_LEFT;
}


/* 获取窗口 DPI 缩放百分比
* 
* 参数:
*		_hwnd         |  窗口句柄
* 
* 返回值:
*		返回一个 FSIZE 的结构体；
*		无错误返回，失败则使用默认 DPI。
*/
FSIZE lrGetWindowDpiZoom(HWND _hwnd)
{
//	if (!IsWindow(_hwnd)) return (FSIZE) { 1, 1 };

#if 0	/* 仅支持 WINDOWS 10 以上 */

	UINT dpi = GetDpiForWindow(_hwnd);
	/*
	*  公式由测试得出，zoom 是最后缩放的百分比。
	*
	*  if (dpi ==  96) zoom = 100;
	*  if (dpi == 120) zoom = 125;
	*  if (dpi == 144) zoom = 150;
	*  if (dpi == 168) zoom = 175;
	*  if (dpi == 192) zoom = 200;
	*/
	return dpi >= 96 ? dpi + ((dpi - 96) / 24) + 4 : 100;

#else	/* 支持 WINDOWS 2000 以上 */


	HDC hdc = GetDC(_hwnd);

	FSIZE fSize =
	{
		((float)GetDeviceCaps(hdc, DESKTOPHORZRES) / (float)GetDeviceCaps(hdc, HORZRES)),
		((float)GetDeviceCaps(hdc, DESKTOPVERTRES) / (float)GetDeviceCaps(hdc, VERTRES))
	};

	ReleaseDC(_hwnd, hdc);

	return fSize;
	
#endif
}


/* 获取游标在窗口中的坐标
* 
* 参数:
*		hWnd          |  窗口句柄
* 
* 返回值:
*		返回游标在 hWnd 窗口中的坐标；
*		会根据窗口风格减去窗体部分像素；
*		并兼容 DPI 缩放；
*		如果 hWnd 无效或游标不在窗口内则返回 0。
*/
POINT lrGetCursorInWindowPosition(HWND hWnd)
{
	if (!IsWindow(hWnd)) return (POINT) { 0L, 0L };
	POINT curpos;
	GetCursorPos(&curpos);

	ScreenToClient(hWnd, &curpos);
	return curpos;
}


/* 获取窗口归属进程的路径
* 
* 参数:
*		_hWnd         |  窗口句柄
*		_ProcessPath  |  路径载体的字符串指针
*		_PathMaxLen   |  _ProcessPath 的最大长度
* 
* 返回值:
*		返回 _ProcessPath。
*/
char* lrGetWindowProcessPath(HWND _hWnd, char* _ProcessPath, size_t _PathMaxLen)
{
	DWORD pid;
	GetWindowThreadProcessId(_hWnd, &pid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	GetModuleFileNameExA(hProcess, NULL, _ProcessPath, (DWORD)_PathMaxLen);
	CloseHandle(hProcess);

	return _ProcessPath;
}

/* 获取所有指定的进程路径的窗口（包括子窗口）
* 
* 参数:
*		_ProcessPath  |  指定的进程路径
*		_AllWindow    |  如果不为 NULL 则赋值所有符合条件的窗口，需要释放
*		_WindowQuan   |  如果 _AllWindow 不为 NULL，则赋值 _AllWindow 的窗口数量
* 
* 返回值:
*		成功找到则返回第一个找到符合条件的窗口；
*		失败则返回 NULL。
*/
HWND lrFindProcessPathWindow(const char* const _ProcessPath, HWND** _AllWindow, UINT* _WindowQuan)
{
	size_t awq = 0;
	HWND* aw = lrGetParentAndChildWindowHwndSets(&awq);

	char path[MAX_PATH] = "\0";

	if (_AllWindow == NULL || _WindowQuan == NULL) {
		HWND ret = NULL;

		for (size_t i = 0; i < awq; ++i)
		{
			lrGetWindowProcessPath(aw[i], path, sizeof(path));
			if (!stricmp(_ProcessPath, path))
			{
				ret = aw[i];
				free(aw);
				return ret;
			}
		}
		free(aw);
		return NULL;
	}
	else
	{
		*_WindowQuan = 0;

		if (*_AllWindow = (HWND*)malloc(sizeof(HWND)))
		{
			for (size_t i = 0; i < awq; ++i)
			{
				lrGetWindowProcessPath(aw[i], path, sizeof(path));
				if (!stricmp(_ProcessPath, path))
				{
					(*_AllWindow)[(*_WindowQuan)++] = aw[i];
					*_AllWindow = (HWND*)realloc(*_AllWindow, (size_t)((size_t)(*_WindowQuan) + 1) * sizeof(HWND));
					if (*_AllWindow == NULL) {
						*_WindowQuan = 0;
						free(aw);
						return NULL;
					}
					continue;
				}
			}
			free(aw);

#pragma warning (suppress : 6001)
			if (*_WindowQuan) return *_AllWindow[0];
			else {
				free(*_AllWindow);
				*_AllWindow = NULL;
				return NULL;
			}
		}
		else return NULL;
	}
	return NULL;
}


BOOL lrGetCommandLine(DWORD pId, CHAR* pCmdLine, DWORD dwBufLen)
{
	HANDLE hProc = OpenProcess(PROCESS_VM_READ, FALSE, pId);
	if (hProc == NULL)
	{
		return FALSE;
	}

	/* 第2个字节开始才是我们要读的地址 */
	DWORD dwAddr = *(DWORD*)((DWORD)GetCommandLineA + 1);
	CHAR tcBuf[512] = { '\0' };

	DWORD dwRead = 0;

	DWORD dwVer = GetVersion();
	
	if (dwVer < 0x80000000)	// Windows NT/2000/XP
	{
		if (ReadProcessMemory(hProc, (LPVOID)dwAddr, &dwAddr, 4, &dwRead))
		{
			if (ReadProcessMemory(hProc, (LPVOID)dwAddr, tcBuf, sizeof(tcBuf), &dwRead))
			{
				/* 最好检查一下 dwRead 和 dwBufLen 的大小，使用较小的那个 */
				strncpy(pCmdLine, tcBuf, dwBufLen);
			}
		}
	}
	else	// Windows 95/98/Me and Win32s
	{
		/* 使用 while 是为了出错时方便跳出循环 */
		while (TRUE)
		{
			if (!ReadProcessMemory(hProc, (LPVOID)dwAddr, &dwAddr, 4, &dwRead)) break;

			if (!ReadProcessMemory(hProc, (LPVOID)dwAddr, &dwAddr, 4, &dwRead)) break;

			if (!ReadProcessMemory(hProc, (LPVOID)(dwAddr + 0xC0), tcBuf, sizeof(tcBuf), &dwRead)) break;

			if (*tcBuf == 0)
			{
				if (!ReadProcessMemory(hProc, (LPVOID)(dwAddr + 0x40), &dwAddr, 4, &dwRead)) break;

				if (!ReadProcessMemory(hProc, (LPVOID)(dwAddr + 0x08), &dwAddr, 4, &dwRead)) break;

				if (!ReadProcessMemory(hProc, (LPVOID)dwAddr, tcBuf, sizeof(tcBuf), &dwRead)) break;
			}
			/* 最好检查一下 dwRead 和 dwBufLen 的大小，使用较小的那个 */
			strncpy(pCmdLine, tcBuf, dwBufLen);
			break;
		}
	}
	CloseHandle(hProc);
	return TRUE;
}

/* 获取窗口所有信息
* 
* 参数:
*		hwnd          |  窗口句柄
*		p             |  指向一个 WINDOWALLINFO 类型的结构体
*/
void lrGetWindowAllInfo(HWND hwnd, PWINDOWALLINFO p)
{
	memset(p, 0, sizeof(WINDOWALLINFO));

	p->ID = hwnd;

	if (!IsWindow(hwnd)) return;

	GetWindowThreadProcessId(hwnd, &(p->PID));

	GetWindowTextA(hwnd, p->TITLE, sizeof(p->TITLE));
	RealGetWindowClassA(hwnd, p->CLASS, sizeof(p->CLASS));

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, p->PID);
	GetModuleFileNameExA(hProcess, NULL, p->PPATH, MAX_PATH);
	CloseHandle(hProcess);

	if (IsWindowUnicode(hwnd)) p->CODE = WINCODE_UNICODE; else p->CODE = WINCODE_ASCII;

	GetWindowRect(hwnd, &(p->VERTEX));

	p->PARENT = GetParent(hwnd);
	if (p->PARENT)
	{
		POINT point = { p->VERTEX.left,p->VERTEX.top };
		ScreenToClient(p->PARENT, &point);
		p->CVERTEX.left = point.x;
		p->CVERTEX.top = point.y;

		point = (POINT){ p->VERTEX.right,p->VERTEX.bottom };
		ScreenToClient(p->PARENT, &point);
		p->CVERTEX.right = point.x;
		p->CVERTEX.bottom = point.y;
	}
	else p->CVERTEX = p->VERTEX;
	
	p->WINSIZE.cx = p->VERTEX.right - p->VERTEX.left;
	p->WINSIZE.cy = p->VERTEX.bottom - p->VERTEX.top;

	p->DPIZOOM = lrGetWindowDpiZoom(p->ID);

	p->SHOW = IsWindowVisible(hwnd);

	GetCursorPos(&(p->CURPOS));

	HDC hdc = GetDC(NULL);
	p->CURCOLOR = GetPixel(hdc, (int)(p->CURPOS.x * p->DPIZOOM.fw), (int)(p->CURPOS.y * p->DPIZOOM.fh));
	ReleaseDC(NULL, hdc);

	p->CURINWINPOS.x = p->CURPOS.x;
	p->CURINWINPOS.y = p->CURPOS.y;
	ScreenToClient(hwnd, &(p->CURINWINPOS));

	p->STYLE = GetWindowLongA(hwnd, GWL_STYLE);
	p->STYLEEX = GetWindowLongA(hwnd, GWL_EXSTYLE);
	p->CLASSSTYLE = GetClassLongA(hwnd, GCL_STYLE);

	return;
}



/* 获取所有父窗口句柄
*
* 参数:
*		_WinQuantity  |  接收返回的句柄数量（不能为NULL）
*
* 返回值:
*		成功则返回获取的窗口句柄，需要释放。
*/
HWND* lrGetWindowHwndSets(size_t* _WinQuantity)
{
	ALLHWND AllHwnd = { 0 };

	AllHwnd.quan = 1;

	AllHwnd.hwnd = (HWND*)malloc(sizeof(HWND));
	if (AllHwnd.hwnd == NULL) return NULL;

	EnumWindows((WNDENUMPROC)lrGetWindowHwndSetsProc, (LPARAM)&AllHwnd);

	*_WinQuantity = --AllHwnd.quan;

	return AllHwnd.hwnd;
}


/* 获取所有子窗口句柄
* 
* 参数:
*		_ParentWindow |  父窗口句柄
*		_WinQuantity  |  接收返回的句柄数量（不能为NULL）
* 
* 返回值:
*		成功则返回获取的窗口句柄，需要释放。
*/
HWND* lrGetChildWindowHwndSets(HWND _ParentWindow, size_t* _WinQuantity)
{
	if (_WinQuantity == NULL) return NULL;

	if (IsWindow(_ParentWindow))
	{
		ALLHWND AllHwnd = { 0 };

		AllHwnd.quan = 1;

		AllHwnd.hwnd = (HWND*)malloc(sizeof(HWND));
		if (AllHwnd.hwnd == NULL) return NULL;

		EnumChildWindows(_ParentWindow, (WNDENUMPROC)lrGetWindowHwndSetsProc, (LPARAM)&AllHwnd);

		*_WinQuantity = --AllHwnd.quan;

		return AllHwnd.hwnd;
	}
	else return NULL;
}


/* 获取所有父窗口和子窗口句柄
* 
* 参数:
*		_wq           |  接收返回的句柄数量（不能为NULL）
* 
* 返回值:
*		成功则返回获取的窗口句柄，需要释放。
*/
HWND* lrGetParentAndChildWindowHwndSets(size_t* _wq)
{
	if (_wq == NULL) return NULL;

	size_t parentQuan = 0;
	HWND* parent = lrGetWindowHwndSets(&parentQuan);

	if (parent == NULL) return NULL;
	if (parentQuan == 0)
	{
		free(parent);
		return NULL;
	}

	*_wq = parentQuan;

	HWND* wins = (HWND*)malloc(parentQuan * sizeof(HWND));
	if (wins == NULL)
	{
		*_wq = 0;
		free(parent);
		return NULL;
	}

	for (size_t i = 0, pos = 0; i < parentQuan; ++i)
	{
		wins[pos++] = parent[i];

		size_t childQuan = 0;
		HWND* child = lrGetChildWindowHwndSets(parent[i], &childQuan);
		if (child)
		{
			if (childQuan)
			{
				if (wins = (HWND*)realloc(wins, ((*_wq) += childQuan) * sizeof(HWND)))
				{
					for (size_t j = 0; j < childQuan; ++j, ++pos)
					{
						wins[pos] = child[j];
					}
				}
			}
			free(child);
		}
	}
	free(parent);
	return wins;
}


static BOOL CALLBACK lrGetWindowHwndSetsProc(HWND hwnd, PALLHWND pAllHwnd)
{
	pAllHwnd->hwnd[pAllHwnd->quan - 1] = hwnd;

	HWND* HwndTemp = (HWND*)realloc(pAllHwnd->hwnd, (++pAllHwnd->quan) * sizeof(HWND));
	if (HwndTemp == NULL)
	{
		--pAllHwnd->quan;
		return FALSE;
	}
	else pAllHwnd->hwnd = HwndTemp;
	
	return TRUE;
}