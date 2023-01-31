// wdo_log.c -- Output log of lrWindowDo()
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lr_wdo.h"
#include "../Head/lrWindowDo.h"

#pragma warning (disable : 6387)

static HANDLE mutex;


/* 打印日志信息。
* 
* 参数:
*		_Ins        |  当前的指令字符串。可以为 NULL
*		_State      |  当前的状态。值为 OUTLOG_NORMAL 等
*		_Format     |  格式化字符串
*		...         |  可选参数
*/
void wdo_log(const char* const _Ins, BYTE _State, const char* const _Format, ...)
{
	if (mutex == NULL) mutex = CreateMutexA(NULL, FALSE, NULL);
	WaitForSingleObject(mutex, OUTLOG_WAIT);


	if (_State == OUTLOG_NORMAL)
	{
		wdo_setcolor(OUTLOG_NORMAL_COLOR);
	}
	else if (_State == OUTLOG_ERROR)
	{
		wdo_setcolor(OUTLOG_ERROR_COLOR);
	}

	if (_Format != NULL && _Format[0] != '\0')
	{
		va_list arg;
		va_start(arg, _Format);

		char buffer[5000];
		vsnprintf(buffer, sizeof(buffer), _Format, arg);
		buffer[sizeof(buffer) - 1] = '\0';
		
		wdo_puts(buffer);

		va_end(arg);
	}
	
	ReleaseMutex(mutex);

	return;
}



void wdo_log_error(const char* const _function)
{
	LPVOID ErrorText = NULL;
	DWORD ErrorCode = GetLastError();

	if (ErrorCode) wdo_setcolor(OUTLOG_ERROR_COLOR);
	else wdo_setcolor(OUTLOG_NORMAL_COLOR);

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		ErrorCode,
		lrERRORTEXT_LANGUAGE,
		(LPSTR)&ErrorText,
		0,
		NULL
	);
	if (_function)
	{
		char* out = (char*)malloc(strlen(ErrorText) + strlen(_function) + 50);
		if (out)
		{
			sprintf(out, "~%s failed with error %lu: %s\n", _function, ErrorCode, (char*)ErrorText);
			wdo_puts(out);
			free(out);
		}
	}
	else
	{
		char* out = (char*)malloc(strlen(ErrorText) + 50);
		if (out)
		{
			sprintf(out, "ErrorCode: %lu\nExplain: %s\n", ErrorCode, (char*)ErrorText);
			wdo_puts(out);
			free(out);
		}
	}

	LocalFree(ErrorText);
	return;
}


/* 打印窗口简要信息。
* 
* 参数:
*		_hWnd       |  窗口句柄
*		_child      |  如果为 TRUE 则加上前缀
*/
void wdo_log_win_brief(HWND _hWnd, BOOL _child)
{
	if (mutex == NULL) mutex = CreateMutexA(NULL, FALSE, NULL);
	WaitForSingleObject(mutex, OUTLOG_WAIT);

	if (IsWindow(_hWnd))
	{
		char WinClass[MAXLEN_WIN_CLASS], WinTitle[MAXLEN_WIN_TITLE];

		UINT WinClassLen = RealGetWindowClassA(_hWnd, WinClass, MAXLEN_WIN_CLASS);
		int  WinTitleLen = GetWindowTextA(_hWnd, WinTitle, MAXLEN_WIN_TITLE);

		for (UINT i = 0; i < WinClassLen; ++i) if (WinClass[i] == '\n' || WinClass[i] == '\r') { WinClass[i] = '\0'; break; }
		for (int  i = 0; i < WinTitleLen; ++i) if (WinTitle[i] == '\n' || WinTitle[i] == '\r') { WinTitle[i] = '\0'; break; }

		if (_child) if (GetParent(_hWnd)) wdo_log(NULL, OUTLOG_NORMAL, wdo_language("C", "子"));

		wdo_log(NULL
			, OUTLOG_NORMAL
			, wdo_language("WIN = _%X_  >  CLASS = %s  >  TITLE = %s", "窗口 = _%X_  >  类名 = %s  >  标题 = %s") "\n"
			, _hWnd, WinClass, WinTitle);
	}
	else
	{
		wdo_log(NULL
			, OUTLOG_NORMAL
			, wdo_language("WIN = _%X_  >  CLASS = %s  >  TITLE = %s", "窗口 = _%X_  >  类名 = %s  >  标题 = %s") "\n"
			, _hWnd, " ", " ");
	}

	ReleaseMutex(mutex);
	return;
}



/* 打印帮助日志中的所有指令集合。
* 
* 参数:
*		_inss       |  所有指令
* 
* 返回值:
*		返回指令数量。
*/
unsigned int wdo_log_help_all(const char* const _inss)
{
	if (mutex == NULL) mutex = CreateMutexA(NULL, FALSE, NULL);
	WaitForSingleObject(mutex, OUTLOG_WAIT);

	unsigned int ret = 0;

	size_t len = strlen(_inss);

	wdo_putc(' ');
	wdo_putc(' ');

	BOOL Space = FALSE;
	BOOL ColorMode = FALSE;

	for (size_t i = 0, j = 1; i < len; ++i, ++j)
	{
		if (Space == FALSE && _inss[i] == ' ')
		{
			Space = TRUE;

			for (size_t k = j; k < 22; ++k) wdo_putc(' ');

			while (_inss[i] == ' ') ++i;
		}
		if (_inss[i] & 0x80)
		{
			char s[3] = { '\0' };
			s[0] = _inss[i];
			s[1] = _inss[++i];
			s[2] = '\0';
			wdo_puts(s);
		}
		else wdo_putc(_inss[i]);

		if (_inss[i] == '\n')
		{
			Space = FALSE;
			j = 0;

			wdo_putc(' ');
			wdo_putc(' ');

			++ret;
		}
	}

	ReleaseMutex(mutex);
	return ret;
}


