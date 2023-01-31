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


/* ������ */
#define WDO_EXE_FILE	"wdo.exe"
/* ����ʱʹ��Ӣ�ģ�Ϊ����ʹ������ */
#define WDO_LANGUAGE	01
/* wdo �汾�� */
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
* �ű���������á�
*/

/* �ű���׺�������� '.' */
#define WDO_SCRIPT_SUFFIX			".wdo"
/* �ű��ļ����� */
#define WDO_SCRIPT_FILETYPE			"lrWdoScript"
/* ����ʽҳ�����̨���� */
#define WDO_CONSOLE_TITLE			" LoveRain WindowDoCommand.exe"
/* ����ʽҳ�����̨���½��ı� */
#define WDO_CONSOLE_TEXT			"l  r �������"
/* ���нű��ļ�ʱ�ı���ǰ׺ */
#define WDO_SCRIPT_TITLE			" LoveRain WindowDoScript.exe > "
/* ����ָ�����󳤶� */
#define WDO_COMMAND_MAXLEN			1024

/* �˳�����Ŀ�ݼ� *//* ALT + W */
#define WDO_EXIT_HOTKEY_MOD			MOD_ALT | MOD_NOREPEAT
#define WDO_EXIT_HOTKEY_VK			0x57
#define WDO_EXIT_HOTKEY_STR			"ALT + W"


/* wdo �������ƴ��� */
#define WDO_WINDOW_CLASS_WDOCONTROL	"wdoControlWindowClass"
/* wdo �����˳���ݼ��Ĵ������� */
#define WDO_WINDOW_CLASS_WDOEXIT	"wdoExitWindowClass"


// wdo Ĭ�ϴ��� alpha ֵ
#define WDOWINDOWALPHA				200
// wdo Ĭ�ϴ��ڱ�����ɫ
#define WDOWINDOWCOLOR				RGB(0x80, 0x80, 0x80)
// wdo �����ཻ��������ɫ
// �ڵ���׵�
#define WDOWINDOWPOSLINECOLOR1		RGB(0x00, 0xff, 0x00)
#define WDOWINDOWPOSLINECOLOR2		RGB(0xff, 0x00, 0x00)


/*
* wdo ���ڵ�λ�ô�С�ͱ߾ࡣ
* 
* ��������Ϊ�����������ڣ�
* �����ʾ����̨���ڣ�
* �ұ���ʾ���ְ�ť��
*/
// ���ڵĿ�ȣ��������ұ߿�
#define WDOWINDOWSIZE_WIDTH			900
// ���ڵ�ͳһ�߶�
#define WDOWINDOWSIZE_HEIGHT		525
// �������ڱȿ���̨���ڵ�ͻ���߾�
#define WDOWINDOWSIZE_JAMB			130
// �������ĸ߶�
#define WDOWINDOWSIZE_INPUT_HEIGHT	30
// ����������ʼ X ������
#define WDOWINDOWSIZE_INPUT_POSX	160
#define WDOWINDOWSIZE_INPUT_POSY	10
// ��������ĩβ���
#define	WDOWINDOWSIZE_INPUT_ENDX	20
// ���������ʼ X ������
#define WDOWINDOWSIZE_OUTPUT_POSX	5
// ��������������ļ��
#define WDOWINDOWSIZE_IO_SPACE		30
// ��ť�� WDOWINDOWSIZE_JAMB �����е���ʼ����
#define WDOWINDOW_BUTTON_POSX		2
// ��һ����ť���붥�˵� Y ������
#define WDOWINDOW_BUTTON_POSY		20
// ͳһ��ť�߶�
#define WDOWINDOW_BUTTON_HEIGHT		wdo_language(35, 30)
// ÿ����ť�� Y ����
#define WDOWINDOW_BUTTON_JAMB		(WDOWINDOW_BUTTON_HEIGHT + 10)
// ͳһ��ť���
#define WDOWINDOW_BUTTON_WIDTH		(WDOWINDOWSIZE_JAMB - 20)
// ��̬�ı���ĸ߶�
#define WDOWINDOW_STATIC_HEIGHT		100
// ��̬�ı���ľ���ײ�ı߾�
#define WDOWINDOW_STATIC_JAMP		16
// ����ͼƬ�Ĵ�С
#define WDOWINDOW_BK_WIDTH			900
#define WDOWINDOW_BK_HEIGHT			500



