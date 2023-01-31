// lrWindowAlias.c -- For lrWindowAlias.h
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lr_wdo.h"
#include "../Head/lrWindowDo.h"
#include "../Head/lrIO.h"
#include "../Resource/resource.h"

#if ALIAS_CASE_SENSITIVE
#	define strcmp_wdoalias strcmp
#else
#	define strcmp_wdoalias stricmp
#endif

static ALIASSINGLE* AliasLibrary = NULL;
static size_t AliasLibraryCount = 0;

#pragma warning (disable : 6324)


/* 验证别名名称是否有效
* 
* 参数:
*		_AliasName    |  需要验证的名称
*		_CorrectName  |  如果为 TRUE，则修正别名名称
* 
* 返回值:
*		返回 _AliasName 最终是否有效，有效返回 TRUE。
*/
BOOL lrVerifyAliasName(char* _AliasName, BOOL _CorrectName)
{
	if (_AliasName == NULL || _AliasName[0] == '\0') return FALSE;

	if (_CorrectName)
	{
		char newName[ALIAS_MAX_LENGTH] = { '\0' };
		size_t pos = 0;

		size_t len = strlen(_AliasName) + 1;

		for (size_t i = 0; i < len && pos < ALIAS_MAX_LENGTH; ++i)
		{
			if (pos == 0 && (isalpha(_AliasName[i]) || _AliasName[i] == '_'))
			{
				newName[pos++] = _AliasName[i];
			}
			else if (pos && (isalnum(_AliasName[i]) || _AliasName[i] == '_'))
			{
				newName[pos++] = _AliasName[i];
			}
		}
		newName[pos] = '\0';
		strcpy(_AliasName, newName);
		return !!newName[0];
	}
	else
	{
		/* 第一位不能为数字 */
		if (_AliasName[0] >= '0' && _AliasName[0] <= '9') return FALSE;

		size_t len = strlen(_AliasName);

		/* 长度溢出 */
		if (len + 1 > ALIAS_MAX_LENGTH) return FALSE;

		for (size_t i = 0; i < len; ++i)
		{
			/* 只能以 a-z, A-Z, 0-9, '_' 命名 */
			if (isalnum(_AliasName[i]) == 0 && _AliasName[i] != '_')
			{
				return FALSE;
			}
		}
		return TRUE;
	}
}


/* 初始化别名。
* 
* 参数:
*		_Name   |  别名的名称
*		_Type   |  别名的类型
*		_Alias  |  指向要初始化的别名
* 
* 言论:
*		使用此函数后请使用 lrClearAlias() 清空别名。
*/
void lrInitAlias(LPCSTR _Name, ALIASTYPE _Type, PALIASSINGLE _alias)
{
	memset(_alias, 0, sizeof(ALIASSINGLE));
	memcpy(_alias->name, _Name, ALIAS_MAX_LENGTH);
	_alias->name[ALIAS_MAX_LENGTH - 1] = '\0';

	_alias->type = _Type;

	switch (_Type)
	{
	case alias_type_null:
		break;
	case alias_type_win:
		_alias->data.win = NULL;
		break;
	case alias_type_path:
		_alias->data.path.Path[0] = '\0';
		break;
	case alias_type_out:
		_alias->data.out.BackColor = RGB(0, 0, 0);
		_alias->data.out.TextColor = RGB(0, 255, 0);
		_alias->data.out.IsTransparent = TRUE;
		_alias->data.out.hFont = IA_HFONT;
		_alias->data.out.fontinfo.weight = IA_ALIAS_FONTWIDTH;
		_alias->data.out.fontinfo.height = IA_ALIAS_FONTHEIGHT;
		_alias->data.out.fontinfo.name[0] = '\0';
		_alias->data.out.fontinfo.weight = IA_ALIAS_FONTWEIGHT;
		_alias->data.out.fontinfo.Italic = FALSE;
		_alias->data.out.fontinfo.Underline = FALSE;
		_alias->data.out.fontinfo.StrikeOut = FALSE;
		break;
	case alias_type_draw:
		_alias->data.draw.StrokeColor = RGB(0x00, 0xff, 0x00);
		_alias->data.draw.Fill = FALSE;
		_alias->data.draw.StrokeWidth = 1;
		break;
	case alias_type_box:
		_alias->data.box.BackColor = IA_ALIAS_BACKCOLOR;
		_alias->data.box.TextColor = IA_ALIAS_TEXTCOLOR;
		_alias->data.box.hCursor = IA_ALIAS_HCURSOR;
		_alias->data.box.hIcon = IA_ALIAS_HICON;
		_alias->data.box.hFont = IA_HFONT;
		_alias->data.box.fontinfo.width = IA_ALIAS_FONTWIDTH;
		_alias->data.box.fontinfo.height = IA_ALIAS_FONTHEIGHT;
		_alias->data.box.fontinfo.name[0] = '\0';
		_alias->data.box.fontinfo.weight = IA_ALIAS_FONTWEIGHT;
		_alias->data.box.fontinfo.Italic = FALSE;
		_alias->data.box.fontinfo.Underline = FALSE;
		_alias->data.box.fontinfo.StrikeOut = FALSE;
		strcpy(_alias->data.box.ClassName, IA_ALIAS_CLASSNAME);
		break;
	}
	return;
}


