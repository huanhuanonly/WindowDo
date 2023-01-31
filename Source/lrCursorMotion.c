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


/* ʹ�α����Ư���˶���
* ����ȴ�Ư����ɣ�һ����ʼƯ�ƣ����ǵ���Ŀ�ĵػ������������򲻿���ֹ��
* 
* ����:
*		_x          |  �յ�����
*		_y          |  �յ�����
*		_speed      |  �ƶ��ٶ�[1-100]
* 
* ����ֵ:
*		FALSE       |  ����ʧ�ܡ��ڴ����ʧ�ܡ�
*		TRUE        |  �����ɹ���
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



/* ʹ�α���ж������Ư���˶���
* ����ȴ�Ư����ɣ�һ����ʼƯ�ƣ����ǵ�������Ŀ�ĵػ������������򲻿���ֹ��
*
* ����:
*		_quantity   |  �����˶��Ĵ�����ͬ��Ҳ������Ĵ�С
*		_x          |  �յ���������飬��С��ӦС�� _quantity
*		_y          |  �յ���������飬��С��ӦС�� _quantity
*		_speed      |  �ƶ��ٶȵ����飬��С��ӦС�� _quantity
*
* ����ֵ:
*		FALSE       |  ����ʧ�ܡ��ڴ����ʧ�ܻ� _quantity Ϊ 0��
*		TRUE        |  �����ɹ���
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


/* ʹ�α���аڶ���
* 
* ����:
*		_range      |  �ڶ��ķ�Χ
*		_freq       |  �ڶ���Ƶ��
*		_switch     |  TRUE: ��ʼ�ڶ���FALSE: �����ڶ�
* 
* ����ֵ:
*		FALSE: ����ʧ�ܡ�
*		TRUE:  �����ɹ���
*/
BOOL lrCursorWiggle(UINT _range, UINT _freq, BOOL _switch)
{
	lr_srand();

	/* ��ʼ�� */
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
	/* �ر��� */
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

		/* ��ȡ���ڵĸ������� */
		POINT curpos;
		GetCursorPos(&curpos);


		/* ��ʼ���� */
		double x0 = (double)curpos.x;
		double y0 = (double)curpos.y;

		/* ��ǰ��������� */
		double x1 = x0;
		double y1 = y0;

		/* �յ����� */
		double x2 = (double)_PCURDRIFT->x[i];
		double y2 = (double)_PCURDRIFT->y[i];

		if (x0 == x2 && y0 == y2) continue;

		/* ֱ��б�� */
		double k = (x1 != x2) ? ((y1 - y2) / (x1 - x2)) : 0;

		while (x1 != x2 || y1 != y2)
		{
			/* ˢ����ֵ������α걻��Ϊ�ƶ� */
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
			* ���㷨���� DDA (Digital Differential Analyzer) ����
			* �ο���https://www.zhihu.com/question/20330720
			*
			* �Ӳ�ͬ��������������������ٶȿ��ܲ��ϴ�
			* ��ʱֻ�ܱ�֤·����ȷ��
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

			/* У׼���꣬�������������ѭ�� */
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
	/* �м�ֵ */
	POINT val_mid = { 0,0 };

	/* ����ֵ */
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

		/* �˳� */
		if (CursorWiggle_closebool)
		{
			ReleaseMutex(CursorWiggle_mutex);
			return 0;
		}

		ReleaseMutex(CursorWiggle_mutex);
	} while (TRUE);

	return 0;
}