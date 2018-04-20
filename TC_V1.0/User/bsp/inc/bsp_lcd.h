/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : bsp_lcd.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/


#ifndef _BSP_LCD_H
#define _BSP_LCD_H

#if 1
	#include "gui.h"	/* 仅使用了其中的 FONT_T 结构体 */
#else
	/* 字体代码 */
	typedef enum
	{
		FC_ST_12 = 0,		/* 宋体12x12点阵 （宽x高） */
		FC_ST_16,			/* 宋体15x16点阵 （宽x高） */
		FC_ST_24,			/* 宋体24x24点阵 （宽x高） -- 暂时未支持 */
		FC_ST_32,			/* 宋体32x32点阵 （宽x高） -- 暂时未支持 */	
	}FONT_CODE_E;
	typedef struct
	{
		uint16_t FontCode;	/* 字体代码 0 表示16点阵 */
		uint16_t FrontColor;/* 字体颜色 */
		uint16_t BackColor;	/* 文字背景颜色，透明 */
		uint16_t Space;		/* 文字间距，单位 = 像素 */
	}FONT_T;
#endif

/* 可供外部模块调用的函数 */
void LCD_InitHard(void);
void LCD_DispOn(void);
void LCD_DispOff(void);
void LCD_SetDir(uint8_t _ucDir);
void LCD_SetContrast(uint8_t ucValue);
void LCD_StartDraw(void);
void LCD_EndDraw(void);
void LCD_ClrScr(uint8_t _ucMode);
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor);
uint8_t LCD_GetPixel(uint16_t _usX, uint16_t _usY);
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor);
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor);
void LCD_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor);
void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr);
	
//void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);	
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor);
void LCD_Fill_Rect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor);
#endif


