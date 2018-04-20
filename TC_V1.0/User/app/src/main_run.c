#include "bsp.h"
#include "param.h"
#include "main_run.h"
#include "gui.h"
#include "main.h"

#define LABEL1_X  	40
#define LABEL1_Y	10
#define LABEL1_H	0
#define LABEL1_W	0
#define LABEL1_T	"鱼片机控制系统"

#define LABEL2_X  	6
#define LABEL2_Y	(LABEL1_Y + 25)
#define LABEL2_H	0
#define LABEL2_W	0
#define LABEL2_T	"累计:"

#define LABEL3_X  	(LABEL2_X + 40)
#define LABEL3_Y	LABEL2_Y
#define LABEL3_H	0
#define LABEL3_W	0
#define LABEL3_T	" "

#define LABEL4_X  	(LABEL3_X + 40)
#define LABEL4_Y	LABEL2_Y
#define LABEL4_H	0
#define LABEL4_W	0
#define LABEL4_T	"工作模式:"

#define LABEL5_X  	(LABEL4_X + 70)
#define LABEL5_Y	LABEL2_Y
#define LABEL5_H	0
#define LABEL5_W	0
#define LABEL5_T	"自动"


#define RISE_TIMER_ID	0
#define FALL_TIMER_ID	1

static void InitFormMainRun(void);
static void DispFormMainRun(void);

static void DispProcessNum(uint32_t num);
static void DispWorkMode(char *_str);

FormMain_T *FormMain;

void Main_Run(void)
{
	uint8_t ucKeyCode;		/* 按键代码 */
	uint8_t fRefresh;		/* 刷屏请求标志,1表示需要刷新 */
	uint8_t ucWorkMode;

	FormMain_T form;
	FormMain = &form;
	
	InitFormMainRun();
	DispFormMainRun();

	fRefresh = 1;	/* 1表示需要刷新LCD */
	ucWorkMode = 0; /*默认手动模式*/
	while (g_MainStatus == MS_MAIN_RUN)
	{
		bsp_Idle();

		if (fRefresh)
		{
			fRefresh = 0;
		
		}
		if (bsp_CheckTimer(RISE_TIMER_ID))/*上升延时到*/
		{
			bsp_OpenRelay(RISE_MOTOR);
			BSP_Printf("鱼片机工作中，切刀电机运转,上升电机运转");
		}
		if (bsp_CheckTimer(FALL_TIMER_ID))/*下降延时到*/
		{
			bsp_OpenRelay(FALL_MOTOR);
			BSP_Printf("鱼片机工作中，下降电机运转");
		}
		ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* 有键按下 */
			switch (ucKeyCode)
			{
				case KEY_DOWN_K0:
					bsp_CloseRelay(CUT_MOTOR);/*关闭切刀电机*/
					bsp_CloseRelay(RISE_MOTOR);/*关闭上升电机*/
					bsp_StartTimer(FALL_TIMER_ID, g_tParam.FallDelay*1000);
					BSP_Printf("鱼片机工作中，切刀电机停止,上升电机停止");
					break;
				case KEY_DOWN_K1:
					bsp_CloseRelay(FALL_MOTOR);/*关闭下降电机*/
					BSP_Printf("鱼片机工作完成，下降电机停止");
					g_tParam.TotalNum++;
					DispProcessNum(g_tParam.TotalNum);
					break;
				default:
					break;
			}
		}
		ucKeyCode = bsp_GetMtxKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* 有键按下 */
			switch (ucKeyCode)
			{
				case KEY_DOWN_RIGHT:
					g_MainStatus = MS_SYSTEM_SET;
					break;
				case KEY_DOWN_CLEAR:
					g_tParam.TotalNum = 0;
					DispProcessNum(g_tParam.TotalNum);
					break;
				case KEY_DOWN_ENTER:
					if(ucWorkMode == 1)
					{
						BSP_Printf("鱼片机开始工作，切刀电机运转");
						bsp_OpenRelay(CUT_MOTOR);
						bsp_StartTimer(RISE_TIMER_ID, g_tParam.RiseDelay*1000);
					}
					break;
				case KEY_DOWN_ESC:
					if(ucWorkMode == 1)
					{
						bsp_StopTimer(RISE_TIMER_ID);
						bsp_StopTimer(FALL_TIMER_ID);
						bsp_CloseRelay(CUT_MOTOR);
						bsp_CloseRelay(RISE_MOTOR);
						bsp_CloseRelay(FALL_MOTOR);
					}
					break;
				case KEY_DOWN_SET:
					if(ucWorkMode  == 0)
					{
						ucWorkMode = 1;
						DispWorkMode("自动");
					}
					else
					{
						ucWorkMode = 0;
						DispWorkMode("手动");
					}
					break;
				case KEY_DOWN_NUM7:
					if(ucWorkMode == 0)
					{
						bsp_OpenRelay(CUT_MOTOR);
					}
					break;
				case KEY_UP_NUM7:
					if(ucWorkMode == 0)
					{
						bsp_CloseRelay(CUT_MOTOR);
					}
					break;
				case KEY_DOWN_NUM8:
					if(ucWorkMode == 0)
					{
						bsp_OpenRelay(RISE_MOTOR);
					}
					break;
				case KEY_UP_NUM8:
					if(ucWorkMode == 0)
					{
						bsp_CloseRelay(RISE_MOTOR);
					}
					break;
				case KEY_DOWN_NUM9:
					if(ucWorkMode == 0)
					{
						bsp_OpenRelay(FALL_MOTOR);
					}
					break;
				case KEY_UP_NUM9:
					if(ucWorkMode == 0)
					{
						bsp_CloseRelay(FALL_MOTOR);
					}
					break;
				default:
					break;
			}
		}
	}
	SaveParam();
}

