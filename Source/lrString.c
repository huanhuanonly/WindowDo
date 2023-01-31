// lrString.c -- string manipulation
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#ifndef LR_STRING_C
#define LR_STRING_C


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#pragma warning (disable : 6308)
#pragma warning (disable : 6031)
#pragma warning (disable : 4996)

/* 分割字符串为多个段。
* 
* 参数:
*		_String     |  原字符串
*		_SplitCount |  分割数量，也是段的数量
*		...         |  接收分割后的段字符串
* 
* 言论:
*		使用一个或多个空格或制表符或换行分割字符串，除非使用花括号括起来。
*		因为不确定 _String 分割后的每段长度，所以接收的载体长度尽量不要小于 _String 的长度。
*		如果分割后的段数大于 _SplitCount 指定的数量，则将多余的段数字符串拼接到末尾。
*/
void lrstrsep(const char* const _String, size_t _SplitCount, ...)
{
	if (_String == NULL || _String[0] == '\0' || _SplitCount == 0) return;
	
	/* 当前的可变参指针 */
	va_list arg;
	va_start(arg, _SplitCount);

	/* 记录当前 _String 读取位置 */
	size_t pos = 0;
	/* 记录 _String 长度 */
	size_t len = strlen(_String) + 1;

	/* 当前操作的可变参指针 */
	char* str = NULL;

	for (size_t i = 0; i < _SplitCount; ++i)
	{
		if (pos < len)
		{
			/* 记录花括号数量 */
			int bq = 0;

			/* 获取当前载体指针 */
			str = va_arg(arg, char*);

			/* 过滤开头不需要的字符 */
			while (_String[pos] == ' '
				|| _String[pos] == '\t'
				|| _String[pos] == '\n'
				|| _String[pos] == '\r'
				) {
				++pos;
			}

			for (size_t j = 0; pos < len; ++j, ++pos)
			{
				if (_String[pos] == '{') ++bq;
				else if (_String[pos] == '}') --bq;

				/* 如果则是第一个左花括号且不在第一个字符，也将分段 */
				if (_String[pos] == '{'
					&& j > 0
					&& bq == 1
					) {
					str[j] = '\0';
					break;
				}
				/* 如果这是最后一个右花括号，也将分段 */
				else if (_String[pos] == '}'
					&& bq <= 0
					) {
					++pos;
					str[j] = '}';
					str[j + 1] = '\0';
					break;
				}
				/* 以下都是分隔符，没有遇到则正常赋值 */
				else if (_String[pos] != '\n'
					&& _String[pos] != '\r'
					&& _String[pos] != ' '
					&& _String[pos] != '\t'
					&& _String[pos] != '\0'
					) {
					str[j] = _String[pos];
				}
				/* 如果遇到分隔符，同时不在花括号内，则分段 */
				else if (bq <= 0) {
					str[j] = '\0';
					break;
				}
				/* 如果这是最后一个字符则补 '\0' 后退出 */
				else if (pos + 1 == len) {
					str[j] = '\0';
					break;
				}
				/* 如果在花括号中，则将分隔符也正常写入 */
				else
				{
					str[j] = _String[pos];
				}
			}
		}
		/* 如果长度封顶，则直接赋空值 */
		else
		{
			if (str != NULL) str[0] = '\0';
			continue;
		}
	}
	if (str != NULL) {
		/* 如果载体（可变参数）不够，则将剩余所有数据装在最后一个可变参数载体 */
		for (size_t i = strlen(str); pos < len; ++i, ++pos)
		{
			str[i] = _String[pos];
		}
		/* 清空末尾的无用数据 */
		if (strlen(str)) {
			for (size_t i = strlen(str) - 1; i; --i)
			{
				if (str[i] == ' '
					|| str[i] == '\t'
					|| str[i] == '\n'
					|| str[i] == '\r'
					) {
					str[i] = '\0';
				}
				else break;
			}
		}
	}

	va_end(arg);
	return;
}


