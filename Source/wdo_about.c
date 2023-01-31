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
			"    - Data in alias is used by instructions, see " WDOINS_HELP " " WDOINS_ALIAS_APPLY "��\r\n"
			"    - The instruction represents the main function of this command, for the list of instructions, see " WDOINS_HELP "��\r\n"
			"    - Loop-count is the number of times this command will be executed repeatedly, please use string constant " WDO_LOOPCOUNT_STR " for infinite loop��\r\n"
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
			"    'key'        - A key name��\r\n"
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
			"The end��\r\n"
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
			"��飺\r\n"
			"    - " WDO_EXE_FILE " ��һ��ڿ��ƵĽ��������ԣ�\r\n"
			"    - ӵ�н���ʽҳ�棬����ʹ�ýű��ļ���" WDO_SCRIPT_SUFFIX "������ʵ���Զ�����\r\n"
			"    - ʹ�ý϶̺ͼ�������Դ󲿷ִ��ڽ��п��ƣ��ͻ�ȡ������Ϣ��\r\n"
			"    - ֧�ֶ��̲߳�����ѭ�������ݹ飬��ʱ����\r\n"
			"    - �Ա�����Ϊ�������壬��֧�ִ󲿷�ָ��Ĳ���ȱʡ��\r\n"
			"    - ����Ԥ����Ĵ��ڴ�����֧�����ã�\r\n"
			"    - ���ڴ� C ���Ա�д������ WindowsAPI(Win32)������չ��һЩ���ܣ�\r\n"
			"    - �ó����� Windows11 �ϲ���ͨ��������ѡ��ϸ߰汾��ϵͳ��\r\n"
			"ʹ�ã�\r\n"
			"    - ԭ�� Alias.Instruction*count  parameter\r\n"
			"    - һ�������������ɣ�����(Alias)��ָ��(Instruction)��ѭ������(count)������(parameter)��ɣ�\r\n"
			"    - ����װ�ص����ݹ�ָ��ѡ��ʹ�ã���� " WDOINS_HELP " " WDOINS_ALIAS_APPLY "��\r\n"
			"    - ָ�������һ���������Ҫ���ܣ�ָ���б���� " WDOINS_HELP "��\r\n"
			"    - ѭ����������һ������ظ�ִ�еĴ���������ѭ����ʹ���ַ������� " WDO_LOOPCOUNT_STR "��\r\n"
			"    - ��������ָ�������" WDOINS_HELP " �������ĵ����ͣ�\r\n"
			"\r\n"
			"    - ԭ�� Instruction  parameter\r\n"
			"    - ����һ���������������ɣ�\r\n"
			"    - Ĭ�ϲ������κα�����ѭ������Ϊһ��ִֻ��һ�Σ�\r\n"
			"\r\n"
			"    - ԭ�� Instruction\r\n"
			"          {\r\n"
			"              parameters1\r\n"
			"              parameters2\r\n"
			"              ...\r\n"
			"          }\r\n"
			"    - �󲿷ִ�������ָ�����ʹ������ԭ�ͣ�����һ�������б�����ִ�У�\r\n"
			"    - �ȴ��� parameters1 ִ��һ�Σ��ٴ��� parameter2 ִ��һ�Σ�Instruction �ܹ���ִ�����Σ�\r\n"
			"    - �����б���ʹ�û��ŷָ����ָ��֧�ֶ��ŷָ���� " WDOINS_HELP " �ĵ���ָ����\r\n"
			"    - �������������Ҫ������ո��·�������ܹ�ʶ��˫���ţ�����������Χʹ�� {} ������\r\n"
			"    - ����֮��ʹ�ÿո���Ʊ���ָ˫���Ų������Ӳ��������� {} ���ԣ���ʹ��·��ǰ������ȥ�����˷��ţ�\r\n"
			"�������ͣ�\r\n"
			"    [options]    - һ������ѡ����ѡ��֮��ʹ�� '|' �� '/' �ָ���\r\n"
			"    <int>        - һ������������ʶ�𸺺ţ�\r\n"
			"    <hex>        - һ��ʮ����������\r\n"
			"    \"string\"    -  һ���ַ�����\r\n"
			"    'char'       - һ���ַ���\r\n"
			"    'key'        - һ����������\r\n"
			"    {set}        - һ�����ϣ�ÿ��Ԫ��ʹ�û��зָ\r\n"
			"    (null)       - ���ָ��û�в�����\r\n"
			"\r\n"
			"    - ÿ��ָ�����ӵ�ж��ԭ�ͣ����ȼ�����������ʾ��\r\n"
			"�����е��ã�\r\n"
			"    " WDO_EXE_FILE " �ܹ�ʶ����ļ���׺�����磺(" WDO_SCRIPT_SUFFIX ")��(" FALIAS_EXT ")\r\n"
			"    .txt         - ���� " WDOINS_BOX_TEXT  " ָ�������\r\n"
			"    .bmp         - ���� " WDOINS_BOX_IMAGE " ָ�������\r\n"
			"    .mp3         - ���� " WDOINS_MUSIC     " ָ�������\r\n"
			"\r\n"
			"    - �������ʶ������Ϊ��һ�� wdo ������н������˳���\r\n"
			"�ű��ļ��ı�д��\r\n"
			"    - �ű��ļ�ʹ�� " WDO_SCRIPT_SUFFIX " ��׺����\r\n"
			"    - Ĭ��ʹ�� ANSI �����ļ���\r\n"
			"    - �������ļ���һ��ָ�����ļ��Ľ��뷽ʽ�������ڵ�һ���ұ��뵥��һ�У�\r\n"
			"    - UTF-8 ʹ��  ��--UNICODE--�� ������˫���ţ���\r\n"
			"    - ANSI  ʹ��  ��--ANSI--��������˫���ţ���\r\n"
			"ĩβ��\r\n"
			"    ��Ȼ����һ�����ֺ���ϰ������Ŀ�����������ߵĵڶ��� C ����\r\n"
			"    ׼ȷ��˵�������ߵ�һ�������� C ����\r\n"
			"    ��Ϊ�������ޣ�������ʱ��ı�д�Ͷ���Ż�ɾ�������ܻ��ǻ��кܶ�δ֪ BUG��\r\n"
			"    Ϊ�˳���������ԣ��صػ���д��һЩ�����ĵ�����Щ�ĵ���ʽ�ο���һЩ WindowsCMD �ķ��\r\n"
			"    ����ϣ�������ܵİﵽѡ��ʹ���������ʹ�����ǣ���л��ҡ�\r\n"
			"\r\n"
			"    �汾 [1.0] ����Ϊ���б�ҵ���Դ˼����������ȥ���ഺ��\r\n"
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