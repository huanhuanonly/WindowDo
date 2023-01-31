// lrCreateWindow.c -- Create a window
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lr_wdo.h"
#include "../Head/lrWindowDo.h"

#pragma comment (lib, "Msimg32.lib")

#pragma warning (disable : 4477)
#pragma warning (disable : 4313)
#pragma warning (disable : 6273)

#pragma warning (disable : 6308)

/* ��ʱ����Ϣ�������ı��� ID */
#define ID_TIMER_TEXT	(UINT_PTR)0x0001


static HWND*		all_hwnd	= NULL;
static ALIAS_BOX*	all_alias	= NULL;
static size_t		all_quan	= 0;
static HANDLE		all_mutex	= NULL;


static DWORD WINAPI lrReceiveMessagesThreadProc(PCWE _PCWE);
static LRESULT CALLBACK lrCreateBlankBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK lrCreateTextBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK lrCreateInfoBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK lrCreateLogBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK lrCreateImageBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK lrCreateBlusBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



static BOOL lrApplyNewWindowBox(ALIASDEF _BoxType, const ALIAS_BOX* _InputAlias)
{
	if (!all_mutex) all_mutex = CreateMutexA(NULL, FALSE, NULL);
	if (all_mutex == NULL) return FALSE;

	WaitForSingleObject(all_mutex, INFINITE);

	if (all_quan == 0)
	{
		if ((all_hwnd = (HWND*)malloc(sizeof(HWND))) == NULL)
		{
			ReleaseMutex(all_mutex);
			return FALSE;
		}
		if ((all_alias = (ALIAS_BOX*)malloc(sizeof(ALIAS_BOX))) == NULL)
		{
			free(all_hwnd);
			ReleaseMutex(all_mutex);
			return FALSE;
		}

		++all_quan;
	}
	else
	{
		++all_quan;

		void* temp = (HWND*)realloc(all_hwnd, all_quan * sizeof(HWND));
		if (temp == NULL)
		{
			--all_quan;
			ReleaseMutex(all_mutex);
			return FALSE;
		}
		all_hwnd = temp;


		temp = (ALIAS_BOX*)realloc(all_alias, all_quan * sizeof(ALIAS_BOX));
		if (temp == NULL)
		{
			--all_quan;
			ReleaseMutex(all_mutex);
			return FALSE;
		}
		all_alias = temp;
	}
	ALIASSINGLE BoxAlias = { 0 };

	if (_InputAlias) lrCopyAliasEx(&(all_alias[all_quan - 1]), _InputAlias, alias_type_box);

	else if (lrGetAliasDef(_BoxType, &BoxAlias) == FALSE)
	{
		--all_quan;
		if (all_quan == 0)
		{
			free(all_hwnd);
			free(all_alias);
		}
		ReleaseMutex(all_mutex);
		return FALSE;
	}
	else all_alias[all_quan - 1] = BoxAlias.data.box;

	return TRUE;
}


/* �½�һ�����ںͽ��մ�����Ϣ���̡߳�
* 
* ����:
*		_pcwe         |  һ��ָ�� CWE �ṹ���ָ��
* 
* ����ֵ:
*		����ɹ��������´����Ĵ��ڵľ����
*		���ʧ�ܣ����� NULL��
* 
* ����:
*		������ڱ��رգ���Ӧ�߳�Ҳ���Զ��رա�
*/
HWND lrCreateWindow(PCWE _pcwe)
{
	_pcwe->done = FALSE;

	CreateThread(NULL, 0, lrReceiveMessagesThreadProc, _pcwe, 0, 0);

	while (_pcwe->done == FALSE) Sleep(0);

	return _pcwe->hwnd;
}


/* �����򲢻�ȡ�����ı����ı����� GetLastError()��*/
void lrCreateErrorTextBox()
{
	LPVOID lpMsgBuf = NULL;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		lrERRORTEXT_LANGUAGE,
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	MessageBoxA(NULL, lpMsgBuf, lr_TO_STR(lrCreateErrorTextBox), 0);
	LocalFree(lpMsgBuf);
	return;
}


/* ��֤�Ƿ��Ǵ��ڿ�
* 
* ����:
*		hWnd          |  Ҫ��֤�Ĵ��ھ��
* 
* ����ֵ:
*		���򷵻� TRUE��
*		���߷��� FALSE��
*/
BOOL lrIsWindowBox(HWND hWnd)
{
	if (all_quan && all_hwnd)
		for (size_t i = 0; i < all_quan; ++i)
			if (all_hwnd[i] == hWnd)
				return TRUE;
	return FALSE;
}



/* ��ȡ��ı������������ lrCreateTextBox() �Ⱥ���������
*
* ����:
*		_Box          |  ��Ĵ��ھ��
*/
BOOL lrGetBoxAlias(HWND _Box, ALIAS_BOX* _BoxAlias)
{
	if (all_mutex == NULL) return FALSE;
	WaitForSingleObject(all_mutex, INFINITE);

	/* �����������Ϊ�������ֱ�ӷ��� */
	if (all_quan == 0)
	{
		ReleaseMutex(all_mutex);
		return FALSE;
	}

	for (size_t i = 0; i < all_quan; ++i)
	{
		if (all_hwnd[i] == _Box)
		{
			*_BoxAlias = all_alias[i];
			ReleaseMutex(all_mutex);

			return TRUE;
		}
	}
	ReleaseMutex(all_mutex);
	return FALSE;
}


/* ���ÿ�ı������������ lrCreateTextBox() �Ⱥ���������
*
* ����:
*		_Box          |  ��Ĵ��ھ��
*		_BoxAlias     |  ����±����������������
*
* ����ֵ:
*		FALSE         |  ����ʧ�ܡ�
*		TRUE          |  �����ɹ���
* 
* ����:
*		���ͷ�ԭ���ڴ����Դ��ԭ��ָ�뽫ʧЧ��
*/
BOOL lrSetBoxAlias(HWND _Box, const ALIAS_BOX* _BoxAlias)
{
	/* �жϴ����Ƿ���Ч */
	if (!IsWindow(_Box)) return FALSE;

	if (all_mutex == NULL) return FALSE;
	WaitForSingleObject(all_mutex, INFINITE);

	/* �����������Ϊ�������ֱ�ӷ��� */
	if (all_quan == 0)
	{
		ReleaseMutex(all_mutex);
		return FALSE;
	}


	for (size_t i = 0; i < all_quan; ++i)
	{
		if (all_hwnd[i] == _Box)
		{
			/* �ͷ�ԭ���ڴ� */
			lrClearAliasEx(&(all_alias[i]), alias_type_box);

			all_alias[i] = *_BoxAlias;
			ReleaseMutex(all_mutex);

			SendMessageA(_Box, WM_PAINT, 0, 0);
			SendMessageA(_Box, WM_WDO_RELOADBOXALIAS, 0, 0);
			return TRUE;
		}
	}
	/* ����Ҳ����˴��� */
	ReleaseMutex(all_mutex);
	return FALSE;
}


/* �½�һ���հ׿� */
HWND lrCreateBlankBox(int _x, int _y, int _w, int _h, const ALIAS_BOX* _BoxAlias)
{
	if (lrApplyNewWindowBox(alias_def_box_blank, _BoxAlias) == FALSE)
	{
		return 0;
	}

	WNDCLASS wndclass = { 0 };

	wndclass.lpszClassName = all_alias[all_quan - 1].ClassName;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = all_alias[all_quan - 1].hCursor;
	wndclass.hIcon = all_alias[all_quan - 1].hIcon;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = 0;
	wndclass.lpfnWndProc = lrCreateBlankBoxProc;
	wndclass.lpszMenuName = NULL;
	wndclass.style = 0;

	/* ע�ᴰ���� */
	RegisterClassA(&wndclass);


	CWE cwe = { 0 };
	cwe.dwExStyle = 0;
	strcpy(cwe.lpClassName, all_alias[all_quan - 1].ClassName);
	strcpy(cwe.lpWindowName, lrTITLETEXT_BLANKBOX);
	cwe.dwStyle = WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX;
	cwe.X = _x;
	cwe.Y = _y;
	cwe.nWidth = _w > 20 ? _w : 20;
	cwe.nHeight = _h > 20 ? _h : 20;
	cwe.hWndParent = 0;
	cwe.hMenu = 0;
	cwe.hInstance = 0;
	cwe.lpParam = 0;

	ReleaseMutex(all_mutex);

	return lrCreateWindow(&cwe);
}

static char** TextboxText = NULL;
static HWND* TextboxHomewin = NULL;
static ULONG TextboxCount = 0;
static HANDLE TextboxMutex = NULL;

