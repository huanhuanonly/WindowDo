// wdo_thread.c -- Multi thread of lrWindowDo()
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lr_wdo.h"
#include "../Head/lrWindowDo.h"
#include "../Head/wdo_instructions.h"

typedef struct IQ
{
	CHAR** InsSets;			// 指令集合
	size_t InsQuan;			// 指令数量
}IQ, * PIQ;

typedef struct ATR
{
	/* 运行时间 */
	UINT t_hour;
	UINT t_minute;
	UINT t_second;

	IQ iq;
}ATR, * PATR;


static DWORD WINAPI wdo_thread_run_proc(PIQ _PIQ);
static DWORD WINAPI wdo_thread_timing_proc(PATR _PATR);


/* 另起一条线程执行 lrWindowDo() 的指令。
* 
* 参数:
*		_InsSets      |  指令集合
*		_InsQuan      |  指令数量
* 
* 返回值:
*		创建的线程句柄。
*/
HANDLE wdo_thread_run(const char** const _InsSets, size_t _InsQuan)
{
	if (_InsSets == NULL || _InsQuan == 0) return NULL;

	PIQ piq = (PIQ)malloc(sizeof(IQ));
	if (piq == NULL) return NULL;

	piq->InsQuan = _InsQuan;

	piq->InsSets = (char**)malloc(_InsQuan * sizeof(char*));
	if (piq->InsSets == NULL) {
		free(piq);
		return NULL;
	}

	for (size_t i = 0; i < _InsQuan; ++i)
	{
		piq->InsSets[i] = (char*)malloc(strlen(_InsSets[i]) + 1);

		if (piq->InsSets[i] == NULL) {
			free(piq);
			for (size_t j = 0; j < i; ++j) free(piq->InsSets[j]);
			return NULL;
		}

		strcpy(piq->InsSets[i], _InsSets[i]);
	}
	return CreateThread(NULL, 0, wdo_thread_run_proc, piq, 0, 0);
}


/* 另起一条线程定时执行 lrWindowDo() 的指令。
* 如果时间已过则第二天这个时间执行。
* 
* 参数:
*		_InsSets      |  指令集合
*		_InsQuan      |  指令数量
*		_hour         |  运行时间[0-23]
*		_minute       |  运行时间[0-59]
*		_second       |  运行时间[0-59]
*
* 返回值:
*		创建的线程句柄。
*/
HANDLE wdo_thread_timing(const char** const _InsSets, size_t _InsQuan, UINT _hour, UINT _minute, UINT _second)
{
	if (_InsSets == NULL || _InsQuan == 0) return NULL;

	PATR patr = (PATR)malloc(sizeof(ATR));
	if (patr == NULL) return NULL;

	char** ins = (char**)malloc(_InsQuan * sizeof(char*));
	if (ins == NULL) {
		free(patr);
		return NULL;
	}

	for (size_t i = 0; i < _InsQuan; ++i)
	{
		ins[i] = (char*)malloc(strlen(_InsSets[i]) + 1);

		if (ins[i] == NULL) {
			free(patr);
			for (size_t j = 0; j < i; ++j) free(ins[j]);
			return NULL;
		}

		strcpy(ins[i], _InsSets[i]);
	}

	patr->iq.InsSets = ins;
	patr->iq.InsQuan = _InsQuan;
	patr->t_hour = _hour;
	patr->t_minute = _minute;
	patr->t_second = _second;

	return CreateThread(NULL, 0, wdo_thread_timing_proc, patr, 0, 0);
}


static DWORD WINAPI wdo_thread_run_proc(PIQ _PIQ)
{
	wdo_add_thread(GetCurrentThreadId());

	for (size_t i = 0; i < _PIQ->InsQuan; ++i)
	{
		lrWindowDo(_PIQ->InsSets[i], WDOECHO_FALSE);
	}
	wdo_log(WDOINS_THREAD, OUTLOG_NORMAL, wdo_language("The thread %X exited...\n", "线程 %X 已退出。。。\n"), GetCurrentThreadId());

	lrfrees(&_PIQ->InsSets, _PIQ->InsQuan);

	wdo_del_thread(GetCurrentThreadId());

	return 0;
}

static DWORD WINAPI wdo_thread_timing_proc(PATR _PATR)
{
	wdo_add_thread(GetCurrentThreadId());

	PATR patr = _PATR;

	if (patr->t_second > 59) patr->t_second = 59;
	if (patr->t_minute > 59) patr->t_minute = 59;
	if (patr->t_hour > 23) patr->t_hour = 23;


	time_t RunTimeLong = 0;
	time_t NowTimeLong = time(NULL);

	struct tm* RunTime = gmtime(&NowTimeLong);
	RunTime->tm_hour = patr->t_hour;
	RunTime->tm_min = patr->t_minute;
	RunTime->tm_sec = patr->t_second;

	RunTimeLong = mktime(RunTime);

	if (time(NULL) > RunTimeLong) RunTimeLong += (time_t)60 * 60 * 24;

	while (TRUE) if (time(NULL) >= RunTimeLong) break; else Sleep(1000 / 3);

	wdo_putc('\n');

	/* 依次执行指令 */
	for (size_t i = 0; i < patr->iq.InsQuan; ++i)
	{
		wdo_log(WDOINS_QUEUE, OUTLOG_NORMAL, NULL);
		lrWindowDo(patr->iq.InsSets[i], WDOECHO_DEF);
	}

	/* 释放内存 */
	lrfrees(&patr->iq.InsSets, patr->iq.InsQuan);
	free(patr);

	wdo_del_thread(GetCurrentThreadId());

	return 0;
}