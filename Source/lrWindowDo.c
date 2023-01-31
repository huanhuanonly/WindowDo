// lrWindowDo.c -- lrWindowDo() and lrWindowDoScript()
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#ifndef _LR_WINDOW_DO_C_
#define _LR_WINDOW_DO_C_


#include "../Head/lr_wdo.h"
#include "../Resource/resource.h"
#include "../Head/dirent.h"
#include "../Head/lrWindowDo.h"
#include "../Head/wdo_errorlog.h"
#include "../Head/wdo_instructions.h"


#pragma comment (lib, "Winmm.lib")

#pragma warning (disable : 6273; disable : 4477; disable : 4313)
#pragma warning (disable : 4551)
#pragma warning (disable : 6031)

#pragma warning (disable : 6308)


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 恋雨·致青春 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+//

static BOOL running_script_file = FALSE;


errno_t lrWindowDoScript(const char* const _ScriptPath, BOOL _RestoreTitle)
{
	errno_t err = 0;

	HWND hwnd = wdo_gethwnd(WDOGETHWND_CONTROL);
	HWND commandWnd = wdo_gethwnd(WDOGETHWND_COMMAND);

	char title_old[MAXLEN_WIN_TITLE];
	if (_RestoreTitle) GetWindowTextA(hwnd, title_old, sizeof(title_old));

	char title_new[MAXLEN_WIN_TITLE];
	strcpy(title_new, WDO_SCRIPT_TITLE);
	SetWindowTextA(hwnd, strncat(title_new, _ScriptPath, sizeof(title_new) - 1));

	char ext[_MAX_EXT];

	/* 获取文件后缀 */
	_splitpath(_ScriptPath, NULL, NULL, NULL, ext);

	/* 如果是该程序的可执行文件 */
	if (!stricmp(ext, WDO_SCRIPT_SUFFIX))
	{
		enum WDOFILECODE { FCode_Unicode, FCode_Ansi }wdoFileCode = FCode_Ansi;
		FILE* fp = fopen(_ScriptPath, "r");
		if (fp)
		{
			char CodeString[20];
			fgets(CodeString, sizeof(CodeString), fp);

			if (!strnicmp(CodeString, "--UNICODE--", sizeof("--UNICODE--") - 1))
			{
				wdoFileCode = FCode_Unicode;
				SetWindowTextA(hwnd, strncat(title_new, " ~[UNICODE]", sizeof(title_new) - 1));

				fclose(fp);
				if (fp = fopen(_ScriptPath, "rt,ccs=UTF-8"))
				{
					fseek(fp, sizeof(" ~[UNICODE]"), SEEK_SET);
				}
			}
			else if (!strnicmp(CodeString, "--ANSI--", sizeof("--ANSI--") - 1))
			{
				wdoFileCode = FCode_Ansi;
				SetWindowTextA(hwnd, strncat(title_new, " ~[ANSI]", sizeof(title_new) - 1));
			}
			else
			{
				fseek(fp, 0, SEEK_SET);
				SetWindowTextA(hwnd, strncat(title_new, " ~[DEFAULT]", sizeof(title_new) - 1));
			}
		}
		if (fp)
		{
			/* 设置当前进程的当前目录 */
			SetCurrentDirectoryA(_ScriptPath);
			running_script_file = TRUE;
			

			while (!feof(fp))
			{
				char* text = wdo_fgetline(fp, wdoFileCode == FCode_Unicode);
				
				wdo_cls();
				SetWindowTextA(commandWnd, text);
				lrWindowDo(text, WDOECHO_DEF);

				free(text);
			}
			fclose(fp);
		}
		else err = 2;	// 文件打开失败
	}
	else err = 1;		// 如果不是该程序的可执行文件

	/* 将标题设置回原标题 */
	if (_RestoreTitle) SetWindowTextA(hwnd, title_old);

	running_script_file = FALSE;
	return err;
}



