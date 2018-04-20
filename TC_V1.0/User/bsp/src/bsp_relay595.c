/*
*********************************************************************************************************
*
*	模块名称 : 继电器驱动模块
*	文件名称 : bsp_relay595.c
*	版    本 : V1.0
*	说    明 : 
*				
*	修改记录 :
*		版本号  日期        作者    说明
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


uint16_t g_LacthStatus = 0;	/*595锁存状态*/
/*
*********************************************************************************************************
*	函 数 名: bsp_InitRLY
*	功能说明: 配置继电器输出相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
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
*	函 数 名: bsp_OpenRLY
*	功能说明: 打开指定的继电器
*	形    参:  _no : 指示灯序号，范围 1 - 2
*	返 回 值: 无
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
*	函 数 名: bsp_CloseRelay
*	功能说明: 关闭指定的继电器
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
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
*	函 数 名: bsp_ToggleRealy
*	功能说明: 翻转指定的继电器。
*	形    参:  _no : 继电器序号，范围 1 - 4
*	返 回 值: 无
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
*	函 数 名: 595_LockOutData
*	功能说明: 发送1个输出锁存脉冲
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: LED595_RefeshDisp
*	功能说明: 将全局显示缓冲区的显示数据刷新到LED数码管
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: bsp_GetRelayState
*	功能说明: 读取继电器的状态
*	形    参:  _ucRlyID : 继电器ID，从0开始
*	返 回 值: 1 表示打开， 0 表示未打开
*********************************************************************************************************
*/
uint8_t bsp_GetRelayState(uint8_t _ucRlyID)
{
	return (g_LacthStatus&(1<<_ucRlyID));
}
