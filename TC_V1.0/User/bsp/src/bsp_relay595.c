/*
*********************************************************************************************************
*
*	ģ������ : �̵�������ģ��
*	�ļ����� : bsp_relay595.c
*	��    �� : V1.0
*	˵    �� : 
*				
*	�޸ļ�¼ :
*		�汾��  ����        ����    ˵��
*
*	Copyright (C), 2010-2012, XXXXXXXXXX
*
*********************************************************************************************************
*/
#include "bsp.h"


#define HC595_SCK_1()		P22 = 1
#define HC595_SCK_0()		P22 = 0

#define HC595_SDI_1()		P24 = 1
#define HC595_SDI_0() 		P24 = 0

#define HC595_LCK_1() 		P23 = 1
#define HC595_LCK_0() 		P23 = 0


uint16_t g_LacthStatus = 0;	/*595����״̬*/
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitRLY
*	����˵��: ���ü̵��������ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitRelay(void)
{
	GPIO_SetMode(P2, BIT2, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P2, BIT3, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P2, BIT4, GPIO_PMD_OUTPUT);
	HC595_SendData(g_LacthStatus);
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_OpenRLY
*	����˵��: ��ָ���ļ̵���
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_OpenRelay(uint8_t _no)
{
	if (_no == 0)
	{
		g_LacthStatus &= (1<<6);
		g_LacthStatus |= (1<<6);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 1)
	{
		g_LacthStatus &= (1<<7);
		g_LacthStatus |= (1<<7);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 2)
	{
		g_LacthStatus &= (1<<8);
		g_LacthStatus |= (1<<8);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 3)
	{
		g_LacthStatus &= (1<<9);
		g_LacthStatus |= (1<<9);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 4)
	{
		g_LacthStatus &= (1<<10);
		g_LacthStatus |= (1<<10);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 5)
	{
		g_LacthStatus &= (1<<11);
		g_LacthStatus |= (1<<1);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 6)
	{
		g_LacthStatus &= (1<<12);
		g_LacthStatus |= (1<<12);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 7)
	{
		g_LacthStatus &= (1<<13);
		g_LacthStatus |= (1<<13);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 8)
	{
		g_LacthStatus &= (1<<14);
		g_LacthStatus |= (1<<14);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 9)
	{
		g_LacthStatus &= (1<<15);
		g_LacthStatus |= (1<<15);
		HC595_SendData(g_LacthStatus);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_CloseRelay
*	����˵��: �ر�ָ���ļ̵���
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_CloseRelay(uint8_t _no)
{
	if (_no == 0)
	{
		g_LacthStatus &= ~(1<<6);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 1)
	{
		g_LacthStatus &= ~(1<<7);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 2)
	{
		g_LacthStatus &= ~(1<<8);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 3)
	{
		g_LacthStatus &= ~(1<<9);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 4)
	{
		g_LacthStatus &= ~(1<<10);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 5)
	{
		g_LacthStatus &= ~(1<<11);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 6)
	{
		g_LacthStatus &= ~(1<<12);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 7)
	{
		g_LacthStatus &= ~(1<<13);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 8)
	{
		g_LacthStatus &= ~(1<<14);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 9)
	{
		g_LacthStatus &= ~(1<<15);
		HC595_SendData(g_LacthStatus);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ToggleRealy
*	����˵��: ��תָ���ļ̵�����
*	��    ��:  _no : �̵�����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_ToggleRealy(uint8_t _no)
{
	if (_no == 1)
	{
		g_LacthStatus ^= (1<<1);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 2)
	{
		g_LacthStatus ^= (1<<2);
		HC595_SendData(g_LacthStatus);
	}
	else if (_no == 3)
	{
		g_LacthStatus ^= (1<<3);
		HC595_SendData(g_LacthStatus);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 595_LockOutData
*	����˵��: ����1�������������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void HC595_LockOutData(void)
{
	HC595_LCK_0();
//	bsp_DelayUS(20);	
	HC595_LCK_1();
	HC595_LCK_0();
}
/*
*********************************************************************************************************
*	�� �� ��: LED595_RefeshDisp
*	����˵��: ��ȫ����ʾ����������ʾ����ˢ�µ�LED�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HC595_SendData(uint16_t dat)
{
	uint8_t i;

	for (i = 0; i < 16; i++)
	{
		if (dat & 0x8000)
		{
			HC595_SDI_1();
		}
		else
		{
			HC595_SDI_0();
		}
//		bsp_DelayUS(20);
		HC595_SCK_0();	
//		bsp_DelayUS(20);		
		HC595_SCK_1();
		dat <<= 1;
	}
	HC595_LockOutData();
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_GetRelayState
*	����˵��: ��ȡ�̵�����״̬
*	��    ��:  _ucRlyID : �̵���ID����0��ʼ
*	�� �� ֵ: 1 ��ʾ�򿪣� 0 ��ʾδ��
*********************************************************************************************************
*/
uint8_t bsp_GetRelayState(uint8_t _ucRlyID)
{
	return (g_LacthStatus&(1<<_ucRlyID));
}