/* 清空别名。
*
* 参数:
*		_Alias         |  指向清空的别名
*/
void lrClearAlias(PALIASSINGLE _Alias)
{
	if (_Alias == NULL || _Alias->type == alias_type_null) return;

	switch (_Alias->type)
	{
	case alias_type_win:
		break;

	case alias_type_path:
		break;

	case alias_type_out:
		if (_Alias->data.out.hFont)
			DeleteObject(_Alias->data.out.hFont);
		break;

	case alias_type_draw:
		break;

	case alias_type_box:
		if (_Alias->data.box.hFont)
			DeleteObject(_Alias->data.box.hFont);

		if (_Alias->data.box.hCursor)
			DestroyCursor(_Alias->data.box.hCursor);

		if (_Alias->data.box.hIcon)
			DestroyIcon(_Alias->data.box.hIcon);
		break;
	}
	memset(_Alias, 0, sizeof(ALIASSINGLE));

	return;
}


void lrClearAliasEx(void* _Alias, ALIASTYPE _AliasType)
{
	if (_Alias == NULL) return;

	switch (_AliasType)
	{
	case alias_type_win:
		break;

	case alias_type_path:
		memset(_Alias, 0, sizeof(ALIAS_PATH));
		break;

	case alias_type_out:
		if (((ALIAS_OUT*)_Alias)->hFont)
			DeleteObject(((ALIAS_OUT*)_Alias)->hFont);

		memset(_Alias, 0, sizeof(ALIAS_OUT));
		break;

	case alias_type_draw:
		memset(_Alias, 0, sizeof(ALIAS_DRAW));
		break;

	case alias_type_box:
		if (((ALIAS_BOX*)_Alias)->hFont)
			DeleteObject(((ALIAS_BOX*)_Alias)->hFont);

		if (((ALIAS_BOX*)_Alias)->hCursor)
			DestroyCursor(((ALIAS_BOX*)_Alias)->hCursor);

		if (((ALIAS_BOX*)_Alias)->hIcon)
			DestroyIcon(((ALIAS_BOX*)_Alias)->hIcon);

		memset(_Alias, 0, sizeof(ALIAS_BOX));
		break;
	}
	return;
}



BOOL lrCopyAlias(ALIASSINGLE* _Destination, const ALIASSINGLE* _Source)
{
	if (_Destination == NULL || _Source == NULL) return FALSE;

	*_Destination = *_Source;

	switch (_Destination->type)
	{
	case alias_type_out:

		if (_Source->data.out.hFont)
		{
			_Destination->data.out.hFont = lr_CreateFont(
				_Destination->data.out.fontinfo.height,
				_Destination->data.out.fontinfo.width,
				_Destination->data.out.fontinfo.weight,
				_Destination->data.out.fontinfo.Italic,
				_Destination->data.out.fontinfo.Underline,
				_Destination->data.out.fontinfo.StrikeOut,
				_Destination->data.out.fontinfo.name
			);
		}
		break;

	case alias_type_box:

		if (_Source->data.box.hFont)
		{
			_Destination->data.box.hFont = lr_CreateFont(
				_Destination->data.box.fontinfo.height,
				_Destination->data.box.fontinfo.width,
				_Destination->data.box.fontinfo.weight,
				_Destination->data.box.fontinfo.Italic,
				_Destination->data.box.fontinfo.Underline,
				_Destination->data.box.fontinfo.StrikeOut,
				_Destination->data.box.fontinfo.name
			);
		}
		if (_Source->data.box.hIcon)
		{
			_Destination->data.box.hIcon = CopyIcon(_Source->data.box.hIcon);
		}
		if (_Source->data.box.hCursor)
		{
			_Destination->data.box.hCursor = CopyCursor(_Source->data.box.hCursor);
		}

		break;
	}
	return TRUE;
}


BOOL lrCopyAliasEx(void* _Destination, const void* _Source, ALIASTYPE _AliasType)
{
	if (_Destination == NULL || _Source == NULL) return FALSE;

	switch (_AliasType)
	{
	case alias_type_win:

		(HWND*)_Destination = (HWND*)_Source;
		break;

	case alias_type_path:

		memcpy(_Destination, _Source, sizeof(ALIAS_PATH));
		break;

	case alias_type_out:

		memcpy(_Destination, _Source, sizeof(ALIAS_OUT));

		if (((ALIAS_BOX*)_Source)->hFont)
		{
			((ALIAS_OUT*)_Destination)->hFont = lr_CreateFont(
				((ALIAS_OUT*)_Destination)->fontinfo.height,
				((ALIAS_OUT*)_Destination)->fontinfo.width,
				((ALIAS_OUT*)_Destination)->fontinfo.weight,
				((ALIAS_OUT*)_Destination)->fontinfo.Italic,
				((ALIAS_OUT*)_Destination)->fontinfo.Underline,
				((ALIAS_OUT*)_Destination)->fontinfo.StrikeOut,
				((ALIAS_OUT*)_Destination)->fontinfo.name
			);
		}
		break;

	case alias_type_draw:

		memcpy(_Destination, _Source, sizeof(ALIAS_DRAW));
		break;

	case alias_type_box:

		memcpy(_Destination, _Source, sizeof(ALIAS_BOX));

		if (((ALIAS_BOX*)_Source)->hFont)
		{
			((ALIAS_BOX*)_Destination)->hFont = lr_CreateFont(
				((ALIAS_BOX*)_Destination)->fontinfo.height,
				((ALIAS_BOX*)_Destination)->fontinfo.width,
				((ALIAS_BOX*)_Destination)->fontinfo.weight,
				((ALIAS_BOX*)_Destination)->fontinfo.Italic,
				((ALIAS_BOX*)_Destination)->fontinfo.Underline,
				((ALIAS_BOX*)_Destination)->fontinfo.StrikeOut,
				((ALIAS_BOX*)_Destination)->fontinfo.name
			);
		}
		if (((ALIAS_BOX*)_Source)->hIcon)
		{
			((ALIAS_BOX*)_Destination)->hIcon = CopyIcon(((ALIAS_BOX*)_Source)->hIcon);
		}
		if (((ALIAS_BOX*)_Source)->hCursor)
		{
			((ALIAS_BOX*)_Destination)->hCursor = CopyCursor(((ALIAS_BOX*)_Source)->hCursor);
		}
		break;
		
	default: return FALSE;
	}
	return TRUE;
}

