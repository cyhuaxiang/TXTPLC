
/*
*********************************************************************************************************
*
*	ģ������ : ϵͳ�趨
*	�ļ����� : system_set.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2010-2011, xxxxxxxxx
*
*********************************************************************************************************
*/
#ifndef __SYSTEM_SET_H
#define __SYSTEM_SET_H

/* �������ṹ */
typedef struct
{
	FONT_T LbNFont;	/* ��̬������ */
	FONT_T LbVFont;	/* �仯���������� */
	FONT_T FontEdit;	/* �༭������� */
	
	LABEL_T Label1;
	LABEL_T Label2;
	
	EDIT_T Edit1;
	EDIT_T Edit2;
	
	uint8_t EditCursor;		/* ��ǰѡ���Edit, 0 ��ʾδѡ�� Edit��1��ʼ���� */
}FormSYS_T;

void System_Set(void);

#endif 
