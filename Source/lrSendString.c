// lrSendString.c -- Send string message to window
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lrWindowDo.h"

#pragma warning (disable : 6385)


BOOL lrSendWordA(HWND hWnd, short nWord)
{
	wchar_t newWord = L'\0';

	if ((nWord & 0x8080) == 0x8080)
	{
		char wWord[3] = { (nWord >> 8) & 0x00ff, nWord & 0x00ff, '\0' };
		mbtowc(&newWord, wWord, MB_CUR_MAX);
	}
	else mbtowc(&newWord, &((char)nWord), MB_CUR_MAX);

	if (newWord == L'\n' || newWord == L'\r')
	{
		SendMessageA(hWnd, WM_KEYDOWN, VK_RETURN, 0L);
		SendMessageA(hWnd, WM_CHAR, '\r', 0L);
		SendMessageA(hWnd, WM_CHAR, '\n', 0L);
		return !SendMessageA(hWnd, WM_KEYUP, VK_RETURN, 0L);
	}
	return !SendMessageW(hWnd, WM_CHAR, newWord, 0);
}
BOOL lrSendWordW(HWND hWnd, wchar_t wcWord)
{
	if (wcWord == L'\n' || wcWord == L'\r')
	{
		SendMessageA(hWnd, WM_KEYDOWN, VK_RETURN, 0L);
		SendMessageA(hWnd, WM_CHAR, '\r', 0L);
		SendMessageA(hWnd, WM_CHAR, '\n', 0L);
		return !SendMessageA(hWnd, WM_KEYUP, VK_RETURN, 0L);
	}
	else return !SendMessageW(hWnd, WM_CHAR, wcWord, 0);
}

BOOL lrSendStringA(HWND hWnd, const char* const lpText)
{
	if (IsWindow(hWnd) && lpText && lpText[0])
	{
		wchar_t* newText = (wchar_t*)malloc((strlen(lpText) + 1) * sizeof(wchar_t));

		if (newText != NULL)
		{
			size_t wcsize = mbstowcs(newText, lpText, strlen(lpText) + 1);

			for (size_t i = 0; i < wcsize; ++i)
			{
				if (newText[i] == L'\r')
				{
					SendMessageA(hWnd, WM_KEYDOWN, VK_RETURN, 0L);
					SendMessageA(hWnd, WM_CHAR, '\r', 0L);
					SendMessageA(hWnd, WM_CHAR, '\n', 0L);
					SendMessageA(hWnd, WM_KEYUP, VK_RETURN, 0L);
				}
				else if (newText[i] == L'\n' && i && newText[i - 1] != L'\r')
				{
					SendMessageA(hWnd, WM_KEYDOWN, VK_RETURN, 0L);
					SendMessageA(hWnd, WM_CHAR, '\r', 0L);
					SendMessageA(hWnd, WM_CHAR, '\n', 0L);
					SendMessageA(hWnd, WM_KEYUP, VK_RETURN, 0L);
				}
				else SendMessageW(hWnd, WM_CHAR, newText[i], 0);
			}
			free(newText);
		}
		else return FALSE;
	}
	else return FALSE;

	return TRUE;
}


BOOL lrSendStringW(HWND hWnd, const wchar_t* const lpwText)
{
	if (IsWindow(hWnd))
	{
		size_t len = wcslen(lpwText);
		for (size_t i = 0; i < len; ++i)
		{
			if (lpwText[i] == L'\r')
			{
				SendMessageA(hWnd, WM_KEYDOWN, VK_RETURN, 0L);
				SendMessageA(hWnd, WM_CHAR, '\r', 0L);
				SendMessageA(hWnd, WM_CHAR, '\n', 0L);
				SendMessageA(hWnd, WM_KEYUP, VK_RETURN, 0L);
			}
			else if (lpwText[i] == L'\n' && i && lpwText[i - 1] != L'\r')
			{
				SendMessageA(hWnd, WM_KEYDOWN, VK_RETURN, 0L);
				SendMessageA(hWnd, WM_CHAR, '\r', 0L);
				SendMessageA(hWnd, WM_CHAR, '\n', 0L);
				SendMessageA(hWnd, WM_KEYUP, VK_RETURN, 0L);
			}
			else SendMessageW(hWnd, WM_CHAR, lpwText[i], 0);
		}
	}
	else return FALSE;

	return TRUE;
}