/* wdo Button ��ť������
*/
#define WDOBUTTONNAME_OPEN_CLS						wdo_language("Refresh",		 "����ˢ��")
#define WDOBUTTONNAME_OPEN_RESTORE					wdo_language("Restore",	     "��λ��ԭ")
#define WDOBUTTONNAME_OPEN_SETTOP					wdo_language("Window top",	 "���ڶ���")
#define WDOBUTTONNAME_CLOSE_SETTOP					wdo_language("Cancel top",	 "ȡ������")
#define WDOBUTTONNAME_OPEN_BKMODE					wdo_language("Solid BK",	 "��ɫ����")
#define WDOBUTTONNAME_CLOSE_BKMODE					wdo_language("Colour BK",	 "��ɫ����")
#define WDOBUTTONNAME_OPEN_CHANGEBK					wdo_language("Change BK",	 "���ı���")
#define WDOBUTTONNAME_OPEN_SCROLLBAR				wdo_language("Open scroll",	 "�򿪹���")
#define WDOBUTTONNAME_CLOSE_SCROLLBAR				wdo_language("Close scroll", "�رչ���")


/*
* wdo �������ƴ��ڵ���Ϣ��
*/
// �����˳���ݼ��Ĵ�����û�а����˳���ݼ�ʱ���˳�����Ҫ���������ƴ������´���
// �������´��ھ��
#define WM_WDO_CREATEEXIT			(WM_USER + 1)
// ������˳���ݼ��Ĵ������������
// wParam (HWND)��Ҫ��ӵĴ��ھ��
#define WM_WDO_ADDEXITWINDOW		(WM_USER + 2)
// �����´���ʱ��֪ͨ���ƴ���
// wParam (HWND)���ھ��
// return BOOL;
#define WM_WDO_ADDNEWWINDOW			(WM_USER + 3)
// �������߳�ʱ��֪ͨ���ƴ���
// wParam (DWORD)�߳̾��
// return BOOL;
#define WM_WDO_ADDNEWTHREAD			(WM_USER + 4)
// ���ٴ���ʱ��֪ͨ���ƴ���
// wParam (HWND)���ھ��
// return BOOL;
#define WM_WDO_DESTROYWINDOW		(WM_USER + 5)
// �����߳�ʱ��֪ͨ���ƴ���
// wParam (HANDLE)�߳̾��
// return BOOL;
#define WM_WDO_DESTROYTHREAD		(WM_USER + 6)
// ����ƴ��������˳�����
// wParam (int)�˳���
// lParam (WDOEXITASKMODE) �ṹ
// return BOOL;
#define WM_WDO_EXITASK				(WM_USER + 7)
// ���¼��ؿ����
#define WM_WDO_RELOADBOXALIAS		(WM_USER + 8)
// ���Ŀ����ı�
// wParam (char*)���µ��ı�ָ��
// return BOOL;
#define WM_WDO_SETBOXTEXT			(WM_USER + 9)
// ������������ı���ɫ
// wParam (COLORREF)��ɫ
#define WM_WDO_SETCOLOR				(WM_USER + 10)
// ���ô����ȼ�����
// wParam (LPCSTR)��ݼ����ַ���
// lParam (WDOINSTRUCTIONS*)�����
// return (DWORD) ErrorCode from GetLastError()
#define WM_WDO_SETHOTKEY			(WM_USER + 11)

/*
* wdo.exe ���˳��롣
*/

/* �����˳���ݼ� */
#define WDO_EXIT_CODE__HKEY			'aw'
/* �����в��� -? */
#define WDO_EXIT_CODE__HELP			'-?'
/* �����е��� wdo �ļ�·������ */
#define WDO_EXIT_CODE__NOWDOPATH	'np'
/* ������ָ����� */
#define WDO_EXIT_CODE__NOWDOINS		'nc'
/* ���� wdo_error() �˳� */
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
    WDOEXITASK_RETURN = 0,	/* �������� */
    WDOEXITASK_ENDEDEXIT,	/* �������أ������˳�ָ�� */
    WDOEXITASK_WAITEXIT		/* �ȴ�����ɹ��󷵻� */
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