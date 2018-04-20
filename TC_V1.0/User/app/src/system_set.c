
/*
*********************************************************************************************************
*
*	模块名称 : 系统设置界面
*	文件名称 : system_set.c
*	版    本 : V1.0
*	说    明 : 本模块进行文件选择
*	修改记录 :
*		版本号  日期       作者    说明
*
*	Copyright (C), 2015-2020, xxxxxxxx
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "main.h"
#include "param.h"
#include "system_set.h"

#define LABEL1_X  	6
#define LABEL1_Y	6
#define LABEL1_H	0
#define LABEL1_W	0
#define LABEL1_T	"上升延时:"

#define LABEL2_X  	LABEL1_X
#define LABEL2_Y	(LABEL1_Y + 20)
#define LABEL2_H	0
#define LABEL2_W	0
#define LABEL2_T	"下降延时:"

#define EDIT1_X  	(LABEL1_X + 60)
#define EDIT1_Y		(LABEL1_Y-2)
#define EDIT1_H		15
#define EDIT1_W		35

#define EDIT2_X  	(LABEL2_X + 60)
#define EDIT2_Y		(LABEL2_Y-2)
#define EDIT2_H		EDIT1_H
#define EDIT2_W		EDIT1_W

static void InitFormSysSet(void);
static void DispFormSysSet(void);
static void EditAdjustValue(int16_t _add);
static void DispParamValue(uint16_t _ParamCode);
static void EditQuitFocus(uint8_t _Idx);
static void EditEnterFocus(EDIT_T *_Edit);

static FormSYS_T *FormSysSet;
/*
*********************************************************************************************************
*	函 数 名: System_Set
*	功能说明: 系统设定
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void System_Set(void)
{
	uint8_t ucKeyCode;		/* 按键代码 */
	uint8_t fRefresh;		/* 刷屏请求标志,1表示需要刷新 */
	
	FormSYS_T form;
	FormSysSet = &form;
	
	InitFormSysSet();
	DispFormSysSet();
	FormSysSet->EditCursor = 1;			
	EditEnterFocus(&FormSysSet->Edit1);
	while (g_MainStatus == MS_SYSTEM_SET)
	{
		bsp_Idle();
		if (fRefresh)
		{
			fRefresh = 0;
		
		}
		ucKeyCode = bsp_GetMtxKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* 有键按下 */
			switch (ucKeyCode)
			{
				case KEY_DOWN_LEFT:
					g_MainStatus = MS_MAIN_RUN;
					break;
				case KEY_DOWN_SET:
					if(FormSysSet->EditCursor == 1)
					{
						FormSysSet->EditCursor = 2;
						EditEnterFocus(&FormSysSet->Edit2);
						EditQuitFocus(2);
					}
					else if(FormSysSet->EditCursor == 2)
					{
						FormSysSet->EditCursor = 1;
						EditEnterFocus(&FormSysSet->Edit1);
						EditQuitFocus(1);
					}
					break;
				case KEY_DOWN_UP:
					EditAdjustValue(1);
					break;
				case KEY_LONG_UP:
					EditAdjustValue(1);
					break;
				case KEY_DOWN_DOWN:
					EditAdjustValue(-1);
					break;
				case KEY_LONG_DOWN:
					EditAdjustValue(-1);
					break;
				default:
					break;
			}
		}
		
	}
	
	SaveParam();	/* 保存参数 */
}
/*
*********************************************************************************************************
*	函 数 名: EditEnterFocus
*	功能说明: 编辑框进入编辑状态
*	形    参: _Idx : 排除失去焦点的编辑框
*	返 回 值: 无
*********************************************************************************************************
*/
static void EditEnterFocus(EDIT_T *_Edit)
{
	_Edit->Focus = 1;
	LCD_DrawEdit(_Edit);
}
/*
*********************************************************************************************************
*	函 数 名: EditQuitEdit
*	功能说明: 编辑框退出编辑状态
*	形    参: _Idx : 排除失去焦点的编辑框
*	返 回 值: 无
*********************************************************************************************************
*/
static void EditQuitFocus(uint8_t _Idx)
{
	if (FormSysSet->Edit1.Focus && _Idx != 1)
	{
		LCD_EditRefresh(&FormSysSet->Edit1);	/* 失去焦点，重回编辑框 */
	}
	if (FormSysSet->Edit2.Focus && _Idx != 2)
	{
		LCD_EditRefresh(&FormSysSet->Edit2);
	}
}