/* �½�һ���ı���
* 
* ����:
*		_text         |  ���ڵ��ı�����
*		_x            |  ���ڳ�ʼ�Ķ�������
*		_y            |  ���ڳ�ʼ�Ķ�������
*		_BoxAlias     |  ��ʼ�������ã����Ϊ NULL ��ʹ��Ĭ�ϱ���
* 
* ����ֵ:
*		�µĿ��������ʧ���򷵻� NULL��
*/
HWND lrCreateTextBox(const char* const _text, int _x, int _y, const ALIAS_BOX* _BoxAlias)
{
	if (!TextboxMutex) TextboxMutex = CreateMutexA(NULL, FALSE, NULL);
	if (TextboxMutex == NULL) return 0;
	WaitForSingleObject(TextboxMutex, INFINITE);
	
	if ((TextboxText = (char**)realloc(TextboxText, (++TextboxCount) * sizeof(char*))) == NULL)
	{
		--TextboxCount;
		ReleaseMutex(TextboxMutex);
		return 0;
	}
	if ((TextboxText[TextboxCount - 1] = (char*)malloc(_text ? strlen(_text) + 1 : sizeof("TEXT"))) == NULL)
	{
		--TextboxCount;
		ReleaseMutex(TextboxMutex);
		return 0;
	}
	strcpy(TextboxText[TextboxCount - 1], _text ? _text : "TEXT");

	if ((TextboxHomewin = (HWND*)realloc(TextboxHomewin, TextboxCount * sizeof(HWND))) == NULL)
	{
		--TextboxCount;
		free(TextboxText[TextboxCount - 1]);
		ReleaseMutex(TextboxMutex);
		return 0;
	}

	if (lrApplyNewWindowBox(alias_def_box_text, _BoxAlias) == FALSE)
	{
		free(TextboxText[TextboxCount - 1]);

		--TextboxCount;
		if (TextboxCount == 0)
		{
			free(TextboxText);
			free(TextboxHomewin);
		}

		ReleaseMutex(TextboxMutex);
		return 0;
	}
	ReleaseMutex(TextboxMutex);

	WNDCLASS wndclass = { 0 };

	wndclass.lpszClassName = all_alias[all_quan - 1].ClassName;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = all_alias[all_quan - 1].hCursor;
	wndclass.hIcon = all_alias[all_quan - 1].hIcon;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = 0;
	wndclass.lpfnWndProc = lrCreateTextBoxProc;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	/* ע�ᴰ���� */
	RegisterClassA(&wndclass);
	

	CWE cwe = { 0 };
	cwe.dwExStyle = 0;
	strcpy(cwe.lpClassName, all_alias[all_quan - 1].ClassName);
	strcpy(cwe.lpWindowName, lrTITLETEXT_TEXTBOX);
	cwe.dwStyle = 0;
	cwe.X = _x;
	cwe.Y = _y;
	cwe.nWidth = 0;
	cwe.nHeight = 0;
	cwe.hWndParent = 0;
	cwe.hMenu = 0;
	cwe.hInstance = 0;
	cwe.lpParam = 0;

	ReleaseMutex(all_mutex);

	return lrCreateWindow(&cwe);
}



/* ��Ϣ���ڽṹ�� */
static struct __WINDOWINFOBOX
{
	/* Ҫ��ʾ�Ĵ������� */
	INFOBOX_WINDOWTYPE type;

	HWND* infowin;
	HWND HomeWin;			// �˽ṹ��Ĺ�������
} *Infobox;
/* lrCreateInfoBox(): �������� */
static ULONG InfoboxCount = 0;
/* lrCreateInfoBox(): �ٽ绥�� */
static HANDLE InfoboxMutex = 0;

/* �½�һ����Ϣ��
* 
* ����:
*		_hwnd         |  һ������ָ���ַ������ _type ���� _hwnd ��ʹ�ã�������Ĵ�ֵ
*		_type         |  ����ʹ���ĸ����ڣ��Լ��Ƿ�ʹ�� _hwnd
*		_x            |  ���ڳ�ʼ�Ķ�������
*		_y            |  ���ڳ�ʼ�Ķ�������
*		_BoxAlias     |  ��ʼ�������ã����Ϊ NULL ��ʹ��Ĭ�ϱ���
* 
* ����ֵ:
*		�µĿ��������ʧ���򷵻� NULL��
*/
HWND lrCreateInfoBox(volatile HWND* volatile _hwnd, INFOBOX_WINDOWTYPE _type, int _x, int _y, const ALIAS_BOX* _BoxAlias)
{
	/* ���ò������ٽ��� */
	if (!InfoboxMutex) InfoboxMutex = CreateMutexA(NULL, FALSE, NULL);
	if (InfoboxMutex == 0) return 0;
	WaitForSingleObject(InfoboxMutex, INFINITE);

	if (InfoboxCount == 0)
	{
		Infobox = (struct __WINDOWINFOBOX*)malloc((++InfoboxCount) * sizeof(struct __WINDOWINFOBOX));
		if (Infobox == NULL)
		{
			--InfoboxCount;
			ReleaseMutex(InfoboxMutex);
			return 0;
		}
	}
	else
	{

		Infobox = (struct __WINDOWINFOBOX*)realloc((void*)Infobox, (++InfoboxCount) * sizeof(struct __WINDOWINFOBOX));
		if (Infobox == NULL)
		{
			--InfoboxCount;
			ReleaseMutex(InfoboxMutex);
			return 0;
		}
	}
	Infobox[InfoboxCount - 1].type = _type;

	
	switch (_type)
	{
	case infobox_s_ct:
		if(_hwnd)
			Infobox[InfoboxCount - 1].infowin = (HWND*)(*_hwnd);
		break;
	case infobox_s_par:
		if(_hwnd)
			Infobox[InfoboxCount - 1].infowin = (volatile HWND*)GetParent((HWND)(*_hwnd));
		break;
	case infobox_s_top:
		Infobox[InfoboxCount - 1].infowin = (volatile HWND*)GetForegroundWindow();
		break;
	case infobox_s_desk:
		Infobox[InfoboxCount - 1].infowin = (volatile HWND*)GetDesktopWindow();
		break;
	case infobox_s_cur:
	{
		POINT curpos;
		GetCursorPos(&curpos);
		Infobox[InfoboxCount - 1].infowin = (volatile HWND*)WindowFromPoint(curpos);
		break;
	}
	case infobox_s_cp:
	{
		POINT curpos;
		GetCursorPos(&curpos);
		HWND hwnd = WindowFromPoint(curpos);

		Infobox[InfoboxCount - 1].infowin = (volatile HWND*)GetParent(hwnd);

		if (Infobox[InfoboxCount - 1].infowin == NULL)
			Infobox[InfoboxCount - 1].infowin = (volatile HWND*)hwnd;

		break;
	}
	case infobox_v_ct:
	case infobox_v_par:
	case infobox_v_top:
	case infobox_v_desk:
	case infobox_v_cur:
	case infobox_v_cp:
		Infobox[InfoboxCount - 1].infowin = _hwnd;
		break;
	default:
		Infobox[InfoboxCount - 1].type = infobox_s_ct;
		if (_hwnd) Infobox[InfoboxCount - 1].infowin = (volatile HWND*)(*_hwnd);
		break;
	}


	if (lrApplyNewWindowBox(alias_def_box_info, _BoxAlias) == FALSE)
	{
		--InfoboxCount;
		if (InfoboxCount == 0)
		{
			free(Infobox);
		}
		ReleaseMutex(InfoboxMutex);
		return 0;
	}
	
	ReleaseMutex(InfoboxMutex);

	WNDCLASS wndclass = { 0 };

	wndclass.lpszClassName = all_alias[all_quan - 1].ClassName;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = all_alias[all_quan - 1].hCursor;
	wndclass.hIcon = all_alias[all_quan - 1].hIcon;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = 0;
	wndclass.lpfnWndProc = lrCreateInfoBoxProc;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	/* ע������� */
	RegisterClassA(&wndclass);
	

	CWE cwe = { 0 };
	cwe.dwExStyle = 0;
	strcpy(cwe.lpClassName, all_alias[all_quan - 1].ClassName);
	strcpy(cwe.lpWindowName, lrTITLETEXT_INFOBOX);
	cwe.dwStyle = WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX;
	cwe.X = _x;
	cwe.Y = _y;
	cwe.nWidth = 900;
	cwe.nHeight = 500;
	cwe.hWndParent = 0;
	cwe.hMenu = 0;
	cwe.hInstance = 0;
	cwe.lpParam = 0;

	ReleaseMutex(all_mutex);

	return lrCreateWindow(&cwe);
}



HWND lrCreateLogBox(int _x, int _y, int _w, int _h, const char* const _LogText, const ALIAS_BOX* _BoxAlias)
{
	if (lrApplyNewWindowBox(alias_def_box_log, _BoxAlias) == FALSE)
	{
		return 0;
	}

	WNDCLASS wndclass = { 0 };

	wndclass.lpszClassName = all_alias[all_quan - 1].ClassName;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = all_alias[all_quan - 1].hCursor;
	wndclass.hIcon = all_alias[all_quan - 1].hIcon;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = 0;
	wndclass.lpfnWndProc = lrCreateLogBoxProc;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	/* ע������� */
	RegisterClassA(&wndclass);


	CWE cwe = { 0 };
	cwe.dwExStyle = 0;
	strcpy(cwe.lpClassName, all_alias[all_quan - 1].ClassName);
	strcpy(cwe.lpWindowName, lrTITLETEXT_LOGBOX);
	cwe.dwStyle = WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX;
	cwe.X = _x;
	cwe.Y = _y;
	cwe.nWidth = _w;
	cwe.nHeight = _h;
	cwe.hWndParent = 0;
	cwe.hMenu = 0;
	cwe.hInstance = 0;
	cwe.lpParam = 0;

	ReleaseMutex(all_mutex);

	HWND logbox = lrCreateWindow(&cwe);
	if (logbox)
	{
		SendMessageA(logbox, WM_WDO_SETBOXTEXT, (WPARAM)_LogText, 0L);
	}
	return logbox;
}


