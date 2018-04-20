/*										
*********************************************************************************************************
*	                                  
*	ģ������ : �ֿ�ģ��
*	�ļ����� : fonts.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2010-2011, xxxxxxxxx
*
*********************************************************************************************************
*/

#ifndef __FONTS_H_
#define __FONTS_H_


#define USE_SMALL_FONT			/* ������б�ʾʹ��С�ֿ⣬ �����ֻ��bsp_tft+lcd.c��ʹ�� */

// ------------------  ������ģ�����ݽṹ���� ------------------------ //
typedef struct                 // ������ģ���ݽṹ
{
	unsigned char Index[2];              // ������������
	unsigned char Msk[24];               // ����������
}typFNT_GB12;
typedef struct                 // ������ģ���ݽṹ
{
	unsigned char Index[2];              // ������������
	unsigned char Msk[32];               // ����������
}typFNT_GB16;
typedef struct                 // ������ģ���ݽṹ
{
	unsigned char Index[2];              // ������������
	unsigned char Msk[72];               // ����������
}typFNT_GB24;
typedef struct                 // ������ģ���ݽṹ
{
	unsigned char Index[2];              // ������������
	unsigned char Msk[128];               // ����������
}typFNT_GB32;

/* CPU����С�ֿ�ĳ������� */
extern unsigned char const g_Ascii12[];		/* 12���� ASCII,ȫ�ַ� */
extern unsigned char const g_Ascii16[];		/* 16���� ASCII,ȫ�ַ� */
extern unsigned char const g_Ascii24[];		/* 24���� ASCII,�����ַ� */
extern unsigned char const g_Ascii32[];		/* 32���� ASCII,�����ַ� */

extern typFNT_GB12 const g_Hz12[];		/* 12�����ֵ���,���ֺ��� */	
extern typFNT_GB16 const g_Hz16[];		/* 16�����ֵ���,���ֺ��� */	
extern typFNT_GB24 const g_Hz24[];		/* 24�����ֵ���,���ֺ��� */	
extern typFNT_GB32 const g_Hz32[];		/* 32�����ֵ���,���ֺ��� */	
#endif
