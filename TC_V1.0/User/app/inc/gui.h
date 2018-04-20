#ifndef __GUI_H
#define __GUI_H


/* ���ֶ��뷽ʽ */
enum
{
	ALIGN_LEFT = 0,
	ALIGN_CENTER = 1,
	ALIGN_RIGHT = 2
};

/* �༭���� */
enum
{
	EDIT_BORDER_COLOR		= 0,		/* �༭���ĸ��ߵ���ɫ��δѡ��ʱ */
	EDIT_BORDER_COLOR2		= 0,		/* �༭���ĸ��ߵ���ɫ,ѡ��ʱ */
	EDIT_BACK_COLOR			= 0,		/* �༭�򱳾���δѡ��ʱ */
	EDIT_BACK_COLOR2		= 0,		/* �༭�򱳾���ɫ��ѡ��ʱ */
};

/* ��ť��� */
enum
{
	/* δ����ʱ */
	BTN_BORDER_COLOR1	= 0,	/* ��ť�߿����ɫ */
	BTN_BODY_COLOR1		= 0,	/* ��ť��ɫ */
	BTN_SHADOW_COLOR1	= 0,	/* ��ť�ϰ벿��Ӱɫ */

	/* ����ʱ */
	BTN_BORDER_COLOR2	= 0,	/* ����ʱ��ť�߿����ɫ */
	BTN_BODY_COLOR2		= 0,	/* ����ʱ��ť��ɫ */
	BTN_SHADOW_COLOR2	= 0,	/* ����ʱ��ť�ϰ벿��Ӱɫ */	
};

/* ���ڷ�� */
enum
{
	WIN_BORDER_COLOR	= 0,		/* ���ڱ߿� */
	WIN_TITLE_COLOR		= 0,	/* ���ڱ�����������ɫ */  
	WIN_CAPTION_COLOR	= 0,		/* ���ڱ�����������ɫ */
	WIN_BODY_COLOR		= 0,		/* ǳ��  ������ɫ */
};

/* ������ */
enum
{
	CHECK_BOX_BORDER_COLOR	= 0,		/* �����ĸ��ߵ���ɫ */
	CHECK_BOX_BACK_COLOR	= 0,		/* ���򱳾� */
	CHECK_BOX_CHECKED_COLOR	= 0,		/* ����򹴵���ɫ */

	CHECK_BOX_H			= 24,				/* ����߶� */
	CHECK_BOX_W			= 24,				/* ����߶� */
};

/* ������� */
typedef enum
{
	FC_ST_12 = 0,		/* ����12x12���� ����x�ߣ� */
	FC_ST_16,			/* ����15x16���� ����x�ߣ� */
	FC_ST_24,			/* ����24x24���� ����x�ߣ� -- ��ʱδ֧�� */
	FC_ST_32,			/* ����32x32���� ����x�ߣ� -- ��ʱδ֧�� */	
	
	FC_RA8875_16,		/* RA8875 �������� 16���� */
	FC_RA8875_24,		/* RA8875 �������� 24���� */
	FC_RA8875_32		/* RA8875 �������� 32���� */	
}FONT_CODE_E;

/* �������Խṹ, ����LCD_DispStr() */
typedef struct
{
	FONT_CODE_E FontCode;	/* ������� FONT_CODE_E  */
	uint16_t FrontColor;/* ������ɫ */
	uint16_t BackColor;	/* ���ֱ�����ɫ��͸�� */
	uint16_t Space;		/* ���ּ�࣬��λ = ���� */
}FONT_T;

/* �ؼ�ID */
typedef enum
{
	ID_ICON		= 1,
	ID_WIN		= 2,
	ID_LABEL	= 3,
	ID_BUTTON	= 4,
	ID_CHECK 	= 5,
	ID_EDIT 	= 6,
	ID_GROUP 	= 7,
}CONTROL_ID_T;