/*
*********************************************************************************************************
*	函 数 名: EditAdjustValue
*	功能说明: 调节编辑框内的参数
*	形    参:_add : 0表示清零，+1表示减1， -1表示加1
*	返 回 值: 无
*********************************************************************************************************
*/
static void EditAdjustValue(int16_t _add)
{		
	if (FormSysSet->EditCursor <= 2)
	{
		AdjustParamValue(FormSysSet->EditCursor, _add);
		DispParamValue(FormSysSet->EditCursor);
	}
}
/*
*********************************************************************************************************
*	函 数 名: DispParamValue
*	功能说明: 显示参数值，刷新编辑框
*	形    参:_ParamCode : 参数代码
*	返 回 值: 无
*********************************************************************************************************
*/
static void DispParamValue(uint16_t _ParamCode)
{	
	char disp_buf[32];
	
	switch (_ParamCode)
	{
		case 1:	/* P001 */
			FormatParamValue(1, disp_buf);
			FormSysSet->Edit1.pCaption = disp_buf;
			LCD_DrawEdit(&FormSysSet->Edit1);
			break;

		case 2:	/* P002 */
			FormatParamValue(2, disp_buf);
			FormSysSet->Edit2.pCaption = disp_buf;
			LCD_DrawEdit(&FormSysSet->Edit2);	
			break;
		default:
			break;
	}	
}
/*
*********************************************************************************************************
*	函 数 名: InitFormMain
*	功能说明: 初始化控件属性
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitFormSysSet(void)
{
	/* 字体1 用于静止标签 */
	FormSysSet->LbNFont.FontCode = FC_ST_12;/* 字体代码 16点阵 */
	FormSysSet->LbNFont.BackColor = 0;	  /* 文字背景颜色 0 或 1 */
	FormSysSet->LbNFont.FrontColor = 1;	  /* 字体颜色 0 或 1 */
	FormSysSet->LbNFont.Space = 1;		  /* 文字间距，单位 = 像素 */
	/* 静态标签 */
	LCD_InitLabel(&FormSysSet->Label1, LABEL1_X, LABEL1_Y, LABEL1_H, LABEL1_W, LABEL1_T, &FormSysSet->LbNFont);
	LCD_InitLabel(&FormSysSet->Label2, LABEL2_X, LABEL2_Y, LABEL2_H, LABEL2_W, LABEL2_T, &FormSysSet->LbNFont);
	
	FormSysSet->FontEdit.FontCode = FC_ST_12;/* 字体代码 16点阵 */
	FormSysSet->FontEdit.BackColor = 0;	  /* 文字背景颜色 0 或 1 */
	FormSysSet->FontEdit.FrontColor = 1;	  /* 字体颜色 0 或 1 */
	FormSysSet->FontEdit.Space = 0;		  /* 文字间距，单位 = 像素 */
	/* 编辑框 */
	LCD_InitEdit(&FormSysSet->Edit1, EDIT1_X, EDIT1_Y, EDIT1_H, EDIT1_W, "", &FormSysSet->FontEdit);
	LCD_InitEdit(&FormSysSet->Edit2, EDIT2_X, EDIT2_Y, EDIT2_H, EDIT2_W, "", &FormSysSet->FontEdit);	
	
}
void Test1()
{
	bsp_OpenRelay(CUT_MOTOR);
}
/*
*********************************************************************************************************
*	函 数 名: DispRSVInitFace
*	功能说明: 显示所有的静态控件
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DispFormSysSet(void)
{
	LCD_ClrScr(0x00);

	/* 静态标签 */
	LCD_DrawLabel(&FormSysSet->Label1);
	LCD_DrawLabel(&FormSysSet->Label2);
	
	/* 编辑框 */
	#if 1
		DispParamValue(1);
		DispParamValue(2);
	#else
		LCD_DrawEdit(&FormSysSet->Edit1);	
		LCD_DrawEdit(&FormSysSet->Edit2);
	#endif
}