/* 获取别名类型字符串。
* 
* 参数:
*		_aliastype     |  别名的类型
* 
* 返回值:
*		返回别名类型的字符串名称（常量）。
*/
const char* const lrGetAliasTypeString(ALIASTYPE _aliastype)
{
	switch (_aliastype)
	{
	case alias_type_null:
		return ALIASTYPE_STR_NULL;
	case alias_type_win:
		return ALIASTYPE_STR_WIN;
	case alias_type_path:
		return ALIASTYPE_STR_PATH;
	case alias_type_out:
		return ALIASTYPE_STR_OUT;
	case alias_type_draw:
		return ALIASTYPE_STR_DRAW;
	case alias_type_box:
		return ALIASTYPE_STR_BOX;
	default:
		return "\0";
	}
}


/* 获取预定义的别名数据。
* 
* 参数:
*		_aliasdef     |  别名的预定义类型
*		_pa           |  别名载体
* 
* 返回值:
*		成功返回 TRUE，否则返回 FALSE。
*/
BOOL lrGetAliasDef(ALIASDEF _aliasdef, PALIASSINGLE _pa)
{
	if (_pa == NULL) return FALSE;
	memset(_pa, 0, sizeof(ALIASSINGLE));

	switch (_aliasdef)
	{
	case alias_def_null:
		_pa->type = alias_type_null;
		break;
	case alias_def_out:
		_pa->type = alias_type_out;
		_pa->data.out.BackColor = RGB(0, 0, 0);
		_pa->data.out.TextColor = RGB(0, 0xff, 0);
		_pa->data.out.IsTransparent = TRUE;
		_pa->data.out.fontinfo.height = 50;
		_pa->data.out.fontinfo.width = 50;
		_pa->data.out.fontinfo.weight = 400;
		_pa->data.out.hFont = lr_CreateFont(
			_pa->data.out.fontinfo.height,
			_pa->data.out.fontinfo.width,
			_pa->data.out.fontinfo.weight,
			FALSE, FALSE, FALSE, NULL);
		break;
	case alias_def_draw:
		_pa->type = alias_type_draw;
		_pa->data.draw.Fill = FALSE;
		_pa->data.draw.FillColor = RGB(0, 0, 0);
		_pa->data.draw.StrokeColor = RGB(0x00, 0xff, 0x00);
		_pa->data.draw.StrokeWidth = 1;
		_pa->data.draw.FillImage[0] = '\0';
		break;
	case alias_def_box_blank:
		_pa->type = alias_type_box;
		strcpy(_pa->data.box.ClassName, lrCLASSNAME_BLANKBOX);
		_pa->data.box.TextColor = RGB(0x00, 0x00, 0x00);
		_pa->data.box.BackColor = RGB(0xff, 0xff, 0xff);
		_pa->data.box.fontinfo.height = 30;
		_pa->data.box.fontinfo.width = 20;
		_pa->data.box.fontinfo.weight = 400;
		_pa->data.box.fontinfo.Italic = FALSE;
		_pa->data.box.fontinfo.Underline = FALSE;
		_pa->data.box.fontinfo.StrikeOut = FALSE;
		strcpy(_pa->data.box.fontinfo.name, "Cascadia Mono");
		_pa->data.box.hCursor = LoadCursorA(NULL, IDC_ARROW);
		_pa->data.box.hIcon = LoadIconA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDI_ICON_CHILDMIN));
		_pa->data.box.hFont = lr_CreateFont(
			_pa->data.box.fontinfo.height,
			_pa->data.box.fontinfo.width,
			_pa->data.box.fontinfo.weight,
			_pa->data.box.fontinfo.Italic,
			_pa->data.box.fontinfo.Underline,
			_pa->data.box.fontinfo.StrikeOut,
			_pa->data.box.fontinfo.name
		);
		break;
	case alias_def_box_text:
		_pa->type = alias_type_box;
		strcpy(_pa->data.box.ClassName, lrCLASSNAME_TEXTBOX);
		_pa->data.box.TextColor = RGB(0x00, 0x00, 0x00);
		_pa->data.box.BackColor = RGB(0xff, 0xff, 0xff);
		_pa->data.box.fontinfo.height = 30;
		_pa->data.box.fontinfo.width = 20;
		_pa->data.box.fontinfo.weight = 400;
		_pa->data.box.fontinfo.Italic = FALSE;
		_pa->data.box.fontinfo.Underline = FALSE;
		_pa->data.box.fontinfo.StrikeOut = FALSE;
		strcpy(_pa->data.box.fontinfo.name, "Cascadia Mono");
		_pa->data.box.hCursor = LoadCursorA(NULL, IDC_ARROW);
		_pa->data.box.hIcon = LoadIconA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDI_ICON_CHILDMIN));
		_pa->data.box.hFont = lr_CreateFont(
			_pa->data.box.fontinfo.height,
			_pa->data.box.fontinfo.width,
			_pa->data.box.fontinfo.weight,
			_pa->data.box.fontinfo.Italic,
			_pa->data.box.fontinfo.Underline,
			_pa->data.box.fontinfo.StrikeOut,
			_pa->data.box.fontinfo.name
		);
		break;
	case alias_def_box_log:
		_pa->type = alias_type_box;
		strcpy(_pa->data.box.ClassName, lrCLASSNAME_LOGBOX);
		_pa->data.box.TextColor = RGB(0x00, 0x00, 0x00);
		_pa->data.box.BackColor = RGB(0xff, 0xff, 0xff);
		_pa->data.box.fontinfo.height = 15;
		_pa->data.box.fontinfo.width = 0;
		_pa->data.box.fontinfo.weight = 100;
		_pa->data.box.fontinfo.Italic = FALSE;
		_pa->data.box.fontinfo.Underline = FALSE;
		_pa->data.box.fontinfo.StrikeOut = FALSE;
		strcpy(_pa->data.box.fontinfo.name, "Cascadia Mono");
		_pa->data.box.hCursor = LoadCursorA(NULL, IDC_ARROW);
		_pa->data.box.hIcon = LoadIconA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDI_ICON_CHILDMIN));
		_pa->data.box.hFont = lr_CreateFont(
			_pa->data.box.fontinfo.height,
			_pa->data.box.fontinfo.width,
			_pa->data.box.fontinfo.weight,
			_pa->data.box.fontinfo.Italic,
			_pa->data.box.fontinfo.Underline,
			_pa->data.box.fontinfo.StrikeOut,
			_pa->data.box.fontinfo.name
		);
		break;
	case alias_def_box_info:
		_pa->type = alias_type_box;
		strcpy(_pa->data.box.ClassName, lrCLASSNAME_INFOBOX);
		_pa->data.box.TextColor = RGB(0x00, 0xff, 0x00);
		_pa->data.box.BackColor = RGB(0x00, 0x00, 0x00);
		_pa->data.box.fontinfo.height = 15;
		_pa->data.box.fontinfo.width = 0;
		_pa->data.box.fontinfo.weight = 100;
		_pa->data.box.fontinfo.Italic = FALSE;
		_pa->data.box.fontinfo.Underline = FALSE;
		_pa->data.box.fontinfo.StrikeOut = FALSE;
		strcpy(_pa->data.box.fontinfo.name, "Cascadia Mono");
		_pa->data.box.hCursor = LoadCursorA(NULL, IDC_ARROW);
		_pa->data.box.hIcon = LoadIconA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDI_ICON_CHILDMIN));
		_pa->data.box.hFont = lr_CreateFont(
			_pa->data.box.fontinfo.height,
			_pa->data.box.fontinfo.width,
			_pa->data.box.fontinfo.weight,
			_pa->data.box.fontinfo.Italic,
			_pa->data.box.fontinfo.Underline,
			_pa->data.box.fontinfo.StrikeOut,
			_pa->data.box.fontinfo.name
		);
		break;
	case alias_def_box_image:
		_pa->type = alias_type_box;
		strcpy(_pa->data.box.ClassName, lrCLASSNAME_IMAGEBOX);
		_pa->data.box.TextColor = RGB(0x00, 0xff, 0x00);
		_pa->data.box.BackColor = 0;
		_pa->data.box.fontinfo.height = 20;
		_pa->data.box.fontinfo.width = 0;
		_pa->data.box.fontinfo.weight = 400;
		_pa->data.box.fontinfo.Italic = FALSE;
		_pa->data.box.fontinfo.Underline = FALSE;
		_pa->data.box.fontinfo.StrikeOut = FALSE;
		strcpy(_pa->data.box.fontinfo.name, "Cascadia Mono");
		_pa->data.box.hCursor = LoadCursorA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDC_CURSOR_IMAGEBOX));
		_pa->data.box.hIcon = LoadIconA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDI_ICON_CHILDMIN));
		_pa->data.box.hFont = lr_CreateFont(
			_pa->data.box.fontinfo.height,
			_pa->data.box.fontinfo.width,
			_pa->data.box.fontinfo.weight,
			_pa->data.box.fontinfo.Italic,
			_pa->data.box.fontinfo.Underline,
			_pa->data.box.fontinfo.StrikeOut,
			_pa->data.box.fontinfo.name
		);
		break;
	case alias_def_box_blue:
		_pa->type = alias_type_box;
		strcpy(_pa->data.box.ClassName, lrCLASSNAME_BLUEBOX);
		_pa->data.box.TextColor = RGB(0xff, 0xff, 0xff);
		_pa->data.box.BackColor = RGB(0x00, 0x00, 0xff);
		_pa->data.box.fontinfo.height = 5;
		_pa->data.box.fontinfo.width = 0;
		_pa->data.box.fontinfo.weight = 400;
		_pa->data.box.fontinfo.Italic = FALSE;
		_pa->data.box.fontinfo.Underline = FALSE;
		_pa->data.box.fontinfo.StrikeOut = FALSE;
		strcpy(_pa->data.box.fontinfo.name, "Cascadia Mono");
		_pa->data.box.hCursor = LoadCursorA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDC_CURSOR_IMAGEBOX));
		_pa->data.box.hIcon = LoadIconA(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDI_ICON_CHILDMIN));
		_pa->data.box.hFont = lr_CreateFont(
			_pa->data.box.fontinfo.height,
			_pa->data.box.fontinfo.width,
			_pa->data.box.fontinfo.weight,
			_pa->data.box.fontinfo.Italic,
			_pa->data.box.fontinfo.Underline,
			_pa->data.box.fontinfo.StrikeOut,
			_pa->data.box.fontinfo.name
		);
		break;
	}
	return TRUE;
}



