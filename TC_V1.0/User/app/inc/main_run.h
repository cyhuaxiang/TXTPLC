/*
*********************************************************************************************************
*
*	模块名称 : 主界面
*	文件名称 : main_menu.h
*	版    本 : V1.0
*
*	Copyright (C), 2013-2014, xxxxxxxxxx
*
*********************************************************************************************************
*/

#ifndef _MAIN_RUN_H
#define _MAIN_RUN_H

/* 定义界面结构 */
typedef struct
{
	FONT_T LbNFont;	/* 静态的文字 */
	FONT_T LbVFont;	/* 动态的文字 */
	
	LABEL_T Label1;
	LABEL_T Label2;
	LABEL_T Label4;

}FormMain_T;

extern void Main_Run(void);

#endif

