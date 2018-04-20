#include "bsp.h"
#include "param.h"
#include "main_run.h"
#include "gui.h"
#include "main.h"

#define LABEL1_X  	40
#define LABEL1_Y	10
#define LABEL1_H	0
#define LABEL1_W	0
#define LABEL1_T	"��Ƭ������ϵͳ"

#define LABEL2_X  	6
#define LABEL2_Y	(LABEL1_Y + 25)
#define LABEL2_H	0
#define LABEL2_W	0
#define LABEL2_T	"�ۼ�:"

#define LABEL3_X  	(LABEL2_X + 40)
#define LABEL3_Y	LABEL2_Y
#define LABEL3_H	0
#define LABEL3_W	0
#define LABEL3_T	" "

#define LABEL4_X  	(LABEL3_X + 40)
#define LABEL4_Y	LABEL2_Y
#define LABEL4_H	0
#define LABEL4_W	0
#define LABEL4_T	"����ģʽ:"

#define LABEL5_X  	(LABEL4_X + 70)
#define LABEL5_Y	LABEL2_Y
#define LABEL5_H	0
#define LABEL5_W	0
#define LABEL5_T	"�Զ�"


#define RISE_TIMER_ID	0
#define FALL_TIMER_ID	1

static void InitFormMainRun(void);
static void DispFormMainRun(void);

static void DispProcessNum(uint32_t num);
static void DispWorkMode(char *_str);

FormMain_T *FormMain;

void Main_Run(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t fRefresh;		/* ˢ�������־,1��ʾ��Ҫˢ�� */
	uint8_t ucWorkMode;

	FormMain_T form;
	FormMain = &form;
	
	InitFormMainRun();
	DispFormMainRun();

	fRefresh = 1;	/* 1��ʾ��Ҫˢ��LCD */
	ucWorkMode = 0; /*Ĭ���ֶ�ģʽ*/
	while (g_MainStatus == MS_MAIN_RUN)
	{
		bsp_Idle();

		if (fRefresh)
		{
			fRefresh = 0;
		
		}
		if (bsp_CheckTimer(RISE_TIMER_ID))/*������ʱ��*/
		{
			bsp_OpenRelay(RISE_MOTOR);
			BSP_Printf("��Ƭ�������У��е������ת,���������ת");
		}
		if (bsp_CheckTimer(FALL_TIMER_ID))/*�½���ʱ��*/
		{
			bsp_OpenRelay(FALL_MOTOR);
			BSP_Printf("��Ƭ�������У��½������ת");
		}
		ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* �м����� */
			switch (ucKeyCode)
			{
				case KEY_DOWN_K0:
					bsp_CloseRelay(CUT_MOTOR);/*�ر��е����*/
					bsp_CloseRelay(RISE_MOTOR);/*�ر��������*/
					bsp_StartTimer(FALL_TIMER_ID, g_tParam.FallDelay*1000);
					BSP_Printf("��Ƭ�������У��е����ֹͣ,�������ֹͣ");
					break;
				case KEY_DOWN_K1:
					bsp_CloseRelay(FALL_MOTOR);/*�ر��½����*/
					BSP_Printf("��Ƭ��������ɣ��½����ֹͣ");
					g_tParam.TotalNum++;
					DispProcessNum(g_tParam.TotalNum);
					break;
				default:
					break;
			}
		}
		ucKeyCode = bsp_GetMtxKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* �м����� */
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
						BSP_Printf("��Ƭ����ʼ�������е������ת");
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
						DispWorkMode("�Զ�");
					}
					else
					{
						ucWorkMode = 0;
						DispWorkMode("�ֶ�");
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
*	�� �� ��: DispWorkMode
*	����˵��: ��ʾ����ģʽ
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: DispProcessNum
*	����˵��: ��ʾ�ӹ�����
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: InitFormMain
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormMainRun(void)
{
	/* ����1 ���ھ�ֹ��ǩ */
	FormMain->LbNFont.FontCode = FC_ST_16;/* ������� 16���� */
	FormMain->LbNFont.BackColor = 0;	  /* ���ֱ�����ɫ 0 �� 1 */
	FormMain->LbNFont.FrontColor = 1;	  /* ������ɫ 0 �� 1 */
	FormMain->LbNFont.Space = 0;		  /* ���ּ�࣬��λ = ���� */
	LCD_InitLabel(&FormMain->Label1, LABEL1_X, LABEL1_Y, LABEL1_H, LABEL1_W, LABEL1_T, &FormMain->LbNFont);
	LCD_InitLabel(&FormMain->Label2, LABEL2_X, LABEL2_Y, LABEL2_H, LABEL2_W, LABEL2_T, &FormMain->LbNFont);
	LCD_InitLabel(&FormMain->Label4, LABEL4_X, LABEL4_Y, LABEL4_H, LABEL4_W, LABEL4_T, &FormMain->LbNFont);

}

/*
*********************************************************************************************************
*	�� �� ��: DispRSVInitFace
*	����˵��: ��ʾ���еľ�̬�ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormMainRun(void)
{
	LCD_ClrScr(0x00);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormMain->Label1);
	LCD_DrawLabel(&FormMain->Label2);
	LCD_DrawLabel(&FormMain->Label4);
	DispWorkMode("�ֶ�");
	DispProcessNum(g_tParam.TotalNum);
}
