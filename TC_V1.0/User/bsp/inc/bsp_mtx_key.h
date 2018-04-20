/*
*********************************************************************************************************
*
*	模块名称 : 按键驱动模块
*	文件名称 : bsp_key.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_MTX_KEY_H
#define __BSP_MTX_KEY_H

#define KEY_NONE	 	  0x00

#define KEY_DOWN_ESC	  0x14
#define KEY_DOWN_ALARM    0x13
#define KEY_DOWN_UP       0x11
#define KEY_DOWN_DOWN     0x12
#define KEY_DOWN_LEFT     0x10
#define KEY_DOWN_RIGHT    0x0E
#define KEY_DOWN_SET      0x0F
#define KEY_DOWN_ENTER    0x0D
#define KEY_DOWN_NUM0     0x0A
#define KEY_DOWN_NUM1     0x07
#define KEY_DOWN_NUM2     0x08
#define KEY_DOWN_NUM3     0x09
#define KEY_DOWN_NUM4     0x02
#define KEY_DOWN_NUM5     0x01
#define KEY_DOWN_NUM6     0x06
#define KEY_DOWN_NUM7     0x05
#define KEY_DOWN_NUM8     0x04
#define KEY_DOWN_NUM9     0x03
#define KEY_DOWN_OPERATOR 0x0B
#define KEY_DOWN_CLEAR    0x0C

#define KEY_UP_NUM7       0x25
#define KEY_UP_NUM8       0x24
#define KEY_UP_NUM9       0x23

#define KEY_LONG_UP       0x51
#define KEY_LONG_DOWN     0x52





/* 供外部调用的函数声明 */
void bsp_InitMtxKey(void);
uint8_t bsp_GetMtxKey(void);
void bsp_MtxKeyScan(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
