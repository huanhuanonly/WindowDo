// wdo_string.c -- String manipulation of lrWindowDo()
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lrWindowDo.h"
#include "../Head/lr_wdo.h"

#pragma warning (disable : 6308)
#pragma warning (disable : 6031)


#define STR_EXPAND	1024

/* 用于读取文件内容，一次读取一条命令，然后传给 lrWindowDo()。
*
* 参数:
*		fp            |  已打开的文件指针
*
* 返回值:
*		返回读取到的命令字符串，一般情况下为一行，需要释放。
*		花括号括起来的也算一行。
*/
char* wdo_fgetline(FILE* fp, BOOL IsUnicodeFile)
{
	if (fp == NULL) return NULL;

	if (IsUnicodeFile)
	{
		/* 长度 */
		size_t len = STR_EXPAND;

		/* 内容 */
		wchar_t* str = (wchar_t*)malloc(len * sizeof(wchar_t));

		if (str == NULL) return NULL;

		/* 大括号的数量，为 0 时结束读取 */
		int bq = 0;

		size_t pos = 0;

	W_START_READ:

		while (!feof(fp))
		{
			/* 扩展内存 */
			if (pos >= len)
			{
				len += STR_EXPAND;

				wchar_t* strTemp = realloc(str, len * sizeof(wchar_t));

				if (strTemp) str = strTemp;
				else
				{
					len -= STR_EXPAND;
					break;
				}
			}

			str[pos++] = fgetwc(fp);

			if (str[pos - 1] == L'{') ++bq;

			else if (str[pos - 1] == L'}') --bq;

			else if (str[pos - 1] == L'\n' && bq <= 0) break;
		}
		/* 检查下一行内容 */
		while (!feof(fp))
		{
			wchar_t ch = fgetwc(fp);

			if (ch == L'{') {
				fseek(fp, -1, SEEK_CUR);
				goto W_START_READ;
			}

			else
				if (ch != L' '
					&& ch != L'\t'
					&& ch != L'\n'
					&& ch != L'\r'
					&& ch != L'\0'
					) {

					fseek(fp, -1, SEEK_CUR);
					break;
				}
		}
		/* feof() 会多读取一次 */
		if (pos) --pos;

		/* 加上结束符 */
		str[pos] = '\0';

		if (pos == 0) goto W_DONE;

		/* 去掉开头无效字符 */
		for (UINT i = 0; i < pos; ++i)
		{
			if (str[i] != L' '
				&& str[i] != L'\t'
				&& str[i] != L'\n'
				&& str[i] != L'\r'
				&& str[i] != L'\0'
				) {
				lrstrmov((char*)str, i * sizeof(wchar_t), '\0', pos * sizeof(wchar_t));
				break;
			}
		}
		while (--pos)
		{
			if (str[pos] != L' '
				&& str[pos] != L'\t'
				&& str[pos] != L'\n'
				&& str[pos] != L'\r'
				&& str[pos] != L'\0'
				) {

				str[++pos] = L'\0';
				break;
			}
		}

	W_DONE:
		;
		/* 将 Unicode 转 Ansi */
		size_t count = wcslen(str) + 1;
		char* mbs = (char*)malloc(count);
		wcstombs(mbs, str, count);
		free(str);
		return mbs;
	}
	else    // IsUnicodeFile
	{
		/* 长度 */
		size_t len = STR_EXPAND;

		/* 内容 */
		char* str = (char*)malloc(len);

		if (str == NULL) return NULL;

		/* 大括号的数量，为 0 时结束读取 */
		int bq = 0;

		size_t pos = 0;

	A_START_READ:

		while (!feof(fp))
		{
			/* 扩展内存 */
			if (pos >= len)
			{
				len += STR_EXPAND;

				char* strTemp = realloc(str, len);

				if (strTemp) str = strTemp;
				else
				{
					len -= STR_EXPAND;
					break;
				}
			}

			str[pos++] = fgetc(fp);

			if (str[pos - 1] == '{') ++bq;

			else if (str[pos - 1] == '}') --bq;

			else if (str[pos - 1] == '\n' && bq <= 0) break;
		}
		/* 检查下一行内容 */
		while (!feof(fp))
		{
			char ch = fgetc(fp);

			if (ch == '{') {
				fseek(fp, -1, SEEK_CUR);
				goto A_START_READ;
			}

			else
				if (ch != ' '
					&& ch != '\t'
					&& ch != '\n'
					&& ch != '\r'
					&& ch != '\0'
					) {

					fseek(fp, -1, SEEK_CUR);
					break;
				}
		}
		/* feof() 会多读取一次 */
		if (pos) --pos;

		/* 加上结束符 */
		str[pos] = '\0';

		if (pos == 0) goto A_DONE;

		/* 去掉开头无效字符 */
		for (UINT i = 0; i < pos; ++i)
		{
			if (str[i] != ' '
				&& str[i] != '\t'
				&& str[i] != '\n'
				&& str[i] != '\r'
				&& str[i] != '\0'
				) {
				lrstrmov(str, i, '\0', pos);
				break;
			}
		}
		while (--pos)
		{
			if (str[pos] != ' '
				&& str[pos] != '\t'
				&& str[pos] != '\n'
				&& str[pos] != '\r'
				&& str[pos] != '\0'
				) {

				str[++pos] = '\0';
				break;
			}
		}

	A_DONE:

		return str;
	}
}

