// wdo_help_chinese.c -- For wdo_help() function
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
//
#include "../Head/lr_wdo.h"
#include "../Head/lrWindowDo.h"
#include "../Head/wdo_instructions.h"


#if 1// !WDO_LANGUAGE

void wdo_help_chinese(const char* const _str)
{
	char StrPar[1024];
	char StrRem[1024 * 2];

	wdo_setcolor(OUTLOG_NORMAL_COLOR);

	/*************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 目录 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	**************************************************************************************/
	if (_str == NULL || _str[0] == '\0')
	{
		unsigned int InstructionCount = 0;

		wdo_log(NULL, OUTLOG_NORMAL, "\n基本类：\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_ECHO				" 打开或关闭回显开关，或在控制台打印文本数据\n"
			WDOINS_CD				" 设置当前工作路径\n"
			WDOINS_CLIP				" 设置剪贴板数据(文本)\n"
			WDOINS_STARTUP			" 快速添加开机启动项\n"
			WDOINS_PAUSE			" 使程序暂停(毫秒)\n"
			WDOINS_DELAY			" 设置延时器(毫秒)\n"
			WDOINS_MUSIC			" 播放一首音乐\n"
			WDOINS_BEEP				" 生成蜂鸣声\n"
			WDOINS_CALL				" 调用 WDO 文件\n"
			WDOINS_CMD				" 调用 CMD 指令\n"
			WDOINS_MCI				" 调用 MCI 指令\n"
			WDOINS_EXIT				" 退出 " WDO_EXE_FILE "\n"
			WDOINS_EXITASK			" 申请退出 " WDO_EXE_FILE "\n"
			WDOINS_HELP				" 获取帮助和使用方法\n"
			WDOINS_ABOUT			" 关于 " WDO_EXE_FILE "\n"
		);
		wdo_log(NULL, OUTLOG_NORMAL, "\n别名类：\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_ALIAS_APPLY		" 申请一个别名，用于存储特定的数据\n"
			WDOINS_ALIAS_GET		" 获取一个预定义的默认别名\n"
			WDOINS_ALIAS_SET		" 设置或更改别名中的数据\n"
			WDOINS_ALIAS_SETUP		" 安装一个别名，将类型为 " ALIASTYPE_STR_BOX " 的别名安装到一个已有的框（窗口）上\n"
			WDOINS_ALIAS_SEE		" 显示别名结构\n"
			WDOINS_ALIAS_SAVE		" 保存一个别名文件（" FALIAS_EXT "）。可共享给其他 " WDO_SCRIPT_SUFFIX " 脚本加载使用\n"
			WDOINS_ALIAS_LOAD		" 加载一个别名文件（" FALIAS_EXT "）。必须由指令 " WDOINS_ALIAS_SAVE " 保存\n"
		);
		wdo_log(NULL, OUTLOG_NORMAL, "\n窗口控制类：\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_PTR				" 指定（选中）一个要操作的窗口。对窗口进行操作时必须使用此指令确认要操作窗口\n"
			WDOINS_UPDATEPTR			" " WDOINS_PTR " 更新开关\n"
			WDOINS_OPEN				" 打开一个进程并指向一个新的窗口\n"
			WDOINS_LIST				" 遍历所有现有窗口，并打印至控制台\n"
			WDOINS_INFO				" 打印 " WDOINS_PTR " 指向窗口的信息在当前控制台\n"
			WDOINS_TITLE			" 更改 " WDOINS_PTR " 指向窗口的标题文本\n"
			WDOINS_TOP				" 将 " WDOINS_PTR " 指向窗口放置前台\n"
			WDOINS_TOPMOST			" 将 " WDOINS_PTR " 指向窗口置顶\n"
			WDOINS_BOTTOM			" 将 " WDOINS_PTR " 指向窗口置底\n"
			WDOINS_ACTIVE			" 将 " WDOINS_PTR " 指向窗口设置为活动窗口\n"
			WDOINS_FOCUS			" 将 " WDOINS_PTR " 指向窗口设置为焦点窗口\n"
			WDOINS_CAPTURE			" 将 " WDOINS_PTR " 指向窗口设置为捕获窗口\n"
			WDOINS_TRANSP			" 设置 " WDOINS_PTR " 指向窗口的透明度\n"
			WDOINS_ERASE			" 擦除 " WDOINS_PTR " 指向窗口的某一颜色\n"
			WDOINS_TRANSPERASE		" 设置 " WDOINS_PTR " 指向窗口的透明度，并擦除一种颜色\n"
			WDOINS_SEND				" 向 " WDOINS_PTR " 指向窗口发送文本消息\n"
			WDOINS_TOPSEND			" " WDOINS_TOP " 和 " WDOINS_SEND " 的结合\n"
			WDOINS_OUT				" 在 " WDOINS_PTR " 指向窗口中输出文字\n"
			WDOINS_DRAW				" 在 " WDOINS_PTR " 指向窗口中绘图\n"
			WDOINS_IMAGE			" 在 " WDOINS_PTR " 指向的窗口中加载一张图像\n"
			WDOINS_IMAGES			" 在 " WDOINS_PTR " 指向的窗口中加载一个目录中所有图像\n"
			WDOINS_PROJECT			" 投影 " WDOINS_PTR " 指向窗口到指定窗口\n"
			WDOINS_PARENT			" 设置 " WDOINS_PTR " 指向窗口的新父窗口\n"
			WDOINS_CHILD			" 设置 " WDOINS_PTR " 指向窗口的子窗口。\n"
			WDOINS_COPY				" 复制 " WDOINS_PTR " 指向窗口的内容到其他指定的窗口\n"
			WDOINS_MOVE				" 移动 " WDOINS_PTR " 指向窗口的坐标\n"
			WDOINS_MOVEX			" 沿 X 轴移动 " WDOINS_PTR " 指向窗口坐标\n"
			WDOINS_MOVEY			" 沿 Y 轴移动 " WDOINS_PTR " 指向窗口坐标\n"
			WDOINS_POS				" 更改 " WDOINS_PTR " 指向窗口的左上角顶点坐标位置\n"
			WDOINS_POSX				" 更改 " WDOINS_PTR " 指向窗口的左上角顶点坐标 X 位置\n"
			WDOINS_POSY				" 更改 " WDOINS_PTR " 指向窗口的左上角顶点坐标 Y 位置\n"
			WDOINS_RPOS				" 随机设置 " WDOINS_PTR " 指向窗口坐标\n"
			WDOINS_ZOOM				" 增大或缩小 " WDOINS_PTR " 指向窗口的大小\n"
			WDOINS_ZOOMX			" 增大或缩小 " WDOINS_PTR " 指向窗口的宽度\n"
			WDOINS_ZOOMY			" 增大或缩小 " WDOINS_PTR " 指向窗口的高度\n"
			WDOINS_SIZEX			" 设置 " WDOINS_PTR " 指向窗口的新宽度\n"
			WDOINS_SIZEY			" 设置 " WDOINS_PTR " 指向窗口的新高度\n"
			WDOINS_SHOW				" 显示或隐藏 " WDOINS_PTR " 指向窗口\n"
			WDOINS_STYLE			" 设置 " WDOINS_PTR " 指向窗口风格\n"
			WDOINS_MAX				" 将 " WDOINS_PTR " 指向窗口最大化，相当于按下窗口右上角最大化按钮\n"
			WDOINS_MIN				" 将 " WDOINS_PTR " 指向窗口最小化，相当于按下窗口右上角最小化按钮\n"
			WDOINS_RESTORE			" 将处于最大化的 " WDOINS_PTR " 指向窗口还原原来的大小，相当于按下窗口右上角还原按钮\n"
			WDOINS_REPAINT			" 重绘 " WDOINS_PTR " 指向窗口。向其发送 WM_PAINT 消息\n"
			WDOINS_STROKE			" 描边 " WDOINS_PTR " 指向窗口\n"
			WDOINS_FILL				" 填充 " WDOINS_PTR " 指向窗口\n"
			WDOINS_FLASH			" 在任务栏闪烁 " WDOINS_PTR " 指向窗口。\n"
			WDOINS_DRIFT			" 使 " WDOINS_PTR " 指向窗口从当前坐标漂移到指定坐标\n"
			WDOINS_FOLLOW			" 使 " WDOINS_PTR " 指向窗口跟随游标移动\n"
			WDOINS_WIGGLE			" 使 " WDOINS_PTR " 指向窗口进行震动\n"
			WDOINS_ASIDE			" 使 " WDOINS_PTR " 指向窗口靠边收入或弹出\n"
			WDOINS_PUSH				" 使 " WDOINS_PTR " 指向窗口可以被游标推动。\n"
			WDOINS_COLLIDE			" 使 " WDOINS_PTR " 指向窗口和游标碰撞时发生事件。\n"
			WDOINS_LOCKPAINT		" 锁定 " WDOINS_PTR " 指向窗口，使其禁止绘图\n"
			WDOINS_CLOSE			" 关闭 " WDOINS_PTR " 指向窗口或关闭窗口所属进程\n"
		);
		wdo_log(NULL, OUTLOG_NORMAL, "\n游标控制类：\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_CUR_MOVE			" 移动游标\n"
			WDOINS_CUR_MOVEX		" 移动游标 X 轴坐标\n"
			WDOINS_CUR_MOVEY		" 移动游标 Y 轴坐标\n"
			WDOINS_CUR_POS			" 设置游标坐标\n"
			WDOINS_CUR_POSX			" 设置游标 X 轴坐标\n"
			WDOINS_CUR_POSY			" 设置游标 Y 轴坐标\n"
			WDOINS_CUR_RPOS			" 随机设置游标坐标\n"
			WDOINS_CUR_CPOS			" 以 " WDOINS_PTR " 指向窗口左上角为原点(0,0)，设置游标坐标\n"
			WDOINS_CUR_CRPOS		" 以 " WDOINS_PTR " 指向窗口为区域，随机设置游标坐标\n"
			WDOINS_CUR_KEY			" 模拟按下游标键\n"
			WDOINS_CUR_DRIFT		" 使游标进行漂移运动\n"
			WDOINS_CUR_WIGGLE		" 使游标开始摆动\n"
			WDOINS_CUR_RECT			" 限制游标移动区域在一个矩形内\n"
		);
		wdo_log(NULL, OUTLOG_NORMAL, "\n新建窗口类：\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_BOX_BLANK		" 新建一个空白框，不会重绘\n"
			WDOINS_BOX_TEXT			" 新建一个文本框并显示文字\n"
			WDOINS_BOX_INFO			" 新建一个信息框并查看指定窗口的信息\n"
			WDOINS_BOX_LOG			" 新建一个日志框并显示日志\n"
			WDOINS_BOX_IMAGE		" 新建一个图片框并显示图片\n"
			WDOINS_BOX_BLUE			" 新建一个蓝屏框并禁止关闭\n"
			WDOINS_BOX_SET_TEXT		" 设置框中文本\n"
		);
		wdo_log(NULL, OUTLOG_NORMAL, "\n特殊类：\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_LOOP				" 循环执行一个命令块\n"
			WDOINS_RANDOMRUN		" 随机执行一个命令块中的一条命令\n"
			WDOINS_QUEUE			" 设置运行队列\n"
			WDOINS_AT				" 设置定时运行任务\n"
			WDOINS_HOTKEY			" 设置全局快捷键的命令块\n"
			WDOINS_THREAD			" 新建一条线程\n"
		);
		wdo_putc('\n');
		wdo_log(WDOINS_HELP, OUTLOG_NORMAL, WDOINS_HELP " 已找到 %u 项指令。\n\n", InstructionCount);
		wdo_log(WDOINS_HELP, OUTLOG_NORMAL, "所有指令不区分大小写。关于 " WDO_EXE_FILE " 的使用与技巧，详见 " WDOINS_HELP " WDO\n\n");
	}


	/***************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 基本类 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	****************************************************************************************/
	else if (!stricmp(_str, WDOINS_ECHO))
	{
		wdo_log_help("打开或关闭回显开关，或在控制台打印文本数据。"
			,
			WDOINS_ECHO "  [on/off]\n"
			WDOINS_ECHO "  \"text\"\n"
			,
			"[on]                    打开回显。\n"
			"[off]                   关闭回显。\n"
			"\n"
			"\"text\"                  要打印的文本数据。\n"
			,
			NULL
			,
			"如果正在运行脚本文件，将一并隐藏或显示控制台窗口。"
			,
			"." WDOINS_ECHO "  off"
		);
	}
	else if (!stricmp(_str, WDOINS_CD))
	{
		wdo_log_help(
			"设置当前工作路径。"
			,
			WDOINS_CD "  \"dir\"\n"
			WDOINS_CD "  (null)\n"
			,
			"\"dir\"                   新目录。可以是相对路径。\n"
			"(null)                  显示当前工作目录。\n"
			,
			ALIASTYPE_STR_PATH   " 如果 dir 缺省，则使用该值。"
			,
			"返回上一级目录：    " WDOINS_CD "  ..\n"
			,
			"." WDOINS_CD "  c:\\windows"
		);
	}
	else if (!stricmp(_str, WDOINS_CLIP))
	{
		wdo_log_help("设置剪贴板文本数据。剪贴板使用期间归属于当前 " WDO_EXE_FILE " 的窗口。"
			,
			WDOINS_CLIP "  [-T text | -F file | -C]"
			,
			"-T       text           直接设置一行不带换行的文本数据。\n"
			"-F       file           设置为文件中的数据。（所有的，包括换行）\n"
			"-C                      清空剪贴板数据。\n"
			,
			NULL
			,
			"该指令只能设置文本数据，暂不支持其他格式。\n"
			"如果失败，则可能是其他程序正在使用剪贴板。\n"
			,
			"." WDOINS_CLIP "  -t  欢迎使用 " WDO_EXE_FILE ""
		);
	}
	else if (!stricmp(_str, WDOINS_STARTUP))
	{
		wdo_log_help(
			"快速添加开机启动项。\n"
			,
			WDOINS_STARTUP "  \"cmdinstruction\"\n"
			WDOINS_STARTUP "  (null)\n"
			,
			"cmdinstruction          CMD 的指令，可以直接是一个文件路径。\n"
			"(null)                  检索路径别名，或显示当前启动项列表。\n"
			,
			ALIASTYPE_STR_PATH   " 如果参数缺省，则使用该值。否则仅打印列表。"
			,
			"参数 \"cmdinstruction\" 将原封不动添加到注册表中；\n"
			"所以允许路径后使用命令行参数和环境变量；\n"
			"\n"
			"将直接修改注册表中 HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run 的值；\n"
			"不需要管理员权限；\n"
			"如果有杀毒软件可能会被提示并拦截；\n"
			,
			"." WDOINS_STARTUP "  %Userprofile%\\Desktop\\MyProgram.exe"
		);
	}
	else if (!stricmp(_str, WDOINS_PAUSE))
	{
		wdo_log_help(
			"将程序挂机一段时间以暂停。单位为毫秒。（1s = 1000ms）"
			,
			WDOINS_PAUSE "  <time>"
			,
			"<time>                  需要暂停的时间。"
			,
			NULL
			,
			WDOINS_PAUSE " 仅暂停当前线程。\n"
			"但是在 " WDOINS_ECHO " on 状态下对其他线程略有影响，\n"
			"因为 " WDOINS_ECHO " 有使用多线程临界互斥锁。\n"
			,
			"." WDOINS_PAUSE "  520"
		);
	}
	else if (!stricmp(_str, WDOINS_DELAY))
	{
		wdo_log_help(
			"设置延时值，避免频繁的使用指令 " WDOINS_PAUSE "。单位为毫秒。（1s = 1000ms）"
			,
			WDOINS_DELAY "  [-I | -SW | -SL | -SF]  <time>\n"
			,
			"-I                      每执行一条指令后的延时。\n"
			"-SW                     使用指令 " WDOINS_SEND " 时每发送一个字符的延时。\n"
			"-SL                     使用指令 " WDOINS_SEND " 时每发送一行的延时。\n"
			"-SF                     使用指令 " WDOINS_SEND " 时每发送一个文件的延时。\n"
			"\n"
			"<time>                  设置的延时。\n"
			,
			NULL
			,
			"如果不再需要请将数值设置为零即可。"
			,
			"." WDOINS_DELAY "  -i  10\n"
			"." WDOINS_MOVEX "*20  1"
		);
	}
	else if (!stricmp(_str, WDOINS_MUSIC))
	{
		wdo_log_help(
			"播放一首音乐。需要更多功能请使用 " WDOINS_MCI " 指令。"
			,
			WDOINS_MUSIC "  [-O | -R | -W | -C | -P | -S]  \"path\""
			,
			"-O                      只播放一次。\n"
			"-R                      循环播放。\n"
			"-W                      只播放一次，等待播放完后才执行下一条指令。\n"
			"-C                      关闭播放。\n"
			"-P                      暂停播放。\n"
			"-S                      继续播放。\n"
			"\n"
			"\"path\"                  音乐文件路径（带后缀名）。\n"
			,
			ALIASTYPE_STR_PATH       " 如果 path 缺省，则使用此路径。"
			,
			NULL
			,
			"." WDOINS_MUSIC " -r  c:\\起风了.mp3"
		);
	}
	else if (!stricmp(_str, WDOINS_BEEP))
	{
		wdo_log_help(
			"生成蜂鸣声。"
			,
			WDOINS_BEEP "  <freq>  <duration>"
			,
			"<freq>                  声音频率，以赫兹为单位。\n"
			"<duration>              声音的持续时间，以毫秒为单位。\n"
			,
			NULL
			,
			"如果不带参数，则生成一声系统蜂鸣声。\n"
			"\n"
			"系统蜂鸣声不会等待声音停止，自定义蜂鸣则会等待 duration 毫秒；\n"
			"如果不希望等待，请使用多线程指令 " WDOINS_THREAD "。\n"
			"\n"
			"Windows 7 系统以下，不受静音和音量影响，采用 独立蜂鸣器（8254可编程间隔定时器芯片）来生成蜂鸣声；\n"
			"Windows 7(含 Windows 7) 系统则采用扬声器生成。\n"
			,
			"." WDOINS_BEEP "  500  1000"
		);
	}
	else if (!stricmp(_str, WDOINS_CALL))
	{
		wdo_log_help(
			"调用 " WDO_SCRIPT_SUFFIX " 文件。"
			,
			WDOINS_CALL "  \"path\""
			,
			"\"path\"                  文件路径。必须是 .wdo 后缀。\n"
			,
			ALIASTYPE_STR_PATH       " 如果 path 缺省，则使用此路径。"
			,
			"若要该指令立即返回或同时运行多个文件，请使用多线程指令 " WDOINS_THREAD "。详见 " WDOINS_HELP " " WDOINS_THREAD "。"
			,
			"." WDOINS_CALL "  c:\\main" WDO_SCRIPT_SUFFIX
		);
	}
	else if (!stricmp(_str, WDOINS_CMD))
	{
		wdo_log_help(
			"调用 CMD 指令。"
			,
			WDOINS_CMD "  {instructions}"
			,
			"{instructions}          cmd 指令。"
			,
			NULL
			,
			NULL
			,
			NULL
		);
	}
	else if (!stricmp(_str, WDOINS_MCI))
	{
		wdo_log_help(
			"调用 mci 指令。"
			,
			WDOINS_MCI "  {instructions}"
			,
			"{instructions}          mci 指令。"
			,
			NULL
			,
			NULL
			,
			"." WDOINS_MCI "  open  c:\\起风了.mp3  alias  music_1\n"
			"." WDOINS_MCI "  play  music_1  wait\n"
			"." WDOINS_MCI "  close  music_1"
		);
	}
	else if (!stricmp(_str, WDOINS_HELP))
	{
		wdo_log_help(
			"获取帮助和使用方法。"
			,
			WDOINS_HELP "  {instructions}"
			,
			"{instructions}          wdo 指令。"
			,
			NULL
			,
			NULL
			,
			"." WDOINS_HELP "  " WDOINS_PTR
		);
	}
	else if (!stricmp(_str, WDOINS_EXIT))
	{
		wdo_log_help(
			"退出 " WDO_EXE_FILE "。"
			,
			WDOINS_EXIT "  <code>"
			,
			"<code>                  退出码。默认为 0。"
			,
			NULL
			,
			"运行脚本文件（" WDO_SCRIPT_SUFFIX "）时将会等待所有使用指令创建的窗口和线程全部关闭时自动退出。\n"
			"退出快捷键 <" WDO_EXIT_HOTKEY_STR ">（全局） 。\n"
			,
			"." WDOINS_EXIT "  520"
		);
	}
	else if (!stricmp(_str, WDOINS_EXITASK))
	{
		wdo_log_help(
			"申请退出 " WDO_EXE_FILE "。"
			,
			WDOINS_EXITASK "  <code>"
			,
			"<code>                  退出码。默认为 0。"
			,
			NULL
			,
			"该条指令可能不会立即退出，会等待用户使用命令创建的窗口和线程全部关闭，然后自动退出。"
			"脚本文件（" WDO_SCRIPT_SUFFIX "）末尾自动添加该条指令。\n"
			"退出快捷键 <" WDO_EXIT_HOTKEY_STR ">（全局） 。\n"
			,
			"." WDOINS_EXIT "  520"
		);
	}


	/***************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 别名类 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	****************************************************************************************/
	else if (!stricmp(_str, WDOINS_ALIAS_APPLY))
	{
		sprintf(StrRem,
			"关于别名：\n"
			"        - 别名应用于指令中，可以更改指令行为，可使部分参数额外缺省；\n"
			"        - 别名使用 " WDOINS_ALIAS_APPLY " 和 " WDOINS_ALIAS_GET " 申请；\n"
			"        - 别名根据类型装载着不同的数据，使用 " WDOINS_ALIAS_SEE " 指令查看详细数据；\n"
			"        - 每申请一个别名，大约占用 %llu byte 的内存空间；\n"		    // sizeof(ALIAS)
			"命名约定：\n"
			"        - 别名的名称由：字母（A-Z,a-z），数字（0-9），下划线（_）组成；\n"
			"        - 第一个字符不可以是数字（0-9）；\n"
			"        - 单个别名的最大长度为 %u；\n"								// ALIAS_MAX_LENGTH
			"        - 别名名称%s区分大小写；\n"								// ALIAS_CASE_SENSITIVE
			"关于别名库：\n"
			"        - 使用 " WDOINS_ALIAS_APPLY " 和 " WDOINS_ALIAS_GET " 申请的别名都存放在别名库中；\n"
			"        - 别名库中的别名名称都是唯一的，放入重名别名将覆盖原有别名；\n"
			"        - 别名库中的别名将在进程结束时清空，或者手动删除；\n"
			"        - 别名库可以保存为一个文件，提供给后续使用，详见 " WDOINS_ALIAS_SAVE "；\n"
			"        - 可以从一个文件中加载一个或多个别名，详见 " WDOINS_ALIAS_LOAD "；\n"
			"使用别名：\n"
			"        - 原型 ALIAS.Instruction\n"
			"        - 别名放在指令前，与指令使用 '.' 分隔；\n"
			"\n"
			"        - 原型 ALIAS1.ALIAS2.Instruction\n"
			"        - 可带入多个别名，别名与别名之间使用 '.' 分隔；\n"
			"        - 如果带入多个同类型的别名，将只使用最右边的；\n"
			"\n"
			"        - 原型 ..Instruction\n"
			"        - “..” 是一个预定义的别名，类型为 " ALIASTYPE_STR_WIN "，代表自身的窗口；\n"
			"        - 这个预定义的别名不在别名库中，也不可修改其数据和类型；\n"
			"\n"
			"        - 原型 .Instruction\n"
			"        - 不带入任何别名；\n"
			"\n"
			"        - 别名名称需要自行检查输入是否正确；\n"
			"        - 查看指令会使用的别名，请参考 " WDOINS_HELP " 中各个指令中的 RetrieveAlias 区；\n"
			, sizeof(ALIASSINGLE)
			, ALIAS_MAX_LENGTH
			, ALIAS_CASE_SENSITIVE ? "" : "不"
		);

		wdo_log_help(
			"申请一个别名，用于存储特定的数据。\n"
			,
			WDOINS_ALIAS_APPLY "  [-NULL | -WIN | -PATH | -DRAW | -BOX | -DEL]  \"name\"\n"
			,
			"-NULL    name           指定类名为 " ALIASTYPE_STR_NULL "。\n"
			"-WIN     name           指定类型为 " ALIASTYPE_STR_WIN "。窗口为当前 " WDOINS_PTR " 指定的窗口。\n"
			"-PATH    name           指定类型为 " ALIASTYPE_STR_PATH "。\n"
			"-DRAW    name           指定类型为 " ALIASTYPE_STR_DRAW "。\n"
			"-OUT     name           指定类型为 " ALIASTYPE_STR_OUT "。\n"
			"-BOX     name           指定类型为 " ALIASTYPE_STR_BOX "。\n"
			"-DEL     name           删除一个别名。\n"
			,
			ALIASTYPE_STR_WIN        " 如果 -WIN  则复制其数据给新申请的别名。\n"
			ALIASTYPE_STR_PATH       " 如果 -PATH 则复制其数据给新申请的别名。\n"
			ALIASTYPE_STR_OUT        " 如果 -OUT  则复制其数据给新申请的别名。\n"
			ALIASTYPE_STR_DRAW       " 如果 -DRAW 则复制其数据给新申请的别名。\n"
			ALIASTYPE_STR_BOX        " 如果 -BOX  则复制其数据给新申请的别名。\n"
			,
			StrRem
			,
			"." WDOINS_ALIAS_APPLY "  -win  mywin\n"
			"mywin." WDOINS_MOVE "  20  20"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_GET))
	{
		wdo_log_help(
			"获取一个预定义的默认别名。"
			,
			WDOINS_ALIAS_GET "  [-OUT | -DRAW | -BOX_TEXT | -BOX_INFO | -BOX_LOG | -BOX_IMAGE | -BOX_BLUE | -BOX_RAIN]  \"alias\""
			,
			"-OUT                    指令 " WDOINS_OUT "  默认使用的别名。\n"
			"-DRAW                   指令 " WDOINS_DRAW "  默认使用的别名。\n"
			"-BOX_TEXT               指令 " WDOINS_BOX_TEXT  "  默认使用的别名。\n"
			"-BOX_INFO               指令 " WDOINS_BOX_INFO  "  默认使用的别名。\n"
			"-BOX_LOG                指令 " WDOINS_BOX_LOG   "  默认使用的别名。\n"
			"-BOX_IMAGE              指令 " WDOINS_BOX_IMAGE " 默认使用的别名。\n"
			"-BOX_BLUE               指令 " WDOINS_BOX_BLUE  "  默认使用的别名。\n"
			"\n"
			"alias                   别名的新名称，和指令 " WDOINS_ALIAS_APPLY " 一样放入别名库中不能重名。\n"
			,
			NULL
			,
			"使用指令 " WDOINS_ALIAS_GET " 获取的别名，同样放入别名库中，\n"
			"与指令 " WDOINS_ALIAS_APPLY " 不同的是，" WDOINS_ALIAS_GET " 获取到的是某个指令默认使用的别名，\n"
			"可以直接在保留原设置的情况下，仅改变单个设置。\n"
			"而指令 " WDOINS_ALIAS_APPLY " 需要从头构建。\n"
			"请根据需要选择两个指令。\n"
			,
			"." WDOINS_ALIAS_GET "  -box_info  ib\n"
			"ib." WDOINS_ALIAS_SET "  -box  -tc  00ff00\n"
			"ib." WDOINS_BOX_INFO "  -v  cur\n"
			"\n"
			"// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
			"// 创建一个信息框，保留原来设置，\n"
			"// 仅将文本颜色改为绿色\n"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_SET))
	{
		wdo_log_help(
			// function
			"设置或更改别名中的数据。"

			,// Syntax
			WDOINS_ALIAS_SET "  [-WIN | -PATH path]\n"
			"\n"
			WDOINS_ALIAS_SET "  [-OUT]  [-IMAGE path | -IPOS x y | -TC color | -BC color | -TR [on/off]]\n"
			WDOINS_ALIAS_SET "  [-OUT]  [-FONT]  [-N name | -W val | -H val | -WEI val | -I [on/off] | -U [on/off] | -S [on/off]]\n"
			"\n"
			WDOINS_ALIAS_SET "  [-DRAW] [-SC color | -SW width | -FILL [on/off] | -FC color | -FI image_path]\n"
			"\n"
			WDOINS_ALIAS_SET "  [-BOX]  [-CLASS class | -CURSOR path | -ICON path | -TC color | -BC color]\n"
			WDOINS_ALIAS_SET "  [-BOX]  [-FONT]  [-N name | -W val | -H val | -WEI val | -I [on/off] | -U [on/off] | -S [on/off]]\n"

			,// Parameters
			"-WIN                    设置类型为 " ALIASTYPE_STR_WIN " 的别名，设置窗口为当前 " WDOINS_PTR " 指向的窗口。\n"
			"-PATH    path           设置类型为 " ALIASTYPE_STR_PATH " 的别名，重新设置别名路径。\n"
			"\n"
			"-OUT                    设置类型为 " ALIASTYPE_STR_OUT " 的别名。\n"
			"-IMAGE   path           设置一张图片，打印文本的同时将一并打印这张图片。\n"
			"-IPOS    x  y           图片的打印坐标，以文本打印坐标为原点 (0, 0)。\n"
			"-TC      color          打印文本时使用的字体颜色。color 由六位十六进制数组成，使用 RGB 颜色。\n"
			"-BC      color          打印文本时使用的背景颜色。color 由六位十六进制数组成，使用 RGB 颜色。\n"
			"-TR      [on/off]       背景是否透明，如果透明，输出文本时将不输出背景颜色。\n"
			"-FONT    [-N]    name   设置字体名称，并根据当前字体设置生成新的字体。\n"
			"-FONT    [-W]    val    设置字体宽度，并根据当前字体设置生成新的字体。\n"
			"-FONT    [-H]    val    设置字体高度，并根据当前字体设置生成新的字体。\n"
			"-FONT    [-WEI]  val    设置字体粗细[0-1000]，并根据当前字体设置生成新的字体。\n"
			"-FONT    [-I [on/off]]  设置字体倾斜，并根据当前字体设置生成新的字体。\n"
			"-FONT    [-U [on/off]]  设置字体下划线，并根据当前字体设置生成新的字体。\n"
			"-FONT    [-S [on/off]]  设置字体删除线，并根据当前字体设置生成新的字体。\n"
			"\n"
			"-DRAW                   设置类型为 " ALIASTYPE_STR_DRAW " 的别名。\n"
			"-SC      color          设置描边颜色，color 由六位十六进制数组成，使用 RGB 颜色。\n"
			"-SW      width          设置描边宽度。\n"
			"-FILL    [on/off]       是否填充。\n"
			"-FC      color          设置填充颜色，color 由六位十六进制数组成，使用 RGB 颜色。\n"
			"-FI      image_path     设置填充图片，忽略 -FC 填充颜色。\n"
			"\n"
			"-BOX                    设置类型为 " ALIASTYPE_STR_BOX " 的别名。\n"
			"-CLASS   class          窗口所使用的类名。\n"
			"-CURSOR  path           窗口所使用的游标路径，后缀名为 \".cur\"。\n"
			"-ICON    path           窗口所使用的小图标路径，后缀名为 \".ico\"。\n"
			"-TC      color          窗口所使用的文本颜色，color 由六位十六进制数组成，使用 RGB 颜色。\n"
			"-BC      color          窗口所使用的背景颜色，color 由六位十六进制数组成，使用 RGB 颜色。\n"
			,// RetrieveAlias
			ALIASTYPE_STR_WIN        " 如果参数一为 -WIN ，将设置该别名中的数据。\n"
			ALIASTYPE_STR_PATH       " 如果参数一为 -PATH，将设置该别名中的数据。\n"
			ALIASTYPE_STR_OUT        " 如果参数一为 -OUT ，将设置该别名中的数据。\n"
			ALIASTYPE_STR_DRAW       " 如果参数一为 -DRAW，将设置该别名中的数据。\n"
			ALIASTYPE_STR_BOX        " 如果参数一为 -BOX ，将设置该别名中的数据。\n"
			,// Remarks
			"申请别名请使用指令 " WDOINS_ALIAS_APPLY "，详见 " WDOINS_HELP " " WDOINS_ALIAS_APPLY "。\n"
			"使用时请带入对应类型的别名，否则将设置失败。\n"
			"\n"
			" \"" ALIASTYPE_STR_BOX "\" 类型的别名可以更改指令 " WDOINS_BOX_TEXT " 等创建的框窗口，\n"
			"新窗口都有自己的默认别名设置，在不满意默认设置的情况下可以使用本指令更改并在创建窗口时带入。\n"
			"设置窗口类名时一定要在窗口创建时带入别名，因为窗口一旦创建，类名将无法更改。\n"
			"  例：\n"
			"          ." WDOINS_ALIAS_GET "  -box_info  mybox\n"
			"          mybox." WDOINS_ALIAS_SET "  -box -class  myclass\n"
			"          mybox." WDOINS_BOX_INFO "  -v  cur\n\n"
			"以上例子将以 \"myclass\" 作为类名创建一个信息窗口。否则将使用默认的类名 \"" lrCLASSNAME_INFOBOX "\"。\n"
			"如果想在窗口创建完成后更改文本颜色和字体名称，可以使用以下命令：\n"
			"  例：\n"
			"          ." WDOINS_BOX_INFO "  -v cur\n"
			"          ." WDOINS_ALIAS_GET "  -box_info  mybox\n"
			"          mybox." WDOINS_ALIAS_SET "  -box  -tc  9181CB\n"
			"          mybox." WDOINS_ALIAS_SET "  -box  -font  -name  宋体\n"
			"          mybox." WDOINS_ALIAS_SETUP "\n\n"
			"指令 " WDOINS_ALIAS_SETUP " 用于启用 \"" ALIASTYPE_STR_BOX "\" 类型的别名，只有使用此指令，设置才能生效。详见 " WDOINS_HELP " " WDOINS_ALIAS_SETUP "。\n"
			,// Examples
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_ALIAS_APPLY "  -out  alias_out\n"
			"alias_out." WDOINS_ALIAS_SET "\n"
			"{\n"
			"    -out  -tr  on\n"
			"    -out  -tc  00FF00\n"
			"    -out  -font  -w  20\n"
			"    -out  -font  -h  20\n"
			"    -out  -image  c:\\out.bmp\n"
			"    -out  -ipos  20  20\n"
			"}\n"
			"alias_out." WDOINS_OUT "  5  10  Hello\n"
			"\n"
			"// 在当前游标指向的窗口的(5,10)点用绿色打印 \"Hello\"，\n"
			"// 并打印一张路径 \"c:\\out.bmp\" 的图片。\n"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_SETUP))
	{
		wdo_log_help(
			"安装一个别名，将类型为 " ALIASTYPE_STR_BOX " 的别名安装到一个已有的框（窗口）上。"
			,
			WDOINS_ALIAS_SETUP "  \"alias\"\n"
			WDOINS_ALIAS_SETUP "  (null)\n"
			,
			"alias                   要加载的别名。如果省略则必须带入别名。\n"
			"(null)                  使用带入的别名。\n"
			,
			ALIASTYPE_STR_WIN		 " 当前的框（窗口）。\n"
			ALIASTYPE_STR_BOX        " 如果 alias 缺省，则使用该别名。\n"
			,
			"申请别名请使用指令 " WDOINS_ALIAS_APPLY "，详见 " WDOINS_HELP " " WDOINS_ALIAS_APPLY"。\n"
			"设置别名数据请使用指令 " WDOINS_ALIAS_SET "，详见 " WDOINS_HELP " " WDOINS_ALIAS_SET"。\n"
			"指令 " WDOINS_ALIAS_SETUP " 一般用于使用指令 " WDOINS_ALIAS_SET " 后。\n"
			,
			"." WDOINS_BOX_INFO "  -v  cur\n"
			"." WDOINS_ALIAS_APPLY "  -box  box\n"
			"box." WDOINS_ALIAS_SET "  -tc  00FF00\n"
			"box." WDOINS_ALIAS_SET "  -bc  0000FF\n"
			"box." WDOINS_ALIAS_SETUP "\n"
			"\n"
			"// 将信息框变为蓝色背景和绿色字体。"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_SEE))
	{
		wdo_log_help(
			"显示别名结构。"
			,
			WDOINS_ALIAS_SEE "  (null)\n"
			WDOINS_ALIAS_SEE "  [-ALL]\n"
			WDOINS_ALIAS_SEE "  \"alias\"\n"
			,
			"(null)                  显示当前所有别名列表（不显示数据结构）。\n"
			"\n"
			"-ALL                    依次打印别名库中所有别名的结构。\n"
			"\n"
			"alias                   指定要打印结构的别名。\n"
			,
			NULL
			,
			"此指令只用来快速检测别名中的数据情况。\n"
			,
			"." WDOINS_ALIAS_APPLY "  -box  MyBoxAlias\n"
			"." WDOINS_ALIAS_SEE "  MyBoxAlias\n"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_SAVE))
	{
		wdo_log_help(
			"保存一个别名文件（" FALIAS_EXT "）。可共享给其他 " WDO_SCRIPT_SUFFIX " 脚本加载使用。"
			,
			WDOINS_ALIAS_SAVE "  [-LIB]   \"path\"\n"
			WDOINS_ALIAS_SAVE "  \"alias\"  \"path\"\n"
			,
			"-LIB                    保存当前整个别名库。\n"
			"\n"
			"alias                   保存指定的别名。\n"
			"path                    将在这个路径生成文件。（可以不加后缀名，但必须指定文件名）\n"
			,
			ALIASTYPE_STR_PATH       " 如果 path 缺省，则使用此路径。"
			,
			"同一个文件可以保存多个别名和数据，如果文件存在则追加，不存在则创建。\n"
			"加载别名库请使用 " WDOINS_ALIAS_LOAD "，详见 " WDOINS_HELP " " WDOINS_ALIAS_LOAD "。\n"
			,
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_ALIAS_APPLY"  -win  CtCurWin\n"
			"." WDOINS_ALIAS_SAVE "  CtCurWin  c:\\username\\desktop\\MyWinAlias" FALIAS_EXT "\n"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_LOAD))
	{
		wdo_log_help(
			"加载一个别名文件（" FALIAS_EXT "）。必须由指令 " WDOINS_ALIAS_SAVE " 保存。"
			,
			WDOINS_ALIAS_LOAD "  \"path\"\n"
			WDOINS_ALIAS_LOAD "  (null)\n"
			,
			"path                    别名文件的路径。(如果不带后缀则假设后缀名为 " FALIAS_EXT ")\n"
			"(null)                  使用路径别名。\n"
			,
			ALIASTYPE_STR_PATH       " 如果 path 缺省，则使用此路径。"
			,
			"加载的别名直接放入别名库中，可以直接根据别名的名字调用。\n"
			"如果出现重名，将覆盖以前的别名。\n"
			,
			"." WDOINS_ALIAS_LOAD "  c:\\username\\desktop\\AliasLib" FALIAS_EXT
		);
	}


	/***************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 窗口类 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	****************************************************************************************/

	else if (!stricmp(_str, WDOINS_PTR))
	{
		sprintf(StrPar,
			"-P       [top]          指定当前前台窗口。\n"
			"-P       [par]          指定当前窗口的父窗口(如果有)。\n"
			"-P       [act]          指定当前活动窗口。\n"
			"-P       [foc]          指定当前具有焦点的窗口。\n"
			"-P       [desk]         指定当前桌面窗口。\n"
			"-P       [cur]          指定当前游标所在的窗口。\n"
			"-P       [cp]           指定当前游标所在的父窗口。\n"
			"-P       [rand]         指定一个随机的父窗口。\n"
			"-P       [crand]        指定一个当前父窗口的随机子窗口。\n"
			"\n"
			"-T       title          按窗口的标题文本查找窗口(不推荐)。最大长度支持 %d byte。\n"
			"-C       class          按窗口类名查找窗口。最大长度支持 %u byte。\n"
			"-F       filepath       按窗口进程路径查找窗口。完整路径包括后缀名（.exe），最大长度支持 %u byte。\n"
			"-I       id             按窗口的句柄 ID 查找窗口。使用十六进制数（最高 8 位）。\n"
			"-D       x  y           指定一个坐标点的归属窗口。\n"
			"\n"
			"(null)                  使用窗口别名。\n"
			, MAXLEN_WIN_TITLE
			, MAXLEN_WIN_CLASS
			, MAX_PATH
		);

		wdo_log_help(
			"指定（选中）一个要操作的窗口。对窗口进行操作时必须使用此指令确认要操作窗口。\n"
			,
			WDOINS_PTR "  [-P [top/par/act/foc/desk/cur/cp/rand/crand]]\n"
			WDOINS_PTR "  [-T title | -C class | -F filepath | -I id | -D x y]\n"
			WDOINS_PTR "  (null)\n"
			,
			StrPar
			,
			ALIASTYPE_STR_WIN        " 如果缺省所有参数，则指向该窗口。"
			,
			"指令 " WDOINS_PTR " 默认指向当前 " WDO_EXE_FILE " 窗口。\n"
			WDOINS_PTR " 将优先查找父窗口，如果父窗口未找到则查找子窗口。\n"
			"\n"
			"使用命令 \".." WDOINS_PTR " \" 可重新指定回 " WDO_EXE_FILE " 的窗口。\n"
			"遍历所有窗口请使用指令 " WDOINS_LIST "，详见 " WDOINS_HELP " " WDOINS_LIST "。\n"
			,
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_MOVE "  20  20"
		);
	}
	else if (!stricmp(_str, WDOINS_UPDATEPTR))
	{
		wdo_log_help(
			WDOINS_PTR " 的更新开关。"
			,
			WDOINS_UPDATEPTR "  [on/off]"
			,
			"on                      开启（默认）。\n"
			"off                     关闭。\n"
			,
			NULL
			,
			"该指令并不是禁止指令 " WDOINS_PTR " 更新窗口，\n"
			"而是禁止除了指令 " WDOINS_PTR " 外的指令更新 " WDOINS_PTR " 指向的窗口。\n"
			"\n"
			"比如指令：" WDOINS_BOX_TEXT ", " WDOINS_BOX_INFO ", " WDOINS_OPEN " ...\n"
			"这些类型的指令会直接将 " WDOINS_PTR " 指向新的窗口，\n"
			"如果不需要，请使用 " WDOINS_UPDATEPTR " 指令关闭即可。\n"
			,
			"." WDOINS_UPDATEPTR "  off"
		);
	}
	else if (!stricmp(_str, WDOINS_OPEN))
	{
		wdo_log_help(
			"打开一个进程并指向一个新的窗口。"
			,
			WDOINS_OPEN "  \"path\"\n"
			WDOINS_OPEN "  (null)\n"
			,
			"path                    进程路径，带后缀名（.exe）。\n"
			"(null)                  从别名中获取数据。\n"
			,
			ALIASTYPE_STR_PATH		" 如果 path 缺省，则检索该路径。（前提是该路径具有文件名）\n"
			ALIASTYPE_STR_WIN		" 如果 path 缺省，则使用该窗口的进程路径替代。\n"
			,
			"如果 " WDOINS_UPDATEPTR " on，则会将 " WDOINS_PTR " 指向一个新的窗口，\n"
			"如果不想更改 " WDOINS_PTR " 指向，请使用 " WDOINS_UPDATEPTR " 指令。\n"
			"新指向的窗口不一定是新打开进程的窗口，但是一定来源于该路径的程序。\n"
			"\n"
			"该指令成功后还会列出所有来自该路径程序的所有窗口（包括子窗口）。\n"
			,
			"." WDOINS_OPEN "  c:\\wdo.exe"
		);
	}
	else if (!stricmp(_str, WDOINS_LIST))
	{
		wdo_log_help(
			"遍历所有现有窗口，并打印至控制台。"
			,
			WDOINS_LIST "  (null)\n"
			WDOINS_LIST "  [-ALL | -CT]\n"
			,
			"(null)                  遍历所有父窗口。\n"
			"\n"
			"-ALL                    遍历所有窗口，包括所有子窗口。\n"
			"-CT                     仅遍历当前窗口的所有子窗口。\n"
			,
			ALIASTYPE_STR_WIN        " 如果参数一使用 -CT，则该别名指定当前窗口。"
			,
			NULL
			,
			"." WDOINS_LIST "  -all"
		);
	}
	else if (!stricmp(_str, WDOINS_INFO))
	{
		wdo_log_help("打印 " WDOINS_PTR " 指向窗口的信息在当前控制台。", WDOINS_INFO "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。", "可以使用指令 " WDOINS_BOX_INFO " 打印至图形化界面。", NULL);
	}
	else if (!stricmp(_str, WDOINS_TITLE))
	{
		sprintf(StrPar,
			"text                    新的标题文本。最大长度支持 %u byte。"
			, MAXLEN_WIN_TITLE
		);

		wdo_log_help(
			"更改 " WDOINS_PTR " 指向窗口的标题文本。"
			,
			WDOINS_TITLE "  \"text\""
			,
			StrPar
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。"
			,
			NULL
			,
			"." WDOINS_TITLE "  HelloWorld"
		);
	}
	else if (!stricmp(_str, WDOINS_TOP))
	{
		wdo_log_help("将 " WDOINS_PTR " 指向窗口放置前台。", WDOINS_TOP "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_TOPMOST))
	{
		wdo_log_help(
			"将 " WDOINS_PTR " 指向窗口置顶。"
			,
			WDOINS_TOPMOST "  (null)\n"
			WDOINS_TOPMOST "  [on/off]\n"
			,
			"(null)                  顶置\n"
			"\n"
			"[on]                    置顶\n"
			"[off]                   取消置顶\n"
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。"
			, NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_BOTTOM))
	{
		wdo_log_help("将 " WDOINS_PTR " 指向窗口置底。", WDOINS_BOTTOM "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_ACTIVE))
	{
		wdo_log_help("将 " WDOINS_PTR " 指向窗口设置为活动窗口。", WDOINS_ACTIVE "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_FOCUS))
	{
		wdo_log_help("将 " WDOINS_PTR " 指向窗口设置为焦点窗口。", WDOINS_FOCUS "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_CAPTURE))
	{
		wdo_log_help("将 " WDOINS_PTR " 指向窗口设置为捕获窗口。", WDOINS_CAPTURE "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_TRANSP))
	{
		wdo_log_help(
			"设置 " WDOINS_PTR " 指向窗口的透明度。"
			,
			WDOINS_TRANSP "  <percentage>"
			,
			"<percentage>            要透明的百分比[0-100]，0 = 完全透明，100 = 完全不透明。\n"
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。"
			,
			"关于透明窗口：\n"
			"        - 将成为分层窗口，游标可穿透；\n"
			"        - 具有扩展风格 " lr_TO_STR(WS_EX_LAYERED) "(0x00080000)；\n"
			"        - 使用指令 " WDOINS_ERASE " 可擦除窗口某一颜色（完全透明）；\n"
			,
			"." WDOINS_TRANSP "  20"
		);
	}
	else if (!stricmp(_str, WDOINS_ERASE))
	{
		wdo_log_help("擦除 " WDOINS_PTR " 指向窗口的某一颜色。"
			,
			WDOINS_ERASE "  <color>"
			,
			"<color>                 要擦除的颜色，由六位十六进制数值组成。"
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。"
			,
			"颜色采用 RGB 颜色模式。不区分大小写。\n"
			"可以使用指令 " WDOINS_INFO " 或 " WDOINS_BOX_INFO " 获取当前游标所在点的颜色值。\n"
			"\n"
			"黑色  =  000000  |  白色  =  FFFFFF  |  灰色  =  C0C0C0" "\n"
			"红色  =  FF0000  |  绿色  =  00FF00  |  蓝色  =  0000FF" "\n"
			"紫红  =  FF00FF  |  碧绿  =  7FFFD4  |  浅蓝  =  B0E0E6" "\n"
			"粉红  =  FF007F  |  淡绿  =  CCFFE5  |  淡蓝  =  CCE5FF" "\n"
			"黄色  =  FFFF00  |  黄绿  =  7FFF00  |  天蓝  =  87CEEB" "\n"
			"金黄  =  FFD700  |  绿土  =  385E0F  |  深蓝  =  191970" "\n"
			"浅黄  =  F5DEB3  |  翠绿  =  00C957  |  紫色  =  A020F0" "\n"
			"棕色  =  802A2A  |  嫩绿  =  00FF7F  |  淡紫  =  DA70D6" "\n"
			"黄褐  =  F0E68C  |  薄荷  =  BDFCC9  |  青色  =  00FFFF" "\n"
			,
			"." WDOINS_ERASE "  FFFFFF"
		);
	}
	else if (!stricmp(_str, WDOINS_TRANSPERASE))
	{
		wdo_log_help("设置 " WDOINS_PTR " 指向窗口的透明度，并擦除一种颜色。"
			,
			WDOINS_TRANSPERASE "  <percentage>  <color>\n"
			WDOINS_TRANSPERASE "  [-CANCEL]\n"
			,
			"<percentage>            要透明的百分比[0-100]，0 = 完全透明，100 = 完全不透明。\n"
			"<color>                 要擦除的颜色，由六位十六进制数值组成。\n"
			"\n"
			"[-CANCEL]               取消窗口透明属性。\n"
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。"
			,
			WDOINS_TRANSPERASE " 是结合于 " WDOINS_TRANSP " 和 " WDOINS_ERASE " 指令的结合体。\n"
			,
			"." WDOINS_TRANSPERASE "  50  FFFFFF\n"
			"// 设置窗口 50% 透明，并擦除 FFFFFF 颜色(100% 透明)\n"
		);
	}
	else if (!stricmp(_str, WDOINS_SEND))
	{
		wdo_log_help(
			"向 " WDOINS_PTR " 指向窗口发送文本消息。"
			,
			WDOINS_SEND "  [-T text| -I text| -F file| -D dir| -V]\n"
			WDOINS_SEND "  [-RC text | -RT file | -RF dir]\n"
			,
			"-T       text             发送一行文本消息。\n"
			"-I       text             发送一行文本消息，末尾自动补换行（回车）。\n"
			"-F       file             指定一个文件，依次发送其文件文本。可以是任意格式，但文件必须存在。\n"
			"-D       dir              指定一个目录，依次发送目录中的文件。不影响目录中的目录。\n"
			"-V                        发送粘贴信息。可使用指令 " WDOINS_CLIP " 设置剪贴板数据。\n"
			"-RC      text             发送 text 中的随机字符。\n"
			"-RT      file             发送 file 中的随机行。\n"
			"-RF      dir              发送 dir 中的随机文件。\n"
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。"
			,
			"文件请使用 ANSI 编码，否则可能出现乱码情况。\n"
			"可以使用指令 " WDOINS_DELAY " 减慢发送速度（延时）。\n"
			"\n"
			"可以利用该指令编写自动向QQ好友发送信息的脚本，配合指令 " WDOINS_AT " 可以实现定时发送。\n"
			"\n"
			"某些窗口需要获取键盘焦点或成为前台窗口时，才会接收窗口消息，如：微信\n"
			"可以调用 " WDOINS_TOPSEND " 指令。\n"
			,
			"." WDOINS_PTR "  -c  TXGuiFoundation\n"
			"." WDOINS_SEND "  -t  Hello\n"
			"\n"
			"// TXGuiFoundation 是 QQ 窗口的类名\n"
		);

	}
	else if (!stricmp(_str, WDOINS_TOPSEND))
	{
		wdo_log_help(
			WDOINS_TOP " 和 " WDOINS_SEND " 的结合。"
			,
			"同 " WDOINS_SEND
			,
			"同 " WDOINS_SEND
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。"
			,
			"先执行 " WDOINS_TOP "，再执行 " WDOINS_SEND "；\n"
			"部分窗口仅在窗口顶层并具有键盘焦点时，接收输入。\n"
			,
			NULL
		);
	}
	else if (!stricmp(_str, WDOINS_OUT))
	{
		wdo_log_help(
			"在 " WDOINS_PTR " 指向窗口中输出文字。"
			,
			WDOINS_OUT "  <cx>  <cy>  \"text\""
			,
			"<cx>                    输出的 X 坐标，以窗口左上角为原点(0,0)。\n"
			"<cy>                    输出的 Y 坐标，以窗口左上角为原点(0,0)。\n"
			"text                    输出的文本。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			ALIASTYPE_STR_OUT        " 输出时的设置。默认设置使用指令 " WDOINS_ALIAS_GET " 获取。\n"
			,
			"需要输出文本的同时一并输出图片，请带入 " ALIASTYPE_STR_OUT " 类型的别名并设置。\n"
			,
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_ALIAS_GET "  -out  myout\n"
			"myout." WDOINS_ALIAS_SET "  -out  -tc  ff0000\n"
			"myout." WDOINS_OUT "  20  20  Hello\n"
			"\n"
			"// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
			"// 在当前游标所指向的窗口上，\n"
			"// 使用红色输出文本 \"Hello\"。\n"
		);
	}
	else if (!stricmp(_str, WDOINS_DRAW))
	{
		wdo_log_help(
			"在 " WDOINS_PTR " 指向窗口中绘图。\n"
			,
			WDOINS_DRAW "  [--RECT]     <cx>  <cy>  <cx2>  <cy2>\n"
			WDOINS_DRAW "  [--CIRCLE]   <cx>  <cy>  <radius>\n"
			WDOINS_DRAW "  [--ELLIPSE]  <cx>  <cy>  <cx2>  <cy2>\n"
			WDOINS_DRAW "  [--LINE]     <cx>  <cy>  <cx2>  <cy2>\n"
			WDOINS_DRAW "  [--LINE]     <cx>  <cy>\n"
			,
			"--rect                  画一个矩形。\n"
			"--circle                画一个正圆。\n"
			"--ellipse               画一个椭圆。\n"
			"--line                  画一条线段或一条折线。\n"
			"\n"
			"<cx>                    第一个点的 X 坐标。\n"
			"<cy>                    第一个点的 Y 坐标。\n"
			"<cx2>                   第二个点的 X 坐标。\n"
			"<cy2>                   第二个点的 Y 坐标。\n"
			"<radius>                圆的半径。此时 cx 和 cy 是圆心坐标。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			ALIASTYPE_STR_DRAW       " 绘画时的设置。默认设置使用指令 " WDOINS_ALIAS_GET " 获取。\n"
			,
			"--line 有两种类型，如果带入四个参数则只画一条线段；\n"
			"  例：\n"
			"         " WDOINS_DRAW "  --line  100  100  500  500\n"
			"只有两个参数画的是折线，需要配合 '{''}' 使用。\n"
			"  例：\n"
			"         " WDOINS_DRAW "  --line  {10 10, 200 200, 500 20}\n"
			"  或：\n"
			"         " WDOINS_DRAW "  --line\n"
			"         {\n"
			"             10   10\n"
			"             200  200\n"
			"             500  20\n"
			"         }\n"
			"\n"
			"以上示例会用线段连接(10,10)(200,200)(500,20)三个点，\n"
			"如果只有一个点，则没有任何作用。\n"
			"\n"
			"\n"
			"除了 [--line]，其他参数都额外支持例一。\n"
			"  例一：\n"
			"         " WDOINS_DRAW "\n"
			"         {\n"
			"             --circle  100  100  50\n"
			"             --circle  100  150  50\n"
			"             --circle  150  150  50\n"
			"         }\n"
			"  例二：\n"
			"         " WDOINS_DRAW "  --circle\n"
			"         {\n"
			"             100  100  50\n"
			"             100  150  50\n"
			"             150  150  50\n"
			"         }\n"
			"\n"
			,
			"." WDOINS_DRAW "  --circle  200  200  50"
		);
	}
	else if (!stricmp(_str, WDOINS_IMAGE))
	{
		wdo_log_help("在 " WDOINS_PTR " 指向的窗口中加载一张图像。"
			,
			WDOINS_IMAGE "  <x>  <y>  \"imagepath\""
			,
			"<x>                     设置 X 顶点；\n"
			"<y>                     设置 Y 顶点；\n"
			"imagepath               图像路径（.bmp）\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			ALIASTYPE_STR_PATH       " 如果路径缺省则检索此路径。\n"
			,
			"将按源图像大小输出。\n"
			,
			"." WDOINS_IMAGE "  0  0  C:\\image.bmp"
		);
	}
	else if (!stricmp(_str, WDOINS_IMAGES))
	{
		wdo_log_help("在 " WDOINS_PTR " 指向的窗口中加载一个目录中所有图像。"
			,
			WDOINS_IMAGES "  <x>  <y>  \"imagedir\""
			,
			"<x>                     设置 X 顶点；\n"
			"<y>                     设置 Y 顶点；\n"
			"imagedir                图像目录路径（会依次加载目录中的所有图像）\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			ALIASTYPE_STR_PATH       " 如果路径缺省则检索此路径。\n"
			,
			"将按源图像大小输出。\n"
			,
			"." WDOINS_IMAGES "  0  0  C:\\images"
		);
	}
	else if (!stricmp(_str, WDOINS_PROJECT))
	{
		wdo_log_help(
			"投影 " WDOINS_PTR " 指向窗口到指定窗口。"
			,
			WDOINS_PROJECT "  \"alias\"  <cx>  <cy>  <w>  <h>"
			,
			"alias                   一个 " ALIASTYPE_STR_WIN " 类型的别名，指定目标窗口。\n"
			"<cx>                    投影的目标点 X 轴坐标。缺省则为 0。\n"
			"<cy>                    投影的目标点 Y 轴坐标。缺省则为 0。\n"
			"<w>                     目标的宽度，如果缺省则和源窗口一样。\n"
			"<h>                     目标的高度，如果缺省则和源窗口一样。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"源窗口是当前 " WDOINS_PTR " 指向的窗口，\n"
			"将会截取源窗口整个客户区投影至目标窗口。只会截取一帧。\n"
			"提供 w 和 h 参数支持投影缩放。\n"
			,
			"." WDOINS_THREAD "\n"
			"{\n"
			"    ." WDOINS_LOOP "*" WDO_LOOPCOUNT_STR "\n"
			"    {\n"
			"        ." WDOINS_PTR "  -p  cur\n"
			"        ." WDOINS_PROJECT "  ..  0  100\n"
			"    }\n"
			"}\n"
			"\n"
			"// 以上命令可以实现命令 \"" WDOINS_BOX_INFO " -v cur\" 的投影效果。\n"
			"// 只不过投影对象是当前 " WDO_EXE_FILE " 窗口。\n"
			"// \"." WDOINS_PROJECT "  ..\" 中的 \"..\" 是一个默认的窗口别名，表示的是当前的 " WDO_EXE_FILE " 窗口。\n"
		);
	}
	else if (!stricmp(_str, WDOINS_PARENT))
	{
		wdo_log_help(
			"设置 " WDOINS_PTR " 指向窗口的新父窗口。"
			,
			WDOINS_PARENT "  \"alias\""
			,
			"alias                   新父窗口的别名，如果缺省则没有父窗口。"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"指令 " WDOINS_LIST " 可以遍历父窗口的所有子窗口。详见 " WDOINS_HELP " " WDOINS_LIST
			,
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_PARENT "  ..\n"
			"\n"
			"// 设置当前游标所在的窗口为当前 " WDO_EXE_FILE " 窗口的子窗口。\n"
			"// \"..\" 是一个默认的别名，指向当前 " WDO_EXE_FILE " 窗口。\n"
		);
	}
	else if (!stricmp(_str, WDOINS_CHILD))
	{
		wdo_log_help(
			"设置 " WDOINS_PTR " 指向窗口的子窗口。"
			,
			WDOINS_CHILD "  [-ADD]  \"alias\"  <cx>  <cy>\n"
			WDOINS_CHILD "  [-DEL]  \"alias\"\n"
			,
			"-ADD                    添加一个子窗口。\n"
			"-DEL                    删除一个子窗口。\n"
			"\n"
			"alias                   子窗口的别名。\n"
			"<cx>                    成为子窗口后的新 X 轴坐标，缺省则为 0。\n"
			"<cy>                    成为子窗口后的新 Y 轴坐标，缺省则为 0。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"如果不带参数则删除所有子窗口。\n"
			"删除子窗口后子窗口并不会关闭，只是成为了父窗口。\n"
			"\n"
			"使用指令 " WDOINS_LIST " 可遍历子窗口。\n"
			,
			"." WDOINS_PTR "  -c  MyWinClass\n"
			"."	WDOINS_ALIAS_APPLY "  -win  Win_1\n"
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_ALIAS_APPLY "  -win  Win_2\n"
			"Win_1." WDOINS_CHILD "  -add  Win_2\n"
			"\n"
			"// Win_1 是一个类名为 MyWinClass 的窗口；\n"
			"// Win_2 是当前游标指向的窗口；\n"
			"// 为 Win_1 添加子窗口 Win_2。\n"
		);
	}
	else if (!stricmp(_str, WDOINS_COPY))
	{
		wdo_log_help(
			"复制 " WDOINS_PTR " 指向窗口的内容到其他指定的窗口。"
			,
			WDOINS_COPY "  \"alias\"  [-NEW class | -TITLE | -STYLE | -SHOW | -TRANSP | -IMAGE]\n"
			WDOINS_COPY "  \"alias\"  [-W | -H | -WH | -X | -Y | -XY]\n"
			WDOINS_COPY "  (null)\n"
			,
			"alias                   目标窗口的别名。\n"
			"class                   指定新窗口的类名。缺省则自动选择。\n"
			"\n"
			"-NEW                    新建一个复制的窗口，忽略 alias 参数。\n"
			"-TITLE                  仅复制标题。\n"
			"-STYLE                  仅复制风格。\n"
			"-SHOW                   仅复制显示模式（显示或隐藏窗口）。\n"
			"-TRANSP                 仅复制透明度和擦除颜色（如果有）。\n"
			"-IMAGE                  仅复制图像（一帧）。\n"
			"-W                      仅复制宽度。\n"
			"-H                      仅复制高度。\n"
			"-WH                     仅复制宽和高。\n"
			"-X                      仅复制 X 轴坐标。\n"
			"-Y                      仅复制 Y 轴坐标。\n"
			"-XY                     仅复制坐标。\n"
			"\n"
			"(null)                  创建一个复制窗口，使用默认的新类名。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"部分窗口暂不支持图像映射。\n"
			,
			"." WDOINS_PTR "  -c  Win_1\n"
			"." WDOINS_ALIAS_APPLY "  -win  Win1_alias\n"
			"." WDOINS_PTR "  -c  Win_2\n"
			"." WDOINS_COPY "  Win1_alias  -title\n"
			"\n"
			"//  将类名为 Win_1 的窗口申请别名为 Win1_alias；\n"
			"//  将类名为 Win_2 的窗口的标题复制到 Win1_alias 窗口。\n"
		);
	}
	else if (!stricmp(_str, WDOINS_MOVE))
	{
		wdo_log_help(
			"移动 " WDOINS_PTR " 指向窗口的坐标。"
			,
			WDOINS_MOVE "  <xaxis>  <yaxis>"
			,
			"<xaxis>                 沿 X 轴移动的像素点。可以使用指令 " WDOINS_MOVEX " 单独移动。\n"
			"<yaxis>                 沿 Y 轴移动的像素点。可以使用指令 " WDOINS_MOVEY " 单独移动。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"如果是父窗口，则以屏幕左上角顶点为原点（0，0）；\n"
			"如果是子窗口，则以父窗口左上角顶点为原点（0，0）。\n"
			"\n"
			"如果 yaxis 缺省则默认使用数值 0。数值可以为负数。\n"
			"如果只是移动 X 轴，则指令 " WDOINS_MOVE " 和 " WDOINS_MOVEX " 无区别。\n"
			"\n"
			"如果需要连续调用则可以使用大括号 '{''}' 传递数组，使用换行或 ',' 分隔。\n"
			"  例：\n"
			"          " WDOINS_MOVE " {1 2, 3 4}\n"
			"  同等于：\n"
			"          " WDOINS_MOVE " 1 2\n"
			"          " WDOINS_MOVE " 3 4\n"
			"\n"
			"如果连续调用相同指令则可以在指令后加上'*'和执行次数。\n"
			"  例：\n"
			"         " WDOINS_MOVE "*3 5 2\n"
			"  等同于：\n"
			"         " WDOINS_MOVE " 5 2\n"
			"         " WDOINS_MOVE " 5 2\n"
			"         " WDOINS_MOVE " 5 2\n"
			"\n"
			"如果需要无限执行则可以使用 " WDOINS_MOVE "*" WDO_LOOPCOUNT_STR "  5 2\n"
			"将无限执行 " WDOINS_MOVE " 5 2，这种方式适用于所有指令。\n"
			"\n"
			"以上两种方式可同时出现。\n"
			"  例：\n"
			"         " WDOINS_MOVE "*2 {1 2, 3 4}\n"
			"  等同于：\n"
			"         " WDOINS_MOVE " 1 2\n"
			"         " WDOINS_MOVE " 3 4\n"
			"         " WDOINS_MOVE " 1 2\n"
			"         " WDOINS_MOVE " 3 4\n"
			"\n"
			"注意这是一条完整的指令，不应有空格，否则将作为参数传入。\n"
			"执行次数如果为负数则取其绝对值，次数为 0 或 1 或不填的效果是一样的。\n"
			"\n"
			"\"{ }\" 有两种类型，一种传递数组，一种传递字符串组。\n"
			"根据不同的指令而定。\n"
			"  数组：\n"
			"         >> 当指令的所有参数均为数字时使用；\n"
			"         >> 使用 ',' 和 '换行' 分割；\n"
			"  字符串组：\n"
			"         >> 当指令中需要除了数字之外的字符出现时使用；\n"
			"         >> 使用 '换行' 分割；\n"
			"\n"
			"如果不确定使用哪种，可以统一使用换行分割，\n"
			"\"{ }\" 支持多重内嵌，但必须成对出现。\n"
			,
			"." WDOINS_MOVE "  10  20"
		);
	}
	else if (!stricmp(_str, WDOINS_MOVEX))
	{
		wdo_log_help(
			"沿 X 轴移动 " WDOINS_PTR " 指向窗口坐标。"
			,
			WDOINS_MOVEX "  <xaxis>"
			,
			"<xaxis>                 沿 X 轴移动的像素点。"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			NULL
			,
			"." WDOINS_MOVEX "  10"
		);
	}
	else if (!stricmp(_str, WDOINS_MOVEY))
	{
		wdo_log_help(
			"沿 Y 轴移动 " WDOINS_PTR " 指向窗口坐标。"
			,
			WDOINS_MOVEY "  <yaxis>"
			,
			"<yaxis>                 沿 Y 轴移动的像素点。"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			NULL
			,
			"." WDOINS_MOVEY "  10"
		);
	}
	else if (!stricmp(_str, WDOINS_POS))
	{
		wdo_log_help(
			"更改 " WDOINS_PTR " 指向窗口的左上角顶点坐标位置。"
			,
			WDOINS_POS "  <xaxis>  <yaxis>"
			,
			"<xaxis>                 新的 X 轴坐标。可以使用指令 " WDOINS_POSX " 单独设置。\n"
			"<yaxis>                 新的 Y 轴坐标。可以使用指令 " WDOINS_POSY " 单独设置。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"如果是父窗口，则以屏幕左上角顶点为原点（0，0）；\n"
			"如果是子窗口，则以父窗口左上角顶点为原点（0，0）。\n"
			"\n"
			"需要缓慢移动请使用指令 " WDOINS_DRIFT "，详见 " WDOINS_HELP " " WDOINS_DRIFT "。\n"
			,
			"." WDOINS_POS "  200  200"
		);
	}
	else if (!stricmp(_str, WDOINS_POSX))
	{
		wdo_log_help(
			"更改 " WDOINS_PTR " 指向窗口的左上角顶点坐标 X 位置。"
			,
			WDOINS_POSX "  <xaxis>"
			,
			"<xaxis>                 新的 X 轴坐标。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			NULL
			,
			"." WDOINS_POSX "  200"
		);
	}
	else if (!stricmp(_str, WDOINS_POSY))
	{
		wdo_log_help(
			"更改 " WDOINS_PTR " 指向窗口的左上角顶点坐标 Y 位置。"
			,
			WDOINS_POSY "  <yaxis>"
			,
			"<yaxis>                 新的 Y 轴坐标。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			NULL
			,
			"." WDOINS_POSY "  200"
		);
	}
	else if (!stricmp(_str, WDOINS_RPOS))
	{
		wdo_log_help("随机设置 " WDOINS_PTR " 指向窗口坐标。", WDOINS_RPOS "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。\n", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_ZOOM))
	{
		wdo_log_help(
			"增大或缩小 " WDOINS_PTR " 指向窗口的大小。"
			,
			WDOINS_ZOOM "  <width>  <height>"
			,
			"<width>                 要增大的窗口宽度，可以为负数。\n"
			"<height>                要增大的窗口高度，可以为负数。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			NULL
			,
			"." WDOINS_ZOOM "  20  20"
		);

	}
	else if (!stricmp(_str, WDOINS_ZOOMX))
	{
		wdo_log_help(
			"增大或缩小 " WDOINS_PTR " 指向窗口的宽度。"
			,
			WDOINS_ZOOMX "  <width>"
			,
			"<width>                 要增大的窗口宽度，单位为像素。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			NULL
			,
			"." WDOINS_ZOOMX "  20"
		);
	}
	else if (!stricmp(_str, WDOINS_ZOOMY))
	{
		wdo_log_help(
			"增大或缩小 " WDOINS_PTR " 指向窗口的高度。"
			,
			WDOINS_ZOOMY "  <height>"
			,
			"<height>                要增大的窗口高度，单位为像素。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			NULL
			,
			"." WDOINS_ZOOMY "  20"
		);
	}
	else if (!stricmp(_str, WDOINS_SIZEX))
	{
		wdo_log_help(
			"设置 " WDOINS_PTR " 指向窗口的新宽度。"
			,
			WDOINS_SIZEX "  <width>"
			,
			"<width>                 窗口的新宽度。单位为像素。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			NULL
			,
			"." WDOINS_SIZEX "  200"
		);
	}
	else if (!stricmp(_str, WDOINS_SIZEY))
	{
		wdo_log_help(
			"设置 " WDOINS_PTR " 指向窗口的新高度。"
			,
			WDOINS_SIZEY "  <height>"
			,
			"<height>                窗口的新高度。单位为像素。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			NULL
			,
			"." WDOINS_SIZEY "  200"
		);
	}
	else if (!stricmp(_str, WDOINS_SHOW))
	{
		wdo_log_help(
			"显示或隐藏 " WDOINS_PTR " 指向窗口。"
			,
			WDOINS_SHOW "  [on/off]"
			,
			"[on]                    显示。\n"
			"[off]                   隐藏。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"使用指令 " WDOINS_STYLE " 以显示或隐藏窗口部分控件，详见 " WDOINS_HELP " " WDOINS_STYLE "。"
			,
			"." WDOINS_SHOW "  off"
		);

	}
	else if (!stricmp(_str, WDOINS_STYLE))
	{
		wdo_log_help(
			"设置 " WDOINS_PTR " 指向窗口风格。"
			,
			WDOINS_STYLE "  [-TITLE | -MENU | -MAX | -MIN | -SB | -R | -HR | -VR | -CLOSE]  [on/off]"
			,
			"-TITLE                  窗口具有标题栏。\n"
			"-MENU                   窗口具有菜单栏。\n"
			"-MAX                    窗口右上角具有最大化按钮。需要菜单栏。\n"
			"-MIN                    窗口右上角具有最小化按钮。需要菜单栏。\n"
			"-SB                     窗口具有大小调整边框，可直接用游标调整窗口大小。\n"
			"-R                      窗口具有水平和垂直滚动条。\n"
			"-HR                     窗口具有水平滚动条。\n"
			"-VR                     窗口具有垂直滚动条。\n"
			"-CLOSE                  窗口右上角具有关闭按钮。需要菜单栏。\n"
			"\n"
			"[on]                    开启。\n"
			"[off]                   关闭。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"该指令改变的仅是窗口风格。对于部分窗口，以实际效果为准。\n"
			"\n"
			"为没有开启滚动条的窗口添加滚动条，窗口可能并不会处理滚动条消息。\n"
			"\n"
			"开启菜单栏的同时会开启标题栏。\n"
			,
			"." WDOINS_STYLE "  -close  off"
		);
	}
	else if (!stricmp(_str, WDOINS_MAX))
	{
		wdo_log_help("将 " WDOINS_PTR " 指向窗口最大化，相当于按下窗口右上角最大化按钮，可以通过指令 " WDOINS_RESTORE " 还原。\n"
			, WDOINS_MAX "  (null)"
			, NULL
			, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。\n"
			, NULL
			, NULL);
	}
	else if (!stricmp(_str, WDOINS_MIN))
	{
		wdo_log_help("将 " WDOINS_PTR " 指向窗口最小化，相当于按下窗口右上角最小化按钮。"
			, WDOINS_MIN "  (null)"
			, NULL
			, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。\n"
			, NULL
			, NULL);
	}
	else if (!stricmp(_str, WDOINS_RESTORE))
	{
		wdo_log_help("将处于最大化的 " WDOINS_PTR " 指向窗口还原原来的大小，相当于按下窗口右上角还原按钮。"
			, WDOINS_RESTORE "  (null)"
			, NULL
			, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。\n"
			, NULL
			, NULL);
	}
	else if (!stricmp(_str, WDOINS_REPAINT))
	{
		wdo_log_help("重绘 " WDOINS_PTR " 指向窗口。向其发送 " lr_TO_STR(WM_PAINT) " 消息。"
			, WDOINS_REPAINT "  (null)"
			, NULL
			, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。\n"
			, NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_FLASH))
	{
		wdo_log_help("在任务栏闪烁 " WDOINS_PTR " 指向窗口。"
			,WDOINS_FLASH "  (null)"
			,NULL
			,ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。\n"
			, NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_STROKE))
	{
		wdo_log_help(
			"描边 " WDOINS_PTR " 指向窗口。"
			,
			WDOINS_STROKE "  <color>  <width>\n"
			WDOINS_STROKE "  (null)\n"
			,
			"<color>                 六位十六进制的 RGB 颜色。\n"
			"<width>                 描边宽度。\n"
			"\n"
			"(null)                  使用别名数据。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			ALIASTYPE_STR_DRAW       " 如果不带参数，则从该别名中提取数据。\n"
			,
			"只会从 " ALIASTYPE_STR_DRAW " 中提取描边颜色与描边宽度，忽略其他数据。\n"
			,
			"." WDOINS_STROKE "  ff0000  5"
		);
	}
	else if (!stricmp(_str, WDOINS_FILL))
	{
		wdo_log_help(
			"填充 " WDOINS_PTR " 指向窗口。"
			,
			WDOINS_FILL "  <color>\n"
			WDOINS_FILL "  (null)\n"
			,
			"<color>                 六位十六进制的 RGB 颜色。\n"
			"(null)                  使用别名数据。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			ALIASTYPE_STR_DRAW       " 如果不带参数，则从该别名中提取数据。\n"
			,
			"只会从 " ALIASTYPE_STR_DRAW " 中提取填充颜色，忽略其他数据。\n"
			,
			"." WDOINS_FILL "  ff0000"
		);
	}
	else if (!stricmp(_str, WDOINS_DRIFT))
	{
		wdo_log_help(
			"使 " WDOINS_PTR " 指向窗口从当前坐标漂移到指定坐标。"
			,
			WDOINS_DRIFT "  <x>  <y>  <speed>"
			,
			"<x>                     目标点 X。缺省则为 0。\n"
			"<y>                     目标点 Y。缺省则为 0。\n"
			"<speed>                 运动速度[1-100]。缺省则为 100。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"窗口到达目标点前除了退出 " WDO_EXE_FILE " 之外，窗口无法停下运动。\n"
			"如果想同时给窗口设置好几个目标点，让窗口依次进行漂移运动，请使用 '{''}' 传参。\n"
			"  例：\n"
			"          " WDOINS_DRIFT " {10 10 95, 100 200 95, 70 70 95}\n"
			"  或：\n"
			"          " WDOINS_DRIFT "\n"
			"          {\n"
			"               10  10  95\n"
			"               100 200 95\n"
			"               70  70  95\n"
			"          }\n"
			"\n"
			"以上两种示例都可以让窗口从当前位置以 5ms 运动一次的速度，\n"
			"先漂移到 (X- 10,Y- 10)，在从 (X- 10,Y- 10) 到 (X- 100,Y- 200)，\n"
			"最后到达 (X- 70,Y- 70) 停下。\n"
			"\n"
			"因为该指令会多开一条线程，所以请不要使用以下类型的命令，\n"
			"否则窗口可能永远无法到达目标点，从而陷入死循环或出现未知问题。\n"
			"  例：\n"
			"          " WDOINS_DRIFT " 10  10  95\n"
			"          " WDOINS_DRIFT " 100 200 95\n"
			"          " WDOINS_DRIFT " 70  70  95\n"
			,
			"." WDOINS_DRIFT "  200  200  90"
		);
	}
	else if (!stricmp(_str, WDOINS_FOLLOW))
	{
		wdo_log_help(
			"使 " WDOINS_PTR " 指向窗口跟随游标移动。"
			,
			WDOINS_FOLLOW "  <x>  <y>\n"
			WDOINS_FOLLOW "  [-C]\n"
			,
			"<x>                     窗口偏移游标 X 轴距离的像素点。缺省则为 0。\n"
			"<y>                     窗口偏移游标 Y 轴距离的像素点。缺省则为 0。\n"
			"\n"
			"-C                      取消跟随。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"在取消跟随前，对同一个窗口进行二次跟随，\n"
			"只会更新其 xy 参数值，不会另开一条线程。\n"
			,
			"." WDOINS_FOLLOW "  20  20"
		);
	}
	else if (!stricmp(_str, WDOINS_WIGGLE))
	{
		wdo_log_help(
			"使 " WDOINS_PTR " 指向窗口进行震动。"
			,
			WDOINS_WIGGLE "  <range>  <frequency>\n"
			WDOINS_WIGGLE "  [-C]\n"
			,
			"<range>                 震动的范围(方圆半径)。\n"
			"<frequency>             震动的频率(每运动一次将等待的毫秒数)。\n"
			"\n"
			"-C                      关闭震动。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"在取消震动前，对同一个窗口进行二次震动，\n"
			"只会更新其 range 和 frequency 参数值，不会另开一条线程。\n"
			,
			"." WDOINS_WIGGLE "  5  10"
		);
	}
	else if (!stricmp(_str, WDOINS_ASIDE))
	{
		wdo_log_help(
			"使 " WDOINS_PTR " 指向窗口靠边收入或弹出。"
			,
			WDOINS_ASIDE "  <speed>  <rx>  <ry>\n"
			WDOINS_ASIDE "  [-C]\n"
			,
			"<speed>                 移动速度[1-10]。缺省则为 1。\n"
			"<rx>                    X 轴保留，收起后露出在外的宽度。缺省则为 0。\n"
			"<ry>                    Y 轴保留，收起后露出在外的高度。缺省则为 0。\n"
			"\n"
			"-C                      关闭。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"在取消靠边前，对同一个窗口进行二次靠边，\n"
			"只会更新其 speed 和 rx 和 ry 参数值，不会另开一条线程。\n"
			"\n"
			"需要将窗口脱到屏幕边缘才可查看具体效果。\n"
			,
			"." WDOINS_ASIDE "  10  20  20"
		);
	}
	else if (!stricmp(_str, WDOINS_PUSH))
	{
		wdo_log_help(
			"使 " WDOINS_PTR " 指向窗口可以被游标推动。"
			,
			WDOINS_PUSH "  (null)\n"
			WDOINS_PUSH "  [-C]\n"
			,
			"(null)                  开始推动。\n"
			"-C                      关闭。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"在取消推动前，对同一个窗口进行二次推动，则该指令没有任何影响。\n"
			,
			"." WDOINS_PUSH
		);
	}
	else if (!stricmp(_str, WDOINS_COLLIDE))
	{
		wdo_log_help(
			"使 " WDOINS_PTR " 指向窗口和游标碰撞时发生事件。"
			,
			WDOINS_COLLIDE "  [-C]\n"
			WDOINS_COLLIDE "  [-MW | -MC | -RM]  <distance>  <backpixel>\n"
			,
			"-C                      取消碰撞事件。\n"
			"\n"
			"-MW                     碰撞后弹开窗口；\n"
			"-MC                     碰撞后弹开游标；\n"
			"-RM                     碰撞后弹开窗口和游标。\n"
			"\n"
			"<distance>              弹开的距离（单位：像素）。\n"
			"<backpixel>             回弹的像素点，窗口碰到屏幕边缘时将回弹。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"同一个窗口不可设置多个碰撞事件；\n"
			"多次设置时将覆盖上一次的设置。\n"
			,
			"." WDOINS_COLLIDE "  -mw  100 20"
		);
	}
	else if (!stricmp(_str, WDOINS_LOCKPAINT))
	{
		wdo_log_help("锁定 " WDOINS_PTR " 指向窗口，使其禁止绘图。"
			, WDOINS_LOCKPAINT "  (null)"
			, NULL
			, ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			, NULL
			, NULL);
	}
	else if (!stricmp(_str, WDOINS_CLOSE))
	{
		wdo_log_help(
			"关闭 " WDOINS_PTR " 指向窗口或关闭窗口所属进程。"
			,
			WDOINS_CLOSE "  (null)\n"
			WDOINS_CLOSE "  [-C | -D]\n"
			,
			"(null)                  关闭窗口，通过向窗口发送 " lr_TO_STR(WM_CLOSE) "(0x0010) 消息。\n"
			"-C                      关闭窗口所属进程。\n"
			"-D                      销毁窗口。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			"关闭窗口：\n"
			"        - 相当于按下窗口右上角关闭按钮；\n"
			"        - 窗口可以拒绝关闭，或询问是否保存尚未保存的文件；\n"
			"        - 窗口可能不会关闭，或者不会立即关闭；\n"
			"        - 窗口关闭后，程序可能还正在运行；\n"
			"关闭进程：\n"
			"        - 直接关闭窗口所属的进程；\n"
			"        - 同进程的窗口都将关闭；\n"
			"        - 不会存在后台程序；\n"
			"        - 执行该操作需要的权限不能低于对方；\n"
			"销毁窗口：\n"
			"        - 不经过 " lr_TO_STR(WM_CLOSE) "(0x0010)；\n"
			"        - 直接销毁对方窗口并发送 " lr_TO_STR(WM_DESTROY) "(0x0002) 消息；\n"
			,
			"." WDOINS_CLOSE "  -c"
		);
	}

	/***************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 游标类 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	****************************************************************************************/

	else if (!stricmp(_str, WDOINS_CUR_MOVE))
	{
		wdo_log_help(
			"移动游标。"
			,
			WDOINS_CUR_MOVE "  <x>  <y>\n"
			WDOINS_CUR_MOVE "  (null)\n"
			,
			"<x>                     要移动的 X 轴数值。可以是负数。\n"
			"<y>                     要移动的 Y 轴数值。可以是负数。\n"
			"\n"
			"(null)                  显示当前游标坐标。\n"
			,
			NULL
			,
			NULL
			,
			"." WDOINS_CUR_MOVE "  20  20"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_MOVEX))
	{
		wdo_log_help(
			"移动游标 X 轴坐标。"
			,
			WDOINS_CUR_MOVEX "  <x>"
			,
			"<x>                     要移动的 X 轴数值。\n"
			,
			NULL
			,
			NULL
			,
			"." WDOINS_CUR_MOVEX "  20"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_MOVEY))
	{
		wdo_log_help(
			"移动游标 Y 轴坐标。"
			,
			WDOINS_CUR_MOVEY "  <y>"
			,
			"<y>                     要移动的 Y 轴数值。\n"
			,
			NULL
			,
			NULL
			,
			"." WDOINS_CUR_MOVEY "  20"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_POS))
	{
		wdo_log_help(
			"设置游标坐标。"
			,
			WDOINS_CUR_POS "  <x>  <y>"
			,
			"<x>                     新的 X 轴坐标。缺省则为 0。\n"
			"<y>                     新的 Y 轴坐标。缺省则为 0。\n"
			,
			NULL
			,
			"需要缓慢移动请使用指令 " WDOINS_CUR_DRIFT "，详见 " WDOINS_HELP " " WDOINS_CUR_DRIFT "。\n"
			,
			"." WDOINS_CUR_POS "  200  200"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_POSX))
	{
		wdo_log_help(
			"设置游标 X 轴坐标。"
			,
			WDOINS_CUR_POSX "  <x>"
			,
			"<x>                     新的 X 轴坐标。\n"
			,
			NULL
			,
			NULL
			,
			"." WDOINS_CUR_POSX "  200"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_POSY))
	{
		wdo_log_help(
			"设置游标 Y 轴坐标。"
			,
			WDOINS_CUR_POSY "  <y>"
			,
			"<y>                     新的 Y 轴坐标。\n"
			,
			NULL
			,
			NULL
			,
			"." WDOINS_CUR_POSY "  200"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_RPOS))
	{
		wdo_log_help("随机设置游标坐标。", WDOINS_CUR_RPOS "  (null)", NULL, NULL, NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_CUR_CPOS))
	{
		wdo_log_help(
			"以 " WDOINS_PTR " 指向窗口左上角为原点(0,0)，设置游标坐标。"
			,
			WDOINS_CUR_CPOS "  <cx>  <cy>"
			,
			"<cx>                    新的 X 轴坐标。缺省则为 0。\n"
			"<cy>                    新的 Y 轴坐标。缺省则为 0。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。\n"
			,
			NULL
			,
			"." WDOINS_CUR_CPOS "  20  20"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_CRPOS))
	{
		wdo_log_help("以 " WDOINS_PTR " 指向窗口为区域，随机设置游标坐标。", WDOINS_CUR_CRPOS "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " 临时指向。\n", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_CUR_KEY))
	{
		wdo_log_help(
			"模拟按下游标键。"
			,
			WDOINS_CUR_KEY "  [--LEFT | --RIGHT | --MIDDLE]\n"
			WDOINS_CUR_KEY "  [--LEFT_TWICE | --RIGHT_TWICE | --MIDDLE_TWICE]\n"
			WDOINS_CUR_KEY "  [--ROLLER_UPPER | --ROOLER_DOWN | --ROLLER_LEFT | --ROLLER_RIGHT]\n"
			,
			"--left                  单击左键。\n"
			"--right                 单击右键。\n"
			"--middle                单击中键。\n"
			"\n"
			"--left_twice            双击左键。\n"
			"--right_twice           双击右键。\n"
			"--middle_twice          双击中键。\n"
			"\n"
			"--roller_upper          滚轮上划。\n"
			"--roller_down           滚轮下划。\n"
			"--roller_left           滚轮左划。\n"
			"--roller_right          滚轮右划。\n"
			,
			NULL
			,
			"该指令是通过发送窗口消息的方式达到模拟游标按键。\n"
			"但是并不是发送到 " WDOINS_PTR " 指向的窗口，而是当前游标所在的窗口。\n"
			"\n"
			"以下两条指令都能双击左键，但是是有区别的，请尽量选择后者。\n"
			"  例：\n"
			"         " WDOINS_CUR_KEY "*2  --left\n"
			"         " WDOINS_CUR_KEY "  --left_twice\n"
			"\n"
			"滚轮一次只会滑动一次，多次滑动可以使用以下指令让滚轮向下滑动 10 次。\n"
			"  例：\n"
			"         " WDOINS_CUR_KEY "*10  --roller_down\n"
			,
			"." WDOINS_CUR_KEY "  --left"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_DRIFT))
	{
		wdo_log_help(
			"使游标进行漂移运动。"
			,
			WDOINS_CUR_DRIFT "  <x>  <y>  <speed>"
			,
			"<x>                     目标点 X。\n"
			"<y>                     目标点 Y。\n"
			"<speed>                 运动速度[1-100]。缺省则为 100。\n"
			,
			NULL
			,
			"详见 " WDOINS_HELP " " WDOINS_CUR_DRIFT "。"
			,
			"." WDOINS_CUR_DRIFT "  200  200  90"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_WIGGLE))
	{
		wdo_log_help(
			"使游标进行震动。"
			,
			WDOINS_CUR_WIGGLE "  <range>  <frequency>\n"
			WDOINS_CUR_WIGGLE "  [-C]\n"
			,
			"<range>                 震动的范围(方圆半径)。\n"
			"<frequency>             震动的频率(每运动一次将等待的毫秒数)。\n"
			"\n"
			"-C                      关闭震动。\n"
			,
			NULL
			,
			"在取消震动前，对同一个窗口进行二次震动，\n"
			"只会更新其 range 和 frequency 参数值，不会另开一条线程。\n"
			,
			"." WDOINS_CUR_WIGGLE "  5  10"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_RECT))
	{
		wdo_log_help(
			"限制游标移动区域在一个矩形内。"
			,
			WDOINS_CUR_RECT "  [-WIN]\n"
			WDOINS_CUR_RECT "  <x>  <y>  <w>  <h>"
			,
			"-WIN                    矩形为当前窗口矩形。\n"
			"\n"
			"<x>                     左上角 X 轴坐标。\n"
			"<y>                     左上角 Y 轴坐标。\n"
			"<w>                     矩形宽度。\n"
			"<h>                     矩形高度。\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " 临时指向。如果携带 [-WIN] 参数时生效。\n"
			,
			"如果不带参数则解除限制。"
			,
			"." WDOINS_CUR_RECT "  200  200  500  500"
		);
	}

	/*****************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 新建窗口 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	******************************************************************************************/
	
	else if (!stricmp(_str, WDOINS_BOX_BLANK))
	{
		wdo_log_help(
			"新建一个空白框，不会重绘。"
			,
			WDOINS_BOX_BLANK "  <width>  <height>\n"
			,
			"<width>                 新窗口宽度。\n"
			"<height>                新窗口高度。\n"
			,
			ALIASTYPE_STR_BOX      " 新建窗口采用的窗口（框）设置。\n"
			,
			"可使用 {} 带入一个数组，使用 ',' 或 换行分割，连续创建多个不同大小的窗口。\n"
			,
			"." WDOINS_BOX_BLANK "  520  520"
		);
	}

	else if (!stricmp(_str, WDOINS_BOX_TEXT))
	{
		wdo_log_help(
			"新建一个文本框并显示文字。"
			,
			WDOINS_BOX_TEXT "  {text}"
			,
			"text                    文本框中的文本（可带换行）。\n"
			,
			ALIASTYPE_STR_BOX      " 新建窗口采用的窗口（框）设置。\n"
			,
			"窗口的初始顶点为当前的游标坐标。\n"
			"\n"
			"如果要中途更改窗口设置，请重新带入新别名。详见 " WDOINS_HELP " " WDOINS_ALIAS_GET " 和 " WDOINS_HELP " " WDOINS_ALIAS_SET " 和 " WDOINS_HELP " " WDOINS_ALIAS_SETUP ".\n"
			"\n"
			"此指令会更改当前 " WDOINS_PTR " 的窗口指向，会更改为新创建的文本框窗口。\n"
			"如果不需要请使用指令 " WDOINS_UPDATEPTR "，将其参数设置为 off 即可。详见 " WDOINS_HELP " " WDOINS_UPDATEPTR ".\n"
			"\n"
			"更改窗口文本请参考指令 " WDOINS_HELP " " WDOINS_BOX_SET_TEXT ".\n"
			,
			"." WDOINS_BOX_TEXT "  Hello"
		);
	}
	else if (!stricmp(_str, WDOINS_BOX_INFO))
	{
		wdo_log_help(
			"新建一个信息框以查看指定窗口的信息。"
			,
			WDOINS_BOX_INFO "  [-S | -V]  [ct/par/top/desk/cur/cp]"
			,
			"-S                      静态信息框。不会更新窗口ID。\n"
			"-V                      动态信息框。持续更新。\n"
			"\n"
			"[ct]                    " WDOINS_PTR " 当前指向的窗口。\n"
			"[par]                   " WDOINS_PTR " 当前指向窗口的父窗口。\n"
			"[top]                   当前前台窗口。\n"
			"[desk]                  当前桌面窗口。\n"
			"[cur]                   当前游标指向的窗口。\n"
			"[cp]                    当前游标指向的窗口的父窗口。\n"
			,
			ALIASTYPE_STR_WIN       " " WDOINS_PTR " 临时指向。\n"
			ALIASTYPE_STR_BOX       " 新建窗口采用的窗口（框）设置。\n"
			,
			"窗口的初始顶点为当前的游标坐标。\n"
			"\n"
			"此指令会更改当前 " WDOINS_PTR " 的窗口指向，会更改为新创建的信息框窗口。\n"
			"如果不需要请使用指令 " WDOINS_UPDATEPTR "，将其参数设置为 [off] 即可。详见 " WDOINS_HELP " " WDOINS_UPDATEPTR ".\n"
			"\n"
			"可以使用指令 " WDOINS_PROJECT " 实现信息框的窗口投影效果。详见 " WDOINS_HELP " " WDOINS_PROJECT ".\n"
			,
			"." WDOINS_BOX_INFO "  -v  cur"
		);
	}
	else if (!stricmp(_str, WDOINS_BOX_LOG))
	{
		wdo_log_help(
			"新建一个日志框并显示日志。\n"
			,
			WDOINS_BOX_LOG "  {text}"
			,
			"text                    日志文本（可带换行）。\n"
			,
			ALIASTYPE_STR_BOX       " 新建窗口采用的窗口（框）设置。\n"
			,
			"窗口的初始顶点为当前的游标坐标。\n"
			"\n"
			"如果要中途更改窗口设置，请重新带入新别名。详见 " WDOINS_HELP " " WDOINS_ALIAS_GET " 和 " WDOINS_HELP " " WDOINS_ALIAS_SET " 和 " WDOINS_HELP " " WDOINS_ALIAS_SETUP ".\n"
			"\n"
			"此指令会更改当前 " WDOINS_PTR " 的窗口指向，会更改为新创建的文本框窗口。\n"
			"如果不需要请使用指令 " WDOINS_UPDATEPTR "，将其参数设置为 off 即可。详见 " WDOINS_HELP " " WDOINS_UPDATEPTR ".\n"
			"\n"
			"更改窗口文本请参考指令 " WDOINS_HELP " " WDOINS_BOX_SET_TEXT ".\n"
			,
			"." WDOINS_BOX_TEXT "  Hello"
		);
	}
	else if (!stricmp(_str, WDOINS_BOX_IMAGE))
	{
		wdo_log_help(
			"新建一个图片框并显示图片。"
			,
			WDOINS_BOX_IMAGE "  \"path\""
			,
			"path                    图片的路径。缺省则检索带入的路径别名。"
			,
			ALIASTYPE_STR_PATH      " 如果 path 缺省，则使用该路径代替。\n"
			ALIASTYPE_STR_BOX       " 新建窗口采用的窗口（框）设置。\n"
			,
			"窗口的初始顶点为当前的游标坐标。\n"
			"\n"
			"此指令会更改当前 " WDOINS_PTR " 的窗口指向，会更改为新创建的图片框窗口。\n"
			"如果不需要请使用指令 " WDOINS_UPDATEPTR "，将其参数设置为 off 即可。详见 " WDOINS_HELP " " WDOINS_UPDATEPTR ".\n"
			"\n"
			"该指令使用 " ALIASTYPE_STR_BOX " 类型的别名，但是他只使用其中的窗口类型数据。\n"
			"\n"
			"关于 " WDOINS_BOX_IMAGE " 框：\n"
			"\t1:> 右击该窗口任意位置可重绘刷新该窗口；\n"
			"\t2:> 左击该窗口任意位置可绘制折线；\n"
			"\t3:> 具有键盘焦点时，可输入任意字符在当前游标点位置；\n"
			"\t4:> 该窗口不会加载中文输入法界面，但可正常输入中文；\n"
			,
			"." WDOINS_BOX_IMAGE "  c:\\lr.bmp"
		);
	}
	else if (!stricmp(_str, WDOINS_BOX_BLUE))
	{
		wdo_log_help(
			"新建一个蓝屏框并禁止关闭。"
			,
			WDOINS_BOX_BLUE "  \"text\""
			,
			"text                    蓝屏框中央使用的文本。"
			,
			ALIASTYPE_STR_BOX      " 新建窗口采用的窗口（框）设置。\n"
			,
			"此指令会更改当前 " WDOINS_PTR " 的窗口指向，会更改为新创建的蓝屏框窗口。\n"
			"如果不需要请使用指令 " WDOINS_UPDATEPTR "，将其参数设置为 off 即可。详见 " WDOINS_HELP " " WDOINS_UPDATEPTR ".\n"
			"\n"
			"该指令是一个伪蓝屏效果，窗口存在时，\n"
			"将禁用任务管理器，和禁用除 " WDO_EXE_FILE " 外的新打开的 cmd 窗口；\n"
			"会隐藏任务栏窗口和游标；\n"
			"蓝框最大化且无法改变大小；\n"
			"蓝框每隔一小段时间置顶并获取一次键盘焦点。\n"
			"\n"
			"该指令没有任何危害，一切更改将在关闭窗口或关闭进程时还原。\n"
			"可以使用快捷键 " WDO_EXIT_HOTKEY_STR " 一键关闭 " WDO_EXE_FILE " 进程。\n"
			,
			"." WDOINS_BOX_BLUE "  蓝屏警告"
		);
	}
	
	else if (!stricmp(_str, WDOINS_BOX_SET_TEXT))
	{
		wdo_log_help(
			"设置框中文本。\n"
			,
			WDOINS_BOX_SET_TEXT "  \"text\""
			,
			"text                    新文本\n"
			,
			NULL
			,
			"框必须由 " WDO_EXE_FILE " 创建；\n"
			"不拆分参数，仅去掉边缘 '{' '}'；\n"
			"需要换行请使用 '{' '}' 包裹。\n"
			,
			WDOINS_BOX_TEXT "  Hello Wrold!\n"
			WDOINS_BOX_SET_TEXT "  {新的行一文本；\n"
			"新的行二文本。}\n"
		);
	}

	/*****************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 特殊指令 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	******************************************************************************************/

	else if (!stricmp(_str, WDOINS_LOOP))
	{
		wdo_log_help(
			"循环执行一个命令块。"
			,
			WDOINS_LOOP "  {instructions}"
			,
			"{instructions}          循环执行的 wdo 指令集。\n"
			,
			NULL
			,
			"此指令可以采用递归调用。\n"
			"  例：\n"
			"         " WDOINS_LOOP "*5  " WDOINS_LOOP "*3  " WDOINS_LOOP "  " WDOINS_MOVEY "*2  1\n"
			"以上示例总共执行了 30(5*3*2=30) 次 \"" WDOINS_MOVEY "  1\"。\n"
			"但是不建议写成以上写法，因为它除了增大内存和减慢运行速度外，无任何意义。\n"
			,
			"." WDOINS_LOOP "*" WDO_LOOPCOUNT_STR "\n"
			"{\n"
			"    " WDOINS_PTR "  -p  cur\n"
			"    " WDOINS_MOVEY "*10  1\n"
			"    " WDOINS_MOVEY "*10  -1\n"
			"}\n"
			"\n"
			"// 使当前游标指向的窗口进行上下移动。\n"
			"// \"*" WDO_LOOPCOUNT_STR "\" 表示无限循环。\n"
		);
	}
	else if (!stricmp(_str, WDOINS_RANDOMRUN))
	{
		wdo_log_help(
			"随机执行一个命令块中的一条命令。"
			,
			WDOINS_RANDOMRUN "  {instructions}"
			,
			"{instructions}          随机执行的 wdo 指令集。\n"
			,
			NULL
			,
			"此指令可以采用递归调用。\n"
			,
			"." WDOINS_RANDOMRUN "\n"
			"{\n"
			"    " WDOINS_MOVEX " 10\n"
			"    " WDOINS_MOVEY " 10\n"
			"}\n"
			"// 随机执行 " WDOINS_MOVEX " 或者 " WDOINS_MOVEY "\n"
		);
	}
	else if (!stricmp(_str, WDOINS_QUEUE))
	{
		wdo_log_help(
			"将一条语句放入定时运行的队列。"
			,
			WDOINS_QUEUE "  {instructions}"
			,
			NULL
			,
			"{instructions}          要放入的 wdo 指令。\n"
			,
			"此指令需配合指令 " WDOINS_AT " 使用，详见 " WDOINS_HELP " " WDOINS_AT "。"
			,
			NULL
		);
	}
	else if (!stricmp(_str, WDOINS_AT))
	{
		wdo_log_help(
			"设置定时运行队列的运行时间。"
			,
			WDOINS_AT "  <hour>  <minute>  <second>"
			,
			"<hour>                  小时[0-23]。\n"
			"<minute>                分钟[0-59]。\n"
			"<second>                秒钟[0-59]。\n"
			,
			NULL
			,
			"请不要将 " WDOINS_QUEUE " 和 " WDOINS_AT " 指令使用多线程(" WDOINS_THREAD ")调用，因为毫无必要，\n"
			"在同一个时间，只允许有一个 " WDOINS_QUEUE " 运行队列，遇到 " WDOINS_AT " 时清空，\n"
			"在遇到 " WDOINS_AT " 指令前，所有的 " WDOINS_QUEUE " 指令放入的都是同一个运行队列，\n"
			"\n"
			"此指令一般配合 " WDOINS_QUEUE " 使用，用于定时向QQ或者微信好友发送信息。\n"
			"不过 " WDOINS_AT " 指令只会生效一次。需要更多功能，请参考 " WDOINS_CMD " 指令 schtasks.exe\n"
			,
			"." WDOINS_PTR "  -c  TXGuiFoundation"
			"." WDOINS_QUEUE "\n"
			"{\n"
			"    " WDOINS_SEND "  -i  嗨害嗨。\n"
			"    " WDOINS_SEND "  -i  早上好！\n"
			"}\n"
			"." WDOINS_AT "  8  0  0"
			"\n"
			"// 将在早上八点整发送两条信息到类名为 \"TXGuiFoundation\" 的窗口。\n"
			"// \"TXGuiFoundation\" 一般是QQ的聊天窗口的类名。\n"
		);
	}
	else if (!stricmp(_str, WDOINS_HOTKEY))
	{
		wdo_log_help(
			"设置全局快捷键的命令块。"
			,
			WDOINS_HOTKEY "  [[ACSW]+'key']  {instructions}"
			,
			"[ACSW]                  A 和 C 和 S 和 W 的任意组合，详见言论。\n"
			"'key'                   可以是任意一个键盘上的按键，缺省则使用空格。\n"
			"\n"
			"{instructions}          按下快捷键后执行的 wdo 指令集，如果缺省，则取消该快捷键。\n"
			,
			NULL
			,
			"关于 [ACSW]：\n"
			"        A:    Alt 键\n"
			"        C:    Ctrl 键\n"
			"        S:    Shift 键\n"
			"        W:    Windows 键\n"
			"关于 'key':\n"
			"        1 2 3 4 5 6 7 8 9 0 - =\n"
			"        Q W E R T Y U I O P [ ]\n"
			"        A S D F G H J K L ; ' \\\n"
			"        Z X C V B N M , . / `\n"
			"\n"
			"        F1  F2  F3  F4  F5  F6\n"
			"        F7  F8  F9  F10 F11 F12\n"
			"        F13 F14 F15 F16 F17 F18\n"
			"        F19 F20 F21 F22 F23 F24\n"
			"\n"
			"        'key' 可以是以上列表中任意一个选项，每个选项代表一个按键。\n"
			"        暂时不支持以上列表中没有的字符。\n"
			"示例:\n"
			"        AC+R        <Alt + Ctrl + R>\n"
			"        AS+9        <Alt + Shift + 9>\n"
			"        C+-         <Ctrl + ->\n"
			"        ACS+F1      <Alt + Ctrl + Shift + F1>\n"
			"注意:\n"
			"       '+' 号两边不能有空格；\n"
			"       'key' 可以缺省，但是 '+' 号必须存在；\n"
			"       请不要单独使用 Shift 键，它的定义是输出大写；\n"
			"       不能使用系统已有的快捷键，因为这是全局的；\n"
			"       Windows 键本应属于系统，请谨慎使用；\n"
			,
			"." WDOINS_HOTKEY "  AC+R\n"
			"{\n"
			"    " WDOINS_RPOS "\n"
			"}\n"
			"// 设置全局快捷键 <Alt + Ctrl + R>，按下后执行 " WDOINS_RPOS "命令\n"
		);
	}
	else if (!stricmp(_str, WDOINS_THREAD))
	{
		wdo_log_help(
			"另开一条线程执行一个命令块。"
			,
			WDOINS_THREAD "  {instructions}"
			,
			"{instructions}          新线程执行的 wdo 指令集。\n"
			,
			NULL
			,
			"为了效率考虑，执行多线程命令块中的指令时，会自动关闭回显，\n"
			"不会影响当前主线程。\n"
			"如果要获取错误，请在主线程测试命令后，再放入多线程中。\n"
			"\n"
			"此指令可以采用递归调用。\n"
			"  例：\n"
			"         " WDOINS_THREAD "*5  " WDOINS_THREAD "*3  " WDOINS_THREAD "  " WDOINS_MOVEY "*2  1\n"
			"以上语句虽然合法，但是不建议使用，采用多线程的本意是加快运行速度或同一时间做多件事，\n"
			"而以上命令除了增大内存和减慢运行速度外，无任何意义。\n"
			,
			"." WDOINS_PTR "  -p  top\n"
			"." WDOINS_THREAD "\n"
			"{\n"
			"    " WDOINS_POS "  0  0\n"
			"    " WDOINS_MOVEX "*100  2\n"
			"    " WDOINS_MOVEY "*100  2\n"
			"    " WDOINS_MOVEX "*100  -2\n"
			"    " WDOINS_MOVEY "*100  -2\n"
			"    " WDOINS_ZOOM "*50  4  4\n"
			"    " WDOINS_EXIT "\n"
			"}\n"
			"\n"
			"// 以上是一个窗口运动的小动画。"
		);
	}

	else wdo_log(WDOINS_HELP, OUTLOG_ERROR, wdo_language("Instruction %s not found!", "没有找到 \"%s\" 指令！") "\n", _str);
}

#endif