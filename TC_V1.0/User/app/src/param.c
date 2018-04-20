/*
*********************************************************************************************************
*
*	模块名称 : 应用程序参数模块
*	文件名称 : param.c
*	版    本 : V1.0
*	说    明 : 读取和保存应用程序的参数
*	修改记录 :
*		版本号  日期        作者     说明
*
*	Copyright (C), 2015-2020, xxxxxx
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "param.h"

PARAM_T g_tParam;
const PARAM_CODE_T g_PaList[] = 
{
	P000_DEF,
	P001_DEF,
	P002_DEF,

};


/*
*********************************************************************************************************
*	函 数 名: LoadParam
*	功能说明: 从Flash读参数到g_tParam
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void LoadParam(void)
{
	/* 读取CPU Flash中的参数 */
	bsp_ReadCpuFlash(PARAM_ADDR, (uint32_t *)&g_tParam, sizeof(PARAM_T));

	/* 填充缺省参数 */
	if (g_tParam.ParamVer != PARAM_VER)
	{
		g_tParam.ParamVer = PARAM_VER;

		g_tParam.RiseDelay = 0;
		g_tParam.FallDelay = 0;

		SaveParam();							/* 将新参数写入Flash */
	}
}

/*
*********************************************************************************************************
*	函 数 名: SaveParam
*	功能说明: 将全局变量g_tParam 写入到CPU内部Flash
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SaveParam(void)
{
	/* 将全局的参数变量保存到 CPU Flash */
	bsp_WriteCpuFlash(PARAM_ADDR, (uint32_t *)&g_tParam, sizeof(PARAM_T));
}

/*
*********************************************************************************************************
*	函 数 名: AdjustParamValue
*	功能说明: 调节参数值
*	形    参: _ParamCode: 参数代码  P_CODE_E
*			  _mode : 0表示清零，1表示+1， -1表示减1
*	返 回 值: 1 表示参数值更改了， 0 表示参数值未变化。 APP据此决定是否刷新参数.
*********************************************************************************************************
*/
#define PARAM_FUNC(param)   \
	if (_add == 0)			\
	{						\
		param = 0;			\
	}						\
	else					\
	{						\
		iTemp = param;		\
		iTemp += _add;		\
		if (iTemp < g_PaList[_ParamCode].Min)	\
		{										\
			iTemp = g_PaList[_ParamCode].Min;	\
		}										\
		else if (iTemp > g_PaList[_ParamCode].Max)	\
		{											\
			iTemp = g_PaList[_ParamCode].Max;		\
		}											\
		param = iTemp;								\
	}
			
uint8_t AdjustParamValue(uint16_t _ParamCode, int16_t _add)
{	
	int32_t iTemp;
	
	switch (_ParamCode)
	{
		case 1:  PARAM_FUNC(g_tParam.RiseDelay); break;
		case 2:  PARAM_FUNC(g_tParam.FallDelay); break;
			
		default:
			return 0;			
	}
	
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: FormatParamValue
*	功能说明: 将参数值转换为显示字符串
*	形    参: _ParamCode: 参数代码  P_CODE_E
*			  _str : 将参数值转换为字符串，用于APP显示
*	返 回 值: 无
*********************************************************************************************************
*/
void FormatParamValue(uint16_t _ParamCode, char *_str)
{	
	switch (_ParamCode)
	{
		case 1:	/* P001 上升延时时间 */
			sprintf(_str, "%d.%dS", g_tParam.RiseDelay / 10, g_tParam.RiseDelay % 10);					
			break;				
		case 2:	/* P002 下降延时时间 */
			sprintf(_str, "%d.%dS", g_tParam.FallDelay / 10, g_tParam.FallDelay % 10);					
			break;
		default:
			strcpy(_str, "---");
			break;
	}
}