/* ͼ��ṹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;		/* ���Ͻ�X���� */
	uint16_t Top;		/* ���Ͻ�Y���� */
	uint16_t Height;	/* ͼ��߶� */
	uint16_t Width;		/* ͼ���� */
	uint16_t *pBmp;		/* ָ��ͼ��ͼƬ���� */
	char  Text[16];	/* ͼ���ı�, �����ʾ5������16���� */
}ICON_T;

/* ����ṹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint16_t Color;
	FONT_T *Font;
	char *pCaption;
}WIN_T;

/* �ı���ǩ�ṹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* ���Ͻ�X���� */
	uint16_t Top;			/* ���Ͻ�Y���� */
	uint16_t Height;		/* �߶� */
	uint16_t Width;			/* ��� */
	uint16_t MaxLen;		/* �ַ������� */
	FONT_T *Font;			/* ���� */
	char  *pCaption;
}LABEL_T;

/* ��ť�ṹ */
typedef struct
{
	uint8_t id;
	uint8_t Style;		/* ��ť���0=��ͨ��ť�� 1=Բ�ǰ�ť�� 2=ͼƬ��ť */
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	/* ��ť����ɫ���ɵײ��Զ����� */
	FONT_T *Font;			/* ���� */
	char *pCaption;
	uint8_t Focus;			/* ���� */
}BUTTON_T;

/* ��ť��������ı������ṹ */
typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t h;
	uint16_t w;
	char *text;		/* ��ť������ */
}BTN_PARAM_T;

/* ͼƬ��ť�ṹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint32_t Pic1;
	uint32_t Pic2;	
	uint8_t Focus;			/* ���� */
}BMP_BUTTON_T;

/* ѡ�нṹ */
typedef struct
{
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	
}SELECT_T;

/* �༭��ṹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint16_t Color;
	FONT_T *Font;			/* ���� */
	char   *pCaption;
	uint8_t Focus;
	char Text[32+1];		/* ����༭���ڵ�ֵ */
}EDIT_T;

/* ���� CHECK BOX �ṹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* ���Ͻ�X���� */
	uint16_t Top;			/* ���Ͻ�Y���� */
	uint16_t Height;		/* �߶� */
	uint16_t Width;			/* ��� */
	uint16_t Color;			/* ��ɫ */
	FONT_T *Font;			/* ���� */
	char  *pCaption;
	uint8_t Checked;		/* 1��ʾ�� */
}CHECK_T;

/* �����GROUP BOX �ṹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* ���Ͻ�X���� */
	uint16_t Top;			/* ���Ͻ�Y���� */
	uint16_t Height;		/* �߶� */
	uint16_t Width;			/* ��� */
	FONT_T *Font;			/* ���� */
	char  *pCaption;
}GROUP_T;

/* �ɹ��ⲿģ����õĺ��� */
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);
void LCD_DrawWin(WIN_T *_pWin);

void LCD_DrawEdit(EDIT_T *_pEdit);

void LCD_DrawButton(BUTTON_T *_pBtn);
void LCD_DrawLabel(LABEL_T *_pLabel);
void LCD_DrawCheckBox(CHECK_T *_pCheckBox);

void LCD_InitGroupBox(GROUP_T *_pBox, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w,
	char  *pCaption, FONT_T *Font);
void LCD_DrawGroupBox(GROUP_T *_pBox);

void LCD_DispControl(void *_pControl);

uint16_t LCD_GetFontWidth(FONT_T *_tFont);
uint16_t LCD_GetFontHeight(FONT_T *_tFont);
uint16_t LCD_GetStrWidth(char *_ptr, FONT_T *_tFont);

void LCD_InitButton(BUTTON_T *_btn, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, char *_pCaption, FONT_T *_pFont);

void LCD_InitLabel(LABEL_T *_pLabel, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, 
	char *_Text, FONT_T *_tFont);
	
void LCD_InitEdit(EDIT_T *_pEdit, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w,
	char  *pCaption, FONT_T *Font);	
void LCD_EditRefresh(EDIT_T *_Edit);

#endif

