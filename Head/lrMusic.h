// lrMusic.h -- Music control
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#pragma once

#ifndef _LR_MUSIC_H_
#define _LR_MUSIC_H_

#include <Windows.h>
#include <mciapi.h>
#include <stdio.h>

MCIERROR lrPlayMusic(LPCSTR lpPath, BOOL bLoop, BOOL bWait);
MCIERROR lrCloseMusic(LPCSTR lpPath);
MCIERROR lrPauseMusic(LPCSTR lpPath);
MCIERROR lrContinueMusic(LPCSTR lpPath);

#endif