/* ͼƬ��ͼƬ */
static HBITMAP* ImageboxImage = 0;
/* ͼƬ��С */
static SIZE* ImageboxImageSize = 0;
/* ��һ���α����� */
static POINT* ImageboxLastCurpos = 0;
/* ��һ���α�������������ֽ��� */
static BYTE* ImageboxLastCurbyte = 0;
/* �������� */
static HWND* ImageboxHomeWin = 0;
/* �������� */
static ULONG ImageboxCount = 0;
/* �ٽ绥�� */
static HANDLE ImageboxMutex = 0;


/* �½�һ��ͼƬ��
* 
* ����:
*		_imagepath    |  ͼƬ·��
*		_x            |  ���ڳ�ʼ�Ķ�������
*		_y            |  ���ڳ�ʼ�Ķ�������
*		_BoxAlias     |  ��ʼ�������ã����Ϊ NULL ��ʹ��Ĭ�ϱ���
* 
* ����ֵ:
*		�µĿ��������ʧ���򷵻� NULL��
*/
HWND lrCreateImageBox(const char* const _imagepath, int _x, int _y, const ALIAS_BOX* _BoxAlias)
{
	/* ���ͼƬ·������ȷ���˳� */
	if (_imagepath == NULL || _imagepath[0] == '\0') return 0;
	else
	{
		HBITMAP hBitmap = LoadImageA(0, _imagepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (hBitmap) DeleteObject(hBitmap);
		else return 0;
	}


	if(!ImageboxMutex) ImageboxMutex = CreateMutexA(NULL, FALSE, NULL);
	if (ImageboxMutex == 0) return 0;
	WaitForSingleObject(ImageboxMutex, INFINITE);

	if (ImageboxCount == 0)
	{
		++ImageboxCount;

		if ((ImageboxImage = (HBITMAP*)malloc(sizeof(HBITMAP))) == NULL) wdo_error_ex();

		if ((ImageboxImageSize = (SIZE*)malloc(sizeof(SIZE))) == NULL) wdo_error_ex();

		if ((ImageboxLastCurpos = (POINT*)malloc(sizeof(POINT))) == NULL) wdo_error_ex();

		if ((ImageboxLastCurbyte = (BYTE*)malloc(sizeof(BYTE))) == NULL) wdo_error_ex();

		if ((ImageboxHomeWin = (HWND*)malloc(sizeof(HWND))) == NULL) wdo_error_ex();
	}
	else
	{
		++ImageboxCount;

		ImageboxImage = (HBITMAP*)realloc(ImageboxImage, ImageboxCount * sizeof(HBITMAP));
		if (ImageboxImage == NULL) wdo_error_ex();

		ImageboxImageSize = (SIZE*)realloc(ImageboxImageSize, ImageboxCount * sizeof(SIZE));
		if (ImageboxImageSize == NULL) wdo_error_ex();

		ImageboxLastCurpos = (POINT*)realloc(ImageboxLastCurpos, ImageboxCount * sizeof(POINT));
		if (ImageboxLastCurpos == NULL) wdo_error_ex();

		ImageboxLastCurbyte = (BYTE*)realloc(ImageboxLastCurbyte, ImageboxCount * sizeof(BYTE));
		if (ImageboxLastCurbyte == NULL) wdo_error_ex();

		ImageboxHomeWin = (HWND*)realloc(ImageboxHomeWin, ImageboxCount * sizeof(HWND));
		if (ImageboxHomeWin == NULL) wdo_error_ex();
	}

	if (ImageboxImage != NULL) ImageboxImage[ImageboxCount - 1] = LoadImageA(0, _imagepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	/* ��ȡͼƬ��С */
	SIZE ImageSize = ImageboxImageSize[ImageboxCount - 1] = lrGetBitmapSize(_imagepath);

	if (lrApplyNewWindowBox(alias_def_box_image, _BoxAlias) == FALSE)
	{
		--ImageboxCount;
		if (ImageboxCount == 0)
		{
			free(ImageboxImage);
			free(ImageboxImageSize);
			free(ImageboxLastCurpos);
			free(ImageboxLastCurbyte);
			free(ImageboxHomeWin);
		}
		ReleaseMutex(ImageboxMutex);
		return 0;
	}
	ReleaseMutex(ImageboxMutex);

	WNDCLASS wndclass = { 0 };
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = all_alias[all_quan - 1].hCursor;
	wndclass.hIcon = all_alias[all_quan - 1].hIcon;
	wndclass.hInstance = 0;
	wndclass.lpfnWndProc = lrCreateImageBoxProc;
	wndclass.lpszClassName = all_alias[all_quan - 1].ClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	/* ע������� */
	RegisterClassA(&wndclass);
	

	CWE cwe = { 0 };

	cwe.dwExStyle = 0;
	strcpy(cwe.lpClassName, all_alias[all_quan - 1].ClassName);

	/* �ϳɴ������� */
	char ImageName[_MAX_FNAME], ImageExt[_MAX_EXT];
	_splitpath(_imagepath, NULL, NULL, ImageName, ImageExt);
	strcpy(cwe.lpWindowName, strcat(ImageName, ImageExt));
	
	cwe.dwStyle = WS_SIZEBOX | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_BORDER;
	cwe.X = _x;
	cwe.Y = _y;
	cwe.nWidth = ImageSize.cx;
	cwe.nHeight = ImageSize.cy;
	if (cwe.nWidth < 200) {
		cwe.nHeight += 200 - cwe.nWidth;
		cwe.nWidth = 200;
	}
	if (cwe.nHeight < 200) {
		cwe.nWidth += 200 - cwe.nHeight;
		cwe.nHeight = 200;
	}

	cwe.hWndParent = 0;
	cwe.hMenu = 0;
	cwe.hInstance = 0;
	cwe.lpParam = 0;

	ReleaseMutex(all_mutex);
	
	return lrCreateWindow(&cwe);
}


static char* BlueboxText = NULL;


/* ����һ��������
* 
*		_text         |  ���ڵ��ı�
*		_BoxAlias     |  ��ʼ�������ã����Ϊ NULL ��ʹ��Ĭ�ϱ���
*
* ����ֵ:
*		�µĿ��������ʧ���򷵻� NULL��
*/
HWND lrCreateBlueBox(const char* const _text, const ALIAS_BOX* _BoxAlias)
{
	/* ֻ������һ������ */
	HWND lastBluebox = FindWindowA(lrCLASSNAME_BLUEBOX, lrTITLETEXT_BLUEBOX);
	if (lastBluebox) SendMessageA(lastBluebox, WM_CLOSE, 0, 0);

	if (_text && _text[0])
	{
		if (BlueboxText) free(BlueboxText);
		if ((BlueboxText = (char*)malloc(strlen(_text) + 1)) == NULL)
		{
			BlueboxText = NULL;
			return 0;
		}
		else strcpy(BlueboxText, _text);
	}
	else
	{
		if (BlueboxText) free(BlueboxText);
		if ((BlueboxText = (char*)malloc(sizeof("Blue screen warning"))) == NULL)
		{
			BlueboxText = NULL;
			return 0;
		}
		else strcpy(BlueboxText, "Blue screen warning");
	}

	if (lrApplyNewWindowBox(alias_def_box_blue, _BoxAlias) == FALSE)
	{
		free(BlueboxText);
		BlueboxText = NULL;
	}
	
	WNDCLASS wndclass = { 0 }; 

	wndclass.lpszClassName = all_alias[all_quan - 1].ClassName;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = all_alias[all_quan - 1].hCursor;
	wndclass.hIcon = all_alias[all_quan - 1].hIcon;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = 0;
	wndclass.lpfnWndProc = lrCreateBlusBoxProc;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_NOCLOSE;

	/* ע������� */
	RegisterClassA(&wndclass);


	CWE cwe = { 0 };
	cwe.dwExStyle = WS_EX_TOPMOST;
	strcpy(cwe.lpClassName, all_alias[all_quan - 1].ClassName);
	strcpy(cwe.lpWindowName, lrTITLETEXT_BLUEBOX);
	cwe.dwStyle = 0;
	cwe.X = 0;
	cwe.Y = 0;
	cwe.nWidth = GetSystemMetrics(SM_CXSCREEN);
	cwe.nHeight = GetSystemMetrics(SM_CYSCREEN);
	cwe.hWndParent = 0;
	cwe.hMenu = 0;
	cwe.hInstance = 0;
	cwe.lpParam = 0;

	ReleaseMutex(all_mutex);

	return lrCreateWindow(&cwe);
}


/* lrCreateWindow(): ����һ���߳��Դ��ڵ���Ϣ���� */
static DWORD WINAPI lrReceiveMessagesThreadProc(PCWE _PCWE)
{
	_PCWE->done = FALSE;

	/* �������� */
	HWND hwnd = _PCWE->hwnd =
		CreateWindowExA(
			_PCWE->dwExStyle,
			_PCWE->lpClassName,
			_PCWE->lpWindowName,
			_PCWE->dwStyle,
			_PCWE->X,
			_PCWE->Y,
			_PCWE->nWidth,
			_PCWE->nHeight,
			_PCWE->hWndParent,
			_PCWE->hMenu,
			_PCWE->hInstance,
			_PCWE->lpParam
		);

	_PCWE->done = TRUE;

	if (hwnd == 0) return 1;


	if(!_PCWE->CencelShow)
		ShowWindow(hwnd, SW_SHOW);

	UpdateWindow(hwnd);

	BOOL bRet;
	MSG msg;

	/* ��ȡ��Ϣ */
	while ((bRet = GetMessageA(&msg, hwnd, 0, 0)) != 0)
	{
		if (bRet == -1) break;
		else
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	return (DWORD)msg.wParam;
}

/* lrCreateBlankBox(): �հ׿���� */
static LRESULT CALLBACK lrCreateBlankBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		wdo_add_window(hWnd);

		WaitForSingleObject(all_mutex, INFINITE);

		all_hwnd[all_quan - 1] = hWnd;

		RECT rect;
		GetClientRect(hWnd, &rect);

		HDC hdc = GetDC(hWnd);
		HBRUSH BackBrush = CreateSolidBrush(all_alias[all_quan - 1].BackColor);
		SelectObject(hdc, BackBrush);

		/* ˢ�»�������������Ӱ */
		PatBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);

		DeleteObject(BackBrush);
		ReleaseDC(hWnd, hdc);

		ReleaseMutex(all_mutex);
		return 0;
	}
	case WM_DESTROY:
	{
		wdo_del_window(hWnd);
		WaitForSingleObject(all_mutex, INFINITE);

		size_t all_number = 0;

		for (ULONG i = 0; i < all_quan; ++i) {
			if (all_hwnd[i] == hWnd) {
				all_number = i;
				break;
			}
		}

		if (all_quan == 1)
		{
			all_quan = 0;

			lrClearAliasEx(all_alias, alias_type_box);

			free(all_alias);
			all_alias = NULL;

			free(all_hwnd);
			all_hwnd = NULL;

			CloseHandle(all_mutex);
			all_mutex = NULL;
		}
		else
		{
			--all_quan;

			lrClearAliasEx(&(all_alias[all_number]), alias_type_box);

			all_alias[all_number] = all_alias[all_quan];
			all_hwnd[all_number] = all_hwnd[all_quan];

			ReleaseMutex(all_mutex);
		}

		PostQuitMessage(0);
		return 0;
	}
	default: return DefWindowProcA(hWnd, uMsg, wParam, lParam);
	}
}

