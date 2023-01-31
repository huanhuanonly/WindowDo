// lrCursorMotion.c -- Cursor control
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lrWindowDo.h"


typedef struct __CURSOR_DRIFT
{
	int* x;
	int* y;
	DWORD* frequency;
	size_t quantity;

}CURDRIFT, * PCURDRIFT;


static DWORD WINAPI lrCursorDriftProc(PCURDRIFT _PCURDRIFT);
static DWORD WINAPI lrCursorWiggleProc(PVOID _unused);


/* 使游标进行漂移运动。
* 不会等待漂移完成，一旦开始漂移，除非到达目的地或程序结束，否则不可终止。
* 
* 参数:
*		_x          |  终点坐标
*		_y          |  终点坐标
*		_speed      |  移动速度[1-100]
* 
* 返回值:
*		FALSE       |  操作失败。内存分配失败。
*		TRUE        |  操作成功。
*/
BOOL lrCursorDrift(int _x, int _y, BYTE _speed)
{
	CURDRIFT* CurDrift = (PCURDRIFT)malloc(sizeof(CURDRIFT));
	if (CurDrift == NULL) return FALSE;


	CurDrift->quantity = 1;

	CurDrift->x = (int*)malloc(sizeof(int));
	CurDrift->y = (int*)malloc(sizeof(int));
	CurDrift->frequency = (DWORD*)malloc(sizeof(DWORD));

	if (CurDrift->x == NULL
		|| CurDrift->y == NULL
		|| CurDrift->frequency == NULL)
		return FALSE;

	CurDrift->x[0] = (int)_x;
	CurDrift->y[0] = (int)_y;
	CurDrift->frequency[0] = (DWORD)(100 - lr_LIMIT_VALSIZE(_speed, 1, 100));

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lrCursorDriftProc, CurDrift, 0, 0);

	return TRUE;
}



/* 使游标进行多次连续漂移运动。
* 不会等待漂移完成，一旦开始漂移，除非到达所有目的地或程序结束，否则不可终止。
*
* 参数:
*		_quantity   |  连续运动的次数，同样也是数组的大小
*		_x          |  终点坐标的数组，大小不应小于 _quantity
*		_y          |  终点坐标的数组，大小不应小于 _quantity
*		_speed      |  移动速度的数组，大小不应小于 _quantity
*
* 返回值:
*		FALSE       |  操作失败。内存分配失败或 _quantity 为 0。
*		TRUE        |  操作成功。
*/
BOOL lrCursorDrifts(size_t _quantity, int* _x, int* _y, BYTE _speed[])
{
	if (!_quantity) return FALSE;

	CURDRIFT* CurDrift = (PCURDRIFT)malloc(_quantity * sizeof(CURDRIFT));
	if (CurDrift == NULL) return FALSE;

	CurDrift->quantity = _quantity;

	CurDrift->x = (int*)malloc(_quantity * sizeof(int));
	CurDrift->y = (int*)malloc(_quantity * sizeof(int));
	CurDrift->frequency = (DWORD*)malloc(_quantity * sizeof(DWORD));

	if (CurDrift->x == NULL
		|| CurDrift->y == NULL
		|| CurDrift->frequency == NULL)
		return FALSE;

	for (size_t i = 0; i < _quantity; ++i)
	{
		CurDrift->x[i] = _x[i];
		CurDrift->y[i] = _y[i];
		CurDrift->frequency[i] = (DWORD)(100 - lr_LIMIT_VALSIZE(_speed[i], 1, 100));
	}

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lrCursorDriftProc, CurDrift, 0, 0);

	return TRUE;
}



static volatile UINT CursorWiggle_range = 0;
static volatile UINT CursorWiggle_freq = 0;
static volatile BOOL CursorWiggle_closebool = TRUE;
static volatile HANDLE CursorWiggle_mutex = 0;


