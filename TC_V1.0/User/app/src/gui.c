#include "bsp.h"
#include "gui.h"
#include "fonts.h"

/*
*********************************************************************************************************
*	函 数 名: LCD_GetFontWidth
*	功能说明: 读取字体的宽度（像素单位)
*	形    参:
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 字体的宽度（像素单位)
*********************************************************************************************************
*/
uint16_t LCD_GetFontWidth(FONT_T *_tFont)
{
	uint16_t font_width = 16;

	switch (_tFont->FontCode)
	{
		case FC_ST_12:
			font_width = 12;
			break;

		case FC_ST_16:
		case FC_RA8875_16:			
			font_width = 16;
			break;
			
		case FC_RA8875_24:			
		case FC_ST_24:
			font_width = 24;
			break;
			
		case FC_ST_32:
		case FC_RA8875_32:	
			font_width = 32;
			break;			
	}
	return font_width;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetFontHeight
*	功能说明: 读取字体的高度（像素单位)
*	形    参:
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 字体的宽度（像素单位)
*********************************************************************************************************
*/
uint16_t LCD_GetFontHeight(FONT_T *_tFont)
{
	uint16_t height = 16;

	switch (_tFont->FontCode)
	{
		case FC_ST_12:
			height = 12;
			break;

		case FC_ST_16:
		case FC_RA8875_16:			
			height = 16;
			break;
			
		case FC_RA8875_24:			
		case FC_ST_24:
			height = 24;
			break;
			
		case FC_ST_32:
		case FC_RA8875_32:	
			height = 32;
			break;			
	}
	return height;
}
/*
*********************************************************************************************************
*	函 数 名: LCD_GetStrWidth
*	功能说明: 计算字符串宽度(像素单位)
*	形    参:
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LCD_GetStrWidth(char *_ptr, FONT_T *_tFont)
{
	char *p = _ptr;
	uint16_t width = 0;
	uint8_t code1, code2;
	uint16_t font_width;

	font_width = LCD_GetFontWidth(_tFont);

	while (*p != 0)
	{
		code1 = *p;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
		if (code1 < 0x80)	/* ASCII */
		{
			switch(_tFont->FontCode)
			{
				case FC_ST_12:
					font_width = 6;
					break;

				case FC_ST_16:		
					font_width = 8;
					break;
					
				case FC_ST_24:			
					font_width = 12;
					break;
					
				case FC_ST_32:
					font_width = 16;
					break;
				
				default:
					font_width = 8;
					break;					
			}
			
		}
		else	/* 汉字 */
		{
			code2 = *++p;
			if (code2 == 0)
			{
				break;
			}
			font_width = LCD_GetFontWidth(_tFont);
			
		}
		width += font_width;
		p++;
	}

	return width;
}

/*
*********************************************************************************************************
*	函 数 名: _LCD_ReadAsciiDot
*	功能说明: 读取1个ASCII字符的点阵数据
*	形    参:
*		_code : ASCII字符的编码，1字节。1-128
*		_fontcode ：字体代码
*		_pBuf : 存放读出的字符点阵数据
*	返 回 值: 文字宽度
*********************************************************************************************************
*/
static void _LCD_ReadAsciiDot(uint8_t _code, uint8_t _fontcode, uint8_t *_pBuf)
{
#ifdef USE_SMALL_FONT	/* 使用CPU 内部Flash 小字库 */
	const uint8_t *pAscDot;
	uint8_t font_bytes = 0;
	uint16_t m;
	uint16_t address;
	uint8_t fAllHz = 1;	/* 1表示程序中内嵌全部的ASCII字符集 */

	pAscDot = 0;
	switch (_fontcode)
	{
		case FC_ST_12:		/* 12点阵 */
			font_bytes = 24 / 2;
			pAscDot = g_Ascii12;	
			fAllHz = 1;
			break;
		
		case FC_ST_16:
			/* 缺省是16点阵 */
			font_bytes = 32 / 2;
			pAscDot = g_Ascii16;
			fAllHz = 1;
			break;
		
//		case FC_ST_24:
//			font_bytes = 48;
//			pAscDot = g_Ascii24;
//			break;
//		
//		case FC_ST_32:
//			font_bytes = 64;
//			pAscDot = g_Ascii32;
//			break;
		
		default:
			return;
	}	
	 
	if (fAllHz == 1)	/* 内嵌全部ASCII字符点阵 */
	{
		/* 将CPU内部Flash中的ascii字符点阵复制到buf */
		memcpy(_pBuf, &pAscDot[(_code-0x20) * (font_bytes)], (font_bytes));
	}
	else	/* 内嵌部分字符，字模数组首字节是ASCII码 */
	{
		m = 0;
		while(1)
		{
		   address = m * (font_bytes + 1);
		   m++;
		   if (_code == pAscDot[address + 0])
		   {
			  address += 1;
			  memcpy(_pBuf, &pAscDot[address], font_bytes);
			  break;
		   }
		   else if ((pAscDot[address + 0] == 0xFF) && (pAscDot[address + 1] == 0xFF))
		   {
			  /* 字库搜索完毕，未找到，则填充全FF */
			  memset(_pBuf, 0xFF, font_bytes);
			  break;
		   }	   
	   }
   }
#endif
}

