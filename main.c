// main.c
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
// This project was written in 2022.01.18 - 19:12:36
// 
// 
// Abstract:
//     OS  = Microsoft Windows [VER 10.0.22621.963]
//     IDE = Microsoft Visual Studio 2022(v143)
// 
//     STD = ISO C11 (/std:c11)
//     CS  = ANSI
// 
// You need to configure the XML file in the project properties -> Inventory tool
//
#include "Head/lr_wdo.h"
#include "Head/lrWindowDo.h"
#include "Resource/resource.h"
#include "Head/wdo_instructions.h"

#include <locale.h>
#include <VersionHelpers.h>

#pragma comment (linker, "/subsystem:windows /entry:mainCRTStartup")

#pragma warning (disable : 6273)
#pragma warning (disable : 4477)
#pragma warning (disable : 4313)
#pragma warning (disable : 6011)

#if 1
int main(int argc, char** argv)
{
	/* Initialize language region */
	setlocale(LC_ALL, "");
	
	/* Initialize wdo, and create windows, and manage these windows */
	wdo_create();
	

	/* Test script file */
#if 0
	lrWindowDoScript("C:\\Users\\33474\\Desktop\\lrWindowDo\\Test\\test.wdo", FALSE);
	wdo_puts(wdo_language("\n~Script command runs to the end, waiting for exit...", "\n~脚本命令运行到末尾，正在等待退出。。。"));
	SendMessageA(wdo_gethwnd(WDOGETHWND_COMMAND), EM_SETREADONLY, TRUE, 0);
	wdo_exit_ask(WDO_EXIT_CODE__NOWDOPATH, WDOEXITASK_WAITEXIT);
#endif

	
	/* Check command line parameters */
	if (argc > 1)
	{
		/* If parameter 1 is a valid path */
		if (!access(argv[1], 0))
		{
			/* Select the opening method according to the file suffix name */
			char ext[_MAX_EXT];
			_splitpath(argv[1], NULL, NULL, NULL, ext);

			if (!stricmp(ext, WDO_SCRIPT_SUFFIX))
			{
				lrWindowDoScript(argv[1], FALSE);
				wdo_puts(wdo_language("\n~Script command runs to the end, waiting for exit...", "\n~脚本命令运行到末尾，正在等待退出。。。"));
				SendMessageA(wdo_gethwnd(WDOGETHWND_COMMAND), EM_SETREADONLY, TRUE, 0);
				wdo_exit_ask(WDO_EXIT_CODE__NOWDOPATH, WDOEXITASK_WAITEXIT);
			}
			else if (!stricmp(ext, FALIAS_EXT))
			{
				char* command = (char*)malloc(strlen(argv[1]) + 10);
				if (command)
				{
					sprintf(command, WDOINS_ALIAS_LOAD " %s", argv[1]);
					lrWindowDo(command, WDOECHO_DEF);
					free(command);
					goto wdo_start_main;
				}
				else {
					wdo_error_ex();
					return WDO_EXIT_CODE__ERROR;
				}
			}
			else if (!stricmp(ext, ".txt"))
			{
				char* command = (char*)malloc(strlen(argv[1]) + 10);
				if (command)
				{
					command = lrfgetall(argv[1], NULL);

					lrCreateTextBox(command, 0, 0, NULL);
					free(command);
					goto wdo_start_main;
				}
				else {
					wdo_error_ex();
					wdo_exit(WDO_EXIT_CODE__ERROR);
				}
			}
			else if (!stricmp(ext, ".bmp"))
			{
				char* command = (char*)malloc(strlen(argv[1]) + 10);
				if (command)
				{
					sprintf(command, WDOINS_BOX_IMAGE " %s", argv[1]);
					lrWindowDo(command, WDOECHO_DEF);
					free(command);
					goto wdo_start_main;
				}
				else {
					wdo_error_ex();
					wdo_exit(WDO_EXIT_CODE__ERROR);
				}
			}
			else if (!stricmp(ext, ".mp3") || !stricmp(ext, ".wav"))
			{
				char* command = (char*)malloc(strlen(argv[1]) + 10);
				if (command)
				{
					sprintf(command, WDOINS_MUSIC " -O %s", argv[1]);
					lrWindowDo(command, WDOECHO_DEF);
					free(command);
					goto wdo_start_main;
				}
				else {
					wdo_error_ex();
					wdo_exit(WDO_EXIT_CODE__ERROR);
				}
			}
		}
		/* Get help, open about window, and waiting for exit */
		else if (!stricmp(argv[1], "-?") || !stricmp(argv[1], "/?"))
		{
			ShowWindow(wdo_gethwnd(WDOGETHWND_CONTROL), SW_HIDE);
			wdo_about(TRUE, wdo_language(LANG_ENGLISH, LANG_CHINESE));
			wdo_exit_ask(WDO_EXIT_CODE__HELP, WDOEXITASK_WAITEXIT);
		}
		else if (!stricmp(argv[1], "-GenerateReg") || !stricmp(argv[1], "/GenerateReg"))
		{
			ShowWindow(wdo_gethwnd(WDOGETHWND_CONTROL), SW_HIDE);

			FILE* fp = fopen("SetOpenMethod.reg", "w");
			if (fp)
			{
				char MyPath[MAX_PATH + 100];
				memcpy(MyPath, argv[0], strlen(argv[0]) + 1);

				for (size_t i = 0, j = 0;; ++i)
				{
					MyPath[j++] = argv[0][i];

					if (argv[0][i] == '\\') MyPath[j++] = '\\';
					else if (argv[0][i] == '\0') break;
				}

				fprintf(fp,
					"Windows Registry Editor Version 5.00\n"
					"\n"
					"[HKEY_CLASSES_ROOT\\.wdo]\n"
					"@=\"lrWdoScript\"\n"
					"\n"
					"[HKEY_CLASSES_ROOT\\lrWdoScript]\n"
					"@=\"Lr.WDO Script\"\n"
					"\n"
					"[HKEY_CLASSES_ROOT\\lrWdoScript\\DefaultIcon]\n"
					"@=\"\\\"%s\\\",1\"\n"
					"\n"
					"[HKEY_CLASSES_ROOT\\lrWdoScript\\shell]\n"
					"\n"
					"[HKEY_CLASSES_ROOT\\lrWdoScript\\shell\\open]\n"
					"@=\"Run WDO Script\"\n"
					"\n"
					"[HKEY_CLASSES_ROOT\\lrWdoScript\\shell\\open\\command]\n"
					"@=\"\\\"%s\\\" \\\"%%1\\\"\"\n"
					"\n"
					, MyPath, MyPath
				);
				fclose(fp);
			}
			MessageBoxA(NULL, "Reg file generation completed.", "lr - Generation", MB_OK | MB_ICONINFORMATION);
			wdo_exit(0);
		}
		/* If not, submit it to lrWindowDo() function, and waiting for exit */
		else
		{
			ShowWindow(wdo_gethwnd(WDOGETHWND_CONTROL), SW_HIDE);

			size_t CommandLen = 1;
			for (size_t i = 0; i < argc; ++i) CommandLen += strlen(argv[i]) + 1;

			char* Command = (char*)malloc(CommandLen);
			if (Command)
			{
				strcpy(Command, argv[1]);
				for (int i = 2; i < argc; ++i) {
					strcat(Command, " ");
					strcat(Command, argv[i]);
				}
				lrWindowDo(Command, WDOECHO_DEF);
				wdo_exit_ask(WDO_EXIT_CODE__NOWDOINS, WDOEXITASK_WAITEXIT);
			}
		}
	}



	
wdo_start_main:
	{
		/* Output Start Page Text */
		HDC MyDC = GetDC(wdo_gethwnd(WDOGETHWND_OUTLOG));

		wdo_log(NULL, OUTLOG_NORMAL, "LoveRain WindowDoSctipt.exe [VER 1.0] Hello Mr.%s!\n", getenv("UserName"));

		wdo_log(NULL, OUTLOG_NORMAL, "Screen resolution is (%d,%d), DPI = [%.2fW, %.2fH]\n"
			, GetDeviceCaps(MyDC, DESKTOPHORZRES)
			, GetDeviceCaps(MyDC, DESKTOPVERTRES)
			, lrGetWindowDpiZoom(wdo_gethwnd(WDOGETHWND_CONTROL)).fw
			, lrGetWindowDpiZoom(wdo_gethwnd(WDOGETHWND_CONTROL)).fh);

		wdo_log(NULL, OUTLOG_NORMAL, "LANGUAGE = %s, IP = %s\n\n", setlocale(LC_ALL, ""), lrGetMyIPv4(NULL));

		wdo_log(NULL, OUTLOG_NORMAL,
			"Use cmd.exe to bring in the path of the (" WDO_SCRIPT_SUFFIX ") file to run the script\n"
			"Use the global shortcut <" WDO_EXIT_HOTKEY_STR "> to exit all running " WDO_EXE_FILE " processes\n"
			"\n"
			"Use the help instruction to obtain the usage mode\n");
		wdo_log(NULL, OUTLOG_NORMAL,
			"\n\n"
			"This project was written in 2022.01.18 - 19:12:36\n"
			"Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.\n"
		);

		/* Draw text in the lower right corner */
		RECT MyRect;
		GetClientRect(wdo_gethwnd(WDOGETHWND_OUTLOG), &MyRect);
		HFONT MyFont = lr_CreateFont(90, 0, FW_EXTRALIGHT, TRUE, FALSE, FALSE, "华文行楷");
		SelectObject(MyDC, MyFont);
		DeleteObject(MyFont);
		SetTextColor(MyDC, RGB(0x00, 0xff, 0x00));
		SetBkMode(MyDC, TRANSPARENT);
		SetTextCharacterExtra(MyDC, -7);
		DrawTextA(MyDC, WDO_CONSOLE_TEXT, -1, &MyRect, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);
		DeleteObject(MyFont);
		ReleaseDC(wdo_gethwnd(WDOGETHWND_OUTLOG), MyDC);
		
		/* Check OS version */
		/* This function requires the program manifest lrGetWindowsVer.xml to work properly */
		if (!IsWindows10OrGreater())
		{
			wdo_log(NULL, OUTLOG_ERROR, "\n\n"
				"~Your version is lower than windows 10, which may affect the normal use of some functions.\n\n");
		}
		/* Failed to register the shortcut key for exiting wdo */
		if (!FindWindowA(WDO_WINDOW_CLASS_WDOEXIT, NULL))
		{
			wdo_log(NULL, OUTLOG_ERROR, "\n\n"
				"~The exit shortcut key \"" WDO_EXIT_HOTKEY_STR "\" register failed, it may have been registered by another program.\n\n");
		}

		/* lrWindowDo() function is called circularly in wdo_create() */
		Sleep(INFINITE);
	}
	return 0;
}
#endif

#undef WDO_COMMAND_SIZE


#if SIZE_MAX != ULLONG_MAX
#	error wdo.exe for windows x64 system.
#	error -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Love·Rain -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#endif

#ifdef UNICODE
#	error Please use ANSI encoding
#	error -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Love·Rain -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#endif

#ifndef _WIN64
#	error wdo.exe running on WINDOWS x64.
#	error -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Love·Rain -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#endif