/* 打印别名数据树。
* 
* 参数:
*		_Alias        |  指向一个别名
*/
void lrPrintAliasTree(PALIASSINGLE _Alias)
{
	if (_Alias == NULL) return;

	wdo_log_language(OUTLOG_NORMAL,
		"Data from the alias # %s\n"
		"│─TYPE> %s\n"
		"└─DATA\n"
		,
		"别名名称 # %s\n"
		"│─类型> %s\n"
		"└─数据\n"
		, _Alias->name
		, lrGetAliasTypeString(_Alias->type)
	);

	switch (_Alias->type)
	{
	case alias_type_win:
		wdo_log_language(OUTLOG_NORMAL,
			"   └─" ALIASTYPE_STR_WIN "\n"
			"      └─HWND> _%X_\n"
			,
			"   └─" ALIASTYPE_STR_WIN "\n"
			"      └─窗口句柄> _%X_\n"
			, _Alias->data.win
		);
		break;
	case alias_type_path:
		wdo_log_language(OUTLOG_NORMAL,
			"   └─" ALIASTYPE_STR_PATH "\n"
			"      │─PATH> %s\n"
			"      └─TYPE> %s\n"
			,
			"   └─" ALIASTYPE_STR_PATH "\n"
			"      │─路径> %s\n"
			"      └─类型> %s\n"
			, _Alias->data.path.Path
			, _Alias->data.path.Type == PATHTYPE_FILE ? wdo_language("File", "文件") : (_Alias->data.path.Type == PATHTYPE_DIR ? wdo_language("Dir", "目录") : "Unknown")
		);
		break;
	case alias_type_out:
		wdo_log_language(OUTLOG_NORMAL,
			"   └─" ALIASTYPE_STR_OUT "\n"
			"      │─ImagePath> %s\n"
			"      │─ImageInTextPos\n"
			"      │ │─X> %d\n"
			"      │ └─Y> %d\n"
			"      │─Font\n"
			"      │ │─Width> %u\n"
			"      │ │─Height> %u\n"
			"      │ │─Weight> %u\n"
			"      │ │─Italic> %s\n"
			"      │ │─Underline> %s\n"
			"      │ │─StrikeOut> %s\n"
			"      │ └─FontName> %s\n"
			"      │─TextColor> %06X\n"
			"      │─BackColor> %06X\n"
			"      └─IsTransparent> %s\n"
			,
			"   └─" ALIASTYPE_STR_OUT "\n"
			"      │─图像路径> %s\n"
			"      │─图像在文本坐标\n"
			"      │ │─X> %d\n"
			"      │ └─Y> %d\n"
			"      │─字体\n"
			"      │ │─宽度> %u\n"
			"      │ │─高度> %u\n"
			"      │ │─厚度> %u\n"
			"      │ │─斜体> %s\n"
			"      │ │─下划线> %s\n"
			"      │ │─删除线> %s\n"
			"      │ └─字体名称> %s\n"
			"      │─文本颜色> %06X\n"
			"      │─背景颜色> %06X\n"
			"      └─背景透明> %s\n"
			, _Alias->data.out.image
			, _Alias->data.out.ImageInTextPos.X
			, _Alias->data.out.ImageInTextPos.Y
			, _Alias->data.out.fontinfo.width
			, _Alias->data.out.fontinfo.height
			, _Alias->data.out.fontinfo.weight
			, _Alias->data.out.fontinfo.Italic ? "True" : "False"
			, _Alias->data.out.fontinfo.Underline ? "True" : "False"
			, _Alias->data.out.fontinfo.StrikeOut ? "True" : "False"
			, _Alias->data.out.fontinfo.name
			, _Alias->data.out.TextColor
			, _Alias->data.out.BackColor
			, _Alias->data.out.IsTransparent ? "True" : "False"
		);
		break;
	case alias_type_draw:
		wdo_log_language(OUTLOG_NORMAL,
			"   └─" ALIASTYPE_STR_DRAW "\n"
			"      │─StrokeColor> %06X\n"
			"      │─StrokeWidth> %d\n"
			"      │─Fill> %s\n"
			"      │─FillColor> %06X\n"
			"      └─FillImage> %s\n"
			,
			"   └─" ALIASTYPE_STR_DRAW "\n"
			"      │─描边颜色> %06X\n"
			"      │─描边宽度> %d\n"
			"      │─是否填充> %s\n"
			"      │─填充颜色> %06X\n"
			"      └─用于填充的图像> %s\n"
			, lr_TO_RGB(_Alias->data.draw.StrokeColor)
			, _Alias->data.draw.StrokeWidth
			, _Alias->data.draw.Fill
			, lr_TO_RGB(_Alias->data.draw.FillColor)
			, _Alias->data.draw.FillImage
		);
		break;
	case alias_type_box:
		wdo_log_language(OUTLOG_NORMAL,
			"   └─" ALIASTYPE_STR_BOX "\n"
			"      │─ClassName> %s\n"
			"      │─hCursor> %X\n"
			"      │─hIcon> %X\n"
			"      │─Font\n"
			"      │ │─Width> %u\n"
			"      │ │─Height> %u\n"
			"      │ │─Weight> %u\n"
			"      │ │─Italic> %s\n"
			"      │ │─Underline> %s\n"
			"      │ │─StrikeOut> %s\n"
			"      │ └─FontName> %s\n"
			"      │─TextColor> %06X\n"
			"      └─BackColor> %06X\n"
			,
			"   └─" ALIASTYPE_STR_BOX "\n"
			"      │─窗口类名> %s\n"
			"      │─游标句柄> %X\n"
			"      │─图标句柄> %X\n"
			"      │─字体\n"
			"      │ │─宽度> %u\n"
			"      │ │─高度> %u\n"
			"      │ │─厚度> %u\n"
			"      │ │─斜体> %s\n"
			"      │ │─下划线> %s\n"
			"      │ │─删除线> %s\n"
			"      │ └─字体名称> %s\n"
			"      │─文本颜色> %06X\n"
			"      └─背景颜色> %06X\n"
			, _Alias->data.box.ClassName
			, _Alias->data.box.hCursor
			, _Alias->data.box.hIcon
			, _Alias->data.box.fontinfo.width
			, _Alias->data.box.fontinfo.height
			, _Alias->data.box.fontinfo.weight
			, _Alias->data.box.fontinfo.Italic ? "on" : "off"
			, _Alias->data.box.fontinfo.Underline ? "on" : "off"
			, _Alias->data.box.fontinfo.StrikeOut ? "on" : "off"
			, _Alias->data.box.fontinfo.name
			, lr_TO_RGB(_Alias->data.box.TextColor)
			, lr_TO_RGB(_Alias->data.box.BackColor)
		);
		break;
	}
	return;
}



