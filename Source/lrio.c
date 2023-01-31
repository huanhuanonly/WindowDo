// lrio.c -- For lrio.h
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lrIO.h"
#include "../Head/dirent.h"
#include "../Head/lrWindowDo.h"
#include "../Head/lr_wdo.h"

#include <sys\stat.h>
#include <winsock.h>

#pragma warning(disable : 6031)
#pragma warning(disable : 6308)

#pragma comment(lib, "ws2_32.lib")


/* 读取文件所有内容。
* 
* 参数:
*		_FilePath     |  文件路径
*		_FileSize     |  接收文件大小
* 
* 返回值:
*		返回文件内容字符串。需要释放。
*/
char* lrfgetall(const char* const _FilePath, size_t* _FileSize)
{
	if (_FilePath == NULL || _FilePath[0] == '\0') return NULL;

	FILE* fileptr = fopen(_FilePath, "rb");

	if (fileptr)
	{
		fseek(fileptr, 0, SEEK_SET);

		struct _stat64 fileinfo;

		_stat64(_FilePath, &fileinfo);

		char* filetext = (char*)malloc((size_t)fileinfo.st_size + 1);

		if (filetext)
		{
			memset(filetext, '\0', (size_t)fileinfo.st_size + 1);

			if(_FileSize)
				*_FileSize = fread(filetext, sizeof(char), fileinfo.st_size, fileptr);
			else
				fread(filetext, sizeof(char), fileinfo.st_size, fileptr);

			fclose(fileptr);
			return filetext;
		}
		else fclose(fileptr);
	}

	return NULL;
}



/* 释放二级指针申请的内存。
* 
* 参数:
*		_FreePtrs     |  指向一个二级指针
*		_StrsSize     |  二级指针中的一级指针数量
*/
void lrfrees(void*** _FreePtrs, size_t _StrsSize)
{
	if (*_FreePtrs && _StrsSize)
	{
		for (size_t i = 0; i < _StrsSize; ++i)
		{
			free((*_FreePtrs)[i]);
		}

		free(*_FreePtrs);

		*_FreePtrs = NULL;
	}
}



BOOL lrGetFileListInDir(const char* const _Dir, char*** _FileList, size_t* _FileCount, BOOL _Absolute)
{
	if (access(_Dir, 0)) return FALSE;
	
	HANDLE handle = INVALID_HANDLE_VALUE;
	DWORD n = GetFullPathNameA(_Dir, 0, NULL, NULL);
	char* dir = (char*)malloc(n * sizeof(char) + 16);

	if (dir == NULL) return FALSE;
	if ((n = GetFullPathNameA(_Dir, n, dir, NULL)) == 0)
	{
		free(dir);
		return FALSE;
	}
	
	char* p = dir + n;
	switch (p[-1])
	{
	case '\\':
	case '/':
	case ':':
		break;

	default: *p++ = '\\';
	}
	*p++ = '*';
	*p = '\0';

	WIN32_FIND_DATAA data = { 0 };
	handle = FindFirstFileExA(dir, FindExInfoStandard, &data, FindExSearchNameMatch, NULL, 0);

	do
	{
		if (!strcmp(data.cFileName, ".") || !strcmp(data.cFileName, "..")) continue;

		++(*_FileCount);

		if (_Absolute)
		{
			if (*_FileList = (char**)realloc(*_FileList, (*_FileCount) * sizeof(char*)))
			{
				size_t len = (strlen(data.cFileName) + n) * sizeof(char) + 1;
				if ((*_FileList)[(*_FileCount) - 1] = (char*)malloc(len))
				{
					for (size_t i = 0; ; ++i)
					{
						if (dir[i] != '\0')
						{
#pragma warning (suppress : 6386)
							(*_FileList)[(*_FileCount) - 1][i] = dir[i];
						}
						else
						{
							(*_FileList)[(*_FileCount) - 1][i - 1] = '\0';
							break;
						}
					}
					strcat((*_FileList)[(*_FileCount) - 1], data.cFileName);
				}
				else wdo_error_ex();
			}
			else wdo_error_ex();
		}
		else
		{
			if (*_FileList = (char**)realloc(*_FileList, (*_FileCount) * sizeof(char*)))
			{
				size_t len = strlen(data.cFileName) * sizeof(char) + 1;
				if ((*_FileList)[(*_FileCount) - 1] = (char*)malloc(len))
				{
					memcpy((*_FileList)[(*_FileCount) - 1], data.cFileName, len);
				}
				else wdo_error_ex();
			}
			else wdo_error_ex();
		}
	} while (FindNextFileA(handle, &data));

	FindClose(handle);
	free(dir);

	return TRUE;
}