#undef STR_EXPAND


void wdo_divided_command(const char* const _Command, char** _Instruction, char** _Parameter)
{
	if (_Command == NULL || _Command[0] == '\0')
	{
		if (*_Instruction = (char*)malloc(1)) (*_Instruction)[0] = '\0';
		if (*_Parameter = (char*)malloc(1)) (*_Parameter)[0] = '\0';

		return;
	}

	BOOL start = FALSE;		// 跳过开头的空格和制表符
	size_t pos = 0;

	for (size_t i = 0;; ++i)
	{
		if (_Command[i] == ' ' || _Command[i] == '\t' || _Command[i] == '\n')
		{
			if (start)
			{
				pos = i;
				break;
			}
		}
		else if (_Command[i] == '\0')
		{
			pos = i;
			break;
		}
		else start = TRUE;
	}
	*_Instruction = (char*)malloc(pos + 1);
	*_Parameter = (char*)malloc(strlen(_Command) + 1);

	lrstrsep(_Command, 2, *_Instruction, *_Parameter);

	return;
}



/* 分割花括号中的字符串内容（如果有）。
*
* 参数:
*		_string     |  原字符串，两端需要是花括号
*		_StrsSize   |  接收返回的一级指针数量
*
* 返回值:
*		如果成功，返回一个二级指针，装载着分割后的字符串组。需要释放。
*		失败返回 NULL。
*
* 言论:
*		使用换行分割字符串，并会去掉花括号、换行、以及两端的空格和制表符。
*		可以调用 lrfrees() 释放内存。
*/
char** wdo_curlybrace_splitstr(const char* const _string, size_t* _StrsSize)
{
	/* 检查 _string 是否为空 */
	if (_string == NULL || _string[0] == '\0')
	{
		*_StrsSize = 0;
		return NULL;
	}
	else *_StrsSize = 1;

	/* 如果没有大括号聚合，则返回值使用 _string 填充，依旧需要释放 */
	if ((_string[0] != '{' || _string[strlen(_string) - 1] != '}')
		|| (_string[0] == '{' && _string[1] == '}'))
	{
		char** _strs = (char**)malloc((*_StrsSize) * sizeof(char*));
		if (_strs == NULL) return NULL;

		_strs[0] = (char*)malloc(strlen(_string) + 1);
		if (_strs[0] == NULL)
		{
			free(_strs);
			return NULL;
		}
		strcpy(_strs[0], _string);
		return _strs;
	}

	/* _string 的长度，省略末尾的 '}' */
	size_t StringLength = strlen(_string) - 1;

	/* 当前的字符串位置，省略开头 '{' */
	size_t pos = 1;

	/* StrsSize 的大小为 = 换行数量 + 1 */
	for (size_t i = pos; i < StringLength; ++i) if (_string[i] == '\n' || _string[i] == '{') ++(*_StrsSize);

	/* 为 strs 分配初始内存 */
	char** _strs = (char**)malloc((*_StrsSize) * sizeof(char*));
	if (_strs == NULL) return NULL;

	/* 单次分配的内存大小 */
#define STR_TEMP_LEN 100

	/* 正在赋值的行编号 */
	size_t StrsSizeNumber = 0;

	for (; StrsSizeNumber < (*_StrsSize) && pos < StringLength; ++StrsSizeNumber)
	{
		size_t StrTempLen = STR_TEMP_LEN;

		char* StrTemp = (char*)malloc(StrTempLen);
		if (StrTemp == NULL) return NULL;

		/* 去除开头的无用内容，通过位移 pos */
		for (; _string[pos] == ' ' || _string[pos] == '\t' || _string[pos] == '\n'; ++pos);

		/* 如果位移过后到达最大长度，则退出当前迭代 */
		if (pos >= StringLength)
		{
			free(StrTemp);
			break;
		}

		size_t i = 0;
		for (; pos < StringLength; ++pos)
		{
			/* 长度达到最大值时，扩展内存 */
			if (i >= StrTempLen)
			{
				StrTempLen += STR_TEMP_LEN;
				char* NewStrTemp = (char*)realloc(StrTemp, StrTempLen);
				if (NewStrTemp != NULL) StrTemp = NewStrTemp;
				else wdo_error_ex();
			}

			/* 如果中间遇到大括号则读取完整括号中的内容，并和上一次的内容合并(如果有)，
			* 这么做是因为大括号中间很可能以嵌套的方式放着一个命令块，大括号内放的是参数区，
			* 而参数不能和指令分开 */
			if (_string[pos] == '{')
			{
				/* 是否与上一次内容合并 */
				BOOL type = (i == 0 && StrsSizeNumber > 0 && _strs[StrsSizeNumber - 1][strlen(_strs[StrsSizeNumber - 1]) - 1] != '}') ? TRUE : FALSE;

				/* 遇到的大括号数量，支持再次嵌套，层层分离 */
				size_t Braces = 0;

				for (; pos < StringLength; ++pos)
				{
					if (i >= StrTempLen)
					{
						StrTempLen += STR_TEMP_LEN;
						char* NewStrTemp = (char*)realloc(StrTemp, StrTempLen);
						if (NewStrTemp != NULL) StrTemp = NewStrTemp;
						else wdo_error_ex();
					}

					StrTemp[i++] = _string[pos];

					if (_string[pos] == '{') ++Braces;
					else if (_string[pos] == '}')
					{
						--Braces;
						if (Braces == 0)
						{
							++pos;
							break;
						}
					}
				}
				StrTemp[i] = '\0';

				if (type)
				{
					/* 为上一次的内容扩展内存 */
					_strs[StrsSizeNumber - 1] = (char*)realloc(_strs[StrsSizeNumber - 1], strlen(_strs[StrsSizeNumber - 1]) + strlen(StrTemp) + 1);
					if (_strs[StrsSizeNumber - 1] == NULL) wdo_error_ex();
					else
					{
						/* 直接合并到上一次内容的末尾，释放此次的内存指针 */
						strcat(_strs[StrsSizeNumber - 1], StrTemp);
						free(StrTemp);
					}
					--StrsSizeNumber;
				}
				else _strs[StrsSizeNumber] = StrTemp;

				goto end;
			}
			else if (_string[pos] != '\n') StrTemp[i++] = _string[pos];
			else
			{
				StrTemp[i] = '\0';

				/* 将末尾的无用内容清除 */
				while (StrTemp[strlen(StrTemp) - 1] == ' ' || StrTemp[strlen(StrTemp) - 1] == '\t')
					StrTemp[strlen(StrTemp) - 1] = '\0';

				/* 将 StrTemp 存入 strs 中 */
				_strs[StrsSizeNumber] = StrTemp;

				++pos; break;
			}
		}
		/* 如果是最后一次 */
		if (StrsSizeNumber >= ((*_StrsSize) - 1) || pos >= StringLength)
		{
			StrTemp[i] = '\0';

			/* 将末尾的无用内容清除 */
			while (StrTemp[strlen(StrTemp) - 1] == ' ' || StrTemp[strlen(StrTemp) - 1] == '\t')
				StrTemp[strlen(StrTemp) - 1] = '\0';

			/* 将 StrTemp 存入 strs 中 */
			_strs[StrsSizeNumber] = StrTemp;

			++StrsSizeNumber; break;
		}
	end:
		continue;
	}

#undef STR_TEMP_LEN

	/* 校准实际一级指针数量，
	* 因为前面的值是总字符串中的换行数量 + 1，是假定的最大值 */
	* _StrsSize = StrsSizeNumber;

	return _strs;
}