/* 使游标进行摆动。
* 
* 参数:
*		_range      |  摆动的范围
*		_freq       |  摆动的频率
*		_switch     |  TRUE: 开始摆动，FALSE: 结束摆动
* 
* 返回值:
*		FALSE: 操作失败。
*		TRUE:  操作成功。
*/
BOOL lrCursorWiggle(UINT _range, UINT _freq, BOOL _switch)
{
	lr_srand();

	/* 开始震动 */
	if (_switch)
	{
		if (!CursorWiggle_mutex) CursorWiggle_mutex = CreateMutexA(NULL, FALSE, NULL);
		if (!CursorWiggle_mutex) return FALSE;

		WaitForSingleObject(CursorWiggle_mutex, INFINITE);

		CursorWiggle_range = _range;
		CursorWiggle_freq = _freq;

		if (CursorWiggle_closebool)
		{
			CursorWiggle_closebool = FALSE;
			CreateThread(NULL, 0, lrCursorWiggleProc, NULL, 0, 0);
		}

		ReleaseMutex(CursorWiggle_mutex);
	}
	/* 关闭震动 */
	else
	{
		if (CursorWiggle_mutex)
		{
			WaitForSingleObject(CursorWiggle_mutex, INFINITE);

			if (CursorWiggle_closebool == FALSE)
			{
				CursorWiggle_closebool = TRUE;
			}
			else
			{
				ReleaseMutex(CursorWiggle_mutex);
				return FALSE;
			}
			ReleaseMutex(CursorWiggle_mutex);
		}
		else return FALSE;
	}
	return TRUE;
}


static DWORD WINAPI lrCursorDriftProc(PCURDRIFT _PCURDRIFT)
{
	for (size_t i = 0; i < _PCURDRIFT->quantity; ++i)
	{
		DWORD delay = _PCURDRIFT->frequency[i];

		/* 获取窗口的各个顶点 */
		POINT curpos;
		GetCursorPos(&curpos);


		/* 起始坐标 */
		double x0 = (double)curpos.x;
		double y0 = (double)curpos.y;

		/* 当前到达的坐标 */
		double x1 = x0;
		double y1 = y0;

		/* 终点坐标 */
		double x2 = (double)_PCURDRIFT->x[i];
		double y2 = (double)_PCURDRIFT->y[i];

		if (x0 == x2 && y0 == y2) continue;

		/* 直线斜率 */
		double k = (x1 != x2) ? ((y1 - y2) / (x1 - x2)) : 0;

		while (x1 != x2 || y1 != y2)
		{
			/* 刷新数值，如果游标被人为移动 */
			GetCursorPos(&curpos);
			if (curpos.x != x1 || curpos.y != y1)
			{
				x0 = (double)curpos.x;
				y0 = (double)curpos.y;

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

			SetCursorPos((int)x1, (int)y1);

			if (delay) Sleep(delay);
		}
	}

	free(_PCURDRIFT->x);
	free(_PCURDRIFT->y);
	free(_PCURDRIFT->frequency);
	free(_PCURDRIFT);

	return 0;
}

static DWORD WINAPI lrCursorWiggleProc(PVOID _unused)
{
	/* 中间值 */
	POINT val_mid = { 0,0 };

	/* 现在值 */
	POINT val_now = { 0,0 };

	do
	{
		WaitForSingleObject(CursorWiggle_mutex, INFINITE);

		POINT curpos;
		GetCursorPos(&curpos);

		val_mid.x = curpos.x;
		val_mid.y = curpos.y;

		val_now = val_mid;

		for (int i = 0; i < lrCURSORWIGGLE_CYCLE; ++i)
		{
			POINT val_new = { rand() % CursorWiggle_range, rand() % CursorWiggle_range };

			if (rand() % 2)
			{
				if ((val_now.x <= val_mid.x + (LONG)CursorWiggle_range)
					&& (val_now.x >= val_mid.x - (LONG)CursorWiggle_range))
					val_now.x += val_new.x;

				if ((val_now.y <= val_mid.y + (LONG)CursorWiggle_range)
					&& (val_now.y >= val_mid.y - (LONG)CursorWiggle_range))
					val_now.y += val_new.y;
			}
			else
			{
				if ((val_now.x <= val_mid.x + (LONG)CursorWiggle_range)
					&& (val_now.x >= val_mid.x - (LONG)CursorWiggle_range))
					val_now.x -= val_new.x;

				if ((val_now.y <= val_mid.y + (LONG)CursorWiggle_range)
					&& (val_now.y >= val_mid.y - (LONG)CursorWiggle_range))
					val_now.y -= val_new.y;
			}
			SetCursorPos(val_now.x, val_now.y);

			Sleep(CursorWiggle_freq);
		}
		SetCursorPos(val_now.x, val_now.y);
		Sleep(CursorWiggle_freq);

		/* 退出 */
		if (CursorWiggle_closebool)
		{
			ReleaseMutex(CursorWiggle_mutex);
			return 0;
		}

		ReleaseMutex(CursorWiggle_mutex);
	} while (TRUE);

	return 0;
}