/* lrCreateTextBox(): �ı������ */
static LRESULT CALLBACK lrCreateTextBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* ��ǰ�Ĵ��ڱ�� */
	size_t all_number = 0;

	ULONG WinNumber = 0;

	/* �����ٽ��� */
	WaitForSingleObject(all_mutex, INFINITE);
	WaitForSingleObject(TextboxMutex, INFINITE);

	/* ������ǰ���� */
	if (uMsg != WM_CREATE) {
		for (ULONG i = 0; i < all_quan; ++i) {
			if (all_hwnd[i] == hWnd) {
				all_number = i;
				break;
			}
		}
		for (ULONG i = 0; i < TextboxCount; ++i) {
			if (TextboxHomewin[i] == hWnd) {
				WinNumber = i;
				break;
			}
		}
	}


	switch (uMsg)
	{
	case WM_CREATE:
		{
			wdo_add_window(hWnd);

			/* ���浱ǰ���ھ���Ա��ȡ��Ӧ�ı� */
			all_hwnd[all_quan - 1] = hWnd;
			WinNumber = TextboxCount - 1;

			TextboxHomewin[TextboxCount - 1] = hWnd;

			LONG style = GetWindowLongA(hWnd, GWL_STYLE);

			HDC hdc = GetDC(hWnd);
			SelectObject(hdc, all_alias[all_number].hFont);

			/* �´��ڿ�Ϊ�������� * �ı����ȣ��߶�Ϊ������߶� * �������� */
			SetWindowPos(hWnd
				, NULL, 0, 0
				, (int)((style & WS_BORDER)
					? (max(all_alias[all_number].fontinfo.width + GetSystemMetrics(SM_CXBORDER), 20)) * (int)strlen(TextboxText[WinNumber])
					: max(all_alias[all_number].fontinfo.width, 10) * (int)strlen(TextboxText[WinNumber])) + 10
				, (int)((style & WS_CAPTION)
					? (max(all_alias[all_number].fontinfo.height + GetSystemMetrics(SM_CYCAPTION), 80)) * (lrstrcnt(TextboxText[WinNumber], '\n') + 1)
					: (max(all_alias[all_number].fontinfo.height, 40)) * (lrstrcnt(TextboxText[WinNumber], '\n') + 1))
				, SWP_NOMOVE | SWP_NOZORDER);
			

			/* �����α� */
			SetCursor(all_alias[all_number].hCursor);

			ReleaseDC(hWnd, hdc);

			/* �˳��ٽ��� */
			ReleaseMutex(TextboxMutex);
			ReleaseMutex(all_mutex);
			return 0;
		}
	case WM_WDO_RELOADBOXALIAS:
		{
			SetCursor(all_alias[all_number].hCursor);
			ReleaseMutex(TextboxMutex);
			ReleaseMutex(all_mutex);
			return 0;
		}
	case WM_WDO_SETBOXTEXT:
		{
			char* p = (char*)wParam;
			if (p && p[0])
			{
				free(TextboxText[WinNumber]);
				if (TextboxText[WinNumber] = (char*)malloc(strlen((char*)wParam) + 1))
				{
					strcpy(TextboxText[WinNumber], p);

					LONG style = GetWindowLongA(hWnd, GWL_STYLE);

					HDC hdc = GetDC(hWnd);
					SelectObject(hdc, all_alias[all_number].hFont);

					SetWindowPos(hWnd
						, NULL, 0, 0
						, (int)((style & WS_BORDER)
							? (max(all_alias[all_number].fontinfo.width + GetSystemMetrics(SM_CXBORDER), 20)) * (int)strlen(TextboxText[WinNumber])
							: max(all_alias[all_number].fontinfo.width, 10) * (int)strlen(TextboxText[WinNumber])) + 10
						, (int)((style & WS_CAPTION)
							? (max(all_alias[all_number].fontinfo.height + GetSystemMetrics(SM_CYCAPTION), 80)) * (lrstrcnt(TextboxText[WinNumber], '\n') + 1)
							: (max(all_alias[all_number].fontinfo.height, 40)) * (lrstrcnt(TextboxText[WinNumber], '\n') + 1))
						, SWP_NOMOVE | SWP_NOZORDER);

					ReleaseDC(hWnd, hdc);

					ReleaseMutex(TextboxMutex);
					ReleaseMutex(all_mutex);
					SendMessageA(hWnd, WM_PAINT, 0, 0);
					return TRUE;
				}
				else
				{
					wdo_error_ex();
					ReleaseMutex(TextboxMutex);
					ReleaseMutex(all_mutex);
					return FALSE;
				}
			}
			else
			{
				ReleaseMutex(TextboxMutex);
				ReleaseMutex(all_mutex);
				return FALSE;
			}
		}
	case WM_STYLECHANGED:
	{
		LONG style = GetWindowLongA(hWnd, GWL_STYLE);

		SetWindowPos(hWnd
			, NULL, 0, 0
			, (int)((style& WS_BORDER)
				? (max(all_alias[all_number].fontinfo.width + GetSystemMetrics(SM_CXBORDER), 20)) * (int)strlen(TextboxText[WinNumber])
				: max(all_alias[all_number].fontinfo.width, 10) * (int)strlen(TextboxText[WinNumber])) + 10
			, (int)((style& WS_CAPTION)
				? (max(all_alias[all_number].fontinfo.height + GetSystemMetrics(SM_CYCAPTION), 80)) * (lrstrcnt(TextboxText[WinNumber], '\n') + 1)
				: (max(all_alias[all_number].fontinfo.height, 40)) * (lrstrcnt(TextboxText[WinNumber], '\n') + 1))
			, SWP_NOMOVE | SWP_NOZORDER);

		ReleaseMutex(all_mutex);
		ReleaseMutex(TextboxMutex);
		return 0;
	}
	case WM_PAINT:
		{
			HDC hdc = GetDC(hWnd);

			/* ָ��λͼ */
			HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 0, 0);
			SelectObject(hdc, hBitmap);

			/* ָ������ */
			SelectObject(hdc, all_alias[all_number].hFont);

			/* ����������ɫ */
			SetTextColor(hdc, all_alias[all_number].TextColor);

			/* ���ñ�����ɫ */
			// SetBkColor(hdc, all_alias[all_number].data.box.BackColor);

			/* ���û�����ɫ�������ػ汳�� */
			HBRUSH BackBrush = CreateSolidBrush(all_alias[all_number].BackColor);
			SelectObject(hdc, BackBrush);

			/* ���û��ģʽ */
			SetBkMode(hdc, TRANSPARENT);


			/* �ͻ����� */
			RECT rect;
			GetClientRect(hWnd, &rect);

			/* ʹ��Ч������Ч���������ظ����� WM_PAINT */
			if (uMsg == WM_PAINT) ValidateRect(hWnd, &rect);

			/* ˢ�»�������������Ӱ */
			PatBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);

			BOOL FindLinebreak = FALSE;
			size_t len = strlen(TextboxText[WinNumber]);
			for (UINT i = 0; i < len; ++i)
			{
				if (TextboxText[WinNumber][i] == '\n')
				{
					FindLinebreak = TRUE;
					break;
				}
			}

			/* �����ı� */
			if (TextboxText[WinNumber])
				DrawTextA(hdc
					, TextboxText[WinNumber]
					, (int)len
					, &rect
					, FindLinebreak ? DT_LEFT : DT_CENTER
				);


			/* ɾ��λͼ */
			DeleteObject(hBitmap);
			/* ɾ������ */
			DeleteObject(BackBrush);
			/* ɾ�� DC */
			ReleaseDC(hWnd, hdc);

			/* �˳��ٽ��� */
			ReleaseMutex(all_mutex);
			ReleaseMutex(TextboxMutex);
			return 0;
		}
	case WM_DESTROY:
		{
			wdo_del_window(hWnd);

			if (all_quan == 1)
			{
				all_quan = 0;

				lrClearAliasEx(all_alias, alias_type_box);

				free(all_alias);
				all_alias = NULL;

				free(all_hwnd);
				all_hwnd = NULL;

				CloseHandle(all_mutex);
				all_mutex = NULL;
			}
			else
			{
				--all_quan;

				lrClearAliasEx(&(all_alias[all_number]), alias_type_box);

				all_alias[all_number] = all_alias[all_quan];
				all_hwnd[all_number] = all_hwnd[all_quan];

				ReleaseMutex(all_mutex);
			}

			if (TextboxCount == 1)
			{
				TextboxCount = 0;

				free(TextboxHomewin);
				TextboxHomewin = NULL;

				free(TextboxText[0]);
				free(TextboxText);
				TextboxText = NULL;

				CloseHandle(TextboxMutex);
				TextboxMutex = NULL;
			}
			else
			{
				--TextboxCount;

				TextboxHomewin[WinNumber] = TextboxHomewin[TextboxCount];
				TextboxText[WinNumber] = TextboxText[TextboxCount];

				free(TextboxText[TextboxCount]);
				ReleaseMutex(TextboxMutex);
			}

			PostQuitMessage(0);
			return 0;
		}
	}
	ReleaseMutex(TextboxMutex);
	ReleaseMutex(all_mutex);

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