/*
*********************************************************************************************************
*	函 数 名: DispWorkMode
*	功能说明: 显示工作模式
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DispWorkMode(char *_str)
{
	FONT_T font;
	
	font.FontCode = FC_ST_16;
	font.BackColor = 0;
	font.FrontColor = 1;
	font.Space = 0;	
	
	LCD_DispStr(LABEL5_X, LABEL5_Y, _str, &font);
}
/*
*********************************************************************************************************
*	函 数 名: DispProcessNum
*	功能说明: 显示加工数量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DispProcessNum(uint32_t num)
{
	char buf[10];
	
	FONT_T font;
	
	font.FontCode = FC_ST_16;
	font.BackColor = 0;
	font.FrontColor = 1;
	font.Space = 0;	
	sprintf(buf, "%04d", num);
	LCD_DispStr(LABEL3_X, LABEL3_Y, buf, &font);
}
/*
*********************************************************************************************************
*	函 数 名: InitFormMain
*	功能说明: 初始化控件属性
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitFormMainRun(void)
{
	/* 字体1 用于静止标签 */
	FormMain->LbNFont.FontCode = FC_ST_16;/* 字体代码 16点阵 */
	FormMain->LbNFont.BackColor = 0;	  /* 文字背景颜色 0 或 1 */
	FormMain->LbNFont.FrontColor = 1;	  /* 字体颜色 0 或 1 */
	FormMain->LbNFont.Space = 0;		  /* 文字间距，单位 = 像素 */
	LCD_InitLabel(&FormMain->Label1, LABEL1_X, LABEL1_Y, LABEL1_H, LABEL1_W, LABEL1_T, &FormMain->LbNFont);
	LCD_InitLabel(&FormMain->Label2, LABEL2_X, LABEL2_Y, LABEL2_H, LABEL2_W, LABEL2_T, &FormMain->LbNFont);
	LCD_InitLabel(&FormMain->Label4, LABEL4_X, LABEL4_Y, LABEL4_H, LABEL4_W, LABEL4_T, &FormMain->LbNFont);

}

/*
*********************************************************************************************************
*	函 数 名: DispRSVInitFace
*	功能说明: 显示所有的静态控件
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DispFormMainRun(void)
{
	LCD_ClrScr(0x00);

	/* 静态标签 */
	LCD_DrawLabel(&FormMain->Label1);
	LCD_DrawLabel(&FormMain->Label2);
	LCD_DrawLabel(&FormMain->Label4);
	DispWorkMode("手动");
	DispProcessNum(g_tParam.TotalNum);
}