BOOL lrGetRegValueListInRegedit(HKEY hKey, LPSTR** _ValueName, DWORD** _ValueType, LPBYTE** _ValueData, DWORD* _ValueCount)
{
	if (hKey == NULL || _ValueCount == 0) return FALSE;

	*_ValueCount = 0;
	if (_ValueName) *_ValueName = NULL;
	if (_ValueType) *_ValueType = NULL;
	if (_ValueData) *_ValueData = NULL;

	char ValueName[300] = "";
	DWORD ValueNameSize = sizeof(ValueName);

	BYTE ValueData[300] = "";
	DWORD ValueDataSize = sizeof(ValueData);

	DWORD ValueType = 0;

	for (DWORD i = 0;; ++i)
	{
		switch (RegEnumValueA(hKey,
			i,
			ValueName,
			(&ValueNameSize),
			NULL,
			(&ValueType),
			(_ValueData ? ValueData : NULL),
			(_ValueData ? (&ValueDataSize) : NULL)))
		{
		case ERROR_SUCCESS:
		{
			++(*_ValueCount);
			if (_ValueName)
			{
				if (*_ValueName = (LPSTR*)realloc(*_ValueName, (*_ValueCount) * sizeof(LPSTR)))
				{
					if ((*_ValueName)[(*_ValueCount) - 1] = (LPSTR)malloc((size_t)(ValueNameSize + 1) * sizeof(char)))
					{
						memcpy((*_ValueName)[(*_ValueCount) - 1], ValueName, (size_t)(ValueNameSize + 1) * sizeof(char));
					}
				}
			}
			if (_ValueType)
			{
				if (*_ValueType = (DWORD*)realloc(*_ValueType, (*_ValueCount) * sizeof(DWORD)))
				{
					(*_ValueType)[(*_ValueCount) - 1] = ValueType;
				}
			}
			if (_ValueData)
			{
				if (*_ValueData = (LPBYTE*)realloc(*_ValueData, (*_ValueCount) * sizeof(LPBYTE)))
				{
					if ((*_ValueData)[(*_ValueCount) - 1] = (LPBYTE)malloc((size_t)(ValueDataSize + 1) * sizeof(BYTE)))
					{
						memcpy((*_ValueData)[(*_ValueCount) - 1], ValueData, (size_t)(ValueDataSize + 1) * sizeof(BYTE));
					}
				}
			}
			ValueNameSize = sizeof(ValueName);
			ValueDataSize = sizeof(ValueData);
			break;
		}
		case ERROR_NO_MORE_ITEMS: return TRUE;
		case ERROR_MORE_DATA: break;
		default: break;
		}
	}
	return TRUE;
}


short lrGetRandomCharacterInString(const char* const _String, size_t _Length)
{
	if (_String == NULL || _Length == 0) return 0;

	short* Carrier = (short*)malloc(_Length * sizeof(short));
	if (Carrier == NULL) return 0;

	int count = 0;

	for (size_t i = 0; i < _Length; ++i)
	{
		if (_String[i] & 0x80 && i + 1 < _Length && _String[i + 1] & 0x80)
		{
			Carrier[count++] = (short)((((short)(_String[i]) & 0x00ff) << 8) | (short)(_String[i + 1]) & 0x00ff);
			++i;
		}
		else Carrier[count++] = _String[i];
	}
	lr_srand();
	short result = Carrier[rand() % count];

	free(Carrier);

	return result;
}


size_t lrGetRandomLineTextInFile(const char* const _FileName, char** _LineText)
{
	if (_FileName == NULL || _FileName[0] == '\0' || _LineText == NULL) return 0;

	FILE* fp = fopen(_FileName, "rb");
	if (fp == NULL) return 0;

	size_t result = 0;

	size_t FileSize = 0;
	char* FileText = lrfgetall(_FileName, &FileSize);
	if (FileText == NULL) goto error;

	size_t LineCount = lrstrcnt(FileText, '\n');
	if (FileText[FileSize - 1] != '\n') ++LineCount;

	lr_srand();
	size_t r = rand() % LineCount;

	size_t pos = 0;

	for (size_t ct = 0; ct < r; ++pos)
	{
		if (FileText[pos] == '\n') ++ct;
	}
	char* ftp = FileText + pos;

	for (pos = 0;; ++pos)
	{
		if (ftp[pos] == '\n' || ftp[pos] == '\0') break;
	}

	if ((*_LineText = (char*)malloc((pos + 1) * sizeof(char))))
	{
		memcpy(*_LineText, ftp, pos * sizeof(char));
		(*_LineText)[pos] = '\0';

		result = pos;
	}
	free(FileText);
error:
	fclose(fp);
	return result;
}


