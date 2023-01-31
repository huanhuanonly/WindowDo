// lrWindowAlias.h -- Introduce alias as data carrier in lrWindowDo()
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
// Abstract:
//     The alias was originally designed to use a string name to represent a window ID,
//     Later, it was extended to introduce the concept of alias type, and add more data structures
//
#pragma once

#ifndef _LR_WINDOWALIAS_H_
#define _LR_WINDOWALIAS_H_

#include "../Head/lrWindowDo.h"
#include "../Head/lrIO.h"
#include "../Resource/resource.h"

/* Alias separator, placed in front of the instruction */
#define ALIAS_SEPARATOR				'.'

/* If TRUE, the alias name is case sensitive */
#define ALIAS_CASE_SENSITIVE		FALSE

/* Maximum length of alias name */
#define ALIAS_MAX_LENGTH			21


/*
* For lrInitAlias()
* 
* Some default definitions of lrInitAlias()
* After calling lrInitAlias(), you must call lrClearAlias()
*/

/* Class name */
#define IA_ALIAS_CLASSNAME			"wdoWindowBoxClass"
/* Font width */
#define IA_ALIAS_FONTWIDTH			10
/* Font height */
#define IA_ALIAS_FONTHEIGHT			20
/* Font weight */
#define IA_ALIAS_FONTWEIGHT			FW_NORMAL
/* Background color */
#define IA_ALIAS_BACKCOLOR			RGB(255,255,255)
/* Text color */
#define IA_ALIAS_TEXTCOLOR			RGB(0, 0, 0)
/* Cursor handle */
#define IA_ALIAS_HCURSOR			LoadCursorA(NULL, IDC_ARROW)
/* Icon handle */
#define IA_ALIAS_HICON				LoadIconA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDI_ICON_CHILDMIN))
/* Font handle */
#define IA_HFONT					lr_CreateFont(IA_ALIAS_FONTHEIGHT, IA_ALIAS_FONTWIDTH, FW_NORMAL, FALSE, FALSE, FALSE, NULL)


#define ALIASTYPE_STR_NULL	"NULL"
#define ALIASTYPE_STR_WIN	"WIN"
#define ALIASTYPE_STR_PATH	"PATH"
#define ALIASTYPE_STR_OUT	"OUT"
#define ALIASTYPE_STR_DRAW	"DRAW"
#define ALIASTYPE_STR_BOX	"BOX"

/* Alias type set */
typedef enum ALIASTYPE
{
	alias_type_null = 0,
	alias_type_win,
	alias_type_path,
	alias_type_out,
	alias_type_draw,
	alias_type_box

} ALIASTYPE;

/* Count of alias types */
#define ALIASTYPECOUNT	(alias_type_box + 1)

/* The default alias data is as follows */
typedef enum ALIASDEF
{
	alias_def_null = 0,
	alias_def_out,
	alias_def_draw,

	alias_def_box_blank,
	alias_def_box_text,
	alias_def_box_info,
	alias_def_box_log,
	alias_def_box_image,
	alias_def_box_blue

} ALIASDEF;


/* These font info in alias */
typedef struct ALIAS_FONTINFO
{
	UINT width;							// Font width
	UINT height;						// Font height

	UINT weight;						// Font weight

	BYTE Italic;						// Italic
	BYTE Underline;						// Underline
	BYTE StrikeOut;						// StrikeOut

	CHAR name[LF_FACESIZE];				// Font name

}ALIAS_FONTINFO;


typedef enum PATHTYPE
{
	PATHTYPE_INVALID = 0,
	PATHTYPE_FILE,
	PATHTYPE_DIR

}PATHTYPE;


typedef struct ALIAS_PATH
{
	CHAR			Path[MAX_PATH];			// Full path
	PATHTYPE		Type;					// File or dir

}ALIAS_PATH, * PALIAS_PATH;

