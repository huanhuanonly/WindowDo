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


/* �ж��α��Ƿ��ڴ�����
* 
* ����:
*		_hwnd         |  ���ھ��
* 
* ����ֵ:
*		TRUE          |  �ڴ���(_hwnd)��
*		FALSE         |  ���ڴ���(_hwnd)��
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

/* �ж��α��Ƿ��ڴ��ڱ߿���
* 
* ����:
*		_hwnd         |  ���ھ��
*		_x            |  X ���ݲ�
*		_y            |  Y ���ݲ�
* 
* ����ֵ:
*		TRUE          |  �ڴ���(_hwnd)�߿���
*		FALSE         |  ���ڴ���(_hwnd)�߿���
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


/* �ж��α���ӽ����ڵ�������
* 
* ����:
*		_hwnd          |  ���ھ��
*		_ReturnOutside |  ���Ϊ TRUE �������� NEARWINDOWJAMB_OUTSIDE
* 
* ����ֵ:
*		������ӽ��Ĵ��ڱߣ�
*		����α��ڴ������� _ReturnOutside Ϊ TRUE ʱ���� NEARWINDOWJAMB_OUTSIDE��
*		���������жϡ�
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


/* ��ȡ���� DPI ���Űٷֱ�
* 
* ����:
*		_hwnd         |  ���ھ��
* 
* ����ֵ:
*		����һ�� FSIZE �Ľṹ�壻
*		�޴��󷵻أ�ʧ����ʹ��Ĭ�� DPI��
*/
FSIZE lrGetWindowDpiZoom(HWND _hwnd)
{
//	if (!IsWindow(_hwnd)) return (FSIZE) { 1, 1 };

#if 0	/* ��֧�� WINDOWS 10 ���� */

	UINT dpi = GetDpiForWindow(_hwnd);
	/*
	*  ��ʽ�ɲ��Եó���zoom ��������ŵİٷֱȡ�
	*
	*  if (dpi ==  96) zoom = 100;
	*  if (dpi == 120) zoom = 125;
	*  if (dpi == 144) zoom = 150;
	*  if (dpi == 168) zoom = 175;
	*  if (dpi == 192) zoom = 200;
	*/
	return dpi >= 96 ? dpi + ((dpi - 96) / 24) + 4 : 100;

#else	/* ֧�� WINDOWS 2000 ���� */


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


/* ��ȡ�α��ڴ����е�����
* 
* ����:
*		hWnd          |  ���ھ��
* 
* ����ֵ:
*		�����α��� hWnd �����е����ꣻ
*		����ݴ��ڷ���ȥ���岿�����أ�
*		������ DPI ���ţ�
*		��� hWnd ��Ч���α겻�ڴ������򷵻� 0��
*/
POINT lrGetCursorInWindowPosition(HWND hWnd)
{
	if (!IsWindow(hWnd)) return (POINT) { 0L, 0L };
	POINT curpos;
	GetCursorPos(&curpos);

	ScreenToClient(hWnd, &curpos);
	return curpos;
}


/* ��ȡ���ڹ������̵�·��
* 
* ����:
*		_hWnd         |  ���ھ��
*		_ProcessPath  |  ·��������ַ���ָ��
*		_PathMaxLen   |  _ProcessPath ����󳤶�
* 
* ����ֵ:
*		���� _ProcessPath��
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

/* ��ȡ����ָ���Ľ���·���Ĵ��ڣ������Ӵ��ڣ�
* 
* ����:
*		_ProcessPath  |  ָ���Ľ���·��
*		_AllWindow    |  �����Ϊ NULL ��ֵ���з��������Ĵ��ڣ���Ҫ�ͷ�
*		_WindowQuan   |  ��� _AllWindow ��Ϊ NULL����ֵ _AllWindow �Ĵ�������
* 
* ����ֵ:
*		�ɹ��ҵ��򷵻ص�һ���ҵ����������Ĵ��ڣ�
*		ʧ���򷵻� NULL��
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

	/* ��2���ֽڿ�ʼ��������Ҫ���ĵ�ַ */
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
				/* ��ü��һ�� dwRead �� dwBufLen �Ĵ�С��ʹ�ý�С���Ǹ� */
				strncpy(pCmdLine, tcBuf, dwBufLen);
			}
		}
	}
	else	// Windows 95/98/Me and Win32s
	{
		/* ʹ�� while ��Ϊ�˳���ʱ��������ѭ�� */
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
			/* ��ü��һ�� dwRead �� dwBufLen �Ĵ�С��ʹ�ý�С���Ǹ� */
			strncpy(pCmdLine, tcBuf, dwBufLen);
			break;
		}
	}
	CloseHandle(hProc);
	return TRUE;
}

/* ��ȡ����������Ϣ
* 
* ����:
*		hwnd          |  ���ھ��
*		p             |  ָ��һ�� WINDOWALLINFO ���͵Ľṹ��
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



/* ��ȡ���и����ھ��
*
* ����:
*		_WinQuantity  |  ���շ��صľ������������ΪNULL��
*
* ����ֵ:
*		�ɹ��򷵻ػ�ȡ�Ĵ��ھ������Ҫ�ͷš�
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


/* ��ȡ�����Ӵ��ھ��
* 
* ����:
*		_ParentWindow |  �����ھ��
*		_WinQuantity  |  ���շ��صľ������������ΪNULL��
* 
* ����ֵ:
*		�ɹ��򷵻ػ�ȡ�Ĵ��ھ������Ҫ�ͷš�
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


/* ��ȡ���и����ں��Ӵ��ھ��
* 
* ����:
*		_wq           |  ���շ��صľ������������ΪNULL��
* 
* ����ֵ:
*		�ɹ��򷵻ػ�ȡ�Ĵ��ھ������Ҫ�ͷš�
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