/* 将别名放入别名库或从别名库中删除别名。
* 
* 参数:
*		_AliasInfo       |  要操作的别名，如果 _Delete 为 TRUE 则只需要设置 _AliasInfo.name
*		_Delete          |  如果为 TRUE 则删除别名，如果为 FALSE 则放入别名
*/
BOOL lrPutAliasLibrary(PALIASSINGLE _AliasInfo, BOOL _Delete)
{
	if (_AliasInfo == NULL || _AliasInfo->name[0] == '\0' || !lrVerifyAliasName(_AliasInfo->name, FALSE)) return FALSE;

	/* 放入别名 */
	if (!_Delete)
	{
		/* 如果内存为 0 时，申请一内存地址 */
		if (AliasLibraryCount == 0)
		{
			if ((AliasLibrary = (ALIASSINGLE*)malloc((++AliasLibraryCount) * sizeof(ALIASSINGLE))) == NULL)
			{
				--AliasLibraryCount;
				return FALSE;
			}

			AliasLibrary[AliasLibraryCount - 1] = *_AliasInfo;
			return TRUE;
		}
		else
		{
			/* 检索是否有重名，如果有则覆盖 */
			for (size_t i = 0; i < AliasLibraryCount; ++i)
			{
				if (!strcmp_wdoalias(AliasLibrary[i].name, _AliasInfo->name))
				{
					lrClearAlias(&AliasLibrary[i]);
					AliasLibrary[i] = *_AliasInfo;

					return TRUE;
				}
			}
			
			/* 扩展内存，只扩展一个位置 */
			ALIASSINGLE* NewAliasLibrary = (ALIASSINGLE*)realloc(AliasLibrary, (++AliasLibraryCount) * sizeof(ALIASSINGLE));
			if (NewAliasLibrary != NULL) AliasLibrary = NewAliasLibrary;
			else
			{
				--AliasLibrary;
				return FALSE;
			}

			AliasLibrary[AliasLibraryCount - 1] = *_AliasInfo;
			return TRUE;
		}
	}
	/* 删除别名 */
	else
	{
		/* 查找别名并将其置空 */
		for (size_t i = 0; i < AliasLibraryCount; ++i) {
			if (!strcmp_wdoalias(AliasLibrary[i].name, _AliasInfo->name))
			{
				/* 将最后一个别名填充到此处，别名库大小 -1 */
				AliasLibrary[i] = AliasLibrary[(--AliasLibraryCount)];
			}
		}
		/* 如果只剩最后一个别名则释放所有内存 */
		if (AliasLibraryCount == 0)
		{
			free(AliasLibrary);
			AliasLibraryCount = 0;
			AliasLibrary = NULL;
		}
	}

	return TRUE;
}




