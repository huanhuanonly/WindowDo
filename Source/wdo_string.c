// wdo_string.c -- String manipulation of lrWindowDo()
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lrWindowDo.h"
#include "../Head/lr_wdo.h"

#pragma warning (disable : 6308)
#pragma warning (disable : 6031)


#define STR_EXPAND	1024

/* ���ڶ�ȡ�ļ����ݣ�һ�ζ�ȡһ�����Ȼ�󴫸� lrWindowDo()��
*
* ����:
*		fp            |  �Ѵ򿪵��ļ�ָ��
*
* ����ֵ:
*		���ض�ȡ���������ַ�����һ�������Ϊһ�У���Ҫ�ͷš�
*		��������������Ҳ��һ�С�
*/
char* wdo_fgetline(FILE* fp, BOOL IsUnicodeFile)
{
	if (fp == NULL) return NULL;

	if (IsUnicodeFile)
	{
		/* ���� */
		size_t len = STR_EXPAND;

		/* ���� */
		wchar_t* str = (wchar_t*)malloc(len * sizeof(wchar_t));

		if (str == NULL) return NULL;

		/* �����ŵ�������Ϊ 0 ʱ������ȡ */
		int bq = 0;

		size_t pos = 0;

	W_START_READ:

		while (!feof(fp))
		{
			/* ��չ�ڴ� */
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
		/* �����һ������ */
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
		/* feof() ����ȡһ�� */
		if (pos) --pos;

		/* ���Ͻ����� */
		str[pos] = '\0';

		if (pos == 0) goto W_DONE;

		/* ȥ����ͷ��Ч�ַ� */
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
		/* �� Unicode ת Ansi */
		size_t count = wcslen(str) + 1;
		char* mbs = (char*)malloc(count);
		wcstombs(mbs, str, count);
		free(str);
		return mbs;
	}
	else    // IsUnicodeFile
	{
		/* ���� */
		size_t len = STR_EXPAND;

		/* ���� */
		char* str = (char*)malloc(len);

		if (str == NULL) return NULL;

		/* �����ŵ�������Ϊ 0 ʱ������ȡ */
		int bq = 0;

		size_t pos = 0;

	A_START_READ:

		while (!feof(fp))
		{
			/* ��չ�ڴ� */
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
		/* �����һ������ */
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
		/* feof() ����ȡһ�� */
		if (pos) --pos;

		/* ���Ͻ����� */
		str[pos] = '\0';

		if (pos == 0) goto A_DONE;

		/* ȥ����ͷ��Ч�ַ� */
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

	BOOL start = FALSE;		// ������ͷ�Ŀո���Ʊ��
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



/* �ָ�����е��ַ������ݣ�����У���
*
* ����:
*		_string     |  ԭ�ַ�����������Ҫ�ǻ�����
*		_StrsSize   |  ���շ��ص�һ��ָ������
*
* ����ֵ:
*		����ɹ�������һ������ָ�룬װ���ŷָ����ַ����顣��Ҫ�ͷš�
*		ʧ�ܷ��� NULL��
*
* ����:
*		ʹ�û��зָ��ַ���������ȥ�������š����С��Լ����˵Ŀո���Ʊ����
*		���Ե��� lrfrees() �ͷ��ڴ档
*/
char** wdo_curlybrace_splitstr(const char* const _string, size_t* _StrsSize)
{
	/* ��� _string �Ƿ�Ϊ�� */
	if (_string == NULL || _string[0] == '\0')
	{
		*_StrsSize = 0;
		return NULL;
	}
	else *_StrsSize = 1;

	/* ���û�д����žۺϣ��򷵻�ֵʹ�� _string ��䣬������Ҫ�ͷ� */
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

	/* _string �ĳ��ȣ�ʡ��ĩβ�� '}' */
	size_t StringLength = strlen(_string) - 1;

	/* ��ǰ���ַ���λ�ã�ʡ�Կ�ͷ '{' */
	size_t pos = 1;

	/* StrsSize �Ĵ�СΪ = �������� + 1 */
	for (size_t i = pos; i < StringLength; ++i) if (_string[i] == '\n' || _string[i] == '{') ++(*_StrsSize);

	/* Ϊ strs �����ʼ�ڴ� */
	char** _strs = (char**)malloc((*_StrsSize) * sizeof(char*));
	if (_strs == NULL) return NULL;

	/* ���η�����ڴ��С */
#define STR_TEMP_LEN 100

	/* ���ڸ�ֵ���б�� */
	size_t StrsSizeNumber = 0;

	for (; StrsSizeNumber < (*_StrsSize) && pos < StringLength; ++StrsSizeNumber)
	{
		size_t StrTempLen = STR_TEMP_LEN;

		char* StrTemp = (char*)malloc(StrTempLen);
		if (StrTemp == NULL) return NULL;

		/* ȥ����ͷ���������ݣ�ͨ��λ�� pos */
		for (; _string[pos] == ' ' || _string[pos] == '\t' || _string[pos] == '\n'; ++pos);

		/* ���λ�ƹ��󵽴���󳤶ȣ����˳���ǰ���� */
		if (pos >= StringLength)
		{
			free(StrTemp);
			break;
		}

		size_t i = 0;
		for (; pos < StringLength; ++pos)
		{
			/* ���ȴﵽ���ֵʱ����չ�ڴ� */
			if (i >= StrTempLen)
			{
				StrTempLen += STR_TEMP_LEN;
				char* NewStrTemp = (char*)realloc(StrTemp, StrTempLen);
				if (NewStrTemp != NULL) StrTemp = NewStrTemp;
				else wdo_error_ex();
			}

			/* ����м��������������ȡ���������е����ݣ�������һ�ε����ݺϲ�(�����)��
			* ��ô������Ϊ�������м�ܿ�����Ƕ�׵ķ�ʽ����һ������飬�������ڷŵ��ǲ�������
			* ���������ܺ�ָ��ֿ� */
			if (_string[pos] == '{')
			{
				/* �Ƿ�����һ�����ݺϲ� */
				BOOL type = (i == 0 && StrsSizeNumber > 0 && _strs[StrsSizeNumber - 1][strlen(_strs[StrsSizeNumber - 1]) - 1] != '}') ? TRUE : FALSE;

				/* �����Ĵ�����������֧���ٴ�Ƕ�ף������� */
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
					/* Ϊ��һ�ε�������չ�ڴ� */
					_strs[StrsSizeNumber - 1] = (char*)realloc(_strs[StrsSizeNumber - 1], strlen(_strs[StrsSizeNumber - 1]) + strlen(StrTemp) + 1);
					if (_strs[StrsSizeNumber - 1] == NULL) wdo_error_ex();
					else
					{
						/* ֱ�Ӻϲ�����һ�����ݵ�ĩβ���ͷŴ˴ε��ڴ�ָ�� */
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

				/* ��ĩβ������������� */
				while (StrTemp[strlen(StrTemp) - 1] == ' ' || StrTemp[strlen(StrTemp) - 1] == '\t')
					StrTemp[strlen(StrTemp) - 1] = '\0';

				/* �� StrTemp ���� strs �� */
				_strs[StrsSizeNumber] = StrTemp;

				++pos; break;
			}
		}
		/* ��������һ�� */
		if (StrsSizeNumber >= ((*_StrsSize) - 1) || pos >= StringLength)
		{
			StrTemp[i] = '\0';

			/* ��ĩβ������������� */
			while (StrTemp[strlen(StrTemp) - 1] == ' ' || StrTemp[strlen(StrTemp) - 1] == '\t')
				StrTemp[strlen(StrTemp) - 1] = '\0';

			/* �� StrTemp ���� strs �� */
			_strs[StrsSizeNumber] = StrTemp;

			++StrsSizeNumber; break;
		}
	end:
		continue;
	}

#undef STR_TEMP_LEN

	/* У׼ʵ��һ��ָ��������
	* ��Ϊǰ���ֵ�����ַ����еĻ������� + 1���Ǽٶ������ֵ */
	* _StrsSize = StrsSizeNumber;

	return _strs;
}




/* �ָ�����е��������ݣ�����У���
*
* ����:
*		_string     |  ԭ�ַ�����������Ҫ�ǻ�����
*		_IntsSize   |  ���շ��ص�һ��ָ������
*
* ����ֵ:
*		����ɹ�������һ������ָ�룬װ���ŷָ������顣��Ҫ�ͷš�
*		ʧ�ܷ��� NULL��ֻ���ڴ����ʧ�ܲŻ᷵�� NULL��
*
* ����:
*		����ʹ�ÿո���Ʊ���ָ�������ʹ�û��к� ',' �ָ��ַ�����
*		ÿһ��һ��ָ��ָ�� INT64_OUTPUT_MAX_LEN �� __int64 ��ֵ��
*		���Ե��� lrfrees() �ͷ��ڴ档
*
*		��� _string Ϊ NULL���򷵻�һ�������飬ͬ����Ҫ�ͷš�
*/
__int64** wdo_curlybrace_splitint(const char* const _string, size_t* _IntsSize)
{
	if (_IntsSize == NULL) return NULL;

	/* ��� _string Ϊ���򷵻ؿ����飬ͬ����Ҫ�ͷ� */
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

	/* ��� _string ��û��һ����Ч��ֵ�򷵻�Ϊ�� */
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

	/* ���û�д����žۺϣ���ֻ����һ��һ��ָ�룬ͬ���źö�Ӧ���� */
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

				/* ˢ�� StrTemp */
				j = 0;
				memset(StrTemp, '\0', INT64_INPUT_MAX_LEN);
			}
		}
		return _ints;
	}

	/* ���˺�� _string */
	char* newstr = (char*)malloc(strlen(_string) + 1);
	if (newstr == NULL) return NULL;
	else
	{
		size_t len = strlen(_string) + 1;

		memset(newstr, '\0', len);

		for (size_t i = 0, j = 0; i < len; ++i)
		{
			/* ����������Ŀո���Ʊ���滻��һ���ո� */
			if (_string[i] == ' ' || _string[i] == '\t') {
				if (_string[i + 1] != ' ' && _string[i + 1] != '\t')
					newstr[j++] = ' ';
			}

			/* ����������� ',' �ͻ����滻��һ������ */
			else if (_string[i] == ',' || _string[i] == '\n') {
				newstr[j++] = '\n';

				/* ���� ',' �ͻ��к���Ŀո���Ʊ�� */
				while (_string[i + 1] == ',' || _string[i + 1] == '\n'
					|| _string[i + 1] == ' ' || _string[i + 1] == '\t')
					++i;
			}

			else newstr[j++] = _string[i];
		}
	}


	/* newstr �ĳ��ȣ�ʡ��ĩβ�� '}' */
	size_t StringLength = strlen(newstr) - 1;

	/* ��ǰ���ַ���λ�ã�ʡ�Կ�ͷ '{' */
	size_t pos = 1;

	/* StrsSize �Ĵ�СΪ = ����\�������� + 1����������������� ����\���� ��ֻ��¼һ�� */
	for (size_t i = pos; i < StringLength; ++i)
		if ((newstr[i] == ',' || newstr[i] == '\n')
			&& (newstr[i + 1] != ',' && newstr[i + 1] != '\n' && newstr[i + 1] != '}')) ++(*_IntsSize);

	__int64** _ints = (__int64**)malloc((*_IntsSize) * sizeof(__int64*));
	if (_ints == NULL) return NULL;

	/* ���ڸ�ֵ���б�� */
	size_t IntsHeightNumber = 0;

	for (; IntsHeightNumber < (*_IntsSize) && pos < StringLength; ++IntsHeightNumber)
	{
		/* ת��Ϊ��ֵǰ��Ҫʹ���ַ��������� */
		char StrTemp[INT64_INPUT_MAX_LEN];
		memset(StrTemp, '\0', sizeof(StrTemp));

		/* Ϊ _ints �������ڴ棬�˴��ٶ����ֵ INT64_OUTPUT_MAX_LEN�����ٽ����ڴ���չ */
		_ints[IntsHeightNumber] = (__int64*)malloc(INT64_OUTPUT_MAX_LEN * sizeof(__int64));
		memset(_ints[IntsHeightNumber], 0, INT64_OUTPUT_MAX_LEN * sizeof(__int64));

		/* StrTempNowLen ���ڵ� StrTemp ���ַ������ȣ�IntsWidthNumber _ints ���п��(��λ��ָ����) */
		size_t StrTempNowLen = 0, IntsWidthNumber = 0;

		/* Ϊ�� newstr[pos + 1] �ܶ�ȡ�� '\0' ���Դ˴� StringLength + 1 */
		for (; pos < StringLength + 1; ++pos)
		{
			/* ��� newstr ���ڻ���ʱ����һ��һ���洢 */
			if (newstr[pos] == '\n' || newstr[pos + 1] == '\0')
			{
				/* ��� StrTemp ��������ʱ */
				if (StrTemp[0] != '\0')
				{
					/* ĩβ�� '\0' */
					StrTemp[StrTempNowLen < INT64_INPUT_MAX_LEN ? StrTempNowLen : INT64_INPUT_MAX_LEN] = '\0';

					/* �� StrTemp ת��Ϊ��ֵ���� _ints */
					_ints[IntsHeightNumber][IntsWidthNumber < INT64_OUTPUT_MAX_LEN ? IntsWidthNumber : INT64_OUTPUT_MAX_LEN] = atoll(StrTemp);
				}
				++pos;
				break;
			}

			/* �����ڿո�ʱ�������ݷ��� StrTemp */
			if (newstr[pos] != ' ') StrTemp[StrTempNowLen < INT64_INPUT_MAX_LEN ? StrTempNowLen++ : INT64_INPUT_MAX_LEN] = newstr[pos];

			/* ��������ո������һ�ζ� _ints �Ĵ洢 */
			else if (StrTemp[0] != '\0')
			{
				StrTemp[StrTempNowLen < INT64_INPUT_MAX_LEN ? StrTempNowLen : INT64_INPUT_MAX_LEN] = '\0';

				_ints[IntsHeightNumber][IntsWidthNumber < INT64_OUTPUT_MAX_LEN ? IntsWidthNumber++ : INT64_OUTPUT_MAX_LEN] = atoll(StrTemp);

				/* ��� StrTemp */
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