// lr_wdo.h
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
// This project was written in 2022.01.18 - 19:12:36
// 
// 
// Abstract:
//     These default settings and info for wdo.exe
// 
#pragma once
#ifndef LR_WDO_H
#define LR_WDO_H

#include <Windows.h>
#include "lrio.h"


/* 程序名 */
#define WDO_EXE_FILE	"wdo.exe"
/* 非零时使用英文，为零则使用中文 */
#define WDO_LANGUAGE	01
/* wdo 版本号 */
#define WDO_VER_ID		(UINT)0x01000000



#if WDO_LANGUAGE
#	define wdo_language(_English, _Chinese)		_English
#else
#	define wdo_language(_English, _Chinese)		_Chinese
#endif


#if !defined(lrERRORTEXT_LANGUAGE)

#if !defined(WDO_LANGUAGE)
#define lrERRORTEXT_LANGUAGE			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
#elif WDO_LANGUAGE
#define lrERRORTEXT_LANGUAGE			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)
#else
#define lrERRORTEXT_LANGUAGE			MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#endif

#endif

/*
* 脚本定义和设置。
*/

/* 脚本后缀名，包括 '.' */
#define WDO_SCRIPT_SUFFIX			".wdo"
/* 脚本文件类型 */
#define WDO_SCRIPT_FILETYPE			"lrWdoScript"
/* 交互式页面控制台标题 */
#define WDO_CONSOLE_TITLE			" LoveRain WindowDoCommand.exe"
/* 交互式页面控制台右下角文本 */
#define WDO_CONSOLE_TEXT			"l  r 恋雨设计"
/* 运行脚本文件时的标题前缀 */
#define WDO_SCRIPT_TITLE			" LoveRain WindowDoScript.exe > "
/* 单行指令的最大长度 */
#define WDO_COMMAND_MAXLEN			1024

/* 退出程序的快捷键 *//* ALT + W */
#define WDO_EXIT_HOTKEY_MOD			MOD_ALT | MOD_NOREPEAT
#define WDO_EXIT_HOTKEY_VK			0x57
#define WDO_EXIT_HOTKEY_STR			"ALT + W"


/* wdo 辅助控制窗口 */
#define WDO_WINDOW_CLASS_WDOCONTROL	"wdoControlWindowClass"
/* wdo 接收退出快捷键的窗口类名 */
#define WDO_WINDOW_CLASS_WDOEXIT	"wdoExitWindowClass"


// wdo 默认窗口 alpha 值
#define WDOWINDOWALPHA				200
// wdo 默认窗口背景颜色
#define WDOWINDOWCOLOR				RGB(0x80, 0x80, 0x80)
// wdo 坐标相交线条的颜色
// 黑底与白底
#define WDOWINDOWPOSLINECOLOR1		RGB(0x00, 0xff, 0x00)
#define WDOWINDOWPOSLINECOLOR2		RGB(0xff, 0x00, 0x00)


/*
* wdo 窗口的位置大小和边距。
* 
* 背景窗口为主（父）窗口；
* 左边显示控制台窗口；
* 右边显示各种按钮；
*/
// 窗口的宽度（不包括右边框）
#define WDOWINDOWSIZE_WIDTH			900
// 窗口的统一高度
#define WDOWINDOWSIZE_HEIGHT		525
// 背景窗口比控制台窗口的突出边距
#define WDOWINDOWSIZE_JAMB			130
// 输入区的高度
#define WDOWINDOWSIZE_INPUT_HEIGHT	30
// 输入区的起始 X 轴坐标
#define WDOWINDOWSIZE_INPUT_POSX	160
#define WDOWINDOWSIZE_INPUT_POSY	10
// 输入区的末尾间隔
#define	WDOWINDOWSIZE_INPUT_ENDX	20
// 输出区的起始 X 轴坐标
#define WDOWINDOWSIZE_OUTPUT_POSX	5
// 输入区与输出区的间隔
#define WDOWINDOWSIZE_IO_SPACE		30
// 按钮在 WDOWINDOWSIZE_JAMB 区域中的起始坐标
#define WDOWINDOW_BUTTON_POSX		2
// 第一个按钮距离顶端的 Y 轴坐标
#define WDOWINDOW_BUTTON_POSY		20
// 统一按钮高度
#define WDOWINDOW_BUTTON_HEIGHT		wdo_language(35, 30)
// 每个按钮的 Y 轴间距
#define WDOWINDOW_BUTTON_JAMB		(WDOWINDOW_BUTTON_HEIGHT + 10)
// 统一按钮宽度
#define WDOWINDOW_BUTTON_WIDTH		(WDOWINDOWSIZE_JAMB - 20)
// 静态文本框的高度
#define WDOWINDOW_STATIC_HEIGHT		100
// 静态文本框的距离底层的边距
#define WDOWINDOW_STATIC_JAMP		16
// 背景图片的大小
#define WDOWINDOW_BK_WIDTH			900
#define WDOWINDOW_BK_HEIGHT			500