/* 获取当前别名库。
*
* 参数:
*		_AliasCount   |  接收别名库的大小，也是别名库中别名的数量，如果为 NULL 则不接收
*
* 返回值:
*		当前的别名库，如果别名库为 NULL 则返回 NULL。
*		不需要释放。
*/
const ALIASSINGLE* lrGetAliasLibrary(size_t* _AliasCount)
{
	if (_AliasCount) *_AliasCount = AliasLibraryCount;

	return AliasLibrary;
}


/* 设置当前别名库中的别名数据。
*
* 参数:
*		_Number       |  要设置的别名库中的别名编号
*
* 返回值:
*		返回请求的别名指针，可以通过指针直接修改别名数据，
*		不需要释放。
*/
ALIASSINGLE* lrSetAliasLibrary(size_t _Number)
{
	if (_Number >= AliasLibraryCount) return NULL;

	return &(AliasLibrary[_Number]);
}



/* 在别名库中查找别名。
* 
* 参数:
*		_Alias          |  要查找的别名字符串，不超过 ALIAS_MAX_LENGTH 个字符
*/
BOOL lrFindAliasInAliasLibary(LPCSTR _Name, ALIASSINGLE* _Alias)
{
	if (_Name == NULL || _Name[0] == '\0' || _Alias == NULL || lrVerifyAliasName(_Name, FALSE) == FALSE) return FALSE;

	for (size_t i = 0; i < AliasLibraryCount; ++i)
	{
		if (!strcmp_wdoalias(AliasLibrary[i].name, _Name))
		{
			*_Alias = AliasLibrary[i];
			return TRUE;
		}
	}

	/* 如果别名为一点，则类型自动填充为窗口类型，并且为当前 PTR 指向的窗口 */
	if (!strcmp_wdoalias(_Name, "."))
	{
		_Alias->type = alias_type_win;
		_Alias->data.win = *(lrWindowDo(NULL, WDOECHO_FALSE).PtrWindow);
		return TRUE;
	}
	/* 如果别名为两点，则类型自动填充为窗口类型，并且为当前控制台窗口 */
	if (!strcmp_wdoalias(_Name, ".."))
	{
		_Alias->type = alias_type_win;
		_Alias->data.win = wdo_gethwnd(WDOGETHWND_CONTROL);
		return TRUE;
	}

	return FALSE;
}




