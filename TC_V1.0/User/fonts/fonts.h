/*										
*********************************************************************************************************
*	                                  
*	模块名称 : 字库模块
*	文件名称 : fonts.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2010-2011, xxxxxxxxx
*
*********************************************************************************************************
*/

#ifndef __FONTS_H_
#define __FONTS_H_


#define USE_SMALL_FONT			/* 定义此行表示使用小字库， 这个宏只在bsp_tft+lcd.c中使用 */

// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct                 // 汉字字模数据结构
{
	unsigned char Index[2];              // 汉字内码索引
	unsigned char Msk[24];               // 点阵码数据
}typFNT_GB12;
typedef struct                 // 汉字字模数据结构
{
	unsigned char Index[2];              // 汉字内码索引
	unsigned char Msk[32];               // 点阵码数据
}typFNT_GB16;
typedef struct                 // 汉字字模数据结构
{
	unsigned char Index[2];              // 汉字内码索引
	unsigned char Msk[72];               // 点阵码数据
}typFNT_GB24;
typedef struct                 // 汉字字模数据结构
{
	unsigned char Index[2];              // 汉字内码索引
	unsigned char Msk[128];               // 点阵码数据
}typFNT_GB32;

/* CPU内置小字库的常量数组 */
extern unsigned char const g_Ascii12[];		/* 12点阵 ASCII,全字符 */
extern unsigned char const g_Ascii16[];		/* 16点阵 ASCII,全字符 */
extern unsigned char const g_Ascii24[];		/* 24点阵 ASCII,部分字符 */
extern unsigned char const g_Ascii32[];		/* 32点阵 ASCII,部分字符 */

extern typFNT_GB12 const g_Hz12[];		/* 12点阵汉字点阵,部分汉字 */	
extern typFNT_GB16 const g_Hz16[];		/* 16点阵汉字点阵,部分汉字 */	
extern typFNT_GB24 const g_Hz24[];		/* 24点阵汉字点阵,部分汉字 */	
extern typFNT_GB32 const g_Hz32[];		/* 32点阵汉字点阵,部分汉字 */	
#endif
