// wdo_help_english.c -- For wdo_help() function
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include "../Head/lrWindowDo.h"
#include "../Head/lr_wdo.h"

#if 0//WDO_LANGUAGE

#pragma warning (disable : 4996)


void wdo_help_english(const char* const _str)
{
	if (_str == NULL || _str[0] == '\0')
	{
		wdo_log(NULL,OUTLOG_NORMAL,
			"\n"
			"Instruction set:\n"
			"-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
			"echo           Set echo switch.\n"
			"ptr            Point to a window.\n"
			"alias          Request an alias.\n"
			"set            Set alias data.\n"
			"list           Find all windows and print.\n"
			"info           Displays the designated window information.\n"
			"title          Change the text of the designated window title.\n"
			"transp         Set designated window transparency.\n"
			"erase          Erase a color in the window.\n"
			"send           Send message to designated window.\n"
			"clip           Set Clipboard data text.\n"
			"pause          Pause program (ms).\n"
			"delay          Set delay (ms).\n"
			"move           Move designated window.\n"
			"movex          Moves the X axis of the designated window.\n"
			"movey          Moves the Y axis of the designated window.\n"
			"pos            Set designated window position.\n"
			"posx           Sets the X axis position of the designated window.\n"
			"posy           Sets the Y axis position of the designated window.\n"
			"size           Scale designated window size.\n"
			"sizex          Scale the designated window width.\n"
			"sizey          Scale the designated window height.\n"
			"width          Sets the designated window width.\n"
			"height         Sets the designated window height.\n"
			"copy           Copy designated window.\n"
			"show           Show or hide the designated window.\n"
			"order          Change the z-order of the designated window.\n"
			"top            Set window as foreground window.\n"
			"max            Maximize the designated window.\n"
			"min            Minimize the designated window.\n"
			"restore        Restore designated window.\n"
			"repaint        Repaint designated window.\n"
			"drift          Causes the designated window to drift.\n"
			"follow         Causes the designated window to follow the cursor.\n"
			"wiggle         Causes the designated window to wiggle.\n"
			"aside          Bring the designated window to the side or pop up.\n"
			"lock           Lock designated window.\n"
			"close          Closes the designated window or window process.\n"
			"curmove        Move the designated cursor.\n"
			"curmovex       Move the X coordinate of the designated cursor.\n"
			"curmovey       Move the Y coordinate of the designated cursor.\n"
			"curpos         Sets the designated cursor position.\n"
			"curposx        Sets the X axis position of the designated cursor.\n"
			"curposy        Sets the Y axis position of the designated cursor.\n"
			"curdrift       Make the cursor drift.\n"
			"loadalias      Load box alias.\n"
			"textbox        Create a new text window and display text.\n"
			"infobox        Create a new information box to view the information of the designated window.\n"
			"music          Play music.\n"
			"call           Call wdo file.\n"
			"cmd            Call cmd instruction.\n"
			"mci            Call mci instruction.\n"
			"exit           Exit wdo.exe.\n"
			"\n"
			"special instruction sets:\n"
			"-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
			"loop           Put the instruction in a Loop queue.\n"
			"loopend        Run the statements in the Loop queue in turn,\n"
			"               and empty the circular queue after running.\n"
			"run            Put instructions into the scheduled run queue.\n"
			"at             Set the running time of the running queue,\n"
			"               and empty the running queue immediately.\n"
			"goto           Run the Loop queue, does not empty the queue.\n"
			"thread         Create a new thread execution instruction.\n"
		);

		wdo_log(NULL, OUTLOG_NORMAL, "\n\n");

		wdo_log("Help", OUTLOG_NORMAL,
			"A complete command format is:\n\n\t"
			"alias.instruction*frequency { parameter1  parameter2 }\n\n"
		);
	}

	else wdo_log("Help", OUTLOG_ERROR, "error: The \"%s\" instruction was not found in the instruction library.\n", _str);
}

#endif