/* 从指令中查找别名，并检索别名库数据，截断指令。
* 
* 参数:
*		_Instruction             |  指令字符串，截断 ALIAS_SEPARATOR 前的字符串作为别名检索别名库
*		_IndexArray              |  接收指令中出现的别名在别名库中的下标集合，需要释放
* 
* 返回值:
*		如果成功，返回别名库中对应的别名数据。
*		如果失败，返回一个新别名，但不在别名库中。
*/
ALIAS lrFindAliasInInstruction(char* _Instruction, size_t** _IndexArray, size_t* _IndexCount, BYTE* _Bools)
{
	ALIAS ret = { 0 };
	ret.type = alias_type_null;
	

	if (_Instruction == NULL || _Instruction[0] == '\0') return ret;

	/* 指令的长度 */
	size_t len = strlen(_Instruction) + 1;

	/* 如果两个连续的点，则使用控制窗口 */
	if (_Instruction[0] == ALIAS_SEPARATOR && _Instruction[1] == ALIAS_SEPARATOR)
	{
		lrstrmov(_Instruction, 2, '\0', len - 1);
		len -= 2;

		ret.type = alias_type_win;
		ret.data.win = wdo_gethwnd(WDOGETHWND_CONTROL);
	}
	/* 如果第一位就是分隔符则直接左移一位 */
	else if (_Instruction[0] == ALIAS_SEPARATOR)
	{
		lrstrmov(_Instruction, 1, '\0', --len);
	}

	if (AliasLibraryCount == 0) return ret;

	if (_IndexCount) *_IndexCount = 0;
	if (_IndexArray) *_IndexArray = NULL;

	if (_Bools) memset(_Bools, FALSE, ALIASTYPECOUNT * sizeof(BYTE));

	while (1)
	{
		char name[ALIAS_MAX_LENGTH] = { '\0' };
		size_t pos = 0;
		for (; pos < len; ++pos)
		{
			if (pos < sizeof(name))
			{
				if (_Instruction[pos] == ALIAS_SEPARATOR)
				{
					++pos;
					break;
				}
				else name[pos] = _Instruction[pos];
			}
			else if (_Instruction[pos] == ALIAS_SEPARATOR)
			{
				++pos;
				break;
			}
		}
		if (pos == len) break;

		lrstrmov(_Instruction, (int)pos, '\0', len - 1);
		len -= pos;
		for (size_t i = 0; i < AliasLibraryCount; ++i)
		{
			if (!strcmp_wdoalias(AliasLibrary[i].name, name))
			{
				ret.type = AliasLibrary[i].type;

				if (_IndexArray && _IndexCount)
				{
					++(*_IndexCount);
					size_t* temp = (size_t*)realloc(*_IndexArray, (*_IndexCount) * sizeof(size_t));
					if (temp)
					{
						*_IndexArray = temp;
						(*_IndexArray)[*_IndexCount - 1] = i;
					}
					else --(*_IndexCount);
				}

				switch (AliasLibrary[i].type)
				{
				case alias_type_win:
					ret.data.win = AliasLibrary[i].data.win;
					break;

				case alias_type_path:
					ret.data.path = AliasLibrary[i].data.path;
					break;

				case alias_type_out:
					ret.data.out = AliasLibrary[i].data.out;
					break;

				case alias_type_draw:
					ret.data.draw = AliasLibrary[i].data.draw;
					break;

				case alias_type_box:
					ret.data.box = AliasLibrary[i].data.box;
					break;
				}
				if (_Bools && AliasLibrary[i].type < ALIASTYPECOUNT) _Bools[AliasLibrary[i].type] = TRUE;
			}
		}
	}
	return ret;
}