/*
*********************************************************************************************************
*	函 数 名: _LCD_ReadHZDot
*	功能说明: 读取1个汉字的点阵数据
*	形    参:
*		_code1, _cod2 : 汉字内码. GB2312编码
*		_fontcode ：字体代码
*		_pBuf : 存放读出的字符点阵数据
*	返 回 值: 无
*********************************************************************************************************
*/
static void _LCD_ReadHZDot(uint8_t _code1, uint8_t _code2,  uint8_t _fontcode, uint8_t *_pBuf)
{
#ifdef USE_SMALL_FONT	/* 使用CPU 内部Flash 小字库 */
	uint8_t *pDot;
	uint8_t font_bytes = 0;
	uint32_t address;
	uint16_t m;

	pDot = 0;	/* 仅仅用于避免告警 */
	switch (_fontcode)
	{
		case FC_ST_12:		/* 12点阵 */
			font_bytes = 24;
			pDot = (uint8_t *)g_Hz12;	
			break;
		
		case FC_ST_16:
			font_bytes = 32;
			pDot = (uint8_t *)g_Hz16;
			break;

		case FC_ST_24:
			font_bytes = 72;
			pDot = (uint8_t *)g_Hz24;
			break;			
			
		case FC_ST_32:	
			font_bytes = 128;
			pDot = (uint8_t *)g_Hz32;
			break;						
		
		default:
			return;
	}	

	m = 0;
	while(1)
	{
		address = m * (font_bytes + 2);
		m++;
		if ((_code1 == pDot[address + 0]) && (_code2 == pDot[address + 1]))
		{
			address += 2;
			memcpy(_pBuf, &pDot[address], font_bytes);
			break;
		}
		else if ((pDot[address + 0] == 0xFF) && (pDot[address + 1] == 0xFF))
		{
			/* 字库搜索完毕，未找到，则填充全FF */
			memset(_pBuf, 0xFF, font_bytes);
			break;
		}
	}
#endif
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DispStrEx
*	功能说明: 在LCD指定坐标（左上角）显示一个字符串。 增强型函数。支持左\中\右对齐，支持定长清屏。
*	形    参:
*		_usX : X坐标
*		_usY : Y坐标
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数。可以指定RA8875字库显示汉字
*		_Width : 字符串显示区域的宽度. 0 表示不处理留白区域，此时_Align无效
*		_Align :字符串在显示区域的对齐方式，
*				ALIGN_LEFT = 0,
*				ALIGN_CENTER = 1,
*				ALIGN_RIGHT = 2
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispStrEx(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont, uint16_t _Width,
	uint8_t _Align)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint8_t buf[32 * 32 / 8];	/* 最大支持32点阵汉字 */
	uint8_t width;
	uint16_t m;
	uint8_t font_width = 0;
	uint8_t font_height = 0;
	uint16_t x, y;
	uint16_t offset;
	uint16_t str_width;	/* 字符串实际宽度  */
	
	uint8_t line_bytes;
	uint8_t asc_bytes = 0;
	uint8_t hz_bytes = 0;

	switch (_tFont->FontCode)
	{
		case FC_ST_12:		/* 12点阵 */
			font_height = 12;
			font_width = 12;
			asc_bytes = 1;
			hz_bytes = 2;
			break;
		
		case FC_ST_16:
			font_height = 16;
			font_width = 16;
			asc_bytes = 1;
			hz_bytes = 2;
			break;

		case FC_ST_24:
			font_height = 24;
			font_width = 24;
			asc_bytes = 2;
			hz_bytes = 3;
			break;
						
		case FC_ST_32:	
			font_height = 32;
			font_width = 32;
			asc_bytes = 2;
			hz_bytes = 4;
			break;					
	}
	
	str_width = LCD_GetStrWidth(_ptr, _tFont);	/* 计算字符串实际宽度(RA8875内部ASCII点阵宽度为变长 */
	offset = 0;
	if (_Width > str_width)
	{
		if (_Align == ALIGN_RIGHT)	/* 右对齐 */
		{
			offset = _Width - str_width;
		}
		else if (_Align == ALIGN_CENTER)	/* 左对齐 */
		{
			offset = (_Width - str_width) / 2;
		}
		else	/* 左对齐 ALIGN_LEFT */
		{
			;
		}
	}

	/* 左侧填背景色, 中间对齐和右边对齐  */
	if (offset > 0)
	{
//		LCD_Fill_Rect(_usX, _usY, LCD_GetFontHeight(_tFont), offset,  _tFont->BackColor);
		_usX += offset;
	}
	
	/* 右侧填背景色 */
	if (_Width > str_width)
	{
//		LCD_Fill_Rect(_usX + str_width, _usY, LCD_GetFontHeight(_tFont), _Width - str_width - offset,  _tFont->BackColor);
	}
	
	/* 开始循环处理字符 */
	while (*_ptr != 0)
	{
		code1 = *_ptr;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
		if (code1 < 0x80)
		{
			/* 将ascii字符点阵复制到buf */
			_LCD_ReadAsciiDot(code1, _tFont->FontCode, buf);	/* 读取ASCII字符点阵 */
			width = font_width / 2;
			line_bytes = asc_bytes;
		}
		else
		{
			code2 = *++_ptr;
			if (code2 == 0)
			{
				break;
			}
			/* 读1个汉字的点阵 */
			_LCD_ReadHZDot(code1, code2, _tFont->FontCode, buf);
			width = font_width;
			line_bytes = hz_bytes;
		}

		y = _usY;
		/* 开始刷LCD */
		for (m = 0; m < font_height; m++)	/* 字符高度 */
		{
			x = _usX;
			for (i = 0; i < width; i++)	/* 字符宽度 */
			{
				if ((buf[m * line_bytes + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
				{
					LCD_PutPixel(x, y, _tFont->FrontColor);	/* 设置像素颜色为文字色 */
				}
				else
				{
					LCD_PutPixel(x, y, _tFont->BackColor);	/* 设置像素颜色为文字背景色 */
				}

				x++;
			}
			y++;
		}
		if (_tFont->Space > 0)
		{
			/* 如果文字底色按_tFont->usBackColor，并且字间距大于点阵的宽度，那么需要在文字之间填充(暂时未实现) */
		}
		_usX += width + _tFont->Space;	/* 列地址递增 */
		_ptr++;			/* 指向下一个字符 */
	}
}
/*
*********************************************************************************************************
*	函 数 名: LCD_DispStr
*	功能说明: 在LCD指定坐标（左上角）显示一个字符串
*	形    参:
*		_usX : X坐标
*		_usY : Y坐标
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
	LCD_DispStrEx(_usX, _usY, _ptr, _tFont, 0, 0);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawLabel
*	功能说明: 绘制一个文本标签
*	形    参: _pLabel : Label结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_InitLabel(LABEL_T *_pLabel, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, 
	char *_Text, FONT_T *_tFont)
{
	_pLabel->Left = _x;
	_pLabel->Top = _y;
	_pLabel->Height = _h;
	_pLabel->Width = _w;
	_pLabel->pCaption = _Text;
	_pLabel->Font = _tFont;
	
	_pLabel->MaxLen = 0;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawLabel
*	功能说明: 绘制一个文本标签
*	形    参: 结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawLabel(LABEL_T *_pLabel)
{
	char dispbuf[256];
	uint16_t i;
	uint16_t NewLen;

	NewLen = strlen(_pLabel->pCaption);

	if (NewLen > _pLabel->MaxLen)
	{
		LCD_DispStr(_pLabel->Left, _pLabel->Top, _pLabel->pCaption, _pLabel->Font);
		_pLabel->MaxLen = NewLen;
	}
	else
	{
		for (i = 0; i < NewLen; i++)
		{
			dispbuf[i] = _pLabel->pCaption[i];
		}
		for (; i < _pLabel->MaxLen; i++)
		{
			dispbuf[i] = ' ';		/* 末尾填充空格 */
		}
		dispbuf[i] = 0;
		LCD_DispStr(_pLabel->Left, _pLabel->Top, dispbuf, _pLabel->Font);
	}
}
/*
*********************************************************************************************************
*	函 数 名: LCD_DrawCheckBox
*	功能说明: 绘制一个检查框
*	形    参: 结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawCheckBox(CHECK_T *_pCheckBox)
{
	uint16_t x, y;

	/* 目前只做了16点阵汉字的大小 */

	/* 绘制外框 */
	x = _pCheckBox->Left;
	LCD_DrawRect(x, _pCheckBox->Top, CHECK_BOX_H, CHECK_BOX_W, CHECK_BOX_BORDER_COLOR);
	LCD_DrawRect(x + 1, _pCheckBox->Top + 1, CHECK_BOX_H - 2, CHECK_BOX_W - 2, CHECK_BOX_BORDER_COLOR);