/* wdo Button 按钮的名字
*/
#define WDOBUTTONNAME_OPEN_CLS						wdo_language("Refresh",		 "清屏刷新")
#define WDOBUTTONNAME_OPEN_RESTORE					wdo_language("Restore",	     "复位还原")
#define WDOBUTTONNAME_OPEN_SETTOP					wdo_language("Window top",	 "窗口顶置")
#define WDOBUTTONNAME_CLOSE_SETTOP					wdo_language("Cancel top",	 "取消顶置")
#define WDOBUTTONNAME_OPEN_BKMODE					wdo_language("Solid BK",	 "纯色背景")
#define WDOBUTTONNAME_CLOSE_BKMODE					wdo_language("Colour BK",	 "彩色背景")
#define WDOBUTTONNAME_OPEN_CHANGEBK					wdo_language("Change BK",	 "更改背景")
#define WDOBUTTONNAME_OPEN_SCROLLBAR				wdo_language("Open scroll",	 "打开滚动")
#define WDOBUTTONNAME_CLOSE_SCROLLBAR				wdo_language("Close scroll", "关闭滚动")


/*
* wdo 辅助控制窗口的消息。
*/
// 控制退出快捷键的窗口在没有按下退出快捷键时被退出，会要求其他控制窗口重新创建
// 将返回新窗口句柄
#define WM_WDO_CREATEEXIT			(WM_USER + 1)
// 向控制退出快捷键的窗口添加自身窗口
// wParam (HWND)是要添加的窗口句柄
#define WM_WDO_ADDEXITWINDOW		(WM_USER + 2)
// 创建新窗口时，通知控制窗口
// wParam (HWND)窗口句柄
// return BOOL;
#define WM_WDO_ADDNEWWINDOW			(WM_USER + 3)
// 创建新线程时，通知控制窗口
// wParam (DWORD)线程句柄
// return BOOL;
#define WM_WDO_ADDNEWTHREAD			(WM_USER + 4)
// 销毁窗口时，通知控制窗口
// wParam (HWND)窗口句柄
// return BOOL;
#define WM_WDO_DESTROYWINDOW		(WM_USER + 5)
// 销毁线程时，通知控制窗口
// wParam (HANDLE)线程句柄
// return BOOL;
#define WM_WDO_DESTROYTHREAD		(WM_USER + 6)
// 向控制窗口申请退出进程
// wParam (int)退出码
// lParam (WDOEXITASKMODE) 结构
// return BOOL;
#define WM_WDO_EXITASK				(WM_USER + 7)
// 重新加载框别名
#define WM_WDO_RELOADBOXALIAS		(WM_USER + 8)
// 更改框中文本
// wParam (char*)是新的文本指针
// return BOOL;
#define WM_WDO_SETBOXTEXT			(WM_USER + 9)
// 设置输出区的文本颜色
// wParam (COLORREF)颜色
#define WM_WDO_SETCOLOR				(WM_USER + 10)
// 设置窗口热键命令
// wParam (LPCSTR)快捷键的字符串
// lParam (WDOINSTRUCTIONS*)命令块
// return (DWORD) ErrorCode from GetLastError()
#define WM_WDO_SETHOTKEY			(WM_USER + 11)

/*
* wdo.exe 的退出码。
*/