/* 保存别名为文件。
* 
* 参数:
*		_Path            |  保存路径，带文件名
*		_SaveLibary      |  是否保存别名库，TRUE 忽略 _Alias
*		_Alias           |  指向要保存的别名
* 
* 返回值:
*		成功返回 TRUE，失败返回 FALSE。
*/
BOOL lrSaveAlias(LPCSTR _Path, BOOL _SaveLibary, PALIASSINGLE _Alias)
{
	if ((_Path == NULL || _Path[0] == '\0')
		|| (_SaveLibary == FALSE && _Alias == NULL)
		|| (_SaveLibary == TRUE && AliasLibraryCount == 0)) return FALSE;

	char path[MAX_PATH];

	/* 判断后缀名 */
	if (strcpy(path, _Path))
	{
		char ext[_MAX_EXT];
		_splitpath(path, NULL, NULL, NULL, ext);

		if (stricmp(ext, FALIAS_EXT)) strcat(path, FALIAS_EXT);
	}

	FALIAS fAlias = { 0 };
	BOOL Valid = FALSE;

	/* 读取原文内容 */
	FILE* fp = fopen(path, "r");
	if (fp)
	{
		lr_srand();

		fread(&fAlias.fAliasHeader, sizeof(FALIAS_HEADER), 1, fp);

		if (!strncmp(fAlias.fAliasHeader.Header, FALIAS_HEADER_STRID, sizeof(FALIAS_HEADER_STRID)))
		{
			Valid = TRUE;

			fAlias.fAliasTexts = (FALIAS_TEXT*)malloc(fAlias.fAliasHeader.AliasCount * sizeof(FALIAS_TEXT));

			if (fAlias.fAliasTexts)
			{
				fread(fAlias.fAliasTexts, sizeof(FALIAS_TEXT), fAlias.fAliasHeader.AliasCount, fp);
			}
			else
			{
				fclose(fp);
				return FALSE;
			}
			fAlias.fAliasHeader.ChangeTime = time(NULL);
			fAlias.fAliasHeader.VerID = WDO_VER_ID;
		}
		else memset(&fAlias, 0, sizeof(fAlias));

		fclose(fp);
	}

	/* 给文件头赋值 */
	if (Valid == FALSE)
	{
		strcpy(fAlias.fAliasHeader.Header, FALIAS_HEADER_STRID);
		fAlias.fAliasHeader.AliasCount = 0;
		fAlias.fAliasHeader.SaveTime = fAlias.fAliasHeader.ChangeTime = time(NULL);
		srand((unsigned int)(fAlias.fAliasHeader.SaveTime));
		fAlias.fAliasHeader.VerID = WDO_VER_ID;
		fAlias.fAliasHeader.UserSecret = (UINT)rand();

		char* UserName = getenv("UserName");
		memset(fAlias.fAliasHeader.UserName, '\0', sizeof(fAlias.fAliasHeader.UserName));
		for (size_t i = 0; i < sizeof(fAlias.fAliasHeader.UserName) - 1; ++i)
		{
			fAlias.fAliasHeader.UserName[i] = UserName[i];
		}

		srand(fAlias.fAliasHeader.UserSecret);

		for (size_t i = 0; i < sizeof(fAlias.fAliasHeader.UserName); ++i)
		{
			fAlias.fAliasHeader.UserName[i] = fAlias.fAliasHeader.UserName[i] + (fAlias.fAliasHeader.UserSecret & rand());
		}
	}

	/* 写入内容 */
	if ((fp = fopen(path, "w")) == NULL)
	{
		if (Valid && fAlias.fAliasTexts) free(fAlias.fAliasTexts);
		return FALSE;
	}

	/* 写入别名库 */
	if (_SaveLibary)
	{
		if (Valid)
		{
			fwrite(&fAlias.fAliasHeader, sizeof(FALIAS_HEADER), 1, fp);

			if (fAlias.fAliasTexts)
			{
				fwrite(fAlias.fAliasTexts, sizeof(FALIAS_TEXT), fAlias.fAliasHeader.AliasCount, fp);
			}
			if (AliasLibrary)
			{
				fwrite(AliasLibrary, sizeof(FALIAS_TEXT), AliasLibraryCount, fp);
			}

			fclose(fp);
		}
		else
		{
			fAlias.fAliasTexts = lrGetAliasLibrary((size_t*)&fAlias.fAliasHeader.AliasCount);

			fwrite(&fAlias.fAliasHeader, sizeof(fAlias.fAliasHeader), 1, fp);

			if (fAlias.fAliasTexts)
				fwrite(fAlias.fAliasTexts, sizeof(*fAlias.fAliasTexts), fAlias.fAliasHeader.AliasCount, fp);

			fclose(fp);
		}
	}
	/* 写入指定的别名 */
	else if (_Alias)
	{
		if (Valid)
		{
			++fAlias.fAliasHeader.AliasCount;
		}
		fwrite(&fAlias.fAliasHeader, sizeof(FALIAS_HEADER), 1, fp);

		if (Valid && fAlias.fAliasTexts && fAlias.fAliasHeader.AliasCount - 1)
		{
			fwrite(fAlias.fAliasTexts, sizeof(FALIAS_TEXT), (size_t)fAlias.fAliasHeader.AliasCount - 1, fp);
		}
		fwrite(_Alias, sizeof(ALIASSINGLE), 1, fp);

		fclose(fp);
	}
	else return FALSE;

	return TRUE;
}




/* 加载储存别名的文件。
* 
* 参数:
*		_Path            |  别名文件的完整路径，允许不带后缀
*		_fAlias          |  接收别名文件数据
* 
* 返回值:
*		成功返回 TRUE，失败返回 FALSE。
* 
* 言论:
*		_Aliases 需要释放，它的一级指针数量在 _fAlias->fAliasHeader.AliasCount 中。
*/
BOOL lrLoadAlias(LPCSTR _Path, FALIAS* _fAlias)
{
	if (_Path == NULL || _fAlias == NULL || _Path[0] == '\0') return FALSE;
	
	char path[_MAX_PATH] = "\0";
	char ext[_MAX_EXT] = "\0";

	strcpy(path, _Path);
	_splitpath(_Path, NULL, NULL, NULL, ext);

	if (stricmp(ext, FALIAS_EXT)) return FALSE;

	else if (ext[0] == '\0') strcat(path, FALIAS_EXT);


	FILE* fp = fopen(path, "r");
	if (fp)
	{
		fread(&(_fAlias->fAliasHeader), sizeof(FALIAS_HEADER), 1, fp);
		if (strncmp(_fAlias->fAliasHeader.Header, FALIAS_HEADER_STRID, sizeof(FALIAS_HEADER_STRID)))
		{
			fclose(fp);
			return FALSE;
		}

		if (_fAlias->fAliasHeader.AliasCount == 0 || (_fAlias->fAliasTexts = (FALIAS_TEXT*)malloc(_fAlias->fAliasHeader.AliasCount * sizeof(FALIAS_TEXT))) == NULL)
		{
			fclose(fp);
			return FALSE;
		}

		fread(_fAlias->fAliasTexts, sizeof(FALIAS_TEXT), _fAlias->fAliasHeader.AliasCount, fp);

		srand(_fAlias->fAliasHeader.UserSecret);

		for (size_t i = 0; i < sizeof(_fAlias->fAliasHeader.UserName); ++i)
		{
			_fAlias->fAliasHeader.UserName[i] = _fAlias->fAliasHeader.UserName[i] - (_fAlias->fAliasHeader.UserSecret & rand());
		}
		_fAlias->fAliasHeader.UserSecret = 0;

		fclose(fp);
	}
	else return FALSE;

	return TRUE;
}

#undef strcmp_wdoalias