//	LCD_Fill_Rect(x + 2, _pCheckBox->Top + 2, CHECK_BOX_H - 4, CHECK_BOX_W - 4, CHECK_BOX_BACK_COLOR);

	/* 绘制文本标签 */
	x = _pCheckBox->Left + CHECK_BOX_W + 2;
	y = _pCheckBox->Top + CHECK_BOX_H / 2 - 8;
	LCD_DispStr(x, y, _pCheckBox->pCaption, _pCheckBox->Font);

	if (_pCheckBox->Checked)
	{
		FONT_T font;

	    font.FontCode = FC_ST_16;
		font.BackColor = 1;
		font.FrontColor = CHECK_BOX_CHECKED_COLOR;	/* 钩的颜色 */
		font.Space = 0;
		x = _pCheckBox->Left;
		LCD_DispStr(x + 3, _pCheckBox->Top + 3, "√", &font);
	}
}
/*
*********************************************************************************************************
*	函 数 名: LCD_InitEdit
*	功能说明: 初始化编辑框参数
*	形    参: _pBox 分组框
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_InitEdit(EDIT_T *_pEdit, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w,
	char  *pCaption, FONT_T *Font)
{
	_pEdit->Left = _x;
	_pEdit->Top = _y;
	_pEdit->Height = _h;
	_pEdit->Width = _w;
	_pEdit->pCaption = pCaption;
	_pEdit->Font = Font;	
	_pEdit->Focus = 0;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawEdit
*	功能说明: 在LCD上绘制一个编辑框
*	形    参: _pEdit 编辑框结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawEdit(EDIT_T *_pEdit)
{
	uint16_t len, x, y;
	
	if (_pEdit->pCaption > 0)
	{
		for (len = 0; len < 32; len++)
		{
			_pEdit->Text[len] = _pEdit->pCaption[len];
			
			if (_pEdit->pCaption[len] == 0)
			{
				break;
			}
		}
		_pEdit->Text[32] = 0;
		
		_pEdit->pCaption = 0;
	}
	
	/* 文字居中 */
	len = LCD_GetStrWidth(_pEdit->Text,  _pEdit->Font);
	x = _pEdit->Left +  (_pEdit->Width - len) / 2;
	y = _pEdit->Top + (_pEdit->Height - LCD_GetFontHeight(_pEdit->Font)) / 2 + 1;

	LCD_DispStr(x, y, _pEdit->Text, _pEdit->Font);
	/* 仿XP风格，平面编辑框 */
	if (_pEdit->Focus == 0)
	{
		LCD_DrawRect(_pEdit->Left, _pEdit->Top, _pEdit->Height, _pEdit->Width, 1);
	}
	else
	{
//		LCD_Fill_Rect(_pEdit->Left+1, _pEdit->Top+1, _pEdit->Height-2, _pEdit->Width-2, 0);
		LCD_DrawRect(_pEdit->Left, _pEdit->Top, _pEdit->Height, _pEdit->Width, 2);
	}	
}
/*
*********************************************************************************************************
*	函 数 名: LCD_EditTouchRelease
*	功能说明: 编辑框退出编辑状态，重绘
*	形    参:  _Edit : 编辑框对象
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_EditRefresh(EDIT_T *_Edit)
{
	_Edit->Focus = 0;
	LCD_DrawEdit(_Edit);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawWin
*	功能说明: 在LCD上绘制一个窗口
*	形    参: 结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawWin(WIN_T *_pWin)
{
//	uint16_t TitleHegiht;

//	TitleHegiht = 20;

	/* 绘制窗口外框 */
	LCD_DrawRect(_pWin->Left, _pWin->Top, _pWin->Height, _pWin->Width, WIN_BORDER_COLOR);
	LCD_DrawRect(_pWin->Left + 1, _pWin->Top + 1, _pWin->Height - 2, _pWin->Width - 2, WIN_BORDER_COLOR);

	/* 窗口标题栏 */