/* lrCreateInfoBox(): ��Ϣ����� */
static LRESULT CALLBACK lrCreateInfoBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* ��¼��һ�����ڣ�ÿ�θ������ڶ���Ҫ�ػ� */
	static HWND* lastWin = NULL;
	
	/* ��ǰ�Ĵ��ڱ�� */
	ULONG WinNumber = 0;
	size_t all_number = 0;
	/* ��ǰҪ��ʾ�����ĸ����ڵĴ�����Ϣ */
	HWND ctwin = 0;

	/* �����ٽ��� */
	WaitForSingleObject(InfoboxMutex, INFINITE);
	WaitForSingleObject(all_mutex, INFINITE);

	/* ������ǰ���� */
	if (uMsg != WM_CREATE) {
		for (ULONG i = 0; i < InfoboxCount; ++i) {
			if (Infobox[i].HomeWin == hWnd) {
				WinNumber = i;
				break;
			}
		}
		for (ULONG i = 0; i < all_quan; ++i) {
			if (all_hwnd[i] == hWnd) {
				all_number = i;
				break;
			}
		}
	}

	/* ����Ҫ��ʾ�Ĵ������� */
	switch (Infobox[WinNumber].type)
	{
	case infobox_s_ct:
	case infobox_s_par:
	case infobox_s_top:
	case infobox_s_desk:
	case infobox_s_cur:
	case infobox_s_cp:

		ctwin = (volatile HWND*)Infobox[WinNumber].infowin;
		break;
		
	case infobox_v_ct:

		ctwin = Infobox[WinNumber].infowin[0];
		break;

	case infobox_v_par:

		ctwin = GetParent(Infobox[WinNumber].infowin[0]);

		if (ctwin == NULL)
			ctwin = Infobox[WinNumber].infowin[0];
		break;

	case infobox_v_top:
		
		ctwin = GetForegroundWindow();
		break;
		
	case infobox_v_desk:

		ctwin = GetDesktopWindow();
		break;

	case infobox_v_cur:
	{
		POINT curpos;
		GetCursorPos(&curpos);
		ctwin = WindowFromPoint(curpos);
		break;
	}
	case infobox_v_cp:
	{
		POINT curpos;
		GetCursorPos(&curpos);
		HWND hwnd = WindowFromPoint(curpos);

		ctwin = GetParent(hwnd);

		if (ctwin == NULL) ctwin = hwnd;

		break;
	}
	}

	/* �˳��ٽ��� */
	ReleaseMutex(InfoboxMutex);
	ReleaseMutex(all_mutex);

	switch (uMsg)
	{
	case WM_CREATE:
		{
			wdo_add_window(hWnd);

			WaitForSingleObject(InfoboxMutex, INFINITE);
			WaitForSingleObject(all_mutex, INFINITE);

			/* ��ǰ���ڱ��Ĭ��Ϊ���һ�� */
			WinNumber = InfoboxCount - 1;
			all_number = all_quan - 1;

			/* ���浱ǰ���� */
			Infobox[WinNumber].HomeWin = hWnd;
			all_hwnd[all_number] = hWnd;

			lastWin = (HWND*)realloc(lastWin, InfoboxCount * sizeof(HWND));
			if (lastWin == NULL) wdo_error_ex();
			else lastWin[WinNumber] = ctwin;

			/* ���ö�ʱ�� */
			SetTimer(hWnd, (UINT_PTR)hWnd, FREQUENCY_SCREEN_REFRESH, NULL);

			/* �˳��ٽ��� */
			ReleaseMutex(InfoboxMutex);
			ReleaseMutex(all_mutex);

			return 0;
		}
	case WM_PAINT:
		{
			HDC hdc = GetDC(hWnd);

			/* �ͻ����� */
			RECT rect;
			GetClientRect(hWnd, &rect);

			/* ʹ��Ч������Ч���������ظ����� WM_PAINT */
			ValidateRect(hWnd, &rect);

			HBRUSH BackBrush = CreateSolidBrush(all_alias[all_number].BackColor);
			SelectObject(hdc, BackBrush);

			/* ˢ�»�������������Ӱ */
			PatBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);

			DeleteObject(BackBrush);
			ReleaseDC(hWnd, hdc);
		}
	case WM_TIMER:
		{
			/* ���ָ�򴰿ڷ������ģ����ػ洰�� */
			if (lastWin[WinNumber] != ctwin)
			{
				lastWin[WinNumber] = ctwin;
				return SendMessageA(hWnd, WM_PAINT, 0, 0);
			}

			WINDOWALLINFO info = { 0 };
			char output[300] = "\0";

			/* �ͻ����� */
			RECT rect;
			GetClientRect(hWnd, &rect);

			/* ��ȡ������Ϣ */
			lrGetWindowAllInfo(ctwin, &info);

			HDC hdc = GetDC(hWnd);

			/* ָ��λͼ */
			HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 0, 0);
			SelectObject(hdc, hBitmap);

			/* ���û��ģʽ */
			SetBkMode(hdc, OPAQUE);

			

			/* ��߿�������Ϊ 5 */
			rect.left += 5;

			WaitForSingleObject(all_mutex, INFINITE);
			/* �������� */
			SelectObject(hdc, all_alias[all_number].hFont);
			/* ����������ɫ */
			SetTextColor(hdc, all_alias[all_number].TextColor);
			/* ���ñ�����ɫ */
			SetBkColor(hdc, all_alias[all_number].BackColor);

			ReleaseMutex(all_mutex);


			/* ��ʼ���Ƹ�����Ϣ
			* 
			* ��Щ��Ϣĩβ�ռ����ո���Ϊ����Ǽ�ʱ����Ϣ��ֻ���ػ沿�֣�
			* Ϊ�˾���������Ӱ */

			char binaryStr[33];
			int height = all_alias[all_number].fontinfo.height + 5;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Window handle       |   _%X_(hex)  Or %u(decimal) %s",
				"���ھ��            |   _%X_(hex)  Or %u(decimal) %s"), info.ID, info.ID, info.PARENT ? "[CHILD]" : ""));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Process ID          |   _%X_(hex)  Or %u(decimal)",
				"���� ID             |   _%X_(hex)  Or %u(decimal)"
			), info.PID, info.PID));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Window title text   |   %s",
				"���ڱ����ı�        |   %s"
			), info.TITLE));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Window class name   |   %s",
				"��������            |   %s"
			), info.CLASS));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Process path        |   %s",
				"����·��            |   %s"
			), info.PPATH));
			rect.top += height;
			
			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Window vertex       |   X- %d  Y- %d (In Screen), and [%d, %d](In Parent)    ",
				"���ڶ���            |   X- %d  Y- %d (In Screen), and [%d, %d](In Parent)    "
			), info.VERTEX.left, info.VERTEX.top, info.CVERTEX.left, info.CVERTEX.top));
			rect.top += height;
			
			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Window size         |   W- %d  H- %d    ",
				"���ڴ�С            |   W- %d  H- %d    "
			), info.WINSIZE.cx, info.WINSIZE.cy));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Window code         |   %s",
				"���ڱ���            |   %s"
			), info.CODE == WINCODE_UNICODE ? "UNICODE" : "ASCII"));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Window DPI          |   W- %.2f  H- %.2f",
				"���� DPI            |   W- %.2f  H- %.2f"
			), info.DPIZOOM.fw, info.DPIZOOM.fh));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Window style        |   _%08X_(hex)  Or %032s(binary)",
				"���ڷ��            |   _%08X_(hex)  Or %032s(binary)"
			), info.STYLE, itoa(info.STYLE, binaryStr, 2)));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Window style ex     |   _%08X_(hex)  Or %032s(binary)",
				"������չ���        |   _%08X_(hex)  Or %032s(binary)"
			), info.STYLEEX, itoa(info.STYLEEX, binaryStr, 2)));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Window class style  |   _%08X_(hex)  Or %032s(binary)",
				"��������          |   _%08X_(hex)  Or %032s(binary)"
			), info.CLASSSTYLE, itoa(info.CLASSSTYLE, binaryStr, 2)));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Cursor position     |   X- %d  Y- %d    ",
				"�α�λ��            |   X- %d  Y- %d    "
			), info.CURPOS.x, info.CURPOS.y));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Cursor in window    |   X- %d  Y- %d    ",
				"�α��ڴ���������    |   X- %d  Y- %d    "
			), info.CURINWINPOS.x, info.CURINWINPOS.y));
			rect.top += height;

			TextOutA(hdc, rect.left, rect.top, output, sprintf(output, wdo_language(
				"Cursor point color  |   #%06X  Or (R- %u, G- %u, B- %u)      ",
				"�α����ɫ          |   #%06X  Or (R- %u, G- %u, B- %u)      "
			), lr_TO_RGB(info.CURCOLOR), GetRValue(info.CURCOLOR), GetGValue(info.CURCOLOR), GetBValue(info.CURCOLOR)));
			rect.top += height;


			/* ��ȡ��ɫֵ��������ˢ */
			HBRUSH BoxBrush = CreateSolidBrush(info.CURCOLOR);
			SelectObject(hdc, BoxBrush);
			HPEN BoxPen = CreatePen(PS_SOLID, 1, all_alias[all_number].TextColor);
			SelectObject(hdc, BoxPen);

			/* ������ɫ�� */
			Rectangle(hdc,
				rect.left
				, rect.top
				, rect.left + 50
				, rect.top + 50);

			rect.left += 55;
			DeleteObject(BoxBrush);
			DeleteObject(BoxPen);


			/* ����Ŀ�괰�ڵ�����ͼ */

			/* Ŀ�괰�ڵľ��� */
			RECT winrect;
			GetClientRect(ctwin, &winrect);

			if (ctwin != hWnd) {
				winrect.right = (LONG)(winrect.right * lrGetWindowDpiZoom(ctwin).fw);
				winrect.bottom = (LONG)(winrect.bottom * lrGetWindowDpiZoom(ctwin).fh);
			}
			HDC hdcDesk = GetDC(ctwin);
			

			/* ctwin �Ĵ��ڿ�� */
			double pw_done = (double)winrect.right - winrect.left;
			/* ctwin �Ĵ��ڸ߶� */
			double ph_done = (double)winrect.bottom - winrect.top;

			double dif = ph_done / pw_done;

			/* ���Ŀ�괰�ڴ�����Ϣ���� */
			while (pw_done > ((double)rect.right - rect.left) || ph_done > ((double)rect.bottom - rect.top))
			{
				/* ֱ�Ӳ���ѭ���ݼ��㷨 */
				--pw_done;
				ph_done -= dif;
			}

			/* ����λͼ����Ĭ�ϣ�����ͼƬʧ�� */
			SetStretchBltMode(hdc, HALFTONE);

			HBRUSH BackBrush = CreateSolidBrush(all_alias[all_number].BackColor);
			SelectObject(hdc, BackBrush);

			/* ˢ�¿�ȱ�� */
			PatBlt(hdc, (int)pw_done + rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);
			PatBlt(hdc, rect.left, (int)ph_done + rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);

			/* ��ʼ���� */
			StretchBlt(
				hdc, rect.left, rect.top, (int)pw_done, (int)ph_done
				, hdcDesk, winrect.left, winrect.top, winrect.right - winrect.left, winrect.bottom - winrect.top
				, SRCCOPY);

			/* �������û���ԭ�� */
			// SetBrushOrgEx(hdc, 0, 0, NULL);

			ReleaseDC(ctwin, hdcDesk);

			DeleteObject(BackBrush);
			/* ɾ��λͼ */
			DeleteObject(hBitmap);
			/* ɾ�� DC */
			ReleaseDC(hWnd, hdc);
			
			ReleaseMutex(all_mutex);
			return 0;
		}
	case WM_DESTROY:
		{
			wdo_del_window(hWnd);

			/* �����ٽ��� */
			WaitForSingleObject(InfoboxMutex, INFINITE);
			WaitForSingleObject(all_mutex, INFINITE);

			/* ɾ����ʱ�� */
			KillTimer(hWnd, (UINT_PTR)hWnd);

			/* �ͷŲ���Ҫ���ڴ� */

			/* ���ֻ��һ������ʱ */
			if (InfoboxCount == 1)
			{
				free((void*)Infobox);
				free(lastWin);
				lastWin = NULL;
				InfoboxCount = 0;

				CloseHandle(InfoboxMutex);
				InfoboxMutex = NULL;
			}
			/* �����һ�����ڵĽṹ��Ϣ���Ƶ���ǰ */
			else
			{
				--InfoboxCount;

				Infobox[WinNumber].HomeWin = Infobox[InfoboxCount].HomeWin;
				Infobox[WinNumber].infowin = Infobox[InfoboxCount].infowin;
				Infobox[WinNumber].type = Infobox[InfoboxCount].type;
				lastWin[WinNumber] = lastWin[InfoboxCount];

				ReleaseMutex(InfoboxMutex);
			}

			if (all_quan == 1)
			{
				all_quan = 0;

				lrClearAliasEx(all_alias, alias_type_box);

				free(all_alias);
				all_alias = NULL;

				free(all_hwnd);
				all_hwnd = NULL;

				CloseHandle(all_mutex);
				all_mutex = NULL;
			}
			else
			{
				--all_quan;

				lrClearAliasEx(&(all_alias[all_number]), alias_type_box);

				all_alias[all_number] = all_alias[all_quan];
				all_hwnd[all_number] = all_hwnd[all_quan];

				ReleaseMutex(all_mutex);
			}

			PostQuitMessage(0);

			return 0;
		}
	}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

