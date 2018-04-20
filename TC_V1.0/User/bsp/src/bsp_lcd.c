/*
*********************************************************************************************************
*
*	ģ������ : LCD��ʾ������ģ��
*	�ļ����� : bsp_lcd.c
*	��    �� : V1.0
*	˵    �� : 
*				
*	�޸ļ�¼ :
*		�汾��  ����        ����    ˵��
*		V1.0   2017-02-02   cy     ���η���
*	Copyright (C), 2010-2012, XXXXXXXXXX
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "fonts.h"

#define LCD_RS_1()		P45 = 1
#define LCD_RS_0()		P45 = 0


#define LCD_RW_1()		P44 = 1
#define LCD_RW_0()		P44 = 0

#define LCD_EN_1()		P27 = 1
#define LCD_EN_0()		P27 = 0


#define LCD_CS1_1()		P26 = 1
#define LCD_CS1_0()		P26 = 0

#define LCD_CS2_1()		P25 = 1
#define LCD_CS2_0()		P25 = 0

#define LCD_CS3_1()		P40 = 1
#define LCD_CS3_0()		P40 = 0

#define LCD_BUS		    P0

/* LCD Registers */
#define LCD_ON			0x3F	// Turn display on		
#define LCD_OFF			0x3E	// Turn display off
#define LCD_XSET		0xB8	// Adress base for Page 0
#define LCD_YSET		0x40	// Adress base for Y0
#define LCD_STARTLINE	0xC0	// Adress base for line 0


/* 19264 LCD���Դ澵��ռ��1K�ֽ�. ��8�У�ÿ��192���� */
static uint8_t s_ucGRAM[8][192];

/* Ϊ�˱���ˢ����Ļ��̫ǿ������ˢ����־��
0 ��ʾ��ʾ����ֻ��д����������д����1 ��ʾֱ��д����ͬʱд�������� */
static uint8_t s_ucUpdateEn = 1;

static void LCD_WriteCmd(uint8_t _ucCmd);
static void LCD_WriteData(uint8_t _ucData);
static void LCD_BufToPanel(void);
static void LCD_SelScreen(uint8_t _ucCs);

