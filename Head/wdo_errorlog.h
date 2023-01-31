// wdo_errorlog.h -- Error log template for lrWindowDo()
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#pragma once

#ifndef _WDO_ERRORLOG_H_
#define _WDO_ERRORLOG_H_

#ifdef _LR_WINDOW_DO_C_

#include "lrWindowDo.h"


/* 一些常用的错误打印的格式模板 */

#if WDO_LANGUAGE
	/* 窗口无效时 */
#	define WDO_ERROR_NOWINDOW(hwnd)		wdo_log(instruction, OUTLOG_ERROR, "_%X_ is not a valid window!\n", (unsigned __int64)hwnd)
	/* 参数为空时 */
#	define WDO_ERROR_NULLPARAM(instr)	wdo_log(instruction, OUTLOG_ERROR, "The \"%s\" parameter is empty! See help %s.\n", instr, instr)
	/* 参数无效时 */
#	define WDO_ERROR_NOPARAM(param)		wdo_log(instruction, OUTLOG_ERROR, "Invalid parameter \"%s\"! See help %s.\n", param, instruction)
	/* 缺少参数时 */
#	define WDO_ERROR_MISSINGPARAM()		wdo_log(instruction, OUTLOG_ERROR, "The instruction missing parameter. See help %s\n", instruction)
	/* 类型无效时 */
#	define WDO_ERROR_NOTYPE(type)		wdo_log(instruction, OUTLOG_ERROR, "Invalid type identifier \"%s\"! See help %s.\n", type, instruction)
	/* 内存分配失败 */
#	define WDO_ERROR_NOMEMORY()			wdo_log(instruction, OUTLOG_ERROR, "Memory allocation failed! The operation from the \"%s\" instruction cannot be completed.\n", instruction)
	/* 文件打开失败 */
#	define WDO_ERROR_NOOPENFILE(path)	wdo_log(instruction, OUTLOG_ERROR, "The file \"%s\" opening failed! The file may not exist or may not have access rights.\n", path)
	/* 目录打开失败 */
#	define WDO_ERROR_NOOPENDIR(path)	wdo_log(instruction, OUTLOG_ERROR, "The dir \"%s\" opening failed! The dir may not exist or may not have access rights.\n", path)
	/* 文件为空时 */
#	define WDO_ERROR_NULLFILE(file)		wdo_log(instruction, OUTLOG_ERROR, "File \"%s\" is empty!\n", file)
	/* 别名无效时 */
#	define WDO_ERROR_NOALIAS(name)		wdo_log(instruction, OUTLOG_ERROR, "Invalid alias, the name is \"%s\".\n", name)
	/* 缺少正确的别名类型时 */
#	define WDO_ERROR_MISSINGALIAS(type)	wdo_log(instruction, OUTLOG_ERROR, "Missing alias of type \"%s\"! See help %s.\n", lrGetAliasTypeString(type), instruction)
	/* 别名名称为空时 */
#	define WDO_ERROR_NULLALIAS()		wdo_log(instruction, OUTLOG_ERROR, "Alias name cannot be empty!\n")
	/* 参数只能是开关时 */
#	define WDO_ERROR_NOSWITCH(type)		wdo_log(instruction, OUTLOG_ERROR, "Parameter \"%s\" can only be \"on\" or \"off\".\n", type)
	/* 窗口不是框窗口时 */
#	define WDO_ERROR_NOWINDOWBOX(win)	wdo_log(instruction, OUTLOG_ERROR, "Window _%X_ no window box of " WDO_EXE_FILE " create.\n", win)
	/* 文件后缀名错误时 */
#	define WDO_ERROR_NOSUFFIX(suffix)	wdo_log(instruction, OUTLOG_ERROR, "The parameter need a file, the file suffix should be " suffix "\n")
	/* 无效的路径 */
#	define WDO_ERROR_NOPATH(path)		wdo_log(instruction, OUTLOG_ERROR, "The path \"%s\" is invalid!\n", path)

#else

#	define WDO_ERROR_NOWINDOW(hwnd)		wdo_log(instruction, OUTLOG_ERROR, "_%X_ 不是有效的窗口！\n", (unsigned __int64)hwnd)

#	define WDO_ERROR_NULLPARAM(instr)	wdo_log(instruction, OUTLOG_ERROR, "\"%s\" 指令需要携带参数！详见 help %s。\n", instr, instr)

#	define WDO_ERROR_NOPARAM(param)		wdo_log(instruction, OUTLOG_ERROR, "\"%s\" 不是有效的参数！详见 help %s。\n", param, instruction)

#	define WDO_ERROR_MISSINGPARAM()		wdo_log(instruction, OUTLOG_ERROR, "这个指令缺少参数，详见 help %s。\n", instruction)

#	define WDO_ERROR_NOTYPE(type)		wdo_log(instruction, OUTLOG_ERROR, "\"%s\" 无效的类型标识符！详见 help %s。\n", type, instruction)

#	define WDO_ERROR_NOMEMORY()			wdo_log(instruction, OUTLOG_ERROR, "内存分配失败！来自 \"%s\" 指令的操作无法完成。\n", instruction)

#	define WDO_ERROR_NOOPENFILE(path)	wdo_log(instruction, OUTLOG_ERROR, "文件 \"%s\" 打开失败！可能文件不存在或没有访问权限。\n", path)

#	define WDO_ERROR_NOOPENDIR(path)	wdo_log(instruction, OUTLOG_ERROR, "目录 \"%s\" 打开失败！可能目录不存在或没有访问权限。\n", path)

#	define WDO_ERROR_NULLFILE(file)		wdo_log(instruction, OUTLOG_ERROR, "文件 \"%s\" 为空文件！\n", file)

#	define WDO_ERROR_NOALIAS(name)		wdo_log(instruction, OUTLOG_ERROR, "无效的别名 \"%s\"\n", name)

#	define WDO_ERROR_MISSINGALIAS(type)	wdo_log(instruction, OUTLOG_ERROR, "缺少类型为 \"%s\" 的别名！详见 help %s。\n", lrGetAliasTypeString(type), instruction)

#	define WDO_ERROR_NULLALIAS()		wdo_log(instruction, OUTLOG_ERROR, "别名名称不可以为空！\n")

#	define WDO_ERROR_NOSWITCH(type)		wdo_log(instruction, OUTLOG_ERROR, "参数 \"%s\" 只能是 \"on\" 或者 \"off\"。\n", type)

#	define WDO_ERROR_NOWINDOWBOX(win)	wdo_log(instruction, OUTLOG_ERROR, "窗口 _%X_ 不是 " WDO_EXE_FILE " 创建的窗口框。\n", win)

#	define WDO_ERROR_NOSUFFIX(suffix)	wdo_log(instruction, OUTLOG_ERROR, "这个参数需要一个文件，文件后缀名应该为 " suffix "\n")

#	define WDO_ERROR_NOPATH(path)		wdo_log(instruction, OUTLOG_ERROR, "无效的路径 \"%s\"！\n", path)

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