/* lrCreateLogBox(): ��־����� */
static LRESULT CALLBACK lrCreateLogBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		wdo_add_window(hWnd);

		WaitForSingleObject(all_mutex, INFINITE);

		all_hwnd[all_quan - 1] = hWnd;

		RECT rect;
		GetClientRect(hWnd, &rect);

		HWND edit = CreateWindowExA(0
			, "Edit"
			, NULL
			, (WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL)
			| (ES_LEFT | ES_MULTILINE | ES_WANTRETURN)
			, rect.left + 65, rect.top + 10
			, rect.right - rect.left - 65, rect.bottom - rect.top - 10
			, hWnd
			, (HMENU)1
			, NULL, NULL
		);
		SendMessageA(edit, WM_SETFONT, (WPARAM)all_alias[all_quan - 1].hFont, 0);
		SendMessageA(edit, EM_LIMITTEXT, ULLONG_MAX, 0L);
		ShowWindow(edit, SW_SHOW);
		UpdateWindow(edit);

		ReleaseMutex(all_mutex);
		return 0;
	}
	case WM_PAINT:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		ValidateRect(hWnd, &rect);

		HDC hdc = GetDC(hWnd);
		HDC ImageDC = CreateCompatibleDC(hdc);

		WaitForSingleObject(all_mutex, INFINITE);
		size_t all_number = 0;
		for (ULONG i = 0; i < all_quan; ++i)  if (all_hwnd[i] == hWnd) { all_number = i; break; }
		HBRUSH BackBrush = CreateSolidBrush(all_alias[all_number].BackColor);
		SelectObject(hdc, BackBrush);
		PatBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);
		DeleteObject(BackBrush);
		ReleaseMutex(all_mutex);

		HBITMAP hBitmap = LoadBitmapA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDC_BITMAP__LOGBOX_IMAGE));
		SelectObject(ImageDC, hBitmap);

		SetStretchBltMode(hdc, HALFTONE);

		TransparentBlt(
			hdc, rect.left + 10, rect.top + 50, 50, 50,
			ImageDC, 0, 0, 50, 50,
			RGB(0xff, 0xff, 0xff));

		DeleteObject(hBitmap);
		ReleaseDC(hWnd, hdc);

		size_t count = 0;
		HWND* edit = lrGetChildWindowHwndSets(hWnd, &count);
		UpdateWindow(*edit);
		InvalidateRect(*edit, NULL, FALSE);
		free(edit);

		return 0;
	}
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);

		if (width < 500) width = 500;
		if (height < 500) height = 500;

		size_t count = 0;
		HWND* edit = lrGetChildWindowHwndSets(hWnd, &count);
		MoveWindow(*edit, 65, 10, width - 65, height - 10, TRUE);
		UpdateWindow(*edit);
		free(edit);

		return DefWindowProcA(hWnd, uMsg, wParam, (LPARAM)((LPARAM)width | ((LPARAM)height << 16)));
	}
	case WM_CTLCOLOREDIT:
	{
		WaitForSingleObject(all_mutex, INFINITE);

		size_t all_number = 0;
		for (ULONG i = 0; i < all_quan; ++i)  if (all_hwnd[i] == hWnd) { all_number = i; break; }


		SetTextColor((HDC)wParam, all_alias[all_number].TextColor);
		SetBkColor((HDC)wParam, all_alias[all_number].BackColor);

		enum BlackOrWhite { Black, White }BlackOrWhite = White;
		if (all_alias[all_number].BackColor == RGB(0x00, 0x00, 0x00)) BlackOrWhite = Black;

		ReleaseMutex(all_mutex);

		return (LRESULT)GetStockObject(BlackOrWhite == Black ? BLACK_BRUSH : (BlackOrWhite == White ? WHITE_BRUSH : NULL_BRUSH));
	}
	case WM_WDO_SETBOXTEXT:
	{
		size_t count = 0;
		HWND* edit = lrGetChildWindowHwndSets(hWnd, &count);

		LRESULT ret = SendMessageA(*edit, WM_SETTEXT, 0L, (LPARAM)wParam);
		SendMessageA(*edit, WM_PAINT, 0, 0);
		UpdateWindow(*edit);
		InvalidateRect(*edit, NULL, FALSE);
		free(edit);

		return ret;
	}
	case WM_DESTROY:
	{
		wdo_del_window(hWnd);
		WaitForSingleObject(all_mutex, INFINITE);

		size_t all_number = 0;

		for (ULONG i = 0; i < all_quan; ++i) {
			if (all_hwnd[i] == hWnd) {
				all_number = i;
				break;
			}
		}

		if (all_quan == 1)
		{
			all_quan = 0;

			lrClearAliasEx(all_alias, alias_type_box);

			free(all_alias);
			all_alias = NULL;

			free(all_hwnd);
			all_hwnd = NULL;

			CloseHandle(all_mutex);
			all_mutex = NULL;
		}
		else
		{
			--all_quan;

			lrClearAliasEx(&(all_alias[all_number]), alias_type_box);

			all_alias[all_number] = all_alias[all_quan];
			all_hwnd[all_number] = all_hwnd[all_quan];

			ReleaseMutex(all_mutex);
		}

		PostQuitMessage(0);
		return 0;
	}
	default: return DefWindowProcA(hWnd, uMsg, wParam, lParam);
	}
}

