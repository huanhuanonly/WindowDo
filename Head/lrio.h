// lrio.h -- IO stream from lr library
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#pragma once

#ifndef LR_IO_H
#define LR_IO_H

#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#ifdef __cplusplus
extern "C"
{
#endif



typedef struct IPv4_struct
{
    unsigned char ip[4];
} IPv4_struct;



// In file lrio.c



/* Get all text in the file */
char*								// If successful, return the string, need free
lrfgetall(
    const char* const _FilePath,	// File path
    size_t*	          _FileSize		// Receive file size
);



/* Releasing a secondary pointer,
*  which points to countless primary pointers,
*  will release all the memory */
void
lrfrees(
    void***  _FreePtrs,				// This secondary pointer. Need to get the address, it will be set to NULL
    size_t   _StrsSize				// Quantity of primary pointers
);



/* Get file list in dir */
BOOL
lrGetFileListInDir(
    const char* const _Dir,         // The dir name
    char*** _FileList,              // File list, call lrfrees() to free
    size_t* _FileCount,             // File count in the dir
    BOOL _Absolute                  // Absolute path or file name, for return file list
);



/*  */
BOOL
lrGetRegValueListInRegedit(
    HKEY hKey,
    LPSTR** _ValueName,
    DWORD** _ValueType,
    LPBYTE** _ValueData,
    DWORD* _ValueCount
);



/* Get random character in string
* 
*  if ((short & 0x8080) == 0x8080), short = char[2]
*  Get first  char[0], char[0] = (short >> 8) & 0x00ff
*  Get second char[1], char[1] = short & 0x00ff */
short                               // Return a character in _String
lrGetRandomCharacterInString(
    const char* const _String,      // Source string
    size_t _Length                  // Length of string, '\0' is not required
);



/* Get random line text in file */
size_t                              // Return Length of (*_LineText)
lrGetRandomLineTextInFile(
    const char* const _FileName,    // File name
    char** _LineText                // Get line text, need free
);



/* Set or clear clipboard data */
BOOL
lrSetClipboardText(
    const char* const _str			// If it is NULL, clear the clipboard
);



/* Get clipboard text data */
size_t								// Return _strptr length (include '\0')
lrGetClipboardText(
    char** _strptr					// Receives a string pointer in the clipboard, need free
);



/* Get a random number in the specified range. */
unsigned __int64					// Return result
lrrand_ui64(
    unsigned __int64 _min,			// Minimum value
    unsigned __int64 _max			// Maximum value
);



/* Get my ipv4 */
const char*							// Returns a complete IPv4 string. Example: "192.168.16.0"
lrGetMyIPv4(
    IPv4_struct* ip					// If it is not NULL, fill data
);



/* Get a bitmap height and width
*  It will not verify whether the file suffix is (.bmp), but only whether the file header is BM */
SIZE
lrGetBitmapSize(
    const char* const _bitmapPath	// The bitmap file path (.bmp)
);



// In file lrString.c



/* Split a string into multiple segments
* 
*  Splits a string with one or more spaces or tabs or newlines, unless enclosed in curly braces ('{' and '}') */
void
lrstrsep(
    const char* const _String,		// Original string
    size_t _SplitCount,			    // Count of split
    ...								// (char*) Split _String from left to right
);



/* Shift logical left of string */
char*								// Return _str if successful, else return NULL
lrstrmov(
    char*  _str,					// The string
    int    _mov,					// Logical left shifted bytes
    char   _val,					// After logical shift to the left, use this character to fill in the blank bytes
    size_t _maxlen					// Length of string to be logically shifted to the left, cannot be greater than string length
);



/* Remove the characters specified at both ends of the string (if exist) */
char*								// Return _String
lrstrrem(
    char* _String,					// The string
    char  _Left,					// Character at the left  end
    char  _Right					// Character at the right end
);



/* Get the count of the specified character in a string */
unsigned int						// Return count of character in _String
lrstrcnt(
    const char* const _String,		// The string
    char              _Character	// Character in string
);



/* Extract hex from string, and convert to decimal */
unsigned __int64					// Resurn result
lrHexToDec(
    const char* const  _HexString	// The string of hex 
);



/* Replace '\n' in the string with "\r\n" */
size_t                              // Return length of _newString
lrStringLinebreakMode(
    const char* const _String,      // Original string
    char**            _newString    // Pointer to the new string, need free
);


#ifdef __cplusplus
}
#endif

#endif		// LR_IO_H