/* 分割花括号中的数组内容（如果有）。
*
* 参数:
*		_string     |  原字符串，两端需要是花括号
*		_IntsSize   |  接收返回的一级指针数量
*
* 返回值:
*		如果成功，返回一个二级指针，装载着分割后的数组。需要释放。
*		失败返回 NULL，只有内存分配失败才会返回 NULL。
*
* 言论:
*		横向使用空格和制表符分隔，竖向使用换行和 ',' 分割字符串。
*		每一个一级指针指向 INT64_OUTPUT_MAX_LEN 个 __int64 数值，
*		可以调用 lrfrees() 释放内存。
*
*		如果 _string 为 NULL，则返回一个空数组，同样需要释放。
*/
__int64** wdo_curlybrace_splitint(const char* const _string, size_t* _IntsSize)
{
	if (_IntsSize == NULL) return NULL;

	/* 如果 _string 为空则返回空数组，同样需要释放 */
	if (_string == NULL || _string[0] == '\0')
	{
		*_IntsSize = 0;

		__int64** _ints = (__int64**)malloc(sizeof(__int64*));
		if (_ints)
		{
			_ints[0] = (__int64*)malloc(INT64_OUTPUT_MAX_LEN * sizeof(__int64));
			if (_ints[0])
			{
				memset(_ints[0], 0, INT64_OUTPUT_MAX_LEN * sizeof(__int64));
				*_IntsSize = 1;
				return _ints;
			}
		}
		return NULL;
	}
	else *_IntsSize = 1;

	/* 如果 _string 中没有一个有效数值则返回为空 */
	BOOL tf = FALSE;
	for (size_t i = 0; i < strlen(_string); ++i)
	{
		if (_string[i] != ' ' && _string[i] != '\t'
			&& _string[i] != '{' && _string[i] != '}'
			&& _string[i] != ',' && _string[i] != '\n'
			&& _string[i] != '\r') {
			tf = TRUE;
			break;
		}
	}
	if (!tf)
	{
		*_IntsSize = 0;

		__int64** _ints = (__int64**)malloc(sizeof(__int64*));
		if (_ints)
		{
			_ints[0] = (__int64*)malloc(INT64_OUTPUT_MAX_LEN * sizeof(__int64));
			if (_ints[0])
			{
				memset(_ints[0], 0, INT64_OUTPUT_MAX_LEN * sizeof(__int64));
				*_IntsSize = 1;
				return _ints;
			}
		}
		return NULL;
	}

	/* 如果没有大括号聚合，则只返回一个一级指针，同样排好对应数组 */
	if (_string[0] != '{' || _string[strlen(_string) - 1] != '}')
	{
		__int64** _ints = (__int64**)malloc((*_IntsSize) * sizeof(__int64*));
		if (_ints == NULL) return NULL;

		_ints[0] = (__int64*)malloc(INT64_OUTPUT_MAX_LEN * sizeof(__int64));
		if (_ints[0] == NULL) return NULL;
		memset(_ints[0], 0, INT64_OUTPUT_MAX_LEN * sizeof(__int64));


		char StrTemp[INT64_INPUT_MAX_LEN];
		memset(StrTemp, '\0', sizeof(StrTemp));

		for (size_t i = 0, j = 0, IntsTempNowLen = 0; i < strlen(_string) && IntsTempNowLen < INT64_OUTPUT_MAX_LEN; ++i)
		{
			if (_string[i] != ' ' && _string[i] != '\t' && _string[i] != '\0')
				StrTemp[j < INT64_INPUT_MAX_LEN ? j++ : INT64_INPUT_MAX_LEN] = _string[i];

			if (((_string[i] == ' ' || _string[i] == '\t') && _string[i + 1] != ' ' && _string[i + 1] != '\t') || _string[i + 1] == '\0')
			{
				StrTemp[j < INT64_INPUT_MAX_LEN ? j : INT64_INPUT_MAX_LEN] = '\0';

				_ints[0][IntsTempNowLen++] = atoll(StrTemp);

				/* 刷新 StrTemp */
				j = 0;
				memset(StrTemp, '\0', INT64_INPUT_MAX_LEN);
			}
		}
		return _ints;
	}

	/* 过滤后的 _string */
	char* newstr = (char*)malloc(strlen(_string) + 1);
	if (newstr == NULL) return NULL;
	else
	{
		size_t len = strlen(_string) + 1;

		memset(newstr, '\0', len);

		for (size_t i = 0, j = 0; i < len; ++i)
		{
			/* 将多个连续的空格和制表符替换成一个空格 */
			if (_string[i] == ' ' || _string[i] == '\t') {
				if (_string[i + 1] != ' ' && _string[i + 1] != '\t')
					newstr[j++] = ' ';
			}

			/* 将多个连续的 ',' 和换行替换成一个换行 */
			else if (_string[i] == ',' || _string[i] == '\n') {
				newstr[j++] = '\n';

				/* 跳过 ',' 和换行后面的空格和制表符 */
				while (_string[i + 1] == ',' || _string[i + 1] == '\n'
					|| _string[i + 1] == ' ' || _string[i + 1] == '\t')
					++i;
			}

			else newstr[j++] = _string[i];
		}
	}


	/* newstr 的长度，省略末尾的 '}' */
	size_t StringLength = strlen(newstr) - 1;

	/* 当前的字符串位置，省略开头 '{' */
	size_t pos = 1;

	/* StrsSize 的大小为 = 逗号\换行数量 + 1，如果有两个连续的 逗号\换行 则只记录一个 */
	for (size_t i = pos; i < StringLength; ++i)
		if ((newstr[i] == ',' || newstr[i] == '\n')
			&& (newstr[i + 1] != ',' && newstr[i + 1] != '\n' && newstr[i + 1] != '}')) ++(*_IntsSize);

	__int64** _ints = (__int64**)malloc((*_IntsSize) * sizeof(__int64*));
	if (_ints == NULL) return NULL;

	/* 正在赋值的行编号 */
	size_t IntsHeightNumber = 0;

	for (; IntsHeightNumber < (*_IntsSize) && pos < StringLength; ++IntsHeightNumber)
	{
		/* 转换为数值前需要使用字符串当载体 */
		char StrTemp[INT64_INPUT_MAX_LEN];
		memset(StrTemp, '\0', sizeof(StrTemp));

		/* 为 _ints 分配行内存，此处假定最大值 INT64_OUTPUT_MAX_LEN，不再进行内存扩展 */
		_ints[IntsHeightNumber] = (__int64*)malloc(INT64_OUTPUT_MAX_LEN * sizeof(__int64));
		memset(_ints[IntsHeightNumber], 0, INT64_OUTPUT_MAX_LEN * sizeof(__int64));

		/* StrTempNowLen 现在的 StrTemp 的字符串长度，IntsWidthNumber _ints 的行宽度(单位：指针数) */
		size_t StrTempNowLen = 0, IntsWidthNumber = 0;

		/* 为了 newstr[pos + 1] 能读取到 '\0' 所以此处 StringLength + 1 */
		for (; pos < StringLength + 1; ++pos)
		{
			/* 如果 newstr 等于换行时结束一次一级存储 */
			if (newstr[pos] == '\n' || newstr[pos + 1] == '\0')
			{
				/* 如果 StrTemp 中有数据时 */
				if (StrTemp[0] != '\0')
				{
					/* 末尾补 '\0' */
					StrTemp[StrTempNowLen < INT64_INPUT_MAX_LEN ? StrTempNowLen : INT64_INPUT_MAX_LEN] = '\0';

					/* 将 StrTemp 转化为数值放入 _ints */
					_ints[IntsHeightNumber][IntsWidthNumber < INT64_OUTPUT_MAX_LEN ? IntsWidthNumber : INT64_OUTPUT_MAX_LEN] = atoll(StrTemp);
				}
				++pos;
				break;
			}

			/* 不等于空格时，将数据放入 StrTemp */
			if (newstr[pos] != ' ') StrTemp[StrTempNowLen < INT64_INPUT_MAX_LEN ? StrTempNowLen++ : INT64_INPUT_MAX_LEN] = newstr[pos];

			/* 如果遇到空格则进行一次对 _ints 的存储 */
			else if (StrTemp[0] != '\0')
			{
				StrTemp[StrTempNowLen < INT64_INPUT_MAX_LEN ? StrTempNowLen : INT64_INPUT_MAX_LEN] = '\0';

				_ints[IntsHeightNumber][IntsWidthNumber < INT64_OUTPUT_MAX_LEN ? IntsWidthNumber++ : INT64_OUTPUT_MAX_LEN] = atoll(StrTemp);

				/* 清空 StrTemp */
				StrTempNowLen = 0;
				memset(StrTemp, '\0', sizeof(StrTemp));
			}
		}
	}
	return _ints;
}




