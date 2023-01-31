// wdo_thread.c -- Multi thread of lrWindowDo()
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lr_wdo.h"
#include "../Head/lrWindowDo.h"
#include "../Head/wdo_instructions.h"

typedef struct IQ
{
	CHAR** InsSets;			// ָ���
	size_t InsQuan;			// ָ������
}IQ, * PIQ;

typedef struct ATR
{
	/* ����ʱ�� */
	UINT t_hour;
	UINT t_minute;
	UINT t_second;

	IQ iq;
}ATR, * PATR;


static DWORD WINAPI wdo_thread_run_proc(PIQ _PIQ);
static DWORD WINAPI wdo_thread_timing_proc(PATR _PATR);


/* ����һ���߳�ִ�� lrWindowDo() ��ָ�
* 
* ����:
*		_InsSets      |  ָ���
*		_InsQuan      |  ָ������
* 
* ����ֵ:
*		�������߳̾����
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


/* ����һ���̶߳�ʱִ�� lrWindowDo() ��ָ�
* ���ʱ���ѹ���ڶ������ʱ��ִ�С�
* 
* ����:
*		_InsSets      |  ָ���
*		_InsQuan      |  ָ������
*		_hour         |  ����ʱ��[0-23]
*		_minute       |  ����ʱ��[0-59]
*		_second       |  ����ʱ��[0-59]
*
* ����ֵ:
*		�������߳̾����
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
	wdo_log(WDOINS_THREAD, OUTLOG_NORMAL, wdo_language("The thread %X exited...\n", "�߳� %X ���˳�������\n"), GetCurrentThreadId());

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

	/* ����ִ��ָ�� */
	for (size_t i = 0; i < patr->iq.InsQuan; ++i)
	{
		wdo_log(WDOINS_QUEUE, OUTLOG_NORMAL, NULL);
		lrWindowDo(patr->iq.InsSets[i], WDOECHO_DEF);
	}

	/* �ͷ��ڴ� */
	lrfrees(&patr->iq.InsSets, patr->iq.InsQuan);
	free(patr);

	wdo_del_thread(GetCurrentThreadId());

	return 0;
}