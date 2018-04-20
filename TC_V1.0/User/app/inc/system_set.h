
/*
*********************************************************************************************************
*
*	模块名称 : 系统设定
*	文件名称 : system_set.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2010-2011, xxxxxxxxx
*
*********************************************************************************************************
*/
#ifndef __SYSTEM_SET_H
#define __SYSTEM_SET_H

/* 定义界面结构 */
typedef struct
{
	FONT_T LbNFont;	/* 静态的文字 */
	FONT_T LbVFont;	/* 变化的文字字体 */
	FONT_T FontEdit;	/* 编辑框的字体 */
	
	LABEL_T Label1;
	LABEL_T Label2;
	
	EDIT_T Edit1;
	EDIT_T Edit2;
	
	uint8_t EditCursor;		/* 当前选择的Edit, 0 表示未选择， Edit由1开始编码 */
}FormSYS_T;

void System_Set(void);

#endif 