/* lrCreateImageBox(): ͼƬ����� */
static LRESULT CALLBACK lrCreateImageBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* ��ǰ�Ĵ��ڱ�� */
	ULONG WinNumber = 0;
	size_t all_number = 0;

	/* ��һ���α�������꣬���ڻ������� */
	static POINT* LastCursorClickCoord = NULL;

	/* �����ٽ��� */
	WaitForSingleObject(ImageboxMutex, INFINITE);
	WaitForSingleObject(all_mutex, INFINITE);

	/* ������ǰ���� */
	if (uMsg != WM_CREATE) {
		for (ULONG i = 0; i < ImageboxCount; ++i) {
			if (ImageboxHomeWin[i] == hWnd) {
				WinNumber = i;
				break;
			}
		}
		for (ULONG i = 0; i < all_quan; ++i) {
			if (all_hwnd[i] == hWnd) {
				all_number = i;
				break;
			}
		}
	}
	/* �˳��ٽ��� */
	ReleaseMutex(ImageboxMutex);
	ReleaseMutex(all_mutex);

	switch (uMsg)
	{
	case WM_CREATE:
		wdo_add_window(hWnd);

		WaitForSingleObject(ImageboxMutex, INFINITE);
		WaitForSingleObject(all_mutex, INFINITE);

		/* ��ǰ���ڱ��Ĭ��Ϊ���һ�� */
		WinNumber = ImageboxCount - 1;
		all_number = all_quan - 1;

		/* ���浱ǰ���� */
		ImageboxHomeWin[WinNumber] = hWnd;
		all_hwnd[all_number] = hWnd;

		ImageboxLastCurbyte[WinNumber] = 0;
		ImageboxLastCurpos[WinNumber] = (POINT){ 0L,0L };

		if ((LastCursorClickCoord = (POINT*)realloc(LastCursorClickCoord, ImageboxCount * sizeof(POINT))) == NULL)
		{
			wdo_error_ex();
		}
		else
		{
			/* ���Ϊ -1 ʱ����ôû����һ���������꣬���� WM_PAINT ��Ϣʱ���ض����� */
			LastCursorClickCoord[WinNumber] = (POINT){ -1,-1 };
		}
		ReleaseMutex(all_mutex);
		ReleaseMutex(ImageboxMutex);

		return 0;
	case WM_PAINT:

		WaitForSingleObject(ImageboxMutex, INFINITE);

		HDC hdc = GetDC(hWnd);
		HDC ImageDC = CreateCompatibleDC(hdc);

		/* �ͻ����� */
		RECT rect;
		GetClientRect(hWnd, &rect);

		/* ʹ��Ч������Ч���������ظ����� WM_PAINT */
		ValidateRect(hWnd, &rect);

		SelectObject(ImageDC, ImageboxImage[WinNumber]);
		SetStretchBltMode(hdc, HALFTONE);

		StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top
			, ImageDC, 0, 0, ImageboxImageSize[WinNumber].cx, ImageboxImageSize[WinNumber].cy, SRCCOPY);

		ReleaseDC(hWnd, hdc);
		DeleteDC(ImageDC);

		/* �ض����ߵĻ��� */
		LastCursorClickCoord[WinNumber] = (POINT){ -1,-1 };

		ReleaseMutex(ImageboxMutex);
		return 0;
	case WM_LBUTTONDOWN:
		WaitForSingleObject(ImageboxMutex, INFINITE);
		WaitForSingleObject(all_mutex, INFINITE);

		if (LastCursorClickCoord[WinNumber].x == -1 && LastCursorClickCoord[WinNumber].y == -1)
		{
			LastCursorClickCoord[WinNumber] = lrGetCursorInWindowPosition(hWnd);
		}
		else
		{
			HDC hdc = GetDC(hWnd);
			HPEN hPen = CreatePen(PS_DASHDOT, 1, all_alias[all_number].TextColor);
			SelectObject(hdc, hPen);

			/* ���ƶ������ߣ��������ֽ�ʹ�ñ���ɫ��� */
			SetBkMode(hdc, TRANSPARENT);

			MoveToEx(hdc, LastCursorClickCoord[WinNumber].x, LastCursorClickCoord[WinNumber].y, NULL);
			LastCursorClickCoord[WinNumber] = lrGetCursorInWindowPosition(hWnd);
			LineTo(hdc, LastCursorClickCoord[WinNumber].x, LastCursorClickCoord[WinNumber].y);

			DeleteObject(hPen);
			ReleaseDC(hWnd, hdc);
		}

		ReleaseMutex(ImageboxMutex);
		ReleaseMutex(all_mutex);
		return 0;
	case WM_RBUTTONDOWN:	// ����һ���ˢ�¸�λ
		WaitForSingleObject(ImageboxMutex, INFINITE);
		SetWindowPos(hWnd, 0, 0, 0, ImageboxImageSize[WinNumber].cx, ImageboxImageSize[WinNumber].cy, SWP_NOMOVE | SWP_NOZORDER);
		ReleaseMutex(ImageboxMutex);

		SendMessageA(hWnd, WM_PAINT, 0, 0);
		return 0;
	case WM_CHAR:			// �������һ���ַ����ӡ
	{
		WaitForSingleObject(ImageboxMutex, INFINITE);
		WaitForSingleObject(all_mutex, INFINITE);

		POINT curpos = lrGetCursorInWindowPosition(hWnd);

		/* ������ϴ��α�������� */
		if (!memcmp(&curpos, &ImageboxLastCurpos[WinNumber], sizeof(POINT)))
		{
			++ImageboxLastCurbyte[WinNumber];
		}
		else
		{
			ImageboxLastCurbyte[WinNumber] = 0;
		}

		HDC hdc = GetDC(hWnd);
		
		SelectObject(hdc, all_alias[all_number].hFont);

		SetTextColor(hdc, all_alias[all_number].TextColor);
		SetBkMode(hdc, TRANSPARENT);

		unsigned char word[2] = { '\0','\0' };
		word[0] = (unsigned char)wParam;

		/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		* ��Ϊ˫�ֽ���ĵ��ֽڵ�һλ������λΪ 1��
		* ���Խ�������һ����Ϣ���ȶԣ�
		* �ȶԳɹ������һ��˫�ֽ��룻
		-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
		if (word[0] & 0x80)
		{
			MSG msg;
			if (PeekMessageA(&msg, hWnd, WM_CHAR, WM_CHAR, PM_REMOVE | PM_QS_INPUT))
			{
				if (msg.message == WM_CHAR && msg.wParam & 0x80)
				{
					word[1] = (unsigned char)msg.wParam;
				}
			}
		}
		TEXTMETRIC fontinfo;
		GetTextMetricsA(hdc, &fontinfo);

		TextOutA(hdc
			, curpos.x + (ImageboxLastCurbyte[WinNumber] * fontinfo.tmMaxCharWidth)
			, (GetWindowLongA(hWnd, GWL_STYLE) & WS_CAPTION) ? curpos.y - GetSystemMetrics(SM_CYCAPTION) : curpos.y
			, word
			, sizeof(word));

		/* �������� */
		ImageboxLastCurpos[WinNumber] = curpos;

		ReleaseDC(hWnd, hdc);

		ReleaseMutex(ImageboxMutex);
		ReleaseMutex(all_mutex);
		return 0;
	}
	case WM_DESTROY:
		wdo_del_window(hWnd);

		/* �����ٽ��� */
		WaitForSingleObject(ImageboxMutex, INFINITE);
		WaitForSingleObject(all_mutex, INFINITE);

		/* �ͷŲ���Ҫ���ڴ� */

		/* ���ֻ��һ������ʱ */
		if (ImageboxCount == 1)
		{
			DeleteObject(ImageboxImage[WinNumber]);
			free(ImageboxHomeWin);
			free(ImageboxImageSize);
			free(ImageboxLastCurpos);
			free(ImageboxLastCurbyte);
			ImageboxCount = 0;

			free(LastCursorClickCoord);
			LastCursorClickCoord = NULL;

			CloseHandle(ImageboxMutex);
			ImageboxMutex = NULL;
		}
		/* �����һ�����ڵĽṹ��Ϣ���Ƶ���ǰ */
		else
		{
			ImageboxHomeWin[WinNumber] = ImageboxHomeWin[--ImageboxCount];
			DeleteObject(ImageboxImage[WinNumber]);
			ImageboxImage[WinNumber] = ImageboxImage[ImageboxCount];
			ImageboxImageSize[WinNumber] = ImageboxImageSize[ImageboxCount];
			ImageboxLastCurpos[WinNumber] = ImageboxLastCurpos[ImageboxCount];
			ImageboxLastCurbyte[WinNumber] = ImageboxLastCurbyte[ImageboxCount];

			LastCursorClickCoord[WinNumber] = LastCursorClickCoord[ImageboxCount];
			ReleaseMutex(ImageboxMutex);
		}

		if (all_quan == 1)
		{
			all_quan = 0;

			lrClearAliasEx(all_alias, alias_type_box);

			free(all_alias);
			free(all_hwnd);

			CloseHandle(all_mutex);
			all_mutex = NULL;
		}
		else
		{
			--all_quan;

			lrClearAliasEx(&(all_alias[all_number]), alias_type_box);

			all_alias[all_number] = all_alias[all_quan];
			all_hwnd[all_number] = all_hwnd[all_quan];

			ReleaseMutex(all_mutex);
		}
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