WDO lrWindowDo(const char* const _Command, WDOECHO _OutlogState)
{
	/* 当前 ptr 指向的窗口句柄 */
	static HWND ptrwin = 0;
	/* 当前正在使用的窗口句柄，如果没有别名指定则使用 ptrwin */
	HWND ctwin = 0;
	
	/* ptr 窗口是否可以更新，
	* 如果为 FALSE 则除了指令 ptr 外，
	* ptrwin 都将不能更改 */
	static BOOL UpdatePtrwin = TRUE;

	BOOL OldEcho;

	switch (_OutlogState)
	{
	case WDOECHO_DEF: break;
	case WDOECHO_FALSE: OldEcho = wdo_outlog_switch(FALSE); break;
	case WDOECHO_TRUE:  OldEcho = wdo_outlog_switch(TRUE); break;
	default: break;
	}

	/* 当前运行的状态
	* 
	* TRUE  | 正常
	* FALSE | 出错
	*/
	BOOL State = TRUE;


	/* 此为返回值，执行完所有指令后需要更新其数值 */
	WDO ret = { 0 };
	ret.PtrWindow = &ptrwin;
	ret.state = State;
	

	/* 如果 _Command 为空则立即返回，否则将打印错误值 */
	if (_Command == NULL || _Command[0] == '\0') return ret;
	if (_Command[0] == '/' && _Command[1] == '/') return ret;


	/* 延时器 */
	static struct __DELAY
	{
		DWORD ins;			// 每执行一条指令的延时
		DWORD sendword;		// send 发送消息时每发送一字符的延时
		DWORD sendline;		// send 发送消息时每发送一行的延时
		DWORD sendfile;		// send 发送消息时每发送一文件的延时

	}delay = { 0,0,0,0 };

	/* 定时运行的指令载体 */
	static char** TimingRun_InsSets = NULL;
	static size_t TimingRun_InsQuan = 0;

	char* instruction;
	char* parameter;

	/* 分割其指令与参数 */
	wdo_divided_command(_Command, &instruction, &parameter);

	size_t* AliasIndex = NULL;
	size_t  AliasIndexCount = 0;
	BYTE    AliasBools[ALIASTYPECOUNT] = { FALSE };

	/* 获取带入的别名，和检索别名数据，此处截断别名的名称和别名分隔符 */
	ALIAS alias = lrFindAliasInInstruction(instruction, &AliasIndex, &AliasIndexCount, AliasBools);

	if (alias.data.win)
	{
		ctwin = alias.data.win;
	}
	else ctwin = ptrwin;

	if (alias.data.path.Path[0] == '\0')
	{
		getcwd(alias.data.path.Path, sizeof(alias.data.path.Path));
		alias.data.path.Type = PATHTYPE_DIR;
	}

	/* 获取此命令的连续执行次数，如果为 WDO_LOOPCOUNT_ID 则无限循环 */
	unsigned __int64 runtimes = wdo_getloopcount(instruction);

	char PublicString[MAX_PATH];

	wod_log_frominstruction(instruction);
	for (unsigned __int64 runnumber = 0; runtimes == WDO_LOOPCOUNT_ID ? TRUE : runnumber < runtimes; runtimes == WDO_LOOPCOUNT_ID ? (__int64)0 : ++runnumber)
	{
		char**    StrArray = NULL;
		__int64** IntArray = NULL;

		/* 数组或字符串组的高度，默认为 1 */
		size_t ArraySize = 1;

		size_t iStringLength = 0;

		/***************************************************************************************
		* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 基本类 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
		****************************************************************************************/

		/* 设置回显开关 */
		if (!stricmp(instruction, WDOINS_ECHO))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (!stricmp(StrArray[i], "on"))
						{
							wdo_outlog_switch(TRUE);

							wdo_log_insec(OUTLOG_NORMAL
								, "Echo switch on."
								, "回显开关已打开。"
								, NULL
							);

							if (running_script_file) ShowWindow(wdo_gethwnd(WDOGETHWND_CONTROL), SW_SHOW);
						}
						else if (!stricmp(StrArray[i], "off"))
						{
							wdo_outlog_switch(FALSE);
							if (running_script_file) ShowWindow(wdo_gethwnd(WDOGETHWND_CONTROL), SW_HIDE);
						}
						else wdo_log(NULL, OUTLOG_NORMAL, "%s\n", StrArray[i]);

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 设置当前工作路径 */
		else if (!stricmp(instruction, WDOINS_CD))
		{
			StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

			if (StrArray)
			{
				for (size_t i = 0; i < ArraySize; ++i)
				{
					lrstrrem(StrArray[i], '{', '}');
					lrstrrem(StrArray[i], '\"', '\"');

					chdir(StrArray[i]);

					getcwd(PublicString, MAX_PATH);

					wdo_log_insec(OUTLOG_NORMAL, "%s", "%s", PublicString);

					wdo_puts("│─.\n"); wdo_puts("│─..\n");

					char** FileList = NULL;
					size_t FileCount = 0;
					if (lrGetFileListInDir(PublicString, &FileList, &FileCount, FALSE))
					{
						if (FileCount)
						{
							for (size_t FileIndex = 0; FileIndex < FileCount - 1; ++FileIndex)
							{
								wdo_puts("│─");
								wdo_puts(FileList[FileIndex]);
								wdo_putc('\n');
							}
							wdo_puts("└─");
							wdo_puts(FileList[FileCount - 1]);
							wdo_putc('\n');
						}
						lrfrees(&FileList, FileCount);
					}
					if (delay.ins) Sleep(delay.ins);
				}
				lrfrees(&StrArray, ArraySize);
			}
			else
			{
				if (alias.data.path.Type == PATHTYPE_DIR)
				{
					chdir(alias.data.path.Path);

					getcwd(PublicString, MAX_PATH);
					wdo_log_insec(OUTLOG_NORMAL, "%s", "%s", PublicString);

					wdo_puts("│─.\n"); wdo_puts("│─..\n");

					char** FileList = NULL;
					size_t FileCount = 0;
					if (lrGetFileListInDir(PublicString, &FileList, &FileCount, FALSE))
					{
						if (FileCount)
						{
							for (size_t FileIndex = 0; FileIndex < FileCount - 1; ++FileIndex)
							{
								wdo_puts("│─");
								wdo_puts(FileList[FileIndex]);
								wdo_putc('\n');
							}
							wdo_puts("└─");
							wdo_puts(FileList[FileCount - 1]);
							wdo_putc('\n');
						}
						lrfrees(&FileList, FileCount);
					}
				}
				else wdo_log_insec(OUTLOG_ERROR
					, "The path in the alias is not a directory.\n\nPath from alias >> %s"
					, "这个路径在别名中不是一个目录。\n\n来自别名中的路径 >> %s"
					, alias.data.path.Path);
			}
		}
		/* 设置剪贴板数据(文本) */
		else if (!stricmp(instruction, WDOINS_CLIP))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						iStringLength = strlen(StrArray[i]) + 1;

						char* sc_type = (char*)malloc(iStringLength);
						char* sc_text = (char*)malloc(iStringLength);

						if (sc_type != NULL && sc_text != NULL)
						{
							lrstrsep(StrArray[i], 2, sc_type, sc_text);

							/* 直接输入文本 */
							if (!stricmp(sc_type, "-t"))
							{
								if (sc_text[0] == '\0') wdo_log_insec(OUTLOG_ERROR
									, "Text cannot be empty. Please use \"" WDOINS_CLIP " -C\" to clear the clipboard."
									, "文本不能为空，请使用 \"" WDOINS_CLIP " -C\" 清空剪贴板。"
									, NULL);

								else
								{
									if (lrSetClipboardText(sc_text))
									{
										wdo_log_insec(OUTLOG_NORMAL
											, "Copied text to clipboard.\n\nTEXT = %s"
											, "已复制文本至剪切板。\n\nTEXT = %s"
											, sc_text);

									}
									else wdo_log_insec(OUTLOG_ERROR
										, "Clipboard open failed or memory allocation failed!"
										, "剪贴板打开失败或内存分配失败！"
										, NULL);
								}
							}

							/* 从文件中提取文本 */
							else if (!stricmp(sc_type, "-f"))
							{
								if (sc_text[0] == '\0')
								{
									free(sc_text);

									if (sc_text = (char*)malloc(strlen(alias.data.path.Path) + 1))
									{
										strcpy(sc_text, alias.data.path.Path);
									}
									else WDO_ERROR_NOMEMORY();
								}

								if (sc_text != NULL && sc_text[0] != '\0')
								{
									/* 过滤路径 */
									lrstrrem(sc_text, '{', '}');
									lrstrrem(sc_text, '\"', '\"');

									size_t sc_filesize = 0;
									char* sc_filetext = lrfgetall(sc_text, &sc_filesize);

									if (sc_filetext != NULL)
									{
										wdo_log_insec(OUTLOG_NORMAL
											, "File path is \"%s\"\n\nFile size is %llu byte\n\nCopying text to clipboard..."
											, "文件路径：%s\n\n文件大小：%llu byte\n\n正在复制文本至粘贴板。。。"
											, sc_text, sc_filesize
										);

										if (lrSetClipboardText(sc_filetext))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "\n---------- Copy done ----------"
												, "\n---------- 复制完成 ----------"
												, NULL
											);
										}
										else wdo_log_insec(OUTLOG_ERROR
											, "Clipboard open failed, or memory allocation failed!"
											, "剪贴板打开失败或内存分配失败！"
											, NULL
										);
										free(sc_filetext);
									}
									else WDO_ERROR_NOMEMORY();
								}
								else WDO_ERROR_NULLPARAM(instruction);
							}

							/* 清空剪贴板内容 */
							else if (!stricmp(sc_type, "-c"))
							{
								if (lrSetClipboardText(NULL))
								{
									wdo_log_insec(OUTLOG_NORMAL
										, "Clipboard data has been emptied."
										, "剪贴板数据已清空。"
										, NULL
									);

								}
								else wdo_log_insec(OUTLOG_ERROR
									, "Clipboard open failed."
									, "剪贴板打开失败。"
									, NULL
								);
							}

							else WDO_ERROR_NOTYPE(sc_type);

							free(sc_type); free(sc_text);
						}
						else WDO_ERROR_NOMEMORY();

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 设置开机启动项 */
		else if (!stricmp(instruction, WDOINS_STARTUP))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						HKEY hKey;
						if (ERROR_SUCCESS == RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey))
						{
							if (ERROR_SUCCESS != RegSetValueExA(hKey, StrArray[i], 0, REG_SZ, (BYTE*)StrArray[i], (DWORD)strlen(StrArray[i])))
							{
								wdo_log_error(lr_TO_STR(RegSetValueExA));
							}
							if (ERROR_SUCCESS != RegCloseKey(hKey))
							{
								wdo_log_error(lr_TO_STR(RegCloseKey));
							}
						}
						else wdo_log_error(lr_TO_STR(RegOpenKeyExA));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else if (AliasBools[alias_type_path] && alias.data.path.Type == PATHTYPE_FILE)
			{
				HKEY hKey;
				if (ERROR_SUCCESS == RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey))
				{
					if (ERROR_SUCCESS != RegSetValueExA(hKey, alias.data.path.Path, 0, REG_SZ, (BYTE*)alias.data.path.Path, (DWORD)strlen(alias.data.path.Path)))
					{
						wdo_log_error(lr_TO_STR(RegSetValueExA));
					}
					if (ERROR_SUCCESS != RegCloseKey(hKey))
					{
						wdo_log_error(lr_TO_STR(RegCloseKey));
					}
				}
				else wdo_log_error(lr_TO_STR(RegOpenKeyExA));
			}

			/* 遍历该启动项列表 */
			HKEY hKey;
			if (ERROR_SUCCESS == RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey))
			{
				char** VName = NULL;
				DWORD* VType = NULL;
				BYTE** VData = NULL;
				DWORD VCount = 0;

				lrGetRegValueListInRegedit(hKey, &VName, &VType, &VData, &VCount);

				if (VCount)
				{
					for (DWORD VNumber = 0; VNumber < VCount - 1; ++VNumber)
					{
						if (VType[VNumber] == REG_SZ || VType[VNumber] == REG_EXPAND_SZ)
						{
							wdo_log(instruction, OUTLOG_NORMAL, "│─%s\n│ └─%s\n│\n", VName[VNumber], VData[VNumber]);
						}
					}
					if (VType[VCount - 1] == REG_SZ || VType[VCount - 1] == REG_EXPAND_SZ)
					{
						wdo_log(instruction, OUTLOG_NORMAL, "└─%s\n  └─%s\n", VName[VCount - 1], VData[VCount - 1]);
					}
					lrfrees(&VName, VCount);
					lrfrees(&VData, VCount);
					free(VType);
				}

				RegCloseKey(hKey);
			}

		}
		/* 使程序暂停(毫秒) */
		else if (!stricmp(instruction, WDOINS_PAUSE))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (TRUE)
						{
							DWORD remain = (DWORD)llabs(IntArray[i][0]);

							/* 总时间大于一秒时 */
							if (remain > 1000)
							{
								while (remain > 1000)
								{
									wdo_log(NULL, OUTLOG_NORMAL, "Time remaining is %lus\n", remain / 1000);
									Sleep(1000);
									remain -= 1000;
								}
								if (remain)
								{
									wdo_log(NULL, OUTLOG_NORMAL, "Time remaining is %lums\n", remain);
									Sleep(remain);
								}
							}
							else
							{
								wdo_log(NULL, OUTLOG_NORMAL, "Time remaining is %lums\n", remain);
								Sleep(remain);
							}

							wdo_putc('\n');
						}
						else Sleep((DWORD)llabs(IntArray[i][0]));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 设置延时器(毫秒) */
		else if (!stricmp(instruction, WDOINS_DELAY))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						iStringLength = strlen(StrArray[i]) + 1;

						char* d_type = (char*)malloc(iStringLength);
						char* d_text = (char*)malloc(iStringLength);

						if (d_type != NULL && d_text != NULL)
						{
							lrstrsep(StrArray[i], 2, d_type, d_text);

							/* 指令延时器 */
							if (!stricmp(d_type, "-i"))
							{
								delay.ins = abs(atol(d_text));

								wdo_log_insec(OUTLOG_NORMAL
									, "Delayer on, " WDO_EXE_FILE " each instruction executed will wait %lu ms."
									, "延时器开启，" WDO_EXE_FILE " 执行每条指令都将等待 %lu ms。"
									, delay.ins
								);
							}

							/* send 每发送一个字符的延时 */
							else if (!stricmp(d_type, "-sw"))
							{
								delay.sendword = abs(atol(d_text));

								wdo_log_insec(OUTLOG_NORMAL
									, "Delayer on, " WDOINS_SEND " each character sent will wait %lu ms."
									, "延时器开启，" WDOINS_SEND " 每发送一个字符都将会等待 %lu ms。"
									, delay.sendword
								);

							}

							/* send 每发送一行的延时 */
							else if (!stricmp(d_type, "-sl"))
							{
								delay.sendline = abs(atol(d_text));

								wdo_log_insec(OUTLOG_NORMAL
									, "Delayer on, " WDOINS_SEND " each linebreak sent will wait %lu ms."
									, "延时器开启，" WDOINS_SEND " 每发送一行都将会等待 %lu ms。"
									, delay.sendline
								);
							}

							/* send 每发送一个文件的延时 */
							else if (!stricmp(d_type, "-sf"))
							{
								delay.sendfile = abs(atol(d_text));

								wdo_log_insec(OUTLOG_NORMAL
									, "Delayer on, " WDOINS_SEND " each file sent will wait %lu ms."
									, "延时器开启，" WDOINS_SEND " 每发送一个文件都将会等待 %lu ms。"
									, delay.sendfile
								);
							}

							else WDO_ERROR_NOTYPE(d_type);

							free(d_type);
							free(d_text);
						}
						else WDO_ERROR_NOMEMORY();

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 播放音乐 */
		else if (!stricmp(instruction, WDOINS_MUSIC))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						iStringLength = strlen(StrArray[i]) + 1;

						char* type = (char*)malloc(iStringLength);

						if (type != NULL)
						{
							lrstrsep(StrArray[i], 2, type, PublicString);

							/* 如果路径却省则尝试调用别名库数据 */
							if (PublicString[0] == '\0') strcpy(PublicString, alias.data.path.Path);

							lrstrrem(PublicString, '{', '}');
							lrstrrem(PublicString, '\"', '\"');

							MCIERROR mciError = 0;
							BOOL ran = TRUE;

							if (!stricmp(type, "-o"))
							{
								mciError = lrPlayMusic(PublicString, FALSE, FALSE);
							}
							else if (!stricmp(type, "-r"))
							{
								mciError = lrPlayMusic(PublicString, TRUE, FALSE);
							}
							else if (!stricmp(type, "-w"))
							{
								mciError = lrPlayMusic(PublicString, FALSE, TRUE);
							}
							else if (!stricmp(type, "-c"))
							{
								mciError = lrCloseMusic(PublicString);
							}
							else if (!stricmp(type, "-p"))
							{
								mciError = lrPauseMusic(PublicString);
							}
							else if (!stricmp(type, "-s"))
							{
								mciError = lrContinueMusic(PublicString);
							}
							else
							{
								ran = FALSE;
								WDO_ERROR_NOTYPE(type);
							}

							if (ran && TRUE)
							{
								mciGetErrorStringA(mciError, PublicString, sizeof(PublicString));
								wdo_log_insec(!mciError, "mciErrorCode: %lu\n\nmciToString: %s", "错误码：%lu\n\n解释后：%s", mciError, PublicString);
							}
							free(type);
						}
						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 生成蜂鸣声 */
		else if (!stricmp(instruction, WDOINS_BEEP))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				if (IntArray = wdo_curlybrace_splitint(parameter, &ArraySize))
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (Beep((DWORD)IntArray[i][0], (DWORD)IntArray[i][1]))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Windows generated a beep, freq = %lu, duration = %lu."
								, "Windows 生成了一身蜂鸣，频率 = %lu，时长 = %lu。"
								, (DWORD)IntArray[i][0], (DWORD)IntArray[i][1]);
						}
						else  wdo_log_error(lr_TO_STR(Beep));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NOMEMORY();
			}
			else
			{
				if (MessageBeep(0xFFFFFFFF))
				{
					wdo_log_insec(OUTLOG_NORMAL, "Windows generated a system beep.", "Windows 生成了一声系统蜂鸣声。", NULL);
				}
				else wdo_log_error(lr_TO_STR(MessageBeep));

				if (delay.ins) Sleep(delay.ins);
			}
		}
		/* 调用 wdo 文件 */
		else if (!stricmp(instruction, WDOINS_CALL))
		{
			StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

			if (StrArray == NULL && alias.data.path.Type == PATHTYPE_FILE)
			{
				StrArray = (char**)malloc(sizeof(char*));
				if (StrArray != NULL)
				{
					StrArray[0] = (char*)malloc(strlen(alias.data.path.Path) + 1);
					if (StrArray[0] != NULL)
					{
						strcpy(StrArray[0], alias.data.path.Path);
						ArraySize = 1;
					}
					else
					{
						free(StrArray);
						StrArray = NULL;
					}
				}
			}

			if (StrArray != NULL)
			{
				for (size_t i = 0; i < ArraySize; ++i)
				{
					wdo_log(NULL, OUTLOG_NORMAL, "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ %s -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start\n\n", StrArray[i]);

					if (lrWindowDoScript(StrArray[i], TRUE))
					{
						wdo_log_language(OUTLOG_ERROR, "Script call failed!\n", "脚本调用失败！\n", NULL);
					}
					wdo_log(NULL, OUTLOG_NORMAL, "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ %s -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+end\n\n", StrArray[i]);

					if (delay.ins) Sleep(delay.ins);
				}
				lrfrees(&StrArray, ArraySize);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 调用 cmd 指令 */
		else if (!stricmp(instruction, WDOINS_CMD))
		{
			StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

			if (StrArray != NULL)
			{
				for (size_t i = 0; i < ArraySize; ++i)
				{
					system(StrArray[i]);

					if (delay.ins) Sleep(delay.ins);
				}
				lrfrees(&StrArray, ArraySize);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 调用 mci 指令 */
		else if (!stricmp(instruction, WDOINS_MCI))
		{
			StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

			if (StrArray != NULL)
			{
				for (size_t i = 0; i < ArraySize; ++i)
				{
					MCIERROR mciError = 0;

					mciGetErrorStringA(mciError = mciSendStringA(StrArray[i], 0, 0, 0), PublicString, sizeof(PublicString));
					wdo_log_insec(!mciError, "mciErrorCode: %lu\n\nmciToString: %s", "错误码：%lu\n\n解释后：%s", mciError, PublicString);

					if (delay.ins) Sleep(delay.ins);
				}
				lrfrees(&StrArray, ArraySize);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 关于该程序的帮助与教学 */
		else if (!stricmp(instruction, WDOINS_HELP))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						wdo_help(StrArray[i]);

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else wdo_help(NULL);
			}
			else wdo_help(NULL);
		}
		/* 关于 wdo.exe */
		else if (!stricmp(instruction, WDOINS_ABOUT))
		{
			wdo_about(TRUE, wdo_language(LANG_ENGLISH, LANG_CHINESE));
		}
		/* 退出 wdo.exe */
		else if (!stricmp(instruction, WDOINS_EXIT))
		{
			wdo_log_insec(OUTLOG_NORMAL, "The " WDO_EXE_FILE " exited...", WDO_EXE_FILE " 已退出。。。", NULL);

			if (parameter != NULL && parameter[0] != '\0')
			{
				wdo_exit(atoi(parameter));
			}
			else wdo_exit(0);
		}
		/* 申请退出 wdo.exe */
		else if (!stricmp(instruction, WDOINS_EXITASK))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				wdo_exit_ask(atoi(parameter), WDOEXITASK_ENDEDEXIT);
			}
			else wdo_exit_ask(0, WDOEXITASK_ENDEDEXIT);

			wdo_log_insec(OUTLOG_NORMAL
				, "You have applied for exit and are waiting for the following windows and threads to close..."
				, "已申请退出，正在等待以下窗口和线程关闭。。。", NULL);
		}

		/***************************************************************************************
		* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 别名类 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
		****************************************************************************************/

		/* 申请一个指定类型的别名 */
		else if (!stricmp(instruction, WDOINS_ALIAS_APPLY))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						iStringLength = strlen(StrArray[i]) + 1;

						char* type = (char*)malloc(iStringLength);
						char* name = (char*)malloc(iStringLength);

						if (type != NULL && name != NULL)
						{
							lrstrsep(StrArray[i], 2, type, name);
							lrVerifyAliasName(name, TRUE);

							ALIASSINGLE newAlias = { 0 };

							/* 删除别名 */
							if (!stricmp(type, "-del"))
							{
								lrFindAliasInAliasLibary(name, &newAlias);
								lrPutAliasLibrary(&newAlias, TRUE);
								lrClearAlias(&newAlias);
							}
							else
							{
								/* 空类型 */
								if (!stricmp(type, "-null"))
								{
									lrInitAlias(name, alias_type_null, &newAlias);
									lrPutAliasLibrary(&newAlias, FALSE);
								}
								/* 窗口类型 */
								else if (!stricmp(type, "-win"))
								{
									lrInitAlias(name, alias_type_win, &newAlias);
									newAlias.data.win = ctwin;
									lrPutAliasLibrary(&newAlias, FALSE);
								}
								/* 路径类型 */
								else if (!stricmp(type, "-path"))
								{
									lrInitAlias(name, alias_type_path, &newAlias);
									lrPutAliasLibrary(&newAlias, FALSE);
								}
								/* 输出类型 */
								else if (!stricmp(type, "-out"))
								{
									lrInitAlias(name, alias_type_out, &newAlias);
									lrPutAliasLibrary(&newAlias, FALSE);
								}
								/* 绘画类型 */
								else if (!stricmp(type, "-draw"))
								{
									lrInitAlias(name, alias_type_draw, &newAlias);
									lrPutAliasLibrary(&newAlias, FALSE);
								}
								/* 框类型 */
								else if (!stricmp(type, "-box"))
								{
									lrInitAlias(name, alias_type_box, &newAlias);
									lrPutAliasLibrary(&newAlias, FALSE);
								}
								else WDO_ERROR_NOTYPE(type);
							}

							free(type);
							free(name);
						}
						else WDO_ERROR_NOMEMORY();

						if (delay.ins) Sleep(delay.ins);
					}
					if (TRUE)
					{
						/* 打印别名库中所有别名 */
						size_t LibSize = 0;
						ALIASSINGLE* AliasLib = lrGetAliasLibrary(&LibSize);

						for (UINT i = 0; i < LibSize; ++i)
						{
							if ((size_t)i + 1 == LibSize)
								wdo_log(NULL, OUTLOG_NORMAL, "└─");
							else
								wdo_log(NULL, OUTLOG_NORMAL, "│─");

							wdo_log(NULL, OUTLOG_NORMAL
								, wdo_language("ALIAS = %s  >  TYPE = %s\n", "别名 = %s  >  类型 = %s\n")
								, AliasLib[i].name, lrGetAliasTypeString(AliasLib[i].type));
						}
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 获取一个预定义的默认别名 */
		else if (!stricmp(instruction, WDOINS_ALIAS_GET))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						ALIASSINGLE newAlias = { 0 };
						ALIASDEF aliasdef = 0;

						iStringLength = strlen(StrArray[i]) + 1;

						char* type = (char*)malloc(iStringLength);
						char* name = (char*)malloc(iStringLength);

						if (type && name)
						{
							lrstrsep(StrArray[i], 2, type, name);
							lrVerifyAliasName(name, TRUE);

							State = TRUE;

							if (!stricmp(type, "-out")) aliasdef = alias_def_out;
							else if (!stricmp(type, "-draw")) aliasdef = alias_def_draw;
							else if (!stricmp(type, "-box_blank")) aliasdef = alias_def_box_blank;
							else if (!stricmp(type, "-box_text")) aliasdef = alias_def_box_text;
							else if (!stricmp(type, "-box_info")) aliasdef = alias_def_box_info;
							else if (!stricmp(type, "-box_log")) aliasdef = alias_def_box_log;
							else if (!stricmp(type, "-box_image")) aliasdef = alias_def_box_image;
							else if (!stricmp(type, "-box_blue")) aliasdef = alias_def_box_blue;
							else
							{
								State = FALSE;
								WDO_ERROR_NOTYPE(type);
							}
							if (State)
							{
								if (name[0] != '\0')
								{
									lrGetAliasDef(aliasdef, &newAlias);
									strncpy(newAlias.name, name, ALIAS_MAX_LENGTH);
									lrPutAliasLibrary(&newAlias, FALSE);
									if(TRUE) lrPrintAliasTree(&newAlias);
								}
								else WDO_ERROR_NULLALIAS();
							}
							free(type);
							free(name);
						}
						else WDO_ERROR_NOMEMORY();

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 设置别名数据 */
		else if (!stricmp(instruction, WDOINS_ALIAS_SET))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						iStringLength = strlen(StrArray[i]) + 1;

						char* type = (char*)malloc(iStringLength);
						char* set = (char*)malloc(iStringLength);

						if (type != NULL && set != NULL)
						{
							lrstrsep(StrArray[i], 2, type, set);
							ALIASSINGLE* setAlias;

							if (!stricmp(type, "-win"))
							{
								for (size_t number = 0; number < AliasIndexCount; ++number)
								{
									setAlias = lrSetAliasLibrary(AliasIndex[number]);
									if (setAlias->type == alias_type_win)
									{
										setAlias->data.win = ptrwin;
										State = TRUE;

										if (TRUE)
										{
											lrPrintAliasTree(setAlias);
											wdo_putc('\n');
										}
									}
								}
							}

							else if (!stricmp(type, "-path"))
							{
								for (size_t number = 0; number < AliasIndexCount; ++number)
								{
									setAlias = lrSetAliasLibrary(AliasIndex[number]);
									if (setAlias->type == alias_type_path)
									{
										lrstrrem(set, '{', '}');
										lrstrrem(set, '\"', '\"');

										strncpy(setAlias->data.path.Path, set, sizeof(setAlias->data.path.Path));
										setAlias->data.path.Path[sizeof(setAlias->data.path.Path) - 1] = '\0';

										struct stat fileinfo = { 0 };
										stat(setAlias->data.path.Path, &fileinfo);

										if (fileinfo.st_mode & S_IFREG)
										{
											setAlias->data.path.Type = PATHTYPE_FILE;
										}
										else if (fileinfo.st_mode & S_IFDIR)
										{
											setAlias->data.path.Type = PATHTYPE_DIR;
										}
										else setAlias->data.path.Type = PATHTYPE_INVALID;

										State = TRUE;
										if (TRUE)
										{
											lrPrintAliasTree(setAlias);
											wdo_putc('\n');
										}
									}
								}
							}

							else if (!stricmp(type, "-out"))
							{
								for (size_t number = 0; number < AliasIndexCount; ++number)
								{
									setAlias = lrSetAliasLibrary(AliasIndex[number]);
									if (setAlias->type == alias_type_out)
									{
										iStringLength = strlen(set) + 1;

										char* out_type = (char*)malloc(iStringLength);
										char* out_set = (char*)malloc(iStringLength);

										if (out_type != NULL && out_set != NULL)
										{
											lrstrsep(set, 2, out_type, out_set);

											/* 附带的图片 */
											if (!stricmp(out_type, "-image"))
											{
												lrstrrem(out_set, '{', '}');
												lrstrrem(out_set, '\"', '\"');

												if (!access(out_set, 0))
												{
													HBITMAP image = LoadImageA(NULL
														, out_set
														, IMAGE_BITMAP
														, 0, 0
														, LR_LOADFROMFILE
													);
													if (image)
													{
														State = TRUE;

														DeleteObject(image);
														strncpy(setAlias->data.out.image, out_set, sizeof(setAlias->data.out.image));
														setAlias->data.out.image[sizeof(setAlias->data.out.image) - 1] = '\0';
													}
													else
													{
														wdo_log_insec(OUTLOG_ERROR
															, "Alias name: %s\nData state: Setting failed\nFailure reason: Invalid path\nFinal path: %s\n"
															, "别名名称： %s\n数据状态：设置失败\n失败原因：无效路径\n最终路径：%s\n"
															, setAlias->name
															, out_set);
													}

													if (TRUE && State)
													{
														lrPrintAliasTree(setAlias);
														wdo_putc('\n');
													}
												}
												else WDO_ERROR_NOOPENFILE(out_set);
											}
											/* 图片坐标 */
											else if (!stricmp(out_type, "-ipos"))
											{
												iStringLength = strlen(out_set) + 1;

												char* ipos_x = (char*)malloc(iStringLength);
												char* ipos_y = (char*)malloc(iStringLength);

												if (ipos_x && ipos_y)
												{
													lrstrsep(out_set, 2, ipos_x, ipos_y);

													setAlias->data.out.ImageInTextPos.X = (SHORT)atoi(ipos_x);
													setAlias->data.out.ImageInTextPos.Y = (SHORT)atoi(ipos_y);

													free(ipos_x);
													free(ipos_y);

													State = TRUE;
													if (TRUE)
													{
														lrPrintAliasTree(setAlias);
														wdo_putc('\n');
													}
												}
												else WDO_ERROR_NOMEMORY();
											}
											/* 文本颜色 */
											else if (!stricmp(out_type, "-tc"))
											{
												setAlias->data.out.TextColor = (COLORREF)lrHexToDec(out_set);
												
												lr_LIMIT_VALSIZE(setAlias->data.out.TextColor, 0, 0xFFFFFF);
												setAlias->data.out.TextColor = lr_TO_RGB(setAlias->data.out.TextColor);

												State = TRUE;
												if (TRUE)
												{
													lrPrintAliasTree(setAlias);
													wdo_putc('\n');
												}
											}
											/* 背景颜色 */
											else if (!stricmp(out_type, "-bc"))
											{
												setAlias->data.out.BackColor = (COLORREF)lrHexToDec(out_set);

												lr_LIMIT_VALSIZE(setAlias->data.out.BackColor, 0, 0xFFFFFF);
												setAlias->data.out.BackColor = lr_TO_RGB(setAlias->data.out.BackColor);

												State = TRUE;
												if (TRUE)
												{
													lrPrintAliasTree(setAlias);
													wdo_putc('\n');
												}
											}
											/* 文本透明 */
											else if (!stricmp(out_type, "-tr"))
											{
												State = FALSE;

												if (!stricmp(out_set, "on"))
												{
													setAlias->data.out.IsTransparent = TRUE;
													State = TRUE;
												}
												else if (!stricmp(out_set, "off"))
												{
													setAlias->data.out.IsTransparent = FALSE;
													State = TRUE;
												}
												else WDO_ERROR_NOSWITCH(out_set);

												if (TRUE && State)
												{
													lrPrintAliasTree(setAlias);
													wdo_putc('\n');
												}
											}
											/* 字体 */
											else if (!stricmp(out_type, "-font"))
											{
												iStringLength = strlen(out_set) + 1;

												char* font_type = (char*)malloc(iStringLength);
												char* font_name = (char*)malloc(iStringLength);

												if (font_type != NULL && font_name != NULL)
												{
													lrstrsep(out_set, 2, font_type, font_name);

													if (!stricmp(font_type, "-name") || !stricmp(font_type, "-n"))
													{
														strncpy(setAlias->data.out.fontinfo.name, font_name, sizeof(setAlias->data.out.fontinfo.name));
														setAlias->data.out.fontinfo.name[sizeof(setAlias->data.out.fontinfo.name) - 1] = '\0';
													}
													else if (!stricmp(font_type, "-width") || !stricmp(font_type, "-w"))
													{
														setAlias->data.out.fontinfo.width = atoi(font_name);
													}
													else if (!stricmp(font_type, "-height") || !stricmp(font_type, "-h"))
													{
														setAlias->data.out.fontinfo.height = atoi(font_name);
													}
													else if (!stricmp(font_type, "-weight") || !stricmp(font_type, "-wei"))
													{
														setAlias->data.out.fontinfo.weight = atoi(font_name) < 1000 ? atoi(font_name) : 1000;
													}
													else if (!stricmp(font_name, "on") || !stricmp(font_name, "off"))
													{
														if (!stricmp(font_type, "-italic") || !stricmp(font_type, "-i"))
														{
															setAlias->data.out.fontinfo.Italic = !stricmp(font_name, "on") ? TRUE : FALSE;
														}
														else if (!stricmp(font_type, "-under") || !stricmp(font_type, "-u"))
														{
															setAlias->data.out.fontinfo.Underline = !stricmp(font_name, "on") ? TRUE : FALSE;
														}
														else if (!stricmp(font_type, "-strike") || !stricmp(font_type, "-s"))
														{
															setAlias->data.out.fontinfo.StrikeOut = !stricmp(font_name, "on") ? TRUE : FALSE;
														}
														else WDO_ERROR_NOTYPE(font_type);
													}
													else WDO_ERROR_NOTYPE(font_type);

													HFONT hFont = lr_CreateFont(
														setAlias->data.out.fontinfo.height,
														setAlias->data.out.fontinfo.width,
														setAlias->data.out.fontinfo.weight,
														setAlias->data.out.fontinfo.Italic,
														setAlias->data.out.fontinfo.Underline,
														setAlias->data.out.fontinfo.StrikeOut,
														setAlias->data.out.fontinfo.name
													);


													if (hFont)
													{
														/* 先释放原来字体 */
														if (setAlias->data.out.hFont) DeleteObject(setAlias->data.out.hFont);
														setAlias->data.out.hFont = hFont;

														State = TRUE;
														if (TRUE)
														{
															lrPrintAliasTree(setAlias);
															wdo_putc('\n');
														}
													}
													else wdo_log_insec(OUTLOG_ERROR
														, "Alias name: %s\nData state: Setting failed\nFailure reason: Font loading failed\n"
														, "别名名称： %s\n数据状态：设置失败\n失败原因：字体加载失败\n"
														, setAlias->name);

													free(font_type);
													free(font_name);
												}
												else WDO_ERROR_NOMEMORY();
											}

											else WDO_ERROR_NOTYPE(out_type);

											free(out_type);
											free(out_set);
										}
										else WDO_ERROR_NOMEMORY();
									}
								}
							}

							else if (!stricmp(type, "-draw"))
							{
								for (size_t number = 0; number < AliasIndexCount; ++number)
								{
									setAlias = lrSetAliasLibrary(AliasIndex[number]);
									if (setAlias->type == alias_type_draw)
									{
										iStringLength = strlen(set) + 1;

										char* draw_type = (char*)malloc(iStringLength);
										char* draw_set = (char*)malloc(iStringLength);

										if (draw_type && draw_set)
										{
											lrstrsep(set, 2, draw_type, draw_set);

											/* 描边颜色 */
											if (!stricmp(draw_type, "-sc"))
											{
												setAlias->data.draw.StrokeColor = (COLORREF)lrHexToDec(draw_set);
												lr_LIMIT_VALSIZE(setAlias->data.draw.StrokeColor, 0, 0xFFFFFF);
												setAlias->data.draw.StrokeColor = lr_TO_RGB(setAlias->data.draw.StrokeColor);

												State = TRUE;
												if (TRUE)
												{
													lrPrintAliasTree(setAlias);
													wdo_putc('\n');
												}
											}
											/* 描边宽度 */
											else if (!stricmp(draw_type, "-sw"))
											{
												setAlias->data.draw.StrokeWidth = (BYTE)atoi(draw_set);

												State = TRUE;
												if (TRUE)
												{
													lrPrintAliasTree(setAlias);
													wdo_putc('\n');
												}
											}
											/* 是否填充 */
											else if (!stricmp(draw_type, "-fill"))
											{
												State = FALSE;

												if (!stricmp(draw_set, "on"))
												{
													setAlias->data.draw.Fill = TRUE;
													State = TRUE;
												}
												else if (!stricmp(draw_set, "off"))
												{
													setAlias->data.draw.Fill = FALSE;
													State = TRUE;
												}
												else WDO_ERROR_NOSWITCH(draw_set);

												if (TRUE && State)
												{
													lrPrintAliasTree(setAlias);
													wdo_putc('\n');
												}
											}
											/* 填充颜色 */
											else if (!stricmp(draw_type, "-fc"))
											{
												setAlias->data.draw.FillColor = (COLORREF)lrHexToDec(draw_set);
												lr_LIMIT_VALSIZE(setAlias->data.draw.FillColor, 0, 0xFFFFFF);
												setAlias->data.draw.FillColor = lr_TO_RGB(setAlias->data.draw.FillColor);

												State = TRUE;
												if (TRUE)
												{
													lrPrintAliasTree(setAlias);
													wdo_putc('\n');
												}
											}
											/* 填充图片 */
											else if (!stricmp(draw_type, "-fi"))
											{
												lrstrrem(draw_set, '{', '}');
												lrstrrem(draw_set, '\"', '\"');

												if (!access(draw_set, 0))
												{
													HBITMAP image = LoadImageA(NULL
														, draw_set
														, IMAGE_BITMAP
														, 0, 0
														, LR_LOADFROMFILE
													);
													if (image)
													{
														State = TRUE;
														DeleteObject(image);
														strncpy(setAlias->data.draw.FillImage, draw_set, sizeof(setAlias->data.draw.FillImage));
														setAlias->data.draw.FillImage[sizeof(setAlias->data.draw.FillImage) - 1] = '\0';
													}
													else
													{
														wdo_log_insec(OUTLOG_ERROR
															, "Alias name: %s\nData state: Setting failed\nFailure reason: Invalid path\nFinal path: %s\n"
															, "别名名称： %s\n数据状态：设置失败\n失败原因：无效路径\n最终路径：%s\n"
															, setAlias->name
															, draw_set);
													}

													if (TRUE && State)
													{
														lrPrintAliasTree(setAlias);
														wdo_putc('\n');
													}
												}
											}

											else WDO_ERROR_NOTYPE(draw_type);

											free(draw_type);
											free(draw_set);
										}
										else WDO_ERROR_NOMEMORY();
									}
								}
							}

							else if (!stricmp(type, "-box"))
							{
								for (size_t number = 0; number < AliasIndexCount; ++number)
								{
									setAlias = lrSetAliasLibrary(AliasIndex[number]);
									if (setAlias->type == alias_type_box)
									{
										iStringLength = strlen(set) + 1;

										char* box_type = (char*)malloc(iStringLength);
										char* box_set = (char*)malloc(iStringLength);

										if (box_type != NULL && box_set != NULL)
										{
											lrstrsep(set, 2, box_type, box_set);

											/* 类名 */
											if (!stricmp(box_type, "-class"))
											{
												strncpy(setAlias->data.box.ClassName, box_set, sizeof(setAlias->data.box.ClassName));
												setAlias->data.box.ClassName[sizeof(setAlias->data.box.ClassName) - 1] = '\0';

												State = TRUE;
												if (TRUE)
												{
													lrPrintAliasTree(setAlias);
													wdo_putc('\n');
												}
											}
											/* 游标 */
											else if (!stricmp(box_type, "-cursor"))
											{
												HCURSOR hCursor = (HCURSOR)LoadImageA(NULL, box_set, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
												if (hCursor)
												{
													if (setAlias->data.box.hCursor) DestroyCursor(setAlias->data.box.hCursor);
													setAlias->data.box.hCursor = hCursor;

													State = TRUE;
													if (TRUE)
													{
														lrPrintAliasTree(setAlias);
														wdo_putc('\n');
													}
												}
												else wdo_log_insec(OUTLOG_ERROR
													, "Alias name: %s\nData state: Setting failed\nFailure reason: Invalid path\nFinal path: %s\n"
													, "别名名称： %s\n数据状态：设置失败\n失败原因：无效路径\n最终路径：%s\n"
													, setAlias->name
													, box_set);
											}
											/* 图标 */
											else if (!stricmp(box_type, "-icon"))
											{
												HICON hIcon = (HICON)LoadImageA(NULL, box_set, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
												if (hIcon)
												{
													if (setAlias->data.box.hIcon) DestroyIcon(setAlias->data.box.hIcon);
													setAlias->data.box.hIcon = hIcon;

													State = TRUE;
													if (TRUE)
													{
														lrPrintAliasTree(setAlias);
														wdo_putc('\n');
													}
												}
												else wdo_log_insec(OUTLOG_ERROR
													, "Alias name: %s\nData state: Setting failed\nFailure reason: Invalid path\nFinal path: %s\n"
													, "别名名称： %s\n数据状态：设置失败\n失败原因：无效路径\n最终路径：%s\n"
													, setAlias->name
													, box_set);
											}
											/* 字体 */
											else if (!stricmp(box_type, "-font"))
											{
												iStringLength = strlen(box_set) + 1;

												char* font_type = (char*)malloc(iStringLength);
												char* font_name = (char*)malloc(iStringLength);

												if (font_type != NULL && font_name != NULL)
												{
													lrstrsep(box_set, 2, font_type, font_name);

													if (!stricmp(font_type, "-name") || !stricmp(font_type, "-n"))
													{
														strncpy(setAlias->data.box.fontinfo.name, font_name, sizeof(setAlias->data.box.fontinfo.name));
														setAlias->data.box.fontinfo.name[sizeof(setAlias->data.box.fontinfo.name) - 1] = '\0';
													}
													else if (!stricmp(font_type, "-width") || !stricmp(font_type, "-w"))
													{
														setAlias->data.box.fontinfo.width = atoi(font_name);
													}
													else if (!stricmp(font_type, "-height") || !stricmp(font_type, "-h"))
													{
														setAlias->data.box.fontinfo.height = atoi(font_name);
													}
													else if (!stricmp(font_type, "-weight") || !stricmp(font_type, "-wei"))
													{
														setAlias->data.box.fontinfo.weight = atoi(font_name) < 1000 ? atoi(font_name) : 1000;
													}
													else if (!stricmp(font_name, "on") || !stricmp(font_name, "off"))
													{
														if (!stricmp(font_type, "-italic") || !stricmp(font_type, "-i"))
														{
															setAlias->data.box.fontinfo.Italic = !stricmp(font_name, "on") ? TRUE : FALSE;
														}
														else if (!stricmp(font_type, "-under") || !stricmp(font_type, "-u"))
														{
															setAlias->data.box.fontinfo.Underline = !stricmp(font_name, "on") ? TRUE : FALSE;
														}
														else if (!stricmp(font_type, "-strike") || !stricmp(font_type, "-s"))
														{
															setAlias->data.box.fontinfo.StrikeOut = !stricmp(font_name, "on") ? TRUE : FALSE;
														}
														else WDO_ERROR_NOTYPE(font_type);
													}
													else WDO_ERROR_NOTYPE(font_type);

													HFONT hFont = lr_CreateFont(
														setAlias->data.box.fontinfo.height,
														setAlias->data.box.fontinfo.width,
														setAlias->data.box.fontinfo.weight,
														setAlias->data.box.fontinfo.Italic,
														setAlias->data.box.fontinfo.Underline,
														setAlias->data.box.fontinfo.StrikeOut,
														setAlias->data.box.fontinfo.name
													);


													if (hFont)
													{
														if (setAlias->data.out.hFont) DeleteObject(setAlias->data.out.hFont);
														setAlias->data.box.hFont = hFont;

														State = TRUE;
														if (TRUE)
														{
															lrPrintAliasTree(setAlias);
															wdo_putc('\n');
														}
													}
													else wdo_log_insec(OUTLOG_ERROR
														, "Alias name: %s\nData state: Setting failed\nFailure reason: Font loading failed\n"
														, "别名名称： %s\n数据状态：设置失败\n失败原因：字体加载失败\n"
														, setAlias->name);

													free(font_type);
													free(font_name);
												}
												else WDO_ERROR_NOMEMORY();
											}
											/* 文本颜色 */
											else if (!stricmp(box_type, "-tc"))
											{
												setAlias->data.box.TextColor = (COLORREF)lrHexToDec(box_set);
												lr_LIMIT_VALSIZE(setAlias->data.box.TextColor, 0, 0xFFFFFF);
												setAlias->data.box.TextColor = lr_TO_RGB(setAlias->data.box.TextColor);

												State = TRUE;
												if (TRUE)
												{
													lrPrintAliasTree(setAlias);
													wdo_putc('\n');
												}
											}
											/* 背景颜色 */
											else if (!stricmp(box_type, "-bc"))
											{
												setAlias->data.box.BackColor = (COLORREF)lrHexToDec(box_set);
												lr_LIMIT_VALSIZE(setAlias->data.box.BackColor, 0, 0xFFFFFF);
												setAlias->data.box.BackColor = lr_TO_RGB(setAlias->data.box.BackColor);

												State = TRUE;
												if (TRUE)
												{
													lrPrintAliasTree(setAlias);
													wdo_putc('\n');
												}
											}

											else WDO_ERROR_NOTYPE(box_type);

											free(box_type);
											free(box_set);
										}
										else WDO_ERROR_NOMEMORY();
									}
								}
							}

							else WDO_ERROR_NOTYPE(type);

							free(type);
							free(set);
						}
						else WDO_ERROR_NOMEMORY();

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 安装别名 */
		else if (!stricmp(instruction, WDOINS_ALIAS_SETUP))
		{
			if (IsWindow(ctwin))
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (strlen(StrArray[i]) > ALIAS_MAX_LENGTH) StrArray[i][ALIAS_MAX_LENGTH - 1] = '\0';

						ALIASSINGLE setup_alias;
						if (lrFindAliasInAliasLibary(StrArray[i], &setup_alias))
						{
							if (setup_alias.type == alias_type_box)
							{
								if (lrSetBoxAlias(ctwin, &(setup_alias.data.box)))
								{
									wdo_log_insec(OUTLOG_NORMAL
										, "Window _%X_ setup alias. > ALIAS = %s"
										, "窗口 _%X_ 已安装别名。> ALIAS = %s"
										, ctwin, setup_alias.name
									);
								}
								else wdo_log_insec(OUTLOG_ERROR
									, "Window _%X_ the alias cannot be setup, this window may not have been created by instructions such as \"" WDOINS_BOX_TEXT "\"."
									, "窗口 _%X_ 无法安装别名，此窗口可能不是由 \"" WDOINS_BOX_TEXT "\" 等指令创建。"
									, ctwin
								);
							}
							else WDO_ERROR_MISSINGALIAS(alias_type_box);
						}
						else WDO_ERROR_NOALIAS(StrArray[i]);

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else if (AliasBools[alias_type_box] == TRUE)
				{
					if (lrSetBoxAlias(ctwin, &(alias.data.box)))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ setup alias."
							, "窗口 _%X_ 已安装别名。"
							, ctwin
						);
					}
					else wdo_log_insec(OUTLOG_ERROR
						, "Window _%X_ the alias cannot be setup, this window may not have been created by instructions such as \"" WDOINS_BOX_TEXT "\"."
						, "窗口 _%X_ 无法安装别名，此窗口可能不是由 \"" WDOINS_BOX_TEXT "\" 等指令创建。"
						, ctwin
					);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 显示别名结构 */
		else if (!stricmp(instruction, WDOINS_ALIAS_SEE))
		{
			if (TRUE)
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							/* 所有库 */
							if (!stricmp(StrArray[i], "-all"))
							{
								size_t LibSize = 0;
								ALIASSINGLE* AliasLib = lrGetAliasLibrary(&LibSize);

								for (size_t i = 0; i < LibSize; ++i)
								{
									wdo_log(instruction, OUTLOG_NORMAL, "Number <%llu>\n", i + 1);
									lrPrintAliasTree(&AliasLib[i]);
									wdo_putc('\n');
								}
							}
							else
							{
								ALIASSINGLE outAlias;
								if (lrFindAliasInAliasLibary(StrArray[i], &outAlias))
								{
									lrPrintAliasTree(&outAlias);
									State = TRUE;
								}
								else WDO_ERROR_NOALIAS(StrArray[i]);
							}

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else
				{
					/* 打印别名库中所有别名 */
					size_t LibSize = 0;
					ALIASSINGLE* AliasLib = lrGetAliasLibrary(&LibSize);

					for (UINT i = 0; i < LibSize; ++i)
					{
						if ((size_t)i + 1 == LibSize)
							wdo_log(NULL, OUTLOG_NORMAL, "└─");
						else
							wdo_log(NULL, OUTLOG_NORMAL, "│─");

						wdo_log(NULL, OUTLOG_NORMAL
							, wdo_language("ALIAS = %s  >  TYPE = %s\n", "别名 = %s  >  类型 = %s\n")
							, AliasLib[i].name, lrGetAliasTypeString(AliasLib[i].type));
					}
					if (LibSize == 0) wdo_log_insec(OUTLOG_NORMAL
						, "Alias library is empty."
						, "别名库为空。"
						, NULL);
				}
			}
		}
		/* 保存一个别名文件 */
		else if (!stricmp(instruction, WDOINS_ALIAS_SAVE))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						iStringLength = strlen(StrArray[i]) + 1;

						char* AliasName = (char*)malloc(iStringLength);

						if (AliasName)
						{
							lrstrsep(StrArray[i], 2, AliasName, PublicString);

							/* 路径为空则检索别名数据 */
							if (PublicString[0] == '\0' && alias.data.path.Type == PATHTYPE_FILE)
							{
								strncpy(PublicString, alias.data.path.Path, MAX_PATH);
								PublicString[MAX_PATH - 1] = '\0';
							}
							lrstrrem(PublicString, '{', '}');
							lrstrrem(PublicString, '\"', '\"');

							/* 保存整个库 */
							if (!stricmp(AliasName, "-lib"))
							{
								if (lrSaveAlias(PublicString, TRUE, NULL))
								{
									wdo_log_insec(OUTLOG_NORMAL
										, "Alias library saved in file \"%s\"."
										, "别名库已保存在文件 \"%s\" 中。"
										, PublicString
									);
								}
								else wdo_log_insec(OUTLOG_ERROR
									, "Save alias library failed!\nThe path is %s"
									, "别名库保存失败！ \n路径： %s"
									, PublicString
								);
							}
							/* 保存单个别名 */
							else
							{
								ALIASSINGLE SaveAlias;
								if (lrFindAliasInAliasLibary(AliasName, &SaveAlias))
								{
									if (lrSaveAlias(PublicString, FALSE, &SaveAlias))
									{
										wdo_log_insec(OUTLOG_NORMAL
											, "Alias %s saved in file \"%s\"."
											, "别名 %s 已保存在文件 \"%s\" 中。"
											, AliasName, PublicString
										);
									}
									else wdo_log_insec(OUTLOG_ERROR
										, "Save alias \"%s\" failed! Please check whether the alias or path is correct.\nThe path is %s"
										, "别名 \"%s\" 保存失败！请检查别名或路径是否正确。\n路径： %s"
										, AliasName, PublicString
									);
								}
								else WDO_ERROR_NOALIAS(AliasName);
							}

							free(AliasName);
						}
						else WDO_ERROR_NOMEMORY();

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else if(TRUE) WDO_ERROR_NULLPARAM(instruction);
		}
		/* 加载一个别名文件 */
		else if (!stricmp(instruction, WDOINS_ALIAS_LOAD))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray == NULL && alias.data.path.Type == PATHTYPE_FILE)
				{
					StrArray = (char**)malloc(sizeof(char*));
					if (StrArray != NULL)
					{
						StrArray[0] = (char*)malloc(strlen(alias.data.path.Path) + 1);
						if (StrArray[0] != NULL)
						{
							strcpy(StrArray[0], alias.data.path.Path);
							ArraySize = 1;
						}
						else
						{
							free(StrArray);
							StrArray = NULL;
						}
					}
				}

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						lrstrrem(StrArray[i], '{', '}');
						lrstrrem(StrArray[i], '\"', '\"');

						FALIAS fAlias;

						if (lrLoadAlias(StrArray[i], &fAlias))
						{
							/* 放入别名库 */
							for (UINT j = 0; j < fAlias.fAliasHeader.AliasCount; ++j)
							{
								if (fAlias.fAliasTexts[j].type == alias_type_box)
								{
									if (fAlias.fAliasTexts[j].data.box.hFont)
									{
										fAlias.fAliasTexts[j].data.box.hFont = lr_CreateFont(
											fAlias.fAliasTexts[j].data.box.fontinfo.height,
											fAlias.fAliasTexts[j].data.box.fontinfo.width,
											fAlias.fAliasTexts[j].data.box.fontinfo.weight,
											fAlias.fAliasTexts[j].data.box.fontinfo.Italic,
											fAlias.fAliasTexts[j].data.box.fontinfo.Underline,
											fAlias.fAliasTexts[j].data.box.fontinfo.StrikeOut,
											fAlias.fAliasTexts[j].data.box.fontinfo.name
										);
									}
								}
								else if (fAlias.fAliasTexts[j].type = alias_type_out)
								{
									if (fAlias.fAliasTexts[j].data.out.hFont)
									{
										fAlias.fAliasTexts[j].data.out.hFont = lr_CreateFont(
											fAlias.fAliasTexts[j].data.out.fontinfo.height,
											fAlias.fAliasTexts[j].data.out.fontinfo.width,
											fAlias.fAliasTexts[j].data.out.fontinfo.weight,
											fAlias.fAliasTexts[j].data.out.fontinfo.Italic,
											fAlias.fAliasTexts[j].data.out.fontinfo.Underline,
											fAlias.fAliasTexts[j].data.out.fontinfo.StrikeOut,
											fAlias.fAliasTexts[j].data.out.fontinfo.name
										);
									}
								}
								lrPutAliasLibrary(&(fAlias.fAliasTexts[j]), FALSE);
							}

							if (TRUE)
							{
								for (UINT quan = 0; quan < fAlias.fAliasHeader.AliasCount; ++quan)
								{
									if (quan + 1 == fAlias.fAliasHeader.AliasCount)
									{
										wdo_log(NULL, OUTLOG_NORMAL, "└─");
									}
									else wdo_log(NULL, OUTLOG_NORMAL, "│─");

									wdo_log_language(OUTLOG_NORMAL
										, "ALIAS = %s  >  TYPE = %s\n"
										, "别名 = %s  >  类型 = %s\n"
										, fAlias.fAliasTexts[quan].name, lrGetAliasTypeString(fAlias.fAliasTexts[quan].type));
								}
								wdo_log_insec(OUTLOG_NORMAL
									, "\nLoaded alias file from user %s with %u aliases in total."
									, "\n已加载来自用户 %s 的别名文件，共包含 %u 个别名。"
									, fAlias.fAliasHeader.UserName, fAlias.fAliasHeader.AliasCount);
							}
							free(fAlias.fAliasTexts);
						}
						else wdo_log_insec(OUTLOG_ERROR
							, "Load alias file failed! Please check whether the file is correct.\nThe path is %s"
							, "别名文件加载失败！请检查文件是否正确。\n路径：%s"
							, StrArray[i]
						);

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}


		/***************************************************************************************
		* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 窗口类 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
		****************************************************************************************/

		/* 指定一个窗口 */
		else if (!stricmp(instruction, WDOINS_PTR))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						iStringLength = strlen(StrArray[i]) + 1;

						char* ptr_type = (char*)malloc(iStringLength);
						char* ptr_text = (char*)malloc(iStringLength);

						if (ptr_type != NULL && ptr_text != NULL)
						{
							lrstrsep(StrArray[i], 2, ptr_type, ptr_text);

							/* 类型: 预设 */
							if (!stricmp(ptr_type, "-p"))
							{
								/* 指定顶层窗口 */
								if (!stricmp(ptr_text, "top")) ptrwin = ctwin = GetForegroundWindow();
								/* 指定桌面窗口 */
								else if (!stricmp(ptr_text, "desk")) ptrwin = ctwin = GetDesktopWindow();
								/* 指定活动窗口 */
								else if (!stricmp(ptr_text, "act")) ptrwin = ctwin = GetActiveWindow();
								/* 指定当前具有焦点的窗口 */
								else if (!stricmp(ptr_text, "foc")) ptrwin = ctwin = GetFocus();
								/* 指定父窗口 */
								else if (!stricmp(ptr_text, "par"))
								{
									HWND newWin = GetParent(ctwin);

									if (newWin) ptrwin = ctwin = newWin;

									else wdo_log_insec(OUTLOG_ERROR
										, "Window _%X_ no parent window."
										, "窗口 _%X_ 没有父窗口。"
										, ctwin
									);
								}
								/* 指定当前游标所在的窗口 */
								else if (!stricmp(ptr_text, "cur"))
								{
									POINT curpos;
									GetCursorPos(&curpos);

									ptrwin = ctwin = WindowFromPoint(curpos);
								}
								/* 指定当前游标所在的父窗口 */
								else if (!stricmp(ptr_text, "cp"))
								{
									POINT curpos;
									GetCursorPos(&curpos);

									HWND newWin = WindowFromPoint(curpos);

									HWND newParWin = GetParent(newWin);

									if (newParWin) ptrwin = ctwin = newParWin;
									else if (newWin) ptrwin = ctwin = newWin;
								}
								/* 指定一个随机的父窗口 */
								else if (!stricmp(ptr_text, "rand"))
								{
									size_t WndQuantity = 0;
									HWND* Wnds = lrGetWindowHwndSets(&WndQuantity);

									if (Wnds && WndQuantity)
									{
										lr_srand();

										ptrwin = ctwin = Wnds[rand() % WndQuantity];

										free(Wnds);
									}
								}
								/* 指定一个当前父窗口的随机子窗口 */
								else if (!stricmp(ptr_text, "crand"))
								{
									HWND ParentWin = GetParent(ctwin);

									size_t ChildWndQuantity = 0;
									HWND* ChildWnds = lrGetChildWindowHwndSets(ParentWin ? ParentWin : ctwin, &ChildWndQuantity);

									if (ChildWnds && ChildWndQuantity)
									{
										lr_srand();

										ptrwin = ctwin = ChildWnds[rand() % ChildWndQuantity];

										free(ChildWnds);
									}
								}
							}

							/* 类型: 标题文本 */
							else if (!stricmp(ptr_type, "-t"))
							{
								HWND newWin = lrFindWindow((lrFIND_WINDOW_TYPE) { LR__FIND_WINDOW_TYPE__WINDOWTITLE, ptr_text }, TRUE);

								if (newWin) ptrwin = ctwin = newWin;

								else wdo_log_insec(OUTLOG_ERROR
									, "No find window of the specified title, TITLE = %s"
									, "没有发现指定标题的窗口。TITLE = %s"
									, ptr_text
								);
							}

							/* 类型: 类名 */
							else if (!stricmp(ptr_type, "-c"))
							{
								HWND newWin = lrFindWindow((lrFIND_WINDOW_TYPE) { LR__FIND_WINDOW_TYPE__WINDOWCLASS, ptr_text }, TRUE);

								if (newWin) ptrwin = ctwin = newWin;

								else wdo_log_insec(OUTLOG_ERROR
									, "No find window of the specified class name, CLASS = %s"
									, "没有发现指定类名的窗口。CLASS = %s"
									, ptr_text
								);
							}

							/* 类型: 应用程序 */
							else if (!stricmp(ptr_type, "-f"))
							{
								HWND newWin = lrFindWindow((lrFIND_WINDOW_TYPE) { LR__FIND_WINDOW_TYPE__WINDOWPPATH, ptr_text }, TRUE);

								if (newWin) ptrwin = ctwin = newWin;

								else wdo_log_insec(OUTLOG_ERROR
									, "No find window of the specified process path, PPATH = %s"
									, "没有发现指定进程路径的窗口。PPATH = %s"
									, ptr_text
								);
							}

							/* 类型: 窗口唯一标识符 */
							else if (!stricmp(ptr_type, "-i"))
							{
								ptrwin = ctwin = (HWND)lrHexToDec(ptr_text);
							}

							/* 类型: 指定点的窗口 */
							else if (!stricmp(ptr_type, "-d"))
							{
								iStringLength = strlen(ptr_text) + 1;

								char* x = (char*)malloc(iStringLength);
								char* y = (char*)malloc(iStringLength);

								if (x && y)
								{
									lrstrsep(ptr_text, 2, x, y);

									POINT point = { atol(x),atol(y) };
									ptrwin = ctwin = WindowFromPoint(point);

									free(x);
									free(y);
								}
								else WDO_ERROR_NOMEMORY();
							}

							else WDO_ERROR_NOTYPE(ptr_type);

							free(ptr_type); free(ptr_text);

							ctwin = ptrwin;

							/* 判断窗口句柄是否是有效窗口 */
							if (IsWindow(ptrwin) && TRUE)
							{
								wdo_log_win_brief(ptrwin, TRUE);
							}
							else WDO_ERROR_NOWINDOW(ptrwin);
						}
						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else
			{
				ptrwin = ctwin;

				/* 判断窗口句柄是否是有效窗口 */
				if (IsWindow(ptrwin) && TRUE)
				{
					wdo_log_win_brief(ptrwin, TRUE);
				}
				else WDO_ERROR_NOWINDOW(ptrwin);
			}
		}
		/* ptr 更新开关 */
		else if (!stricmp(instruction, WDOINS_UPDATEPTR))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (!stricmp(StrArray[i], "on"))
						{
							UpdatePtrwin = TRUE;

							wdo_log_insec(OUTLOG_NORMAL
								, "Update switch of " WDOINS_PTR " on."
								, WDOINS_PTR " 的更新开关已打开。"
								, NULL
							);
						}
						else if (!stricmp(StrArray[i], "off"))
						{
							UpdatePtrwin = FALSE;

							wdo_log_insec(OUTLOG_NORMAL
								, "Update switch of " WDOINS_PTR " off."
								, WDOINS_PTR " 的更新开关已关闭。"
								, NULL
							);
						}
						else WDO_ERROR_NOSWITCH(StrArray[i]);

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 打开一个窗口 */
		else if (!stricmp(instruction, WDOINS_OPEN))
		{
			StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

			if (StrArray == NULL && IsWindow(ctwin)) {
				StrArray = (char**)malloc(sizeof(char*));
				if (StrArray) {
					if (StrArray[0] = (char*)malloc(MAX_PATH)) {
						ArraySize = 1;

						lrGetWindowProcessPath(ctwin, StrArray[0], MAX_PATH);
					}
					else {
						free(StrArray);
						StrArray = NULL;
					}
				}
			}

			if (StrArray != NULL)
			{
				for (size_t i = 0; i < ArraySize; ++i)
				{
					lrstrrem(StrArray[i], '{', '}');
					lrstrrem(StrArray[i], '\"', '\"');

					/* 返回值 > 32 则成功 */
					if ((INT_PTR)ShellExecuteA(NULL, "open", StrArray[i], NULL, NULL, SW_SHOW) > 32)
					{
						HWND* winSets = NULL;
						UINT winQuan = 0;
						HWND newPtr = lrFindProcessPathWindow(StrArray[i], &winSets, &winQuan);

						if (newPtr && winSets)
						{
							ctwin = newPtr;
							if (UpdatePtrwin) ptrwin = ctwin;

							wdo_log_insec(OUTLOG_NORMAL,
								"Window _%X_ from path \"%s\" opened, " WDOINS_PTR " pointed new window _%X_.",
								"窗口 _%X_ 的归属路径 \"%s\" 已被打开，" WDOINS_PTR " 已指向新窗口 _%X_。", ctwin, StrArray[i], newPtr);
							for (UINT i = 0; i < winQuan; ++i)
							{
								if (i + 1 != winQuan)
									wdo_log(NULL, OUTLOG_NORMAL, "│─");
								else
									wdo_log(NULL, OUTLOG_NORMAL, "└─");

								wdo_log_win_brief(winSets[i], TRUE);
							}
							free(winSets);
						}
						else wdo_log_insec(OUTLOG_NORMAL,
							"Window _%X_ from path \"%s\" opened, no window was created.",
							"窗口 _%X_ 的归属路径 \"%s\" 已被打开，但是并未创建窗口。", ctwin, StrArray[i]);
					}
					else
					{
						wdo_log_insec(OUTLOG_ERROR,
							"Window _%X_ from path \"%s\" open failed!\n",
							"窗口 _%X_ 的归属路径 \"%s\" 已被打开失败！\n", ctwin, StrArray[i]);

						wdo_log_error(lr_TO_STR(ShellExecuteA));
					}

					if (delay.ins) Sleep(delay.ins);
				}
				lrfrees(&StrArray, ArraySize);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 遍历所有窗口并打印 */
		else if (!stricmp(instruction, WDOINS_LIST))
		{
			if (TRUE)
			{
				lrstrrem(parameter, '{', '}');

				size_t WinQuantity = 0;
				HWND* wins = NULL;

				/* 仅遍历当前窗口的子窗口 */
				if (!stricmp(parameter, "-ct"))
				{
					wins = (HWND*)malloc(sizeof(HWND));

					if (wins) {
						wins[0] = ctwin;
						WinQuantity = 1;
					}
				}
				else wins = lrGetWindowHwndSets(&WinQuantity);

				size_t childwin_all_quan = 0;

				BOOL IncludeChild = FALSE;
				if (!stricmp(parameter, "-all") || !stricmp(parameter, "-ct")) IncludeChild = TRUE;

				/* 循环打印 */
				for (size_t i = 0; i < WinQuantity; ++i)
				{
					if (i + 1 != WinQuantity)
						wdo_log(NULL, OUTLOG_NORMAL, "│─");
					else
						wdo_log(NULL, OUTLOG_NORMAL, "└─");

					wdo_log_win_brief(wins[i], FALSE);

					/* 包括子窗口 */
					if (IncludeChild)
					{
						size_t childwin_quan = 0;
						HWND* childwin = lrGetChildWindowHwndSets(wins[i], &childwin_quan);

						for (size_t j = 0; j < childwin_quan; ++j)
						{
							if (i + 1 != WinQuantity)
							{
								if (j + 1 == childwin_quan)
									wdo_log(NULL, OUTLOG_NORMAL, wdo_language("│ └─C", "│ └─子"));

								else
									wdo_log(NULL, OUTLOG_NORMAL, wdo_language("│ │─C", "│ │─子"));
							}
							else
							{
								if (j + 1 == childwin_quan)
									wdo_log(NULL, OUTLOG_NORMAL, wdo_language("   └─C", "   └─子"));

								else
									wdo_log(NULL, OUTLOG_NORMAL, wdo_language("   │─C", "   │─子"));
							}
							wdo_log_win_brief(childwin[j], FALSE);
						}
						free(childwin);
						childwin_all_quan += childwin_quan;
					}
				}

				free(wins);

				wdo_putc('\n');

				wdo_log_insec(OUTLOG_NORMAL,
					"A total of <%llu> windows were found...",
					"总共找到了 <%llu> 个窗口。。。"
					, WinQuantity + childwin_all_quan
				);
			}
			if (delay.ins) Sleep(delay.ins);
		}
		/* 打印窗口信息 */
		else if (!stricmp(instruction, WDOINS_INFO))
		{
			if (TRUE)
			{
				WINDOWALLINFO info;
				lrGetWindowAllInfo(ctwin, &info);

				char binaryStr[33];
				State = OUTLOG_NORMAL;

				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window handle            |   _%X_(hex)  Or %u(decimal)",
					"窗口句柄                  |   _%X_(hex)  Or %u(decimal)"), ctwin, ctwin);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Process ID               |   _%X_(hex)  Or %u(decimal)",
					"进程 ID                   |   _%X_(hex)  Or %u(decimal)"), info.PID, info.PID);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window title text        |   %s",
					"窗口标题文本              |   %s"), info.TITLE);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window class name        |   %s",
					"窗口类名                  |   %s"), info.CLASS);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Process path             |   %s",
					"进程路径                  |   %s"), info.PPATH);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window vertex in screen  |   X- %d  Y- %d",
					"窗口在子窗口中的顶点      |   X- %d  Y- %d"), info.VERTEX.left, info.VERTEX.top);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window vertex in parent  |   X- %d  Y- %d",
					"窗口在父窗口中的顶点      |   X- %d  Y- %d"), info.CVERTEX.left, info.CVERTEX.top);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window size              |   W- %d  H- %d",
					"窗口大小                  |   W- %d  H- %d"), info.WINSIZE.cx, info.WINSIZE.cy);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window code              |   %s",
					"窗口编码                  |   %s"), info.CODE == WINCODE_UNICODE ? "UNICODE" : "ASCII");
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window show              |   %s",
					"窗口可见性                |   %s"), info.SHOW ? "SHOW" : "HIDE");
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window DPI               |   W- %.2f  H- %.2f",
					"窗口 DPI                  |   W- %.2f  H- %.2f"), info.DPIZOOM.fw, info.DPIZOOM.fh);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window style             |   _%08X_(hex)  Or %032s(binary)",
					"窗口风格                  |   _%08X_(hex)  Or %032s(binary)"), info.STYLE, itoa(info.STYLE, binaryStr, 2));
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window style ex          |   _%08X_(hex)  Or %032s(binary)",
					"窗口扩展风格              |   _%08X_(hex)  Or %032s(binary)"), info.STYLEEX, itoa(info.STYLEEX, binaryStr, 2));
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Window class style       |   _%08X_(hex)  Or %032s(binary)",
					"窗口类风格                |   _%08X_(hex)  Or %032s(binary)"), info.CLASSSTYLE, itoa(info.CLASSSTYLE, binaryStr, 2));
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Cursor position          |   X- %d  Y- %d",
					"游标位置                  |   X- %d  Y- %d"), info.CURPOS.x, info.CURPOS.y);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Cursor in window         |   X- %d  Y- %d",
					"游标在窗口中坐标          |   X- %d  Y- %d"), info.CURINWINPOS.x, info.CURINWINPOS.y);
				wdo_putc('\n');
				wdo_log(NULL, OUTLOG_NORMAL, wdo_language(
					"Cursor point color       |   #%06X  Or (R- %u, G- %u, B- %u)",
					"游标点颜色                |   #%06X  Or (R- %u, G- %u, B- %u)"), lr_TO_RGB(info.CURCOLOR), GetRValue(info.CURCOLOR), GetGValue(info.CURCOLOR), GetBValue(info.CURCOLOR));
				wdo_putc('\n');

			}
		}
		/* 更改指定窗口标题的文本 */
		else if (!stricmp(instruction, WDOINS_TITLE))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							if (SetWindowTextA(ctwin, StrArray[i]))
							{
								wdo_log_insec(OUTLOG_NORMAL
									, "The title of window _%X_ has been changed, TITLE = %s"
									, "窗口 _%X_ 的标题文本已经更改。TITLE = %s"
									, ctwin, StrArray[i]
								);
							}
							else wdo_log_error(lr_TO_STR(SetWindowTextA));

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				/* 支持不带参数，清空标题 */
				else
				{
					if (SetWindowTextA(ctwin, "\0"))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "The title of window _%X_ has been changed, TITLE = "
							, "窗口 _%X_ 的标题文本已经更改。TITLE = "
							, ctwin
						);
					}
					else wdo_log_error(lr_TO_STR(SetWindowTextA));
				}
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 将指定窗口成为前台窗口 */
		else if (!stricmp(instruction, WDOINS_TOP))
		{
			if (IsWindow(ctwin))
			{
				if (SetForegroundWindow(ctwin))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ top placed."
						, "窗口 _%X_ 已放置顶部。"
						, ctwin
					);
				}
				else wdo_log_error(lr_TO_STR(SetForegroundWindow));
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 将指定窗口置顶 */
		else if (!stricmp(instruction, WDOINS_TOPMOST))
		{
			if (IsWindow(ctwin))
			{
				if (parameter == NULL || parameter[0] == '\0' || !stricmp(parameter, "on"))
				{
					if (SetWindowPos(ctwin, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ is now at the topmost."
							, "窗口 _%X_ 已置顶。"
							, ctwin
						);
					}
					else wdo_log_error(lr_TO_STR(SetWindowPos));
				}
				else if (parameter && !stricmp(parameter, "off"))
				{
					if (SetWindowPos(ctwin, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ topmost canceled."
							, "窗口 _%X_ 已取消顶置。"
							, ctwin
						);
					}
					else wdo_log_error(lr_TO_STR(SetWindowPos));
				}
				else WDO_ERROR_NOSWITCH(parameter);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 将指定窗口置底 */
		else if (!stricmp(instruction, WDOINS_BOTTOM))
		{
			if (IsWindow(ctwin))
			{
				if (SetWindowPos(ctwin, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ is now at the bottom."
						, "窗口 _%X_ 已置底。"
						, ctwin
					);
				}
				else wdo_log_error(lr_TO_STR(SetWindowPos));
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 将指定窗口成为活动窗口 */
		else if (!stricmp(instruction, WDOINS_ACTIVE))
		{
			if (IsWindow(ctwin))
			{
				if (SetActiveWindow(ctwin))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ has been set as active window."
						, "窗口 _%X_ 已设置为活动窗口。"
						, ctwin
					);
				}
				else wdo_log_error(lr_TO_STR(SetActiveWindow));
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 将指定窗口成为焦点窗口 */
		else if (!stricmp(instruction, WDOINS_FOCUS))
		{
			if (IsWindow(ctwin))
			{
				if (SetFocus(ctwin))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ has been set as focus window."
						, "窗口 _%X_ 已设置为焦点窗口。"
						, ctwin
					);
				}
				else wdo_log_error(lr_TO_STR(SetFocus));
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 将指定窗口成为捕获窗口 */
		else if (!stricmp(instruction, WDOINS_CAPTURE))
		{
			if (IsWindow(ctwin))
			{
				if (SetCapture(ctwin))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ has been set as capture window."
						, "窗口 _%X_ 已设置为捕获窗口。"
						, ctwin
					);
				}
				else wdo_log_error(lr_TO_STR(SetCapture));
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置指定窗口透明度[0-100] | 数值越大越透明 */
		else if (!stricmp(instruction, WDOINS_TRANSP))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

					if (IntArray != NULL)
					{
						/* 更改透明度前，窗口必须具有 WS_EX_LAYERED 风格 */
						LONG style = GetWindowLongA(ctwin, GWL_EXSTYLE);
						SetWindowLongA(ctwin, GWL_EXSTYLE, style | WS_EX_LAYERED);

						for (size_t i = 0; i < ArraySize; ++i)
						{
							lr_LIMIT_VALSIZE(IntArray[i][0], 0, 100);

							/* 将输入的数值[0-100] 反转后转化为真正值[0-255] */
							BYTE transp = (BYTE)(
								(UCHAR_MAX - ((IntArray[i][0] * UCHAR_MAX) / 100)) < UCHAR_MAX
								? (UCHAR_MAX - ((IntArray[i][0] * UCHAR_MAX) / 100))
								: UCHAR_MAX);

							/* https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setlayeredwindowattributes */
							if (SetLayeredWindowAttributes(ctwin, 0, transp, LWA_ALPHA))
							{
								wdo_log_insec(OUTLOG_NORMAL
									, "Window _%X_ Transparency set to %u%%"
									, "窗口 _%X_ 透明度已设置为 %u%%"
									, ctwin, (BYTE)IntArray[i][0]
								);
							}
							else wdo_log_error(lr_TO_STR(SetLayeredWindowAttributes));

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&IntArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 擦除指定窗口某一颜色 | 输入六位十六进制值 */
		else if (!stricmp(instruction, WDOINS_ERASE))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						/* 更改透明度前，窗口必须具有 WS_EX_LAYERED 风格 */
						LONG style = GetWindowLongA(ctwin, GWL_EXSTYLE);
						SetWindowLongA(ctwin, GWL_EXSTYLE, style | WS_EX_LAYERED);

						for (size_t i = 0; i < ArraySize; ++i)
						{
							COLORREF color = (COLORREF)lrHexToDec(StrArray[i]);

							lr_LIMIT_VALSIZE(color, 0, 0xFFFFFF);

							color = lr_TO_RGB(color);

							if (SetLayeredWindowAttributes(ctwin, color, 0, LWA_COLORKEY))
							{
								wdo_log_insec(OUTLOG_NORMAL
									, "Window _%X_ the specified color has been erased. > R- %d, G- %d, B- %d"
									, "窗口 _%X_ 的指定颜色已被擦除。> R- %d, G- %d, B- %d"
									, ctwin
									, GetRValue(color), GetGValue(color), GetBValue(color)
								);
							}
							else wdo_log_error(lr_TO_STR(SetLayeredWindowAttributes));

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置透明度和擦除窗口颜色 */
		else if (!stricmp(instruction, WDOINS_TRANSPERASE))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						/* 更改透明度前，窗口必须具有 WS_EX_LAYERED 风格 */
						LONG style = GetWindowLongA(ctwin, GWL_EXSTYLE);
						SetWindowLongA(ctwin, GWL_EXSTYLE, style | WS_EX_LAYERED);

						for (size_t i = 0; i < ArraySize; ++i)
						{
							if (!stricmp(StrArray[i], "-cancel"))
							{
								SetWindowLongA(ctwin, GWL_EXSTYLE, style &~ WS_EX_LAYERED);
								continue;
							}
							iStringLength = strlen(StrArray[i]);

							char* transp = (char*)malloc(iStringLength);
							char* erase = (char*)malloc(iStringLength);

							if (transp != NULL && erase != NULL)
							{
								lrstrsep(StrArray[i], 2, transp, erase);

								int iTransp = abs(atoi(transp));

								lr_LIMIT_VALSIZE(iTransp, 0, 100);

								/* 将输入的数值[0-100] 反转后转化为真正值[0-255] */
								BYTE byTransp = (BYTE)(
									(UCHAR_MAX - ((iTransp * UCHAR_MAX) / 100)) < UCHAR_MAX
									? (UCHAR_MAX - ((iTransp * UCHAR_MAX) / 100))
									: UCHAR_MAX);

								COLORREF color = (COLORREF)lrHexToDec(erase);

								lr_LIMIT_VALSIZE(color, 0, 0xFFFFFF);

								color = lr_TO_RGB(color);

								if (SetLayeredWindowAttributes(ctwin, color, byTransp, LWA_COLORKEY | LWA_ALPHA))
								{
									wdo_log_insec(OUTLOG_NORMAL
										, "Window _%X_ transparency set to %u%%, and the specified color has been erased. > R- %d, G- %d, B- %d"
										, "窗口 _%X_ 透明度设置为 %u%%，的指定颜色已被擦除。> R- %d, G- %d, B- %d"
										, ctwin, byTransp
										, GetRValue(color), GetGValue(color), GetBValue(color)
									);
								}
								else wdo_log_error(lr_TO_STR(SetLayeredWindowAttributes));

							}
							else WDO_ERROR_NOMEMORY();

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 向指定窗口发送消息 */
		else if (!stricmp(instruction, WDOINS_SEND) || !stricmp(instruction, WDOINS_TOPSEND))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						if (!stricmp(instruction, WDOINS_TOPSEND))
						{
							SetForegroundWindow(ctwin);
							/* 等待对方 */
							Sleep(200);
						}

						for (size_t i = 0; i < ArraySize; ++i)
						{
							iStringLength = strlen(StrArray[i]) + 1;

							char* p_type = (char*)malloc(iStringLength);
							char* p_text = (char*)malloc(iStringLength);

							if (p_type != NULL && p_text != NULL)
							{
								lrstrsep(StrArray[i], 2, p_type, p_text);

								/* -t 指定自定义文本，-rc 指定随机字符，-i 同 -t 末尾补换行 */
								if (!stricmp(p_type, "-t") || !stricmp(p_type, "-rc") || !stricmp(p_type, "-i"))
								{
									if (!stricmp(p_type, "-t") || !stricmp(p_type, "-i"))
									{
										if (!delay.sendword) lrSendStringA(ctwin, p_text);
										else
										{
											iStringLength = strlen(p_text);

											for (size_t i = 0; i < iStringLength; ++i)
											{
												lrSendWordA(ctwin, p_text[i]);
												if (delay.sendword) Sleep(delay.sendword);
											}
											if (delay.sendline) Sleep(delay.sendline);
										}

										if (p_text[0] != '\0') wdo_log_insec(OUTLOG_NORMAL
												, "A text message has been sent to window _%X_, TEXT = %s"
												, "一条文本消息已经发送至窗口 _%X_，文本 = %s"
												, ctwin, p_text
											);
									}
									else if (!stricmp(p_type, "-rc"))
									{
										short c = lrGetRandomCharacterInString(p_text, strlen(p_text));

										char cc[3] = { (c >> 8) & 0x00ff, c & 0x00ff, '\0' };

										if ((c & 0x8080) == 0x8080)
										{
											lrSendStringA(ctwin, cc);
										}
										else
										{
											lrSendWordA(ctwin, (char)c);
											cc[0] = c & 0x00ff;
											cc[1] = '\0';
										}
										wdo_log_insec(OUTLOG_NORMAL
											, "A random character has been sent to window _%X_, Char = '%s', From text = %s"
											, "一个随机字符已经发送至窗口 _%X_，字符 = '%s'，来自文本 = %s"
											, ctwin, cc, p_text
										);

										if (delay.sendword) Sleep(delay.sendword);
									}

									/* 末尾补换行 */
									if (!stricmp(p_type, "-i"))
									{
										SendMessageA(ctwin, WM_KEYDOWN, VK_RETURN, 0L);
										SendMessageA(ctwin, WM_CHAR, '\r', 0L);
										SendMessageA(ctwin, WM_CHAR, '\n', 0L);
										SendMessageA(ctwin, WM_KEYUP, VK_RETURN, 0L);
											
										wdo_log_insec(OUTLOG_NORMAL
											, "A linebreak message has been sent to window _%X_."
											, "一个换行消息已发送至窗口 _%X_。"
											, ctwin
										);
									}
								}

								/* -f 指定文件，-rt 指定文件随机行 */
								else if (!stricmp(p_type, "-f") || !stricmp(p_type, "-rt"))
								{
									/* 提取路径，过滤双引号 */
									lrstrrem(p_text, '{', '}');
									lrstrrem(p_text, '\"', '\"');

									FILE* send_fp = fopen(p_text, "r");
									if (send_fp != NULL)
									{
										wdo_log_insec(OUTLOG_NORMAL
											, "Sending file text to window _%X_ ... > FILE = %s"
											, "正在向窗口 _%X_ 发送文本文件。。。> 文件 = %s"
											, ctwin, p_text
										);

										_fseeki64(send_fp, 0, SEEK_SET);

										/* 用于读取字符，每次读取一个 */
										char c = 0;
										char c2 = 0;
										wchar_t wc = 0;

										if (!stricmp(p_type, "-f"))
										{
											while (TRUE)
											{
												if ((c = fgetc(send_fp)) == EOF) break;
												
												if (c & 0x80)
												{
													if ((c2 = fgetc(send_fp)) == EOF)
													{
														lrSendWordA(ctwin, c);
														break;
													}
													else if (c2 & 0x80)
													{
														lrSendWordA(ctwin, (short)((((short)c & 0x00ff) << 8) | (short)c2 & 0x00ff));
													}
													else
													{
														lrSendWordA(ctwin, c);
														lrSendWordA(ctwin, c2);
													}
												}
												else lrSendWordA(ctwin, c);

												if (c == '\n') if (delay.sendline) Sleep(delay.sendline);
												else if (delay.sendword) Sleep(delay.sendword);
											}
										}
										else
										{
											char* LineText = NULL;
											iStringLength = lrGetRandomLineTextInFile(p_text, &LineText);
											if (LineText && iStringLength)
											{
												if (delay.sendword)
												{
													wchar_t* wLineText = (wchar_t*)malloc((iStringLength + 1) * sizeof(wchar_t));
													if (wLineText)
													{
														iStringLength = mbstowcs(wLineText, LineText, iStringLength + 1);
														for (size_t pos = 0; pos < iStringLength; ++pos)
														{
															lrSendWordW(ctwin, wLineText[pos]);
															Sleep(delay.sendword);
														}
														free(wLineText);
													}
													else WDO_ERROR_NOMEMORY();
												}
												else
												{
													lrSendStringA(ctwin, LineText);
													if (delay.sendline) Sleep(delay.sendline);
												}
												free(LineText);
											}
											else WDO_ERROR_NULLFILE(p_text);
										}
									}
									else if(TRUE) WDO_ERROR_NOOPENFILE(p_text);
								}

								/* -d 指定目录，-rf 指定目录中随机文件 */
								else if (!stricmp(p_type, "-d") || !stricmp(p_type, "-rf"))
								{
									/* 提取路径 */
									lrstrrem(p_text, '{', '}');
									lrstrrem(p_text, '\"', '\"');

									if (!stricmp(p_type, "-d"))
									{
										char** FileList = NULL;
										size_t FileCount = 0;
										if (lrGetFileListInDir(p_text, &FileList, &FileCount, TRUE))
										{
											for (size_t FileIndex = 0; FileIndex < FileCount; ++FileIndex)
											{
												FILE* FilePtr = fopen(FileList[FileIndex], "rb");
												if (FilePtr)
												{
													char c = 0;
													char c2 = 0;
													wchar_t wc = 0;

													wdo_log_insec(OUTLOG_NORMAL
														, "Sending file to window _%X_, FILE = %s"
														, "正在发送文件到窗口 _%X_，文件 = %s"
														, ctwin, FileList[FileIndex]);

													while (TRUE)
													{
														if ((c = fgetc(FilePtr)) == EOF) break;

														if (c & 0x80)
														{
															if ((c2 = fgetc(FilePtr)) == EOF)
															{
																lrSendWordA(ctwin, c);
																break;
															}
															else if (c2 & 0x80)
															{
																lrSendWordA(ctwin, (short)((((short)c & 0x00ff) << 8) | (short)c2 & 0x00ff));
															}
															else
															{
																lrSendWordA(ctwin, c);
																lrSendWordA(ctwin, c2);
															}
														}
														else lrSendWordA(ctwin, c);

														if (c == '\n') if (delay.sendline) Sleep(delay.sendline);
														else if (delay.sendword) Sleep(delay.sendword);
													}
													fclose(FilePtr);
													if (delay.sendfile) Sleep(delay.sendfile);
												}
												else WDO_ERROR_NOOPENFILE(FileList[FileIndex]);
											}
											lrfrees(&FileList, FileCount);
										}
									}

									else if (!stricmp(p_type, "-rf"))
									{
										char** FileList = NULL;
										size_t FileCount = 0;
										if (lrGetFileListInDir(p_text, &FileList, &FileCount, TRUE))
										{
											size_t FileIndex = rand() % FileCount;
											FILE* FilePtr = fopen(FileList[FileIndex], "rb");
											if (FilePtr)
											{
												char c = 0;
												char c2 = 0;
												wchar_t wc = 0;

												wdo_log_insec(OUTLOG_NORMAL
													, "Sending file to window _%X_, FILE = %s"
													, "正在发送文件到窗口 _%X_，文件 = %s"
													, ctwin, FileList[FileIndex]);

												while (TRUE)
												{
													if ((c = fgetc(FilePtr)) == EOF) break;

													if (c & 0x80)
													{
														if ((c2 = fgetc(FilePtr)) == EOF)
														{
															lrSendWordA(ctwin, c);
															break;
														}
														else if (c2 & 0x80)
														{
															lrSendWordA(ctwin, (short)((((short)c & 0x00ff) << 8) | (short)c2 & 0x00ff));
														}
														else
														{
															lrSendWordA(ctwin, c);
															lrSendWordA(ctwin, c2);
														}
													}
													else lrSendWordA(ctwin, c);

													if (c == '\n') if (delay.sendline) Sleep(delay.sendline);
													else if (delay.sendword) Sleep(delay.sendword);
												}
												fclose(FilePtr);
												if (delay.sendfile) Sleep(delay.sendfile);
											}
											else WDO_ERROR_NOOPENFILE(FileList[FileIndex]);
											
											lrfrees(&FileList, FileCount);
										}
									}
								}

								/* -v 发送粘贴信息 */
								else if (!stricmp(p_type, "-v"))
								{
									SendMessageA(ctwin, WM_PASTE, 0, 0);

									wdo_log_insec(OUTLOG_NORMAL
										, "A paste message has been sent to window _%X_.\n"
										, "已经向窗口 _%X_ 发送一个粘贴消息。\n"
										, ctwin
									);
								}

								else WDO_ERROR_NOTYPE(p_type);

								free(p_type); free(p_text);
							}

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 在指定窗口中输出文字 */
		else if (!stricmp(instruction, WDOINS_OUT))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							iStringLength = strlen(StrArray[i]) + 1;

							char* posx = (char*)malloc(iStringLength);
							char* posy = (char*)malloc(iStringLength);
							char* text = (char*)malloc(iStringLength);

							if (posx != NULL && posy != NULL && text != NULL)
							{
								lrstrsep(StrArray[i], 3, posx, posy, text);

								if (lrWindowPrint(ctwin, atoi(posx), atoi(posy), text, AliasBools[alias_type_out] ? &(alias.data.out) : NULL))
								{
									wdo_log_insec(OUTLOG_NORMAL
										, "Text and image have been output to the window _%X_"
										, "文本和图像已经输出到窗口 _%X_。"
										, ctwin
									);
								}
								else wdo_log_insec(OUTLOG_ERROR
									, "To the window _%X_ output text failed!"
									, "向窗口 _%X_ 输出文本失败！"
									, ctwin
								);

								free(posx);
								free(posy);
								free(text);
							}
							else WDO_ERROR_NOMEMORY();

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 在指定窗口中绘图 */
		else if (!stricmp(instruction, WDOINS_DRAW))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							iStringLength = strlen(StrArray[i]) + 1;

							char* type = (char*)malloc(iStringLength);

							char* param1 = (char*)malloc(iStringLength);
							char* param2 = (char*)malloc(iStringLength);
							char* param3 = (char*)malloc(iStringLength);
							char* param4 = (char*)malloc(iStringLength);

							if (type && param1 && param2 && param3 && param4)
							{
								lrstrsep(StrArray[i], 5, type, param1, param2, param3, param4);

								HDC hdc = GetDC(ctwin);
								HPEN hPen = NULL;
								HBRUSH hBrush = NULL;

								/* 当前绘画的别名 */
								ALIASSINGLE DrawAlias = { 0 };
								BOOL ExistDrawAlias = FALSE;

								if (AliasBools[alias_type_draw] == TRUE)
								{
									DrawAlias.data.box = alias.data.box;
									ExistDrawAlias = TRUE;
								}
								else lrGetAliasDef(alias_def_draw, &DrawAlias);

								/* 配置绘画设置 */
								if (TRUE)
								{
									hPen = CreatePen(PS_SOLID, DrawAlias.data.draw.StrokeWidth, DrawAlias.data.draw.StrokeColor);
									SelectObject(hdc, hPen);

									/* 是否填充 */
									if (DrawAlias.data.draw.Fill)
									{
										if (DrawAlias.data.draw.FillImage[0] == '\0')
											hBrush = CreateSolidBrush(DrawAlias.data.draw.FillColor);
										else
										{
											HBITMAP hBitmap = LoadImageA(NULL, DrawAlias.data.draw.FillImage, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
											hBrush = CreatePatternBrush(hBitmap);
											DeleteObject(hBitmap);
										}
										SelectObject(hdc, hBrush);
									}
									else
									{
										/* 空心笔刷 */
										hBrush = GetStockObject(NULL_BRUSH);
										SelectObject(hdc, hBrush);
									}
								}
								/* 删除别名 */
								if (ExistDrawAlias == FALSE) lrClearAlias(&DrawAlias);

								/* 矩形 */
								if (!stricmp(type, "--rect"))
								{
									if (param4[0] != '\0')
										State = Rectangle(hdc, atoi(param1), atoi(param2), atoi(param3), atoi(param4));
									else
									{
										size_t rect_array_size = 0;
										__int64** rect_array = wdo_curlybrace_splitint(param1, &rect_array_size);

										if (rect_array)
										{
											for (size_t rect_array_number = 0; rect_array_number < rect_array_size; ++rect_array_number)
											{
												State = Rectangle(hdc
													, (int)rect_array[rect_array_number][0]
													, (int)rect_array[rect_array_number][1]
													, (int)rect_array[rect_array_number][2]
													, (int)rect_array[rect_array_number][3]
												);
											}
											lrfrees(&rect_array, rect_array_size);

											State = TRUE;
										}
										else State = FALSE;
									}
								}
								/* 正圆 */
								else if (!stricmp(type, "--circle"))
								{
									if (param3[0] != '\0')
									{
										int x = atoi(param1);
										int y = atoi(param2);
										int radius = atoi(param3);

										State = Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
									}
									else
									{
										size_t circle_array_size = 0;
										__int64** circle_array = wdo_curlybrace_splitint(param1, &circle_array_size);

										if (circle_array)
										{
											for (size_t circle_array_number = 0; circle_array_number < circle_array_size; ++circle_array_number)
											{
												State = Ellipse(hdc
													, (int)(circle_array[circle_array_number][0] - circle_array[circle_array_number][2])
													, (int)(circle_array[circle_array_number][1] - circle_array[circle_array_number][2])
													, (int)(circle_array[circle_array_number][0] + circle_array[circle_array_number][2])
													, (int)(circle_array[circle_array_number][1] + circle_array[circle_array_number][2])
												);
											}
											lrfrees(&circle_array, circle_array_size);

											State = TRUE;
										}
										else State = FALSE;
									}
								}
								/* 椭圆 */
								else if (!stricmp(type, "--ellipse"))
								{
									if (param4[0] != '\0')
										State = Ellipse(hdc, atoi(param1), atoi(param2), atoi(param3), atoi(param4));
									else
									{
										size_t ellipse_array_size = 0;
										__int64** ellipse_array = wdo_curlybrace_splitint(param1, &ellipse_array_size);

										if (ellipse_array)
										{
											for (size_t ellipse_array_number = 0; ellipse_array_number < ellipse_array_size; ++ellipse_array_number)
											{
												State = Ellipse(hdc
													, (int)ellipse_array[ellipse_array_number][0]
													, (int)ellipse_array[ellipse_array_number][1]
													, (int)ellipse_array[ellipse_array_number][2]
													, (int)ellipse_array[ellipse_array_number][3]
												);
											}
											lrfrees(&ellipse_array, ellipse_array_size);

											State = TRUE;
										}
										else State = FALSE;
									}
								}
								/* 线或折线 */
								else if (!stricmp(type, "--line"))
								{
									if (param4[0] != '\0')
									{
										MoveToEx(hdc, atoi(param1), atoi(param2), NULL);
										State = LineTo(hdc, atoi(param3), atoi(param4));
									}
									else
									{
										size_t line_array_size = 0;
										__int64** line_array = wdo_curlybrace_splitint(param1, &line_array_size);

										if (line_array)
										{
											/* 设置原点 */
											MoveToEx(hdc, (int)line_array[0][0], (int)line_array[0][1], NULL);

											for (size_t line_array_number = 1; line_array_number < line_array_size; ++line_array_number)
											{
												/* 持续连线 */
												LineTo(hdc, (int)line_array[line_array_number][0], (int)line_array[line_array_number][1]);
											}
											lrfrees(&line_array, line_array_size);

											State = TRUE;
										}
										else State = FALSE;
									}
								}
								/* 贝塞尔曲线 */
								else if (!stricmp(type, "--bezier"))
								{
									size_t bezier_array_size = 0;
									__int64** bezier_array = wdo_curlybrace_splitint(param1, &bezier_array_size);

									if (bezier_array)
									{
										POINT* point = (POINT*)malloc(bezier_array_size * sizeof(POINT));
										if (point)
										{
											for (size_t bezier_array_number = 0; bezier_array_number < bezier_array_size; ++bezier_array_number)
											{
												point[bezier_array_number].x = (LONG)bezier_array[bezier_array_number][0];
												point[bezier_array_number].y = (LONG)bezier_array[bezier_array_number][1];
											}
											State = PolyBezier(hdc, point, (DWORD)bezier_array_size);

											free(point);
										}
										else State = FALSE;

										lrfrees(&bezier_array, bezier_array_size);
									}
									else State = FALSE;
								}

								else WDO_ERROR_NOTYPE(type);

								if (State == OUTLOG_NORMAL)
									wdo_log_insec(State, "Window _%X_ draw done.", "窗口 _%X_ 绘画完成。", ctwin);
								else
									wdo_log_insec(State, "Window _%X_ draw failed!", "窗口 _%X_ 绘画失败！", ctwin);

								free(type);
								free(param1);
								free(param2);
								free(param3);
								free(param4);

								ReleaseDC(ctwin, hdc);
								DeleteObject(hPen);
								DeleteObject(hBrush);
							}
							else WDO_ERROR_NOMEMORY();

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 在指定的窗口中贴图 */
		else if (!stricmp(instruction, WDOINS_IMAGE))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							iStringLength = strlen(StrArray[i] + 1);

							char* x = (char*)malloc(iStringLength);
							char* y = (char*)malloc(iStringLength);
							char* path = (char*)malloc(iStringLength);

							if (x && y && path)
							{
								lrstrsep(StrArray[i], 3, x, y, path);

								if (path[0] == '\0' && alias.data.path.Type == PATHTYPE_FILE)
								{
									char* temp = (char*)realloc(path, strlen(alias.data.path.Path) + 1);
									if (temp)
									{
										path = temp;
										strcpy(path, alias.data.path.Path);
									}
									else WDO_ERROR_NOMEMORY();
								}

								if (path[0])
								{
									int ix = atoi(x);
									int iy = atoi(y);

									lrstrrem(path, '{', '}');
									lrstrrem(path, '\"', '\"');

									HDC hdc = GetDC(ctwin);
									HBITMAP hBitmap = LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
									if (hBitmap)
									{
										HDC hdcMem = CreateCompatibleDC(hdc);
										SelectObject(hdcMem, hBitmap);

										SIZE imageSize = lrGetBitmapSize(path);
										BitBlt(hdc, ix, iy, imageSize.cx, imageSize.cy, hdcMem, 0, 0, SRCCOPY);
										DeleteObject(hBitmap);
										DeleteDC(hdcMem);

										wdo_log_insec(OUTLOG_NORMAL
											, "Window _%X_ loading a image, path = %s"
											, "窗口 _%X_ 加载了一张图片，PATH = %s"
											, ctwin, path);
									}
									else WDO_ERROR_NOSUFFIX(".bmp");

									ReleaseDC(ctwin, hdc);
								}
								else WDO_ERROR_MISSINGPARAM();

								free(x);
								free(y);
								free(path);
							}
							else WDO_ERROR_NOMEMORY();

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 在指定的窗口中绘制一个目录中的所有图片 */
		else if (!stricmp(instruction, WDOINS_IMAGES))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							iStringLength = strlen(StrArray[i] + 1);

							char* x = (char*)malloc(iStringLength);
							char* y = (char*)malloc(iStringLength);
							char* path = (char*)malloc(iStringLength);

							if (x && y && path)
							{
								lrstrsep(StrArray[i], 3, x, y, path);

								if (path[0] == '\0' && alias.data.path.Type == PATHTYPE_FILE)
								{
									path = (char*)realloc(path, strlen(alias.data.path.Path) + 1);
									if (path)
									{
										strcpy(path, alias.data.path.Path);
									}
									else WDO_ERROR_NOMEMORY();
								}

								if (path)
								{
									int ix = atoi(x);
									int iy = atoi(y);

									lrstrrem(path, '{', '}');
									lrstrrem(path, '\"', '\"');

									size_t pathlen = strlen(path) + 1;
									wchar_t* dirpath = (wchar_t*)malloc(pathlen * sizeof(wchar_t));
									mbstowcs(dirpath, path, pathlen);

									_WDIR* dir = _wopendir(dirpath);
									_wdirent* dirent = NULL;

									if (dir)
									{
										HDC hdc = GetDC(ctwin);
										HDC memhdc = CreateCompatibleDC(hdc);
										HBITMAP hBitmap;
										SIZE bitmapSize;

										getcwd(PublicString, sizeof(PublicString));

										chdir(path);

										while ((dirent = _wreaddir(dir)) != NULL)
										{
											if (!wcscmp(dirent->d_name, L".") || !wcscmp(dirent->d_name, L"..")) continue;

											hBitmap = LoadImageW(NULL, dirent->d_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
											if (hBitmap)
											{
												char* dirname = (char*)malloc(dirent->d_namlen + 1);
												if (dirname)
												{
													wcstombs(dirname, dirent->d_name, dirent->d_namlen + 1);

													SelectObject(memhdc, hBitmap);
													bitmapSize = lrGetBitmapSize(dirname);
													BitBlt(hdc, ix, iy, bitmapSize.cx, bitmapSize.cy, memhdc, 0, 0, SRCCOPY);
													DeleteObject(hBitmap);

													wdo_log_insec(OUTLOG_NORMAL
														, "Window _%X_ loading a image, name = %s"
														, "窗口 _%X_ 加载了一张图片，NAME = %s"
														, ctwin, dirname);

													free(dirname);
												}
												else WDO_ERROR_NOMEMORY();
											}
										}
										_wclosedir(dir);
										ReleaseDC(ctwin, hdc);
										DeleteDC(memhdc);
										chdir(PublicString);
									}
									else WDO_ERROR_NOOPENDIR(StrArray[i]);
								}
								else WDO_ERROR_MISSINGPARAM();

								free(x);
								free(y);
								free(path);
							}
							else WDO_ERROR_NOMEMORY();

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 投影指定窗口到指定窗口 */
		else if (!stricmp(instruction, WDOINS_PROJECT))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							iStringLength = strlen(StrArray[i]) + 1;

							char* targetWin = (char*)malloc(iStringLength);
							char* x = (char*)malloc(iStringLength);
							char* y = (char*)malloc(iStringLength);
							char* w = (char*)malloc(iStringLength);
							char* h = (char*)malloc(iStringLength);

							if (targetWin && x && y && w && h)
							{
								lrstrsep(StrArray[i], 5, targetWin, x, y, w, h);

								ALIASSINGLE targetWinAlias;
								if (lrFindAliasInAliasLibary(targetWin, &targetWinAlias))
								{
									if (targetWinAlias.type == alias_type_win && IsWindow(targetWinAlias.data.win))
									{
										HDC targetDC = GetDC(targetWinAlias.data.win);
										HDC MyDC = GetDC(ctwin);

										RECT MyRect;
										GetClientRect(ctwin, &MyRect);
										
										FSIZE fsize = lrGetWindowDpiZoom(ctwin);

										SetStretchBltMode(targetDC, HALFTONE);

										if (StretchBlt(targetDC
											, atoi(x)
											, atoi(y)
											, w[0] == '\0' ? (MyRect.right - MyRect.left) : atoi(w)
											, h[0] == '\0' ? (MyRect.bottom - MyRect.top) : atoi(h)
											, MyDC
											, MyRect.left
											, MyRect.top
											, (int)(MyRect.right * fsize.fw - MyRect.left)
											, (int)(MyRect.bottom * fsize.fh - MyRect.top)
											, SRCCOPY))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "Window _%X_ projected to window _%X_  >  X = %d, Y = %d, W = %d, H = %d."
												, "窗口 _%X_ 已投影到窗口 _%X_  >  X = %d, Y = %d, W = %d, H = %d."
												, ctwin, targetWinAlias.data.win
												, atoi(x), atoi(y)
												, w[0] == '\0' ? (MyRect.right - MyRect.left) : atoi(w)
												, h[0] == '\0' ? (MyRect.bottom - MyRect.top) : atoi(h)
											);
										}
										else wdo_log_insec(OUTLOG_ERROR
											, "Window _%X_ project to window _%X_ failed!"
											, "窗口 _%X_ 投影到窗口 _%X_ 失败！"
											, ctwin, targetWinAlias.data.win
										);

										ReleaseDC(targetWinAlias.data.win, targetDC);
										ReleaseDC(ctwin, MyDC);
									}
									else WDO_ERROR_NOWINDOW(targetWinAlias.data.win);
								}
								else WDO_ERROR_MISSINGALIAS(alias_type_win);

								free(targetWin); free(x);
								free(y); free(w); free(h);
							}
							else WDO_ERROR_NOMEMORY();

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置指定窗口的新父窗口 */
		else if (!stricmp(instruction, WDOINS_PARENT))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							ALIASSINGLE WinAlias;
							if (lrFindAliasInAliasLibary(StrArray[i], &WinAlias) && WinAlias.type == alias_type_win)
							{
								if (SetParent(ctwin, WinAlias.data.win))
								{
									wdo_log_insec(OUTLOG_NORMAL
										, "Window _%X_ become parent window of window _%X_."
										, "窗口 _%X_ 已成为窗口 _%X_ 的父窗口。"
										, WinAlias.data.win, ctwin
									);
								}
								else wdo_log_error(lr_TO_STR(SetParent));
							}
							else WDO_ERROR_MISSINGALIAS(alias_type_win);

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else
				{
					if (SetParent(ctwin, NULL))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ become parent window."
							, "窗口 _%X_ 已成为父窗口。"
							, ctwin);
					}
					else wdo_log_error(lr_TO_STR(SetParent));
				}
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置指定窗口的子窗口 */
		else if (!stricmp(instruction, WDOINS_CHILD))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							iStringLength = strlen(StrArray[i] + 1);

							char* type = (char*)malloc(iStringLength);
							char* win_alias = (char*)malloc(iStringLength);
							char* pos_x = (char*)malloc(iStringLength);
							char* pos_y = (char*)malloc(iStringLength);

							if (type && win_alias && pos_x && pos_y)
							{
								lrstrsep(StrArray[i], 4, type, win_alias, pos_x, pos_y);

								ALIASSINGLE WinAlias;

								/* 添加 */
								if (!stricmp(type, "-add"))
								{
									if (lrFindAliasInAliasLibary(win_alias, &WinAlias) && WinAlias.type == alias_type_win)
									{
										if (SetParent(WinAlias.data.win, ctwin))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "Window _%X_ added child window _%X_."
												, "窗口 _%X_ 已添加子窗口 _%X_。"
												, ctwin, WinAlias.data.win
											);
										}
										else wdo_log_error(lr_TO_STR(SetParent));

										SetWindowPos(WinAlias.data.win, NULL, atoi(pos_x), atoi(pos_y), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
									}
									else WDO_ERROR_MISSINGALIAS(alias_type_win);
								}
								/* 删除。子窗口不会关闭，只会成为父窗口 */
								else if (!stricmp(type, "-del"))
								{
									if (lrFindAliasInAliasLibary(win_alias, &WinAlias) && WinAlias.type == alias_type_win)
									{
										/* 验证是否是自身的子窗口 */
										if (GetParent(WinAlias.data.win) == ctwin)
										{
											/* 如果是，则直接设为父窗口 */
											if (SetParent(WinAlias.data.win, NULL))
											{
												wdo_log_insec(OUTLOG_NORMAL
													, "Window _%X_ deleted child window _%X_."
													, "窗口 _%X_ 已删除子窗口 _%X_。"
													, ctwin, WinAlias.data.win);
											}
											else wdo_log_error(lr_TO_STR(SetParent));
										}
										else wdo_log_insec(OUTLOG_ERROR
											, "Winodw _%X_ not have the child window _%X_ !"
											, "窗口 _%X_ 没有这个子窗口 _%X_！"
											, ctwin, WinAlias.data.win);
									}
									else WDO_ERROR_MISSINGALIAS(alias_type_win);
								}

								else WDO_ERROR_NOTYPE(type);
							}
							else WDO_ERROR_NOMEMORY();

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				/* 不带参数则清空所有子窗口 */
				else
				{
					size_t quantity = 0;
					HWND* child = lrGetChildWindowHwndSets(ctwin, &quantity);

					if (child)
					{
						for (size_t i = 0; i < quantity; ++i) SetParent(child[i], NULL);

						free(child);
					}
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ deleted all child window, total found %u child window."
						, "窗口 _%X_ 已删除所有子窗口，总共找到 %u 个子窗口。"
						, ctwin, quantity);
				}
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 复制 PTR 窗口的指定内容到指定窗口 */
		else if (!stricmp(instruction, WDOINS_COPY))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							iStringLength = strlen(StrArray[i]) + 1;

							/* 复制到窗口的窗口别名 */
							char* windowAlias = (char*)malloc(iStringLength);

							char* type = (char*)malloc(iStringLength);
							char* text = (char*)malloc(iStringLength);

							if (windowAlias && type && text)
							{
								lrstrsep(StrArray[i], 3, windowAlias, type, text);

								ALIASSINGLE wa;
								if ((lrFindAliasInAliasLibary(windowAlias, &wa) && wa.type == alias_type_win) || !stricmp(type, "-new"))
								{
									/* 复制一个新窗口 */
									if (!stricmp(type, "-new"))
									{
										if (text[0] != '\0') {
											ctwin = lrCopyWindow(ctwin, text);
											wdo_log_win_brief(ctwin, TRUE);

											if (UpdatePtrwin) ptrwin = ctwin;
										}
										/* 如果类名缺省则使用默认类名 */
										else
										{
											char WinClass[MAXLEN_WIN_CLASS];

											RealGetWindowClassA(ctwin, WinClass, MAXLEN_WIN_CLASS);
											sprintf(PublicString, "%s%s", lrCOPYWINDOW_NEWCLASSPREFIX, WinClass);

											ctwin = lrCopyWindow(ctwin, PublicString);
											if (UpdatePtrwin) ptrwin = ctwin;

											wdo_log_win_brief(ctwin, TRUE);
										}
									}
									/* 标题 */
									else if (!stricmp(type, "-title"))
									{
										char title[MAXLEN_WIN_TITLE];
										memset(title, '\0', sizeof(title));

										GetWindowTextA(ctwin, title, sizeof(title));
										SetWindowTextA(wa.data.win, title);

										wdo_log_insec(OUTLOG_NORMAL
											, "Title of the window _%X_ copied to window _%X_ > TITLE = \"%s\"."
											, "窗口 _%X_ 的标题已复制到窗口 _%X_ > TITLE = \"%s\"。"
											, ctwin, wa.data.win, title);
									}
									/* 风格 */
									else if (!stricmp(type, "-style"))
									{
										LONG style = GetWindowLongA(ctwin, GWL_STYLE);
										SetWindowLongA(wa.data.win, GWL_STYLE, style);

										wdo_log_insec(OUTLOG_NORMAL
											, "Style of the window _%X_ copied to window _%X_ > STYLE = %ld."
											, "窗口 _%X_ 的风格已复制到窗口 _%X_ > STYLE = %ld。"
											, ctwin, wa.data.win, style);
									}
									/* 显示模式 */
									else if (!stricmp(type, "-show"))
									{
										BOOL show = IsWindowVisible(ctwin);

										if (show)
											ShowWindow(wa.data.win, SW_SHOW);
										else
											ShowWindow(wa.data.win, SW_HIDE);

										wdo_log_insec(OUTLOG_NORMAL
											, "Show mode of the window _%X_ copied to window _%X_ > SHOW = %s."
											, "窗口 _%X_ 的显示模式已复制到窗口 _%X_ > SHOW = %s。"
											, ctwin, wa.data.win, show ? "on" : "off");
									}
									/* 透明度 */
									else if (!stricmp(type, "-transp"))
									{
										/* 检查被复制窗口的风格是否带有 WS_EX_LAYERED */
										if (GetWindowLongA(ctwin, GWL_EXSTYLE) & WS_EX_LAYERED)
										{
											SetWindowLongA(wa.data.win, GWL_EXSTYLE, GetWindowLongA(wa.data.win, GWL_EXSTYLE) | WS_EX_LAYERED);

											COLORREF color = 0;
											BYTE alpha = 0;
											DWORD flags = 0;

											GetLayeredWindowAttributes(ctwin, &color, &alpha, &flags);
											SetLayeredWindowAttributes(wa.data.win, color, alpha, flags);

											wdo_log_insec(OUTLOG_NORMAL
												, "Transparency of the window _%X_ copied to window _%X_ > COLOR = %X, ALPHA = %u, FLAGS = %lu."
												, "窗口 _%X_ 的透明度已复制到窗口 _%X_ > COLOR = %X, ALPHA = %u, FLAGS = %lu。"
												, ctwin, wa.data.win, lr_TO_RGB(color), alpha, flags);
										}
										else wdo_log_insec(OUTLOG_ERROR
											, "Transparency of the window _%X_ copy to window _%X_ failed! The original window _%X_ not have style %s."
											, "窗口 _%X_ 的透明度复制到窗口 _%X_ 失败！源窗口 _%X_ 不具有风格 %s。"
											, ctwin, wa.data.win, ctwin, lr_TO_STR(WS_EX_LAYERED));
									}
									/* 图像 */
									else if (!stricmp(type, "-image"))
									{
										HDC myDC = GetDC(ctwin);
										HDC yourDC = GetDC(wa.data.win);

										/* 自身窗口(被复制)，复制窗口 */
										RECT mr, cr;
										GetClientRect(ctwin, &mr);
										GetClientRect(wa.data.win, &cr);

										FSIZE dpi = lrGetWindowDpiZoom(ctwin);

										cr.right = (LONG)(cr.right * dpi.fw);
										cr.bottom = (LONG)(cr.bottom * dpi.fh);

										mr.right = (LONG)(mr.right * dpi.fw);
										mr.bottom = (LONG)(mr.bottom * dpi.fh);

										SetStretchBltMode(myDC, HALFTONE);
										StretchBlt(
											yourDC, cr.left, cr.top, cr.right - cr.left, cr.bottom - cr.top
											, myDC, mr.left, mr.top, mr.right - mr.left, mr.bottom - mr.top
											, SRCCOPY);

										ReleaseDC(ctwin, myDC);
										ReleaseDC(wa.data.win, yourDC);

										wdo_log_insec(OUTLOG_NORMAL
											, "Image of the window _%X_ copied to window _%X_."
											, "窗口 _%X_ 的图像已复制窗口 _%X_。"
											, ctwin, wa.data.win);
									}
									/* 其他 */
									else
									{
										/* 自身窗口(被复制)，复制窗口 */
										RECT mr, cr;
										GetWindowRect(ctwin, &mr);
										GetWindowRect(wa.data.win, &cr);

										State = OUTLOG_NORMAL;

										/* 宽 */
										if (!stricmp(type, "-w"))
										{
											MoveWindow(wa.data.win, cr.left, cr.top, mr.right - mr.left, cr.bottom - cr.top, TRUE);
										}
										/* 高 */
										else if (!stricmp(type, "-h"))
										{
											MoveWindow(wa.data.win, cr.left, cr.top, cr.right - cr.left, mr.bottom - mr.top, TRUE);
										}
										/* 宽高 */
										else if (!stricmp(type, "-wh"))
										{
											MoveWindow(wa.data.win, cr.left, cr.top, mr.right - mr.left, mr.bottom - mr.top, TRUE);
										}
										/* X轴坐标 */
										else if (!stricmp(type, "-x"))
										{
											MoveWindow(wa.data.win, mr.left, cr.top, cr.right - cr.left, cr.bottom - cr.top, TRUE);
										}
										/* Y轴坐标 */
										else if (!stricmp(type, "-y"))
										{
											MoveWindow(wa.data.win, cr.left, mr.top, cr.right - cr.left, cr.bottom - cr.top, TRUE);
										}
										/* XY轴坐标 */
										else if (!stricmp(type, "-xy"))
										{
											MoveWindow(wa.data.win, mr.left, mr.top, cr.right - cr.left, cr.bottom - cr.top, TRUE);
										}
										else {
											WDO_ERROR_NOTYPE(type);
											State = OUTLOG_ERROR;
										}

										if (State = OUTLOG_NORMAL)
											wdo_log_insec(OUTLOG_NORMAL
												, "Rect of window _%X_ copied to window _%X_."
												, "窗口 _%X_ 的矩形已复制到窗口 _%X_。"
												, ctwin, wa.data.win);
									}
								}
								else WDO_ERROR_MISSINGALIAS(alias_type_win);

								free(windowAlias);
								free(type);
								free(text);
							}
							else WDO_ERROR_NOMEMORY();

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				/* 默认创建一个新窗口 */
				else
				{
					char WinClass[MAXLEN_WIN_CLASS];

					RealGetWindowClassA(ctwin, WinClass, MAXLEN_WIN_CLASS);
					sprintf(PublicString, "%s%s", lrCOPYWINDOW_NEWCLASSPREFIX, WinClass);

					ctwin = lrCopyWindow(ctwin, PublicString);
					if (UpdatePtrwin) ptrwin = ctwin;

					wdo_log_win_brief(ctwin, TRUE);

					if (delay.ins) Sleep(delay.ins);
				}
			}
		}
		/* 移动指定窗口 */
		else if (!stricmp(instruction, WDOINS_MOVE))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if(lrMoveWindow(ctwin, (int)IntArray[i][0], (int)IntArray[i][1]))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Window _%X_ moved. > X- %d, Y- %d"
								, "窗口 _%X_ 已移动。> X- %d, Y- %d"
								, ctwin, (int)IntArray[i][0], (int)IntArray[i][1]);
						}
						else wdo_log_error(lr_TO_STR(lrMoveWindow));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 移动指定窗口 X 轴 */
		else if (!stricmp(instruction, WDOINS_MOVEX))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if(lrMoveWindow(ctwin, (int)IntArray[i][0], 0))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Window _%X_ X-axis moved %d pixel"
								, "窗口 _%X_ X轴已移动 %d pixel"
								, ctwin, (int)IntArray[i][0]);
						}
						else wdo_log_error(lr_TO_STR(lrMoveWindow));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 移动指定窗口 Y 轴 */
		else if (!stricmp(instruction, WDOINS_MOVEY))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (lrMoveWindow(ctwin, 0, (int)IntArray[i][0]))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Window _%X_ Y-axis moved %d pixel"
								, "窗口 _%X_ Y轴已移动 %d pixel"
								, ctwin, (int)IntArray[i][0]);
						}
						else wdo_log_error(lr_TO_STR(lrMoveWindow));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置指定窗口位置 */
		else if (!stricmp(instruction, WDOINS_POS))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (lrSetWindowPos(ctwin, (int)IntArray[i][0], (int)IntArray[i][1], GWP_LR_ORIGINALPOINT_PARENT))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Window _%X_ vertex coordinates has been changed. > X- %d, Y- %d"
								, "窗口 _%X_ 顶点坐标更改。> X- %d, Y- %d"
								, ctwin, (int)IntArray[i][0], (int)IntArray[i][1]
							);
						}
						else wdo_log_error(lr_TO_STR(lrSetWindowPos));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置指定窗口 X 轴位置 */
		else if (!stricmp(instruction, WDOINS_POSX))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (lrSetWindowPos(ctwin, (int)IntArray[i][0], 0, GWP_LR_ORIGINALPOINT_PARENT))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Window _%X_ vertex coordinates has been changed. > X- %d"
								, "窗口 _%X_ 顶点坐标更改。> X- %d"
								, ctwin, (int)IntArray[i][0]
							);
						}
						else wdo_log_error(lr_TO_STR(lrSetWindowPos));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置指定窗口 Y 轴位置 */
		else if (!stricmp(instruction, WDOINS_POSY))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (lrSetWindowPos(ctwin, 0, (int)IntArray[i][0], GWP_LR_ORIGINALPOINT_PARENT))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Window _%X_ vertex coordinates has been changed. > Y- %d"
								, "窗口 _%X_ 顶点坐标更改。> Y- %d"
								, ctwin, (int)IntArray[i][0]);
						}
						else wdo_log_error(lr_TO_STR(lrSetWindowPos));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 随机设置指定窗口坐标 */
		else if (!stricmp(instruction, WDOINS_RPOS))
		{
			if (IsWindow(ctwin))
			{
				lr_srand();

				RECT rect;
				GetWindowRect(ctwin, &rect);

				HWND Parent = GetParent(ctwin);

				RECT BackRect;
				if (Parent) GetWindowRect(Parent, &BackRect);
				else GetWindowRect(GetDesktopWindow(), &BackRect);

				int x, y;

				if (lrSetWindowPos(ctwin
					, x = rand() % ((BackRect.right - BackRect.left) - (rect.right - rect.left))
					, y = rand() % ((BackRect.bottom - BackRect.top) - (rect.bottom - rect.top))
					, GWP_LR_ORIGINALPOINT_PARENT))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ vertex coordinates has been changed. > X- %d, Y- %d"
						, "窗口 _%X_ 顶点坐标更改。> X- %d, Y- %d"
						, ctwin, x, y);
				}
				else wdo_log_error(lr_TO_STR(lrSetWindowPos));

				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 缩放指定窗口大小 */
		else if (!stricmp(instruction, WDOINS_ZOOM))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (lrZoomWindow(ctwin, (int)IntArray[i][0], (int)IntArray[i][1]))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Size of window _%X_ increased, width = %d, height = %d"
								, "窗口 _%X_ 大小已增大，宽度 = %d，高度 = %d"
								, ctwin, (int)IntArray[i][0], (int)IntArray[i][1]
							);
						}
						else wdo_log_error(lr_TO_STR(lrZoomWindow));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 缩放指定窗口宽度 */
		else if (!stricmp(instruction, WDOINS_ZOOMX))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						RECT rect;

						GetClientRect(ctwin, &rect);

						if (lrZoomWindow(ctwin, (int)IntArray[i][0], 0))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Width of window _%X_ increased %d"
								, "窗口 _%X_ 宽度已增加 %d"
								, ctwin, (int)IntArray[i][0]
							);
						}
						else wdo_log_error(lr_TO_STR(lrZoomWindow));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 缩放指定窗口高度 */
		else if (!stricmp(instruction, WDOINS_ZOOMY))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						RECT rect;

						GetClientRect(ctwin, &rect);

						if (lrZoomWindow(ctwin, 0, (int)IntArray[i][0]))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Height window _%X_ increased %d"
								, "窗口 _%X_ 高度已增加 %d"
								, ctwin, (int)IntArray[i][0]
							);
						}
						else wdo_log_error(lr_TO_STR(lrZoomWindow));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置指定窗口宽高 */
		else if (!stricmp(instruction, WDOINS_SIZE))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

					if (IntArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							RECT rect;

							GetClientRect(ctwin, &rect);

							if (SetWindowPos(ctwin, NULL, 0, 0, (int)IntArray[i][0], (int)IntArray[i][1], SWP_NOZORDER | SWP_NOMOVE))
							{
								wdo_log_insec(OUTLOG_NORMAL
									, "Size of window _%X_ set to %d of width, %d of height."
									, "窗口 _%X_ 的大小设置为宽度 %d，高度 %d"
									, ctwin, (int)IntArray[i][0]
								);
							}
							else wdo_log_error(lr_TO_STR(SetWindowPos));

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&IntArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置指定窗口宽度 */
		else if (!stricmp(instruction, WDOINS_SIZEX))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

					if (IntArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							RECT rect;

							GetClientRect(ctwin, &rect);

							if (SetWindowPos(ctwin, NULL, 0, 0, (int)IntArray[i][0], rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE))
							{
								wdo_log_insec(OUTLOG_NORMAL
									, "Width of window _%X_ set to %d."
									, "窗口 _%X_ 的宽度设置为 %d"
									, ctwin, (int)IntArray[i][0]
								);
							}
							else wdo_log_error(lr_TO_STR(SetWindowPos));

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&IntArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置指定窗口高度 */
		else if (!stricmp(instruction, WDOINS_SIZEY))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

					if (IntArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							RECT rect;

							GetClientRect(ctwin, &rect);

							if (SetWindowPos(ctwin, NULL, 0, 0, rect.right - rect.left, (int)IntArray[i][0], SWP_NOZORDER | SWP_NOMOVE))
							{
								wdo_log_insec(OUTLOG_NORMAL
									, "Height of window _%X_ set to %d."
									, "窗口 _%X_ 的宽度设置为 %d"
									, ctwin, (int)IntArray[i][0]
								);
							}
							else wdo_log_error(lr_TO_STR(SetWindowPos));

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&IntArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 显示或隐藏指定窗口 */
		else if (!stricmp(instruction, WDOINS_SHOW))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							if (!stricmp(StrArray[i], "on"))
							{
								if (ShowWindow(ctwin, SW_SHOWNORMAL))
								{
									wdo_log_insec(OUTLOG_NORMAL
										, "Window _%X_ showed."
										, "窗口 _%X_ 已显示。"
										, ctwin
									);
								}
								else wdo_log_error(lr_TO_STR(ShowWindow));
								
							}
							else if (!stricmp(StrArray[i], "off"))
							{
								if (ShowWindow(ctwin, SW_HIDE))
								{
									wdo_log_insec(OUTLOG_NORMAL
										, "Window _%X_ hid."
										, "窗口 _%X_ 已隐藏。"
										, ctwin
									);
								}
								else wdo_log_error(lr_TO_STR(ShowWindow));
							}
							else WDO_ERROR_NOSWITCH(StrArray[i]);

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 设置指定窗口风格 */
		else if (!stricmp(instruction, WDOINS_STYLE))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							iStringLength = strlen(StrArray[i]) + 1;

							char* style_type = (char*)malloc(iStringLength);
							char* style_switch = (char*)malloc(iStringLength);

							if (style_type != NULL && style_switch != NULL)
							{
								lrstrsep(StrArray[i], 2, style_type, style_switch);

								/* 标题栏 */
								if (!stricmp(style_type, "-title"))
								{
									/* 通过更改窗口风格的方式 */
									LONG Style = GetWindowLongA(ctwin, GWL_STYLE);

									if (!stricmp(style_switch, "on"))
									{
										if (SetWindowLongA(ctwin, GWL_STYLE, (Style & ~WS_POPUP & ~WS_MAXIMIZE) | WS_CAPTION | WS_THICKFRAME | WS_BORDER))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The title bar of the window _%X_ showed."
												, "窗口 _%X_ 的标题栏已显示。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else if (!stricmp(style_switch, "off"))
									{
										if (SetWindowLongA(ctwin, GWL_STYLE, (Style | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The title bar of the window _%X_ hid."
												, "窗口 _%X_ 的标题栏已隐藏。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else WDO_ERROR_NOPARAM(style_switch);
								}
								/* 菜单栏 */
								else if (!stricmp(style_type, "-menu"))
								{
									LONG Style = GetWindowLongA(ctwin, GWL_STYLE);

									if (!stricmp(style_switch, "on"))
									{
										if (SetWindowLongA(ctwin, GWL_STYLE, Style | WS_SYSMENU | WS_CAPTION))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The menu bar of the window _%X_ showed."
												, "窗口 _%X_ 的菜单栏已显示。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else if (!stricmp(style_switch, "off"))
									{
										if (SetWindowLongA(ctwin, GWL_STYLE, Style &~ WS_SYSMENU))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The menu bar of the window _%X_ hid."
												, "窗口 _%X_ 的菜单栏已隐藏。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
								}
								/* 最大化按钮 */
								else if (!stricmp(style_type, "-max"))
								{
									LONG Style = GetWindowLongA(ctwin, GWL_STYLE);

									if (!stricmp(style_switch, "on"))
									{
										if (SetWindowLongA(ctwin, GWL_STYLE, Style | WS_MAXIMIZEBOX))
										{
											wdo_log_insec(OUTLOG_NORMAL,
												, "The maximize button of the window _%X_ showed."
												, "窗口 _%X_ 的最大化按钮已显示。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else if (!stricmp(style_switch, "off"))
									{
										if(SetWindowLongA(ctwin, GWL_STYLE, Style & ~WS_MAXIMIZEBOX))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The maximize button of the window _%X_ hid."
												, "窗口 _%X_ 的最大化按钮已隐藏。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else WDO_ERROR_NOPARAM(style_switch);
								}
								/* 最小化按钮 */
								else if (!stricmp(style_type, "-min"))
								{
									LONG Style = GetWindowLongA(ctwin, GWL_STYLE);

									if (!stricmp(style_switch, "on"))
									{
										if(SetWindowLongA(ctwin, GWL_STYLE, Style | WS_MINIMIZEBOX))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The minimize button of the window _%X_ showed."
												, "窗口 _%X_ 的最小化按钮已显示。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else if (!stricmp(style_switch, "off"))
									{
										if(SetWindowLongA(ctwin, GWL_STYLE, Style & ~WS_MINIMIZEBOX))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The minimize button of the window _%X_ hid."
												, "窗口 _%X_ 的最小化按钮已隐藏。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else WDO_ERROR_NOPARAM(style_switch);
								}
								/* 大小调整边框 */
								else if (!stricmp(style_type, "-sb"))
								{
									LONG Style = GetWindowLongA(ctwin, GWL_STYLE);

									if (!stricmp(style_switch, "on"))
									{
										if (SetWindowLongA(ctwin, GWL_STYLE, Style | WS_SIZEBOX))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The size box of the window _%X_ showed."
												, "窗口 _%X_ 的大小调整边框已显示。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else if (!stricmp(style_switch, "off"))
									{
										if (SetWindowLongA(ctwin, GWL_STYLE, Style & ~WS_SIZEBOX))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The size box of the window _%X_ hid."
												, "窗口 _%X_ 的大小调整边框已隐藏。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
								}
								/* 滚动条，水平，垂直 */
								else if (!stricmp(style_type, "-r") || !stricmp(style_type, "-hr") || !stricmp(style_type, "-vr"))
								{
									LONG Style = GetWindowLongA(ctwin, GWL_STYLE);

									enum mode { r, hr, vr } mode = r;

									if (!stricmp(style_type, "-hr")) mode = hr;
									else if (!stricmp(style_type, "-vr")) mode = vr;


									if (!stricmp(style_switch, "on"))
									{
										switch (mode)
										{
										case r:
											Style = Style | WS_HSCROLL | WS_VSCROLL;
											break;
										case hr:
											Style |= WS_HSCROLL;
											break;
										case vr:
											Style |= WS_VSCROLL;
											break;
										}

										if (SetWindowLongA(ctwin, GWL_STYLE, Style))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The scroll bar of the window _%X_ showed."
												, "窗口 _%X_ 的滚动条已显示。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else if (!stricmp(style_switch, "off"))
									{
										switch (mode)
										{
										case r:
											Style = Style &~ WS_HSCROLL & ~WS_VSCROLL;
											break;
										case hr:
											Style = Style &~WS_HSCROLL;
											break;
										case vr:
											Style = Style &~WS_VSCROLL;
											break;
										}

										if (SetWindowLongA(ctwin, GWL_STYLE, Style))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The scroll bar of the window _%X_ hid."
												, "窗口 _%X_ 的滚动条已隐藏。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else WDO_ERROR_NOPARAM(style_switch);
								}
								/* 关闭按钮 */
								else if (!stricmp(style_type, "-close"))
								{
									if (!stricmp(style_switch, "on"))
									{
										EnableMenuItem(GetSystemMenu(ctwin, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);

										if(DrawMenuBar(ctwin))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The close button of the window _%X_ showed."
												, "窗口 _%X_ 的关闭按钮已显示。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(SetWindowLongA));
									}
									else if (!stricmp(style_switch, "off"))
									{
										/* 禁用此菜单项 */
										EnableMenuItem(GetSystemMenu(ctwin, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

										/* 删除菜单项 */
										// DeleteMenu(GetSystemMenu(ctwin, FALSE), SC_CLOSE, MF_BYCOMMAND);
										// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-enablemenuitem

										/* 重绘菜单区 */
										if(DrawMenuBar(ctwin))
										{
											wdo_log_insec(OUTLOG_NORMAL
												, "The close button of the window _%X_ hid."
												, "窗口 _%X_ 的关闭按钮已隐藏。"
												, ctwin
											);
										}
										else wdo_log_error(lr_TO_STR(DrawMenuBar));
									}
									else WDO_ERROR_NOPARAM(style_switch);
								}

								else WDO_ERROR_NOTYPE(style_type);
									
								free(style_type); free(style_switch);
							}
							else WDO_ERROR_NOMEMORY();

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 最大化指定窗口 */
		else if (!stricmp(instruction, WDOINS_MAX))
		{
			if (IsWindow(ctwin))
			{
				if (ShowWindow(ctwin, SW_MAXIMIZE))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ maximized."
						, "窗口 _%X_ 已最大化。"
						, ctwin
					);
				}
				else wdo_log_error(lr_TO_STR(ShowWindow));

				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 最小化指定窗口 */
		else if (!stricmp(instruction, WDOINS_MIN))
		{
			if (IsWindow(ctwin))
			{
				if (ShowWindow(ctwin, SW_SHOWMINIMIZED))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ minimized."
						, "窗口 _%X_ 已最小化。"
						, ctwin
					);
				}
				else wdo_log_error(lr_TO_STR(ShowWindow));

				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 还原指定窗口 */
		else if (!stricmp(instruction, WDOINS_RESTORE))
		{
			if (IsWindow(ctwin))
			{
				if (ShowWindow(ctwin, SW_RESTORE))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ has been restore show."
						, "窗口 _%X_ 已还原显示。"
						, ctwin
					);
				}
				else wdo_log_error(lr_TO_STR(ShowWindow));

				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 重绘指定窗口 */
		else if (!stricmp(instruction, WDOINS_REPAINT))
		{
			if (IsWindow(ctwin))
			{
				/* 直接将窗口变为无效区，Windows 将自动发送 WM_PAINT 消息 */
				if (InvalidateRect(ctwin, NULL, FALSE))
				{
					if (UpdateWindow(ctwin))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ repainted."
							, "窗口 _%X_ 已重绘。"
							, ctwin
						);
					}
					else wdo_log_error(lr_TO_STR(UpdateWindow));
				}
				else wdo_log_error(lr_TO_STR(InvalidateRect));

				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 在任务栏闪烁指定窗口 */
		else if (!stricmp(instruction, WDOINS_FLASH))
		{
			if (IsWindow(ctwin))
			{
				if (FlashWindow(ctwin, TRUE))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ flashing..."
						, "窗口 _%X_ 闪烁中。。。"
						, ctwin);
				}
				else wdo_log_error(lr_TO_STR(FlashWindow));

				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 描边 */
		else if (!stricmp(instruction, WDOINS_STROKE))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						iStringLength = strlen(StrArray[i]) + 1;

						char* str_color = (char*)malloc(iStringLength);
						char* str_width = (char*)malloc(iStringLength);

						if (str_color && str_width)
						{
							lrstrsep(StrArray[i], 2, str_color, str_width);

							COLORREF color = (COLORREF)lrHexToDec(str_color);
							int width = abs(atoi(str_width));

							HDC hdc = GetDC(ctwin);
							HPEN hPen = CreatePen(PS_SOLID, width ? width : 1, lr_TO_RGB(color));
							SelectObject(hdc, hPen);

							HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 0, 0);
							SelectObject(hdc, hBitmap);

							RECT rect;
							GetClientRect(ctwin, &rect);

							FSIZE fSize = lrGetWindowDpiZoom(ctwin);
							if (ctwin != wdo_gethwnd(WDOGETHWND_CONTROL))
							{
								rect.left = (long)((float)rect.left * fSize.fw);
								rect.right = (long)((float)rect.right * fSize.fw);
								rect.top = (long)((float)rect.top * fSize.fh);
								rect.bottom = (long)((float)rect.bottom * fSize.fh);
							}

							MoveToEx(hdc, rect.left, rect.top, NULL);
							LineTo(hdc, rect.right, rect.top);
							LineTo(hdc, rect.right, rect.bottom);
							LineTo(hdc, rect.left, rect.bottom);
							LineTo(hdc, rect.left, rect.top);

							ReleaseDC(ctwin, hdc);
							DeleteObject(hPen);
							DeleteObject(hBitmap);

							wdo_log_insec(OUTLOG_NORMAL
								, "Window _%X_ stroke complete, use color #%06X."
								, "窗口 _%X_ 描边完成，使用颜色 #%06X."
								, ctwin, color);
						}
						else WDO_ERROR_NOMEMORY();

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NOMEMORY();
			}
			else if (AliasBools[alias_type_draw] == TRUE)
			{
				HDC hdc = GetDC(ctwin);
				HPEN hPen = CreatePen(PS_SOLID, alias.data.draw.StrokeWidth, alias.data.draw.StrokeColor);
				SelectObject(hdc, hPen);

				RECT rect;
				GetClientRect(ctwin, &rect);

				FSIZE fSize = lrGetWindowDpiZoom(ctwin);
				if (ctwin != wdo_gethwnd(WDOGETHWND_CONTROL))
				{
					rect.left = (long)((float)rect.left * fSize.fw);
					rect.right = (long)((float)rect.right * fSize.fw);
					rect.top = (long)((float)rect.top * fSize.fh);
					rect.bottom = (long)((float)rect.bottom * fSize.fh);
				}

				MoveToEx(hdc, rect.left, rect.top, NULL);
				LineTo(hdc, rect.right, rect.top);
				LineTo(hdc, rect.right, rect.bottom);
				LineTo(hdc, rect.left, rect.bottom);
				LineTo(hdc, rect.left, rect.top);

				ReleaseDC(ctwin, hdc);
				DeleteObject(hPen);

				wdo_log_insec(OUTLOG_NORMAL
					, "Window _%X_ stroke complete, use color #%06X."
					, "窗口 _%X_ 描边完成，使用颜色 #%06X."
					, ctwin, lr_TO_RGB(alias.data.draw.StrokeColor));

				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 填充 */
		else if (!stricmp(instruction, WDOINS_FILL))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						COLORREF color = (COLORREF)lrHexToDec(StrArray[i]);

						HDC hdc = GetDC(ctwin);
						HBRUSH hBrush = CreateSolidBrush(lr_TO_RGB(color));
						SelectObject(hdc, hBrush);

						/* 兼容子窗口和 DPI 缩放 */
						RECT rect;
						GetClientRect(ctwin, &rect);

						FSIZE fSize = lrGetWindowDpiZoom(ctwin);
						if (ctwin != wdo_gethwnd(WDOGETHWND_CONTROL))
						{
							rect.left = (long)((float)rect.left * fSize.fw);
							rect.right = (long)((float)rect.right * fSize.fw);
							rect.top = (long)((float)rect.top * fSize.fh);
							rect.bottom = (long)((float)rect.bottom * fSize.fh);
						}

						PatBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);

						ReleaseDC(ctwin, hdc);
						DeleteObject(hBrush);

						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ fill complete, use color #%06X."
							, "窗口 _%X_ 填充完成，使用颜色 #%06X."
							, ctwin, color);

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NOMEMORY();
			}
			else if (AliasBools[alias_type_draw] == TRUE)
			{
				COLORREF color = alias.data.draw.FillColor;

				HDC hdc = GetDC(ctwin);
				HBRUSH hBrush = CreateSolidBrush(color);
				SelectObject(hdc, hBrush);

				RECT rect;
				GetClientRect(ctwin, &rect);

				FSIZE fSize = lrGetWindowDpiZoom(ctwin);
				if (ctwin != wdo_gethwnd(WDOGETHWND_CONTROL))
				{
					rect.left = (long)((float)rect.left * fSize.fw);
					rect.right = (long)((float)rect.right * fSize.fw);
					rect.top = (long)((float)rect.top * fSize.fh);
					rect.bottom = (long)((float)rect.bottom * fSize.fh);
				}

				PatBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);

				ReleaseDC(ctwin, hdc);
				DeleteObject(hBrush);

				wdo_log_insec(OUTLOG_NORMAL
					, "Window _%X_ fill complete, use color #%06X."
					, "窗口 _%X_ 填充完成，使用颜色 #%06X."
					, ctwin, lr_TO_RGB(color));

				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 使指定窗口进行漂移运动 */
		else if (!stricmp(instruction, WDOINS_DRIFT))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					if (IntArray[0][2] == 0) IntArray[0][2] = 100;
					else lr_LIMIT_VALSIZE(IntArray[0][2], 1, 100);

					if (ArraySize == 1)
					{
						if (lrWindowDrift(ctwin, (int)IntArray[0][0], (int)IntArray[0][1], (BYTE)IntArray[0][2]))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Window _%X_ has begun to drift. > X- %d, Y- %d, speed = %u"
								, "窗口 _%X_ 开始漂移。 > X- %d, Y- %d, speed = %u"
								, ctwin, (int)IntArray[0][0], (int)IntArray[0][1], (BYTE)IntArray[0][2]
							);
						}
						else wdo_log_insec(OUTLOG_ERROR
							, "Window _%X_  drift failed! > X- %d, Y- %d, speed = %d"
							, "窗口 _%X_ 漂移失败！> X- %d, Y- %d, speed = %d"
							, ctwin, (int)IntArray[0][0], (int)IntArray[0][1], (BYTE)IntArray[0][2]
						);
						if (delay.ins) Sleep(delay.ins);
					}
					else
					{
						int* x = (int*)malloc(ArraySize * sizeof(int));
						int* y = (int*)malloc(ArraySize * sizeof(int));
						BYTE* speed = (BYTE*)malloc(ArraySize * sizeof(BYTE));

						for (size_t i = 0; i < ArraySize; ++i)
						{
							x[i] = (int)IntArray[i][0];
							y[i] = (int)IntArray[i][1];
							speed[i] = (BYTE)IntArray[i][2];
						}

						if (lrWindowDrifts(ctwin, ArraySize, x, y, speed))
						{
							for (size_t i = 0; i < ArraySize; ++i) {
								if (TRUE)
								{
									if (IntArray[i][2] == 0) IntArray[i][2] = 100;
									else lr_LIMIT_VALSIZE(IntArray[i][2], 1, 100);

									wdo_log_insec(OUTLOG_NORMAL
										, "Window _%X_ has begun to drift. > X- %d, Y- %d, speed = %u"
										, "窗口 _%X_ 开始漂移。 > X- %d, Y- %d, speed = %u"
										, ctwin, (int)IntArray[0][0], (int)IntArray[0][1], (BYTE)IntArray[0][2]
									);
									if (delay.ins) Sleep(delay.ins);
								}
							}
						}
						else if (TRUE)
						{
							for (size_t i = 0; i < ArraySize; ++i) {
								if (IntArray[i][2] == 0) IntArray[i][2] = 100;
								else lr_LIMIT_VALSIZE(IntArray[i][2], 1, 100);

								wdo_log_insec(OUTLOG_ERROR
									, "Window _%X_  drift failed! > X- %d, Y- %d, speed = %u"
									, "窗口 _%X_ 漂移失败！> X- %d, Y- %d, speed = %u"
									, ctwin, (int)IntArray[0][0], (int)IntArray[0][1], (BYTE)IntArray[0][2]
								);
								if (delay.ins) Sleep(delay.ins);
							}
						}
						free(x);
						free(y);
						free(speed);
					}

					if (!TRUE) if (delay.ins) Sleep((DWORD)ArraySize * delay.ins);

					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 使指定窗口跟随游标移动 */
		else if (!stricmp(instruction, WDOINS_FOLLOW))
		{
			if (IsWindow(ctwin))
			{
				/* 关闭跟随 */
				if (!stricmp(parameter, "-c") || !stricmp(parameter, "{-c}"))
				{
					if (lrWindowFollowCursor(ctwin, 0, 0, FALSE))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ follow canceled."
							, "窗口 _%X_ 已取消跟随。"
							, ctwin
						);
					}
					else wdo_log_insec(OUTLOG_ERROR
						, "The current window does not follow the cursor, cannot be cancelled."
						, "当前窗口没有跟随着游标移动，无法取消。"
						, NULL
					);

					if (delay.ins) Sleep(delay.ins);
				}
				else
				{
					IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

					if (IntArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							if (lrWindowFollowCursor(ctwin, (int)IntArray[i][0], (int)IntArray[i][0], TRUE))
							{
								wdo_log_insec(OUTLOG_NORMAL
									, "Window _%X_ has been start following cursor moved, offset = X- %d, Y- %d"
									, "窗口 _%X_ 已经开始跟随游标移动，偏移量 = X- %d, Y- %d"
									, ctwin, (int)IntArray[i][0], (int)IntArray[i][1]
								);
							}
							else wdo_log_insec(OUTLOG_ERROR
									, "Window _%X_ follow cursor failed!"
									, "窗口 _%X_ 跟随游标失败！"
									, ctwin
								);

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&IntArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 使指定窗口开始摆动 */
		else if (!stricmp(instruction, WDOINS_WIGGLE))
		{
			if (IsWindow(ctwin))
			{
				/* 关闭摆动 */
				if (!stricmp(parameter, "-c") || !stricmp(parameter, "{-c}"))
				{
					if (lrWindowWiggle(ctwin, 0, 0, FALSE))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ wiggle canceled."
							, "窗口 _%X_ 已取消摆动。"
							, ctwin
						);
					}
					else wdo_log_insec(OUTLOG_ERROR
						, "The current window is not in wiggle state, cannot be cancelled."
						, "当前窗口没有处于摆动状态，无法取消。"
						, NULL
					);

					if (delay.ins) Sleep(delay.ins);
				}
				else
				{
					IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

					if (IntArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							if (lrWindowWiggle(ctwin, (int)IntArray[i][0], (int)IntArray[i][1], TRUE))
							{
								wdo_log_insec(OUTLOG_NORMAL
									, "Window _%X_ has been start wiggling, range = %d, frequency = %d."
									, "窗口 _%X_ 已经开始摆动，幅度 = %d，频率 = %d。"
									, ctwin, (int)IntArray[i][0], (int)IntArray[i][1]
								);
									
							}
							else wdo_log_insec(OUTLOG_ERROR
									, "Window _%X_ wiggle failed!"
									, "窗口 _%X_ 摆动失败！"
									, ctwin
								);

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&IntArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 使指定窗口靠边收入或弹出 */
		else if (!stricmp(instruction, WDOINS_ASIDE))
		{
			if (IsWindow(ctwin))
			{
				/* 关闭靠边 */
				if (!stricmp(parameter, "-c") || !stricmp(parameter, "{-c}"))
				{
					if (lrWindowAside(ctwin, 0, 0, 0, FALSE))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ aside canceled."
							, "窗口 _%X_ 已取消靠边。"
							, ctwin
						);
					}
					else wdo_log_insec(OUTLOG_ERROR
						, "The current window is not in aside state, cannot be cancelled."
						, "当前窗口没有处于靠边状态，无法取消。"
						, NULL
					);

					if (delay.ins) Sleep(delay.ins);
				}
				else
				{
					IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

					if (IntArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							lr_LIMIT_VALSIZE((BYTE)IntArray[i][0], 1, 10);

							if (lrWindowAside(ctwin, (BYTE)IntArray[i][0], (UINT)IntArray[i][1], (UINT)IntArray[i][2], TRUE))
							{
								wdo_log_insec(OUTLOG_NORMAL
									, "Window _%X_ has been start aside, speed = %u, retainX = %u, retainY = %u"
									, "窗口 _%X_ 已经开始靠边收入或弹出，速度 = %u，保留X = %u，保留Y = %u"
									, ctwin, (BYTE)IntArray[i][0], (UINT)IntArray[i][1], (UINT)IntArray[i][2]
								);

							}
							else wdo_log_insec(OUTLOG_ERROR
									, "Window _%X_ aside failed!"
									, "窗口 _%X_ 靠边失败！"
									, ctwin
								);

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&IntArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 游标推动窗口 */
		else if (!stricmp(instruction, WDOINS_PUSH))
		{
			if (IsWindow(ctwin))
			{
				/* 关闭 */
				if (!stricmp(parameter, "-c") || !stricmp(parameter, "{-c}"))
				{
					if (lrWindowAndCursorMoveAfterCollide(ctwin, WINDOWMOTION_WINDOWANDCURSORCOLLIDE_PUSHMOVE, 0, TRUE, 0, FALSE))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ pushed by cursor canceled."
							, "窗口 _%X_ 已取消被游标推动。"
							, ctwin
						);
					}
					else wdo_log_insec(OUTLOG_ERROR
						, "The current window is not being pushed by the cursor, cannot be cancelled."
						, "当前窗口没有处于被游标推动状态，无法取消。"
						, NULL
					);

					if (delay.ins) Sleep(delay.ins);
				}
				else
				{
					if (lrWindowAndCursorMoveAfterCollide(ctwin, WINDOWMOTION_WINDOWANDCURSORCOLLIDE_PUSHMOVE, 0, TRUE, 0, TRUE))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ be pushed by the cursor."
							, "窗口 _%X_ 已经开始被游标推动。"
							, ctwin
						);

					}
					else wdo_log_insec(OUTLOG_ERROR
							, "Window _%X_ failed to be pushed by the cursor!"
							, "窗口 _%X_ 被游标推动失败！"
							, ctwin
						);

					if (delay.ins) Sleep(delay.ins);
				}
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 指定窗口和游标碰撞时发生事件 */
		else if (!stricmp(instruction, WDOINS_COLLIDE))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							size_t len = strlen(parameter) + 1;

							/* 类型 */
							char* CollideType = (char*)malloc(len);
							/* 击退距离 */
							char* CollideDistance = (char*)malloc(len);
							/* 回弹距离 */
							char* CollideSpringBackPixel = (char*)malloc(len);

							if (CollideType && CollideDistance && CollideSpringBackPixel)
							{
								lrstrsep(StrArray[i], 3, CollideType, CollideDistance, CollideSpringBackPixel);

								/* 游标击退窗口 */
								if (!stricmp(CollideType, "-MW"))
								{
									if (lrWindowAndCursorMoveAfterCollide(ctwin
										, WINDOWMOTION_WINDOWANDCURSORCOLLIDE_MOVEWINDOW
										, abs(atoi(CollideDistance))
										, TRUE
										, abs(atoi(CollideSpringBackPixel))
										, TRUE))
									{
										wdo_log_insec(OUTLOG_NORMAL
											, "Window _%X_ start to be repel by the window, distance = %u, springback pixel = %u."
											, "窗口 _%X_ 开始被游标击退，击退距离 = %u，弹回距离 = %u."
											, ctwin, abs(atoi(CollideDistance)), abs(atoi(CollideSpringBackPixel)));
									}
									else wdo_log_insec(OUTLOG_ERROR
										, "Window _%X_ unable to be repel by the cursor!"
										, "窗口 _%X_ 无法被游标击退！"
										, ctwin);
								}
								/* 窗口击退游标 */
								else if (!stricmp(CollideType, "-MC"))
								{
									if (lrWindowAndCursorMoveAfterCollide(ctwin
										, WINDOWMOTION_WINDOWANDCURSORCOLLIDE_MOVECURSOR
										, abs(atoi(CollideDistance))
										, TRUE
										, abs(atoi(CollideSpringBackPixel))
										, TRUE))
									{
										wdo_log_insec(OUTLOG_NORMAL
											, "Window _%X_ start to repel the cursor, distance = %u, springback pixel = %u."
											, "窗口 _%X_ 开始击退游标，击退距离 = %u，弹回距离 = %u."
											, ctwin, abs(atoi(CollideDistance)), abs(atoi(CollideSpringBackPixel)));
									}
									else wdo_log_insec(OUTLOG_ERROR
										, "Window _%X_ unable to repel the cursor!"
										, "窗口 _%X_ 无法击退游标！"
										, ctwin);
								}
								/* 窗口和游标反向移动 */
								else if (!stricmp(CollideType, "-RM"))
								{
									if (lrWindowAndCursorMoveAfterCollide(ctwin
										, WINDOWMOTION_WINDOWANDCURSORCOLLIDE_REVERSEMOVE
										, abs(atoi(CollideDistance))
										, TRUE
										, abs(atoi(CollideSpringBackPixel))
										, TRUE))
									{
										wdo_log_insec(OUTLOG_NORMAL
											, "Window _%X_ start mutually exclusive with cursor, distance = %u, springback pixel = %u."
											, "窗口 _%X_ 开始与游标互斥，击退距离 = %u，弹回距离 = %u."
											, ctwin, abs(atoi(CollideDistance)), abs(atoi(CollideSpringBackPixel)));
									}
									else wdo_log_insec(OUTLOG_ERROR
										, "Window _%X_ cannot mutually exclusive with cursor!"
										, "窗口 _%X_ 无法与游标互斥！"
										, ctwin);
								}
								/* 关闭 */
								else if (!stricmp(CollideType, "-C"))
								{
									if (lrWindowAndCursorMoveAfterCollide(ctwin, 0, 0, 0, 0, FALSE))
									{
										wdo_log_insec(OUTLOG_NORMAL
											, "Window _%X_ the collision event between the cursor has been canceled."
											, "窗口 _%X_ 已经取消与游标的碰撞事件。"
											, ctwin, abs(atoi(CollideDistance)), abs(atoi(CollideSpringBackPixel)));
									}
									else wdo_log_insec(OUTLOG_ERROR
										, "Window _%X_ failed to cancel collision event with cursor!"
										, "窗口 _%X_ 与游标的碰撞事件取消失败！"
										, ctwin);
								}

								else WDO_ERROR_NOTYPE(CollideType);
							}
							else WDO_ERROR_NOMEMORY();

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 锁定指定窗口 */
		else if (!stricmp(instruction, WDOINS_LOCKPAINT))
		{
			if (IsWindow(ctwin))
			{
				if (LockWindowUpdate(ctwin))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Window _%X_ locked."
						, "窗口 _%X_ 已锁定。"
						, ctwin
					);
				}
				else wdo_log_error(lr_TO_STR(LockWindowUpdate));

				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 关闭指定的窗口或窗口进程 */
		else if (!stricmp(instruction, WDOINS_CLOSE))
		{
			if (IsWindow(ctwin))
			{
				if (parameter != NULL && parameter[0] != '\0')
				{
					StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

					if (StrArray != NULL)
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							/* 直接关闭窗口所属进程 */
							if (!stricmp(StrArray[i], "-c"))
							{
								DWORD pid;
								GetWindowThreadProcessId(ctwin, &pid);

								HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

								if (TerminateProcess(hProcess, 0))
								{
									wdo_log_insec(OUTLOG_NORMAL
										, "From process #%d of window _%X_ has closed windows and processes. > PID = %d"
										, "来自进程 #%d 的窗口 _%X_ 已关闭窗口和进程。> PID = %d"
										, pid, ctwin, pid
									);
								}
								else
								{
									wdo_log_insec(OUTLOG_ERROR
										, "Window _%X_ and process #%5d close failed!\n"
										, "窗口 _%X_ 和进程 #%5d 关闭失败！\n"
										, ctwin, pid
									);

									wdo_log_error(lr_TO_STR(TerminateProcess));
								}
								CloseHandle(hProcess);
							}
							/* 直接销毁窗口 */
							else if (!stricmp(StrArray[i], "-d"))
							{
								if (DestroyWindow(ctwin))
								{
									wdo_log_insec(OUTLOG_ERROR
										, "Window _%X_ closed."
										, "窗口 _%X_ 已关闭。"
										, ctwin
									);
								}
								else wdo_log_error(lr_TO_STR(DestroyWindow));
							}
							else WDO_ERROR_NOTYPE(StrArray[i]);

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&StrArray, ArraySize);
					}
					else WDO_ERROR_NULLPARAM(instruction);
				}
				/* 如果不携带参数则向指定窗口发送关闭请求 */
				else
				{
					if (!SendMessageA(ctwin, WM_CLOSE, 0, 0))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Window _%X_ closed."
							, "窗口 _%X_ 已关闭。"
							, ctwin
						);
					}
					else wdo_log_insec(OUTLOG_ERROR,
							, "Window _%X_ close failed!"
							, "窗口 _%X_ 关闭失败!"
							, ctwin
						);
					if (delay.ins) Sleep(delay.ins);
				}
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}


		/***************************************************************************************
		* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 游标类 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
		****************************************************************************************/

		/* 移动游标 */
		else if (!stricmp(instruction, WDOINS_CUR_MOVE))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						POINT curpos;

						GetCursorPos(&curpos);

						State = SetCursorPos(curpos.x + (int)IntArray[i][0], curpos.y + (int)IntArray[i][1]);

						if (State) wdo_log_insec(OUTLOG_NORMAL
							, "Cursor has been moved. > X- %d, Y- %d"
							, "游标已移动。> X- %d, Y- %d"
							, (int)IntArray[i][0], (int)IntArray[i][1]
						);
						else wdo_log_error(lr_TO_STR(SetCursorPos));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			/* 如果不带参数则显示当前游标坐标 */
			else if (TRUE)
			{
				POINT curpos;
				GetCursorPos(&curpos);

				wdo_log_insec(OUTLOG_NORMAL
					, "Current cursor coordinates: X- %d, Y- %d"
					, "当前游标坐标：X- %d, Y- %d"
					, curpos.x, curpos.y
				);

				if (delay.ins) Sleep(delay.ins);
			}
		}
		/* 移动游标 XY 轴坐标 */
		else if (!stricmp(instruction, WDOINS_CUR_MOVEX) || !stricmp(instruction, WDOINS_CUR_MOVEY))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						POINT curpos;

						GetCursorPos(&curpos);

						if (!stricmp(instruction, WDOINS_CUR_MOVEX))
							State = SetCursorPos(curpos.x + (int)IntArray[i][0], curpos.y);
						else
							State = SetCursorPos(curpos.x, curpos.y + (int)IntArray[i][0]);


						if (State) wdo_log_insec(OUTLOG_NORMAL
							, "Cursor has been moved. > X- %d, Y- %d"
							, "游标已移动。> X- %d, Y- %d"
							, !stricmp(instruction, WDOINS_CUR_MOVEX) ? (int)IntArray[i][0] : curpos.x
							, !stricmp(instruction, WDOINS_CUR_MOVEY) ? (int)IntArray[i][0] : curpos.y
						);
						else wdo_log_error(lr_TO_STR(SetCursorPos));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NOMEMORY();
			}
			/* 如果不带参数则显示当前游标坐标 */
			else if (TRUE)
			{
				POINT curpos;
				GetCursorPos(&curpos);

				wdo_log_insec(OUTLOG_NORMAL
					, "Current cursor coordinates: X- %d, Y- %d"
					, "当前游标坐标：X- %d, Y- %d"
					, curpos.x, curpos.y
				);
				if (delay.ins) Sleep(delay.ins);
			}
		}
		/* 设置游标坐标 */
		else if (!stricmp(instruction, WDOINS_CUR_POS))
		{
			IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

			if (IntArray != NULL)
			{
				for (size_t i = 0; i < ArraySize; ++i)
				{
					if (SetCursorPos((int)IntArray[i][0], (int)IntArray[i][1]))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "Cursor coordinates has been changed. > X- %d, Y- %d"
							, "游标坐标已更改。> X- %d | Y- %d"
							, (int)IntArray[i][0], (int)IntArray[i][1]
						);
					}
					else wdo_log_error(lr_TO_STR(SetCursorPos));

					if (delay.ins) Sleep(delay.ins);
				}
				lrfrees(&IntArray, ArraySize);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 设置游标 XY 轴坐标 */
		else if (!stricmp(instruction, WDOINS_CUR_POSX) || !stricmp(instruction, WDOINS_CUR_POSY))
		{
			IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

			if (IntArray != NULL)
			{
				for (size_t i = 0; i < ArraySize; ++i)
				{
					POINT curpos;

					GetCursorPos(&curpos);

					if (!stricmp(instruction, WDOINS_CUR_POSX))
						State = SetCursorPos((int)IntArray[i][0], curpos.y);
					else
						State = SetCursorPos(curpos.x, (int)IntArray[i][0]);


						if (State) wdo_log_insec(OUTLOG_NORMAL
							, "Cursor coordinates has been changed, X- %d, Y- %d"
							, "游标坐标已更改，X- %d, Y- %d"
							, !stricmp(instruction, WDOINS_CUR_POSX) ? (int)IntArray[i][0] : curpos.x
							, !stricmp(instruction, WDOINS_CUR_POSY) ? (int)IntArray[i][0] : curpos.y
						);
						else wdo_log_error(lr_TO_STR(SetCursorPos));

					if (delay.ins) Sleep(delay.ins);
				}
				lrfrees(&IntArray, ArraySize);
			}
			else WDO_ERROR_NOMEMORY();
		}
		/* 随机设置游标坐标 */
		else if (!stricmp(instruction, WDOINS_CUR_RPOS))
		{
			State = SetCursorPos(rand() % GetSystemMetrics(SM_CXSCREEN), rand() % GetSystemMetrics(SM_CYSCREEN));

			{
				POINT newCurpos;
				GetCursorPos(&newCurpos);

				wdo_log_insec(OUTLOG_NORMAL
					, "Cursor coordinates changed. > X- %d, Y- %d"
					, "游标坐标已更改。> X- %d | Y- %d"
					, newCurpos.x, newCurpos.y
				);
			}
			if (delay.ins) Sleep(delay.ins);
		}
		/* 以指定窗口为原点，设置游标坐标 */
		else if (!stricmp(instruction, WDOINS_CUR_CPOS))
		{
			if (IsWindow(ctwin))
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						RECT rect;
						GetWindowRect(ctwin, &rect);

						if (SetCursorPos((int)IntArray[i][0] - rect.left, (int)IntArray[i][1] - rect.top))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Cursor coordinates has been changed. > X- %d, Y- %d"
								, "游标坐标已更改。> X- %d | Y- %d"
								, (int)IntArray[i][0], (int)IntArray[i][1]
							);
						}
						else wdo_log_error(lr_TO_STR(SetCursorPos));

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NOMEMORY();
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 以指定窗口为区域，随机设置游标坐标 */
		else if (!stricmp(instruction, WDOINS_CUR_CRPOS))
		{
			if (IsWindow(ctwin))
			{
				RECT rect;
				GetWindowRect(ctwin, &rect);

				State = SetCursorPos(rand() % (rect.right - rect.left) + rect.left, rand() % (rect.bottom - rect.top) + rect.top);

				{
					POINT newCurpos;
					GetCursorPos(&newCurpos);

					wdo_log_insec(OUTLOG_NORMAL
						, "Cursor coordinates has been changed. > X- %d, Y- %d"
						, "游标坐标已更改。> X- %d | Y- %d"
						, newCurpos.x, newCurpos.y
					);
				}
				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NOWINDOW(ctwin);
		}
		/* 模拟按下游标键 */
		else if (!stricmp(instruction, WDOINS_CUR_KEY))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						POINT curpos;
						GetCursorPos(&curpos);

						/* 当前要发送消息的窗口 */
						HWND target = WindowFromPoint(curpos);

						/* 用于获取窗口顶点，求出游标在窗口客户区中的坐标 */
						RECT rect;
						GetWindowRect(target, &rect);

						/* 获取游标在客户区中的坐标 */
						curpos.x -= rect.left;
						curpos.y -= rect.top;

						/* 将游标坐标合成到 cCurpos 中，高字节放 Y，低字节放 X */
						LPARAM cCurpos = ((LPARAM)curpos.y << 16) | ((LPARAM)curpos.x & 0xffff);

						/* 需要判断是否具有 CS_DBLCLKS 风格 */
						LONG Style = GetClassLongA(target, GCL_STYLE);

						/* 左键 */
						if (!stricmp(StrArray[i], "--left"))
						{
							SendMessageA(target, WM_LBUTTONDOWN, 0, cCurpos);
							SendMessageA(target, WM_LBUTTONUP, 0, cCurpos);
						}
						/* 右键 */
						else if (!stricmp(StrArray[i], "--right"))
						{
							SendMessageA(target, WM_RBUTTONDOWN, 0, cCurpos);
							SendMessageA(target, WM_RBUTTONUP, 0, cCurpos);
						}
						/* 中键 */
						else if (!stricmp(StrArray[i], "--middle"))
						{
							SendMessageA(target, WM_MBUTTONDOWN, 0, cCurpos);
							SendMessageA(target, WM_MBUTTONUP, 0, cCurpos);
						}
						/* 左双击 */
						else if (!stricmp(StrArray[i], "--left_twice"))
						{
							SendMessageA(target, WM_LBUTTONDOWN, 0, cCurpos);
							SendMessageA(target, WM_LBUTTONUP, 0, cCurpos);

							if (Style & CS_DBLCLKS)
								SendMessageA(target, WM_LBUTTONDBLCLK, 0, cCurpos);
							else
								SendMessageA(target, WM_LBUTTONDOWN, 0, cCurpos);

							SendMessageA(target, WM_LBUTTONUP, 0, cCurpos);
						}
						/* 右双击 */
						else if (!stricmp(StrArray[i], "--right_twice"))
						{
							SendMessageA(target, WM_RBUTTONDOWN, 0, cCurpos);
							SendMessageA(target, WM_RBUTTONUP, 0, cCurpos);

							if (Style & CS_DBLCLKS)
								SendMessageA(target, WM_RBUTTONDBLCLK, 0, cCurpos);
							else
								SendMessageA(target, WM_RBUTTONDOWN, 0, cCurpos);

							SendMessageA(target, WM_RBUTTONUP, 0, cCurpos);
						}
						/* 中双击 */
						else if (!stricmp(StrArray[i], "--middle_twice"))
						{
							SendMessageA(target, WM_MBUTTONDOWN, 0, cCurpos);
							SendMessageA(target, WM_MBUTTONUP, 0, cCurpos);

							if (Style & CS_DBLCLKS)
								SendMessageA(target, WM_MBUTTONDBLCLK, 0, cCurpos);
							else
								SendMessageA(target, WM_MBUTTONDOWN, 0, cCurpos);

							SendMessageA(target, WM_MBUTTONUP, 0, cCurpos);
						}
						/* 滚轮向上 */
						else if (!stricmp(StrArray[i], "--roller_upper"))
						{
							/* wParam 的高字节是表示滚动的幅度，必须是 WHEEL_DELTA 的倍数，
							* 如果是负数则向下滚动。*/
							SendMessageA(target, WM_MOUSEWHEEL, WHEEL_DELTA << 16, cCurpos);
						}
						/* 滚轮向下 */
						else if (!stricmp(StrArray[i], "--roller_down"))
						{
							SendMessageA(target, WM_MOUSEWHEEL, -(WHEEL_DELTA << 16), cCurpos);
						}
						/* 滚轮向左 */
						else if (!stricmp(StrArray[i], "--roller_left"))
						{
							SendMessageA(target, WM_MOUSEHWHEEL, WHEEL_DELTA << 16, cCurpos);
						}
						/* 滚轮向右 */
						else if (!stricmp(StrArray[i], "--roller_right"))
						{
							SendMessageA(target, WM_MOUSEHWHEEL, -(WHEEL_DELTA << 16), cCurpos);
						}
						else WDO_ERROR_NOTYPE(StrArray[i]);

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);	
		}
		/* 使游标进行漂移运动 */
		else if (!stricmp(instruction, WDOINS_CUR_DRIFT))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					if (ArraySize == 1)
					{
						if (lrCursorDrift((int)IntArray[0][0], (int)IntArray[0][1], (BYTE)IntArray[0][2]))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "The cursor has begun to drift. > X- %d, Y- %d, speed = %d"
								, "游标开始漂移。> X- %d, Y- %d, speed = %d"
								, (int)IntArray[0][0], (int)IntArray[0][1], (BYTE)IntArray[0][2]
							);
						}
						else wdo_log_insec(OUTLOG_ERROR
							, "Cursor drift failed! > X- %d, Y- %d, speed = %lu"
							, "游标漂移失败！> X- %d, Y- %d, speed = %lu"
							, (int)IntArray[0][0], (int)IntArray[0][1], (BYTE)IntArray[0][2]
						);

						if (delay.ins) Sleep(delay.ins);
					}
					else
					{
						int* x = (int*)malloc(ArraySize * sizeof(int));
						int* y = (int*)malloc(ArraySize * sizeof(int));
						BYTE* speed = (BYTE*)malloc(ArraySize * sizeof(BYTE));

						for (size_t i = 0; i < ArraySize; ++i)
						{
							x[i] = (int)IntArray[i][0];
							y[i] = (int)IntArray[i][1];
							speed[i] = (BYTE)IntArray[i][2];
						}

						if (lrCursorDrifts(ArraySize, x, y, speed))
						{
							for (size_t i = 0; i < ArraySize; ++i) {
								wdo_log_insec(OUTLOG_ERROR
									, "Cursor drift failed! > X- %d, Y- %d, speed = %lu"
									, "游标漂移失败！> X- %d, Y- %d, speed = %lu"
									, (int)IntArray[0][0], (int)IntArray[0][1], (BYTE)IntArray[0][2]
								);

								if (delay.ins) Sleep(delay.ins);
							}
						}
						else if (TRUE)
						{
							for (size_t i = 0; i < ArraySize; ++i) {

								wdo_log_insec(OUTLOG_ERROR
									, "Cursor drift failed! > X- %d, Y- %d, speed = %lu"
									, "游标漂移失败！> X- %d, Y- %d, speed = %lu"
									, (int)IntArray[0][0], (int)IntArray[0][1], (BYTE)IntArray[0][2]
								);

								if(TRUE) if (delay.ins) Sleep(delay.ins);
							}
						}
						free(x);
						free(y);
						free(speed);
					}

					if (!TRUE) if (delay.ins) Sleep((DWORD)ArraySize * delay.ins);

					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 使游标开始摆动 */
		else if (!stricmp(instruction, WDOINS_CUR_WIGGLE))
		{
			/* 关闭摆动 */
			if (!stricmp(parameter, "-c"))
			{
				if (lrCursorWiggle(0, 0, FALSE))
				{
					wdo_log_insec(OUTLOG_NORMAL
						, "Cursor wiggle canceled."
						, "游标已取消摆动。"
						, NULL
					);
				}
				else wdo_log_insec(OUTLOG_ERROR
					, "The current cursor is not in vibration state, cannot be cancelled."
					, "当前游标没有在摆动状态，无法取消。"
					, NULL
				);

				if (delay.ins) Sleep(delay.ins);
			}
			else
			{
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (lrCursorWiggle((int)IntArray[i][0], (int)IntArray[i][0], TRUE))
						{
							wdo_log_insec(OUTLOG_NORMAL
								, "Cursor has been Start wiggling, range = %d, frequency = %d."
								, "游标已经开始摆动，幅度 = %d，频率 = %d。"
								, (int)IntArray[i][0], (int)IntArray[i][1]
							);
						}
						else wdo_log_insec(OUTLOG_ERROR
							, "Cursor wiggle failed!"
							, "游标摆动失败！"
							, NULL
						);

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&IntArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
		}
		/* 限制游标移动区域在一个矩形内 */
		else if (!stricmp(instruction, WDOINS_CUR_RECT))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				if (!stricmp(parameter, "-win") || !stricmp(parameter, "{-win}"))
				{
					RECT rect;
					GetWindowRect(ctwin, &rect);

					if (ClipCursor(&rect))
					{
						wdo_log_insec(OUTLOG_NORMAL
							, "The moving area of the cursor has been limited to a rectangle, X = %ld, Y = %ld, W = %ld, H = %ld."
							, "游标的移动区域已经限制在了一个矩形内，X = %ld, Y = %ld, W = %ld, H = %ld。"
							, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top
						);
					}
					else
					{
						wdo_log_insec(OUTLOG_ERROR
							, "Move area limit of cursor failed, X = %ld, Y = %ld, W = %ld, H = %ld.\n"
							, "游标的移动区域限制失败，X = %ld, Y = %ld, W = %ld, H = %ld\n"
							, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top
						);

						wdo_log_error(lr_TO_STR(ClipCursor));
					}


					if (delay.ins) Sleep(delay.ins);
				}
				else
				{
					if (IntArray = wdo_curlybrace_splitint(parameter, &ArraySize))
					{
						for (size_t i = 0; i < ArraySize; ++i)
						{
							RECT rect = { (LONG)IntArray[i][0], (LONG)IntArray[i][1], (LONG)(IntArray[i][2] + IntArray[i][0]), (LONG)(IntArray[i][3] + IntArray[i][1]) };

							if (ClipCursor(&rect))
							{
								wdo_log_insec(OUTLOG_NORMAL
									, "The moving area of the cursor has been limited to a rectangle, X = %ld, Y = %ld, W = %ld, H = %ld."
									, "游标的移动区域已经限制在了一个矩形内，X = %ld, Y = %ld, W = %ld, H = %ld。"
									, (LONG)IntArray[i][0], (LONG)IntArray[i][1], (LONG)IntArray[i][2], (LONG)IntArray[i][3]
								);
							}
							else
							{
								wdo_log_insec(OUTLOG_ERROR
									, "Move area limit of cursor failed, X = %ld, Y = %ld, W = %ld, H = %ld.\n"
									, "游标的移动区域限制失败，X = %ld, Y = %ld, W = %ld, H = %ld\n"
									, (LONG)IntArray[i][0], (LONG)IntArray[i][1], (LONG)IntArray[i][2], (LONG)IntArray[i][3]
								);

								wdo_log_error(lr_TO_STR(ClipCursor));
							}

							if (delay.ins) Sleep(delay.ins);
						}
						lrfrees(&IntArray, ArraySize);
					}
					else WDO_ERROR_NOMEMORY();
				}
			}
			/* 如果不带参数则取消限制 */
			else ClipCursor(NULL);
		}


		/*****************************************************************************************
		* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 新建窗口 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
		******************************************************************************************/

		/* 新建一个空白框 */
		else if (!stricmp(instruction, WDOINS_BOX_BLANK))
		{
			IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

			if (IntArray != NULL)
			{
				for (size_t i = 0; i < ArraySize; ++i)
				{
					POINT curpos;
					GetCursorPos(&curpos);

					ctwin = lrCreateBlankBox(curpos.x, curpos.y, (int)IntArray[i][0], (int)IntArray[i][1], AliasBools[alias_type_box] ? &(alias.data.box) : NULL);
					if (UpdatePtrwin) ptrwin = ctwin;

					wdo_log(instruction, State = !!ctwin, NULL);
					wdo_log_win_brief(ctwin, TRUE);

					if (delay.ins) Sleep(delay.ins);
				}
				lrfrees(&IntArray, ArraySize);
			}
			else WDO_ERROR_NOMEMORY();
		}
		/* 新建一个文本框并显示文字 */
		else if (!stricmp(instruction, WDOINS_BOX_TEXT))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				POINT curpos;
				GetCursorPos(&curpos);

				lrstrrem(parameter, '{', '}');

				ctwin = lrCreateTextBox(parameter, curpos.x, curpos.y, AliasBools[alias_type_box] ? &(alias.data.box) : NULL);
				if (UpdatePtrwin) ptrwin = ctwin;

				{
					wdo_log(instruction, State = !!ctwin, NULL);
					wdo_log_win_brief(ctwin, TRUE);
				}
			}
			else
			{
				ctwin = lrCreateTextBox(NULL, 0, 0, AliasBools[alias_type_box] ? &(alias.data.box) : NULL);
				if (UpdatePtrwin) ptrwin = ctwin;

				{
					wdo_log(instruction, State = !!ctwin, NULL);
					wdo_log_win_brief(ctwin, TRUE);
				}
			}
			if (delay.ins) Sleep(delay.ins);
		}
		/* 新建一个信息框并查看指定窗口的信息 */
		else if (!stricmp(instruction, WDOINS_BOX_INFO))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						iStringLength = strlen(StrArray[i]) + 1;

						char* ib_type = (char*)malloc(iStringLength);
						char* ib_preset = (char*)malloc(iStringLength);

						if (ib_type != NULL && ib_preset != NULL)
						{

							lrstrsep(StrArray[i], 2, ib_type, ib_preset);

							State = TRUE;

							INFOBOX_WINDOWTYPE type = -1;

							/* 静态 */
							if (!stricmp(ib_type, "-s"))
							{
								if (!stricmp(ib_preset, "ct")) type = infobox_s_ct;
								else if (!stricmp(ib_preset, "par")) type = infobox_s_par;
								else if (!stricmp(ib_preset, "top")) type = infobox_s_top;
								else if (!stricmp(ib_preset, "cur")) type = infobox_s_cur;
								else if (!stricmp(ib_preset, "desk")) type = infobox_s_desk;
								else if (!stricmp(ib_preset, "cp")) type = infobox_s_cp;

								else WDO_ERROR_NOTYPE(ib_preset);
							}
							/* 动态 */
							else if (!stricmp(ib_type, "-v"))
							{
								if (!stricmp(ib_preset, "ct")) type = infobox_v_ct;
								else if (!stricmp(ib_preset, "par")) type = infobox_v_par;
								else if (!stricmp(ib_preset, "top")) type = infobox_v_top;
								else if (!stricmp(ib_preset, "cur")) type = infobox_v_cur;
								else if (!stricmp(ib_preset, "desk")) type = infobox_v_desk;
								else if (!stricmp(ib_preset, "cp")) type = infobox_v_cp;

								else WDO_ERROR_NOTYPE(ib_preset);
							}
							else WDO_ERROR_NOTYPE(ib_type);

							if (type != -1)
							{
								POINT curpos;
								GetCursorPos(&curpos);

								ctwin = lrCreateInfoBox(type < 6 ? (&ctwin) : (&ptrwin), type, curpos.x, curpos.y, AliasBools[alias_type_box] ? &(alias.data.box) : NULL);
								if (UpdatePtrwin) ptrwin = ctwin;

								{
									wdo_log(instruction, State = !!ctwin, NULL);
									wdo_log_win_brief(ctwin, TRUE);
								}
							}
							else State = FALSE;

							free(ib_type);
							free(ib_preset);
						}

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 新建一个日志框并显示日志 */
		else if (!stricmp(instruction, WDOINS_BOX_LOG))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				POINT curpos;
				GetCursorPos(&curpos);

				lrstrrem(parameter, '{', '}');

				char* TextCarrier;
				lrStringLinebreakMode(parameter, &TextCarrier);

				ctwin = lrCreateLogBox(curpos.x, curpos.y, 500, 500, TextCarrier, AliasBools[alias_type_box] ? &(alias.data.box) : NULL);
				free(TextCarrier);

				if (UpdatePtrwin) ptrwin = ctwin;

				if (TRUE)
				{
					wdo_log(instruction, State = !!ctwin, NULL);
					wdo_log_win_brief(ctwin, TRUE);
				}
				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 新建一个图片框并显示图片 */
		else if (!stricmp(instruction, WDOINS_BOX_IMAGE))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						POINT curpos;
						GetCursorPos(&curpos);

						lrstrrem(StrArray[i], '{', '}');
						lrstrrem(StrArray[i], '\"', '\"');

						ctwin = lrCreateImageBox(StrArray[i], curpos.x, curpos.y, AliasBools[alias_type_box] ? &(alias.data.box) : NULL);
						if (UpdatePtrwin) ptrwin = ctwin;

						if (TRUE)
						{
							wdo_log_win_brief(ctwin, TRUE);

							wdo_log_insec(!!ctwin
								,
								"Tips:\n"
								"    - Right click anywhere in the window to repaint and refresh the window;\n"
								"    - Left click anywhere in the window to draw a polyline;\n"
								"    - With keyboard focus, you can enter any character at the current cursor point position;\n"
								"    - This window will not load the Chinese input method interface, but it can input Chinese normally;\n"
								,
								"提示：\n"
								"    - 右击该窗口任意位置可重绘刷新该窗口；\n"
								"    - 左击该窗口任意位置可绘制折线；\n"
								"    - 具有键盘焦点时，可输入任意字符在当前游标点位置；\n"
								"    - 该窗口不会加载中文输入法界面，但可正常输入中文；\n"
								, NULL);
						}

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else if (AliasBools[alias_type_path] == TRUE && alias.data.path.Type == PATHTYPE_FILE)
			{
				POINT curpos;
				GetCursorPos(&curpos);

				ctwin = lrCreateImageBox(alias.data.path.Path, curpos.x, curpos.y, AliasBools[alias_type_box] ? &(alias.data.box) : NULL);
				if (UpdatePtrwin) ptrwin = ctwin;

				if (TRUE)
				{
					wdo_log_win_brief(ctwin, TRUE);

					wdo_log_insec(!!ctwin
						,
						"Tips:\n"
						"    - Right click anywhere in the window to repaint and refresh the window;\n"
						"    - Left click anywhere in the window to draw a polyline;\n"
						"    - With keyboard focus, you can enter any character at the current cursor point position;\n"
						"    - This window will not load the Chinese input method interface, but it can input Chinese normally;\n"
						,
						"提示：\n"
						"    - 右击该窗口任意位置可重绘刷新该窗口；\n"
						"    - 左击该窗口任意位置可绘制折线；\n"
						"    - 具有键盘焦点时，可输入任意字符在当前游标点位置；\n"
						"    - 该窗口不会加载中文输入法界面，但可正常输入中文；\n"
						, NULL);
				}

				if (delay.ins) Sleep(delay.ins);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 新建一个蓝屏框并禁止关闭 */
		else if (!stricmp(instruction, WDOINS_BOX_BLUE))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				lrstrrem(parameter, '{', '}');

				ctwin = lrCreateBlueBox(parameter, AliasBools[alias_type_box] ? &(alias.data.box) : NULL);
				if (UpdatePtrwin) ptrwin = ctwin;

				if (TRUE)
				{
					wdo_log(instruction, State = !!ctwin, NULL);
					wdo_log_win_brief(ctwin, TRUE);
				}

				lrfrees(&StrArray, ArraySize);
			}
			else
			{
				ctwin = lrCreateBlueBox(NULL, AliasBools[alias_type_box] ? &(alias.data.box) : NULL);
				if (UpdatePtrwin) ptrwin = ctwin;

				if (TRUE)
				{
					wdo_log(instruction, State = !!ctwin, NULL);
					wdo_log_win_brief(ctwin, TRUE);
				}
			}
			if (delay.ins) Sleep(delay.ins);
		}
		/* 设置框中文本 */
		else if (!stricmp(instruction, WDOINS_BOX_SET_TEXT))
		{
			if (lrIsWindowBox(ctwin))
			{
				lrstrrem(parameter, '{', '}');
				SendMessageA(ctwin, WM_WDO_SETBOXTEXT, (WPARAM)parameter, 0);

				wdo_log_insec(OUTLOG_NORMAL
					, "Window box _%X_ new text set done."
					, "窗口框 _%X_ 新文本设置完成。"
					, ctwin);
			}
			else WDO_ERROR_NOWINDOWBOX(ctwin);
		}


		/*****************************************************************************************
		* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 特殊指令 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
		******************************************************************************************/

		/* 循环执行一个命令块 */
		else if (!stricmp(instruction, WDOINS_LOOP) || !stricmp(instruction, "wdo"))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						lrstrrem(StrArray[i], '{', '}');
						lrWindowDo(StrArray[i], _OutlogState);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 随机执行一个命令块中的一条命令 */
		else if (!stricmp(instruction, WDOINS_RANDOMRUN))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					lr_srand();
					int r = rand() % ArraySize;

					lrstrrem(StrArray[r], '{', '}');
					lrWindowDo(StrArray[r], _OutlogState);

					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 设置运行队列 */
		else if (!stricmp(instruction, WDOINS_QUEUE))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				if (TimingRun_InsQuan == 0)
				{
					TimingRun_InsSets = (char**)malloc(sizeof(char*));
					if (TimingRun_InsSets == NULL) wdo_error_ex();
				}

				size_t ArraySize = 0;
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						lrstrrem(StrArray[i], '{', '}');

						char** TimingRun_InsSets_NEW = (char**)realloc(TimingRun_InsSets, (++TimingRun_InsQuan) * sizeof(char*));

						if (TimingRun_InsSets_NEW != NULL) TimingRun_InsSets = TimingRun_InsSets_NEW;
						else wdo_error_ex();

						TimingRun_InsSets[TimingRun_InsQuan - 1] = (char*)malloc(strlen(StrArray[i]) + 1);
						if (TimingRun_InsSets[TimingRun_InsQuan - 1] == NULL) wdo_error_ex();

						strcpy(TimingRun_InsSets[TimingRun_InsQuan - 1], StrArray[i]);

						if (delay.ins) Sleep(delay.ins);
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 设置定时运行任务 */
		else if (!stricmp(instruction, WDOINS_AT))
		{
			if (TimingRun_InsQuan)
			{
				size_t ArraySize = 0;
				IntArray = wdo_curlybrace_splitint(parameter, &ArraySize);

				/* 如果参数为空时则使用缺省值 */
				if (IntArray == NULL)
				{
					IntArray = (__int64**)malloc(sizeof(__int64*));

					if (IntArray != NULL)
					{
						IntArray[0] = (__int64*)malloc(INT64_OUTPUT_MAX_LEN * sizeof(__int64));

						if (IntArray[0] != NULL)
						{
							memset(IntArray[0], 0, INT64_OUTPUT_MAX_LEN * sizeof(__int64));
							ArraySize = 1;
						}
						else free(IntArray);
					}
				}

				if (IntArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						if (State = !!wdo_thread_timing(TimingRun_InsSets, TimingRun_InsQuan, (UINT)IntArray[i][0], (UINT)IntArray[i][1], (UINT)IntArray[i][2]))
						{
							if (TRUE)
							{
								wdo_log_insec(State
									, "The scheduled task is set successfully. These instructions will be run at %02u:%02u:%02u:"
									, "定时任务设置成功，在 %02u:%02u:%02u 将运行以下指令："
									, (UINT)IntArray[i][0], (UINT)IntArray[i][1], (UINT)IntArray[i][2]
								);

								for (size_t j = 0; j < TimingRun_InsQuan; ++j) wdo_log(NULL, OUTLOG_NORMAL, "\t+%llu.> %s\n", j + 1, TimingRun_InsSets[j]);
							}
						}
						else if (TRUE)
						{
							wdo_log_insec(State
								, "The scheduled task setting running at %02u:%02u:%02u failed."
								, "在 %02u:%02u:%02u 运行的定时任务设置失败。"
								, (UINT)IntArray[i][0], (UINT)IntArray[i][1], (UINT)IntArray[i][2]
							);
						}
					}
					lrfrees(&IntArray, ArraySize);

					lrfrees(&TimingRun_InsSets, TimingRun_InsQuan);
					TimingRun_InsQuan = 0;
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else wdo_log_insec(OUTLOG_ERROR
				, "The run queue of \"" WDOINS_QUEUE "\" is empty! Cannot set scheduled task. See " WDOINS_HELP " " WDOINS_QUEUE "."
				, "\"" WDOINS_QUEUE "\" 的运行队列为空！无法设置定时任务。详见 " WDOINS_HELP " " WDOINS_QUEUE "。"
				, NULL
			);
		}
		/* 设置全局快捷键的命令块 */
		else if (!stricmp(instruction, WDOINS_HOTKEY))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i)
					{
						iStringLength = strlen(StrArray[i]) + 1;
						char* hotkey = (char*)malloc(iStringLength);
						char* hotins = (char*)malloc(iStringLength);

						if (hotkey && hotins)
						{
							lrstrsep(StrArray[i], 2, hotkey, hotins);

							size_t hotins_count = 0;
							char** hotins_split = wdo_curlybrace_splitstr(hotins, &hotins_count);

							if ((State = wdo_set_hotkey(hotkey, hotins_count, hotins_split)) == FALSE)
							{
								if (hotins_count) lrfrees(&hotins_split, hotins_count);

								if (TRUE)
								{
									wdo_log_error(lr_TO_STR(wdo_set_hotkey));
									wdo_log_language(OUTLOG_ERROR, "HOTKEY <%s>\n", "快捷键 <%s>\n", hotkey);
								}
							}
							else
							{
								if (hotins_count)
								{
									wdo_log_insec(OUTLOG_NORMAL
										, "Hotkey registration succeeded.\nHotkey is <%s>\n"
										, "快捷键 <%s> 注册成功。\n"
										, hotkey);
								}
								else wdo_log_insec(OUTLOG_NORMAL
									, "Hotkey logout succeeded.\nHotkey is <%s>\n"
									, "快捷键 <%s> 注销成功。\n"
									, hotkey);
							}

							free(hotkey);
							free(hotins);
						}
						else if(TRUE) WDO_ERROR_NOMEMORY();
					}
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}
		/* 新建一条线程 */
		else if (!stricmp(instruction, WDOINS_THREAD))
		{
			if (parameter != NULL && parameter[0] != '\0')
			{
				size_t ArraySize = 0;
				StrArray = wdo_curlybrace_splitstr(parameter, &ArraySize);

				if (StrArray != NULL)
				{
					for (size_t i = 0; i < ArraySize; ++i) lrstrrem(StrArray[i], '{', '}');

					HANDLE hThread;

					if (State = !!(hThread = wdo_thread_run(StrArray, ArraySize)))
					{
						wdo_log_insec(State
							, "A new thread created. ID = %X"
							, "已新建一条线程。ID = %X"
							, GetThreadId(hThread)
						);
					}
					else wdo_log_insec(State
						, "Thread creation failed!"
						, "线程创建失败！"
						, NULL
					);
					
					lrfrees(&StrArray, ArraySize);
				}
				else WDO_ERROR_NULLPARAM(instruction);
			}
			else WDO_ERROR_NULLPARAM(instruction);
		}

		/* 其他没有的指令 */
		else wdo_log_insec(OUTLOG_ERROR,
			"Unrecognized instruction, Please enter " WDOINS_HELP " to get all instruction sets.",
			"无法识别的指令，请输入 " WDOINS_HELP " 以获取所有指令集合。", NULL);

		/* 如果 ArraySize 为零时说明指令并没有成功运行，需要在此延时 */
		if(!ArraySize) if(delay.ins) Sleep(delay.ins);
	}
	

	// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 准备返回 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ //

	// _goto_end_:

	ret.state = State;

	if (instruction) free(instruction);
	if (parameter)   free(parameter);

	if (AliasIndex)  free(AliasIndex);

	if (_OutlogState == WDOECHO_TRUE || _OutlogState == WDOECHO_FALSE)
	{
		wdo_outlog_switch(OldEcho);
	}

	return ret;
}

#undef WDO_LANGUAGE

#include "../Head/wdo_errorlog.h"

#endif			// _LR_WINDOW_DO_C_