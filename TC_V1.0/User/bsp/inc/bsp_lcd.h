/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ����ʾ������ģ��
*	�ļ����� : bsp_lcd.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/


#ifndef _BSP_LCD_H
#define _BSP_LCD_H

#if 1
	#include "gui.h"	/* ��ʹ�������е� FONT_T �ṹ�� */
#else
	/* ������� */
	typedef enum
	{
		FC_ST_12 = 0,		/* ����12x12���� ����x�ߣ� */
		FC_ST_16,			/* ����15x16���� ����x�ߣ� */
		FC_ST_24,			/* ����24x24���� ����x�ߣ� -- ��ʱδ֧�� */
		FC_ST_32,			/* ����32x32���� ����x�ߣ� -- ��ʱδ֧�� */	
	}FONT_CODE_E;
	typedef struct
	{
		uint16_t FontCode;	/* ������� 0 ��ʾ16���� */
		uint16_t FrontColor;/* ������ɫ */
		uint16_t BackColor;	/* ���ֱ�����ɫ��͸�� */
		uint16_t Space;		/* ���ּ�࣬��λ = ���� */
	}FONT_T;
#endif

/* �ɹ��ⲿģ����õĺ��� */
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


