
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


const uint8_t KeyDownCodeMap[KEY_LINE_NUM][KEY_COL_NUM] = {  //���󰴼�����׼�����ӳ���
    { 0x01, 0x02, 0x03, 0x04, 0x05},  
    { 0x06, 0x07, 0x08, 0x09, 0x0A},
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
    { 0x10, 0x11, 0x12, 0x13, 0x14}
};
const uint8_t KeyUpCodeMap[KEY_LINE_NUM][KEY_COL_NUM] = {  //���󰴼�����׼�����ӳ���
    { 0x21, 0x22, 0x23, 0x24, 0x25},  
    { 0x26, 0x27, 0x28, 0x29, 0x2A},
    { 0x2B, 0x2C, 0x2D, 0x2E, 0x2F},
    { 0x30, 0x31, 0x32, 0x33, 0x34}
};
const uint8_t KeyLongCodeMap[KEY_LINE_NUM][KEY_COL_NUM] = {  //���󰴼�����׼�����ӳ���
    { 0x41, 0x42, 0x43, 0x44, 0x45},  
    { 0x46, 0x47, 0x48, 0x49, 0x4A},
    { 0x4B, 0x4C, 0x4D, 0x4E, 0x4F},
    { 0x50, 0x51, 0x52, 0x53, 0x54}
};
uint8_t KeySta[KEY_LINE_NUM][KEY_COL_NUM] = {  //ȫ�����󰴼��ĵ�ǰ״̬
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
*	�� �� ��: bsp_InitMtxKey
*	����˵��: ���ð�����Ӧ��GPIO����ģʽ
*	��    ��:  ��
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_GetMtxKey
*	����˵��: �õ����󰴼���ֵ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t bsp_GetMtxKey(void)
{
	uint8_t i, j;
	uint8_t ret = 0;
    static uint8_t backup[KEY_LINE_NUM][KEY_COL_NUM] = {  //����ֵ���ݣ�����ǰһ�ε�ֵ
        {1, 1, 1, 1, 1}, 
		{1, 1, 1, 1, 1}, 
		{1, 1, 1, 1, 1},  
		{1, 1, 1, 1, 1}
    };
    static uint32_t TimeThr[KEY_LINE_NUM][KEY_COL_NUM] = {  //��������ִ�е�ʱ����ֵ
        {1000, 1000, 1000, 1000, 1000},  
		{1000, 1000, 1000, 1000, 1000},
        {1000, 1000, 1000, 1000, 1000},  
		{1000, 1000, 1000, 1000, 1000}
    };
    
    for (i=0; i<KEY_LINE_NUM; i++)  //ѭ�����4*5�ľ��󰴼�
    {
        for (j=0; j<KEY_COL_NUM; j++)
        {
            if (backup[i][j] != KeySta[i][j])    //��ⰴ������
            {
                if (backup[i][j] != 0)           //��������ʱִ�ж���
                {
                    ret =  KeyDownCodeMap[i][j];               
                }
                else
                {
                    ret =  KeyUpCodeMap[i][j]; 
                }
                
                backup[i][j] = KeySta[i][j];     //ˢ��ǰһ�εı���ֵ
            }
 
            if (KeyDownTime[i][j] > 0)            //���ִ�п�������
            {
                if (KeyDownTime[i][j] >= TimeThr[i][j])
                {                                 //�ﵽ��ֵʱִ��һ�ζ���
                    ret =  KeyLongCodeMap[i][j];  
                    TimeThr[i][j] += 200; //ʱ����ֵ����200ms����׼���´�ִ��
                }
            }
            else   //��������ʱ��λ��ֵʱ��
            {
                TimeThr[i][j] = 1000;  //�ָ�1s�ĳ�ʼ��ֵʱ��
            }
        }
    }  
	return ret;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_MtxKeyScan
*	����˵��: ɨ�����а���������������systick�ж������Եĵ���
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_MtxKeyScan(void)
{
    uint8_t i;
    static uint8_t keyout = 0;   //���󰴼�ɨ���������
    static uint8_t keybuf[KEY_LINE_NUM][KEY_COL_NUM] = {  //���󰴼�ɨ�軺����
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
		{0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  
		{0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
    };

    //��һ�е�4������ֵ���뻺����
    keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEY_IN_1;
    keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEY_IN_2;
    keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEY_IN_3;
    keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEY_IN_4;
	keybuf[keyout][4] = (keybuf[keyout][4] << 1) | KEY_IN_5;
    //��������°���״̬
    for (i=0; i<KEY_COL_NUM; i++)  //ÿ��5������������ѭ��5��
    {
        if ((keybuf[keyout][i] & 0x0F) == 0x00)
        {   //����4��ɨ��ֵΪ0����4*5ms�ڶ��ǰ���״̬ʱ������Ϊ�������ȶ��İ���
            KeySta[keyout][i] = 0;
            KeyDownTime[keyout][i] += 20;
        }
        else if ((keybuf[keyout][i] & 0x0F) == 0x0F)
        {   //����4��ɨ��ֵΪ1����4*5ms�ڶ��ǵ���״̬ʱ������Ϊ�������ȶ��ĵ���
            KeySta[keyout][i] = 1;
            KeyDownTime[keyout][i] = 0;
        }
    }
    //ִ����һ�ε�ɨ�����
	if(++keyout >= KEY_LINE_NUM)
	{
		keyout = 0;
	}
    switch (keyout)  //��������ֵ���ͷŵ�ǰ������ţ������´ε��������
    {
        case 0: KOUT4_1(); KOUT1_0(); break;
        case 1: KOUT1_1(); KOUT2_0(); break;
        case 2: KOUT2_1(); KOUT3_0(); break;
        case 3: KOUT3_1(); KOUT4_0(); break;
        default: break;
    }
}
/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