/* 按下退出快捷键 */
#define WDO_EXIT_CODE__HKEY			'aw'
/* 命令行参数 -? */
#define WDO_EXIT_CODE__HELP			'-?'
/* 命令行调用 wdo 文件路径错误 */
#define WDO_EXIT_CODE__NOWDOPATH	'np'
/* 命令行指令错误 */
#define WDO_EXIT_CODE__NOWDOINS		'nc'
/* 调用 wdo_error() 退出 */
#define WDO_EXIT_CODE__ERROR		'e'






#if WDO_LANGUAGE
#	define wdo_log_insec(_State, _English_Format, _Chinese_Format, ...)	wdo_log(instruction, State = _State, _English_Format "\n", ##__VA_ARGS__)
#else
#	define wdo_log_insec(_State, _English_Format, _Chinese_Format, ...)	wdo_log(instruction, State = _State, _Chinese_Format "\n", ##__VA_ARGS__)
#endif



#define wdo_log_language(_State, _English_Format, _Chinese_Format, ...)	wdo_log(NULL, _State, wdo_language(_English_Format, _Chinese_Format), ##__VA_ARGS__)


typedef enum WDOGETHWND
{
    WDOGETHWND_OUTLOG = 0,
    WDOGETHWND_COMMAND,
    WDOGETHWND_CONTROL,
    WDOGETHWND_EXITCONTROL
} WDOGETHWND;

typedef enum WDOEXITASKMODE
{
    WDOEXITASK_RETURN = 0,	/* 立即返回 */
    WDOEXITASK_ENDEDEXIT,	/* 立即返回，留下退出指令 */
    WDOEXITASK_WAITEXIT		/* 等待请求成功后返回 */
} WDOEXITASKMODE;


typedef struct WDOINSTRUCTIONS
{
    size_t Count;
    char** Instructions;
    
} WDOINSTRUCTIONS;

#ifdef __cplusplus
extern "C"
{
#endif

    /* -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ lr_wdo.c -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

    HWND				wdo_create();
    HWND				wdo_gethwnd(WDOGETHWND _type);

    BOOL                wdo_outlog_switch(BOOL _Switch);
    void				wdo_puts(const char* const _String);
    void				wdo_puts_upper(const char* const _String);
    void				wdo_puts_lower(const char* const _String);
    void				wdo_putc(int c);
    void				wdo_cls();
    void				wdo_setcolor(COLORREF color);
    static inline BOOL	wdo_settext(LPCSTR lpText) { return SetWindowTextA(wdo_gethwnd(WDOGETHWND_OUTLOG), lpText); }

    BOOL				wdo_add_window(HWND _window);
    BOOL				wdo_del_window(HWND _window);
    BOOL				wdo_add_thread(DWORD _thread);
    BOOL				wdo_del_thread(DWORD _thread);

    /* If return FALSE, you need to free momory yourself */
    BOOL				wdo_set_hotkey(const char* const _HotkeyString, size_t _Count, const char** const _Instructions);

#if 0 //WDO_LANGUAGE
    void wdo_help_english(const char* const _str);
#else
    void wdo_help_chinese(const char* const _str);
#endif

    static inline void	wdo_help(const char* const _str)
    {
#if 0 //WDO_LANGUAGE
        wdo_help_english(_str);
#else
        wdo_help_chinese(_str);
#endif
    }

    const char* const	wdo_about(BOOL _InBox, DWORD _LanguageId);

    void				wdo_error(const unsigned __int64 _Line, const unsigned char* const _File, const unsigned char* const _Func);
    void				wdo_exit(int _ExitCode);
    BOOL				wdo_exit_ask(int _ExitCode, WDOEXITASKMODE _AskMode);

#if 1
#define wdo_error_ex()	wdo_error(__LINE__, __FILE__, __func__)
#else
#define wdo_error_ex()	wdo_error(__LINE__, __FILE__, __FUNCSIG__)
#endif

    /* -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ lrThread.c -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

    HANDLE				wdo_thread_run(const char** const _InsSets, size_t _InsQuan);
    HANDLE				wdo_thread_timing(const char** const _InsSets, size_t _InsQuan, UINT _hour, UINT _minute, UINT _second);

#ifdef __cplusplus
}
#endif

#endif		// LR_WDO_H