//	LCD_Fill_Rect(_pWin->Left + 2, _pWin->Top + 2, TitleHegiht, _pWin->Width - 4, WIN_TITLE_COLOR);

	/* 窗体填充 */
//	LCD_Fill_Rect(_pWin->Left + 2, _pWin->Top + TitleHegiht + 2, _pWin->Height - 4 - TitleHegiht,
//		_pWin->Width - 4, WIN_BODY_COLOR);

	LCD_DispStr(_pWin->Left + 3, _pWin->Top + 2, _pWin->pCaption, _pWin->Font);
}
/*
*********************************************************************************************************
*	函 数 名: LCD_InitGroupBox
*	功能说明: 初始化分组框参数
*	形    参: _pBox 分组框
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_InitGroupBox(GROUP_T *_pBox, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w,
	char  *pCaption, FONT_T *Font)
{
	_pBox->Left = _x;
	_pBox->Top = _y;
	_pBox->Height = _h;
	_pBox->Width = _w;
	_pBox->pCaption = pCaption;
	_pBox->Font = Font;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawGroupBox
*	功能说明: 在LCD上绘制一个分组框
*	形    参: _pBox 分组框
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawGroupBox(GROUP_T *_pBox)
{
	uint16_t x, y;
	uint16_t x1,y1;		/* 矩形左上角 */
	uint16_t x2, y2;	/* 矩形右下角 */
	uint16_t len;

	
	len = LCD_GetStrWidth(_pBox->pCaption, _pBox->Font);	/* 字符串的总宽度 */
	
	/* 画阴影线 */
	//LCD_DrawRect(_pBox->Left + 1, _pBox->Top + 5, _pBox->Height, _pBox->Width - 1, CL_BOX_BORDER2);
	x1 = _pBox->Left + 1;
	y1 = _pBox->Top + 5;
	x2 = _pBox->Left + 1 + _pBox->Width - 2;
	y2 = _pBox->Top + 5 + _pBox->Height - 1;
	
	LCD_DrawLine(x1, y1, x1 + 6, y1, 0);	/* 顶1 */
	LCD_DrawLine(x1 + 8 + len + 1, y1, x2, y1, 0);	/* 顶2 */	
	LCD_DrawLine(x1, y2, x2, y2, 0);	/* 底 */
	LCD_DrawLine(x1, y1, x1, y2, 0);	/* 左 */
	LCD_DrawLine(x2, y1, x2, y2, 0);	/* 右 */	

	/* 画主框线 */
	//LCD_DrawRect(_pBox->Left, _pBox->Top + 4, _pBox->Height, _pBox->Width - 1, CL_BOX_BORDER1);
	x1 = _pBox->Left;
	y1 = _pBox->Top + 4;
	x2 = _pBox->Left + _pBox->Width - 2;
	y2 = _pBox->Top + 4 + _pBox->Height - 1;
	
	LCD_DrawLine(x1, y1, x1 + 6, y1, 0);	/* 顶1 */
	LCD_DrawLine(x1 + 9 + len + 1, y1, x2, y1, 0);	/* 顶2 */	
	LCD_DrawLine(x1, y2, x2, y2, 0);	/* 底 */
	LCD_DrawLine(x1, y1, x1, y2, 0);	/* 左 */
	LCD_DrawLine(x2, y1, x2, y2, 0);	/* 右 */		

	/* 显示分组框标题（文字在左上角） */
	x = _pBox->Left + 9;
	y = _pBox->Top;
	LCD_DispStr(x, y, _pBox->pCaption, _pBox->Font);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_InitButton