typedef struct ALIAS_OUT
{
	CHAR			image[MAX_PATH];		// This image will be printed after the text is printed
	COORD			ImageInTextPos;			// Coordinates with text as origin

	HFONT			hFont;					// Font handle

	ALIAS_FONTINFO	fontinfo;				// Font info for createing new font

	COLORREF		TextColor;				// Text color
	COLORREF		BackColor;				// Background color

	BYTE			IsTransparent;			// If TRUE, Transparent background

}ALIAS_OUT, * PALIAS_OUT;

typedef struct ALIAS_DRAW
{
	COLORREF		StrokeColor;			// Stroke color
	BYTE			StrokeWidth;			// Stroke width

	BYTE			Fill;					// If TRUE, fill
	COLORREF		FillColor;				// Fill color
	CHAR			FillImage[MAX_PATH];	// Image for fill

}ALIAS_DRAW, * PALIAS_DRAE;

typedef struct ALIAS_BOX
{
	CHAR			ClassName[MAXLEN_WIN_CLASS];

	HCURSOR			hCursor;				// Cursor handle
	HICON			hIcon;					// Icon handle
	HFONT			hFont;					// Font handle

	ALIAS_FONTINFO	fontinfo;				// Font info

	COLORREF		TextColor;				// Text color
	COLORREF		BackColor;				// Background color

}ALIAS_BOX, * PALIAS_BOX;


typedef struct ALIAS
{
	CHAR			name[ALIAS_MAX_LENGTH];
	ALIASTYPE		type;

	struct _ALIAS_DATA
	{
		HWND		win;

		ALIAS_PATH	path;
		ALIAS_OUT	out;
		ALIAS_DRAW	draw;
		ALIAS_BOX	box;

	}data;

}ALIAS, * PALIAS;


typedef struct ALIASSINGLE
{
	CHAR			name[ALIAS_MAX_LENGTH];
	ALIASTYPE		type;

	union _ALIASSINGLE_DATA
	{
		HWND		win;
		ALIAS_PATH	path;
		ALIAS_OUT	out;
		ALIAS_DRAW	draw;
		ALIAS_BOX	box;

		PALIAS		Integration;

	}data;

}ALIASSINGLE, * PALIASSINGLE;



/* Header ID of alias file */
#define FALIAS_HEADER_STRID						"LRALIAS"
/* Suffix of alias file */
#define FALIAS_EXT								".wdas"

/*
* The alias binary file consists of two structures
*/

/* The alias file header, alias info in file */
typedef struct FALIAS_HEADER
{
	CHAR	Header[sizeof(FALIAS_HEADER_STRID)];	// File header
	UINT	VerID;									// Version
	UINT	AliasCount;								// Alias count in this file

	time_t	SaveTime;								// Save time of this file
	time_t	ChangeTime;								// Change time of this file

	UINT	UserSecret;								// User secret key
	CHAR	UserName[20];							// User name

}FALIAS_HEADER;

/* Main text */
typedef ALIASSINGLE FALIAS_TEXT, * FALIAS_TEXTS;

typedef struct FALIAS
{
	FALIAS_HEADER fAliasHeader;
	FALIAS_TEXTS  fAliasTexts;
}FALIAS;


