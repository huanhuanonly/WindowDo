// lrWindowMotion.c -- Window control
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lrWindowDo.h"
#include "../Head/lr_wdo.h"
#include "../Head/lrMath.h"

#pragma warning (disable : 6308)

/* lrWindowDriftProc() : 参数的载体 */
typedef struct __lrWindowDriftProc
{
	HWND hwnd;				// 正在运动的窗口句柄

	int* End_X;				// 终点的坐标集合
	int* End_Y;

	DWORD* frequency;		// 运动频率集合

	size_t quantity;		// 集合数量

}DRIFT, * PDRIFT;


typedef struct WINDOWMOTION_HWNDANDSWITCH
{
	HWND _hWnd;
	BOOL _Switch;
}WINDOWMOTION_HWNDANDSWITCH;


typedef struct WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO
{
	HWND _hWnd;				// 窗口句柄
	BOOL _Switch;			// 开关
	UINT _Distance;			// 击退距离

	BOOL _SpringBack;		// 如果为 TRUE 则回弹（窗口越过屏幕边界时自动弹回）
	UINT _SpringBackPixel;	// 额外回弹的像素（方便游标重新推回窗口）

	lrWINDOW_AND_CURSOR_MOVE_AFTER_COLLIDE_MODE _Mode;

}WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO;






static DWORD WINAPI lrWindowDriftProc(DRIFT* _PDRIFT);
static DWORD WINAPI lrWindowFollowCursorProc(HWND _hwnd);
static DWORD WINAPI lrWindowWiggleProc(HWND _hwnd);
static DWORD WINAPI lrWindowAsideProc(HWND _hwnd);
static DWORD WINAPI lrWindowAndCursorMoveAfterCollideProc(WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO* _info);



