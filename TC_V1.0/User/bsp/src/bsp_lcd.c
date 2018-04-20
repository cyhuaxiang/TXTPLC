/*
*********************************************************************************************************
*
*	模块名称 : LCD显示器驱动模块
*	文件名称 : bsp_lcd.c
*	版    本 : V1.0
*	说    明 : 
*				
*	修改记录 :
*		版本号  日期        作者    说明
*		V1.0   2017-02-02   cy     初次发布
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


/* 19264 LCD的显存镜像，占用1K字节. 共8行，每行192像素 */
static uint8_t s_ucGRAM[8][192];

/* 为了避免刷屏拉幕感太强，引入刷屏标志。
0 表示显示函数只改写缓冲区，不写屏。1 表示直接写屏（同时写缓冲区） */
static uint8_t s_ucUpdateEn = 1;

static void LCD_WriteCmd(uint8_t _ucCmd);
static void LCD_WriteData(uint8_t _ucData);
static void LCD_BufToPanel(void);
static void LCD_SelScreen(uint8_t _ucCs);

/*
*********************************************************************************************************
*	函 数 名: LCD_InitHard
*	功能说明: 初始化LCD屏
*	形    参:  无
*	返 回 值: 无
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

	/* 上电延迟 */
	bsp_DelayMS(50);
	LCD_SelScreen(1);
	LCD_WriteCmd(LCD_OFF);//关显示
	LCD_SelScreen(2);
	LCD_WriteCmd(LCD_OFF);
	LCD_SelScreen(3);
	LCD_WriteCmd(LCD_OFF);
	
	LCD_SelScreen(1);
	LCD_WriteCmd(LCD_XSET);//起始页地址
	LCD_SelScreen(2);
	LCD_WriteCmd(LCD_XSET);
	LCD_SelScreen(3);
	LCD_WriteCmd(LCD_XSET);
	
	LCD_SelScreen(1);
	LCD_WriteCmd(LCD_STARTLINE);//起始行
	LCD_SelScreen(2);
	LCD_WriteCmd(LCD_STARTLINE);
	LCD_SelScreen(3);
	LCD_WriteCmd(LCD_STARTLINE);
	
	LCD_SelScreen(1);
	LCD_WriteCmd(LCD_YSET);//起始列地址
	LCD_SelScreen(2);
	LCD_WriteCmd(LCD_YSET);
	LCD_SelScreen(3);
	LCD_WriteCmd(LCD_YSET);
	
	LCD_SelScreen(1);
	LCD_WriteCmd(LCD_ON);//开显示
	LCD_SelScreen(2);
	LCD_WriteCmd(LCD_ON);
	LCD_SelScreen(3);
	LCD_WriteCmd(LCD_ON);
}
/*
*********************************************************************************************************
*	函 数 名: LCD_SelScreen
*	功能说明: 选择屏幕
*	形    参: 
*	返 回 值: 无
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
*	函 数 名: LCD_DispOn
*	功能说明: 打开显示
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispOn(void)
{
	
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DispOff
*	功能说明: 关闭显示
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispOff(void)
{
	
}

/*
*********************************************************************************************************
*	函 数 名: LCD_SetDir
*	功能说明: 设置显示方向
*	形    参: _ucDir = 0 表示正常方向，1表示翻转180度
*	返 回 值: 无
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
*	函 数 名: LCD_SetContrast
*	功能说明: 设置对比度
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_SetContrast(uint8_t ucValue)
{
	
}

/*
*********************************************************************************************************
*	函 数 名: LCD_StartDraw
*	功能说明: 开始绘图。以后绘图函数只改写缓冲区，不改写面板显存
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_StartDraw(void)
{
	s_ucUpdateEn = 0;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_EndDraw
*	功能说明: 结束绘图。缓冲区的数据刷新到面板显存。 LCD_StartDraw() 和 LCD_EndDraw() 必须成对使用
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_EndDraw(void)
{
	s_ucUpdateEn = 1;
	LCD_BufToPanel();
}

/*
*********************************************************************************************************
*	函 数 名: LCD_ClrScr
*	功能说明: 清屏
*	形    参:  _ucMode : 0 表示全黑； 0xFF表示全亮
*	返 回 值: 无
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
*	函 数 名: LCD_BufToPanel
*	功能说明: 将缓冲区中的点阵数据写入面板
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void LCD_BufToPanel(void)
{
	uint8_t i,j;

	LCD_SelScreen(1);
	for (i = 0 ; i< 8; i++)
	{
		LCD_WriteCmd (LCD_XSET + i);	/* 设置页地址（0~7） */
		LCD_WriteCmd (LCD_YSET);
		for (j = 0 ; j < 64; j++)
		{
			LCD_WriteData(s_ucGRAM[i][j]);
		}
	}
	LCD_SelScreen(2);
	for (i = 0 ; i< 8; i++)
	{
		LCD_WriteCmd (LCD_XSET + i);	/* 设置页地址（0~7） */
		LCD_WriteCmd (LCD_YSET);
		for (j = 64 ; j < 128; j++)
		{
			LCD_WriteData(s_ucGRAM[i][j]);
		}
	}
	LCD_SelScreen(3);
	for (i = 0 ; i< 8; i++)
	{
		LCD_WriteCmd (LCD_XSET + i);	/* 设置页地址（0~7） */
		LCD_WriteCmd (LCD_YSET);
		for (j = 128 ; j < 192; j++)
		{
			LCD_WriteData(s_ucGRAM[i][j]);
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DispStr
*	功能说明: 在屏幕指定坐标（左上角为0，0）显示一个字符串
*	形    参:
*		_usX : X坐标，对于19264屏，范围为【0 - 192】
*		_usY : Y坐标，对于19264屏，范围为【0 - 63】
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 无
*********************************************************************************************************
*/
//void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
//{
//	uint32_t i;
//	uint8_t code1;
//	uint8_t code2;
//	uint32_t address = 0;
//	uint8_t buf[32 * 32 / 8];	/* 最大支持32点阵汉字 */
//	uint8_t m, width;
//	uint8_t font_width,font_height, font_bytes;
//	uint16_t x, y;
//	const uint8_t *pAscDot;	

//#ifdef USE_SMALL_FONT		
//	const uint8_t *pHzDot;
//#else	
//	uint32_t AddrHZK;
//#endif	

//	/* 如果字体结构为空指针，则缺省按16点阵 */
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
//		/* 缺省是16点阵 */
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

//	/* 开始循环处理字符 */
//	while (*_ptr != 0)
//	{
//		code1 = *_ptr;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
//		if (code1 < 0x80)
//		{
//			/* 将ascii字符点阵复制到buf */
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

//			/* 计算16点阵汉字点阵地址
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
//						/* 字库搜索完毕，未找到，则填充全FF */
//						memset(buf, 0xFF, font_bytes);
//						break;
//					}
//				}
//			#else	/* 用全字库 */
//				/* 此处需要根据字库文件存放位置进行修改 */
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
//		/* 开始刷LCD */
//		for (m = 0; m < font_height; m++)	/* 字符高度 */
//		{
//			x = _usX;
//			for (i = 0; i < width; i++)	/* 字符宽度 */
//			{
//				if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
//				{
//					LCD_PutPixel(x, y, _tFont->FrontColor);	/* 设置像素颜色为文字色 */
//				}
//				else
//				{
//					LCD_PutPixel(x, y, _tFont->BackColor);	/* 设置像素颜色为文字背景色 */
//				}

//				x++;
//			}
//			y++;
//		}

//		if (_tFont->Space > 0)
//		{
//			/* 如果文字底色按_tFont->usBackColor，并且字间距大于点阵的宽度，那么需要在文字之间填充(暂时未实现) */
//		}
//		_usX += width + _tFont->Space;	/* 列地址递增 */
//		_ptr++;			/* 指向下一个字符 */
//	}
//}

/*
*********************************************************************************************************
*	函 数 名: LCD_PutPixel
*	功能说明: 画1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_ucColor  ：像素颜色
*	返 回 值: 无
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
	/*判断绘点区域*/
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
		LCD_WriteCmd (LCD_XSET + ucPageAddr);					/* 设置页地址（0~7） */
		LCD_WriteCmd (LCD_YSET + ucColAddr);			/* 设置列地址*/
		
		LCD_WriteData(ucValue);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetPixel
*	功能说明: 读取1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*	返 回 值: 颜色值 (0, 1)
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
*	函 数 名: LCD_DrawLine
*	功能说明: 采用 Bresenham 算法，在2点间画一条直线。
*	形    参:
*			_usX1, _usY1 ：起始点坐标
*			_usX2, _usY2 ：终止点Y坐标
*			_ucColor     ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor)
{
	int32_t dx , dy ;
	int32_t tx , ty ;
	int32_t inc1 , inc2 ;
	int32_t d , iTag ;
	int32_t x , y ;

	/* 采用 Bresenham 算法，在2点间画一条直线 */

	LCD_PutPixel(_usX1 , _usY1 , _ucColor);

	/* 如果两点重合，结束后面的动作。*/
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

	if ( dx < dy )   /*如果dy为计长方向，则交换纵横坐标。*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = _usX1; _usX1 = _usY1; _usY1 = temp;
		temp = _usX2; _usX2 = _usY2; _usY2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = _usX2 > _usX1 ? 1 : -1 ;    /* 确定是增1还是减1 */
	ty = _usY2 > _usY1 ? 1 : -1 ;
	x = _usX1 ;
	y = _usY1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != _usX2 )     /* 循环画点 */
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
*	函 数 名: LCD_DrawPoints
*	功能说明: 采用 Bresenham 算法，绘制一组点，并将这些点连接起来。可用于波形显示。
*	形    参:
*			x, y     ：坐标数组
*			_ucColor ：颜色
*	返 回 值: 无
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
*	函 数 名: LCD_DrawRect
*	功能说明: 绘制矩形。
*	形    参:
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor)
{
	/*
	 ---------------->---
	|(_usX，_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	LCD_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _ucColor);	/* 顶 */
	LCD_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _ucColor);	/* 底 */

	LCD_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _ucColor);	/* 左 */
	LCD_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _ucColor);	/* 右 */
}
/*
*********************************************************************************************************
*	函 数 名: LCD_Fill_Rect
*	功能说明: 填充矩形，反色填充。
*	形    参:
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Fill_Rect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor)
{
	uint8_t i,j;
	uint16_t x , y ;
	
	i = _ucColor;/* 仅仅用于避免告警 */
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
*	函 数 名: LCD_DrawCircle
*	功能说明: 绘制一个圆，笔宽为1个像素
*	形    参:
*			_usX,_usY  ：圆心的坐标
*			_usRadius  ：圆的半径
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* 当前 X 值 */
	uint32_t  CurY;		/* 当前 Y 值 */

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
*	函 数 名: LCD_DrawBMP
*	功能说明: 在LCD上显示一个BMP位图，位图点阵扫描次序：从左到右，从上到下
*	形    参:
*			_usX, _usY : 图片的坐标
*			_usHeight  ：图片高度
*			_usWidth   ：图片宽度
*			_ptr       ：单色图片点阵指针，每个像素占用1个字节
*	返 回 值: 无
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
*	函 数 名: LCD_WaitBusy
*	功能说明: 向KS0108发送一字节命令
*	形    参: 屏幕区域片选
*	返 回 值: 无
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
        sta = GPIO_GET_IN_DATA(LCD_BUS); //读取状态字
        LCD_EN_0();
    } while (sta & 0x80); //bit7等于1表示液晶正忙，重复检测直到其等于0为止
	GPIO_SetMode(LCD_BUS, BIT7, GPIO_PMD_OUTPUT);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_WriteCmd
*	功能说明: 向KS0108发送一字节命令
*	形    参:  命令字
*	返 回 值: 无
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
*	函 数 名: LCD_WriteData
*	功能说明: 向KS0108发送一字节数据
*	形    参:  命令字
*	返 回 值: 无
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

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
