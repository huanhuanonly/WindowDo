// lrMath.c -- For lrMath.h
// 
//      Copyright (c) 2022-2023 LoveRain Yanghuanhuan 3347484963@qq.com.  All rights reserved.
// 
#include <stdio.h>

double lr_slope(int _x1, int _x2, int _y1, int _y2)
{
	/* Slope does not exist */
	if (_x1 == _x2) return 0;

	else return (double)(_y2 - _y1) / (double)(_x2 - _x1);
}