/* 设置或清空剪贴板数据。
* 
* 参数:
*		_str          |  要设置的字符数据
* 
* 返回值:
*		成功返回 TRUE，否则返回 FALSE。
*/
BOOL lrSetClipboardText(const char* const _str)
{
	BOOL ret = TRUE;

	/* 打开剪贴板 */
	if (OpenClipboard(NULL))
	{
		/* 清空剪贴板数据 */
		EmptyClipboard();

		/* 如果 _str 为空的话则只清空剪贴板数据 */
		if (_str != NULL && _str[0] != '\0')
		{
			size_t strsize = strlen(_str) + 1;

			/* GMEM_MOVEABLE 分配可移动内存。内存块永远不会在物理内存中移动，但它们可以在默认堆中移动 */
			HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE, strsize);

			if (memory != NULL)
			{
				/* 锁定内存区域 */
				LPTSTR memorypoint = (LPTSTR)GlobalLock(memory);

				if (memorypoint != NULL)
				{
					/* 复制数据到内存区域 */
					memcpy(memorypoint, _str, strsize);

					/* 解除内存锁定 */
					GlobalUnlock(memory);

					/* 设置剪切板数据 */
					SetClipboardData(CF_TEXT, memory);
				}
				else ret = FALSE;		//内存查找失败
			}
			else ret = FALSE;			//内存分配失败
		}
	}
	else ret = FALSE;					//剪贴板打开失败

	/* 关闭剪贴板 */
	CloseClipboard();

	return ret;
}


/* 获取剪贴板数据。
*
* 参数:
*		_strptr       |  接收剪贴板中的字符串指针(需要释放)
*
* 返回值:
*		返回 _strptr 的长度。
*/
size_t lrGetClipboardText(char** _strptr)
{
	if (_strptr == NULL) return 0;

	*_strptr = (char*)malloc(sizeof(""));
	if (*_strptr == NULL) return 0;

	if (OpenClipboard(NULL) == FALSE) return 1;
	
	HGLOBAL hClip = GetClipboardData(CF_TEXT);
	if (hClip == NULL)
	{
		CloseClipboard();
		return 1;
	}

	char* Text = (char*)GlobalLock(hClip);
	if (Text == NULL || Text[0] == '\0')
	{
		CloseClipboard();
		return 1;
	}
	
	size_t len = strlen(Text) + 1;

	free(*_strptr);
	*_strptr = (char*)malloc(len);
	memcpy(*_strptr, Text, len);

	GlobalUnlock(hClip);

	CloseClipboard();
	return len;
}


/* 获取一个指定范围内的随机数。
* 
* 参数:
*		_min          |  最小值
*		_max          |  最大值
* 
* 返回值:
*		返回一个指定范围内的随机数。
*/
unsigned __int64 lrrand_ui64(unsigned __int64 _min, unsigned __int64 _max)
{
	if (_max < RAND_MAX) return (unsigned __int64)(rand() % (_max + 1 - _min) + _min);
	else return (unsigned __int64)((rand() * (rand() % ((ULLONG_MAX / RAND_MAX) - 1) + 1)) % (_max + 1 - _min) + _min);
}



/* 获取本机 IP 地址。
* 
* 参数:
*		ip            |  如果不为 NULL，则返回一个类型为 IPv4_struct 的 IP地址
* 返回值:
*		返回一个完整的IP地址字符串指针；
*		不需要释放，也不应该修改值；
*		例：“192.168.16.0”。
*/
const char* lrGetMyIPv4(IPv4_struct* ip)
{
	WORD ver = MAKEWORD(1, 1);
	WSADATA wsaData;

	if (WSAStartup(ver, &wsaData) == 0)
	{
		struct hostent* phostinfo = gethostbyname("");

		char* p = inet_ntoa(*((struct in_addr*)(*phostinfo->h_addr_list)));

		WSACleanup();

		if (ip && p)
		{
			char ip_str[4][5];
			memset(ip_str, '\0', sizeof(ip_str));

			size_t len = strlen(p);
			for (size_t i = 0, j = 0, k = 1; i < len; ++i, (p[i] == '.' ? (k = 0, ++j) : ++k))
			{
				if (p[i] != '.') ip_str[j][k - 1] = p[i];
			}
			for (size_t i = 0; i < 4; ++i) ip->ip[i] = (unsigned char)atoi(ip_str[i]);
		}
		return p;
	}
	else return NULL;
}


/* 获取位图高和宽。
* 
* 参数:
*		_bitmapPath   |  位图路径
* 
* 返回值:
*		返回包含位图高和宽的结构体，
*		如果失败，则结构体内数据都为 0。
* 
* 言论:
*		不会验证文件后缀名是否为 .bmp，
*		只会验证文件头是否为 BM。
*/
SIZE lrGetBitmapSize(const char* const _bitmapPath)
{
	SIZE bitmapSize = { 0,0 };

	if (_bitmapPath == NULL || _bitmapPath[0] == '\0') goto done;

	FILE* bitmapFile = fopen(_bitmapPath, "rb");
	if (bitmapFile == NULL) goto done;

	/* 位图结构参考： https://blog.csdn.net/redline2005/article/details/17024867 */
	BITMAPFILEHEADER bitmapHeader;
	BITMAPINFOHEADER bitmapInfo;

	fread(&bitmapHeader, sizeof(BITMAPFILEHEADER), 1, bitmapFile);
	fread(&bitmapInfo, sizeof(BITMAPINFOHEADER), 1, bitmapFile);

	/* 验证文件头 */
	if (bitmapHeader.bfType == 19778)
	{
		bitmapSize.cx = bitmapInfo.biWidth;
		bitmapSize.cy = bitmapInfo.biHeight;
	}
	fclose(bitmapFile);

done:
	return bitmapSize;
}