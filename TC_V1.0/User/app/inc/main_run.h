/*
*********************************************************************************************************
*
*	ģ������ : ������
*	�ļ����� : main_menu.h
*	��    �� : V1.0
*
*	Copyright (C), 2013-2014, xxxxxxxxxx
*
*********************************************************************************************************
*/

#ifndef _MAIN_RUN_H
#define _MAIN_RUN_H

/* �������ṹ */
typedef struct
{
	FONT_T LbNFont;	/* ��̬������ */
	FONT_T LbVFont;	/* ��̬������ */
	
	LABEL_T Label1;
	LABEL_T Label2;
	LABEL_T Label4;

}FormMain_T;

extern void Main_Run(void);

#endif

