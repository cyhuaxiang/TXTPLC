
/*
*********************************************************************************************************
*
*	ģ������ : ϵͳ���ý���
*	�ļ����� : system_set.c
*	��    �� : V1.0
*	˵    �� : ��ģ������ļ�ѡ��
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
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
#define LABEL1_T	"������ʱ:"

#define LABEL2_X  	LABEL1_X
#define LABEL2_Y	(LABEL1_Y + 20)
#define LABEL2_H	0
#define LABEL2_W	0
#define LABEL2_T	"�½���ʱ:"

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
*	�� �� ��: System_Set
*	����˵��: ϵͳ�趨
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void System_Set(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t fRefresh;		/* ˢ�������־,1��ʾ��Ҫˢ�� */
	
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
		ucKeyCode = bsp_GetMtxKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* �м����� */
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
	
	SaveParam();	/* ������� */
}
/*
*********************************************************************************************************
*	�� �� ��: EditEnterFocus
*	����˵��: �༭�����༭״̬
*	��    ��: _Idx : �ų�ʧȥ����ı༭��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void EditEnterFocus(EDIT_T *_Edit)
{
	_Edit->Focus = 1;
	LCD_DrawEdit(_Edit);
}
/*
*********************************************************************************************************
*	�� �� ��: EditQuitEdit
*	����˵��: �༭���˳��༭״̬
*	��    ��: _Idx : �ų�ʧȥ����ı༭��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void EditQuitFocus(uint8_t _Idx)
{
	if (FormSysSet->Edit1.Focus && _Idx != 1)
	{
		LCD_EditRefresh(&FormSysSet->Edit1);	/* ʧȥ���㣬�ػر༭�� */
	}
	if (FormSysSet->Edit2.Focus && _Idx != 2)
	{
		LCD_EditRefresh(&FormSysSet->Edit2);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: EditAdjustValue
*	����˵��: ���ڱ༭���ڵĲ���
*	��    ��:_add : 0��ʾ���㣬+1��ʾ��1�� -1��ʾ��1
*	�� �� ֵ: ��
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
*	�� �� ��: DispParamValue
*	����˵��: ��ʾ����ֵ��ˢ�±༭��
*	��    ��:_ParamCode : ��������
*	�� �� ֵ: ��
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
*	�� �� ��: InitFormMain
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormSysSet(void)
{
	/* ����1 ���ھ�ֹ��ǩ */
	FormSysSet->LbNFont.FontCode = FC_ST_12;/* ������� 16���� */
	FormSysSet->LbNFont.BackColor = 0;	  /* ���ֱ�����ɫ 0 �� 1 */
	FormSysSet->LbNFont.FrontColor = 1;	  /* ������ɫ 0 �� 1 */
	FormSysSet->LbNFont.Space = 1;		  /* ���ּ�࣬��λ = ���� */
	/* ��̬��ǩ */
	LCD_InitLabel(&FormSysSet->Label1, LABEL1_X, LABEL1_Y, LABEL1_H, LABEL1_W, LABEL1_T, &FormSysSet->LbNFont);
	LCD_InitLabel(&FormSysSet->Label2, LABEL2_X, LABEL2_Y, LABEL2_H, LABEL2_W, LABEL2_T, &FormSysSet->LbNFont);
	
	FormSysSet->FontEdit.FontCode = FC_ST_12;/* ������� 16���� */
	FormSysSet->FontEdit.BackColor = 0;	  /* ���ֱ�����ɫ 0 �� 1 */
	FormSysSet->FontEdit.FrontColor = 1;	  /* ������ɫ 0 �� 1 */
	FormSysSet->FontEdit.Space = 0;		  /* ���ּ�࣬��λ = ���� */
	/* �༭�� */
	LCD_InitEdit(&FormSysSet->Edit1, EDIT1_X, EDIT1_Y, EDIT1_H, EDIT1_W, "", &FormSysSet->FontEdit);
	LCD_InitEdit(&FormSysSet->Edit2, EDIT2_X, EDIT2_Y, EDIT2_H, EDIT2_W, "", &FormSysSet->FontEdit);	
	
}
void Test1()
{
	bsp_OpenRelay(CUT_MOTOR);
}
/*
*********************************************************************************************************
*	�� �� ��: DispRSVInitFace
*	����˵��: ��ʾ���еľ�̬�ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormSysSet(void)
{
	LCD_ClrScr(0x00);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormSysSet->Label1);
	LCD_DrawLabel(&FormSysSet->Label2);
	
	/* �༭�� */
	#if 1
		DispParamValue(1);
		DispParamValue(2);
	#else
		LCD_DrawEdit(&FormSysSet->Edit1);	
		LCD_DrawEdit(&FormSysSet->Edit2);
	#endif
}
