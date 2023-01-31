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

/* �ָ��ַ���Ϊ����Ρ�
* 
* ����:
*		_String     |  ԭ�ַ���
*		_SplitCount |  �ָ�������Ҳ�Ƕε�����
*		...         |  ���շָ��Ķ��ַ���
* 
* ����:
*		ʹ��һ�������ո���Ʊ�����зָ��ַ���������ʹ�û�������������
*		��Ϊ��ȷ�� _String �ָ���ÿ�γ��ȣ����Խ��յ����峤�Ⱦ�����ҪС�� _String �ĳ��ȡ�
*		����ָ��Ķ������� _SplitCount ָ�����������򽫶���Ķ����ַ���ƴ�ӵ�ĩβ��
*/
void lrstrsep(const char* const _String, size_t _SplitCount, ...)
{
	if (_String == NULL || _String[0] == '\0' || _SplitCount == 0) return;
	
	/* ��ǰ�Ŀɱ��ָ�� */
	va_list arg;
	va_start(arg, _SplitCount);

	/* ��¼��ǰ _String ��ȡλ�� */
	size_t pos = 0;
	/* ��¼ _String ���� */
	size_t len = strlen(_String) + 1;

	/* ��ǰ�����Ŀɱ��ָ�� */
	char* str = NULL;

	for (size_t i = 0; i < _SplitCount; ++i)
	{
		if (pos < len)
		{
			/* ��¼���������� */
			int bq = 0;

			/* ��ȡ��ǰ����ָ�� */
			str = va_arg(arg, char*);

			/* ���˿�ͷ����Ҫ���ַ� */
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

				/* ������ǵ�һ���������Ҳ��ڵ�һ���ַ���Ҳ���ֶ� */
				if (_String[pos] == '{'
					&& j > 0
					&& bq == 1
					) {
					str[j] = '\0';
					break;
				}
				/* ����������һ���һ����ţ�Ҳ���ֶ� */
				else if (_String[pos] == '}'
					&& bq <= 0
					) {
					++pos;
					str[j] = '}';
					str[j + 1] = '\0';
					break;
				}
				/* ���¶��Ƿָ�����û��������������ֵ */
				else if (_String[pos] != '\n'
					&& _String[pos] != '\r'
					&& _String[pos] != ' '
					&& _String[pos] != '\t'
					&& _String[pos] != '\0'
					) {
					str[j] = _String[pos];
				}
				/* ��������ָ�����ͬʱ���ڻ������ڣ���ֶ� */
				else if (bq <= 0) {
					str[j] = '\0';
					break;
				}
				/* ����������һ���ַ��� '\0' ���˳� */
				else if (pos + 1 == len) {
					str[j] = '\0';
					break;
				}
				/* ����ڻ������У��򽫷ָ���Ҳ����д�� */
				else
				{
					str[j] = _String[pos];
				}
			}
		}
		/* ������ȷⶥ����ֱ�Ӹ���ֵ */
		else
		{
			if (str != NULL) str[0] = '\0';
			continue;
		}
	}
	if (str != NULL) {
		/* ������壨�ɱ��������������ʣ����������װ�����һ���ɱ�������� */
		for (size_t i = strlen(str); pos < len; ++i, ++pos)
		{
			str[i] = _String[pos];
		}
		/* ���ĩβ���������� */
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


/* ��ƫ���ַ�����
* 
* ����:
*		_str        |  �ַ���������Ҫ�� '\0' ��β
*		_mov        |  �ַ�������ƫ�Ƶ��ַ�����
*		_val        |  �������ȱ�ַ�
*		_maxlen     |  ��Ҫƫ�Ƶ��ַ������ȣ���Ӧ���� _str ��ʵ�ʳ���
* 
* ����ֵ:
*		�����ɹ�������ƫ�ƺ�� _str��
*		����ʧ�ܣ����� NULL��
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


/* ȥ���ַ�������ָ�����ַ�������У���
* 
* ����:
*		_str        |  �ַ���
*		_ch1        |  Ҫȥ��������ַ�
*		_ch2        |  Ҫȥ�����Ҷ��ַ�
* 
* ����ֵ:
*		���� _str��
* 
* ����:
*		���ֻ��һ���д��ַ��򲻻���ġ�
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



/* ��ȡ�ַ����е�ָ���ַ�������
*
* ����:
*		_str        |  Դ�ַ���
*		_ch         |  Ҫ���ҵ��ַ�
*
* ����ֵ:
*		���� _str �е� _ch ������
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



/* ���ַ�������ȡʮ������תΪʮ���ơ�
* 
* ����:
*		_hexadecimal  |  ����ʮ�����Ƶ��ַ���
* 
* ����ֵ:
*		����ת�������ʮ��������
*/
unsigned __int64 lrHexToDec(const char* const  _hexadecimal)
{
	/* �µ�ʮ���������壨���˵������ַ��� */
	char new_hexadecimal[20];

	memset(new_hexadecimal, '\0', sizeof(new_hexadecimal));

	/* ���յ�ʮ�������� */
	unsigned __int64 decimal = 0;

	if (new_hexadecimal != NULL)
	{
		unsigned short int i = 0, j = 0;
		char ch = 0;

		for (; i < 20; ++i)
		{
			/* �� ch ת��Ϊ��д��ĸ */
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


/* ���ַ����е� '\n' �滻�� "\r\n"
* ���� _newString �ĳ���
*/
size_t lrStringLinebreakMode(const char* const _String, char** _newString)
{
	if (_String == NULL || _String[0] == '\0') return 0;
	if (_newString == NULL) return 0;

	/* ������ø���ķ�����ȡ�ַ������Ⱥͻ��������������ε��� strlen() */

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