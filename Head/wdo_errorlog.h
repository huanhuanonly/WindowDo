// wdo_errorlog.h -- Error log template for lrWindowDo()
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#pragma once

#ifndef _WDO_ERRORLOG_H_
#define _WDO_ERRORLOG_H_

#ifdef _LR_WINDOW_DO_C_

#include "lrWindowDo.h"


/* һЩ���õĴ����ӡ�ĸ�ʽģ�� */

#if WDO_LANGUAGE
	/* ������Чʱ */
#	define WDO_ERROR_NOWINDOW(hwnd)		wdo_log(instruction, OUTLOG_ERROR, "_%X_ is not a valid window!\n", (unsigned __int64)hwnd)
	/* ����Ϊ��ʱ */
#	define WDO_ERROR_NULLPARAM(instr)	wdo_log(instruction, OUTLOG_ERROR, "The \"%s\" parameter is empty! See help %s.\n", instr, instr)
	/* ������Чʱ */
#	define WDO_ERROR_NOPARAM(param)		wdo_log(instruction, OUTLOG_ERROR, "Invalid parameter \"%s\"! See help %s.\n", param, instruction)
	/* ȱ�ٲ���ʱ */
#	define WDO_ERROR_MISSINGPARAM()		wdo_log(instruction, OUTLOG_ERROR, "The instruction missing parameter. See help %s\n", instruction)
	/* ������Чʱ */
#	define WDO_ERROR_NOTYPE(type)		wdo_log(instruction, OUTLOG_ERROR, "Invalid type identifier \"%s\"! See help %s.\n", type, instruction)
	/* �ڴ����ʧ�� */
#	define WDO_ERROR_NOMEMORY()			wdo_log(instruction, OUTLOG_ERROR, "Memory allocation failed! The operation from the \"%s\" instruction cannot be completed.\n", instruction)
	/* �ļ���ʧ�� */
#	define WDO_ERROR_NOOPENFILE(path)	wdo_log(instruction, OUTLOG_ERROR, "The file \"%s\" opening failed! The file may not exist or may not have access rights.\n", path)
	/* Ŀ¼��ʧ�� */
#	define WDO_ERROR_NOOPENDIR(path)	wdo_log(instruction, OUTLOG_ERROR, "The dir \"%s\" opening failed! The dir may not exist or may not have access rights.\n", path)
	/* �ļ�Ϊ��ʱ */
#	define WDO_ERROR_NULLFILE(file)		wdo_log(instruction, OUTLOG_ERROR, "File \"%s\" is empty!\n", file)
	/* ������Чʱ */
#	define WDO_ERROR_NOALIAS(name)		wdo_log(instruction, OUTLOG_ERROR, "Invalid alias, the name is \"%s\".\n", name)
	/* ȱ����ȷ�ı�������ʱ */
#	define WDO_ERROR_MISSINGALIAS(type)	wdo_log(instruction, OUTLOG_ERROR, "Missing alias of type \"%s\"! See help %s.\n", lrGetAliasTypeString(type), instruction)
	/* ��������Ϊ��ʱ */
#	define WDO_ERROR_NULLALIAS()		wdo_log(instruction, OUTLOG_ERROR, "Alias name cannot be empty!\n")
	/* ����ֻ���ǿ���ʱ */
#	define WDO_ERROR_NOSWITCH(type)		wdo_log(instruction, OUTLOG_ERROR, "Parameter \"%s\" can only be \"on\" or \"off\".\n", type)
	/* ���ڲ��ǿ򴰿�ʱ */
#	define WDO_ERROR_NOWINDOWBOX(win)	wdo_log(instruction, OUTLOG_ERROR, "Window _%X_ no window box of " WDO_EXE_FILE " create.\n", win)
	/* �ļ���׺������ʱ */
#	define WDO_ERROR_NOSUFFIX(suffix)	wdo_log(instruction, OUTLOG_ERROR, "The parameter need a file, the file suffix should be " suffix "\n")
	/* ��Ч��·�� */
#	define WDO_ERROR_NOPATH(path)		wdo_log(instruction, OUTLOG_ERROR, "The path \"%s\" is invalid!\n", path)

#else

#	define WDO_ERROR_NOWINDOW(hwnd)		wdo_log(instruction, OUTLOG_ERROR, "_%X_ ������Ч�Ĵ��ڣ�\n", (unsigned __int64)hwnd)

