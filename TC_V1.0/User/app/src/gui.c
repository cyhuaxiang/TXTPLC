#include "bsp.h"
#include "gui.h"
#include "fonts.h"

/*
*********************************************************************************************************
*	�� �� ��: LCD_GetFontWidth
*	����˵��: ��ȡ����Ŀ�ȣ����ص�λ)
*	��    ��:
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ����Ŀ�ȣ����ص�λ)
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
*	�� �� ��: LCD_GetFontHeight
*	����˵��: ��ȡ����ĸ߶ȣ����ص�λ)
*	��    ��:
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ����Ŀ�ȣ����ص�λ)
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
*	�� �� ��: LCD_GetStrWidth
*	����˵��: �����ַ������(���ص�λ)
*	��    ��:
*		_ptr  : �ַ���ָ��
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ��
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
		code1 = *p;	/* ��ȡ�ַ������ݣ� �����ݿ�����ascii���룬Ҳ���ܺ��ִ���ĸ��ֽ� */
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
		else	/* ���� */
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
*	�� �� ��: _LCD_ReadAsciiDot
*	����˵��: ��ȡ1��ASCII�ַ��ĵ�������
*	��    ��:
*		_code : ASCII�ַ��ı��룬1�ֽڡ�1-128
*		_fontcode ���������
*		_pBuf : ��Ŷ������ַ���������
*	�� �� ֵ: ���ֿ��
*********************************************************************************************************
*/
static void _LCD_ReadAsciiDot(uint8_t _code, uint8_t _fontcode, uint8_t *_pBuf)
{
#ifdef USE_SMALL_FONT	/* ʹ��CPU �ڲ�Flash С�ֿ� */
	const uint8_t *pAscDot;
	uint8_t font_bytes = 0;
	uint16_t m;
	uint16_t address;
	uint8_t fAllHz = 1;	/* 1��ʾ��������Ƕȫ����ASCII�ַ��� */

	pAscDot = 0;
	switch (_fontcode)
	{
		case FC_ST_12:		/* 12���� */
			font_bytes = 24 / 2;
			pAscDot = g_Ascii12;	
			fAllHz = 1;
			break;
		
		case FC_ST_16:
			/* ȱʡ��16���� */
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
	 
	if (fAllHz == 1)	/* ��Ƕȫ��ASCII�ַ����� */
	{
		/* ��CPU�ڲ�Flash�е�ascii�ַ������Ƶ�buf */
		memcpy(_pBuf, &pAscDot[(_code-0x20) * (font_bytes)], (font_bytes));
	}
	else	/* ��Ƕ�����ַ�����ģ�������ֽ���ASCII�� */
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
			  /* �ֿ�������ϣ�δ�ҵ��������ȫFF */
			  memset(_pBuf, 0xFF, font_bytes);
			  break;
		   }	   
	   }
   }
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: _LCD_ReadHZDot
*	����˵��: ��ȡ1�����ֵĵ�������
*	��    ��:
*		_code1, _cod2 : ��������. GB2312����
*		_fontcode ���������
*		_pBuf : ��Ŷ������ַ���������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _LCD_ReadHZDot(uint8_t _code1, uint8_t _code2,  uint8_t _fontcode, uint8_t *_pBuf)
{
#ifdef USE_SMALL_FONT	/* ʹ��CPU �ڲ�Flash С�ֿ� */
	uint8_t *pDot;
	uint8_t font_bytes = 0;
	uint32_t address;
	uint16_t m;

	pDot = 0;	/* �������ڱ���澯 */
	switch (_fontcode)
	{
		case FC_ST_12:		/* 12���� */
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
			/* �ֿ�������ϣ�δ�ҵ��������ȫFF */
			memset(_pBuf, 0xFF, font_bytes);
			break;
		}
	}
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DispStrEx
*	����˵��: ��LCDָ�����꣨���Ͻǣ���ʾһ���ַ����� ��ǿ�ͺ�����֧����\��\�Ҷ��룬֧�ֶ���������
*	��    ��:
*		_usX : X����
*		_usY : Y����
*		_ptr  : �ַ���ָ��
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���������ָ��RA8875�ֿ���ʾ����
*		_Width : �ַ�����ʾ����Ŀ��. 0 ��ʾ�������������򣬴�ʱ_Align��Ч
*		_Align :�ַ�������ʾ����Ķ��뷽ʽ��
*				ALIGN_LEFT = 0,
*				ALIGN_CENTER = 1,
*				ALIGN_RIGHT = 2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DispStrEx(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont, uint16_t _Width,
	uint8_t _Align)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint8_t buf[32 * 32 / 8];	/* ���֧��32������ */
	uint8_t width;
	uint16_t m;
	uint8_t font_width = 0;
	uint8_t font_height = 0;
	uint16_t x, y;
	uint16_t offset;
	uint16_t str_width;	/* �ַ���ʵ�ʿ��  */
	
	uint8_t line_bytes;
	uint8_t asc_bytes = 0;
	uint8_t hz_bytes = 0;

	switch (_tFont->FontCode)
	{
		case FC_ST_12:		/* 12���� */
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
	
	str_width = LCD_GetStrWidth(_ptr, _tFont);	/* �����ַ���ʵ�ʿ��(RA8875�ڲ�ASCII������Ϊ�䳤 */
	offset = 0;
	if (_Width > str_width)
	{
		if (_Align == ALIGN_RIGHT)	/* �Ҷ��� */
		{
			offset = _Width - str_width;
		}
		else if (_Align == ALIGN_CENTER)	/* ����� */
		{
			offset = (_Width - str_width) / 2;
		}
		else	/* ����� ALIGN_LEFT */
		{
			;
		}
	}

	/* ������ɫ, �м������ұ߶���  */
	if (offset > 0)
	{
//		LCD_Fill_Rect(_usX, _usY, LCD_GetFontHeight(_tFont), offset,  _tFont->BackColor);
		_usX += offset;
	}
	
	/* �Ҳ����ɫ */
	if (_Width > str_width)
	{
//		LCD_Fill_Rect(_usX + str_width, _usY, LCD_GetFontHeight(_tFont), _Width - str_width - offset,  _tFont->BackColor);
	}
	
	/* ��ʼѭ�������ַ� */
	while (*_ptr != 0)
	{
		code1 = *_ptr;	/* ��ȡ�ַ������ݣ� �����ݿ�����ascii���룬Ҳ���ܺ��ִ���ĸ��ֽ� */
		if (code1 < 0x80)
		{
			/* ��ascii�ַ������Ƶ�buf */
			_LCD_ReadAsciiDot(code1, _tFont->FontCode, buf);	/* ��ȡASCII�ַ����� */
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
			/* ��1�����ֵĵ��� */
			_LCD_ReadHZDot(code1, code2, _tFont->FontCode, buf);
			width = font_width;
			line_bytes = hz_bytes;
		}

		y = _usY;
		/* ��ʼˢLCD */
		for (m = 0; m < font_height; m++)	/* �ַ��߶� */
		{
			x = _usX;
			for (i = 0; i < width; i++)	/* �ַ���� */
			{
				if ((buf[m * line_bytes + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
				{
					LCD_PutPixel(x, y, _tFont->FrontColor);	/* ����������ɫΪ����ɫ */
				}
				else
				{
					LCD_PutPixel(x, y, _tFont->BackColor);	/* ����������ɫΪ���ֱ���ɫ */
				}

				x++;
			}
			y++;
		}
		if (_tFont->Space > 0)
		{
			/* ������ֵ�ɫ��_tFont->usBackColor�������ּ����ڵ���Ŀ�ȣ���ô��Ҫ������֮�����(��ʱδʵ��) */
		}
		_usX += width + _tFont->Space;	/* �е�ַ���� */
		_ptr++;			/* ָ����һ���ַ� */
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_DispStr
*	����˵��: ��LCDָ�����꣨���Ͻǣ���ʾһ���ַ���
*	��    ��:
*		_usX : X����
*		_usY : Y����
*		_ptr  : �ַ���ָ��
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
	LCD_DispStrEx(_usX, _usY, _ptr, _tFont, 0, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawLabel
*	����˵��: ����һ���ı���ǩ
*	��    ��: _pLabel : Label�ṹ��ָ��
*	�� �� ֵ: ��
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
*	�� �� ��: LCD_DrawLabel
*	����˵��: ����һ���ı���ǩ
*	��    ��: �ṹ��ָ��
*	�� �� ֵ: ��
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
			dispbuf[i] = ' ';		/* ĩβ���ո� */
		}
		dispbuf[i] = 0;
		LCD_DispStr(_pLabel->Left, _pLabel->Top, dispbuf, _pLabel->Font);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawCheckBox
*	����˵��: ����һ������
*	��    ��: �ṹ��ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawCheckBox(CHECK_T *_pCheckBox)
{
	uint16_t x, y;

	/* Ŀǰֻ����16�����ֵĴ�С */

	/* ������� */
	x = _pCheckBox->Left;
	LCD_DrawRect(x, _pCheckBox->Top, CHECK_BOX_H, CHECK_BOX_W, CHECK_BOX_BORDER_COLOR);
	LCD_DrawRect(x + 1, _pCheckBox->Top + 1, CHECK_BOX_H - 2, CHECK_BOX_W - 2, CHECK_BOX_BORDER_COLOR);
//	LCD_Fill_Rect(x + 2, _pCheckBox->Top + 2, CHECK_BOX_H - 4, CHECK_BOX_W - 4, CHECK_BOX_BACK_COLOR);

	/* �����ı���ǩ */
	x = _pCheckBox->Left + CHECK_BOX_W + 2;
	y = _pCheckBox->Top + CHECK_BOX_H / 2 - 8;
	LCD_DispStr(x, y, _pCheckBox->pCaption, _pCheckBox->Font);

	if (_pCheckBox->Checked)
	{
		FONT_T font;

	    font.FontCode = FC_ST_16;
		font.BackColor = 1;
		font.FrontColor = CHECK_BOX_CHECKED_COLOR;	/* ������ɫ */
		font.Space = 0;
		x = _pCheckBox->Left;
		LCD_DispStr(x + 3, _pCheckBox->Top + 3, "��", &font);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_InitEdit
*	����˵��: ��ʼ���༭�����
*	��    ��: _pBox �����
*	�� �� ֵ: ��
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
*	�� �� ��: LCD_DrawEdit
*	����˵��: ��LCD�ϻ���һ���༭��
*	��    ��: _pEdit �༭��ṹ��ָ��
*	�� �� ֵ: ��
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
	
	/* ���־��� */
	len = LCD_GetStrWidth(_pEdit->Text,  _pEdit->Font);
	x = _pEdit->Left +  (_pEdit->Width - len) / 2;
	y = _pEdit->Top + (_pEdit->Height - LCD_GetFontHeight(_pEdit->Font)) / 2 + 1;

	LCD_DispStr(x, y, _pEdit->Text, _pEdit->Font);
	/* ��XP���ƽ��༭�� */
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
*	�� �� ��: LCD_EditTouchRelease
*	����˵��: �༭���˳��༭״̬���ػ�
*	��    ��:  _Edit : �༭�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_EditRefresh(EDIT_T *_Edit)
{
	_Edit->Focus = 0;
	LCD_DrawEdit(_Edit);
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_DrawWin
*	����˵��: ��LCD�ϻ���һ������
*	��    ��: �ṹ��ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawWin(WIN_T *_pWin)
{
//	uint16_t TitleHegiht;

//	TitleHegiht = 20;

	/* ���ƴ������ */
	LCD_DrawRect(_pWin->Left, _pWin->Top, _pWin->Height, _pWin->Width, WIN_BORDER_COLOR);
	LCD_DrawRect(_pWin->Left + 1, _pWin->Top + 1, _pWin->Height - 2, _pWin->Width - 2, WIN_BORDER_COLOR);

	/* ���ڱ����� */
//	LCD_Fill_Rect(_pWin->Left + 2, _pWin->Top + 2, TitleHegiht, _pWin->Width - 4, WIN_TITLE_COLOR);

	/* ������� */
//	LCD_Fill_Rect(_pWin->Left + 2, _pWin->Top + TitleHegiht + 2, _pWin->Height - 4 - TitleHegiht,
//		_pWin->Width - 4, WIN_BODY_COLOR);

	LCD_DispStr(_pWin->Left + 3, _pWin->Top + 2, _pWin->pCaption, _pWin->Font);
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_InitGroupBox
*	����˵��: ��ʼ����������
*	��    ��: _pBox �����
*	�� �� ֵ: ��
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
*	�� �� ��: LCD_DrawGroupBox
*	����˵��: ��LCD�ϻ���һ�������
*	��    ��: _pBox �����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawGroupBox(GROUP_T *_pBox)
{
	uint16_t x, y;
	uint16_t x1,y1;		/* �������Ͻ� */
	uint16_t x2, y2;	/* �������½� */
	uint16_t len;

	
	len = LCD_GetStrWidth(_pBox->pCaption, _pBox->Font);	/* �ַ������ܿ�� */
	
	/* ����Ӱ�� */
	//LCD_DrawRect(_pBox->Left + 1, _pBox->Top + 5, _pBox->Height, _pBox->Width - 1, CL_BOX_BORDER2);
	x1 = _pBox->Left + 1;
	y1 = _pBox->Top + 5;
	x2 = _pBox->Left + 1 + _pBox->Width - 2;
	y2 = _pBox->Top + 5 + _pBox->Height - 1;
	
	LCD_DrawLine(x1, y1, x1 + 6, y1, 0);	/* ��1 */
	LCD_DrawLine(x1 + 8 + len + 1, y1, x2, y1, 0);	/* ��2 */	
	LCD_DrawLine(x1, y2, x2, y2, 0);	/* �� */
	LCD_DrawLine(x1, y1, x1, y2, 0);	/* �� */
	LCD_DrawLine(x2, y1, x2, y2, 0);	/* �� */	

	/* �������� */
	//LCD_DrawRect(_pBox->Left, _pBox->Top + 4, _pBox->Height, _pBox->Width - 1, CL_BOX_BORDER1);
	x1 = _pBox->Left;
	y1 = _pBox->Top + 4;
	x2 = _pBox->Left + _pBox->Width - 2;
	y2 = _pBox->Top + 4 + _pBox->Height - 1;
	
	LCD_DrawLine(x1, y1, x1 + 6, y1, 0);	/* ��1 */
	LCD_DrawLine(x1 + 9 + len + 1, y1, x2, y1, 0);	/* ��2 */	
	LCD_DrawLine(x1, y2, x2, y2, 0);	/* �� */
	LCD_DrawLine(x1, y1, x1, y2, 0);	/* �� */
	LCD_DrawLine(x2, y1, x2, y2, 0);	/* �� */		

	/* ��ʾ�������⣨���������Ͻǣ� */
	x = _pBox->Left + 9;
	y = _pBox->Top;
	LCD_DispStr(x, y, _pBox->pCaption, _pBox->Font);
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_InitButton
*	����˵��: ��ʼ����ť�ṹ���Ա��
*	��    ��:  _x, _y : ����
*			  _h, _w : �߶ȺͿ��
*			  _pCaption : ��ť����
*			  _pFont : ��ť����
*	�� �� ֵ: ��
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
*	�� �� ��: LCD_DrawButton2
*	����˵��: ��LCD�ϻ���һ����ť������emwin��ť
*	��    ��:
*			_usX, _usY : ͼƬ������
*			_usHeight  : ͼƬ�߶�
*			_usWidth   : ͼƬ���
*			_ptr       : ͼƬ����ָ��
*	�� �� ֵ: ��
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
//		LCD_Fill_Rect(_pBtn->Left + Arc, _pBtn->Top + 1, _pBtn->Height / 2, _pBtn->Width - 2 * Arc, BTN_SHADOW_COLOR1);	/* ����Ӱ�Ա�ɫ */
	}
	else
	{
//		SOFT_FillRoundRect(_pBtn->Left, _pBtn->Top, _pBtn->Height, _pBtn->Width, Arc,  BTN_BODY_COLOR2);			
//		SOFT_DrawRoundRect(_pBtn->Left, _pBtn->Top, _pBtn->Height, _pBtn->Width, Arc,  BTN_BORDER_COLOR2);	
//		LCD_Fill_Rect(_pBtn->Left + Arc, _pBtn->Top + 1, _pBtn->Height / 2, _pBtn->Width - 2 * Arc, BTN_SHADOW_COLOR2);	/* ����Ӱ�Ա�ɫ */
	}

	/* �����ַ���ʵ�ʿ��(RA8875�ڲ�ASCII������Ϊ�䳤 */
	str_width = LCD_GetStrWidth(_pBtn->pCaption, _pBtn->Font);	
	
	if (_pBtn->Width >= str_width)
	{
		x = _pBtn->Left + (_pBtn->Width - str_width) / 2;		/* ���� */
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
*	�� �� ��: LCD_DispControl
*	����˵��: ���ƿؼ�
*	��    ��: _pControl �ؼ�ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DispControl(void *_pControl)
{
	uint8_t id;

	id = *(uint8_t *)_pControl;	/* ��ȡID */

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

