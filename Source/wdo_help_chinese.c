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
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Ŀ¼ -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	**************************************************************************************/
	if (_str == NULL || _str[0] == '\0')
	{
		unsigned int InstructionCount = 0;

		wdo_log(NULL, OUTLOG_NORMAL, "\n�����ࣺ\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_ECHO				" �򿪻�رջ��Կ��أ����ڿ���̨��ӡ�ı�����\n"
			WDOINS_CD				" ���õ�ǰ����·��\n"
			WDOINS_CLIP				" ���ü���������(�ı�)\n"
			WDOINS_STARTUP			" ������ӿ���������\n"
			WDOINS_PAUSE			" ʹ������ͣ(����)\n"
			WDOINS_DELAY			" ������ʱ��(����)\n"
			WDOINS_MUSIC			" ����һ������\n"
			WDOINS_BEEP				" ���ɷ�����\n"
			WDOINS_CALL				" ���� WDO �ļ�\n"
			WDOINS_CMD				" ���� CMD ָ��\n"
			WDOINS_MCI				" ���� MCI ָ��\n"
			WDOINS_EXIT				" �˳� " WDO_EXE_FILE "\n"
			WDOINS_EXITASK			" �����˳� " WDO_EXE_FILE "\n"
			WDOINS_HELP				" ��ȡ������ʹ�÷���\n"
			WDOINS_ABOUT			" ���� " WDO_EXE_FILE "\n"
		);
		wdo_log(NULL, OUTLOG_NORMAL, "\n�����ࣺ\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_ALIAS_APPLY		" ����һ�����������ڴ洢�ض�������\n"
			WDOINS_ALIAS_GET		" ��ȡһ��Ԥ�����Ĭ�ϱ���\n"
			WDOINS_ALIAS_SET		" ���û���ı����е�����\n"
			WDOINS_ALIAS_SETUP		" ��װһ��������������Ϊ " ALIASTYPE_STR_BOX " �ı�����װ��һ�����еĿ򣨴��ڣ���\n"
			WDOINS_ALIAS_SEE		" ��ʾ�����ṹ\n"
			WDOINS_ALIAS_SAVE		" ����һ�������ļ���" FALIAS_EXT "�����ɹ�������� " WDO_SCRIPT_SUFFIX " �ű�����ʹ��\n"
			WDOINS_ALIAS_LOAD		" ����һ�������ļ���" FALIAS_EXT "����������ָ�� " WDOINS_ALIAS_SAVE " ����\n"
		);
		wdo_log(NULL, OUTLOG_NORMAL, "\n���ڿ����ࣺ\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_PTR				" ָ����ѡ�У�һ��Ҫ�����Ĵ��ڡ��Դ��ڽ��в���ʱ����ʹ�ô�ָ��ȷ��Ҫ��������\n"
			WDOINS_UPDATEPTR			" " WDOINS_PTR " ���¿���\n"
			WDOINS_OPEN				" ��һ�����̲�ָ��һ���µĴ���\n"
			WDOINS_LIST				" �����������д��ڣ�����ӡ������̨\n"
			WDOINS_INFO				" ��ӡ " WDOINS_PTR " ָ�򴰿ڵ���Ϣ�ڵ�ǰ����̨\n"
			WDOINS_TITLE			" ���� " WDOINS_PTR " ָ�򴰿ڵı����ı�\n"
			WDOINS_TOP				" �� " WDOINS_PTR " ָ�򴰿ڷ���ǰ̨\n"
			WDOINS_TOPMOST			" �� " WDOINS_PTR " ָ�򴰿��ö�\n"
			WDOINS_BOTTOM			" �� " WDOINS_PTR " ָ�򴰿��õ�\n"
			WDOINS_ACTIVE			" �� " WDOINS_PTR " ָ�򴰿�����Ϊ�����\n"
			WDOINS_FOCUS			" �� " WDOINS_PTR " ָ�򴰿�����Ϊ���㴰��\n"
			WDOINS_CAPTURE			" �� " WDOINS_PTR " ָ�򴰿�����Ϊ���񴰿�\n"
			WDOINS_TRANSP			" ���� " WDOINS_PTR " ָ�򴰿ڵ�͸����\n"
			WDOINS_ERASE			" ���� " WDOINS_PTR " ָ�򴰿ڵ�ĳһ��ɫ\n"
			WDOINS_TRANSPERASE		" ���� " WDOINS_PTR " ָ�򴰿ڵ�͸���ȣ�������һ����ɫ\n"
			WDOINS_SEND				" �� " WDOINS_PTR " ָ�򴰿ڷ����ı���Ϣ\n"
			WDOINS_TOPSEND			" " WDOINS_TOP " �� " WDOINS_SEND " �Ľ��\n"
			WDOINS_OUT				" �� " WDOINS_PTR " ָ�򴰿����������\n"
			WDOINS_DRAW				" �� " WDOINS_PTR " ָ�򴰿��л�ͼ\n"
			WDOINS_IMAGE			" �� " WDOINS_PTR " ָ��Ĵ����м���һ��ͼ��\n"
			WDOINS_IMAGES			" �� " WDOINS_PTR " ָ��Ĵ����м���һ��Ŀ¼������ͼ��\n"
			WDOINS_PROJECT			" ͶӰ " WDOINS_PTR " ָ�򴰿ڵ�ָ������\n"
			WDOINS_PARENT			" ���� " WDOINS_PTR " ָ�򴰿ڵ��¸�����\n"
			WDOINS_CHILD			" ���� " WDOINS_PTR " ָ�򴰿ڵ��Ӵ��ڡ�\n"
			WDOINS_COPY				" ���� " WDOINS_PTR " ָ�򴰿ڵ����ݵ�����ָ���Ĵ���\n"
			WDOINS_MOVE				" �ƶ� " WDOINS_PTR " ָ�򴰿ڵ�����\n"
			WDOINS_MOVEX			" �� X ���ƶ� " WDOINS_PTR " ָ�򴰿�����\n"
			WDOINS_MOVEY			" �� Y ���ƶ� " WDOINS_PTR " ָ�򴰿�����\n"
			WDOINS_POS				" ���� " WDOINS_PTR " ָ�򴰿ڵ����ϽǶ�������λ��\n"
			WDOINS_POSX				" ���� " WDOINS_PTR " ָ�򴰿ڵ����ϽǶ������� X λ��\n"
			WDOINS_POSY				" ���� " WDOINS_PTR " ָ�򴰿ڵ����ϽǶ������� Y λ��\n"
			WDOINS_RPOS				" ������� " WDOINS_PTR " ָ�򴰿�����\n"
			WDOINS_ZOOM				" �������С " WDOINS_PTR " ָ�򴰿ڵĴ�С\n"
			WDOINS_ZOOMX			" �������С " WDOINS_PTR " ָ�򴰿ڵĿ��\n"
			WDOINS_ZOOMY			" �������С " WDOINS_PTR " ָ�򴰿ڵĸ߶�\n"
			WDOINS_SIZEX			" ���� " WDOINS_PTR " ָ�򴰿ڵ��¿��\n"
			WDOINS_SIZEY			" ���� " WDOINS_PTR " ָ�򴰿ڵ��¸߶�\n"
			WDOINS_SHOW				" ��ʾ������ " WDOINS_PTR " ָ�򴰿�\n"
			WDOINS_STYLE			" ���� " WDOINS_PTR " ָ�򴰿ڷ��\n"
			WDOINS_MAX				" �� " WDOINS_PTR " ָ�򴰿���󻯣��൱�ڰ��´������Ͻ���󻯰�ť\n"
			WDOINS_MIN				" �� " WDOINS_PTR " ָ�򴰿���С�����൱�ڰ��´������Ͻ���С����ť\n"
			WDOINS_RESTORE			" ��������󻯵� " WDOINS_PTR " ָ�򴰿ڻ�ԭԭ���Ĵ�С���൱�ڰ��´������Ͻǻ�ԭ��ť\n"
			WDOINS_REPAINT			" �ػ� " WDOINS_PTR " ָ�򴰿ڡ����䷢�� WM_PAINT ��Ϣ\n"
			WDOINS_STROKE			" ��� " WDOINS_PTR " ָ�򴰿�\n"
			WDOINS_FILL				" ��� " WDOINS_PTR " ָ�򴰿�\n"
			WDOINS_FLASH			" ����������˸ " WDOINS_PTR " ָ�򴰿ڡ�\n"
			WDOINS_DRIFT			" ʹ " WDOINS_PTR " ָ�򴰿ڴӵ�ǰ����Ư�Ƶ�ָ������\n"
			WDOINS_FOLLOW			" ʹ " WDOINS_PTR " ָ�򴰿ڸ����α��ƶ�\n"
			WDOINS_WIGGLE			" ʹ " WDOINS_PTR " ָ�򴰿ڽ�����\n"
			WDOINS_ASIDE			" ʹ " WDOINS_PTR " ָ�򴰿ڿ�������򵯳�\n"
			WDOINS_PUSH				" ʹ " WDOINS_PTR " ָ�򴰿ڿ��Ա��α��ƶ���\n"
			WDOINS_COLLIDE			" ʹ " WDOINS_PTR " ָ�򴰿ں��α���ײʱ�����¼���\n"
			WDOINS_LOCKPAINT		" ���� " WDOINS_PTR " ָ�򴰿ڣ�ʹ���ֹ��ͼ\n"
			WDOINS_CLOSE			" �ر� " WDOINS_PTR " ָ�򴰿ڻ�رմ�����������\n"
		);
		wdo_log(NULL, OUTLOG_NORMAL, "\n�α�����ࣺ\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_CUR_MOVE			" �ƶ��α�\n"
			WDOINS_CUR_MOVEX		" �ƶ��α� X ������\n"
			WDOINS_CUR_MOVEY		" �ƶ��α� Y ������\n"
			WDOINS_CUR_POS			" �����α�����\n"
			WDOINS_CUR_POSX			" �����α� X ������\n"
			WDOINS_CUR_POSY			" �����α� Y ������\n"
			WDOINS_CUR_RPOS			" ��������α�����\n"
			WDOINS_CUR_CPOS			" �� " WDOINS_PTR " ָ�򴰿����Ͻ�Ϊԭ��(0,0)�������α�����\n"
			WDOINS_CUR_CRPOS		" �� " WDOINS_PTR " ָ�򴰿�Ϊ������������α�����\n"
			WDOINS_CUR_KEY			" ģ�ⰴ���α��\n"
			WDOINS_CUR_DRIFT		" ʹ�α����Ư���˶�\n"
			WDOINS_CUR_WIGGLE		" ʹ�α꿪ʼ�ڶ�\n"
			WDOINS_CUR_RECT			" �����α��ƶ�������һ��������\n"
		);
		wdo_log(NULL, OUTLOG_NORMAL, "\n�½������ࣺ\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_BOX_BLANK		" �½�һ���հ׿򣬲����ػ�\n"
			WDOINS_BOX_TEXT			" �½�һ���ı�����ʾ����\n"
			WDOINS_BOX_INFO			" �½�һ����Ϣ�򲢲鿴ָ�����ڵ���Ϣ\n"
			WDOINS_BOX_LOG			" �½�һ����־����ʾ��־\n"
			WDOINS_BOX_IMAGE		" �½�һ��ͼƬ����ʾͼƬ\n"
			WDOINS_BOX_BLUE			" �½�һ�������򲢽�ֹ�ر�\n"
			WDOINS_BOX_SET_TEXT		" ���ÿ����ı�\n"
		);
		wdo_log(NULL, OUTLOG_NORMAL, "\n�����ࣺ\n\n");
		InstructionCount += wdo_log_help_all(
			WDOINS_LOOP				" ѭ��ִ��һ�������\n"
			WDOINS_RANDOMRUN		" ���ִ��һ��������е�һ������\n"
			WDOINS_QUEUE			" �������ж���\n"
			WDOINS_AT				" ���ö�ʱ��������\n"
			WDOINS_HOTKEY			" ����ȫ�ֿ�ݼ��������\n"
			WDOINS_THREAD			" �½�һ���߳�\n"
		);
		wdo_putc('\n');
		wdo_log(WDOINS_HELP, OUTLOG_NORMAL, WDOINS_HELP " ���ҵ� %u ��ָ�\n\n", InstructionCount);
		wdo_log(WDOINS_HELP, OUTLOG_NORMAL, "����ָ����ִ�Сд������ " WDO_EXE_FILE " ��ʹ���뼼�ɣ���� " WDOINS_HELP " WDO\n\n");
	}


	/***************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ������ -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	****************************************************************************************/
	else if (!stricmp(_str, WDOINS_ECHO))
	{
		wdo_log_help("�򿪻�رջ��Կ��أ����ڿ���̨��ӡ�ı����ݡ�"
			,
			WDOINS_ECHO "  [on/off]\n"
			WDOINS_ECHO "  \"text\"\n"
			,
			"[on]                    �򿪻��ԡ�\n"
			"[off]                   �رջ��ԡ�\n"
			"\n"
			"\"text\"                  Ҫ��ӡ���ı����ݡ�\n"
			,
			NULL
			,
			"����������нű��ļ�����һ�����ػ���ʾ����̨���ڡ�"
			,
			"." WDOINS_ECHO "  off"
		);
	}
	else if (!stricmp(_str, WDOINS_CD))
	{
		wdo_log_help(
			"���õ�ǰ����·����"
			,
			WDOINS_CD "  \"dir\"\n"
			WDOINS_CD "  (null)\n"
			,
			"\"dir\"                   ��Ŀ¼�����������·����\n"
			"(null)                  ��ʾ��ǰ����Ŀ¼��\n"
			,
			ALIASTYPE_STR_PATH   " ��� dir ȱʡ����ʹ�ø�ֵ��"
			,
			"������һ��Ŀ¼��    " WDOINS_CD "  ..\n"
			,
			"." WDOINS_CD "  c:\\windows"
		);
	}
	else if (!stricmp(_str, WDOINS_CLIP))
	{
		wdo_log_help("���ü������ı����ݡ�������ʹ���ڼ�����ڵ�ǰ " WDO_EXE_FILE " �Ĵ��ڡ�"
			,
			WDOINS_CLIP "  [-T text | -F file | -C]"
			,
			"-T       text           ֱ������һ�в������е��ı����ݡ�\n"
			"-F       file           ����Ϊ�ļ��е����ݡ������еģ��������У�\n"
			"-C                      ��ռ��������ݡ�\n"
			,
			NULL
			,
			"��ָ��ֻ�������ı����ݣ��ݲ�֧��������ʽ��\n"
			"���ʧ�ܣ��������������������ʹ�ü����塣\n"
			,
			"." WDOINS_CLIP "  -t  ��ӭʹ�� " WDO_EXE_FILE ""
		);
	}
	else if (!stricmp(_str, WDOINS_STARTUP))
	{
		wdo_log_help(
			"������ӿ��������\n"
			,
			WDOINS_STARTUP "  \"cmdinstruction\"\n"
			WDOINS_STARTUP "  (null)\n"
			,
			"cmdinstruction          CMD ��ָ�����ֱ����һ���ļ�·����\n"
			"(null)                  ����·������������ʾ��ǰ�������б�\n"
			,
			ALIASTYPE_STR_PATH   " �������ȱʡ����ʹ�ø�ֵ���������ӡ�б�"
			,
			"���� \"cmdinstruction\" ��ԭ�ⲻ����ӵ�ע����У�\n"
			"��������·����ʹ�������в����ͻ���������\n"
			"\n"
			"��ֱ���޸�ע����� HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run ��ֵ��\n"
			"����Ҫ����ԱȨ�ޣ�\n"
			"�����ɱ��������ܻᱻ��ʾ�����أ�\n"
			,
			"." WDOINS_STARTUP "  %Userprofile%\\Desktop\\MyProgram.exe"
		);
	}
	else if (!stricmp(_str, WDOINS_PAUSE))
	{
		wdo_log_help(
			"������һ�һ��ʱ������ͣ����λΪ���롣��1s = 1000ms��"
			,
			WDOINS_PAUSE "  <time>"
			,
			"<time>                  ��Ҫ��ͣ��ʱ�䡣"
			,
			NULL
			,
			WDOINS_PAUSE " ����ͣ��ǰ�̡߳�\n"
			"������ " WDOINS_ECHO " on ״̬�¶������߳�����Ӱ�죬\n"
			"��Ϊ " WDOINS_ECHO " ��ʹ�ö��߳��ٽ绥������\n"
			,
			"." WDOINS_PAUSE "  520"
		);
	}
	else if (!stricmp(_str, WDOINS_DELAY))
	{
		wdo_log_help(
			"������ʱֵ������Ƶ����ʹ��ָ�� " WDOINS_PAUSE "����λΪ���롣��1s = 1000ms��"
			,
			WDOINS_DELAY "  [-I | -SW | -SL | -SF]  <time>\n"
			,
			"-I                      ÿִ��һ��ָ������ʱ��\n"
			"-SW                     ʹ��ָ�� " WDOINS_SEND " ʱÿ����һ���ַ�����ʱ��\n"
			"-SL                     ʹ��ָ�� " WDOINS_SEND " ʱÿ����һ�е���ʱ��\n"
			"-SF                     ʹ��ָ�� " WDOINS_SEND " ʱÿ����һ���ļ�����ʱ��\n"
			"\n"
			"<time>                  ���õ���ʱ��\n"
			,
			NULL
			,
			"���������Ҫ�뽫��ֵ����Ϊ�㼴�ɡ�"
			,
			"." WDOINS_DELAY "  -i  10\n"
			"." WDOINS_MOVEX "*20  1"
		);
	}
	else if (!stricmp(_str, WDOINS_MUSIC))
	{
		wdo_log_help(
			"����һ�����֡���Ҫ���๦����ʹ�� " WDOINS_MCI " ָ�"
			,
			WDOINS_MUSIC "  [-O | -R | -W | -C | -P | -S]  \"path\""
			,
			"-O                      ֻ����һ�Ρ�\n"
			"-R                      ѭ�����š�\n"
			"-W                      ֻ����һ�Σ��ȴ���������ִ����һ��ָ�\n"
			"-C                      �رղ��š�\n"
			"-P                      ��ͣ���š�\n"
			"-S                      �������š�\n"
			"\n"
			"\"path\"                  �����ļ�·��������׺������\n"
			,
			ALIASTYPE_STR_PATH       " ��� path ȱʡ����ʹ�ô�·����"
			,
			NULL
			,
			"." WDOINS_MUSIC " -r  c:\\�����.mp3"
		);
	}
	else if (!stricmp(_str, WDOINS_BEEP))
	{
		wdo_log_help(
			"���ɷ�������"
			,
			WDOINS_BEEP "  <freq>  <duration>"
			,
			"<freq>                  ����Ƶ�ʣ��Ժ���Ϊ��λ��\n"
			"<duration>              �����ĳ���ʱ�䣬�Ժ���Ϊ��λ��\n"
			,
			NULL
			,
			"�������������������һ��ϵͳ��������\n"
			"\n"
			"ϵͳ����������ȴ�����ֹͣ���Զ���������ȴ� duration ���룻\n"
			"�����ϣ���ȴ�����ʹ�ö��߳�ָ�� " WDOINS_THREAD "��\n"
			"\n"
			"Windows 7 ϵͳ���£����ܾ���������Ӱ�죬���� ������������8254�ɱ�̼����ʱ��оƬ�������ɷ�������\n"
			"Windows 7(�� Windows 7) ϵͳ��������������ɡ�\n"
			,
			"." WDOINS_BEEP "  500  1000"
		);
	}
	else if (!stricmp(_str, WDOINS_CALL))
	{
		wdo_log_help(
			"���� " WDO_SCRIPT_SUFFIX " �ļ���"
			,
			WDOINS_CALL "  \"path\""
			,
			"\"path\"                  �ļ�·���������� .wdo ��׺��\n"
			,
			ALIASTYPE_STR_PATH       " ��� path ȱʡ����ʹ�ô�·����"
			,
			"��Ҫ��ָ���������ػ�ͬʱ���ж���ļ�����ʹ�ö��߳�ָ�� " WDOINS_THREAD "����� " WDOINS_HELP " " WDOINS_THREAD "��"
			,
			"." WDOINS_CALL "  c:\\main" WDO_SCRIPT_SUFFIX
		);
	}
	else if (!stricmp(_str, WDOINS_CMD))
	{
		wdo_log_help(
			"���� CMD ָ�"
			,
			WDOINS_CMD "  {instructions}"
			,
			"{instructions}          cmd ָ�"
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
			"���� mci ָ�"
			,
			WDOINS_MCI "  {instructions}"
			,
			"{instructions}          mci ָ�"
			,
			NULL
			,
			NULL
			,
			"." WDOINS_MCI "  open  c:\\�����.mp3  alias  music_1\n"
			"." WDOINS_MCI "  play  music_1  wait\n"
			"." WDOINS_MCI "  close  music_1"
		);
	}
	else if (!stricmp(_str, WDOINS_HELP))
	{
		wdo_log_help(
			"��ȡ������ʹ�÷�����"
			,
			WDOINS_HELP "  {instructions}"
			,
			"{instructions}          wdo ָ�"
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
			"�˳� " WDO_EXE_FILE "��"
			,
			WDOINS_EXIT "  <code>"
			,
			"<code>                  �˳��롣Ĭ��Ϊ 0��"
			,
			NULL
			,
			"���нű��ļ���" WDO_SCRIPT_SUFFIX "��ʱ����ȴ�����ʹ��ָ����Ĵ��ں��߳�ȫ���ر�ʱ�Զ��˳���\n"
			"�˳���ݼ� <" WDO_EXIT_HOTKEY_STR ">��ȫ�֣� ��\n"
			,
			"." WDOINS_EXIT "  520"
		);
	}
	else if (!stricmp(_str, WDOINS_EXITASK))
	{
		wdo_log_help(
			"�����˳� " WDO_EXE_FILE "��"
			,
			WDOINS_EXITASK "  <code>"
			,
			"<code>                  �˳��롣Ĭ��Ϊ 0��"
			,
			NULL
			,
			"����ָ����ܲ��������˳�����ȴ��û�ʹ��������Ĵ��ں��߳�ȫ���رգ�Ȼ���Զ��˳���"
			"�ű��ļ���" WDO_SCRIPT_SUFFIX "��ĩβ�Զ���Ӹ���ָ�\n"
			"�˳���ݼ� <" WDO_EXIT_HOTKEY_STR ">��ȫ�֣� ��\n"
			,
			"." WDOINS_EXIT "  520"
		);
	}


	/***************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ������ -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	****************************************************************************************/
	else if (!stricmp(_str, WDOINS_ALIAS_APPLY))
	{
		sprintf(StrRem,
			"���ڱ�����\n"
			"        - ����Ӧ����ָ���У����Ը���ָ����Ϊ����ʹ���ֲ�������ȱʡ��\n"
			"        - ����ʹ�� " WDOINS_ALIAS_APPLY " �� " WDOINS_ALIAS_GET " ���룻\n"
			"        - ������������װ���Ų�ͬ�����ݣ�ʹ�� " WDOINS_ALIAS_SEE " ָ��鿴��ϸ���ݣ�\n"
			"        - ÿ����һ����������Լռ�� %llu byte ���ڴ�ռ䣻\n"		    // sizeof(ALIAS)
			"����Լ����\n"
			"        - �����������ɣ���ĸ��A-Z,a-z�������֣�0-9�����»��ߣ�_����ɣ�\n"
			"        - ��һ���ַ������������֣�0-9����\n"
			"        - ������������󳤶�Ϊ %u��\n"								// ALIAS_MAX_LENGTH
			"        - ��������%s���ִ�Сд��\n"								// ALIAS_CASE_SENSITIVE
			"���ڱ����⣺\n"
			"        - ʹ�� " WDOINS_ALIAS_APPLY " �� " WDOINS_ALIAS_GET " ����ı���������ڱ������У�\n"
			"        - �������еı������ƶ���Ψһ�ģ�������������������ԭ�б�����\n"
			"        - �������еı������ڽ��̽���ʱ��գ������ֶ�ɾ����\n"
			"        - ��������Ա���Ϊһ���ļ����ṩ������ʹ�ã���� " WDOINS_ALIAS_SAVE "��\n"
			"        - ���Դ�һ���ļ��м���һ��������������� " WDOINS_ALIAS_LOAD "��\n"
			"ʹ�ñ�����\n"
			"        - ԭ�� ALIAS.Instruction\n"
			"        - ��������ָ��ǰ����ָ��ʹ�� '.' �ָ���\n"
			"\n"
			"        - ԭ�� ALIAS1.ALIAS2.Instruction\n"
			"        - �ɴ��������������������֮��ʹ�� '.' �ָ���\n"
			"        - ���������ͬ���͵ı�������ֻʹ�����ұߵģ�\n"
			"\n"
			"        - ԭ�� ..Instruction\n"
			"        - ��..�� ��һ��Ԥ����ı���������Ϊ " ALIASTYPE_STR_WIN "����������Ĵ��ڣ�\n"
			"        - ���Ԥ����ı������ڱ������У�Ҳ�����޸������ݺ����ͣ�\n"
			"\n"
			"        - ԭ�� .Instruction\n"
			"        - �������κα�����\n"
			"\n"
			"        - ����������Ҫ���м�������Ƿ���ȷ��\n"
			"        - �鿴ָ���ʹ�õı�������ο� " WDOINS_HELP " �и���ָ���е� RetrieveAlias ����\n"
			, sizeof(ALIASSINGLE)
			, ALIAS_MAX_LENGTH
			, ALIAS_CASE_SENSITIVE ? "" : "��"
		);

		wdo_log_help(
			"����һ�����������ڴ洢�ض������ݡ�\n"
			,
			WDOINS_ALIAS_APPLY "  [-NULL | -WIN | -PATH | -DRAW | -BOX | -DEL]  \"name\"\n"
			,
			"-NULL    name           ָ������Ϊ " ALIASTYPE_STR_NULL "��\n"
			"-WIN     name           ָ������Ϊ " ALIASTYPE_STR_WIN "������Ϊ��ǰ " WDOINS_PTR " ָ���Ĵ��ڡ�\n"
			"-PATH    name           ָ������Ϊ " ALIASTYPE_STR_PATH "��\n"
			"-DRAW    name           ָ������Ϊ " ALIASTYPE_STR_DRAW "��\n"
			"-OUT     name           ָ������Ϊ " ALIASTYPE_STR_OUT "��\n"
			"-BOX     name           ָ������Ϊ " ALIASTYPE_STR_BOX "��\n"
			"-DEL     name           ɾ��һ��������\n"
			,
			ALIASTYPE_STR_WIN        " ��� -WIN  ���������ݸ�������ı�����\n"
			ALIASTYPE_STR_PATH       " ��� -PATH ���������ݸ�������ı�����\n"
			ALIASTYPE_STR_OUT        " ��� -OUT  ���������ݸ�������ı�����\n"
			ALIASTYPE_STR_DRAW       " ��� -DRAW ���������ݸ�������ı�����\n"
			ALIASTYPE_STR_BOX        " ��� -BOX  ���������ݸ�������ı�����\n"
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
			"��ȡһ��Ԥ�����Ĭ�ϱ�����"
			,
			WDOINS_ALIAS_GET "  [-OUT | -DRAW | -BOX_TEXT | -BOX_INFO | -BOX_LOG | -BOX_IMAGE | -BOX_BLUE | -BOX_RAIN]  \"alias\""
			,
			"-OUT                    ָ�� " WDOINS_OUT "  Ĭ��ʹ�õı�����\n"
			"-DRAW                   ָ�� " WDOINS_DRAW "  Ĭ��ʹ�õı�����\n"
			"-BOX_TEXT               ָ�� " WDOINS_BOX_TEXT  "  Ĭ��ʹ�õı�����\n"
			"-BOX_INFO               ָ�� " WDOINS_BOX_INFO  "  Ĭ��ʹ�õı�����\n"
			"-BOX_LOG                ָ�� " WDOINS_BOX_LOG   "  Ĭ��ʹ�õı�����\n"
			"-BOX_IMAGE              ָ�� " WDOINS_BOX_IMAGE " Ĭ��ʹ�õı�����\n"
			"-BOX_BLUE               ָ�� " WDOINS_BOX_BLUE  "  Ĭ��ʹ�õı�����\n"
			"\n"
			"alias                   �����������ƣ���ָ�� " WDOINS_ALIAS_APPLY " һ������������в���������\n"
			,
			NULL
			,
			"ʹ��ָ�� " WDOINS_ALIAS_GET " ��ȡ�ı�����ͬ������������У�\n"
			"��ָ�� " WDOINS_ALIAS_APPLY " ��ͬ���ǣ�" WDOINS_ALIAS_GET " ��ȡ������ĳ��ָ��Ĭ��ʹ�õı�����\n"
			"����ֱ���ڱ���ԭ���õ�����£����ı䵥�����á�\n"
			"��ָ�� " WDOINS_ALIAS_APPLY " ��Ҫ��ͷ������\n"
			"�������Ҫѡ������ָ�\n"
			,
			"." WDOINS_ALIAS_GET "  -box_info  ib\n"
			"ib." WDOINS_ALIAS_SET "  -box  -tc  00ff00\n"
			"ib." WDOINS_BOX_INFO "  -v  cur\n"
			"\n"
			"// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
			"// ����һ����Ϣ�򣬱���ԭ�����ã�\n"
			"// �����ı���ɫ��Ϊ��ɫ\n"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_SET))
	{
		wdo_log_help(
			// function
			"���û���ı����е����ݡ�"

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
			"-WIN                    ��������Ϊ " ALIASTYPE_STR_WIN " �ı��������ô���Ϊ��ǰ " WDOINS_PTR " ָ��Ĵ��ڡ�\n"
			"-PATH    path           ��������Ϊ " ALIASTYPE_STR_PATH " �ı������������ñ���·����\n"
			"\n"
			"-OUT                    ��������Ϊ " ALIASTYPE_STR_OUT " �ı�����\n"
			"-IMAGE   path           ����һ��ͼƬ����ӡ�ı���ͬʱ��һ����ӡ����ͼƬ��\n"
			"-IPOS    x  y           ͼƬ�Ĵ�ӡ���꣬���ı���ӡ����Ϊԭ�� (0, 0)��\n"
			"-TC      color          ��ӡ�ı�ʱʹ�õ�������ɫ��color ����λʮ����������ɣ�ʹ�� RGB ��ɫ��\n"
			"-BC      color          ��ӡ�ı�ʱʹ�õı�����ɫ��color ����λʮ����������ɣ�ʹ�� RGB ��ɫ��\n"
			"-TR      [on/off]       �����Ƿ�͸�������͸��������ı�ʱ�������������ɫ��\n"
			"-FONT    [-N]    name   �����������ƣ������ݵ�ǰ�������������µ����塣\n"
			"-FONT    [-W]    val    ���������ȣ������ݵ�ǰ�������������µ����塣\n"
			"-FONT    [-H]    val    ��������߶ȣ������ݵ�ǰ�������������µ����塣\n"
			"-FONT    [-WEI]  val    ���������ϸ[0-1000]�������ݵ�ǰ�������������µ����塣\n"
			"-FONT    [-I [on/off]]  ����������б�������ݵ�ǰ�������������µ����塣\n"
			"-FONT    [-U [on/off]]  ���������»��ߣ������ݵ�ǰ�������������µ����塣\n"
			"-FONT    [-S [on/off]]  ��������ɾ���ߣ������ݵ�ǰ�������������µ����塣\n"
			"\n"
			"-DRAW                   ��������Ϊ " ALIASTYPE_STR_DRAW " �ı�����\n"
			"-SC      color          ���������ɫ��color ����λʮ����������ɣ�ʹ�� RGB ��ɫ��\n"
			"-SW      width          ������߿�ȡ�\n"
			"-FILL    [on/off]       �Ƿ���䡣\n"
			"-FC      color          ���������ɫ��color ����λʮ����������ɣ�ʹ�� RGB ��ɫ��\n"
			"-FI      image_path     �������ͼƬ������ -FC �����ɫ��\n"
			"\n"
			"-BOX                    ��������Ϊ " ALIASTYPE_STR_BOX " �ı�����\n"
			"-CLASS   class          ������ʹ�õ�������\n"
			"-CURSOR  path           ������ʹ�õ��α�·������׺��Ϊ \".cur\"��\n"
			"-ICON    path           ������ʹ�õ�Сͼ��·������׺��Ϊ \".ico\"��\n"
			"-TC      color          ������ʹ�õ��ı���ɫ��color ����λʮ����������ɣ�ʹ�� RGB ��ɫ��\n"
			"-BC      color          ������ʹ�õı�����ɫ��color ����λʮ����������ɣ�ʹ�� RGB ��ɫ��\n"
			,// RetrieveAlias
			ALIASTYPE_STR_WIN        " �������һΪ -WIN �������øñ����е����ݡ�\n"
			ALIASTYPE_STR_PATH       " �������һΪ -PATH�������øñ����е����ݡ�\n"
			ALIASTYPE_STR_OUT        " �������һΪ -OUT �������øñ����е����ݡ�\n"
			ALIASTYPE_STR_DRAW       " �������һΪ -DRAW�������øñ����е����ݡ�\n"
			ALIASTYPE_STR_BOX        " �������һΪ -BOX �������øñ����е����ݡ�\n"
			,// Remarks
			"���������ʹ��ָ�� " WDOINS_ALIAS_APPLY "����� " WDOINS_HELP " " WDOINS_ALIAS_APPLY "��\n"
			"ʹ��ʱ������Ӧ���͵ı�������������ʧ�ܡ�\n"
			"\n"
			" \"" ALIASTYPE_STR_BOX "\" ���͵ı������Ը���ָ�� " WDOINS_BOX_TEXT " �ȴ����Ŀ򴰿ڣ�\n"
			"�´��ڶ����Լ���Ĭ�ϱ������ã��ڲ�����Ĭ�����õ�����¿���ʹ�ñ�ָ����Ĳ��ڴ�������ʱ���롣\n"
			"���ô�������ʱһ��Ҫ�ڴ��ڴ���ʱ�����������Ϊ����һ���������������޷����ġ�\n"
			"  ����\n"
			"          ." WDOINS_ALIAS_GET "  -box_info  mybox\n"
			"          mybox." WDOINS_ALIAS_SET "  -box -class  myclass\n"
			"          mybox." WDOINS_BOX_INFO "  -v  cur\n\n"
			"�������ӽ��� \"myclass\" ��Ϊ��������һ����Ϣ���ڡ�����ʹ��Ĭ�ϵ����� \"" lrCLASSNAME_INFOBOX "\"��\n"
			"������ڴ��ڴ�����ɺ�����ı���ɫ���������ƣ�����ʹ���������\n"
			"  ����\n"
			"          ." WDOINS_BOX_INFO "  -v cur\n"
			"          ." WDOINS_ALIAS_GET "  -box_info  mybox\n"
			"          mybox." WDOINS_ALIAS_SET "  -box  -tc  9181CB\n"
			"          mybox." WDOINS_ALIAS_SET "  -box  -font  -name  ����\n"
			"          mybox." WDOINS_ALIAS_SETUP "\n\n"
			"ָ�� " WDOINS_ALIAS_SETUP " �������� \"" ALIASTYPE_STR_BOX "\" ���͵ı�����ֻ��ʹ�ô�ָ����ò�����Ч����� " WDOINS_HELP " " WDOINS_ALIAS_SETUP "��\n"
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
			"// �ڵ�ǰ�α�ָ��Ĵ��ڵ�(5,10)������ɫ��ӡ \"Hello\"��\n"
			"// ����ӡһ��·�� \"c:\\out.bmp\" ��ͼƬ��\n"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_SETUP))
	{
		wdo_log_help(
			"��װһ��������������Ϊ " ALIASTYPE_STR_BOX " �ı�����װ��һ�����еĿ򣨴��ڣ��ϡ�"
			,
			WDOINS_ALIAS_SETUP "  \"alias\"\n"
			WDOINS_ALIAS_SETUP "  (null)\n"
			,
			"alias                   Ҫ���صı��������ʡ���������������\n"
			"(null)                  ʹ�ô���ı�����\n"
			,
			ALIASTYPE_STR_WIN		 " ��ǰ�Ŀ򣨴��ڣ���\n"
			ALIASTYPE_STR_BOX        " ��� alias ȱʡ����ʹ�øñ�����\n"
			,
			"���������ʹ��ָ�� " WDOINS_ALIAS_APPLY "����� " WDOINS_HELP " " WDOINS_ALIAS_APPLY"��\n"
			"���ñ���������ʹ��ָ�� " WDOINS_ALIAS_SET "����� " WDOINS_HELP " " WDOINS_ALIAS_SET"��\n"
			"ָ�� " WDOINS_ALIAS_SETUP " һ������ʹ��ָ�� " WDOINS_ALIAS_SET " ��\n"
			,
			"." WDOINS_BOX_INFO "  -v  cur\n"
			"." WDOINS_ALIAS_APPLY "  -box  box\n"
			"box." WDOINS_ALIAS_SET "  -tc  00FF00\n"
			"box." WDOINS_ALIAS_SET "  -bc  0000FF\n"
			"box." WDOINS_ALIAS_SETUP "\n"
			"\n"
			"// ����Ϣ���Ϊ��ɫ��������ɫ���塣"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_SEE))
	{
		wdo_log_help(
			"��ʾ�����ṹ��"
			,
			WDOINS_ALIAS_SEE "  (null)\n"
			WDOINS_ALIAS_SEE "  [-ALL]\n"
			WDOINS_ALIAS_SEE "  \"alias\"\n"
			,
			"(null)                  ��ʾ��ǰ���б����б�����ʾ���ݽṹ����\n"
			"\n"
			"-ALL                    ���δ�ӡ�����������б����Ľṹ��\n"
			"\n"
			"alias                   ָ��Ҫ��ӡ�ṹ�ı�����\n"
			,
			NULL
			,
			"��ָ��ֻ�������ټ������е����������\n"
			,
			"." WDOINS_ALIAS_APPLY "  -box  MyBoxAlias\n"
			"." WDOINS_ALIAS_SEE "  MyBoxAlias\n"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_SAVE))
	{
		wdo_log_help(
			"����һ�������ļ���" FALIAS_EXT "�����ɹ�������� " WDO_SCRIPT_SUFFIX " �ű�����ʹ�á�"
			,
			WDOINS_ALIAS_SAVE "  [-LIB]   \"path\"\n"
			WDOINS_ALIAS_SAVE "  \"alias\"  \"path\"\n"
			,
			"-LIB                    ���浱ǰ���������⡣\n"
			"\n"
			"alias                   ����ָ���ı�����\n"
			"path                    �������·�������ļ��������Բ��Ӻ�׺����������ָ���ļ�����\n"
			,
			ALIASTYPE_STR_PATH       " ��� path ȱʡ����ʹ�ô�·����"
			,
			"ͬһ���ļ����Ա��������������ݣ�����ļ�������׷�ӣ��������򴴽���\n"
			"���ر�������ʹ�� " WDOINS_ALIAS_LOAD "����� " WDOINS_HELP " " WDOINS_ALIAS_LOAD "��\n"
			,
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_ALIAS_APPLY"  -win  CtCurWin\n"
			"." WDOINS_ALIAS_SAVE "  CtCurWin  c:\\username\\desktop\\MyWinAlias" FALIAS_EXT "\n"
		);
	}
	else if (!stricmp(_str, WDOINS_ALIAS_LOAD))
	{
		wdo_log_help(
			"����һ�������ļ���" FALIAS_EXT "����������ָ�� " WDOINS_ALIAS_SAVE " ���档"
			,
			WDOINS_ALIAS_LOAD "  \"path\"\n"
			WDOINS_ALIAS_LOAD "  (null)\n"
			,
			"path                    �����ļ���·����(���������׺������׺��Ϊ " FALIAS_EXT ")\n"
			"(null)                  ʹ��·��������\n"
			,
			ALIASTYPE_STR_PATH       " ��� path ȱʡ����ʹ�ô�·����"
			,
			"���صı���ֱ�ӷ���������У�����ֱ�Ӹ��ݱ��������ֵ��á�\n"
			"���������������������ǰ�ı�����\n"
			,
			"." WDOINS_ALIAS_LOAD "  c:\\username\\desktop\\AliasLib" FALIAS_EXT
		);
	}


	/***************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ������ -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	****************************************************************************************/

	else if (!stricmp(_str, WDOINS_PTR))
	{
		sprintf(StrPar,
			"-P       [top]          ָ����ǰǰ̨���ڡ�\n"
			"-P       [par]          ָ����ǰ���ڵĸ�����(�����)��\n"
			"-P       [act]          ָ����ǰ����ڡ�\n"
			"-P       [foc]          ָ����ǰ���н���Ĵ��ڡ�\n"
			"-P       [desk]         ָ����ǰ���洰�ڡ�\n"
			"-P       [cur]          ָ����ǰ�α����ڵĴ��ڡ�\n"
			"-P       [cp]           ָ����ǰ�α����ڵĸ����ڡ�\n"
			"-P       [rand]         ָ��һ������ĸ����ڡ�\n"
			"-P       [crand]        ָ��һ����ǰ�����ڵ�����Ӵ��ڡ�\n"
			"\n"
			"-T       title          �����ڵı����ı����Ҵ���(���Ƽ�)����󳤶�֧�� %d byte��\n"
			"-C       class          �������������Ҵ��ڡ���󳤶�֧�� %u byte��\n"
			"-F       filepath       �����ڽ���·�����Ҵ��ڡ�����·��������׺����.exe������󳤶�֧�� %u byte��\n"
			"-I       id             �����ڵľ�� ID ���Ҵ��ڡ�ʹ��ʮ������������� 8 λ����\n"
			"-D       x  y           ָ��һ�������Ĺ������ڡ�\n"
			"\n"
			"(null)                  ʹ�ô��ڱ�����\n"
			, MAXLEN_WIN_TITLE
			, MAXLEN_WIN_CLASS
			, MAX_PATH
		);

		wdo_log_help(
			"ָ����ѡ�У�һ��Ҫ�����Ĵ��ڡ��Դ��ڽ��в���ʱ����ʹ�ô�ָ��ȷ��Ҫ�������ڡ�\n"
			,
			WDOINS_PTR "  [-P [top/par/act/foc/desk/cur/cp/rand/crand]]\n"
			WDOINS_PTR "  [-T title | -C class | -F filepath | -I id | -D x y]\n"
			WDOINS_PTR "  (null)\n"
			,
			StrPar
			,
			ALIASTYPE_STR_WIN        " ���ȱʡ���в�������ָ��ô��ڡ�"
			,
			"ָ�� " WDOINS_PTR " Ĭ��ָ��ǰ " WDO_EXE_FILE " ���ڡ�\n"
			WDOINS_PTR " �����Ȳ��Ҹ����ڣ����������δ�ҵ�������Ӵ��ڡ�\n"
			"\n"
			"ʹ������ \".." WDOINS_PTR " \" ������ָ���� " WDO_EXE_FILE " �Ĵ��ڡ�\n"
			"�������д�����ʹ��ָ�� " WDOINS_LIST "����� " WDOINS_HELP " " WDOINS_LIST "��\n"
			,
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_MOVE "  20  20"
		);
	}
	else if (!stricmp(_str, WDOINS_UPDATEPTR))
	{
		wdo_log_help(
			WDOINS_PTR " �ĸ��¿��ء�"
			,
			WDOINS_UPDATEPTR "  [on/off]"
			,
			"on                      ������Ĭ�ϣ���\n"
			"off                     �رա�\n"
			,
			NULL
			,
			"��ָ����ǽ�ָֹ�� " WDOINS_PTR " ���´��ڣ�\n"
			"���ǽ�ֹ����ָ�� " WDOINS_PTR " ���ָ����� " WDOINS_PTR " ָ��Ĵ��ڡ�\n"
			"\n"
			"����ָ�" WDOINS_BOX_TEXT ", " WDOINS_BOX_INFO ", " WDOINS_OPEN " ...\n"
			"��Щ���͵�ָ���ֱ�ӽ� " WDOINS_PTR " ָ���µĴ��ڣ�\n"
			"�������Ҫ����ʹ�� " WDOINS_UPDATEPTR " ָ��رռ��ɡ�\n"
			,
			"." WDOINS_UPDATEPTR "  off"
		);
	}
	else if (!stricmp(_str, WDOINS_OPEN))
	{
		wdo_log_help(
			"��һ�����̲�ָ��һ���µĴ��ڡ�"
			,
			WDOINS_OPEN "  \"path\"\n"
			WDOINS_OPEN "  (null)\n"
			,
			"path                    ����·��������׺����.exe����\n"
			"(null)                  �ӱ����л�ȡ���ݡ�\n"
			,
			ALIASTYPE_STR_PATH		" ��� path ȱʡ���������·������ǰ���Ǹ�·�������ļ�����\n"
			ALIASTYPE_STR_WIN		" ��� path ȱʡ����ʹ�øô��ڵĽ���·�������\n"
			,
			"��� " WDOINS_UPDATEPTR " on����Ὣ " WDOINS_PTR " ָ��һ���µĴ��ڣ�\n"
			"���������� " WDOINS_PTR " ָ����ʹ�� " WDOINS_UPDATEPTR " ָ�\n"
			"��ָ��Ĵ��ڲ�һ�����´򿪽��̵Ĵ��ڣ�����һ����Դ�ڸ�·���ĳ���\n"
			"\n"
			"��ָ��ɹ��󻹻��г��������Ը�·����������д��ڣ������Ӵ��ڣ���\n"
			,
			"." WDOINS_OPEN "  c:\\wdo.exe"
		);
	}
	else if (!stricmp(_str, WDOINS_LIST))
	{
		wdo_log_help(
			"�����������д��ڣ�����ӡ������̨��"
			,
			WDOINS_LIST "  (null)\n"
			WDOINS_LIST "  [-ALL | -CT]\n"
			,
			"(null)                  �������и����ڡ�\n"
			"\n"
			"-ALL                    �������д��ڣ����������Ӵ��ڡ�\n"
			"-CT                     ��������ǰ���ڵ������Ӵ��ڡ�\n"
			,
			ALIASTYPE_STR_WIN        " �������һʹ�� -CT����ñ���ָ����ǰ���ڡ�"
			,
			NULL
			,
			"." WDOINS_LIST "  -all"
		);
	}
	else if (!stricmp(_str, WDOINS_INFO))
	{
		wdo_log_help("��ӡ " WDOINS_PTR " ָ�򴰿ڵ���Ϣ�ڵ�ǰ����̨��", WDOINS_INFO "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��", "����ʹ��ָ�� " WDOINS_BOX_INFO " ��ӡ��ͼ�λ����档", NULL);
	}
	else if (!stricmp(_str, WDOINS_TITLE))
	{
		sprintf(StrPar,
			"text                    �µı����ı�����󳤶�֧�� %u byte��"
			, MAXLEN_WIN_TITLE
		);

		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڵı����ı���"
			,
			WDOINS_TITLE "  \"text\""
			,
			StrPar
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��"
			,
			NULL
			,
			"." WDOINS_TITLE "  HelloWorld"
		);
	}
	else if (!stricmp(_str, WDOINS_TOP))
	{
		wdo_log_help("�� " WDOINS_PTR " ָ�򴰿ڷ���ǰ̨��", WDOINS_TOP "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_TOPMOST))
	{
		wdo_log_help(
			"�� " WDOINS_PTR " ָ�򴰿��ö���"
			,
			WDOINS_TOPMOST "  (null)\n"
			WDOINS_TOPMOST "  [on/off]\n"
			,
			"(null)                  ����\n"
			"\n"
			"[on]                    �ö�\n"
			"[off]                   ȡ���ö�\n"
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��"
			, NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_BOTTOM))
	{
		wdo_log_help("�� " WDOINS_PTR " ָ�򴰿��õס�", WDOINS_BOTTOM "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_ACTIVE))
	{
		wdo_log_help("�� " WDOINS_PTR " ָ�򴰿�����Ϊ����ڡ�", WDOINS_ACTIVE "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_FOCUS))
	{
		wdo_log_help("�� " WDOINS_PTR " ָ�򴰿�����Ϊ���㴰�ڡ�", WDOINS_FOCUS "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_CAPTURE))
	{
		wdo_log_help("�� " WDOINS_PTR " ָ�򴰿�����Ϊ���񴰿ڡ�", WDOINS_CAPTURE "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_TRANSP))
	{
		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڵ�͸���ȡ�"
			,
			WDOINS_TRANSP "  <percentage>"
			,
			"<percentage>            Ҫ͸���İٷֱ�[0-100]��0 = ��ȫ͸����100 = ��ȫ��͸����\n"
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��"
			,
			"����͸�����ڣ�\n"
			"        - ����Ϊ�ֲ㴰�ڣ��α�ɴ�͸��\n"
			"        - ������չ��� " lr_TO_STR(WS_EX_LAYERED) "(0x00080000)��\n"
			"        - ʹ��ָ�� " WDOINS_ERASE " �ɲ�������ĳһ��ɫ����ȫ͸������\n"
			,
			"." WDOINS_TRANSP "  20"
		);
	}
	else if (!stricmp(_str, WDOINS_ERASE))
	{
		wdo_log_help("���� " WDOINS_PTR " ָ�򴰿ڵ�ĳһ��ɫ��"
			,
			WDOINS_ERASE "  <color>"
			,
			"<color>                 Ҫ��������ɫ������λʮ��������ֵ��ɡ�"
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��"
			,
			"��ɫ���� RGB ��ɫģʽ�������ִ�Сд��\n"
			"����ʹ��ָ�� " WDOINS_INFO " �� " WDOINS_BOX_INFO " ��ȡ��ǰ�α����ڵ����ɫֵ��\n"
			"\n"
			"��ɫ  =  000000  |  ��ɫ  =  FFFFFF  |  ��ɫ  =  C0C0C0" "\n"
			"��ɫ  =  FF0000  |  ��ɫ  =  00FF00  |  ��ɫ  =  0000FF" "\n"
			"�Ϻ�  =  FF00FF  |  ����  =  7FFFD4  |  ǳ��  =  B0E0E6" "\n"
			"�ۺ�  =  FF007F  |  ����  =  CCFFE5  |  ����  =  CCE5FF" "\n"
			"��ɫ  =  FFFF00  |  ����  =  7FFF00  |  ����  =  87CEEB" "\n"
			"���  =  FFD700  |  ����  =  385E0F  |  ����  =  191970" "\n"
			"ǳ��  =  F5DEB3  |  ����  =  00C957  |  ��ɫ  =  A020F0" "\n"
			"��ɫ  =  802A2A  |  ����  =  00FF7F  |  ����  =  DA70D6" "\n"
			"�ƺ�  =  F0E68C  |  ����  =  BDFCC9  |  ��ɫ  =  00FFFF" "\n"
			,
			"." WDOINS_ERASE "  FFFFFF"
		);
	}
	else if (!stricmp(_str, WDOINS_TRANSPERASE))
	{
		wdo_log_help("���� " WDOINS_PTR " ָ�򴰿ڵ�͸���ȣ�������һ����ɫ��"
			,
			WDOINS_TRANSPERASE "  <percentage>  <color>\n"
			WDOINS_TRANSPERASE "  [-CANCEL]\n"
			,
			"<percentage>            Ҫ͸���İٷֱ�[0-100]��0 = ��ȫ͸����100 = ��ȫ��͸����\n"
			"<color>                 Ҫ��������ɫ������λʮ��������ֵ��ɡ�\n"
			"\n"
			"[-CANCEL]               ȡ������͸�����ԡ�\n"
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��"
			,
			WDOINS_TRANSPERASE " �ǽ���� " WDOINS_TRANSP " �� " WDOINS_ERASE " ָ��Ľ���塣\n"
			,
			"." WDOINS_TRANSPERASE "  50  FFFFFF\n"
			"// ���ô��� 50% ͸���������� FFFFFF ��ɫ(100% ͸��)\n"
		);
	}
	else if (!stricmp(_str, WDOINS_SEND))
	{
		wdo_log_help(
			"�� " WDOINS_PTR " ָ�򴰿ڷ����ı���Ϣ��"
			,
			WDOINS_SEND "  [-T text| -I text| -F file| -D dir| -V]\n"
			WDOINS_SEND "  [-RC text | -RT file | -RF dir]\n"
			,
			"-T       text             ����һ���ı���Ϣ��\n"
			"-I       text             ����һ���ı���Ϣ��ĩβ�Զ������У��س�����\n"
			"-F       file             ָ��һ���ļ������η������ļ��ı��������������ʽ�����ļ�������ڡ�\n"
			"-D       dir              ָ��һ��Ŀ¼�����η���Ŀ¼�е��ļ�����Ӱ��Ŀ¼�е�Ŀ¼��\n"
			"-V                        ����ճ����Ϣ����ʹ��ָ�� " WDOINS_CLIP " ���ü��������ݡ�\n"
			"-RC      text             ���� text �е�����ַ���\n"
			"-RT      file             ���� file �е�����С�\n"
			"-RF      dir              ���� dir �е�����ļ���\n"
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��"
			,
			"�ļ���ʹ�� ANSI ���룬������ܳ������������\n"
			"����ʹ��ָ�� " WDOINS_DELAY " ���������ٶȣ���ʱ����\n"
			"\n"
			"�������ø�ָ���д�Զ���QQ���ѷ�����Ϣ�Ľű������ָ�� " WDOINS_AT " ����ʵ�ֶ�ʱ���͡�\n"
			"\n"
			"ĳЩ������Ҫ��ȡ���̽�����Ϊǰ̨����ʱ���Ż���մ�����Ϣ���磺΢��\n"
			"���Ե��� " WDOINS_TOPSEND " ָ�\n"
			,
			"." WDOINS_PTR "  -c  TXGuiFoundation\n"
			"." WDOINS_SEND "  -t  Hello\n"
			"\n"
			"// TXGuiFoundation �� QQ ���ڵ�����\n"
		);

	}
	else if (!stricmp(_str, WDOINS_TOPSEND))
	{
		wdo_log_help(
			WDOINS_TOP " �� " WDOINS_SEND " �Ľ�ϡ�"
			,
			"ͬ " WDOINS_SEND
			,
			"ͬ " WDOINS_SEND
			,
			ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��"
			,
			"��ִ�� " WDOINS_TOP "����ִ�� " WDOINS_SEND "��\n"
			"���ִ��ڽ��ڴ��ڶ��㲢���м��̽���ʱ���������롣\n"
			,
			NULL
		);
	}
	else if (!stricmp(_str, WDOINS_OUT))
	{
		wdo_log_help(
			"�� " WDOINS_PTR " ָ�򴰿���������֡�"
			,
			WDOINS_OUT "  <cx>  <cy>  \"text\""
			,
			"<cx>                    ����� X ���꣬�Դ������Ͻ�Ϊԭ��(0,0)��\n"
			"<cy>                    ����� Y ���꣬�Դ������Ͻ�Ϊԭ��(0,0)��\n"
			"text                    ������ı���\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			ALIASTYPE_STR_OUT        " ���ʱ�����á�Ĭ������ʹ��ָ�� " WDOINS_ALIAS_GET " ��ȡ��\n"
			,
			"��Ҫ����ı���ͬʱһ�����ͼƬ������� " ALIASTYPE_STR_OUT " ���͵ı��������á�\n"
			,
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_ALIAS_GET "  -out  myout\n"
			"myout." WDOINS_ALIAS_SET "  -out  -tc  ff0000\n"
			"myout." WDOINS_OUT "  20  20  Hello\n"
			"\n"
			"// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
			"// �ڵ�ǰ�α���ָ��Ĵ����ϣ�\n"
			"// ʹ�ú�ɫ����ı� \"Hello\"��\n"
		);
	}
	else if (!stricmp(_str, WDOINS_DRAW))
	{
		wdo_log_help(
			"�� " WDOINS_PTR " ָ�򴰿��л�ͼ��\n"
			,
			WDOINS_DRAW "  [--RECT]     <cx>  <cy>  <cx2>  <cy2>\n"
			WDOINS_DRAW "  [--CIRCLE]   <cx>  <cy>  <radius>\n"
			WDOINS_DRAW "  [--ELLIPSE]  <cx>  <cy>  <cx2>  <cy2>\n"
			WDOINS_DRAW "  [--LINE]     <cx>  <cy>  <cx2>  <cy2>\n"
			WDOINS_DRAW "  [--LINE]     <cx>  <cy>\n"
			,
			"--rect                  ��һ�����Ρ�\n"
			"--circle                ��һ����Բ��\n"
			"--ellipse               ��һ����Բ��\n"
			"--line                  ��һ���߶λ�һ�����ߡ�\n"
			"\n"
			"<cx>                    ��һ����� X ���ꡣ\n"
			"<cy>                    ��һ����� Y ���ꡣ\n"
			"<cx2>                   �ڶ������ X ���ꡣ\n"
			"<cy2>                   �ڶ������ Y ���ꡣ\n"
			"<radius>                Բ�İ뾶����ʱ cx �� cy ��Բ�����ꡣ\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			ALIASTYPE_STR_DRAW       " �滭ʱ�����á�Ĭ������ʹ��ָ�� " WDOINS_ALIAS_GET " ��ȡ��\n"
			,
			"--line ���������ͣ���������ĸ�������ֻ��һ���߶Σ�\n"
			"  ����\n"
			"         " WDOINS_DRAW "  --line  100  100  500  500\n"
			"ֻ�������������������ߣ���Ҫ��� '{''}' ʹ�á�\n"
			"  ����\n"
			"         " WDOINS_DRAW "  --line  {10 10, 200 200, 500 20}\n"
			"  ��\n"
			"         " WDOINS_DRAW "  --line\n"
			"         {\n"
			"             10   10\n"
			"             200  200\n"
			"             500  20\n"
			"         }\n"
			"\n"
			"����ʾ�������߶�����(10,10)(200,200)(500,20)�����㣬\n"
			"���ֻ��һ���㣬��û���κ����á�\n"
			"\n"
			"\n"
			"���� [--line]����������������֧����һ��\n"
			"  ��һ��\n"
			"         " WDOINS_DRAW "\n"
			"         {\n"
			"             --circle  100  100  50\n"
			"             --circle  100  150  50\n"
			"             --circle  150  150  50\n"
			"         }\n"
			"  ������\n"
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
		wdo_log_help("�� " WDOINS_PTR " ָ��Ĵ����м���һ��ͼ��"
			,
			WDOINS_IMAGE "  <x>  <y>  \"imagepath\""
			,
			"<x>                     ���� X ���㣻\n"
			"<y>                     ���� Y ���㣻\n"
			"imagepath               ͼ��·����.bmp��\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			ALIASTYPE_STR_PATH       " ���·��ȱʡ�������·����\n"
			,
			"����Դͼ���С�����\n"
			,
			"." WDOINS_IMAGE "  0  0  C:\\image.bmp"
		);
	}
	else if (!stricmp(_str, WDOINS_IMAGES))
	{
		wdo_log_help("�� " WDOINS_PTR " ָ��Ĵ����м���һ��Ŀ¼������ͼ��"
			,
			WDOINS_IMAGES "  <x>  <y>  \"imagedir\""
			,
			"<x>                     ���� X ���㣻\n"
			"<y>                     ���� Y ���㣻\n"
			"imagedir                ͼ��Ŀ¼·���������μ���Ŀ¼�е�����ͼ��\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			ALIASTYPE_STR_PATH       " ���·��ȱʡ�������·����\n"
			,
			"����Դͼ���С�����\n"
			,
			"." WDOINS_IMAGES "  0  0  C:\\images"
		);
	}
	else if (!stricmp(_str, WDOINS_PROJECT))
	{
		wdo_log_help(
			"ͶӰ " WDOINS_PTR " ָ�򴰿ڵ�ָ�����ڡ�"
			,
			WDOINS_PROJECT "  \"alias\"  <cx>  <cy>  <w>  <h>"
			,
			"alias                   һ�� " ALIASTYPE_STR_WIN " ���͵ı�����ָ��Ŀ�괰�ڡ�\n"
			"<cx>                    ͶӰ��Ŀ��� X �����ꡣȱʡ��Ϊ 0��\n"
			"<cy>                    ͶӰ��Ŀ��� Y �����ꡣȱʡ��Ϊ 0��\n"
			"<w>                     Ŀ��Ŀ�ȣ����ȱʡ���Դ����һ����\n"
			"<h>                     Ŀ��ĸ߶ȣ����ȱʡ���Դ����һ����\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"Դ�����ǵ�ǰ " WDOINS_PTR " ָ��Ĵ��ڣ�\n"
			"�����ȡԴ���������ͻ���ͶӰ��Ŀ�괰�ڡ�ֻ���ȡһ֡��\n"
			"�ṩ w �� h ����֧��ͶӰ���š�\n"
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
			"// �����������ʵ������ \"" WDOINS_BOX_INFO " -v cur\" ��ͶӰЧ����\n"
			"// ֻ����ͶӰ�����ǵ�ǰ " WDO_EXE_FILE " ���ڡ�\n"
			"// \"." WDOINS_PROJECT "  ..\" �е� \"..\" ��һ��Ĭ�ϵĴ��ڱ�������ʾ���ǵ�ǰ�� " WDO_EXE_FILE " ���ڡ�\n"
		);
	}
	else if (!stricmp(_str, WDOINS_PARENT))
	{
		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڵ��¸����ڡ�"
			,
			WDOINS_PARENT "  \"alias\""
			,
			"alias                   �¸����ڵı��������ȱʡ��û�и����ڡ�"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"ָ�� " WDOINS_LIST " ���Ա��������ڵ������Ӵ��ڡ���� " WDOINS_HELP " " WDOINS_LIST
			,
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_PARENT "  ..\n"
			"\n"
			"// ���õ�ǰ�α����ڵĴ���Ϊ��ǰ " WDO_EXE_FILE " ���ڵ��Ӵ��ڡ�\n"
			"// \"..\" ��һ��Ĭ�ϵı�����ָ��ǰ " WDO_EXE_FILE " ���ڡ�\n"
		);
	}
	else if (!stricmp(_str, WDOINS_CHILD))
	{
		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڵ��Ӵ��ڡ�"
			,
			WDOINS_CHILD "  [-ADD]  \"alias\"  <cx>  <cy>\n"
			WDOINS_CHILD "  [-DEL]  \"alias\"\n"
			,
			"-ADD                    ���һ���Ӵ��ڡ�\n"
			"-DEL                    ɾ��һ���Ӵ��ڡ�\n"
			"\n"
			"alias                   �Ӵ��ڵı�����\n"
			"<cx>                    ��Ϊ�Ӵ��ں���� X �����꣬ȱʡ��Ϊ 0��\n"
			"<cy>                    ��Ϊ�Ӵ��ں���� Y �����꣬ȱʡ��Ϊ 0��\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"�������������ɾ�������Ӵ��ڡ�\n"
			"ɾ���Ӵ��ں��Ӵ��ڲ�����رգ�ֻ�ǳ�Ϊ�˸����ڡ�\n"
			"\n"
			"ʹ��ָ�� " WDOINS_LIST " �ɱ����Ӵ��ڡ�\n"
			,
			"." WDOINS_PTR "  -c  MyWinClass\n"
			"."	WDOINS_ALIAS_APPLY "  -win  Win_1\n"
			"." WDOINS_PTR "  -p  cur\n"
			"." WDOINS_ALIAS_APPLY "  -win  Win_2\n"
			"Win_1." WDOINS_CHILD "  -add  Win_2\n"
			"\n"
			"// Win_1 ��һ������Ϊ MyWinClass �Ĵ��ڣ�\n"
			"// Win_2 �ǵ�ǰ�α�ָ��Ĵ��ڣ�\n"
			"// Ϊ Win_1 ����Ӵ��� Win_2��\n"
		);
	}
	else if (!stricmp(_str, WDOINS_COPY))
	{
		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڵ����ݵ�����ָ���Ĵ��ڡ�"
			,
			WDOINS_COPY "  \"alias\"  [-NEW class | -TITLE | -STYLE | -SHOW | -TRANSP | -IMAGE]\n"
			WDOINS_COPY "  \"alias\"  [-W | -H | -WH | -X | -Y | -XY]\n"
			WDOINS_COPY "  (null)\n"
			,
			"alias                   Ŀ�괰�ڵı�����\n"
			"class                   ָ���´��ڵ�������ȱʡ���Զ�ѡ��\n"
			"\n"
			"-NEW                    �½�һ�����ƵĴ��ڣ����� alias ������\n"
			"-TITLE                  �����Ʊ��⡣\n"
			"-STYLE                  �����Ʒ��\n"
			"-SHOW                   ��������ʾģʽ����ʾ�����ش��ڣ���\n"
			"-TRANSP                 ������͸���ȺͲ�����ɫ������У���\n"
			"-IMAGE                  ������ͼ��һ֡����\n"
			"-W                      �����ƿ�ȡ�\n"
			"-H                      �����Ƹ߶ȡ�\n"
			"-WH                     �����ƿ�͸ߡ�\n"
			"-X                      ������ X �����ꡣ\n"
			"-Y                      ������ Y �����ꡣ\n"
			"-XY                     ���������ꡣ\n"
			"\n"
			"(null)                  ����һ�����ƴ��ڣ�ʹ��Ĭ�ϵ���������\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"���ִ����ݲ�֧��ͼ��ӳ�䡣\n"
			,
			"." WDOINS_PTR "  -c  Win_1\n"
			"." WDOINS_ALIAS_APPLY "  -win  Win1_alias\n"
			"." WDOINS_PTR "  -c  Win_2\n"
			"." WDOINS_COPY "  Win1_alias  -title\n"
			"\n"
			"//  ������Ϊ Win_1 �Ĵ����������Ϊ Win1_alias��\n"
			"//  ������Ϊ Win_2 �Ĵ��ڵı��⸴�Ƶ� Win1_alias ���ڡ�\n"
		);
	}
	else if (!stricmp(_str, WDOINS_MOVE))
	{
		wdo_log_help(
			"�ƶ� " WDOINS_PTR " ָ�򴰿ڵ����ꡣ"
			,
			WDOINS_MOVE "  <xaxis>  <yaxis>"
			,
			"<xaxis>                 �� X ���ƶ������ص㡣����ʹ��ָ�� " WDOINS_MOVEX " �����ƶ���\n"
			"<yaxis>                 �� Y ���ƶ������ص㡣����ʹ��ָ�� " WDOINS_MOVEY " �����ƶ���\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"����Ǹ����ڣ�������Ļ���ϽǶ���Ϊԭ�㣨0��0����\n"
			"������Ӵ��ڣ����Ը��������ϽǶ���Ϊԭ�㣨0��0����\n"
			"\n"
			"��� yaxis ȱʡ��Ĭ��ʹ����ֵ 0����ֵ����Ϊ������\n"
			"���ֻ���ƶ� X �ᣬ��ָ�� " WDOINS_MOVE " �� " WDOINS_MOVEX " ������\n"
			"\n"
			"�����Ҫ�������������ʹ�ô����� '{''}' �������飬ʹ�û��л� ',' �ָ���\n"
			"  ����\n"
			"          " WDOINS_MOVE " {1 2, 3 4}\n"
			"  ͬ���ڣ�\n"
			"          " WDOINS_MOVE " 1 2\n"
			"          " WDOINS_MOVE " 3 4\n"
			"\n"
			"�������������ָͬ���������ָ������'*'��ִ�д�����\n"
			"  ����\n"
			"         " WDOINS_MOVE "*3 5 2\n"
			"  ��ͬ�ڣ�\n"
			"         " WDOINS_MOVE " 5 2\n"
			"         " WDOINS_MOVE " 5 2\n"
			"         " WDOINS_MOVE " 5 2\n"
			"\n"
			"�����Ҫ����ִ�������ʹ�� " WDOINS_MOVE "*" WDO_LOOPCOUNT_STR "  5 2\n"
			"������ִ�� " WDOINS_MOVE " 5 2�����ַ�ʽ����������ָ�\n"
			"\n"
			"�������ַ�ʽ��ͬʱ���֡�\n"
			"  ����\n"
			"         " WDOINS_MOVE "*2 {1 2, 3 4}\n"
			"  ��ͬ�ڣ�\n"
			"         " WDOINS_MOVE " 1 2\n"
			"         " WDOINS_MOVE " 3 4\n"
			"         " WDOINS_MOVE " 1 2\n"
			"         " WDOINS_MOVE " 3 4\n"
			"\n"
			"ע������һ��������ָ���Ӧ�пո񣬷�����Ϊ�������롣\n"
			"ִ�д������Ϊ������ȡ�����ֵ������Ϊ 0 �� 1 �����Ч����һ���ġ�\n"
			"\n"
			"\"{ }\" ���������ͣ�һ�ִ������飬һ�ִ����ַ����顣\n"
			"���ݲ�ͬ��ָ�������\n"
			"  ���飺\n"
			"         >> ��ָ������в�����Ϊ����ʱʹ�ã�\n"
			"         >> ʹ�� ',' �� '����' �ָ\n"
			"  �ַ����飺\n"
			"         >> ��ָ������Ҫ��������֮����ַ�����ʱʹ�ã�\n"
			"         >> ʹ�� '����' �ָ\n"
			"\n"
			"�����ȷ��ʹ�����֣�����ͳһʹ�û��зָ\n"
			"\"{ }\" ֧�ֶ�����Ƕ��������ɶԳ��֡�\n"
			,
			"." WDOINS_MOVE "  10  20"
		);
	}
	else if (!stricmp(_str, WDOINS_MOVEX))
	{
		wdo_log_help(
			"�� X ���ƶ� " WDOINS_PTR " ָ�򴰿����ꡣ"
			,
			WDOINS_MOVEX "  <xaxis>"
			,
			"<xaxis>                 �� X ���ƶ������ص㡣"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			NULL
			,
			"." WDOINS_MOVEX "  10"
		);
	}
	else if (!stricmp(_str, WDOINS_MOVEY))
	{
		wdo_log_help(
			"�� Y ���ƶ� " WDOINS_PTR " ָ�򴰿����ꡣ"
			,
			WDOINS_MOVEY "  <yaxis>"
			,
			"<yaxis>                 �� Y ���ƶ������ص㡣"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			NULL
			,
			"." WDOINS_MOVEY "  10"
		);
	}
	else if (!stricmp(_str, WDOINS_POS))
	{
		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڵ����ϽǶ�������λ�á�"
			,
			WDOINS_POS "  <xaxis>  <yaxis>"
			,
			"<xaxis>                 �µ� X �����ꡣ����ʹ��ָ�� " WDOINS_POSX " �������á�\n"
			"<yaxis>                 �µ� Y �����ꡣ����ʹ��ָ�� " WDOINS_POSY " �������á�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"����Ǹ����ڣ�������Ļ���ϽǶ���Ϊԭ�㣨0��0����\n"
			"������Ӵ��ڣ����Ը��������ϽǶ���Ϊԭ�㣨0��0����\n"
			"\n"
			"��Ҫ�����ƶ���ʹ��ָ�� " WDOINS_DRIFT "����� " WDOINS_HELP " " WDOINS_DRIFT "��\n"
			,
			"." WDOINS_POS "  200  200"
		);
	}
	else if (!stricmp(_str, WDOINS_POSX))
	{
		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڵ����ϽǶ������� X λ�á�"
			,
			WDOINS_POSX "  <xaxis>"
			,
			"<xaxis>                 �µ� X �����ꡣ\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			NULL
			,
			"." WDOINS_POSX "  200"
		);
	}
	else if (!stricmp(_str, WDOINS_POSY))
	{
		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڵ����ϽǶ������� Y λ�á�"
			,
			WDOINS_POSY "  <yaxis>"
			,
			"<yaxis>                 �µ� Y �����ꡣ\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			NULL
			,
			"." WDOINS_POSY "  200"
		);
	}
	else if (!stricmp(_str, WDOINS_RPOS))
	{
		wdo_log_help("������� " WDOINS_PTR " ָ�򴰿����ꡣ", WDOINS_RPOS "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��\n", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_ZOOM))
	{
		wdo_log_help(
			"�������С " WDOINS_PTR " ָ�򴰿ڵĴ�С��"
			,
			WDOINS_ZOOM "  <width>  <height>"
			,
			"<width>                 Ҫ����Ĵ��ڿ�ȣ�����Ϊ������\n"
			"<height>                Ҫ����Ĵ��ڸ߶ȣ�����Ϊ������\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			NULL
			,
			"." WDOINS_ZOOM "  20  20"
		);

	}
	else if (!stricmp(_str, WDOINS_ZOOMX))
	{
		wdo_log_help(
			"�������С " WDOINS_PTR " ָ�򴰿ڵĿ�ȡ�"
			,
			WDOINS_ZOOMX "  <width>"
			,
			"<width>                 Ҫ����Ĵ��ڿ�ȣ���λΪ���ء�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			NULL
			,
			"." WDOINS_ZOOMX "  20"
		);
	}
	else if (!stricmp(_str, WDOINS_ZOOMY))
	{
		wdo_log_help(
			"�������С " WDOINS_PTR " ָ�򴰿ڵĸ߶ȡ�"
			,
			WDOINS_ZOOMY "  <height>"
			,
			"<height>                Ҫ����Ĵ��ڸ߶ȣ���λΪ���ء�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			NULL
			,
			"." WDOINS_ZOOMY "  20"
		);
	}
	else if (!stricmp(_str, WDOINS_SIZEX))
	{
		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڵ��¿�ȡ�"
			,
			WDOINS_SIZEX "  <width>"
			,
			"<width>                 ���ڵ��¿�ȡ���λΪ���ء�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			NULL
			,
			"." WDOINS_SIZEX "  200"
		);
	}
	else if (!stricmp(_str, WDOINS_SIZEY))
	{
		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڵ��¸߶ȡ�"
			,
			WDOINS_SIZEY "  <height>"
			,
			"<height>                ���ڵ��¸߶ȡ���λΪ���ء�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			NULL
			,
			"." WDOINS_SIZEY "  200"
		);
	}
	else if (!stricmp(_str, WDOINS_SHOW))
	{
		wdo_log_help(
			"��ʾ������ " WDOINS_PTR " ָ�򴰿ڡ�"
			,
			WDOINS_SHOW "  [on/off]"
			,
			"[on]                    ��ʾ��\n"
			"[off]                   ���ء�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"ʹ��ָ�� " WDOINS_STYLE " ����ʾ�����ش��ڲ��ֿؼ������ " WDOINS_HELP " " WDOINS_STYLE "��"
			,
			"." WDOINS_SHOW "  off"
		);

	}
	else if (!stricmp(_str, WDOINS_STYLE))
	{
		wdo_log_help(
			"���� " WDOINS_PTR " ָ�򴰿ڷ��"
			,
			WDOINS_STYLE "  [-TITLE | -MENU | -MAX | -MIN | -SB | -R | -HR | -VR | -CLOSE]  [on/off]"
			,
			"-TITLE                  ���ھ��б�������\n"
			"-MENU                   ���ھ��в˵�����\n"
			"-MAX                    �������ϽǾ�����󻯰�ť����Ҫ�˵�����\n"
			"-MIN                    �������ϽǾ�����С����ť����Ҫ�˵�����\n"
			"-SB                     ���ھ��д�С�����߿򣬿�ֱ�����α�������ڴ�С��\n"
			"-R                      ���ھ���ˮƽ�ʹ�ֱ��������\n"
			"-HR                     ���ھ���ˮƽ��������\n"
			"-VR                     ���ھ��д�ֱ��������\n"
			"-CLOSE                  �������ϽǾ��йرհ�ť����Ҫ�˵�����\n"
			"\n"
			"[on]                    ������\n"
			"[off]                   �رա�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"��ָ��ı�Ľ��Ǵ��ڷ�񡣶��ڲ��ִ��ڣ���ʵ��Ч��Ϊ׼��\n"
			"\n"
			"Ϊû�п����������Ĵ�����ӹ����������ڿ��ܲ����ᴦ���������Ϣ��\n"
			"\n"
			"�����˵�����ͬʱ�Ὺ����������\n"
			,
			"." WDOINS_STYLE "  -close  off"
		);
	}
	else if (!stricmp(_str, WDOINS_MAX))
	{
		wdo_log_help("�� " WDOINS_PTR " ָ�򴰿���󻯣��൱�ڰ��´������Ͻ���󻯰�ť������ͨ��ָ�� " WDOINS_RESTORE " ��ԭ��\n"
			, WDOINS_MAX "  (null)"
			, NULL
			, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��\n"
			, NULL
			, NULL);
	}
	else if (!stricmp(_str, WDOINS_MIN))
	{
		wdo_log_help("�� " WDOINS_PTR " ָ�򴰿���С�����൱�ڰ��´������Ͻ���С����ť��"
			, WDOINS_MIN "  (null)"
			, NULL
			, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��\n"
			, NULL
			, NULL);
	}
	else if (!stricmp(_str, WDOINS_RESTORE))
	{
		wdo_log_help("��������󻯵� " WDOINS_PTR " ָ�򴰿ڻ�ԭԭ���Ĵ�С���൱�ڰ��´������Ͻǻ�ԭ��ť��"
			, WDOINS_RESTORE "  (null)"
			, NULL
			, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��\n"
			, NULL
			, NULL);
	}
	else if (!stricmp(_str, WDOINS_REPAINT))
	{
		wdo_log_help("�ػ� " WDOINS_PTR " ָ�򴰿ڡ����䷢�� " lr_TO_STR(WM_PAINT) " ��Ϣ��"
			, WDOINS_REPAINT "  (null)"
			, NULL
			, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��\n"
			, NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_FLASH))
	{
		wdo_log_help("����������˸ " WDOINS_PTR " ָ�򴰿ڡ�"
			,WDOINS_FLASH "  (null)"
			,NULL
			,ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��\n"
			, NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_STROKE))
	{
		wdo_log_help(
			"��� " WDOINS_PTR " ָ�򴰿ڡ�"
			,
			WDOINS_STROKE "  <color>  <width>\n"
			WDOINS_STROKE "  (null)\n"
			,
			"<color>                 ��λʮ�����Ƶ� RGB ��ɫ��\n"
			"<width>                 ��߿�ȡ�\n"
			"\n"
			"(null)                  ʹ�ñ������ݡ�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			ALIASTYPE_STR_DRAW       " ���������������Ӹñ�������ȡ���ݡ�\n"
			,
			"ֻ��� " ALIASTYPE_STR_DRAW " ����ȡ�����ɫ����߿�ȣ������������ݡ�\n"
			,
			"." WDOINS_STROKE "  ff0000  5"
		);
	}
	else if (!stricmp(_str, WDOINS_FILL))
	{
		wdo_log_help(
			"��� " WDOINS_PTR " ָ�򴰿ڡ�"
			,
			WDOINS_FILL "  <color>\n"
			WDOINS_FILL "  (null)\n"
			,
			"<color>                 ��λʮ�����Ƶ� RGB ��ɫ��\n"
			"(null)                  ʹ�ñ������ݡ�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			ALIASTYPE_STR_DRAW       " ���������������Ӹñ�������ȡ���ݡ�\n"
			,
			"ֻ��� " ALIASTYPE_STR_DRAW " ����ȡ�����ɫ�������������ݡ�\n"
			,
			"." WDOINS_FILL "  ff0000"
		);
	}
	else if (!stricmp(_str, WDOINS_DRIFT))
	{
		wdo_log_help(
			"ʹ " WDOINS_PTR " ָ�򴰿ڴӵ�ǰ����Ư�Ƶ�ָ�����ꡣ"
			,
			WDOINS_DRIFT "  <x>  <y>  <speed>"
			,
			"<x>                     Ŀ��� X��ȱʡ��Ϊ 0��\n"
			"<y>                     Ŀ��� Y��ȱʡ��Ϊ 0��\n"
			"<speed>                 �˶��ٶ�[1-100]��ȱʡ��Ϊ 100��\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"���ڵ���Ŀ���ǰ�����˳� " WDO_EXE_FILE " ֮�⣬�����޷�ͣ���˶���\n"
			"�����ͬʱ���������úü���Ŀ��㣬�ô������ν���Ư���˶�����ʹ�� '{''}' ���Ρ�\n"
			"  ����\n"
			"          " WDOINS_DRIFT " {10 10 95, 100 200 95, 70 70 95}\n"
			"  ��\n"
			"          " WDOINS_DRIFT "\n"
			"          {\n"
			"               10  10  95\n"
			"               100 200 95\n"
			"               70  70  95\n"
			"          }\n"
			"\n"
			"��������ʾ���������ô��ڴӵ�ǰλ���� 5ms �˶�һ�ε��ٶȣ�\n"
			"��Ư�Ƶ� (X- 10,Y- 10)���ڴ� (X- 10,Y- 10) �� (X- 100,Y- 200)��\n"
			"��󵽴� (X- 70,Y- 70) ͣ�¡�\n"
			"\n"
			"��Ϊ��ָ���࿪һ���̣߳������벻Ҫʹ���������͵����\n"
			"���򴰿ڿ�����Զ�޷�����Ŀ��㣬�Ӷ�������ѭ�������δ֪���⡣\n"
			"  ����\n"
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
			"ʹ " WDOINS_PTR " ָ�򴰿ڸ����α��ƶ���"
			,
			WDOINS_FOLLOW "  <x>  <y>\n"
			WDOINS_FOLLOW "  [-C]\n"
			,
			"<x>                     ����ƫ���α� X ���������ص㡣ȱʡ��Ϊ 0��\n"
			"<y>                     ����ƫ���α� Y ���������ص㡣ȱʡ��Ϊ 0��\n"
			"\n"
			"-C                      ȡ�����档\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"��ȡ������ǰ����ͬһ�����ڽ��ж��θ��棬\n"
			"ֻ������� xy ����ֵ��������һ���̡߳�\n"
			,
			"." WDOINS_FOLLOW "  20  20"
		);
	}
	else if (!stricmp(_str, WDOINS_WIGGLE))
	{
		wdo_log_help(
			"ʹ " WDOINS_PTR " ָ�򴰿ڽ����𶯡�"
			,
			WDOINS_WIGGLE "  <range>  <frequency>\n"
			WDOINS_WIGGLE "  [-C]\n"
			,
			"<range>                 �𶯵ķ�Χ(��Բ�뾶)��\n"
			"<frequency>             �𶯵�Ƶ��(ÿ�˶�һ�ν��ȴ��ĺ�����)��\n"
			"\n"
			"-C                      �ر��𶯡�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"��ȡ����ǰ����ͬһ�����ڽ��ж����𶯣�\n"
			"ֻ������� range �� frequency ����ֵ��������һ���̡߳�\n"
			,
			"." WDOINS_WIGGLE "  5  10"
		);
	}
	else if (!stricmp(_str, WDOINS_ASIDE))
	{
		wdo_log_help(
			"ʹ " WDOINS_PTR " ָ�򴰿ڿ�������򵯳���"
			,
			WDOINS_ASIDE "  <speed>  <rx>  <ry>\n"
			WDOINS_ASIDE "  [-C]\n"
			,
			"<speed>                 �ƶ��ٶ�[1-10]��ȱʡ��Ϊ 1��\n"
			"<rx>                    X �ᱣ���������¶������Ŀ�ȡ�ȱʡ��Ϊ 0��\n"
			"<ry>                    Y �ᱣ���������¶������ĸ߶ȡ�ȱʡ��Ϊ 0��\n"
			"\n"
			"-C                      �رա�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"��ȡ������ǰ����ͬһ�����ڽ��ж��ο��ߣ�\n"
			"ֻ������� speed �� rx �� ry ����ֵ��������һ���̡߳�\n"
			"\n"
			"��Ҫ�������ѵ���Ļ��Ե�ſɲ鿴����Ч����\n"
			,
			"." WDOINS_ASIDE "  10  20  20"
		);
	}
	else if (!stricmp(_str, WDOINS_PUSH))
	{
		wdo_log_help(
			"ʹ " WDOINS_PTR " ָ�򴰿ڿ��Ա��α��ƶ���"
			,
			WDOINS_PUSH "  (null)\n"
			WDOINS_PUSH "  [-C]\n"
			,
			"(null)                  ��ʼ�ƶ���\n"
			"-C                      �رա�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"��ȡ���ƶ�ǰ����ͬһ�����ڽ��ж����ƶ������ָ��û���κ�Ӱ�졣\n"
			,
			"." WDOINS_PUSH
		);
	}
	else if (!stricmp(_str, WDOINS_COLLIDE))
	{
		wdo_log_help(
			"ʹ " WDOINS_PTR " ָ�򴰿ں��α���ײʱ�����¼���"
			,
			WDOINS_COLLIDE "  [-C]\n"
			WDOINS_COLLIDE "  [-MW | -MC | -RM]  <distance>  <backpixel>\n"
			,
			"-C                      ȡ����ײ�¼���\n"
			"\n"
			"-MW                     ��ײ�󵯿����ڣ�\n"
			"-MC                     ��ײ�󵯿��αꣻ\n"
			"-RM                     ��ײ�󵯿����ں��αꡣ\n"
			"\n"
			"<distance>              �����ľ��루��λ�����أ���\n"
			"<backpixel>             �ص������ص㣬����������Ļ��Եʱ���ص���\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"ͬһ�����ڲ������ö����ײ�¼���\n"
			"�������ʱ��������һ�ε����á�\n"
			,
			"." WDOINS_COLLIDE "  -mw  100 20"
		);
	}
	else if (!stricmp(_str, WDOINS_LOCKPAINT))
	{
		wdo_log_help("���� " WDOINS_PTR " ָ�򴰿ڣ�ʹ���ֹ��ͼ��"
			, WDOINS_LOCKPAINT "  (null)"
			, NULL
			, ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			, NULL
			, NULL);
	}
	else if (!stricmp(_str, WDOINS_CLOSE))
	{
		wdo_log_help(
			"�ر� " WDOINS_PTR " ָ�򴰿ڻ�رմ����������̡�"
			,
			WDOINS_CLOSE "  (null)\n"
			WDOINS_CLOSE "  [-C | -D]\n"
			,
			"(null)                  �رմ��ڣ�ͨ���򴰿ڷ��� " lr_TO_STR(WM_CLOSE) "(0x0010) ��Ϣ��\n"
			"-C                      �رմ����������̡�\n"
			"-D                      ���ٴ��ڡ�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			"�رմ��ڣ�\n"
			"        - �൱�ڰ��´������Ͻǹرհ�ť��\n"
			"        - ���ڿ��Ծܾ��رգ���ѯ���Ƿ񱣴���δ������ļ���\n"
			"        - ���ڿ��ܲ���رգ����߲��������رգ�\n"
			"        - ���ڹرպ󣬳�����ܻ��������У�\n"
			"�رս��̣�\n"
			"        - ֱ�ӹرմ��������Ľ��̣�\n"
			"        - ͬ���̵Ĵ��ڶ����رգ�\n"
			"        - ������ں�̨����\n"
			"        - ִ�иò�����Ҫ��Ȩ�޲��ܵ��ڶԷ���\n"
			"���ٴ��ڣ�\n"
			"        - ������ " lr_TO_STR(WM_CLOSE) "(0x0010)��\n"
			"        - ֱ�����ٶԷ����ڲ����� " lr_TO_STR(WM_DESTROY) "(0x0002) ��Ϣ��\n"
			,
			"." WDOINS_CLOSE "  -c"
		);
	}

	/***************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ �α��� -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	****************************************************************************************/

	else if (!stricmp(_str, WDOINS_CUR_MOVE))
	{
		wdo_log_help(
			"�ƶ��αꡣ"
			,
			WDOINS_CUR_MOVE "  <x>  <y>\n"
			WDOINS_CUR_MOVE "  (null)\n"
			,
			"<x>                     Ҫ�ƶ��� X ����ֵ�������Ǹ�����\n"
			"<y>                     Ҫ�ƶ��� Y ����ֵ�������Ǹ�����\n"
			"\n"
			"(null)                  ��ʾ��ǰ�α����ꡣ\n"
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
			"�ƶ��α� X �����ꡣ"
			,
			WDOINS_CUR_MOVEX "  <x>"
			,
			"<x>                     Ҫ�ƶ��� X ����ֵ��\n"
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
			"�ƶ��α� Y �����ꡣ"
			,
			WDOINS_CUR_MOVEY "  <y>"
			,
			"<y>                     Ҫ�ƶ��� Y ����ֵ��\n"
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
			"�����α����ꡣ"
			,
			WDOINS_CUR_POS "  <x>  <y>"
			,
			"<x>                     �µ� X �����ꡣȱʡ��Ϊ 0��\n"
			"<y>                     �µ� Y �����ꡣȱʡ��Ϊ 0��\n"
			,
			NULL
			,
			"��Ҫ�����ƶ���ʹ��ָ�� " WDOINS_CUR_DRIFT "����� " WDOINS_HELP " " WDOINS_CUR_DRIFT "��\n"
			,
			"." WDOINS_CUR_POS "  200  200"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_POSX))
	{
		wdo_log_help(
			"�����α� X �����ꡣ"
			,
			WDOINS_CUR_POSX "  <x>"
			,
			"<x>                     �µ� X �����ꡣ\n"
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
			"�����α� Y �����ꡣ"
			,
			WDOINS_CUR_POSY "  <y>"
			,
			"<y>                     �µ� Y �����ꡣ\n"
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
		wdo_log_help("��������α����ꡣ", WDOINS_CUR_RPOS "  (null)", NULL, NULL, NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_CUR_CPOS))
	{
		wdo_log_help(
			"�� " WDOINS_PTR " ָ�򴰿����Ͻ�Ϊԭ��(0,0)�������α����ꡣ"
			,
			WDOINS_CUR_CPOS "  <cx>  <cy>"
			,
			"<cx>                    �µ� X �����ꡣȱʡ��Ϊ 0��\n"
			"<cy>                    �µ� Y �����ꡣȱʡ��Ϊ 0��\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ��\n"
			,
			NULL
			,
			"." WDOINS_CUR_CPOS "  20  20"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_CRPOS))
	{
		wdo_log_help("�� " WDOINS_PTR " ָ�򴰿�Ϊ������������α����ꡣ", WDOINS_CUR_CRPOS "  (null)", NULL, ALIASTYPE_STR_WIN " " WDOINS_PTR " ��ʱָ��\n", NULL, NULL);
	}
	else if (!stricmp(_str, WDOINS_CUR_KEY))
	{
		wdo_log_help(
			"ģ�ⰴ���α����"
			,
			WDOINS_CUR_KEY "  [--LEFT | --RIGHT | --MIDDLE]\n"
			WDOINS_CUR_KEY "  [--LEFT_TWICE | --RIGHT_TWICE | --MIDDLE_TWICE]\n"
			WDOINS_CUR_KEY "  [--ROLLER_UPPER | --ROOLER_DOWN | --ROLLER_LEFT | --ROLLER_RIGHT]\n"
			,
			"--left                  ���������\n"
			"--right                 �����Ҽ���\n"
			"--middle                �����м���\n"
			"\n"
			"--left_twice            ˫�������\n"
			"--right_twice           ˫���Ҽ���\n"
			"--middle_twice          ˫���м���\n"
			"\n"
			"--roller_upper          �����ϻ���\n"
			"--roller_down           �����»���\n"
			"--roller_left           �����󻮡�\n"
			"--roller_right          �����һ���\n"
			,
			NULL
			,
			"��ָ����ͨ�����ʹ�����Ϣ�ķ�ʽ�ﵽģ���α갴����\n"
			"���ǲ����Ƿ��͵� " WDOINS_PTR " ָ��Ĵ��ڣ����ǵ�ǰ�α����ڵĴ��ڡ�\n"
			"\n"
			"��������ָ���˫�������������������ģ��뾡��ѡ����ߡ�\n"
			"  ����\n"
			"         " WDOINS_CUR_KEY "*2  --left\n"
			"         " WDOINS_CUR_KEY "  --left_twice\n"
			"\n"
			"����һ��ֻ�Ử��һ�Σ���λ�������ʹ������ָ���ù������»��� 10 �Ρ�\n"
			"  ����\n"
			"         " WDOINS_CUR_KEY "*10  --roller_down\n"
			,
			"." WDOINS_CUR_KEY "  --left"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_DRIFT))
	{
		wdo_log_help(
			"ʹ�α����Ư���˶���"
			,
			WDOINS_CUR_DRIFT "  <x>  <y>  <speed>"
			,
			"<x>                     Ŀ��� X��\n"
			"<y>                     Ŀ��� Y��\n"
			"<speed>                 �˶��ٶ�[1-100]��ȱʡ��Ϊ 100��\n"
			,
			NULL
			,
			"��� " WDOINS_HELP " " WDOINS_CUR_DRIFT "��"
			,
			"." WDOINS_CUR_DRIFT "  200  200  90"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_WIGGLE))
	{
		wdo_log_help(
			"ʹ�α�����𶯡�"
			,
			WDOINS_CUR_WIGGLE "  <range>  <frequency>\n"
			WDOINS_CUR_WIGGLE "  [-C]\n"
			,
			"<range>                 �𶯵ķ�Χ(��Բ�뾶)��\n"
			"<frequency>             �𶯵�Ƶ��(ÿ�˶�һ�ν��ȴ��ĺ�����)��\n"
			"\n"
			"-C                      �ر��𶯡�\n"
			,
			NULL
			,
			"��ȡ����ǰ����ͬһ�����ڽ��ж����𶯣�\n"
			"ֻ������� range �� frequency ����ֵ��������һ���̡߳�\n"
			,
			"." WDOINS_CUR_WIGGLE "  5  10"
		);
	}
	else if (!stricmp(_str, WDOINS_CUR_RECT))
	{
		wdo_log_help(
			"�����α��ƶ�������һ�������ڡ�"
			,
			WDOINS_CUR_RECT "  [-WIN]\n"
			WDOINS_CUR_RECT "  <x>  <y>  <w>  <h>"
			,
			"-WIN                    ����Ϊ��ǰ���ھ��Ρ�\n"
			"\n"
			"<x>                     ���Ͻ� X �����ꡣ\n"
			"<y>                     ���Ͻ� Y �����ꡣ\n"
			"<w>                     ���ο�ȡ�\n"
			"<h>                     ���θ߶ȡ�\n"
			,
			ALIASTYPE_STR_WIN        " " WDOINS_PTR " ��ʱָ�����Я�� [-WIN] ����ʱ��Ч��\n"
			,
			"������������������ơ�"
			,
			"." WDOINS_CUR_RECT "  200  200  500  500"
		);
	}

	/*****************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ �½����� -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	******************************************************************************************/
	
	else if (!stricmp(_str, WDOINS_BOX_BLANK))
	{
		wdo_log_help(
			"�½�һ���հ׿򣬲����ػ档"
			,
			WDOINS_BOX_BLANK "  <width>  <height>\n"
			,
			"<width>                 �´��ڿ�ȡ�\n"
			"<height>                �´��ڸ߶ȡ�\n"
			,
			ALIASTYPE_STR_BOX      " �½����ڲ��õĴ��ڣ������á�\n"
			,
			"��ʹ�� {} ����һ�����飬ʹ�� ',' �� ���зָ�������������ͬ��С�Ĵ��ڡ�\n"
			,
			"." WDOINS_BOX_BLANK "  520  520"
		);
	}

	else if (!stricmp(_str, WDOINS_BOX_TEXT))
	{
		wdo_log_help(
			"�½�һ���ı�����ʾ���֡�"
			,
			WDOINS_BOX_TEXT "  {text}"
			,
			"text                    �ı����е��ı����ɴ����У���\n"
			,
			ALIASTYPE_STR_BOX      " �½����ڲ��õĴ��ڣ������á�\n"
			,
			"���ڵĳ�ʼ����Ϊ��ǰ���α����ꡣ\n"
			"\n"
			"���Ҫ��;���Ĵ������ã������´����±�������� " WDOINS_HELP " " WDOINS_ALIAS_GET " �� " WDOINS_HELP " " WDOINS_ALIAS_SET " �� " WDOINS_HELP " " WDOINS_ALIAS_SETUP ".\n"
			"\n"
			"��ָ�����ĵ�ǰ " WDOINS_PTR " �Ĵ���ָ�򣬻����Ϊ�´������ı��򴰿ڡ�\n"
			"�������Ҫ��ʹ��ָ�� " WDOINS_UPDATEPTR "�������������Ϊ off ���ɡ���� " WDOINS_HELP " " WDOINS_UPDATEPTR ".\n"
			"\n"
			"���Ĵ����ı���ο�ָ�� " WDOINS_HELP " " WDOINS_BOX_SET_TEXT ".\n"
			,
			"." WDOINS_BOX_TEXT "  Hello"
		);
	}
	else if (!stricmp(_str, WDOINS_BOX_INFO))
	{
		wdo_log_help(
			"�½�һ����Ϣ���Բ鿴ָ�����ڵ���Ϣ��"
			,
			WDOINS_BOX_INFO "  [-S | -V]  [ct/par/top/desk/cur/cp]"
			,
			"-S                      ��̬��Ϣ�򡣲�����´���ID��\n"
			"-V                      ��̬��Ϣ�򡣳������¡�\n"
			"\n"
			"[ct]                    " WDOINS_PTR " ��ǰָ��Ĵ��ڡ�\n"
			"[par]                   " WDOINS_PTR " ��ǰָ�򴰿ڵĸ����ڡ�\n"
			"[top]                   ��ǰǰ̨���ڡ�\n"
			"[desk]                  ��ǰ���洰�ڡ�\n"
			"[cur]                   ��ǰ�α�ָ��Ĵ��ڡ�\n"
			"[cp]                    ��ǰ�α�ָ��Ĵ��ڵĸ����ڡ�\n"
			,
			ALIASTYPE_STR_WIN       " " WDOINS_PTR " ��ʱָ��\n"
			ALIASTYPE_STR_BOX       " �½����ڲ��õĴ��ڣ������á�\n"
			,
			"���ڵĳ�ʼ����Ϊ��ǰ���α����ꡣ\n"
			"\n"
			"��ָ�����ĵ�ǰ " WDOINS_PTR " �Ĵ���ָ�򣬻����Ϊ�´�������Ϣ�򴰿ڡ�\n"
			"�������Ҫ��ʹ��ָ�� " WDOINS_UPDATEPTR "�������������Ϊ [off] ���ɡ���� " WDOINS_HELP " " WDOINS_UPDATEPTR ".\n"
			"\n"
			"����ʹ��ָ�� " WDOINS_PROJECT " ʵ����Ϣ��Ĵ���ͶӰЧ������� " WDOINS_HELP " " WDOINS_PROJECT ".\n"
			,
			"." WDOINS_BOX_INFO "  -v  cur"
		);
	}
	else if (!stricmp(_str, WDOINS_BOX_LOG))
	{
		wdo_log_help(
			"�½�һ����־����ʾ��־��\n"
			,
			WDOINS_BOX_LOG "  {text}"
			,
			"text                    ��־�ı����ɴ����У���\n"
			,
			ALIASTYPE_STR_BOX       " �½����ڲ��õĴ��ڣ������á�\n"
			,
			"���ڵĳ�ʼ����Ϊ��ǰ���α����ꡣ\n"
			"\n"
			"���Ҫ��;���Ĵ������ã������´����±�������� " WDOINS_HELP " " WDOINS_ALIAS_GET " �� " WDOINS_HELP " " WDOINS_ALIAS_SET " �� " WDOINS_HELP " " WDOINS_ALIAS_SETUP ".\n"
			"\n"
			"��ָ�����ĵ�ǰ " WDOINS_PTR " �Ĵ���ָ�򣬻����Ϊ�´������ı��򴰿ڡ�\n"
			"�������Ҫ��ʹ��ָ�� " WDOINS_UPDATEPTR "�������������Ϊ off ���ɡ���� " WDOINS_HELP " " WDOINS_UPDATEPTR ".\n"
			"\n"
			"���Ĵ����ı���ο�ָ�� " WDOINS_HELP " " WDOINS_BOX_SET_TEXT ".\n"
			,
			"." WDOINS_BOX_TEXT "  Hello"
		);
	}
	else if (!stricmp(_str, WDOINS_BOX_IMAGE))
	{
		wdo_log_help(
			"�½�һ��ͼƬ����ʾͼƬ��"
			,
			WDOINS_BOX_IMAGE "  \"path\""
			,
			"path                    ͼƬ��·����ȱʡ����������·��������"
			,
			ALIASTYPE_STR_PATH      " ��� path ȱʡ����ʹ�ø�·�����档\n"
			ALIASTYPE_STR_BOX       " �½����ڲ��õĴ��ڣ������á�\n"
			,
			"���ڵĳ�ʼ����Ϊ��ǰ���α����ꡣ\n"
			"\n"
			"��ָ�����ĵ�ǰ " WDOINS_PTR " �Ĵ���ָ�򣬻����Ϊ�´�����ͼƬ�򴰿ڡ�\n"
			"�������Ҫ��ʹ��ָ�� " WDOINS_UPDATEPTR "�������������Ϊ off ���ɡ���� " WDOINS_HELP " " WDOINS_UPDATEPTR ".\n"
			"\n"
			"��ָ��ʹ�� " ALIASTYPE_STR_BOX " ���͵ı�����������ֻʹ�����еĴ����������ݡ�\n"
			"\n"
			"���� " WDOINS_BOX_IMAGE " ��\n"
			"\t1:> �һ��ô�������λ�ÿ��ػ�ˢ�¸ô��ڣ�\n"
			"\t2:> ����ô�������λ�ÿɻ������ߣ�\n"
			"\t3:> ���м��̽���ʱ�������������ַ��ڵ�ǰ�α��λ�ã�\n"
			"\t4:> �ô��ڲ�������������뷨���棬���������������ģ�\n"
			,
			"." WDOINS_BOX_IMAGE "  c:\\lr.bmp"
		);
	}
	else if (!stricmp(_str, WDOINS_BOX_BLUE))
	{
		wdo_log_help(
			"�½�һ�������򲢽�ֹ�رա�"
			,
			WDOINS_BOX_BLUE "  \"text\""
			,
			"text                    ����������ʹ�õ��ı���"
			,
			ALIASTYPE_STR_BOX      " �½����ڲ��õĴ��ڣ������á�\n"
			,
			"��ָ�����ĵ�ǰ " WDOINS_PTR " �Ĵ���ָ�򣬻����Ϊ�´����������򴰿ڡ�\n"
			"�������Ҫ��ʹ��ָ�� " WDOINS_UPDATEPTR "�������������Ϊ off ���ɡ���� " WDOINS_HELP " " WDOINS_UPDATEPTR ".\n"
			"\n"
			"��ָ����һ��α����Ч�������ڴ���ʱ��\n"
			"������������������ͽ��ó� " WDO_EXE_FILE " ����´򿪵� cmd ���ڣ�\n"
			"���������������ں��αꣻ\n"
			"����������޷��ı��С��\n"
			"����ÿ��һС��ʱ���ö�����ȡһ�μ��̽��㡣\n"
			"\n"
			"��ָ��û���κ�Σ����һ�и��Ľ��ڹرմ��ڻ�رս���ʱ��ԭ��\n"
			"����ʹ�ÿ�ݼ� " WDO_EXIT_HOTKEY_STR " һ���ر� " WDO_EXE_FILE " ���̡�\n"
			,
			"." WDOINS_BOX_BLUE "  ��������"
		);
	}
	
	else if (!stricmp(_str, WDOINS_BOX_SET_TEXT))
	{
		wdo_log_help(
			"���ÿ����ı���\n"
			,
			WDOINS_BOX_SET_TEXT "  \"text\""
			,
			"text                    ���ı�\n"
			,
			NULL
			,
			"������� " WDO_EXE_FILE " ������\n"
			"����ֲ�������ȥ����Ե '{' '}'��\n"
			"��Ҫ������ʹ�� '{' '}' ������\n"
			,
			WDOINS_BOX_TEXT "  Hello Wrold!\n"
			WDOINS_BOX_SET_TEXT "  {�µ���һ�ı���\n"
			"�µ��ж��ı���}\n"
		);
	}

	/*****************************************************************************************
	* ins-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ����ָ�� -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+start *
	******************************************************************************************/

	else if (!stricmp(_str, WDOINS_LOOP))
	{
		wdo_log_help(
			"ѭ��ִ��һ������顣"
			,
			WDOINS_LOOP "  {instructions}"
			,
			"{instructions}          ѭ��ִ�е� wdo ָ���\n"
			,
			NULL
			,
			"��ָ����Բ��õݹ���á�\n"
			"  ����\n"
			"         " WDOINS_LOOP "*5  " WDOINS_LOOP "*3  " WDOINS_LOOP "  " WDOINS_MOVEY "*2  1\n"
			"����ʾ���ܹ�ִ���� 30(5*3*2=30) �� \"" WDOINS_MOVEY "  1\"��\n"
			"���ǲ�����д������д������Ϊ�����������ڴ�ͼ��������ٶ��⣬���κ����塣\n"
			,
			"." WDOINS_LOOP "*" WDO_LOOPCOUNT_STR "\n"
			"{\n"
			"    " WDOINS_PTR "  -p  cur\n"
			"    " WDOINS_MOVEY "*10  1\n"
			"    " WDOINS_MOVEY "*10  -1\n"
			"}\n"
			"\n"
			"// ʹ��ǰ�α�ָ��Ĵ��ڽ��������ƶ���\n"
			"// \"*" WDO_LOOPCOUNT_STR "\" ��ʾ����ѭ����\n"
		);
	}
	else if (!stricmp(_str, WDOINS_RANDOMRUN))
	{
		wdo_log_help(
			"���ִ��һ��������е�һ�����"
			,
			WDOINS_RANDOMRUN "  {instructions}"
			,
			"{instructions}          ���ִ�е� wdo ָ���\n"
			,
			NULL
			,
			"��ָ����Բ��õݹ���á�\n"
			,
			"." WDOINS_RANDOMRUN "\n"
			"{\n"
			"    " WDOINS_MOVEX " 10\n"
			"    " WDOINS_MOVEY " 10\n"
			"}\n"
			"// ���ִ�� " WDOINS_MOVEX " ���� " WDOINS_MOVEY "\n"
		);
	}
	else if (!stricmp(_str, WDOINS_QUEUE))
	{
		wdo_log_help(
			"��һ�������붨ʱ���еĶ��С�"
			,
			WDOINS_QUEUE "  {instructions}"
			,
			NULL
			,
			"{instructions}          Ҫ����� wdo ָ�\n"
			,
			"��ָ�������ָ�� " WDOINS_AT " ʹ�ã���� " WDOINS_HELP " " WDOINS_AT "��"
			,
			NULL
		);
	}
	else if (!stricmp(_str, WDOINS_AT))
	{
		wdo_log_help(
			"���ö�ʱ���ж��е�����ʱ�䡣"
			,
			WDOINS_AT "  <hour>  <minute>  <second>"
			,
			"<hour>                  Сʱ[0-23]��\n"
			"<minute>                ����[0-59]��\n"
			"<second>                ����[0-59]��\n"
			,
			NULL
			,
			"�벻Ҫ�� " WDOINS_QUEUE " �� " WDOINS_AT " ָ��ʹ�ö��߳�(" WDOINS_THREAD ")���ã���Ϊ���ޱ�Ҫ��\n"
			"��ͬһ��ʱ�䣬ֻ������һ�� " WDOINS_QUEUE " ���ж��У����� " WDOINS_AT " ʱ��գ�\n"
			"������ " WDOINS_AT " ָ��ǰ�����е� " WDOINS_QUEUE " ָ�����Ķ���ͬһ�����ж��У�\n"
			"\n"
			"��ָ��һ����� " WDOINS_QUEUE " ʹ�ã����ڶ�ʱ��QQ����΢�ź��ѷ�����Ϣ��\n"
			"���� " WDOINS_AT " ָ��ֻ����Чһ�Ρ���Ҫ���๦�ܣ���ο� " WDOINS_CMD " ָ�� schtasks.exe\n"
			,
			"." WDOINS_PTR "  -c  TXGuiFoundation"
			"." WDOINS_QUEUE "\n"
			"{\n"
			"    " WDOINS_SEND "  -i  �˺��ˡ�\n"
			"    " WDOINS_SEND "  -i  ���Ϻã�\n"
			"}\n"
			"." WDOINS_AT "  8  0  0"
			"\n"
			"// �������ϰ˵�������������Ϣ������Ϊ \"TXGuiFoundation\" �Ĵ��ڡ�\n"
			"// \"TXGuiFoundation\" һ����QQ�����촰�ڵ�������\n"
		);
	}
	else if (!stricmp(_str, WDOINS_HOTKEY))
	{
		wdo_log_help(
			"����ȫ�ֿ�ݼ�������顣"
			,
			WDOINS_HOTKEY "  [[ACSW]+'key']  {instructions}"
			,
			"[ACSW]                  A �� C �� S �� W ��������ϣ�������ۡ�\n"
			"'key'                   ����������һ�������ϵİ�����ȱʡ��ʹ�ÿո�\n"
			"\n"
			"{instructions}          ���¿�ݼ���ִ�е� wdo ָ������ȱʡ����ȡ���ÿ�ݼ���\n"
			,
			NULL
			,
			"���� [ACSW]��\n"
			"        A:    Alt ��\n"
			"        C:    Ctrl ��\n"
			"        S:    Shift ��\n"
			"        W:    Windows ��\n"
			"���� 'key':\n"
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
			"        'key' �����������б�������һ��ѡ�ÿ��ѡ�����һ��������\n"
			"        ��ʱ��֧�������б���û�е��ַ���\n"
			"ʾ��:\n"
			"        AC+R        <Alt + Ctrl + R>\n"
			"        AS+9        <Alt + Shift + 9>\n"
			"        C+-         <Ctrl + ->\n"
			"        ACS+F1      <Alt + Ctrl + Shift + F1>\n"
			"ע��:\n"
			"       '+' �����߲����пո�\n"
			"       'key' ����ȱʡ������ '+' �ű�����ڣ�\n"
			"       �벻Ҫ����ʹ�� Shift �������Ķ����������д��\n"
			"       ����ʹ��ϵͳ���еĿ�ݼ�����Ϊ����ȫ�ֵģ�\n"
			"       Windows ����Ӧ����ϵͳ�������ʹ�ã�\n"
			,
			"." WDOINS_HOTKEY "  AC+R\n"
			"{\n"
			"    " WDOINS_RPOS "\n"
			"}\n"
			"// ����ȫ�ֿ�ݼ� <Alt + Ctrl + R>�����º�ִ�� " WDOINS_RPOS "����\n"
		);
	}
	else if (!stricmp(_str, WDOINS_THREAD))
	{
		wdo_log_help(
			"��һ���߳�ִ��һ������顣"
			,
			WDOINS_THREAD "  {instructions}"
			,
			"{instructions}          ���߳�ִ�е� wdo ָ���\n"
			,
			NULL
			,
			"Ϊ��Ч�ʿ��ǣ�ִ�ж��߳�������е�ָ��ʱ�����Զ��رջ��ԣ�\n"
			"����Ӱ�쵱ǰ���̡߳�\n"
			"���Ҫ��ȡ�����������̲߳���������ٷ�����߳��С�\n"
			"\n"
			"��ָ����Բ��õݹ���á�\n"
			"  ����\n"
			"         " WDOINS_THREAD "*5  " WDOINS_THREAD "*3  " WDOINS_THREAD "  " WDOINS_MOVEY "*2  1\n"
			"���������Ȼ�Ϸ������ǲ�����ʹ�ã����ö��̵߳ı����Ǽӿ������ٶȻ�ͬһʱ��������£�\n"
			"������������������ڴ�ͼ��������ٶ��⣬���κ����塣\n"
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
			"// ������һ�������˶���С������"
		);
	}

	else wdo_log(WDOINS_HELP, OUTLOG_ERROR, wdo_language("Instruction %s not found!", "û���ҵ� \"%s\" ָ�") "\n", _str);
}

#endif