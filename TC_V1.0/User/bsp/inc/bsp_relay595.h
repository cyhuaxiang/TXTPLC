
#ifndef _BSP_RELAY595_H
#define _BSP_RELAY595_H

#define FILLET_MACHINE		//鱼片机

#ifdef FILLET_MACHINE

#define CUT_MOTOR	0
#define RISE_MOTOR	6
#define FALL_MOTOR	7

#endif


/* 供外部调用的函数声明 */
void bsp_InitRelay(void);
void bsp_OpenRelay(uint8_t _no);
void bsp_CloseRelay(uint8_t _no);
void bsp_ToggleRealy(uint8_t _no);
uint8_t bsp_GetRelayState(uint8_t _ucRlyID);
void HC595_SendData(uint16_t dat);


extern uint16_t g_LacthStatus;	

#endif


