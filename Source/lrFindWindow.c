// lrFindWindow.c -- Extension to FindWindow(), use for more comprehensive window search.
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include <Windows.h>

#include "../Head/lrWindowDo.h"

#pragma warning (disable : 6308)
#pragma warning (disable : 28182)


/* Provide window feature information to obtain window hwnd, (the parent window is preferred).
* 
* Parameter:
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*		|	_WindowType				the parameter include the featuer of the window.                           |
*		|	_IncludeChildWindow		find these parent window and these child window if the parameter is true.  |
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* 
* Return:
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*		|	return a type of HWND.                                                                             |
*		|                                                                                                      |
*		|	return a window of the specified feature found(if done).                                           |
*		|	return NULL if failed.                                                                             |
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* 
* Remarks:
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*		|	the parent window is preferred.                                                                    |
*		|	the function can get all windows for find a specified window.                                      |
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HWND lrFindWindow(lrFIND_WINDOW_TYPE _WindowType, BOOL _IncludeChildWindow)
{
	HWND EligileWindow = NULL;
	
	size_t all_WndQuantity = 0;
	HWND* all_Wnd = NULL;

	if (_IncludeChildWindow)
		all_Wnd = lrGetParentAndChildWindowHwndSets(&all_WndQuantity);
	else
		all_Wnd = lrGetWindowHwndSets(&all_WndQuantity);

	if (all_Wnd == NULL || all_WndQuantity == 0) return 0;

	union {
		char WindowClass[MAXLEN_WIN_CLASS];
		char WindowTitle[MAXLEN_WIN_TITLE];
		char WindowPPath[MAX_PATH];
	}wd = { "\0" };

	switch (_WindowType.type)
	{
	case LR__FIND_WINDOW_TYPE__WINDOWCLASS:
		{
			if (((EligileWindow = FindWindowA(_WindowType.data.WindowClass, NULL)) == NULL) && _IncludeChildWindow == TRUE)
			{
				for (size_t i = 0; i < all_WndQuantity; ++i)
				{
					RealGetWindowClassA(all_Wnd[i], wd.WindowClass, MAXLEN_WIN_CLASS);

					if (!stricmp(_WindowType.data.WindowClass, wd.WindowClass))
					{
						EligileWindow = all_Wnd[i];
						break;
					}
				}
			}
			break;
		}
	case LR__FIND_WINDOW_TYPE__WINDOWTITLE:
		{
			if (((EligileWindow = FindWindowA(NULL, _WindowType.data.WindowTitle)) == NULL) && _IncludeChildWindow == TRUE)
			{
				for (size_t i = 0; i < all_WndQuantity; ++i)
				{
					GetWindowTextA(all_Wnd[i], wd.WindowTitle, MAXLEN_WIN_TITLE);

					if (!stricmp(_WindowType.data.WindowTitle, wd.WindowTitle))
					{
						EligileWindow = all_Wnd[i];
						break;
					}
				}
			}
			break;
		}
	case LR__FIND_WINDOW_TYPE__WINDOWPPATH:
		{
			if (_IncludeChildWindow)
			{
				size_t ParentWndQuantity = 0;
				HWND* ParentWnd = lrGetWindowHwndSets(&ParentWndQuantity);

				for (size_t i = 0; i < ParentWndQuantity; ++i)
				{
					lrGetWindowProcessPath(ParentWnd[i], wd.WindowPPath, sizeof(wd.WindowPPath));

					if (!stricmp(_WindowType.data.WindowPPath, wd.WindowPPath))
					{
						EligileWindow = ParentWnd[i];
						break;
					}
				}
				free(ParentWnd);

				if (EligileWindow) break;
			}
			for (size_t i = 0; i < all_WndQuantity; ++i)
			{
				lrGetWindowProcessPath(all_Wnd[i], wd.WindowPPath, sizeof(wd.WindowPPath));

				if (!stricmp(_WindowType.data.WindowPPath, wd.WindowPPath))
				{
					EligileWindow = all_Wnd[i];
					break;
				}
			}

			break;
		}
	}

	free(all_Wnd);
	return EligileWindow;
}


/* Find a window for meet all requirement, (the parent window is preferred).
* 
* Parameter:
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*		|	_WindowClass			Specify a class name of the window, include all window if it is NULL.      |
*		|	_WindowTitle			Specify a title text of the window, include all window if it is NULL.      |
*		|	_WindowPath				Specify a process path of the window, include all window if it is NULL.    |
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* 
* Return:
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*		|	return a type of HWND.                                                                             |
*		|	                                                                                                   |
*		|	return a window of the specified featrue found(if done).                                           |
*		|	return NULL if not found specify window.                                                           |
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* 
* Remarks:
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*		|	the parent window is preferred.                                                                    |
*		|	the function can get all windows for find a specified window.                                      |
*		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HWND lrFindWindowEx(const char* const _WindowClass, const char* const _WindowTitle, const char* const _WindowPath, BOOL _IncludeChildWindow)
{
	/* Eligible window */
	HWND EligileWindow = NULL;


	size_t ParentWndsQuantity = 0;
	HWND* ParentWnds = lrGetWindowHwndSets(&ParentWndsQuantity);

	if (ParentWnds == NULL) return 0;

	/* Include all parent window and all child window */
	size_t ChildWndsQuantity = 0;
	HWND* ChildWnds = NULL;

	/* For avoid wasting memory and time */
	if (_IncludeChildWindow)
	{
		ChildWnds = lrGetParentAndChildWindowHwndSets(&ChildWndsQuantity);

		if (ChildWnds == NULL)
		{
			free(ParentWnds);
			return 0;
		}
	}
	

	union {
		char WindowClass[MAXLEN_WIN_CLASS];
		char WindowTitle[MAXLEN_WIN_TITLE];
		char WindowPPath[MAX_PATH];
	}wd = { "\0" };


	/* The parent window is preferred */
	for (size_t i = 0; i < ParentWndsQuantity; ++i)
	{
		if (_WindowClass)
		{
			RealGetWindowClassA(ParentWnds[i], wd.WindowClass, sizeof(wd.WindowClass));

			/* Tips: If the two window class names are not equal */
			if (stricmp(_WindowClass, wd.WindowClass)) continue;
		}

		if (_WindowTitle)
		{
			GetWindowTextA(ParentWnds[i], wd.WindowTitle, sizeof(wd.WindowTitle));

			if (stricmp(_WindowTitle, wd.WindowTitle)) continue;
		}

		if (_WindowPath)
		{
			lrGetWindowProcessPath(ParentWnds[i], wd.WindowPPath, sizeof(wd.WindowPPath));

			if (stricmp(_WindowPath, wd.WindowPPath)) continue;
		}

		EligileWindow = ParentWnds[i];
		break;
	}

	/* Then seek these child windows */
	if (_IncludeChildWindow && EligileWindow == NULL)
	{
		for (size_t i = 0; i < ChildWndsQuantity; ++i)
		{
			if (_WindowClass)
			{
				RealGetWindowClassA(ChildWnds[i], wd.WindowClass, sizeof(wd.WindowClass));

				/* Tips: If the two window class names are not equal */
				if (stricmp(_WindowClass, wd.WindowClass)) continue;
			}

			if (_WindowTitle)
			{
				GetWindowTextA(ChildWnds[i], wd.WindowTitle, sizeof(wd.WindowTitle));

				if (stricmp(_WindowTitle, wd.WindowTitle)) continue;
			}

			if (_WindowPath)
			{
				lrGetWindowProcessPath(ChildWnds[i], wd.WindowPPath, sizeof(wd.WindowPPath));

				if (stricmp(_WindowPath, wd.WindowPPath)) continue;
			}

			EligileWindow = ChildWnds[i];
			break;
		}
	}

	if (ParentWnds) free(ParentWnds);
	if (ChildWnds)  free(ChildWnds);

	/* Return NULL if not found. */
	return EligileWindow;
}