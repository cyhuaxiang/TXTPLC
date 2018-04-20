/*
*********************************************************************************************************
*
*	模块名称 : 应用程序参数模块
*	文件名称 : param.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __PARAM_H
#define __PARAM_H

#define PARAM_ADDR		ADDR_FLASH_SECTOR_0
#define PARAM_VER			0x00000104					/* 参数版本 */

/* 全局参数 */
typedef struct
{
	uint32_t ParamVer;			/* 参数区版本控制（可用于程序升级时，决定是否对参数区进行升级） */

	uint32_t RiseDelay;/* 上升延时 */
	uint32_t FallDelay;/* 上升延时 */
	uint32_t TotalNum;/*加工数量*/
	uint32_t WorkMode;/*加工模式*/

}
PARAM_T;
/*                    最小值,  最大值,   缺省值,  参数描述 */
#define P000_DEF    {       0,    00,       0, "-------"}
#define P001_DEF    {       0,    99,       0, "上升延时"}
#define P002_DEF    {       0,    99,       0, "下降延时"}


#define	P_SET_NUM	2

typedef struct
{
	int32_t Min;		/* 参数最大值 */
	int32_t Max;		/* 参数最大值 */
	int32_t Default;	/* 缺省参数值 */
	char Name[32+1];	/* 参数名字 */	
}
PARAM_CODE_T;

extern PARAM_T g_tParam;

extern const PARAM_CODE_T g_PaList[];
extern const uint16_t g_PaSet[P_SET_NUM];

void LoadParam(void);
void SaveParam(void);

uint8_t AdjustParamValue(uint16_t _ParamCode, int16_t _add);
void FormatParamValue(uint16_t _ParamCode, char *_str);

#endif
