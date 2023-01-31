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


/* ��֤���������Ƿ���Ч
* 
* ����:
*		_AliasName    |  ��Ҫ��֤������
*		_CorrectName  |  ���Ϊ TRUE����������������
* 
* ����ֵ:
*		���� _AliasName �����Ƿ���Ч����Ч���� TRUE��
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
		/* ��һλ����Ϊ���� */
		if (_AliasName[0] >= '0' && _AliasName[0] <= '9') return FALSE;

		size_t len = strlen(_AliasName);

		/* ������� */
		if (len + 1 > ALIAS_MAX_LENGTH) return FALSE;

		for (size_t i = 0; i < len; ++i)
		{
			/* ֻ���� a-z, A-Z, 0-9, '_' ���� */
			if (isalnum(_AliasName[i]) == 0 && _AliasName[i] != '_')
			{
				return FALSE;
			}
		}
		return TRUE;
	}
}


/* ��ʼ��������
* 
* ����:
*		_Name   |  ����������
*		_Type   |  ����������
*		_Alias  |  ָ��Ҫ��ʼ���ı���
* 
* ����:
*		ʹ�ô˺�������ʹ�� lrClearAlias() ��ձ�����
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


/* ��ձ�����
*
* ����:
*		_Alias         |  ָ����յı���
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

/* ��ȡ���������ַ�����
* 
* ����:
*		_aliastype     |  ����������
* 
* ����ֵ:
*		���ر������͵��ַ������ƣ���������
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


/* ��ȡԤ����ı������ݡ�
* 
* ����:
*		_aliasdef     |  ������Ԥ��������
*		_pa           |  ��������
* 
* ����ֵ:
*		�ɹ����� TRUE�����򷵻� FALSE��
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



/* ��ӡ������������
* 
* ����:
*		_Alias        |  ָ��һ������
*/
void lrPrintAliasTree(PALIASSINGLE _Alias)
{
	if (_Alias == NULL) return;

	wdo_log_language(OUTLOG_NORMAL,
		"Data from the alias # %s\n"
		"����TYPE> %s\n"
		"����DATA\n"
		,
		"�������� # %s\n"
		"��������> %s\n"
		"��������\n"
		, _Alias->name
		, lrGetAliasTypeString(_Alias->type)
	);

	switch (_Alias->type)
	{
	case alias_type_win:
		wdo_log_language(OUTLOG_NORMAL,
			"   ����" ALIASTYPE_STR_WIN "\n"
			"      ����HWND> _%X_\n"
			,
			"   ����" ALIASTYPE_STR_WIN "\n"
			"      �������ھ��> _%X_\n"
			, _Alias->data.win
		);
		break;
	case alias_type_path:
		wdo_log_language(OUTLOG_NORMAL,
			"   ����" ALIASTYPE_STR_PATH "\n"
			"      ����PATH> %s\n"
			"      ����TYPE> %s\n"
			,
			"   ����" ALIASTYPE_STR_PATH "\n"
			"      ����·��> %s\n"
			"      ��������> %s\n"
			, _Alias->data.path.Path
			, _Alias->data.path.Type == PATHTYPE_FILE ? wdo_language("File", "�ļ�") : (_Alias->data.path.Type == PATHTYPE_DIR ? wdo_language("Dir", "Ŀ¼") : "Unknown")
		);
		break;
	case alias_type_out:
		wdo_log_language(OUTLOG_NORMAL,
			"   ����" ALIASTYPE_STR_OUT "\n"
			"      ����ImagePath> %s\n"
			"      ����ImageInTextPos\n"
			"      �� ����X> %d\n"
			"      �� ����Y> %d\n"
			"      ����Font\n"
			"      �� ����Width> %u\n"
			"      �� ����Height> %u\n"
			"      �� ����Weight> %u\n"
			"      �� ����Italic> %s\n"
			"      �� ����Underline> %s\n"
			"      �� ����StrikeOut> %s\n"
			"      �� ����FontName> %s\n"
			"      ����TextColor> %06X\n"
			"      ����BackColor> %06X\n"
			"      ����IsTransparent> %s\n"
			,
			"   ����" ALIASTYPE_STR_OUT "\n"
			"      ����ͼ��·��> %s\n"
			"      ����ͼ�����ı�����\n"
			"      �� ����X> %d\n"
			"      �� ����Y> %d\n"
			"      ��������\n"
			"      �� �������> %u\n"
			"      �� �����߶�> %u\n"
			"      �� �������> %u\n"
			"      �� ����б��> %s\n"
			"      �� �����»���> %s\n"
			"      �� ����ɾ����> %s\n"
			"      �� ������������> %s\n"
			"      �����ı���ɫ> %06X\n"
			"      ����������ɫ> %06X\n"
			"      ��������͸��> %s\n"
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
			"   ����" ALIASTYPE_STR_DRAW "\n"
			"      ����StrokeColor> %06X\n"
			"      ����StrokeWidth> %d\n"
			"      ����Fill> %s\n"
			"      ����FillColor> %06X\n"
			"      ����FillImage> %s\n"
			,
			"   ����" ALIASTYPE_STR_DRAW "\n"
			"      ���������ɫ> %06X\n"
			"      ������߿��> %d\n"
			"      �����Ƿ����> %s\n"
			"      ���������ɫ> %06X\n"
			"      ������������ͼ��> %s\n"
			, lr_TO_RGB(_Alias->data.draw.StrokeColor)
			, _Alias->data.draw.StrokeWidth
			, _Alias->data.draw.Fill
			, lr_TO_RGB(_Alias->data.draw.FillColor)
			, _Alias->data.draw.FillImage
		);
		break;
	case alias_type_box:
		wdo_log_language(OUTLOG_NORMAL,
			"   ����" ALIASTYPE_STR_BOX "\n"
			"      ����ClassName> %s\n"
			"      ����hCursor> %X\n"
			"      ����hIcon> %X\n"
			"      ����Font\n"
			"      �� ����Width> %u\n"
			"      �� ����Height> %u\n"
			"      �� ����Weight> %u\n"
			"      �� ����Italic> %s\n"
			"      �� ����Underline> %s\n"
			"      �� ����StrikeOut> %s\n"
			"      �� ����FontName> %s\n"
			"      ����TextColor> %06X\n"
			"      ����BackColor> %06X\n"
			,
			"   ����" ALIASTYPE_STR_BOX "\n"
			"      ������������> %s\n"
			"      �����α���> %X\n"
			"      ����ͼ����> %X\n"
			"      ��������\n"
			"      �� �������> %u\n"
			"      �� �����߶�> %u\n"
			"      �� �������> %u\n"
			"      �� ����б��> %s\n"
			"      �� �����»���> %s\n"
			"      �� ����ɾ����> %s\n"
			"      �� ������������> %s\n"
			"      �����ı���ɫ> %06X\n"
			"      ����������ɫ> %06X\n"
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



/* ����������������ӱ�������ɾ��������
* 
* ����:
*		_AliasInfo       |  Ҫ�����ı�������� _Delete Ϊ TRUE ��ֻ��Ҫ���� _AliasInfo.name
*		_Delete          |  ���Ϊ TRUE ��ɾ�����������Ϊ FALSE ��������
*/
BOOL lrPutAliasLibrary(PALIASSINGLE _AliasInfo, BOOL _Delete)
{
	if (_AliasInfo == NULL || _AliasInfo->name[0] == '\0' || !lrVerifyAliasName(_AliasInfo->name, FALSE)) return FALSE;

	/* ������� */
	if (!_Delete)
	{
		/* ����ڴ�Ϊ 0 ʱ������һ�ڴ��ַ */
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
			/* �����Ƿ���������������򸲸� */
			for (size_t i = 0; i < AliasLibraryCount; ++i)
			{
				if (!strcmp_wdoalias(AliasLibrary[i].name, _AliasInfo->name))
				{
					lrClearAlias(&AliasLibrary[i]);
					AliasLibrary[i] = *_AliasInfo;

					return TRUE;
				}
			}
			
			/* ��չ�ڴ棬ֻ��չһ��λ�� */
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
	/* ɾ������ */
	else
	{
		/* ���ұ����������ÿ� */
		for (size_t i = 0; i < AliasLibraryCount; ++i) {
			if (!strcmp_wdoalias(AliasLibrary[i].name, _AliasInfo->name))
			{
				/* �����һ��������䵽�˴����������С -1 */
				AliasLibrary[i] = AliasLibrary[(--AliasLibraryCount)];
			}
		}
		/* ���ֻʣ���һ���������ͷ������ڴ� */
		if (AliasLibraryCount == 0)
		{
			free(AliasLibrary);
			AliasLibraryCount = 0;
			AliasLibrary = NULL;
		}
	}

	return TRUE;
}




/* ��ȡ��ǰ�����⡣
*
* ����:
*		_AliasCount   |  ���ձ�����Ĵ�С��Ҳ�Ǳ������б��������������Ϊ NULL �򲻽���
*
* ����ֵ:
*		��ǰ�ı����⣬���������Ϊ NULL �򷵻� NULL��
*		����Ҫ�ͷš�
*/
const ALIASSINGLE* lrGetAliasLibrary(size_t* _AliasCount)
{
	if (_AliasCount) *_AliasCount = AliasLibraryCount;

	return AliasLibrary;
}


/* ���õ�ǰ�������еı������ݡ�
*
* ����:
*		_Number       |  Ҫ���õı������еı������
*
* ����ֵ:
*		��������ı���ָ�룬����ͨ��ָ��ֱ���޸ı������ݣ�
*		����Ҫ�ͷš�
*/
ALIASSINGLE* lrSetAliasLibrary(size_t _Number)
{
	if (_Number >= AliasLibraryCount) return NULL;

	return &(AliasLibrary[_Number]);
}



/* �ڱ������в��ұ�����
* 
* ����:
*		_Alias          |  Ҫ���ҵı����ַ����������� ALIAS_MAX_LENGTH ���ַ�
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

	/* �������Ϊһ�㣬�������Զ����Ϊ�������ͣ�����Ϊ��ǰ PTR ָ��Ĵ��� */
	if (!strcmp_wdoalias(_Name, "."))
	{
		_Alias->type = alias_type_win;
		_Alias->data.win = *(lrWindowDo(NULL, WDOECHO_FALSE).PtrWindow);
		return TRUE;
	}
	/* �������Ϊ���㣬�������Զ����Ϊ�������ͣ�����Ϊ��ǰ����̨���� */
	if (!strcmp_wdoalias(_Name, ".."))
	{
		_Alias->type = alias_type_win;
		_Alias->data.win = wdo_gethwnd(WDOGETHWND_CONTROL);
		return TRUE;
	}

	return FALSE;
}




/* ��ָ���в��ұ��������������������ݣ��ض�ָ�
* 
* ����:
*		_Instruction             |  ָ���ַ������ض� ALIAS_SEPARATOR ǰ���ַ�����Ϊ��������������
*		_IndexArray              |  ����ָ���г��ֵı����ڱ������е��±꼯�ϣ���Ҫ�ͷ�
* 
* ����ֵ:
*		����ɹ������ر������ж�Ӧ�ı������ݡ�
*		���ʧ�ܣ�����һ���±����������ڱ������С�
*/
ALIAS lrFindAliasInInstruction(char* _Instruction, size_t** _IndexArray, size_t* _IndexCount, BYTE* _Bools)
{
	ALIAS ret = { 0 };
	ret.type = alias_type_null;
	

	if (_Instruction == NULL || _Instruction[0] == '\0') return ret;

	/* ָ��ĳ��� */
	size_t len = strlen(_Instruction) + 1;

	/* ������������ĵ㣬��ʹ�ÿ��ƴ��� */
	if (_Instruction[0] == ALIAS_SEPARATOR && _Instruction[1] == ALIAS_SEPARATOR)
	{
		lrstrmov(_Instruction, 2, '\0', len - 1);
		len -= 2;

		ret.type = alias_type_win;
		ret.data.win = wdo_gethwnd(WDOGETHWND_CONTROL);
	}
	/* �����һλ���Ƿָ�����ֱ������һλ */
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




/* �������Ϊ�ļ���
* 
* ����:
*		_Path            |  ����·�������ļ���
*		_SaveLibary      |  �Ƿ񱣴�����⣬TRUE ���� _Alias
*		_Alias           |  ָ��Ҫ����ı���
* 
* ����ֵ:
*		�ɹ����� TRUE��ʧ�ܷ��� FALSE��
*/
BOOL lrSaveAlias(LPCSTR _Path, BOOL _SaveLibary, PALIASSINGLE _Alias)
{
	if ((_Path == NULL || _Path[0] == '\0')
		|| (_SaveLibary == FALSE && _Alias == NULL)
		|| (_SaveLibary == TRUE && AliasLibraryCount == 0)) return FALSE;

	char path[MAX_PATH];

	/* �жϺ�׺�� */
	if (strcpy(path, _Path))
	{
		char ext[_MAX_EXT];
		_splitpath(path, NULL, NULL, NULL, ext);

		if (stricmp(ext, FALIAS_EXT)) strcat(path, FALIAS_EXT);
	}

	FALIAS fAlias = { 0 };
	BOOL Valid = FALSE;

	/* ��ȡԭ������ */
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

	/* ���ļ�ͷ��ֵ */
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

	/* д������ */
	if ((fp = fopen(path, "w")) == NULL)
	{
		if (Valid && fAlias.fAliasTexts) free(fAlias.fAliasTexts);
		return FALSE;
	}

	/* д������� */
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
	/* д��ָ���ı��� */
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




/* ���ش���������ļ���
* 
* ����:
*		_Path            |  �����ļ�������·������������׺
*		_fAlias          |  ���ձ����ļ�����
* 
* ����ֵ:
*		�ɹ����� TRUE��ʧ�ܷ��� FALSE��
* 
* ����:
*		_Aliases ��Ҫ�ͷţ�����һ��ָ�������� _fAlias->fAliasHeader.AliasCount �С�
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