*	功能说明: 初始化按钮结构体成员。
*	形    参:  _x, _y : 坐标
*			  _h, _w : 高度和宽度
*			  _pCaption : 按钮文字
*			  _pFont : 按钮字体
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_InitButton(BUTTON_T *_btn, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, char *_pCaption, FONT_T *_pFont)
{
	_btn->Left = _x;
	_btn->Top = _y;
	_btn->Height = _h;
	_btn->Width = _w;
	_btn->pCaption = _pCaption;	
	_btn->Font = _pFont;
	_btn->Focus = 0;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawButton2
*	功能说明: 在LCD上绘制一个按钮，类似emwin按钮
*	形    参:
*			_usX, _usY : 图片的坐标
*			_usHeight  : 图片高度
*			_usWidth   : 图片宽度
*			_ptr       : 图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawButton(BUTTON_T *_pBtn)
{
	uint16_t x, y;
	uint16_t str_width;
//	uint8_t Arc = 5;
		
	if (_pBtn->Focus == 0)
	{
//		SOFT_FillRoundRect(_pBtn->Left, _pBtn->Top, _pBtn->Height, _pBtn->Width, Arc,  BTN_BODY_COLOR1);			
//		SOFT_DrawRoundRect(_pBtn->Left, _pBtn->Top, _pBtn->Height, _pBtn->Width, Arc,  BTN_BORDER_COLOR1);											
//		LCD_Fill_Rect(_pBtn->Left + Arc, _pBtn->Top + 1, _pBtn->Height / 2, _pBtn->Width - 2 * Arc, BTN_SHADOW_COLOR1);	/* 画阴影对比色 */
	}
	else
	{
//		SOFT_FillRoundRect(_pBtn->Left, _pBtn->Top, _pBtn->Height, _pBtn->Width, Arc,  BTN_BODY_COLOR2);			
//		SOFT_DrawRoundRect(_pBtn->Left, _pBtn->Top, _pBtn->Height, _pBtn->Width, Arc,  BTN_BORDER_COLOR2);	
//		LCD_Fill_Rect(_pBtn->Left + Arc, _pBtn->Top + 1, _pBtn->Height / 2, _pBtn->Width - 2 * Arc, BTN_SHADOW_COLOR2);	/* 画阴影对比色 */
	}

	/* 计算字符串实际宽度(RA8875内部ASCII点阵宽度为变长 */
	str_width = LCD_GetStrWidth(_pBtn->pCaption, _pBtn->Font);	
	
	if (_pBtn->Width >= str_width)
	{
		x = _pBtn->Left + (_pBtn->Width - str_width) / 2;		/* 居中 */
	}	
	else
	{
		x = _pBtn->Left;
	}
	y = _pBtn->Top + (_pBtn->Height - LCD_GetFontHeight(_pBtn->Font)) / 2;
	
	LCD_DispStr(x, y, _pBtn->pCaption, _pBtn->Font);
}
/*
*********************************************************************************************************
*	函 数 名: LCD_DispControl
*	功能说明: 绘制控件
*	形    参: _pControl 控件指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispControl(void *_pControl)
{
	uint8_t id;

	id = *(uint8_t *)_pControl;	/* 读取ID */

	switch (id)
	{
		case ID_ICON:
			//void LCD_DrawIcon(const ICON_T *_tIcon, FONT_T *_tFont, uint8_t _ucFocusMode);
			break;

		case ID_WIN:
			LCD_DrawWin((WIN_T *)_pControl);
			break;

		case ID_LABEL:
			LCD_DrawLabel((LABEL_T *)_pControl);
			break;

		case ID_BUTTON:
			LCD_DrawButton((BUTTON_T *)_pControl);
			break;

		case ID_CHECK:
			LCD_DrawCheckBox((CHECK_T *)_pControl);
			break;

		case ID_EDIT:
			LCD_DrawEdit((EDIT_T *)_pControl);
			break;

		case ID_GROUP:
			LCD_DrawGroupBox((GROUP_T *)_pControl);
			break;
	}
}

