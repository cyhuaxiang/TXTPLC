/*
*********************************************************************************************************
*
*	模块名称 : 汉字点阵字库。
*	文件名称 : hz32.c
*	版    本 : V1.0
*	说    明 : 只包含本程序用到汉字字库，字库点阵来源与晓奇《液晶汉字模提取》，输出格式：从左到右，从上到下 取模方式：横向8点左高位
*	修改记录 :
*		版本号  日期       作者    说明
*		
*
*	Copyright (C), 2015-2016, xxxxxxxxxxxxx
*
*********************************************************************************************************
*/

#include "fonts.h"

/*
	FLASH中内嵌小字库，只包括本程序用到的汉字点阵
	每行点阵数据，头2字节是汉子的内码，后面字节是点阵汉字的字模数据。
*/
#ifdef USE_SMALL_FONT
/* 
*/
typFNT_GB32 const  g_Hz32[] = {


		   
/* 最后一行必须用0xFF,0xFF结束，这是字库数组结束标志 */
0xFF,0xFF

};
#else
	unsigned char const g_Hz32[] = {0xFF, 0xFF};
#endif

