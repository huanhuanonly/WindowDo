// wdo_help_chinese.c -- For wdo_about() function
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/wdoio.h"
#include "../Head/wdo_instructions.h"
#include "../Head/lrWindowDo.h"

static const char* const wdo_getabout(DWORD _LanguageId)
{
	switch (_LanguageId)
	{
	case LANG_ENGLISH:
	{
		return
			"Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.\r\n"
			"\r\n"
			"Introduce:\r\n"
			"    - " WDO_EXE_FILE " is an explanatory language for window control;\r\n"
			"    - It have interactive pages, you can use script file(" WDO_SCRIPT_SUFFIX ") to drive and automate;\r\n"
			"    - Use short and concise commands to control most windows and obtain window information;\r\n"
			"    - Support the use of multiple threads, loops, multi-layer recursion, and timed tasks;\r\n"
			"    - Use aliases as data carriers and support parameter defaults for most instructions;\r\n"
			"    - Support predefined creation windows and settings;\r\n"
			"    - This program is written in pure C language, integrates WindowsAPI(Win32), and extends some functions;\r\n"
			"    - The program passed the test on Windows 11;\r\n"
			"Use:\r\n"
			"    - Prototype->   Alias.Instruction*count  parameter\r\n"
			"    - A complete command consists of alias, instruction, loop-count and parameter;\r\n"
			"    - Data in alias is used by instructions, see " WDOINS_HELP " " WDOINS_ALIAS_APPLY "；\r\n"
			"    - The instruction represents the main function of this command, for the list of instructions, see " WDOINS_HELP "；\r\n"
			"    - Loop-count is the number of times this command will be executed repeatedly, please use string constant " WDO_LOOPCOUNT_STR " for infinite loop；\r\n"
			"    - Parameters are determined according to different instructions, for get explanation, see " WDOINS_HELP ";\r\n"
			"\r\n"
			"    - Prototype->   Instruction  parameter\r\n"
			"    - This is a shortest command;\r\n"
			"    - By default, no aliases are brought in, and the cycle count is one, which is executed only once;\r\n"
			"\r\n"
			"    - Prototype->   Instruction\r\n"
			"                    {\r\n"
			"                        parameters1\r\n"
			"                        parameters2\r\n"
			"                        ...\r\n"
			"                    }\r\n"
			"    - Most instructions with parameters can use this prototype to bring in a parameter list and execute it in turn;\r\n"
			"    - Execute once with parameters1 and then with parameter2, the instruction will be executed twice in total;\r\n"
			"    - The parameter list uses a linebreak split, and some instructions support comma split, which will be indicated in the " WDOINS_HELP " document;\r\n"
			"    - If you need to enter a path with a space for a parameter, it can recognize double-quotation marks, but please use {} to include it in the periphery;\r\n"
			"    - The parameters are separated by spaces or tabs. Double-quotation marks cannot connect the parameters, but {} can. Before using the path, the symbols at both ends will be removed;\r\n"
			"Parameter type:\r\n"
			"    [options]    - One or more options, separated by '|' or '/';\r\n"
			"    <int>        - An integer that can identify negative signs;\r\n"
			"    <hex>        - A hexadecimal number;\r\n"
			"    \"string\"    -  A string;\r\n"
			"    'char'       - A character\r\n"
			"    'key'        - A key name；\r\n"
			"    {set}        - A set, each element is split by a linebreak;\r\n"
			"    (null)       - This instruction has no parameters;\r\n"
			"\r\n"
			"    - Each instruction may have multiple prototypes, with priority displayed from top to bottom;\r\n"
			"Called in cmd:\r\n"
			"    " WDO_EXE_FILE " can recognize the file suffix, such as (" WDO_SCRIPT_SUFFIX "), (" FALIAS_EXT ")\r\n"
			"    .txt         - Call the " WDOINS_BOX_TEXT  " instruction and start it;\r\n"
			"    .bmp         - Call the " WDOINS_BOX_IMAGE " instruction and start it;\r\n"
			"    .mp3         - Call the " WDOINS_MUSIC     " instruction and start it;\r\n"
			"\r\n"
			"    - If the path is not recognized, will considered as a wdo command, and will exit after running;\r\n"
			"Write script file:\r\n"
			"    - The script file uses the suffix " WDO_SCRIPT_SUFFIX ";\r\n"
			"    - ANSI decoding file is used by default;\r\n"
			"    - It is allowed to specify the decoding method of the file in the first line of the file, which must be in the first line and a separate line;\r\n"
			"    - UTF-8 use  \"--UNICODE--\"(without double quotation marks);\r\n"
			"    - ANSI  use  \"--ANSI--\"(without double quotation marks);\r\n"
			"The end：\r\n"
			"    Although this is an entertainment and exercise-level project, it is the author's second C program,\r\n"
			"    To be precise, it is the author's first complete C program;\r\n"
			"    Because of the limited technology, there may still be many unknown bugs after a long time of writing and many times of optimization and deletion;\r\n"
			"    In order to ensure the integrity of the program, some help documents have been specially prepared. These document formats refer to some WindowsCMD styles;\r\n"
			"    I still hope to help users who choose to use this program as much as possible. Thank you all.\r\n"
			"\r\n"
			"    Version [1.0] will be used as a high school graduation commemoration. To our lost youth.\r\n"
			"\r\n"
			"                                            2022.01.18  19:12:36\r\n";
	}
	case LANG_CHINESE:
	{
		return
			"Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.\r\n"
			"\r\n"
			"简介：\r\n"
			"    - " WDO_EXE_FILE " 是一款窗口控制的解释性语言；\r\n"
			"    - 拥有交互式页面，可以使用脚本文件（" WDO_SCRIPT_SUFFIX "）驱动实现自动化；\r\n"
			"    - 使用较短和简洁的命令，对大部分窗口进行控制，和获取窗口信息；\r\n"
			"    - 支持多线程并发，循环，多层递归，定时任务；\r\n"
			"    - 以别名作为数据载体，且支持大部分指令的参数缺省；\r\n"
			"    - 带有预定义的窗口创建并支持设置；\r\n"
			"    - 基于纯 C 语言编写，集成 WindowsAPI(Win32)，并扩展了一些功能；\r\n"
			"    - 该程序在 Windows11 上测试通过，尽量选择较高版本的系统；\r\n"
			"使用：\r\n"
			"    - 原型 Alias.Instruction*count  parameter\r\n"
			"    - 一条完整的命令由：别名(Alias)、指令(Instruction)、循环计数(count)、参数(parameter)组成；\r\n"
			"    - 别名装载的数据供指令选择使用，详见 " WDOINS_HELP " " WDOINS_ALIAS_APPLY "；\r\n"
			"    - 指令代表这一条命令的主要功能，指令列表详见 " WDOINS_HELP "；\r\n"
			"    - 循环计数是这一条命令将重复执行的次数，无限循环请使用字符串常量 " WDO_LOOPCOUNT_STR "；\r\n"
			"    - 参数根据指令决定，" WDOINS_HELP " 中有着文档解释；\r\n"
			"\r\n"
			"    - 原型 Instruction  parameter\r\n"
			"    - 这是一条命令最基本的组成；\r\n"
			"    - 默认不带入任何别名，循环计数为一，只执行一次；\r\n"
			"\r\n"
			"    - 原型 Instruction\r\n"
			"          {\r\n"
			"              parameters1\r\n"
			"              parameters2\r\n"
			"              ...\r\n"
			"          }\r\n"
			"    - 大部分带参数的指令都可以使用这种原型，带入一个参数列表，依次执行；\r\n"
			"    - 先搭配 parameters1 执行一次，再搭配 parameter2 执行一次，Instruction 总共会执行两次；\r\n"
			"    - 参数列表中使用换号分割，部分指令支持逗号分割，将在 " WDOINS_HELP " 文档中指出；\r\n"
			"    - 如果遇到参数需要输入带空格的路径，它能够识别双引号，但是请在外围使用 {} 包含；\r\n"
			"    - 参数之间使用空格或制表符分割，双引号不能连接参数，但是 {} 可以，它使用路径前，将会去掉两端符号；\r\n"
			"参数类型：\r\n"
			"    [options]    - 一个或多个选项，多个选项之间使用 '|' 或 '/' 分隔；\r\n"
			"    <int>        - 一个整数，可以识别负号；\r\n"
			"    <hex>        - 一个十六进制数；\r\n"
			"    \"string\"    -  一个字符串；\r\n"
			"    'char'       - 一个字符；\r\n"
			"    'key'        - 一个按键名；\r\n"
			"    {set}        - 一个集合，每个元素使用换行分割；\r\n"
			"    (null)       - 这个指令没有参数；\r\n"
			"\r\n"
			"    - 每条指令可能拥有多个原型，优先级从上往下显示；\r\n"
			"命令行调用：\r\n"
			"    " WDO_EXE_FILE " 能够识别的文件后缀，例如：(" WDO_SCRIPT_SUFFIX ")，(" FALIAS_EXT ")\r\n"
			"    .txt         - 调用 " WDOINS_BOX_TEXT  " 指令并启动；\r\n"
			"    .bmp         - 调用 " WDOINS_BOX_IMAGE " 指令并启动；\r\n"
			"    .mp3         - 调用 " WDOINS_MUSIC     " 指令并启动；\r\n"
			"\r\n"
			"    - 如果不能识别，则认为是一条 wdo 命令，运行结束后将退出；\r\n"
			"脚本文件的编写：\r\n"
			"    - 脚本文件使用 " WDO_SCRIPT_SUFFIX " 后缀名；\r\n"
			"    - 默认使用 ANSI 解码文件；\r\n"
			"    - 允许在文件第一行指定该文件的解码方式，必须在第一行且必须单独一行；\r\n"
			"    - UTF-8 使用  “--UNICODE--” （不带双引号）；\r\n"
			"    - ANSI  使用  “--ANSI--”（不带双引号）；\r\n"
			"末尾：\r\n"
			"    虽然这是一个娱乐和练习级的项目，但是是作者的第二个 C 程序，\r\n"
			"    准确的说，是作者第一个完整的 C 程序；\r\n"
			"    因为技术有限，经过长时间的编写和多次优化删减，可能还是会有很多未知 BUG；\r\n"
			"    为了程序的完整性，特地还编写了一些帮助文档，这些文档格式参考了一些 WindowsCMD 的风格；\r\n"
			"    还是希望尽可能的帮到选择使用这款程序的使用者们，感谢大家。\r\n"
			"\r\n"
			"    版本 [1.0] 将作为高中毕业作以此纪念。致我们逝去的青春。\r\n"
			"\r\n"
			"                                            2022.01.18  19:12:36\r\n";
	}
	default: return NULL;
	}
}

const char* const wdo_about(BOOL _InBox, DWORD _LanguageId)
{
	const char* const lpString = wdo_getabout(_LanguageId);

	if (_InBox)
	{
		HWND logbox = lrCreateLogBox(GetSystemMetrics(SM_CXSCREEN) / 2 - (900 / 2), GetSystemMetrics(SM_CYSCREEN) / 2 - (500 / 2), 900, 500, lpString, NULL);
		SetWindowTextA(logbox, "About " WDO_EXE_FILE);
	}
	else wdo_puts(lpString);

	return lpString;
}