BOOL lrMoveWindow(HWND _hwnd, int _x, int _y)
{
	if (!IsWindow(_hwnd)) return FALSE;

	RECT rect;
	GetWindowRect(_hwnd, &rect);

	HWND Parent = GetParent(_hwnd);

	if (!Parent)
	{
		return SetWindowPos(_hwnd, NULL, _x + rect.left, _y + rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	else
	{
		POINT point = { rect.left,rect.top };
		ScreenToClient(Parent, &point);

		point.x += _x;
		point.y += _y;
		
		return SetWindowPos(_hwnd, NULL, point.x, point.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
}


BOOL lrZoomWindow(HWND _hwnd, int _w, int _h)
{
	if (!IsWindow(_hwnd)) return FALSE;

	HWND Parent = GetParent(_hwnd);

	RECT rect;

	if (!Parent)
	{
		GetWindowRect(_hwnd, &rect);
	}
	else GetClientRect(_hwnd, &rect);

	return SetWindowPos(_hwnd, NULL, 0, 0, rect.right - rect.left + _w, rect.bottom - rect.top + _h, SWP_NOZORDER | SWP_NOMOVE);
}


BOOL lrGetWindowPos(HWND _hwnd, int* _x, int* _y, int* _w, int* _h, GWP_LR_ORIGINALPOINT _OriginalPoint)
{
	if (!IsWindow(_hwnd)) return FALSE;

	BOOL ret = FALSE;
	RECT rect = { 0 };

	switch (_OriginalPoint)
	{
	case GWP_LR_ORIGINALPOINT_SCREEN:
	{
		ret = GetWindowRect(_hwnd, &rect);
		break;
	}
	case GWP_LR_ORIGINALPOINT_PARENT:
	{
		HWND Parent = GetParent(_hwnd);

		if (Parent)
		{
			if (ret = GetWindowRect(_hwnd, &rect))
			{
				POINT point = { rect.left,rect.top };
				ScreenToClient(Parent, &point);
				rect.left = point.x;
				rect.top = point.y;

				point = (POINT){ rect.right,rect.bottom };
				ScreenToClient(Parent, &point);
				rect.right = point.x;
				rect.bottom = point.y;
			}
		}
		else ret = GetWindowRect(_hwnd, &rect);

		break;
	}
	default:
		ret = FALSE;
		rect = (RECT){ 0,0,0,0 };
		break;
	}

	if (ret)
	{
		if (_x) *_x = rect.left;
		if (_y) *_y = rect.top;
		if (_w) *_w = rect.right - rect.left;
		if (_h) *_h = rect.bottom - rect.top;
	}
	else
	{
		if (_x) *_x = 0;
		if (_y) *_y = 0;
		if (_w) *_w = 0;
		if (_h) *_h = 0;
	}

	return ret;
}


BOOL lrSetWindowPos(HWND _hwnd, int _x, int _y, GWP_LR_ORIGINALPOINT _OriginalPoint)
{
	if (!IsWindow(_hwnd)) return FALSE;

	HWND Parent = GetParent(_hwnd);

	switch (_OriginalPoint)
	{
	case GWP_LR_ORIGINALPOINT_SCREEN:
	{
		POINT point = { _x,_y };
		ScreenToClient(Parent, &point);

		return SetWindowPos(_hwnd, NULL, point.x, point.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	case GWP_LR_ORIGINALPOINT_PARENT:
	{
		return SetWindowPos(_hwnd, NULL, _x, _y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	default: return FALSE;
	}
}


/* 向窗口打印文字和图片。
* 
* 参数:
*		_hwnd       |  窗口句柄
*		_x          |  文本打印点
*		_y          |  文本打印点
*		_Text       |  要打印的文本
*		_OutAlias   |  类型为 alias_type_out 的别名
* 
* 返回值:
*		成功返回 TRUE，失败返回 FALSE。
* 
* 言论:
*		如果 _OutAlias 为 NULL，则使用 lrGetAliasDef() 获取默认别名。
*/
BOOL lrWindowPrint(HWND _hwnd, int _x, int _y, LPCSTR _Text, const ALIAS_OUT* _OutAlias)
{
	if (!IsWindow(_hwnd)) return FALSE;

	ALIASSINGLE alias;
	BOOL NullAlias = FALSE;
	BOOL ret = FALSE;

	if (_OutAlias == NULL)
	{
		NullAlias = TRUE;
		lrGetAliasDef(alias_def_out, &alias);
	}
	else alias.data.out = *_OutAlias;

	HDC hdc = GetDC(_hwnd);

	SetTextColor(hdc, alias.data.out.TextColor);
	SetBkColor(hdc, alias.data.out.BackColor);
	SetBkMode(hdc, alias.data.out.IsTransparent ? TRANSPARENT : OPAQUE);
	SelectObject(hdc, alias.data.out.hFont);

	if (_Text != NULL && _Text[0] != '\0')
		ret = TextOutA(hdc, _x, _y, _Text, (int)strlen(_Text));

	if (alias.data.out.image[0] != '\0')
	{
		HBITMAP hBitmap = LoadImageA(NULL, alias.data.out.image, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (hBitmap)
		{
			HDC hdcMem = CreateCompatibleDC(hdc);
			SIZE imageSize = lrGetBitmapSize(alias.data.out.image);

			SelectObject(hdcMem, hBitmap);
			ret = BitBlt(hdc, _x + alias.data.out.ImageInTextPos.X, _y + alias.data.out.ImageInTextPos.Y, imageSize.cx, imageSize.cy, hdcMem, 0, 0, SRCCOPY);
			DeleteObject(hBitmap);
			DeleteDC(hdcMem);
		}
	}
	if(NullAlias) lrClearAlias(&alias);

	ReleaseDC(_hwnd, hdc);

	return ret;
}




/* 使窗口进行漂移运动。
* 不会等待漂移完成，一旦开始漂移，除非到达目的地或程序结束，否则不可终止。
* 
* 参数:
*		_hwnd		|  目标窗口
*		_x          |  终点坐标
*		_y          |  终点坐标
*		_speed      |  移动速度[1-100]
* 
* 返回值:
*		FALSE       |  操作失败，内存分配失败或 _hwnd 不是一个有效的窗口。
*		TRUE        |  操作成功。
*/
BOOL lrWindowDrift(HWND _hwnd, int _x, int _y, BYTE _speed)
{
	if (!IsWindow(_hwnd)) return FALSE;

	DRIFT* drift = (DRIFT*)malloc(sizeof(DRIFT));
	if (drift == NULL) return FALSE;

	drift->hwnd = _hwnd;
	drift->quantity = 1;

	drift->End_X = (int*)malloc(sizeof(int));
	drift->End_Y = (int*)malloc(sizeof(int));
	drift->frequency = (DWORD*)malloc(sizeof(DWORD));

	if (drift->End_X == NULL
		|| drift->End_Y == NULL
		|| drift->frequency == NULL)
		return FALSE;

	
	drift->End_X[0] = _x;
	drift->End_Y[0] = _y;
	drift->frequency[0] = (DWORD)(100 - lr_LIMIT_VALSIZE(_speed, 1, 100));

		
	/* 调用另一线程执行，否则将等待漂移完成，
	* 内存的释放也将在此线程中完成 */
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lrWindowDriftProc, drift, 0, 0);

	return TRUE;
}



/* 使窗口进行连续多次漂移运动。
* 不会等待漂移完成，一旦开始漂移，除非到达所有目的地或程序结束，否则不可终止。
*
* 参数:
*		_hwnd		|  目标窗口
*		_quantity   |  连续运动的次数，同样也是数组的大小
*		_x          |  终点坐标的数组，大小不应小于 _quantity
*		_y          |  终点坐标的数组，大小不应小于 _quantity
*		_speed      |  移动速度的数组，大小不应小于 _quantity
*
* 返回值:
*		FALSE       |  操作失败，内存分配失败、_hwnd 不是一个有效的窗口、_quantity 为 0。
*		TRUE        |  操作成功。
*/
BOOL lrWindowDrifts(HWND _hwnd, size_t _quantity, int _x[], int _y[], BYTE _speed[])
{
	if (!IsWindow(_hwnd)) return FALSE;
	else if (!_quantity) return FALSE;

	DRIFT* drift = (DRIFT*)malloc(sizeof(DRIFT));
	if (drift == NULL) return FALSE;

	drift->hwnd = _hwnd;
	drift->quantity = _quantity;

	drift->End_X = (int*)malloc(_quantity * sizeof(int));
	drift->End_Y = (int*)malloc(_quantity * sizeof(int));
	drift->frequency = (DWORD*)malloc(_quantity * sizeof(DWORD));

	if (drift->End_X == NULL
		|| drift->End_Y == NULL
		|| drift->frequency == NULL)
		return FALSE;

	for (size_t i = 0; i < _quantity; ++i)
	{
		drift->End_X[i] = _x[i];
		drift->End_Y[i] = _y[i];
		drift->frequency[i] = (DWORD)(100 - lr_LIMIT_VALSIZE(_speed[i], 1, 100));
	}

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lrWindowDriftProc, drift, 0, 0);

	return TRUE;
}



static struct __WindowFollow__Struct
{
	HWND hwnd;			// 归属窗口，同样是标识符
	int x;				// x 偏移量
	int y;				// y 偏移量
	BOOL closebool;		// 为 TRUE 时退出
} *WindowFollow;
/* follow: 窗口数量 */
static volatile ULONG WindowFollow_quantity = 0;
/* follow: 临界互斥 */
static volatile HANDLE WindowFollow_mutex = 0;


/* 使窗口跟随游标移动。
* 
* 参数:
*		_hwnd       |  窗口句柄
*		_x          |  距离游标的偏移量
*		_y          |  距离游标的偏移量
*		_switch     |  TRUE: 开始跟随，FALSE: 结束跟随
* 
* 返回值:
*		FALSE       |  操作失败。可能是以下原因：
*							1.> _hwnd 无效的窗口；
*							2.> 内存分配失败；
*							3.> 临界区设置失败；
*							4.> _switch 为 FALSE 时，_hwnd 指示的窗口没有处于跟随状态；
*		TRUE        |  操作成功。
*/
BOOL lrWindowFollowCursor(HWND _hwnd, int _x, int _y, BOOL _switch)
{
	if (!IsWindow(_hwnd)) return FALSE;

	/* 设置并进入临界区 */
	if (!WindowFollow_mutex) WindowFollow_mutex = CreateMutexA(NULL, FALSE, NULL);
	if (WindowFollow_mutex == NULL) return FALSE;

	WaitForSingleObject(WindowFollow_mutex, INFINITE);

	
	if (_switch)
	{
		/* 确认当前窗口编号 */
		ULONG WinNumber = -1;
		for (ULONG i = 0; i < WindowFollow_quantity; ++i) {
			if (WindowFollow[i].hwnd == _hwnd) {
				WinNumber = i;
				break;
			}
		}

		/* 没有搜索到窗口，需要进行初始化 */
		if (WinNumber == -1)
		{
			if (WindowFollow_quantity == 0)
			{
				++WindowFollow_quantity;

				WindowFollow = (struct __WindowFollow__Struct*)malloc(sizeof(struct __WindowFollow__Struct));
				if (WindowFollow == NULL) return FALSE;
			}
			else
			{
				++WindowFollow_quantity;

#pragma warning (suppress : 6308)
				WindowFollow = (struct __WindowFollow__Struct*)realloc(WindowFollow, WindowFollow_quantity * sizeof(struct __WindowFollow__Struct));
				if (WindowFollow == NULL) return FALSE;
			}
			WindowFollow[WindowFollow_quantity - 1].hwnd = _hwnd;
			WindowFollow[WindowFollow_quantity - 1].closebool = FALSE;
			WindowFollow[WindowFollow_quantity - 1].x = _x;
			WindowFollow[WindowFollow_quantity - 1].y = _y;

			/* 退出临界区 */
			ReleaseMutex(WindowFollow_mutex);

			/* 开始跟随 */
			CreateThread(NULL, 0, lrWindowFollowCursorProc, _hwnd, 0, 0);
		}
		/* 直接覆盖原来设置即可 */
		else
		{
			WindowFollow[WinNumber].x = _x;
			WindowFollow[WinNumber].y = _y;

			ReleaseMutex(WindowFollow_mutex);
		}
	}
	/* 关闭跟随 */
	else
	{
		if (WindowFollow_quantity)
		{
			/* 确认当前窗口编号 */
			ULONG WinNumber = -1;
			for (ULONG i = 0; i < WindowFollow_quantity; ++i) {
				if (WindowFollow[i].hwnd == _hwnd) {
					WinNumber = i;
					break;
				}
			}
			if (WinNumber != -1)
				WindowFollow[WinNumber].closebool = TRUE;

			else {
				ReleaseMutex(WindowFollow_mutex);
				return FALSE;
			}
		}
		else {
			ReleaseMutex(WindowFollow_mutex);
			return FALSE;
		}
		ReleaseMutex(WindowFollow_mutex);
	}
	return TRUE;
}



static struct __lrWindowWiggle__Struct
{
	HWND hwnd;
	UINT range;
	UINT freq;
	BOOL closebool;
}*WindowWiggle;
static volatile ULONG WindowWiggle_quantity = 0;
static volatile HANDLE WindowWiggle_mutex = 0;


/* 使窗口进行摆动。
* 
* 参数:
*		_hwnd       |  窗口句柄
*		_range      |  摆动的范围
*		_freq       |  摆动的频率
*		_switch     |  TRUE: 开始摆动，FALSE: 结束摆动
* 
* 返回值:
*		FALSE: 操作失败。
*		TRUE:  操作成功。
*/
BOOL lrWindowWiggle(HWND _hwnd, UINT _range, UINT _freq, BOOL _switch)
{
	if (!IsWindow(_hwnd)) return FALSE;

	if (!WindowWiggle_mutex) WindowWiggle_mutex = CreateMutexA(NULL, FALSE, NULL);
	if (!WindowWiggle_mutex) return FALSE;

	lr_srand();

	WaitForSingleObject(WindowWiggle_mutex, INFINITE);

	if (_switch)
	{
		/* 确认当前窗口编号 */
		ULONG WinNumber = -1;
		for (ULONG i = 0; i < WindowWiggle_quantity; ++i) {
			if (WindowWiggle[i].hwnd == _hwnd) {
				WinNumber = i;
				break;
			}
		}
		/* 没有搜索到窗口，需要进行初始化 */
		if (WinNumber == -1)
		{
			if (WindowWiggle_quantity == 0)
			{
				++WindowWiggle_quantity;

				WindowWiggle = (struct __lrWindowWiggle__Struct*)malloc(sizeof(struct __lrWindowWiggle__Struct));
				if (WindowWiggle == NULL) return FALSE;
			}
			else
			{
				++WindowWiggle_quantity;

				struct __lrWindowWiggle__Struct* WindowWiggleTemp = (struct __lrWindowWiggle__Struct*)realloc(WindowWiggle, WindowWiggle_quantity * sizeof(struct __lrWindowWiggle__Struct));
				
				if (WindowWiggleTemp == NULL) return FALSE;
				else WindowWiggle = WindowWiggleTemp;
			}

			WindowWiggle[WindowWiggle_quantity - 1].hwnd = _hwnd;
			WindowWiggle[WindowWiggle_quantity - 1].range = _range;
			WindowWiggle[WindowWiggle_quantity - 1].freq = _freq;
			WindowWiggle[WindowWiggle_quantity - 1].closebool = FALSE;

			ReleaseMutex(WindowWiggle_mutex);

			CreateThread(NULL, 0, lrWindowWiggleProc, _hwnd, 0, 0);
		}
		/* 直接覆盖原来设置即可 */
		else
		{
			WindowWiggle[WinNumber].range = _range;
			WindowWiggle[WinNumber].freq = _freq;

			ReleaseMutex(WindowWiggle_mutex);
		}
	}
	else
	{
		if (WindowWiggle_quantity)
		{
			/* 确认当前窗口编号 */
			ULONG WinNumber = -1;
			for (ULONG i = 0; i < WindowWiggle_quantity; ++i) {
				if (WindowWiggle[i].hwnd == _hwnd) {
					WinNumber = i;
					break;
				}
			}
			if (WinNumber != -1)
				WindowWiggle[WinNumber].closebool = TRUE;

			else {
				ReleaseMutex(WindowWiggle_mutex);
				return FALSE;
			}
		}
		else {
			ReleaseMutex(WindowWiggle_mutex);
			return FALSE;
		}
		ReleaseMutex(WindowWiggle_mutex);
	}
	return TRUE;
}


static struct __lrWindowAside__Struct
{
	HWND hwnd;		// 窗口
	BYTE speed;		// 移动速度[1-10]
	UINT rx;		// 露出在外的宽度
	UINT ry;		// 露出在外的高度
	BOOL closebool;	// 如果为 TRUE 则关闭
}*WindowAside;

static ULONG WindowAside_quantity = 0;
static HANDLE WindowAside_mutex = 0;

/* 使窗口靠边收入或弹出。
* 
* 参数:
*		_hwnd       |  窗口句柄
*		_retainX    |  窗口收入后保留的宽度
*		_retainY    |  窗口收入后保留的高度
*		_speed      |  移动的速度[1-10]
*		_switch     |  TRUE: 开始，FALSE: 结束
* 
* 返回值:
*		FALSE: 操作失败。
*		TRUE:  操作成功。
*/
BOOL lrWindowAside(HWND _hwnd, BYTE _speed, UINT _retainX, UINT _retainY, BOOL _switch)
{
	if (!IsWindow(_hwnd)) return FALSE;

	if (!WindowAside_mutex) WindowAside_mutex = CreateMutexA(NULL, FALSE, NULL);
	if (!WindowAside_mutex) return FALSE;

	WaitForSingleObject(WindowAside_mutex, INFINITE);

	/* 限制运动速度，否则速度可能过快 */
	lr_LIMIT_VALSIZE(_speed, 1, 10);

	if (_switch)
	{
		/* 确认当前窗口编号 */
		ULONG WinNumber = -1;
		for (ULONG i = 0; i < WindowAside_quantity; ++i) {
			if (WindowAside[i].hwnd == _hwnd) {
				WinNumber = i;
				break;
			}
		}
		/* 没有搜索到窗口，需要进行初始化 */
		if (WinNumber == -1)
		{
			if (WindowAside_quantity == 0)
			{
				++WindowAside_quantity;

				WindowAside = (struct __lrWindowAside__Struct*)malloc(sizeof(struct __lrWindowAside__Struct));
				if (WindowAside == NULL) return FALSE;
			}
			else
			{
				++WindowAside_quantity;

				struct __lrWindowAside__Struct* WindowAsideTemp = (struct __lrWindowAside__Struct*)realloc(WindowAside, WindowAside_quantity * sizeof(struct __lrWindowAside__Struct));

				if (WindowAsideTemp == NULL) return FALSE;
				else WindowAside = WindowAsideTemp;
			}

			WindowAside[WindowAside_quantity - 1].hwnd = _hwnd;
			WindowAside[WindowAside_quantity - 1].rx = _retainX;
			WindowAside[WindowAside_quantity - 1].ry = _retainY;
			WindowAside[WindowAside_quantity - 1].speed = _speed;
			WindowAside[WindowAside_quantity - 1].closebool = FALSE;

			ReleaseMutex(WindowAside_mutex);

			CreateThread(NULL, 0, lrWindowAsideProc, _hwnd, 0, 0);
		}
		/* 直接覆盖原来设置即可 */
		else
		{
			WindowAside[WinNumber].rx = _retainX;
			WindowAside[WinNumber].ry = _retainY;
			WindowAside[WinNumber].speed = _speed;

			ReleaseMutex(WindowAside_mutex);
		}
	}
	else
	{
		if (WindowAside_quantity)
		{
			/* 确认当前窗口编号 */
			ULONG WinNumber = -1;
			for (ULONG i = 0; i < WindowAside_quantity; ++i) {
				if (WindowAside[i].hwnd == _hwnd) {
					WinNumber = i;
					break;
				}
			}
			if (WinNumber != -1)
				WindowAside[WinNumber].closebool = TRUE;

			else {
				ReleaseMutex(WindowAside_mutex);
				return FALSE;
			}
		}
		else {
			ReleaseMutex(WindowAside_mutex);
			return FALSE;
		}
		ReleaseMutex(WindowAside_mutex);
	}
	return TRUE;
}



BOOL lrWindowAndCursorMoveAfterCollide(HWND _hwnd, 
	lrWINDOW_AND_CURSOR_MOVE_AFTER_COLLIDE_MODE _mode,
	UINT _distance, BOOL _SpringBack, UINT _SpringBackPixel, BOOL _switch)
{
	if (!IsWindow(_hwnd)) return FALSE;

	static WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO** has = NULL;
	static UINT hasq = 0;

	if (_switch)
	{
		BOOL running = FALSE;
		UINT runnumber = 0;

		/* 判断该窗口是否已经在运行中，并获取该编号 */
		for (UINT i = 0; i < hasq; ++i)
		{
			if (_hwnd == has[i]->_hWnd)
			{
				running = TRUE;
				runnumber = i;
			}
		}

		if (running)
		{
			has[runnumber]->_hWnd = _hwnd;
			has[runnumber]->_Switch = TRUE;
			has[runnumber]->_Mode = _mode;
			has[runnumber]->_Distance = _distance;
			has[runnumber]->_SpringBack = _SpringBack;
			has[runnumber]->_SpringBackPixel = _SpringBackPixel;
			return TRUE;
		}
		else /* 窗口不在运行中 */
		{
			if (has)
			{
				++hasq;

				if ((has = (WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO**)realloc(has, hasq * sizeof(WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO*))) == NULL)
				{
					--hasq;
					wdo_error_ex();
					return FALSE;
				}
				if ((has[hasq - 1] = (WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO*)malloc(sizeof(WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO))) == NULL)
				{
					--hasq;
					wdo_error_ex();
					return FALSE;
				}
				has[hasq - 1]->_hWnd = _hwnd;
				has[hasq - 1]->_Switch = TRUE;
				has[hasq - 1]->_Mode = _mode;
				has[hasq - 1]->_Distance = _distance;
				has[hasq - 1]->_SpringBack = _SpringBack;
				has[hasq - 1]->_SpringBackPixel = _SpringBackPixel;
			}
			else
			{
				hasq = 1;

				if ((has = (WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO**)malloc(sizeof(WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO*))) == NULL)
				{
					--hasq;
					wdo_error_ex();
					return FALSE;
				}
				if ((has[0] = (WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO*)malloc(sizeof(WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO))) == NULL)
				{
					--hasq;
					wdo_error_ex();
					return FALSE;
				}
				has[0]->_hWnd = _hwnd;
				has[0]->_Switch = TRUE;
				has[0]->_Mode = _mode;
				has[0]->_Distance = _distance;
				has[0]->_SpringBack = _SpringBack;
				has[0]->_SpringBackPixel = _SpringBackPixel;
			}
		}

		return !!CreateThread(NULL, 0, lrWindowAndCursorMoveAfterCollideProc, has[hasq - 1], 0, 0);
	}
	else    // Close
	{
		for (UINT i = 0; i < hasq; ++i)
		{
			if (has[i]->_hWnd == _hwnd)
			{
				--hasq;
				has[i]->_Switch = FALSE;

				if (hasq == 0)
				{
					free(has);
					has = NULL;
				}
				else
				{
					has[i] = has[hasq];
					has[hasq] = NULL;
				}

				return TRUE;
			}
		}
		return FALSE;
	}
}

static DWORD WINAPI lrWindowDriftProc(DRIFT* _PDRIFT)
{
	wdo_add_thread(GetCurrentThreadId());

	HWND hwnd = _PDRIFT->hwnd;

	for (size_t i = 0; i < _PDRIFT->quantity; ++i)
	{
		DWORD delay = _PDRIFT->frequency[i];

		/* 获取窗口的各个顶点 */
		RECT rect;
		GetWindowRect(hwnd, &rect);

		/* 窗口长和宽 */
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;

		/* 起始坐标 */
		double x0 = (double)rect.left;
		double y0 = (double)rect.top;

		/* 当前到达的坐标 */
		double x1 = x0;
		double y1 = y0;

		/* 终点坐标 */
		double x2 = (double)_PDRIFT->End_X[i];
		double y2 = (double)_PDRIFT->End_Y[i];

		if (x0 == x2 && y0 == y2) continue;

		/* 直线斜率 */
		double k = (x1 != x2) ? ((y1 - y2) / (x1 - x2)) : 0;

		while (x1 != x2 || y1 != y2)
		{
			/* 刷新数值，如果窗口被人为移动 */
			GetWindowRect(hwnd, &rect);
			if (rect.right != x1 || rect.top != y1)
			{
				w = rect.right - rect.left;
				h = rect.bottom - rect.top;

				x0 = (double)rect.left;
				y0 = (double)rect.top;

				x1 = x0;
				y1 = y0;

				if (x1 != x2) k = (y1 - y2) / (x1 - x2);
				else k = 0;
			}

			/*
			* 本算法采用 DDA (Digital Differential Analyzer) 计算
			* 参考：https://www.zhihu.com/question/20330720
			*
			* 从不同的坐标出发，计算量和速度可能差别较大，
			* 暂时只能保证路径正确。
			*/

			if (x0 == x2)
			{
				if (y2 > y0) ++y1;
				else --y1;
			}
			else if (y0 == y2)
			{
				if (x2 > x0) ++x1;
				else --x1;
			}
			else
			{
				if (x0 <= x2)
				{
					++x1;
					y1 += k;
				}
				else
				{
					--x1;
					y1 -= k;
				}
			}

			/* 校准坐标，否则可能陷入死循环 */
			if (x1 == x2) y1 = y2; else if (y1 == y2) x1 = x2;

			MoveWindow(hwnd, (int)x1, (int)y1, w, h, TRUE);

			if (delay) Sleep(delay);
		}
	}

	free(_PDRIFT->End_X);
	free(_PDRIFT->End_Y);
	free(_PDRIFT->frequency);
	free(_PDRIFT);

	wdo_del_thread(GetCurrentThreadId());
	return 0;
}

static DWORD WINAPI lrWindowFollowCursorProc(HWND _hwnd)
{
	wdo_add_thread(GetCurrentThreadId());

	/* 进入临界区 */
	WaitForSingleObject(WindowFollow_mutex, INFINITE);

	/* 窗口编号 */
	ULONG WinNumber = 0;
	/* 搜索当前窗口 */
	for (ULONG i = 0; i < WindowFollow_quantity; ++i) {
		if (WindowFollow[i].hwnd == _hwnd) {
			WinNumber = i;
			break;
		}
	}

	/* 退出临界区 */
	ReleaseMutex(WindowFollow_mutex);

	do
	{
		/* 进入临界区 */
		WaitForSingleObject(WindowFollow_mutex, INFINITE);

		/* 获取当前游标坐标 */
		POINT curpos;
		GetCursorPos(&curpos);

		/* 获取窗口的宽高 */
		RECT rect;
		GetWindowRect(WindowFollow[WinNumber].hwnd, &rect);

		/* 移动窗口至游标偏移处 */
		MoveWindow(
			WindowFollow[WinNumber].hwnd
			, curpos.x + WindowFollow[WinNumber].x
			, curpos.y + WindowFollow[WinNumber].y
			, rect.right - rect.left
			, rect.bottom - rect.top
			, TRUE);

		/* 如果此值为真或窗口无效时，将准备退出该线程 */
		if (WindowFollow[WinNumber].closebool || !IsWindow(_hwnd))
		{
			/* 如果只有一个窗口则释放所有内存并返回 */
			if (WindowFollow_quantity == 1)
			{
				free(WindowFollow);
				WindowFollow_quantity = 0;

				ReleaseMutex(WindowFollow_mutex);
				wdo_del_thread(GetCurrentThreadId());
				return 0;
			}
			/* 否则将最后一个窗口的数据移到此处 */
			else
			{
				--WindowFollow_quantity;

				WindowFollow[WinNumber].hwnd = WindowFollow[WindowFollow_quantity].hwnd;
				WindowFollow[WinNumber].x = WindowFollow[WindowFollow_quantity].x;
				WindowFollow[WinNumber].y = WindowFollow[WindowFollow_quantity].y;
				WindowFollow[WinNumber].closebool = WindowFollow[WindowFollow_quantity].closebool;

				ReleaseMutex(WindowFollow_mutex);
				wdo_del_thread(GetCurrentThreadId());
				return 0;
			}
		}

		/* 退出临界区 */
		ReleaseMutex(WindowFollow_mutex);

		/* 为了减少CPU使用率，每隔一段时间才会移动一次 */
		Sleep(FREQUENCY_MOVE_REFRESH);

	} while (TRUE);

	wdo_del_thread(GetCurrentThreadId());
	return 0;
}

static DWORD WINAPI lrWindowWiggleProc(HWND _hwnd)
{
	wdo_add_thread(GetCurrentThreadId());

	/* 进入临界区 */
	WaitForSingleObject(WindowWiggle_mutex, INFINITE);

	/* 窗口编号 */
	ULONG WinNumber = 0;
	/* 搜索当前窗口 */
	for (ULONG i = 0; i < WindowWiggle_quantity; ++i) {
		if (WindowWiggle[i].hwnd == _hwnd) {
			WinNumber = i;
			break;
		}
	}

	/* 退出临界区 */
	ReleaseMutex(WindowWiggle_mutex);

	/* 中间值 */
	POINT val_mid = { 0,0 };

	/* 现在值 */
	POINT val_now = { 0,0 };

	do
	{
		WaitForSingleObject(WindowWiggle_mutex, INFINITE);

		RECT rect;
		GetWindowRect(WindowWiggle[WinNumber].hwnd, &rect);

		val_mid.x = rect.left;
		val_mid.y = rect.top;

		val_now = val_mid;

		for (int i = 0; i < lrWINDOWWIGGLE_CYCLE; ++i)
		{
			POINT val_new = { rand() % WindowWiggle[WinNumber].range, rand() % WindowWiggle[WinNumber].range };

			if (rand() % 2)
			{
				if ((val_now.x <= val_mid.x + (LONG)WindowWiggle[WinNumber].range)
					&& (val_now.x >= val_mid.x - (LONG)WindowWiggle[WinNumber].range))
					val_now.x += val_new.x;

				if ((val_now.y <= val_mid.y + (LONG)WindowWiggle[WinNumber].range)
					&& (val_now.y >= val_mid.y - (LONG)WindowWiggle[WinNumber].range))
				val_now.y += val_new.y;
			}
			else
			{
				if ((val_now.x <= val_mid.x + (LONG)WindowWiggle[WinNumber].range)
					&& (val_now.x >= val_mid.x - (LONG)WindowWiggle[WinNumber].range))
					val_now.x -= val_new.x;

				if ((val_now.y <= val_mid.y + (LONG)WindowWiggle[WinNumber].range)
					&& (val_now.y >= val_mid.y - (LONG)WindowWiggle[WinNumber].range))
					val_now.y -= val_new.y;
			}
			MoveWindow(WindowWiggle[WinNumber].hwnd, val_now.x, val_now.y, rect.right - rect.left, rect.bottom - rect.top, TRUE);

			Sleep(WindowWiggle[WinNumber].freq);
		}
		MoveWindow(WindowWiggle[WinNumber].hwnd, val_mid.x, val_mid.y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		Sleep(WindowWiggle[WinNumber].freq);

		/* 退出 */
		if (WindowWiggle[WinNumber].closebool)
		{
			--WindowWiggle_quantity;

			if (!WindowWiggle_quantity)
			{
				free(WindowWiggle);
			}

			ReleaseMutex(WindowWiggle_mutex);

			wdo_del_thread(GetCurrentThreadId());
			return 0;
		}

		ReleaseMutex(WindowWiggle_mutex);
	} while (TRUE);

	wdo_del_thread(GetCurrentThreadId());
	return 0;
}

static DWORD WINAPI lrWindowAsideProc(HWND _hwnd)
{
	wdo_add_thread(GetCurrentThreadId());

	/* 进入临界区 */
	WaitForSingleObject(WindowAside_mutex, INFINITE);

	/* 窗口编号 */
	ULONG WinNumber = 0;
	/* 搜索当前窗口 */
	for (ULONG i = 0; i < WindowAside_quantity; ++i) {
		if (WindowAside[i].hwnd == _hwnd) {
			WinNumber = i;
			break;
		}
	}
	/* 退出临界区 */
	ReleaseMutex(WindowAside_mutex);

	/* 为了节省时间，此处直接使用桌面窗口的边框大小 */
	HWND desk = GetDesktopWindow();

	/* 桌面边框 */
	RECT jamb;
	

	do
	{
		WaitForSingleObject(WindowAside_mutex, INFINITE);

		/* 获取桌面边框，可能会改变其值，所以放在循环中 */
		GetWindowRect(desk, &jamb);

		/* 当前游标坐标 */
		POINT curpos;
		GetCursorPos(&curpos);

		/* 当前窗口边框 */
		RECT rect;
		GetWindowRect(_hwnd, &rect);

		/* 窗口的高度和宽度 */
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;

		/* 如果游标离开窗口区域，则判断窗口是否靠边收入 */
		if (!InWindow(_hwnd))
		{
			/* 缩小桌面边框，防止因为游标在窗口上，窗口弹出过大而无法收入 */
			jamb.left += WindowAside[WinNumber].speed;
			jamb.right -= WindowAside[WinNumber].speed;
			jamb.top += WindowAside[WinNumber].speed;
			jamb.bottom -= WindowAside[WinNumber].speed;

			/* 自右向左 收入 */
			if (rect.left <= jamb.left) while (rect.left > (jamb.left + (LONG)WindowAside[WinNumber].rx - w))
			{
				MoveWindow(_hwnd, rect.left -= WindowAside[WinNumber].speed, rect.top, w, h, TRUE);
			}
			/* 自下向上 收入 */
			else if (rect.top <= jamb.top) while (rect.top > (jamb.top + (LONG)WindowAside[WinNumber].ry - h))
			{
				MoveWindow(_hwnd, rect.left, rect.top -= WindowAside[WinNumber].speed, w, h, TRUE);
			}
			/* 自左向右 收入 */
			else if ((rect.left + w) >= jamb.right) while ((rect.left + w) < (jamb.right - (LONG)WindowAside[WinNumber].rx + w))
			{
				MoveWindow(_hwnd, rect.left += WindowAside[WinNumber].speed, rect.top, w, h, TRUE);
			}
			/* 自上向下 收入 */
			else if ((rect.top + h) >= jamb.bottom) while ((rect.top + h) < (jamb.bottom - (LONG)WindowAside[WinNumber].ry + h))
			{
				MoveWindow(_hwnd, rect.left, rect.top += WindowAside[WinNumber].speed, w, h, TRUE);
			}
		}
		/* 如果游标在窗口中，判断窗口是否需要弹出 */
		else
		{
			while (rect.left < jamb.left)
			{
				MoveWindow(_hwnd, rect.left += WindowAside[WinNumber].speed, rect.top, w, h, TRUE);
			}
			while (rect.top < jamb.top)
			{
				MoveWindow(_hwnd, rect.left, rect.top += WindowAside[WinNumber].speed, w, h, TRUE);
			}
			while ((rect.left + w) > jamb.right)
			{
				MoveWindow(_hwnd, rect.left -= WindowAside[WinNumber].speed, rect.top, w, h, TRUE);
			}
			while ((rect.top + h) > jamb.bottom)
			{
				MoveWindow(_hwnd, rect.left, rect.top -= WindowAside[WinNumber].speed, w, h, TRUE);
			}
		}
		/* 如果游标在桌面窗口的边框上，则判断窗口是否需要弹出 */
		if (InWindowJamb(desk, lrWINDOWASIDE_JAMBALW, lrWINDOWASIDE_JAMBALW))
		{
			/* 如果游标 Y 轴与窗口 Y 轴对应 */
			if (curpos.y >= rect.top && curpos.y <= rect.bottom)
			{
				/* 如果游标靠左边，则窗口自左向右弹出 */
				if ((curpos.x >= jamb.left - lrWINDOWASIDE_JAMBALW) && (curpos.x <= jamb.left + lrWINDOWASIDE_JAMBALW))
				{
					/* 弹出窗口时一并将窗口设置为前台窗口 */
					SetForegroundWindow(_hwnd);
					while (rect.left < jamb.left)
					{
						MoveWindow(_hwnd, rect.left += WindowAside[WinNumber].speed, rect.top, w, h, TRUE);
					}
				}
				/* 如果游标靠右边，则窗口自右向左弹出 */
				else if ((curpos.x >= jamb.right - lrWINDOWASIDE_JAMBALW) && (curpos.x <= jamb.right + lrWINDOWASIDE_JAMBALW))
				{
					SetForegroundWindow(_hwnd);
					while ((rect.left + w) > jamb.right)
					{
						MoveWindow(_hwnd, rect.left -= WindowAside[WinNumber].speed, rect.top, w, h, TRUE);
					}
				}
			}
			/* 如果游标 X 轴与窗口 X 轴对应 */
			if (curpos.x >= rect.left && curpos.x <= rect.right)
			{
				/* 如果游标靠上边，则窗口自上向下弹出 */
				if ((curpos.y >= jamb.top - lrWINDOWASIDE_JAMBALW) && (curpos.y <= jamb.top + lrWINDOWASIDE_JAMBALW))
				{
					SetForegroundWindow(_hwnd);
					while (rect.top < jamb.top)
					{
						MoveWindow(_hwnd, rect.left, rect.top += WindowAside[WinNumber].speed, w, h, TRUE);
					}
				}
				/* 如果游标靠下边，则窗口自下向上弹出 */
				else if ((curpos.y >= jamb.bottom - lrWINDOWASIDE_JAMBALW) && curpos.y <= jamb.bottom + lrWINDOWASIDE_JAMBALW)
				{
					SetForegroundWindow(_hwnd);
					while ((rect.top + h) > jamb.bottom)
					{
						MoveWindow(_hwnd, rect.left, rect.top -= WindowAside[WinNumber].speed, w, h, TRUE);
					}
				}
			}
		}
		/* 退出 */
		if (WindowAside[WinNumber].closebool)
		{
			--WindowAside_quantity;
			if (WindowAside_quantity == 0) free(WindowAside);
			ReleaseMutex(WindowAside_mutex);

			wdo_del_thread(GetCurrentThreadId());
			return 0;
		}
		ReleaseMutex(WindowAside_mutex);

		/* 为了节省 CUP */
		Sleep(20);

	} while (TRUE);

	wdo_del_thread(GetCurrentThreadId());
	return 0;
}

static DWORD WINAPI lrWindowAndCursorMoveAfterCollideProc(WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO* _info)
{
	wdo_add_thread(GetCurrentThreadId());

	WINDOWMOTION_WINDOWANDCURSORMODEAFTERCOLLIDEINFO* info = _info;

	/* 延时 */
	const enum { INTERVAL = 20 };

	POINT LastCursorPosition = { 0,0 };
	GetCursorPos(&LastCursorPosition);

	POINT CurrentCursorPosition = { 0,0 };

	RECT CurrentWindowRect = { 0,0,0,0 };

	/* 移动速度，数值越小越快 */
	const enum { MOVE_SPEED = 20 };

	RECT ScreenWindowRect;
	GetWindowRect(GetDesktopWindow(), &ScreenWindowRect);

	/* 缩小屏幕大小以增加回弹像素 */
	ScreenWindowRect.left += _info->_SpringBackPixel;
	ScreenWindowRect.top += _info->_SpringBackPixel;
	ScreenWindowRect.right -= _info->_SpringBackPixel;
	ScreenWindowRect.bottom -= _info->_SpringBackPixel;

	switch (info->_Mode)
	{
	case WINDOWMOTION_WINDOWANDCURSORCOLLIDE_PUSHMOVE:
		while (info->_Switch)
		{
			GetWindowRect(info->_hWnd, &CurrentWindowRect);
			GetCursorPos(&CurrentCursorPosition);

			/* 如果游标在窗口内则移动窗口 */
			if (CurrentCursorPosition.x > CurrentWindowRect.left
				&& CurrentCursorPosition.x < CurrentWindowRect.right
				&& CurrentCursorPosition.y > CurrentWindowRect.top
				&& CurrentCursorPosition.y < CurrentWindowRect.bottom)
			{
				/* 根据最近边框选择推动方向 */
				switch (lrCursorNearWindowJamb(info->_hWnd, TRUE))
				{
				case NEARWINDOWJAMB_LEFT:
					SetWindowPos(info->_hWnd, NULL
						, CurrentCursorPosition.x
						, CurrentWindowRect.top
						, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					break;
				case NEARWINDOWJAMB_RIGHT:
					SetWindowPos(info->_hWnd, NULL
						, CurrentCursorPosition.x - (CurrentWindowRect.right - CurrentWindowRect.left)
						, CurrentWindowRect.top
						, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					break;
				case NEARWINDOWJAMB_TOP:
					SetWindowPos(info->_hWnd, NULL
						, CurrentWindowRect.left
						, CurrentCursorPosition.y
						, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					break;
				case NEARWINDOWJAMB_BOTTOM:
					SetWindowPos(info->_hWnd, NULL
						, CurrentWindowRect.left
						, CurrentCursorPosition.y - (CurrentWindowRect.bottom - CurrentWindowRect.top)
						, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					break;
				}


				LastCursorPosition = CurrentCursorPosition;
				continue;
			}
			/* 游标没有进行推动窗口时，可进行回弹 */
			else if (info->_SpringBack)
			{
				/* 左边越界则左边弹出 */
				if (CurrentWindowRect.left < ScreenWindowRect.left)
				{
					/* 弹出不超过游标坐标 */
					if (CurrentCursorPosition.x < (CurrentWindowRect.right - CurrentWindowRect.left) + ScreenWindowRect.left)
					{
						SetWindowPos(info->_hWnd, NULL, CurrentCursorPosition.x - (CurrentWindowRect.right - CurrentWindowRect.left), CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, ScreenWindowRect.left, CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
				/* 上边越界则上边弹出
				* 不要使用 else if，因为可能同时越界
				*/
				if (CurrentWindowRect.top < ScreenWindowRect.top)
				{
					if (CurrentCursorPosition.y < (CurrentWindowRect.bottom - CurrentWindowRect.top) + ScreenWindowRect.top)
					{
						SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, CurrentCursorPosition.y - (CurrentWindowRect.bottom - CurrentWindowRect.top), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, ScreenWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
				/* 右边越界则右边弹出 */
				if (CurrentWindowRect.right > ScreenWindowRect.right)
				{
					if (CurrentCursorPosition.x > ScreenWindowRect.right - (CurrentWindowRect.right - CurrentWindowRect.left))
					{
						SetWindowPos(info->_hWnd, NULL, CurrentCursorPosition.x, CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, ScreenWindowRect.right - (CurrentWindowRect.right - CurrentWindowRect.left), CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
				/* 下边越界则下边弹出 */
				if (CurrentWindowRect.bottom > ScreenWindowRect.bottom)
				{
					if (CurrentCursorPosition.y > ScreenWindowRect.bottom - (CurrentWindowRect.bottom - CurrentWindowRect.top))
					{
						SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, CurrentCursorPosition.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, ScreenWindowRect.bottom - (CurrentWindowRect.bottom - CurrentWindowRect.top), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
			}

			LastCursorPosition = CurrentCursorPosition;
			Sleep(INTERVAL);
		}
		break;
	case WINDOWMOTION_WINDOWANDCURSORCOLLIDE_MOVEWINDOW:
		while (info->_Switch)
		{
			GetWindowRect(info->_hWnd, &CurrentWindowRect);
			GetCursorPos(&CurrentCursorPosition);

			if (CurrentCursorPosition.x > CurrentWindowRect.left
				&& CurrentCursorPosition.x < CurrentWindowRect.right
				&& CurrentCursorPosition.y > CurrentWindowRect.top
				&& CurrentCursorPosition.y < CurrentWindowRect.bottom)
			{
				switch (lrCursorNearWindowJamb(info->_hWnd, TRUE))
				{
				case NEARWINDOWJAMB_LEFT:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetWindowPos(info->_hWnd, NULL
							, CurrentWindowRect.left + i
							, CurrentWindowRect.top
							, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					break;
				case NEARWINDOWJAMB_RIGHT:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetWindowPos(info->_hWnd, NULL
							, CurrentWindowRect.left - i
							, CurrentWindowRect.top
							, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					break;
				case NEARWINDOWJAMB_TOP:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetWindowPos(info->_hWnd, NULL
							, CurrentWindowRect.left
							, CurrentWindowRect.top + i
							, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					break;
				case NEARWINDOWJAMB_BOTTOM:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetWindowPos(info->_hWnd, NULL
							, CurrentWindowRect.left
							, CurrentWindowRect.top - i
							, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					break;
				}
			}
			else if (info->_SpringBack)
			{
				/* 左边越界则左边弹出 */
				if (CurrentWindowRect.left < ScreenWindowRect.left)
				{
					/* 弹出不超过游标坐标 */
					if (CurrentCursorPosition.x < (CurrentWindowRect.right - CurrentWindowRect.left) + ScreenWindowRect.left)
					{
						SetWindowPos(info->_hWnd, NULL, CurrentCursorPosition.x - (CurrentWindowRect.right - CurrentWindowRect.left), CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, ScreenWindowRect.left, CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
				/* 上边越界则上边弹出
				* 不要使用 else if，因为可能同时越界
				*/
				if (CurrentWindowRect.top < ScreenWindowRect.top)
				{
					if (CurrentCursorPosition.y < (CurrentWindowRect.bottom - CurrentWindowRect.top) + ScreenWindowRect.top)
					{
						SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, CurrentCursorPosition.y - (CurrentWindowRect.bottom - CurrentWindowRect.top), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, ScreenWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
				/* 右边越界则右边弹出 */
				if (CurrentWindowRect.right > ScreenWindowRect.right)
				{
					if (CurrentCursorPosition.x > ScreenWindowRect.right - (CurrentWindowRect.right - CurrentWindowRect.left))
					{
						SetWindowPos(info->_hWnd, NULL, CurrentCursorPosition.x, CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, ScreenWindowRect.right - (CurrentWindowRect.right - CurrentWindowRect.left), CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
				/* 下边越界则下边弹出 */
				if (CurrentWindowRect.bottom > ScreenWindowRect.bottom)
				{
					if (CurrentCursorPosition.y > ScreenWindowRect.bottom - (CurrentWindowRect.bottom - CurrentWindowRect.top))
					{
						SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, CurrentCursorPosition.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, ScreenWindowRect.bottom - (CurrentWindowRect.bottom - CurrentWindowRect.top), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
			}

			LastCursorPosition = CurrentCursorPosition;
			Sleep(INTERVAL);
		}
		break;
	case WINDOWMOTION_WINDOWANDCURSORCOLLIDE_MOVECURSOR:
		while (info->_Switch)
		{
			GetWindowRect(info->_hWnd, &CurrentWindowRect);
			GetCursorPos(&CurrentCursorPosition);

			if (CurrentCursorPosition.x > CurrentWindowRect.left
				&& CurrentCursorPosition.x < CurrentWindowRect.right
				&& CurrentCursorPosition.y > CurrentWindowRect.top
				&& CurrentCursorPosition.y < CurrentWindowRect.bottom)
			{
				switch (lrCursorNearWindowJamb(info->_hWnd, TRUE))
				{
				case NEARWINDOWJAMB_LEFT:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetCursorPos(CurrentCursorPosition.x - i, CurrentCursorPosition.y);
					}
					break;
				case NEARWINDOWJAMB_RIGHT:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetCursorPos(CurrentCursorPosition.x + i, CurrentCursorPosition.y);
					}
					break;
				case NEARWINDOWJAMB_TOP:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetCursorPos(CurrentCursorPosition.x, CurrentCursorPosition.y - i);
					}
					break;
				case NEARWINDOWJAMB_BOTTOM:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetCursorPos(CurrentCursorPosition.x, CurrentCursorPosition.y + i);
					}
					break;
				}
			}

			LastCursorPosition = CurrentCursorPosition;
			Sleep(INTERVAL);
		}
		break;
	case WINDOWMOTION_WINDOWANDCURSORCOLLIDE_REVERSEMOVE:
		while (info->_Switch)
		{
			GetWindowRect(info->_hWnd, &CurrentWindowRect);
			GetCursorPos(&CurrentCursorPosition);

			if (CurrentCursorPosition.x > CurrentWindowRect.left
				&& CurrentCursorPosition.x < CurrentWindowRect.right
				&& CurrentCursorPosition.y > CurrentWindowRect.top
				&& CurrentCursorPosition.y < CurrentWindowRect.bottom)
			{
				switch (lrCursorNearWindowJamb(info->_hWnd, TRUE))
				{
				case NEARWINDOWJAMB_LEFT:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetWindowPos(info->_hWnd, NULL
							, CurrentWindowRect.left + i
							, CurrentWindowRect.top
							, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
						SetCursorPos(CurrentCursorPosition.x - i, CurrentCursorPosition.y);
					}
					break;
				case NEARWINDOWJAMB_RIGHT:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetWindowPos(info->_hWnd, NULL
							, CurrentWindowRect.left - i
							, CurrentWindowRect.top
							, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
						SetCursorPos(CurrentCursorPosition.x + i, CurrentCursorPosition.y);
					}
					break;
				case NEARWINDOWJAMB_TOP:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetWindowPos(info->_hWnd, NULL
							, CurrentWindowRect.left
							, CurrentWindowRect.top + i
							, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
						SetCursorPos(CurrentCursorPosition.x, CurrentCursorPosition.y - i);
					}
					break;
				case NEARWINDOWJAMB_BOTTOM:
					for (UINT i = 1; i < info->_Distance + 1; i += info->_Distance / MOVE_SPEED)
					{
						SetWindowPos(info->_hWnd, NULL
							, CurrentWindowRect.left
							, CurrentWindowRect.top - i
							, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
						SetCursorPos(CurrentCursorPosition.x, CurrentCursorPosition.y + i);
					}
					break;
				}
				Sleep(INTERVAL);
			}
			else if (info->_SpringBack)
			{
				/* 左边越界则左边弹出 */
				if (CurrentWindowRect.left < ScreenWindowRect.left)
				{
					/* 弹出不超过游标坐标 */
					if (CurrentCursorPosition.x < (CurrentWindowRect.right - CurrentWindowRect.left) + ScreenWindowRect.left)
					{
						SetWindowPos(info->_hWnd, NULL, CurrentCursorPosition.x - (CurrentWindowRect.right - CurrentWindowRect.left), CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, ScreenWindowRect.left, CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
				/* 上边越界则上边弹出
				* 不要使用 else if，因为可能同时越界
				*/
				if (CurrentWindowRect.top < ScreenWindowRect.top)
				{
					if (CurrentCursorPosition.y < (CurrentWindowRect.bottom - CurrentWindowRect.top) + ScreenWindowRect.top)
					{
						SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, CurrentCursorPosition.y - (CurrentWindowRect.bottom - CurrentWindowRect.top), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, ScreenWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
				/* 右边越界则右边弹出 */
				if (CurrentWindowRect.right > ScreenWindowRect.right)
				{
					if (CurrentCursorPosition.x > ScreenWindowRect.right - (CurrentWindowRect.right - CurrentWindowRect.left))
					{
						SetWindowPos(info->_hWnd, NULL, CurrentCursorPosition.x, CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, ScreenWindowRect.right - (CurrentWindowRect.right - CurrentWindowRect.left), CurrentWindowRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
				/* 下边越界则下边弹出 */
				if (CurrentWindowRect.bottom > ScreenWindowRect.bottom)
				{
					if (CurrentCursorPosition.y > ScreenWindowRect.bottom - (CurrentWindowRect.bottom - CurrentWindowRect.top))
					{
						SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, CurrentCursorPosition.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
					}
					else SetWindowPos(info->_hWnd, NULL, CurrentWindowRect.left, ScreenWindowRect.bottom - (CurrentWindowRect.bottom - CurrentWindowRect.top), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
			}
		}
		break;
	}
	wdo_del_thread(GetCurrentThreadId());

	free(info);
	return 0;
}