#define help_ifiswchar(s, number)		if (s[number] & 0x80) { char temp_##s[3]; temp_##s[0] = s[number]; temp_##s[1] = s[++number]; temp_##s[2] = '\0'; wdo_puts(temp_##s); } else wdo_putc(s[number])

/* 打印帮助日志。
* 
* 参数:
*		_function     |  描述功能
*		_Syntax       |  描述语法
*		_Parameters   |  描述参数
*		_RetrieveAlias|  描述别名
*		_Remarks      |  描述言论
*/
void wdo_log_help(const char* const _function, const char* const _Syntax, const char* const _Parameters, const char* const _RetrieveAlias, const char* const _Remarks, const char* const _Examples)
{
	if (mutex == NULL) mutex = CreateMutexA(NULL, FALSE, NULL);
	WaitForSingleObject(mutex, OUTLOG_WAIT);

	wdo_putc('\n');

	unsigned __int64 number = 0;

	wdo_puts(wdo_language("function:\n", "功能:\n"));

	if (_function && _function[0])
	{
		if (_function[0] != '\n') wdo_putc('\n');

		wdo_putc('\t');
		while (_function[number])
		{
			help_ifiswchar(_function, number);

			if (_function[number] == '\n') {
				wdo_putc('\t');
			}
			++number;
		}
		wdo_putc('\n');
	}
	else wdo_puts(wdo_language("\n\tNo description for the time being.\n", "\n\t暂无描述。\n"));


	number = 0;


	wdo_puts(wdo_language("Syntax:\n", "语法:\n"));

	if (_Syntax && _Syntax[0])
	{
		if (_Syntax[0] != '\n') wdo_putc('\n');

		wdo_putc('\t');
		while (_Syntax[number])
		{
			help_ifiswchar(_Syntax, number);

			if (_Syntax[number] == '\n') {
				wdo_putc('\t');
			}
			++number;
		}
		wdo_putc('\n');
	}
	else wdo_puts(wdo_language("\n\tNo description for the time being.\n", "\n\t暂无描述。\n"));



	wdo_puts(wdo_language("Parameters:\n", "参数:\n"));

	if (_Parameters && _Parameters[0])
	{
		if (_Parameters[0] != '\n') wdo_putc('\n');

		number = 0;

		wdo_putc('\t');
		while (_Parameters[number])
		{
			help_ifiswchar(_Parameters, number);

			if (_Parameters[number] == '\n') {
				wdo_putc('\t');
			}
			++number;
		}
		wdo_putc('\n');
	}
	else wdo_puts(wdo_language("\n\tNo parameters.\n", "\n\t无参数。\n"));


	wdo_puts(wdo_language("RetrieveAlias:\n", "检索别名:\n"));

	if (_RetrieveAlias && _RetrieveAlias[0])
	{
		if (_RetrieveAlias[0] != '\n') wdo_putc('\n');

		number = 0;

		BOOL ExpandSpaces = TRUE;

		/* 用于字符对齐 */
		UINT pos = 0;

		wdo_putc('\t');
		while (_RetrieveAlias[number])
		{
			++pos;

			if (_RetrieveAlias[number] == ' ' && ExpandSpaces == TRUE) {
				for (UINT i = pos - 1; i < 24 /* 24 是对齐的长度 */; i++) wdo_putc(' ');
				ExpandSpaces = FALSE;
			}
			else
			{
				help_ifiswchar(_RetrieveAlias, number);
			}

			if (_RetrieveAlias[number] == '\n') {
				wdo_putc('\t');
				ExpandSpaces = TRUE;
				pos = 0;
			}
			++number;
		}
		wdo_putc('\n');
	}
	else wdo_puts(wdo_language("\n\tThis instruction does not need to bring in any aliases.\n", "\n\t该指令不需要带入任何别名。\n"));


	wdo_puts(wdo_language("Remarks:\n", "言论:\n"));

	if (_Remarks && _Remarks[0])
	{
		if (_Remarks[0] != '\n') wdo_putc('\n');

		number = 0;

		wdo_putc(' ');
		wdo_putc(' ');
		while (_Remarks[number])
		{
			help_ifiswchar(_Remarks, number);

			if (_Remarks[number] == '\n') {
				wdo_putc(' ');
				wdo_putc(' ');
			}
			++number;
		}
		wdo_putc('\n');
	}
	else wdo_puts(wdo_language("\n\tNULL.\n", "\n\t暂无言论。\n"));



	if (_Examples && _Examples[0])
	{
		wdo_puts(wdo_language("Examples:\n", "示例:\n"));

		if (_Examples[0] != '\n') wdo_putc('\n');

		number = 0;

		wdo_putc(' ');
		wdo_putc(' ');
		while (_Examples[number])
		{
			help_ifiswchar(_Examples, number);

			if (_Examples[number] == '\n') {
				wdo_putc(' ');
				wdo_putc(' ');
			}
			++number;
		}
		wdo_putc('\n');
	}

	ReleaseMutex(mutex);
	return;
}

#undef help_ifiswchar