// wdoio.h -- IO stream from wdo
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#pragma once

#ifndef WDOIO_H
#define WDOIO_H

#include "lr_wdo.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* Get a line of commands from a file */
char*										// need free
wdo_fgetline(
	FILE* fp,
	BOOL  IsUnicodeFile
);



/* Split command into instruction and parameter */
void
wdo_divided_command(
	const char* const _Command,				// String of command
	char** _Instruction,					// Pointing to a string pointer, need free, memory will be allocated at least one byte
	char** _Parameter						// Pointing to a string pointer, need free, memory will be allocated at least one byte
);


/* Split string contents in curly brackets (if any)
* 
* Use linebreak to split the string,
* and delete curly brackets, linebreak, spaces and tab at both ends.
* 
* Can call lrfrees() function to free memory. */
char**										// If failed, return NULL, else return string array, need free
wdo_curlybrace_splitstr(
	const char* const _string,				// String
	size_t* _StrsCount						// Count of receive string arrays
);



/* Split string contents in curly brackets (if any)
*
* Use linebreak to split the string,
* and delete curly brackets, linebreak, spaces and tab at both ends.
*
* Can call lrfrees() function to free memory. */
__int64**									// return int array, will not return NULL, need free
wdo_curlybrace_splitint(
	const char* const _string,				// String
	size_t* _IntsCount						// Count of receive string arrays
);


/* Count of loop after this character */
#define WDO_LOOPCOUNT_CHAR			'*'

/* String constant of representing infinite loop */
#define WDO_LOOPCOUNT_STR			"loop"

/* If infinite loop, return the value */
#define WDO_LOOPCOUNT_ID			ULLONG_MAX


/* Get the count of loop of an instruction
* 
* No return negative number, ignore sign '-' before number
*/
unsigned __int64							// Return count of loop of the instruction
wdo_getloopcount(
	char* _instruction						// Instruction string
);



/* Convert the hotkey string of the instruction WDOINS_HOTKEY to two hotkey codes */
BOOL										// if the string has no errors, return TRUE
wdo_string_to_hotkey(
	const char* const _HotkeyString,		// String of hotkey from the instruction WDOINS_HOTKEY
	UINT* _Modifiers,						// Alt & Ctrl & Shift & Windows
	UINT* _VirtKey							// Code of virt key
);


/* Normal */
#define OUTLOG_NORMAL				(BYTE)TRUE
/* Error */
#define OUTLOG_ERROR				(BYTE)FALSE

/* Use the color if the state is normal */
#define OUTLOG_NORMAL_COLOR			RGB(0x00, 0xff, 0x00)
#define OUTLOG_ERROR_COLOR			RGB(0xff, 0x00, 0x00)

#define OUTLOG_WAIT					INFINITE


/* Output the string from instruction */
static inline void wod_log_frominstruction(const char* const _Instruction)
{
	wdo_setcolor(OUTLOG_NORMAL_COLOR);
	wdo_puts("From instruction # ");
	wdo_puts_upper(_Instruction);
	wdo_putc('\n');
	wdo_putc('\n');
}



/* Output log from wdo instruction */
void
wdo_log(
	const char* const _Ins,					// String of instruction, can be NULL
	BYTE _State,							// OUTLOG_NORMAL or OUTLOG_ERROR
	const char* const _Format,				// Format string, see printf()
	...										// See printf()
);



/* Output the error-code and error-text of the function */
void
wdo_log_error(
	const char* const _function				// String of function
);



/* Output window brief info */
void
wdo_log_win_brief(
	HWND _hWnd,								// Window handle, can be an invalid window handle
	BOOL _child								// If is TRUE, output child window prefix
);



/* Output the list of all wdo instruction */
unsigned int
wdo_log_help_all(
	const char* const _inss					// String of all instruction
);


/* Output these help info of the wdo instruction */
void
wdo_log_help(
	const char* const _function,
	const char* const _Syntax,
	const char* const _Parameters,
	const char* const _RetrieveAlias,
	const char* const _Remarks,
	const char* const _Examples
);


#ifdef __cplusplus
}
#endif
#endif