unsigned __int64 wdo_getloopcount(char* _Ins)
{
	if (_Ins == NULL || _Ins[0] == '\0') return 0;

	char temp[INT64_INPUT_MAX_LEN] = "\0";
	size_t len = strlen(_Ins);

	for (size_t i = 0; i < len; ++i)
	{
		if (_Ins[i] == WDO_LOOPCOUNT_CHAR)
		{
			for (size_t j = 0, k = i + 1; j < INT64_INPUT_MAX_LEN; ++j, ++k)
			{
				temp[j] = _Ins[k];

				if (_Ins[k] == '\0') break;
			}

			_Ins[i] = '\0';

			if (!stricmp(temp, WDO_LOOPCOUNT_STR)) return WDO_LOOPCOUNT_ID;

			else
			{
				unsigned __int64 n = 0;
				sscanf(temp, "%llu", &n);
				return n ? n : 1;
			}
		}
	}

	return 1;
}




BOOL wdo_string_to_hotkey(const char* const _HotkeyString, UINT* _Modifiers, UINT* _VirtKey)
{
	if (_HotkeyString == NULL || _HotkeyString[0] == '\0') return FALSE;

	size_t len = strlen(_HotkeyString);
	BOOL bVk = FALSE;

	char VirtKeyString[5] = { '\0' };

	for (size_t i = 0; i < len; ++i)
	{
		switch (_HotkeyString[i])
		{
		case 'A':
		case 'a':
		{
			if (_Modifiers) *_Modifiers |= MOD_ALT;
			break;
		}
		case 'C':
		case 'c':
		{
			if (_Modifiers) *_Modifiers |= MOD_CONTROL;
			break;
		}
		case 'S':
		case 's':
		{
			if (_Modifiers) *_Modifiers |= MOD_SHIFT;
			break;
		}
		case 'W':
		case 'w':
		{
			if (_Modifiers) *_Modifiers |= MOD_WIN;
			break;
		}
		case '+':
		{
			bVk = TRUE;

			++i;
			for (size_t j = 0; i < len && j < sizeof(VirtKeyString) - 1; ++i, ++j)
			{
				VirtKeyString[j] = _HotkeyString[i];
			}
			break;
		}
		default: return FALSE;
		}
	}
	if (bVk == FALSE) return FALSE;
	
	/* Use space key by default */
	if (VirtKeyString[0] == '\0')
	{
		if (_VirtKey) *_VirtKey = VK_SPACE;
		return TRUE;
	}

	if (VirtKeyString[1] == '\0')
	{
		/* See https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes */

		if (isdigit(VirtKeyString[0]))
		{
			if (_VirtKey) *_VirtKey = VirtKeyString[0];
			return TRUE;
		}
		else if (isalpha(VirtKeyString[0]))
		{
			if (_VirtKey) *_VirtKey = toupper(VirtKeyString[0]);
			return TRUE;
		}
		else switch (VirtKeyString[0])
		{
		case '-':
			if (_VirtKey) *_VirtKey = VK_OEM_MINUS; // VK_SUBTRACT
			return TRUE;
		case '=':
			if (_VirtKey) *_VirtKey = VK_OEM_PLUS;
			return TRUE;
		case ',':
			if (_VirtKey) *_VirtKey = VK_OEM_COMMA;
			return TRUE;
		case '.':
			if (_VirtKey) *_VirtKey = VK_OEM_PERIOD;
			return TRUE;
		case ';':
			if (_VirtKey) *_VirtKey = VK_OEM_1;
			return TRUE;
		case '/':
			if (_VirtKey) *_VirtKey = VK_OEM_2; // VK_DIVIDE
			return TRUE;
		case '`':
			if (_VirtKey) *_VirtKey = VK_OEM_3;
			return TRUE;
		case '[':
			if (_VirtKey) *_VirtKey = VK_OEM_4;
			return TRUE;
		case '\\':
			if (_VirtKey) *_VirtKey = VK_OEM_5;
			return TRUE;
		case ']':
			if (_VirtKey) *_VirtKey = VK_OEM_6;
			return TRUE;
		case '\'':
			if (_VirtKey) *_VirtKey = VK_OEM_7;
			return TRUE;

		default: return FALSE;
		}
	}
	else
	{
		if (VirtKeyString[0] == 'F' || VirtKeyString[0] == 'f')
		{
			if (_VirtKey)
			{
				*_VirtKey = (VK_F1 - 1) + atoi(&(VirtKeyString[1]));

				if (*_VirtKey < VK_F1 || *_VirtKey > VK_F24) return FALSE;

				return TRUE;
			}
		}
		else return FALSE;
	}
	return FALSE;
}