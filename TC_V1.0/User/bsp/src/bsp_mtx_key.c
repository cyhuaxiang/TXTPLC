
#include "bsp.h"

#define KEY_LINE_NUM	 4	
#define KEY_COL_NUM      5

#define KEY_IN_1         P43
#define KEY_IN_2         P35
#define KEY_IN_3         P34
#define KEY_IN_4         P33
#define KEY_IN_5         P32


#define KOUT1_1()        P21 = 1
#define KOUT1_0()        P21 = 0

#define KOUT2_1()        P20 = 1
#define KOUT2_0()        P20 = 0

#define KOUT3_1()        P37 = 1
#define KOUT3_0()        P37 = 0

#define KOUT4_1()        P36 = 1
#define KOUT4_0()        P36 = 0


const uint8_t KeyDownCodeMap[KEY_LINE_NUM][KEY_COL_NUM] = {  //矩阵按键到标准键码的映射表
    { 0x01, 0x02, 0x03, 0x04, 0x05},  
    { 0x06, 0x07, 0x08, 0x09, 0x0A},
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
    { 0x10, 0x11, 0x12, 0x13, 0x14}
};
const uint8_t KeyUpCodeMap[KEY_LINE_NUM][KEY_COL_NUM] = {  //矩阵按键到标准键码的映射表
    { 0x21, 0x22, 0x23, 0x24, 0x25},  
    { 0x26, 0x27, 0x28, 0x29, 0x2A},
    { 0x2B, 0x2C, 0x2D, 0x2E, 0x2F},
    { 0x30, 0x31, 0x32, 0x33, 0x34}
};
const uint8_t KeyLongCodeMap[KEY_LINE_NUM][KEY_COL_NUM] = {  //矩阵按键到标准键码的映射表
    { 0x41, 0x42, 0x43, 0x44, 0x45},  
    { 0x46, 0x47, 0x48, 0x49, 0x4A},
    { 0x4B, 0x4C, 0x4D, 0x4E, 0x4F},
    { 0x50, 0x51, 0x52, 0x53, 0x54}
};
uint8_t KeySta[KEY_LINE_NUM][KEY_COL_NUM] = {  //全部矩阵按键的当前状态
    {1, 1, 1, 1, 1},  
    {1, 1, 1, 1, 1}, 
    {1, 1, 1, 1, 1},  
    {1, 1, 1, 1, 1}
};
uint32_t KeyDownTime[KEY_LINE_NUM][KEY_COL_NUM] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
};
/*
*********************************************************************************************************
*	函 数 名: bsp_InitMtxKey
*	功能说明: 配置按键对应的GPIO工作模式
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitMtxKey(void)
{
	GPIO_SetMode(P4, BIT3, GPIO_PMD_INPUT);
	GPIO_SetMode(P3, BIT5, GPIO_PMD_INPUT);
	GPIO_SetMode(P3, BIT4, GPIO_PMD_INPUT);
	GPIO_SetMode(P3, BIT3, GPIO_PMD_INPUT);
	GPIO_SetMode(P3, BIT2, GPIO_PMD_INPUT);
	
	GPIO_SetMode(P2, BIT1, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P2, BIT0, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P3, BIT7, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P3, BIT6, GPIO_PMD_OUTPUT);
}
/*
*********************************************************************************************************
*	函 数 名: bsp_GetMtxKey
*	功能说明: 得到矩阵按键键值
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_GetMtxKey(void)
{
	uint8_t i, j;
	uint8_t ret = 0;
    static uint8_t backup[KEY_LINE_NUM][KEY_COL_NUM] = {  //按键值备份，保存前一次的值
        {1, 1, 1, 1, 1}, 
		{1, 1, 1, 1, 1}, 
		{1, 1, 1, 1, 1},  
		{1, 1, 1, 1, 1}
    };
    static uint32_t TimeThr[KEY_LINE_NUM][KEY_COL_NUM] = {  //快速输入执行的时间阈值
        {1000, 1000, 1000, 1000, 1000},  
		{1000, 1000, 1000, 1000, 1000},
        {1000, 1000, 1000, 1000, 1000},  
		{1000, 1000, 1000, 1000, 1000}
    };
    
    for (i=0; i<KEY_LINE_NUM; i++)  //循环检测4*5的矩阵按键
    {
        for (j=0; j<KEY_COL_NUM; j++)
        {
            if (backup[i][j] != KeySta[i][j])    //检测按键动作
            {
                if (backup[i][j] != 0)           //按键按下时执行动作
                {
                    ret =  KeyDownCodeMap[i][j];               
                }
                else
                {
                    ret =  KeyUpCodeMap[i][j]; 
                }
                
                backup[i][j] = KeySta[i][j];     //刷新前一次的备份值
            }
 
            if (KeyDownTime[i][j] > 0)            //检测执行快速输入
            {
                if (KeyDownTime[i][j] >= TimeThr[i][j])
                {                                 //达到阈值时执行一次动作
                    ret =  KeyLongCodeMap[i][j];  
                    TimeThr[i][j] += 200; //时间阈值增加200ms，以准备下次执行
                }
            }
            else   //按键弹起时复位阈值时间
            {
                TimeThr[i][j] = 1000;  //恢复1s的初始阈值时间
            }
        }
    }  
	return ret;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_MtxKeyScan
*	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_MtxKeyScan(void)
{
    uint8_t i;
    static uint8_t keyout = 0;   //矩阵按键扫描输出索引
    static uint8_t keybuf[KEY_LINE_NUM][KEY_COL_NUM] = {  //矩阵按键扫描缓冲区
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
		{0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  
		{0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
    };

    //将一行的4个按键值移入缓冲区
    keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEY_IN_1;
    keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEY_IN_2;
    keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEY_IN_3;
    keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEY_IN_4;
	keybuf[keyout][4] = (keybuf[keyout][4] << 1) | KEY_IN_5;
    //消抖后更新按键状态
    for (i=0; i<KEY_COL_NUM; i++)  //每行5个按键，所以循环5次
    {
        if ((keybuf[keyout][i] & 0x0F) == 0x00)
        {   //连续4次扫描值为0，即4*5ms内都是按下状态时，可认为按键已稳定的按下
            KeySta[keyout][i] = 0;
            KeyDownTime[keyout][i] += 20;
        }
        else if ((keybuf[keyout][i] & 0x0F) == 0x0F)
        {   //连续4次扫描值为1，即4*5ms内都是弹起状态时，可认为按键已稳定的弹起
            KeySta[keyout][i] = 1;
            KeyDownTime[keyout][i] = 0;
        }
    }
    //执行下一次的扫描输出
	if(++keyout >= KEY_LINE_NUM)
	{
		keyout = 0;
	}
    switch (keyout)  //根据索引值，释放当前输出引脚，拉低下次的输出引脚
    {
        case 0: KOUT4_1(); KOUT1_0(); break;
        case 1: KOUT1_1(); KOUT2_0(); break;
        case 2: KOUT2_1(); KOUT3_0(); break;
        case 3: KOUT3_1(); KOUT4_0(); break;
        default: break;
    }
}
/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