/* lrCreateBlueBox(): ��������� */
static LRESULT CALLBACK lrCreateBlusBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	size_t all_number = 0;
	WaitForSingleObject(all_mutex, INFINITE);

	if (uMsg != WM_CREATE) {
		for (ULONG i = 0; i < all_quan; ++i) {
			if (all_hwnd[i] == hWnd) {
				all_number = i;
				break;
			}
		}
	}
	ReleaseMutex(all_mutex);

	switch (uMsg)
	{
	case WM_CREATE:
		wdo_add_window(hWnd);

		WaitForSingleObject(all_mutex, INFINITE);

		all_hwnd[all_quan - 1] = hWnd;

		SetWindowLongA(hWnd, GWL_STYLE, GetWindowLongA(hWnd, GWL_STYLE) & ~WS_BORDER & ~WS_CAPTION & ~WS_SIZEBOX);

		ShowCursor(FALSE);

		/* ���������� */
		ShowWindow(FindWindowA("Shell_TrayWnd", NULL), SW_HIDE);

		ReleaseMutex(all_mutex);
		return 0;

	case WM_TIMER:
	{
		if (wParam == (WPARAM)hWnd)
		{
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
			SetForegroundWindow(hWnd);
			SetFocus(hWnd);
			SetWindowLongA(hWnd, GWL_STYLE, GetWindowLongA(hWnd, GWL_STYLE) & ~WS_BORDER & ~WS_CAPTION & ~WS_SIZEBOX);
		}
		return 0;
	}
	case WM_PAINT:
	{
		WaitForSingleObject(all_mutex, INFINITE);

		RECT rect;
		GetClientRect(hWnd, &rect);

		ValidateRect(hWnd, &rect);

		HDC hdc = GetDC(hWnd);

		SetBkMode(hdc, TRANSPARENT);

		HBRUSH hBrush = CreateSolidBrush(all_alias[all_number].BackColor);
		SelectObject(hdc, hBrush);

		/* ˢ�� */
		PatBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);

		DeleteObject(hBrush);


		/* �������� */
		SelectObject(hdc, all_alias[all_number].hFont);
		/* ����������ɫ */
		SetTextColor(hdc, all_alias[all_number].TextColor);

		if (lrstrcnt(BlueboxText, '\n') > 1)
			DrawTextA(hdc, BlueboxText, -1, &rect, DT_LEFT | DT_TOP);
		else
			DrawTextA(hdc, BlueboxText, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		ReleaseMutex(all_mutex);
		return 0;
	}
	case WM_KILLFOCUS:
	case WM_STYLECHANGED:
	{
		PostMessageA(hWnd, WM_TIMER, (WPARAM)hWnd, 0);
		return 0;
	}
	case WM_WDO_SETBOXTEXT:
	{
		WaitForSingleObject(all_mutex, INFINITE);

		char* p = (char*)wParam;
		if (!p) return FALSE;

		if (BlueboxText) free(BlueboxText);
		if ((BlueboxText = (char*)malloc(strlen(p) + 1)) == NULL)
		{
			/* BlueboxText �����ڴ�֮ǰ�����Ϊ NULL �����ͷ��ڴ棬���뽫����� */
			BlueboxText = NULL;
			ReleaseMutex(all_mutex);

			/* ������ı�ָ���ڴ�����ʧ�ܣ������ٴ��ڣ����򽫶�ηǷ����� */
			SendMessageA(hWnd, WM_CLOSE, 0, 0);
			return FALSE;
		}
		memcpy(BlueboxText, p, strlen(p) + 1);

		ReleaseMutex(all_mutex);
		return TRUE;
	}
	case WM_DESTROY:
	{
		wdo_del_window(hWnd);

		WaitForSingleObject(all_mutex, INFINITE);

		/* ��ʾ������ */
		ShowWindow(FindWindowA("Shell_TrayWnd", NULL), SW_SHOWNORMAL);

		if (all_quan == 1)
		{
			all_quan = 0;

			lrClearAliasEx(all_alias, alias_type_box);

			free(all_alias);
			free(all_hwnd);

			CloseHandle(all_mutex);
			all_mutex = NULL;
		}
		else
		{
			--all_quan;

			lrClearAliasEx(&(all_alias[all_number]), alias_type_box);

			all_alias[all_number] = all_alias[all_quan];
			all_hwnd[all_number] = all_hwnd[all_quan];

			ReleaseMutex(all_mutex);
		}

		free(BlueboxText);
		BlueboxText = NULL;

		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}