/* 左偏移字符串。
* 
* 参数:
*		_str        |  字符串，不需要以 '\0' 结尾
*		_mov        |  字符串往左偏移的字符数量
*		_val        |  用于填补空缺字符
*		_maxlen     |  需要偏移的字符串长度，不应大于 _str 的实际长度
* 
* 返回值:
*		操作成功，返回偏移后的 _str。
*		操作失败，返回 NULL。
*/
char* lrstrmov(char* _str, int _mov, char _val, size_t _maxlen)
{
	if (_str == NULL) return NULL;
	else if (_mov == 0 || _maxlen == 0) return _str;


	if (_mov > 0)
	{
		if (_mov > _maxlen)
		{
			memset(_str, _val, _maxlen);
		}
		else
		{
			for (int i = 0; i < _maxlen - _mov; ++i)
			{
				_str[i] = _str[_mov + i];
			}
			for (int i = 0; i < _mov; ++i)
			{
				_str[_maxlen - 1 - i] = _val;
			}
		}
	}
	else
	{
		_mov = -_mov;

		if (_mov > _maxlen)
		{
			memset(_str, _val, _maxlen);
		}
		else
		{
			char* carrier = (char*)malloc(_maxlen);
			if (carrier == NULL) return NULL;

			memcpy(carrier, _str, _maxlen);

			for (int i = 0; i < _mov; ++i)
			{
				_str[i] = _val;
			}
			for (int i = 0; i < _maxlen - _mov ; ++i)
			{
#pragma warning (suppress : 6385)
				_str[_mov + i] = carrier[i];
			}
			free(carrier);
		}
	}

	return _str;
}


/* 去除字符串两端指定的字符（如果有）。
* 
* 参数:
*		_str        |  字符串
*		_ch1        |  要去除的左端字符
*		_ch2        |  要去除的右端字符
* 
* 返回值:
*		返回 _str。
* 
* 言论:
*		如果只有一端有此字符则不会更改。
*/
char* lrstrrem(char* _str, char _ch1, char _ch2)
{
	if (_str == NULL || _str[0] == '\0') return _str;

	size_t len = strlen(_str);

	if (_str[0] == _ch1 && _str[len - 1] == _ch2)
	{
		_str[len - 1] = '\0';

		lrstrmov(_str, 1, '\0', len);
	}
	return _str;
}



/* 获取字符串中的指定字符数量。
*
* 参数:
*		_str        |  源字符串
*		_ch         |  要查找的字符
*
* 返回值:
*		返回 _str 中的 _ch 数量。
*/
unsigned int lrstrcnt(const char* const _str, char _ch)
{
	if (_str == NULL || _str[0] == '\0') return 0;
	else if (_ch == '\0') return 1;

	size_t len = strlen(_str) + 1;

	unsigned int ret = 0;

	for (size_t i = 0; i < len; ++i)
	{
		if (_str[i] == _ch) ++ret;
		else continue;
	}
	return ret;
}



/* 从字符串中提取十六进制转为十进制。
* 
* 参数:
*		_hexadecimal  |  带有十六进制的字符串
* 
* 返回值:
*		返回转换过后的十进制数。
*/
unsigned __int64 lrHexToDec(const char* const  _hexadecimal)
{
	/* 新的十六进制载体（过滤掉无用字符后） */
	char new_hexadecimal[20];

	memset(new_hexadecimal, '\0', sizeof(new_hexadecimal));

	/* 最终的十进制载体 */
	unsigned __int64 decimal = 0;

	if (new_hexadecimal != NULL)
	{
		unsigned short int i = 0, j = 0;
		char ch = 0;

		for (; i < 20; ++i)
		{
			/* 将 ch 转化为大写字母 */
			ch = _hexadecimal[i];
			ch = toupper(ch);

			if (ch >= '0' && ch <= 'F')
			{
				new_hexadecimal[j++] = ch;
			}
			else if (ch == '\0') break;
		}

		new_hexadecimal[j] = '\0';
		sscanf(new_hexadecimal, "%llX", &decimal);
	}

	return decimal;
}


/* 将字符串中的 '\n' 替换成 "\r\n"
* 返回 _newString 的长度
*/
size_t lrStringLinebreakMode(const char* const _String, char** _newString)
{
	if (_String == NULL || _String[0] == '\0') return 0;
	if (_newString == NULL) return 0;

	/* 这里采用更快的方法获取字符串长度和换行数量，避免多次调用 strlen() */

	size_t len = 0;
	unsigned int count = 0;

	while (1)
	{
		if (_String[len] == '\n') ++count;
		else if (_String[len] == '\0') break;

		++len;
	}
	if ((*_newString = (char*)malloc(len + count + 1)) == NULL) return 0;

	size_t ret = 0;

	for (size_t i = 0; i < len; ++i)
	{
		if (_String[i] == '\n') (*_newString)[ret++] = '\r';

		(*_newString)[ret++] = _String[i];
	}
	(*_newString)[ret] = '\0';
	return ret;
}


#endif		// LR_STRING_C