#	define WDO_ERROR_NULLPARAM(instr)	wdo_log(instruction, OUTLOG_ERROR, "\"%s\" ָ����ҪЯ����������� help %s��\n", instr, instr)

#	define WDO_ERROR_NOPARAM(param)		wdo_log(instruction, OUTLOG_ERROR, "\"%s\" ������Ч�Ĳ�������� help %s��\n", param, instruction)

#	define WDO_ERROR_MISSINGPARAM()		wdo_log(instruction, OUTLOG_ERROR, "���ָ��ȱ�ٲ�������� help %s��\n", instruction)

#	define WDO_ERROR_NOTYPE(type)		wdo_log(instruction, OUTLOG_ERROR, "\"%s\" ��Ч�����ͱ�ʶ������� help %s��\n", type, instruction)

#	define WDO_ERROR_NOMEMORY()			wdo_log(instruction, OUTLOG_ERROR, "�ڴ����ʧ�ܣ����� \"%s\" ָ��Ĳ����޷���ɡ�\n", instruction)

#	define WDO_ERROR_NOOPENFILE(path)	wdo_log(instruction, OUTLOG_ERROR, "�ļ� \"%s\" ��ʧ�ܣ������ļ������ڻ�û�з���Ȩ�ޡ�\n", path)

#	define WDO_ERROR_NOOPENDIR(path)	wdo_log(instruction, OUTLOG_ERROR, "Ŀ¼ \"%s\" ��ʧ�ܣ�����Ŀ¼�����ڻ�û�з���Ȩ�ޡ�\n", path)

#	define WDO_ERROR_NULLFILE(file)		wdo_log(instruction, OUTLOG_ERROR, "�ļ� \"%s\" Ϊ���ļ���\n", file)

#	define WDO_ERROR_NOALIAS(name)		wdo_log(instruction, OUTLOG_ERROR, "��Ч�ı��� \"%s\"\n", name)

#	define WDO_ERROR_MISSINGALIAS(type)	wdo_log(instruction, OUTLOG_ERROR, "ȱ������Ϊ \"%s\" �ı�������� help %s��\n", lrGetAliasTypeString(type), instruction)

#	define WDO_ERROR_NULLALIAS()		wdo_log(instruction, OUTLOG_ERROR, "�������Ʋ�����Ϊ�գ�\n")

#	define WDO_ERROR_NOSWITCH(type)		wdo_log(instruction, OUTLOG_ERROR, "���� \"%s\" ֻ���� \"on\" ���� \"off\"��\n", type)

#	define WDO_ERROR_NOWINDOWBOX(win)	wdo_log(instruction, OUTLOG_ERROR, "���� _%X_ ���� " WDO_EXE_FILE " �����Ĵ��ڿ�\n", win)

#	define WDO_ERROR_NOSUFFIX(suffix)	wdo_log(instruction, OUTLOG_ERROR, "���������Ҫһ���ļ����ļ���׺��Ӧ��Ϊ " suffix "\n")

#	define WDO_ERROR_NOPATH(path)		wdo_log(instruction, OUTLOG_ERROR, "��Ч��·�� \"%s\"��\n", path)

#endif




#else
#	error This header file is only used for lrWindowDo. c
#endif		// _LR_WINDOW_DO_C_

#endif		// _WDO_ERRORLOG_H_



#ifndef WDO_LANGUAGE
#	undef	WDO_ERROR_NOWINDOW
#	undef	WDO_ERROR_NULLPARAM
#	undef	WDO_ERROR_NOPARAM
#	undef	WDO_ERROR_MISSINGPARAM
#	undef	WDO_ERROR_NOTYPE
#	undef	WDO_ERROR_NOMEMORY
#	undef	WDO_ERROR_NOOPENFILE
#	undef	WDO_ERROR_NOOPENDIR
#	undef	WDO_ERROR_NULLFILE
#	undef	WDO_ERROR_NOALIAS
#	undef	WDO_ERROR_MISSINGALIAS
#	undef	WDO_ERROR_NULLALIAS
#	undef	WDO_ERROR_NOSWITCH
#	undef	WDO_ERROR_NOWINDOWBOX
#	undef	WDO_ERROR_NOSUFFIX
#	undef	WDO_ERROR_NOPATH
#endif		// WDO_LANGUAGE