/*
*********************************************************************************************************
*	�� �� ��: LCD_InitHard
*	����˵��: ��ʼ��LCD��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_InitHard(void)
{
	GPIO_SetMode(P4, BIT5, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P4, BIT4, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P2, BIT7, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P2, BIT6, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P2, BIT5, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P4, BIT0, GPIO_PMD_OUTPUT);
	
	GPIO_SetMode(P0, BIT0, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P0, BIT1, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P0, BIT2, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P0, BIT3, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P0, BIT4, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P0, BIT5, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P0, BIT6, GPIO_PMD_OUTPUT);
	GPIO_SetMode(P0, BIT7, GPIO_PMD_OUTPUT);

	/* �ϵ��ӳ� */
	bsp_DelayMS(50);
	LCD_SelScreen(1);
	LCD_WriteCmd(LCD_OFF);//����ʾ
	LCD_SelScreen(2);
	LCD_WriteCmd(LCD_OFF);
	LCD_SelScreen(3);
	LCD_WriteCmd(LCD_OFF);
	
	LCD_SelScreen(1);
	LCD_WriteCmd(LCD_XSET);//��ʼҳ��ַ
	LCD_SelScreen(2);
	LCD_WriteCmd(LCD_XSET);
	LCD_SelScreen(3);
	LCD_WriteCmd(LCD_XSET);
	
	LCD_SelScreen(1);
	LCD_WriteCmd(LCD_STARTLINE);//��ʼ��
	LCD_SelScreen(2);
	LCD_WriteCmd(LCD_STARTLINE);
	LCD_SelScreen(3);
	LCD_WriteCmd(LCD_STARTLINE);
	
	LCD_SelScreen(1);
	LCD_WriteCmd(LCD_YSET);//��ʼ�е�ַ
	LCD_SelScreen(2);
	LCD_WriteCmd(LCD_YSET);
	LCD_SelScreen(3);
	LCD_WriteCmd(LCD_YSET);
	
	LCD_SelScreen(1);
	LCD_WriteCmd(LCD_ON);//����ʾ
	LCD_SelScreen(2);
	LCD_WriteCmd(LCD_ON);
	LCD_SelScreen(3);
	LCD_WriteCmd(LCD_ON);
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_SelScreen
*	����˵��: ѡ����Ļ
*	��    ��: 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void LCD_SelScreen(uint8_t _ucCs)
{
	if(_ucCs == 1)
	{
		LCD_CS1_0();
		LCD_CS2_1();
		LCD_CS3_1();
	}
	else if(_ucCs == 2)
	{
		LCD_CS1_1();
		LCD_CS2_0();
		LCD_CS3_1();
	}
	else if(_ucCs == 3)
	{
		LCD_CS1_1();
		LCD_CS2_1();
		LCD_CS3_0();
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_DispOn
*	����˵��: ����ʾ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DispOn(void)
{
	
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DispOff
*	����˵��: �ر���ʾ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DispOff(void)
{
	
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_SetDir
*	����˵��: ������ʾ����
*	��    ��: _ucDir = 0 ��ʾ��������1��ʾ��ת180��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_SetDir(uint8_t _ucDir)
{
	if (_ucDir == 0)
	{
       	
	}
	else
	{
		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_SetContrast
*	����˵��: ���öԱȶ�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_SetContrast(uint8_t ucValue)
{
	
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_StartDraw
*	����˵��: ��ʼ��ͼ���Ժ��ͼ����ֻ��д������������д����Դ�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_StartDraw(void)
{
	s_ucUpdateEn = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_EndDraw
*	����˵��: ������ͼ��������������ˢ�µ�����Դ档 LCD_StartDraw() �� LCD_EndDraw() ����ɶ�ʹ��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_EndDraw(void)
{
	s_ucUpdateEn = 1;
	LCD_BufToPanel();
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_ClrScr
*	����˵��: ����
*	��    ��:  _ucMode : 0 ��ʾȫ�ڣ� 0xFF��ʾȫ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_ClrScr(uint8_t _ucMode)
{
	uint8_t i,j;

	for (i = 0 ; i < 8; i++)
	{
		for (j = 0 ; j < 192; j++)
		{
			s_ucGRAM[i][j] = _ucMode;
		}
	}

	if (s_ucUpdateEn == 1)
	{
		LCD_BufToPanel();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_BufToPanel
*	����˵��: ���������еĵ�������д�����
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void LCD_BufToPanel(void)
{
	uint8_t i,j;

	LCD_SelScreen(1);
	for (i = 0 ; i< 8; i++)
	{
		LCD_WriteCmd (LCD_XSET + i);	/* ����ҳ��ַ��0~7�� */
		LCD_WriteCmd (LCD_YSET);
		for (j = 0 ; j < 64; j++)
		{
			LCD_WriteData(s_ucGRAM[i][j]);
		}
	}
	LCD_SelScreen(2);
	for (i = 0 ; i< 8; i++)
	{
		LCD_WriteCmd (LCD_XSET + i);	/* ����ҳ��ַ��0~7�� */
		LCD_WriteCmd (LCD_YSET);
		for (j = 64 ; j < 128; j++)
		{
			LCD_WriteData(s_ucGRAM[i][j]);
		}
	}
	LCD_SelScreen(3);
	for (i = 0 ; i< 8; i++)
	{
		LCD_WriteCmd (LCD_XSET + i);	/* ����ҳ��ַ��0~7�� */
		LCD_WriteCmd (LCD_YSET);
		for (j = 128 ; j < 192; j++)
		{
			LCD_WriteData(s_ucGRAM[i][j]);
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DispStr
*	����˵��: ����Ļָ�����꣨���Ͻ�Ϊ0��0����ʾһ���ַ���
*	��    ��:
*		_usX : X���꣬����19264������ΧΪ��0 - 192��
*		_usY : Y���꣬����19264������ΧΪ��0 - 63��
*		_ptr  : �ַ���ָ��
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
//{
//	uint32_t i;
//	uint8_t code1;
//	uint8_t code2;
//	uint32_t address = 0;
//	uint8_t buf[32 * 32 / 8];	/* ���֧��32������ */
//	uint8_t m, width;
//	uint8_t font_width,font_height, font_bytes;
//	uint16_t x, y;
//	const uint8_t *pAscDot;	

//#ifdef USE_SMALL_FONT		
//	const uint8_t *pHzDot;
//#else	
//	uint32_t AddrHZK;
//#endif	

//	/* �������ṹΪ��ָ�룬��ȱʡ��16���� */
//	if (_tFont->FontCode == FC_ST_12)
//	{
//		font_height = 12;
//		font_width = 12;
//		font_bytes = 24;
//		pAscDot = g_Ascii12;
//	#ifdef USE_SMALL_FONT		
//		pHzDot = g_Hz12;
//	#else		
//		AddrHZK = HZK12_ADDR;
//	#endif		
//	}
//	else
//	{
//		/* ȱʡ��16���� */
//		font_height = 16;
//		font_width = 16;
//		font_bytes = 32;
//		pAscDot = g_Ascii16;
//	#ifdef USE_SMALL_FONT		
//		pHzDot = g_Hz16;
//	#else
//		AddrHZK = HZK16_ADDR;
//	#endif
//	}

//	/* ��ʼѭ�������ַ� */
//	while (*_ptr != 0)
//	{
//		code1 = *_ptr;	/* ��ȡ�ַ������ݣ� �����ݿ�����ascii���룬Ҳ���ܺ��ִ���ĸ��ֽ� */
//		if (code1 < 0x80)
//		{
//			/* ��ascii�ַ������Ƶ�buf */
//			memcpy(buf, &pAscDot[(code1-0x20) * (font_bytes / 2)], (font_bytes / 2));
//			width = font_width / 2;
//		}
//		else
//		{
//			code2 = *++_ptr;
//			if (code2 == 0)
//			{
//				break;
//			}

//			/* ����16�����ֵ����ַ
//				ADDRESS = [(code1-0xa1) * 94 + (code2-0xa1)] * 32
//				;
//			*/
//			#ifdef USE_SMALL_FONT
//				m = 0;
//				while(1)
//				{
//					address = m * (font_bytes + 2);
//					m++;
//					if ((code1 == pHzDot[address + 0]) && (code2 == pHzDot[address + 1]))
//					{
//						address += 2;
//						memcpy(buf, &pHzDot[address], font_bytes);
//						break;
//					}
//					else if ((pHzDot[address + 0] == 0xFF) && (pHzDot[address + 1] == 0xFF))
//					{
//						/* �ֿ�������ϣ�δ�ҵ��������ȫFF */
//						memset(buf, 0xFF, font_bytes);
//						break;
//					}
//				}
//			#else	/* ��ȫ�ֿ� */
//				/* �˴���Ҫ�����ֿ��ļ����λ�ý����޸� */
//				if (code1 >=0xA1 && code1 <= 0xA9 && code2 >=0xA1)
//				{
//					address = ((code1 - 0xA1) * 94 + (code2 - 0xA1)) * font_bytes + AddrHZK;
//				}
//				else if (code1 >=0xB0 && code1 <= 0xF7 && code2 >=0xA1)
//				{
//					address = ((code1 - 0xB0) * 94 + (code2 - 0xA1) + 846) * font_bytes + AddrHZK;
//				}
//				memcpy(buf, (const uint8_t *)address, font_bytes);
//			#endif

//				width = font_width;
//		}

//		y = _usY;
//		/* ��ʼˢLCD */
//		for (m = 0; m < font_height; m++)	/* �ַ��߶� */
//		{
//			x = _usX;
//			for (i = 0; i < width; i++)	/* �ַ���� */
//			{
//				if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
//				{
//					LCD_PutPixel(x, y, _tFont->FrontColor);	/* ����������ɫΪ����ɫ */
//				}
//				else
//				{
//					LCD_PutPixel(x, y, _tFont->BackColor);	/* ����������ɫΪ���ֱ���ɫ */
//				}

//				x++;
//			}
//			y++;
//		}

//		if (_tFont->Space > 0)
//		{
//			/* ������ֵ�ɫ��_tFont->usBackColor�������ּ����ڵ���Ŀ�ȣ���ô��Ҫ������֮�����(��ʱδʵ��) */
//		}
//		_usX += width + _tFont->Space;	/* �е�ַ���� */
//		_ptr++;			/* ָ����һ���ַ� */
//	}
//}

/*
*********************************************************************************************************
*	�� �� ��: LCD_PutPixel
*	����˵��: ��1������
*	��    ��:
*			_usX,_usY : ��������
*			_ucColor  ��������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor)
{
	uint8_t ucValue;
	uint8_t ucPageAddr;
	uint8_t ucColAddr;

	const uint8_t aOrTab[8]  = {0x01, 0x02, 0x04, 0x08,0x10,0x20,0x40,0x80};
	const uint8_t aAndTab[8] = {0xFE, 0xFD, 0xFB, 0xF7,0xEF,0xDF,0xBF,0x7F};

	
	
	ucPageAddr = _usY / 8;
	ucColAddr = _usX;

	ucValue = s_ucGRAM[ucPageAddr][ucColAddr];
	if (_ucColor == 0)
	{
		ucValue &= aAndTab[_usY % 8];
	}
	else
	{
		ucValue |= aOrTab[_usY % 8];
	}
	s_ucGRAM[ucPageAddr][ucColAddr] = ucValue;
	/*�жϻ������*/
	if(_usX < 64)
	{
		LCD_SelScreen(1);
	}
	else if(_usX < 128)
	{
		LCD_SelScreen(2);
		ucColAddr -= 64;
	}
	else if(_usX < 192)
	{
		LCD_SelScreen(3);
		ucColAddr -= 128;
	}
	else
	{
		return;
	}
	
	if (s_ucUpdateEn == 1)
	{
		LCD_WriteCmd (LCD_XSET + ucPageAddr);					/* ����ҳ��ַ��0~7�� */
		LCD_WriteCmd (LCD_YSET + ucColAddr);			/* �����е�ַ*/
		
		LCD_WriteData(ucValue);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_GetPixel
*	����˵��: ��ȡ1������
*	��    ��:
*			_usX,_usY : ��������
*	�� �� ֵ: ��ɫֵ (0, 1)
*********************************************************************************************************
*/
uint8_t LCD_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint8_t ucValue;
	uint8_t ucPageAddr;
	uint8_t ucColAddr;
	const uint8_t aOrTab[8]  = {0x01, 0x02, 0x04, 0x08,0x10,0x20,0x40,0x80};
	
	ucPageAddr = _usY / 8;
	ucColAddr = _usX;

	ucValue = s_ucGRAM[ucPageAddr][ucColAddr];
	if (ucValue & (aOrTab[_usY % 8]))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawLine
*	����˵��: ���� Bresenham �㷨����2��仭һ��ֱ�ߡ�
*	��    ��:
*			_usX1, _usY1 ����ʼ������
*			_usX2, _usY2 ����ֹ��Y����
*			_ucColor     ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor)
{
	int32_t dx , dy ;
	int32_t tx , ty ;
	int32_t inc1 , inc2 ;
	int32_t d , iTag ;
	int32_t x , y ;

	/* ���� Bresenham �㷨����2��仭һ��ֱ�� */

	LCD_PutPixel(_usX1 , _usY1 , _ucColor);

	/* ��������غϣ���������Ķ�����*/
	if ( _usX1 == _usX2 && _usY1 == _usY2 )
	{
		return;
	}

	iTag = 0 ;
	/* dx = abs ( _usX2 - _usX1 ); */
	if (_usX2 >= _usX1)
	{
		dx = _usX2 - _usX1;
	}
	else
	{
		dx = _usX1 - _usX2;
	}

	/* dy = abs ( _usY2 - _usY1 ); */
	if (_usY2 >= _usY1)
	{
		dy = _usY2 - _usY1;
	}
	else
	{
		dy = _usY1 - _usY2;
	}

	if ( dx < dy )   /*���dyΪ�Ƴ������򽻻��ݺ����ꡣ*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = _usX1; _usX1 = _usY1; _usY1 = temp;
		temp = _usX2; _usX2 = _usY2; _usY2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = _usX2 > _usX1 ? 1 : -1 ;    /* ȷ������1���Ǽ�1 */
	ty = _usY2 > _usY1 ? 1 : -1 ;
	x = _usX1 ;
	y = _usY1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != _usX2 )     /* ѭ������ */
	{
		if ( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if(_ucColor <= 1)
		{
			if ( iTag )
			{
				LCD_PutPixel ( y , x , _ucColor) ;
			}
			else
			{
				LCD_PutPixel ( x , y , _ucColor) ;
			}
		}
		else
		{
			if ( iTag )
			{
				
				if((x-_usX1)%2)
				{
					LCD_PutPixel ( y , x , 0) ;
				}
				else
				{
					LCD_PutPixel ( y , x , 1) ;
				}
			}
			else
			{
				if((x-_usX1)%2)
				{
					LCD_PutPixel ( x , y , 0) ;
				}
				else
				{
					LCD_PutPixel ( x , y , 1) ;
				}
			}
		}
		
		x += tx ;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawPoints
*	����˵��: ���� Bresenham �㷨������һ��㣬������Щ�����������������ڲ�����ʾ��
*	��    ��:
*			x, y     ����������
*			_ucColor ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor)
{
	uint16_t i;

	for (i = 0 ; i < _usSize - 1; i++)
	{
		LCD_DrawLine(x[i], y[i], x[i + 1], y[i + 1], _ucColor);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawRect
*	����˵��: ���ƾ��Ρ�
*	��    ��:
*			_usX,_usY���������Ͻǵ�����
*			_usHeight �����εĸ߶�
*			_usWidth  �����εĿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor)
{
	/*
	 ---------------->---
	|(_usX��_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	LCD_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _ucColor);	/* �� */
	LCD_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _ucColor);	/* �� */

	LCD_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _ucColor);	/* �� */
	LCD_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _ucColor);	/* �� */
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_Fill_Rect
*	����˵��: �����Σ���ɫ��䡣
*	��    ��:
*			_usX,_usY���������Ͻǵ�����
*			_usHeight �����εĸ߶�
*			_usWidth  �����εĿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_Fill_Rect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor)
{
	uint8_t i,j;
	uint16_t x , y ;
	
	i = _ucColor;/* �������ڱ���澯 */
	for (i = 0 ; i< _usHeight; i++)
	{
		_ucColor = i;
		for (j = 0 ; j < _usWidth; j++)
		{
			x = _usX+j;
			y = _usY+i;
			if(LCD_GetPixel(x,y))
			{
				LCD_PutPixel(x,y,0);
			}
			else
			{
				LCD_PutPixel(x,y,1);
			}
		}
	}	
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawCircle
*	����˵��: ����һ��Բ���ʿ�Ϊ1������
*	��    ��:
*			_usX,_usY  ��Բ�ĵ�����
*			_usRadius  ��Բ�İ뾶
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* ��ǰ X ֵ */
	uint32_t  CurY;		/* ��ǰ Y ֵ */

	D = 3 - (_usRadius << 1);
	CurX = 0;
	CurY = _usRadius;

	while (CurX <= CurY)
	{
		LCD_PutPixel(_usX + CurX, _usY + CurY, _ucColor);
		LCD_PutPixel(_usX + CurX, _usY - CurY, _ucColor);
		LCD_PutPixel(_usX - CurX, _usY + CurY, _ucColor);
		LCD_PutPixel(_usX - CurX, _usY - CurY, _ucColor);
		LCD_PutPixel(_usX + CurY, _usY + CurX, _ucColor);
		LCD_PutPixel(_usX + CurY, _usY - CurX, _ucColor);
		LCD_PutPixel(_usX - CurY, _usY + CurX, _ucColor);
		LCD_PutPixel(_usX - CurY, _usY - CurX, _ucColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawBMP
*	����˵��: ��LCD����ʾһ��BMPλͼ��λͼ����ɨ����򣺴����ң����ϵ���
*	��    ��:
*			_usX, _usY : ͼƬ������
*			_usHeight  ��ͼƬ�߶�
*			_usWidth   ��ͼƬ���
*			_ptr       ����ɫͼƬ����ָ�룬ÿ������ռ��1���ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr)
{
	uint16_t x, y;

	for (x = 0; x < _usWidth; x++)
	{
		for (y = 0; y < _usHeight; y++)
		{
			LCD_PutPixel(_usX + x, _usY + y, *_ptr);
		}
	}
}


/*
*********************************************************************************************************
*	�� �� ��: LCD_WaitBusy
*	����˵��: ��KS0108����һ�ֽ�����
*	��    ��: ��Ļ����Ƭѡ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void LCD_WaitBusy()
{
	uint8_t sta;
	
	GPIO_SetMode(LCD_BUS, BIT7, GPIO_PMD_QUASI);
	GPIO_SET_OUT_DATA(LCD_BUS, 0xFF);
	LCD_EN_0();
    LCD_RS_0();
    LCD_RW_1();
    do 
    {
        LCD_EN_1();
        bsp_DelayUS(20);
        sta = GPIO_GET_IN_DATA(LCD_BUS); //��ȡ״̬��
        LCD_EN_0();
    } while (sta & 0x80); //bit7����1��ʾҺ����æ���ظ����ֱ�������0Ϊֹ
	GPIO_SetMode(LCD_BUS, BIT7, GPIO_PMD_OUTPUT);
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_WriteCmd
*	����˵��: ��KS0108����һ�ֽ�����
*	��    ��:  ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void LCD_WriteCmd(uint8_t _ucCmd)
{
    LCD_WaitBusy();
	LCD_EN_0();
	LCD_RS_0();
    LCD_RW_0();
   
    GPIO_SET_OUT_DATA(LCD_BUS, _ucCmd);
    LCD_EN_1();
    bsp_DelayUS(20);
    LCD_EN_0();
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_WriteData
*	����˵��: ��KS0108����һ�ֽ�����
*	��    ��:  ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void LCD_WriteData(uint8_t _ucData)
{
	LCD_WaitBusy();
	LCD_EN_0();
	LCD_RS_1();
    LCD_RW_0();
    
    GPIO_SET_OUT_DATA(LCD_BUS, _ucData);
    LCD_EN_1();
    bsp_DelayUS(20);
    LCD_EN_0();
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
