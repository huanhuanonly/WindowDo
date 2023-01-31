// lrMusic.c -- Music control
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include <Windows.h>
#include <mciapi.h>
#include <stdio.h>

#pragma warning (disable : 4996)

// #pragma comment (lib, "Winmm.lib")

MCIERROR lrPlayMusic(LPCSTR lpPath, BOOL bLoop, BOOL bWait)
{
	MCIERROR mciError = 0;

	char Command[MAX_PATH + 100];

	sprintf(Command, "open \"%s\"", lpPath);
	if (mciError = mciSendStringA(Command, NULL, 0, NULL))
	{
		return mciError;
	}

	if (bWait)
	{
		if (bLoop)
		{
			sprintf(Command, "play \"%s\" repeat wait", lpPath);
		}
		else sprintf(Command, "play \"%s\" wait", lpPath);

		if (mciError = mciSendStringA(Command, NULL, 0, NULL))
		{
			return mciError;
		}

		sprintf(Command, "close \"%s\"", lpPath);
		if (mciError = mciSendStringA(Command, NULL, 0, NULL))
		{
			return mciError;
		}
	}
	else
	{
		if (bLoop)
		{
			sprintf(Command, "play \"%s\" repeat", lpPath);
		}
		else sprintf(Command, "play \"%s\"", lpPath);

		if (mciError = mciSendStringA(Command, NULL, 0, NULL))
		{
			return mciError;
		}

	//	CreateThread(NULL, 0, lrMusicWaitingCloseThread, (LPVOID)bLoop, 0, 0);
	}
	return 0;
}

MCIERROR lrCloseMusic(LPCSTR lpPath)
{
	char Command[MAX_PATH + 100];
	sprintf(Command, "close \"%s\"", lpPath);

	return mciSendStringA(Command, NULL, 0, NULL);
}

MCIERROR lrPauseMusic(LPCSTR lpPath)
{
	char Command[MAX_PATH + 100];
	sprintf(Command, "pause \"%s\"", lpPath);

	return mciSendStringA(Command, NULL, 0, NULL);
}

MCIERROR lrContinueMusic(LPCSTR lpPath)
{
	char Command[MAX_PATH + 100];
	sprintf(Command, "resume \"%s\"", lpPath);

	return mciSendStringA(Command, NULL, 0, NULL);
}


//DWORD WINAPI lrMusicWaitingCloseThread(LPVOID bLoop)
//{
//	return 0;
//}