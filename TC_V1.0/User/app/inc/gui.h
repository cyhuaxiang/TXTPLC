#ifndef __GUI_H
#define __GUI_H


/* 文字对齐方式 */
enum
{
	ALIGN_LEFT = 0,
	ALIGN_CENTER = 1,
	ALIGN_RIGHT = 2
};

/* 编辑框风格 */
enum
{
	EDIT_BORDER_COLOR		= 0,		/* 编辑框四个边的颜色，未选中时 */
	EDIT_BORDER_COLOR2		= 0,		/* 编辑框四个边的颜色,选中时 */
	EDIT_BACK_COLOR			= 0,		/* 编辑框背景，未选中时 */
	EDIT_BACK_COLOR2		= 0,		/* 编辑框背景颜色，选中时 */
};

/* 按钮风格 */
enum
{
	/* 未按下时 */
	BTN_BORDER_COLOR1	= 0,	/* 按钮边框的颜色 */
	BTN_BODY_COLOR1		= 0,	/* 按钮底色 */
	BTN_SHADOW_COLOR1	= 0,	/* 按钮上半部阴影色 */

	/* 按下时 */
	BTN_BORDER_COLOR2	= 0,	/* 按下时按钮边框的颜色 */
	BTN_BODY_COLOR2		= 0,	/* 按下时按钮底色 */
	BTN_SHADOW_COLOR2	= 0,	/* 按下时按钮上半部阴影色 */	
};

/* 窗口风格 */
enum
{
	WIN_BORDER_COLOR	= 0,		/* 窗口边框 */
	WIN_TITLE_COLOR		= 0,	/* 窗口标题蓝背景颜色 */  
	WIN_CAPTION_COLOR	= 0,		/* 窗口标题栏文字颜色 */
	WIN_BODY_COLOR		= 0,		/* 浅蓝  窗体颜色 */
};

/* 检查框风格 */
enum
{
	CHECK_BOX_BORDER_COLOR	= 0,		/* 检查框四个边的颜色 */
	CHECK_BOX_BACK_COLOR	= 0,		/* 检查框背景 */
	CHECK_BOX_CHECKED_COLOR	= 0,		/* 检查框打勾的颜色 */

	CHECK_BOX_H			= 24,				/* 检查框高度 */
	CHECK_BOX_W			= 24,				/* 检查框高度 */
};

/* 字体代码 */
typedef enum
{
	FC_ST_12 = 0,		/* 宋体12x12点阵 （宽x高） */
	FC_ST_16,			/* 宋体15x16点阵 （宽x高） */
	FC_ST_24,			/* 宋体24x24点阵 （宽x高） -- 暂时未支持 */
	FC_ST_32,			/* 宋体32x32点阵 （宽x高） -- 暂时未支持 */	
	
	FC_RA8875_16,		/* RA8875 内置字体 16点阵 */
	FC_RA8875_24,		/* RA8875 内置字体 24点阵 */
	FC_RA8875_32		/* RA8875 内置字体 32点阵 */	
}FONT_CODE_E;

/* 字体属性结构, 用于LCD_DispStr() */
typedef struct
{
	FONT_CODE_E FontCode;	/* 字体代码 FONT_CODE_E  */
	uint16_t FrontColor;/* 字体颜色 */
	uint16_t BackColor;	/* 文字背景颜色，透明 */
	uint16_t Space;		/* 文字间距，单位 = 像素 */
}FONT_T;

/* 控件ID */
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

/* 图标结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;		/* 左上角X坐标 */
	uint16_t Top;		/* 左上角Y坐标 */
	uint16_t Height;	/* 图标高度 */
	uint16_t Width;		/* 图标宽度 */
	uint16_t *pBmp;		/* 指向图标图片数据 */
	char  Text[16];	/* 图标文本, 最多显示5个汉字16点阵 */
}ICON_T;

/* 窗体结构 */
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

/* 文本标签结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* 左上角X坐标 */
	uint16_t Top;			/* 左上角Y坐标 */
	uint16_t Height;		/* 高度 */
	uint16_t Width;			/* 宽度 */
	uint16_t MaxLen;		/* 字符串长度 */
	FONT_T *Font;			/* 字体 */
	char  *pCaption;
}LABEL_T;

/* 按钮结构 */
typedef struct
{
	uint8_t id;
	uint8_t Style;		/* 按钮风格，0=普通按钮， 1=圆角按钮， 2=图片按钮 */
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	/* 按钮的颜色，由底层自动管理 */
	FONT_T *Font;			/* 字体 */
	char *pCaption;
	uint8_t Focus;			/* 焦点 */
}BUTTON_T;

/* 按钮的坐标和文本参数结构 */
typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t h;
	uint16_t w;
	char *text;		/* 按钮的文字 */
}BTN_PARAM_T;

/* 图片按钮结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint32_t Pic1;
	uint32_t Pic2;	
	uint8_t Focus;			/* 焦点 */
}BMP_BUTTON_T;

/* 选中结构 */
typedef struct
{
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	
}SELECT_T;

/* 编辑框结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint16_t Color;
	FONT_T *Font;			/* 字体 */
	char   *pCaption;
	uint8_t Focus;
	char Text[32+1];		/* 保存编辑框内的值 */
}EDIT_T;

/* 检查框 CHECK BOX 结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* 左上角X坐标 */
	uint16_t Top;			/* 左上角Y坐标 */
	uint16_t Height;		/* 高度 */
	uint16_t Width;			/* 宽度 */
	uint16_t Color;			/* 颜色 */
	FONT_T *Font;			/* 字体 */
	char  *pCaption;
	uint8_t Checked;		/* 1表示打勾 */
}CHECK_T;

/* 分组框GROUP BOX 结构 */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* 左上角X坐标 */
	uint16_t Top;			/* 左上角Y坐标 */
	uint16_t Height;		/* 高度 */
	uint16_t Width;			/* 宽度 */
	FONT_T *Font;			/* 字体 */
	char  *pCaption;
}GROUP_T;

/* 可供外部模块调用的函数 */
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