#ifdef __cplusplus
extern "C"
{
#endif


// In file lrWindowAlias.c



/* Verify the alias name is valid */
BOOL							// If _AliasName is finally valid, return TRUE
lrVerifyAliasName(
	char* _AliasName,			// Name to be verified
	BOOL _CorrectName			// If is TRUE, the error of _AliasName will be corrected
);



/* Initialize alias
*  Use lrClearAlias() to clear the alias after using this function */
void
lrInitAlias(
	LPCSTR			_Name,		// The name of the alias, truncate when name exceeds maximum
	ALIASTYPE		_Type,		// The type of the alias
	PALIASSINGLE	_Alias		// Point a alias
);



/* Clear a alias data */
void
lrClearAlias(
	PALIASSINGLE _Alias
);



/* Clear alias data of the specified type */
void
lrClearAliasEx(
	void* _Alias,				// Point to a alias, such as (ALIAS_BOX*)
	ALIASTYPE _AliasType		// Type of the alias
);



/* Copy the data of one alias to another empty alias
* 
*  Use this function to reallocate memory and resources
*  If this function is not used, the pointers and resources in the two aliases will be invalidated after using lrClearAlias() */
BOOL
lrCopyAlias(
	ALIASSINGLE* _Destination,
	const ALIASSINGLE* _Source
);



/* Copy alias data of the specified type
*  Use _AliasType specify the alias type */
BOOL
lrCopyAliasEx(
	void* _Destination,
	const void* _Source,
	ALIASTYPE _AliasType
);



/* Get the string of alias type */
const char* const
lrGetAliasTypeString(
	ALIASTYPE _aliastype		// See enum ALIASTYPE
);



/* Get predefined alias data
* 
*  This function will generate a new alias,
*  When it is no longer needed, call lrClearAlias() to release */
BOOL
lrGetAliasDef(
	ALIASDEF		_aliasdef,	// See enum ALIASDEF
	PALIASSINGLE	_pa			// Point a alias, name will not be overwritten
);



/* Print data tree of alias */
void
lrPrintAliasTree(
	PALIASSINGLE	_Alias
);



/* Put the alias in the alias library, or delete the alias from the alias library */
BOOL
lrPutAliasLibrary(
	PALIASSINGLE	_AliasInfo,	// If _Delete is TRUE, only need set _AliasInfo.name
	BOOL			_Delete		// If _Delete is TRUE, delete the alias form the alias library
);



/* Get current alias library */
const ALIASSINGLE*				// Return the first address of the alias library. Do not free and change data
lrGetAliasLibrary(
	size_t* _AliasCount			// Count of aliases in the receiving alias library
);



/* Set the alias data in the current alias library. */
ALIASSINGLE*					// Returns the requested alias pointer. You can modify the alias data directly through the pointer. Do not free
lrSetAliasLibrary(
	size_t _Number				// Alias number in the alias library to set
);



/* Find aliases in the alias library */
BOOL
lrFindAliasInAliasLibary(
	LPCSTR			_Name,		// String of alias name
	ALIASSINGLE*	_Alias		// If found, fill data
);



/* Find aliases in the instruction */
ALIAS							// No error returned
lrFindAliasInInstruction(
	char*		_Instruction,	// String of instruction
	size_t**	_IndexArray,	// Point a (size_t*), get alias indices in alias library, these alias from instruction, need free
	size_t*		_IndexCount,	// Get count of (*_IndexArray)
	BYTE*		_Bools			// Array of size is ALIASTYPECOUNT, record whether the alias type appears
);



/* Save aliases as file
* 
*  If the file already exists and the format is correct,
*  retain the original data and insert the new data at the end */
BOOL
lrSaveAlias(
	LPCSTR			_Path,		// A path with a file name. If the path has no suffix or the suffix is inconsistent, the suffix will be spliced
	BOOL			_SaveLibary,// If is TRUE, save alias library and ignore parameter C
	PALIASSINGLE	_Alias		// Write the alias data to the file
);



/* Load aliases from file
* 
*  Some alias data are unavailable,
*  such as pointers and resources,
*  and memory needs to be re applied and assigned */
BOOL
lrLoadAlias(
	LPCSTR			_Path,		// A path with a file name. If the path has no suffix, the suffix will be spliced
	FALIAS*			_fAlias		// Receive alias data in file, _fAlias->fAliasTexts need free
);


// In file lrCreateWindow.c



/* Get alias data of the window box */
BOOL
lrGetBoxAlias(
	HWND			_Box,		// Handle of window box
	ALIAS_BOX*		_BoxAlias	// If _Box is a valid window box handle, data will be populated
);



/* Set alias of the window box
*  The original memory and resources will be released, and the original pointer will become invalid */
BOOL
lrSetBoxAlias(
	HWND			 _Box,		// Handle of window box
	const ALIAS_BOX* _BoxAlias	// New alias of the window box
);


#ifdef __cplusplus
}
#endif

#endif		// _LR_WINDOWALIAS_H_