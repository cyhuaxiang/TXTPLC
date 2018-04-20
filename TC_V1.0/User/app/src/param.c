/*
*********************************************************************************************************
*
*	ģ������ : Ӧ�ó������ģ��
*	�ļ����� : param.c
*	��    �� : V1.0
*	˵    �� : ��ȡ�ͱ���Ӧ�ó���Ĳ���
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
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
*	�� �� ��: LoadParam
*	����˵��: ��Flash��������g_tParam
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LoadParam(void)
{
	/* ��ȡCPU Flash�еĲ��� */
	bsp_ReadCpuFlash(PARAM_ADDR, (uint32_t *)&g_tParam, sizeof(PARAM_T));

	/* ���ȱʡ���� */
	if (g_tParam.ParamVer != PARAM_VER)
	{
		g_tParam.ParamVer = PARAM_VER;

		g_tParam.RiseDelay = 0;
		g_tParam.FallDelay = 0;

		SaveParam();							/* ���²���д��Flash */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: SaveParam
*	����˵��: ��ȫ�ֱ���g_tParam д�뵽CPU�ڲ�Flash
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SaveParam(void)
{
	/* ��ȫ�ֵĲ����������浽 CPU Flash */
	bsp_WriteCpuFlash(PARAM_ADDR, (uint32_t *)&g_tParam, sizeof(PARAM_T));
}

/*
*********************************************************************************************************
*	�� �� ��: AdjustParamValue
*	����˵��: ���ڲ���ֵ
*	��    ��: _ParamCode: ��������  P_CODE_E
*			  _mode : 0��ʾ���㣬1��ʾ+1�� -1��ʾ��1
*	�� �� ֵ: 1 ��ʾ����ֵ�����ˣ� 0 ��ʾ����ֵδ�仯�� APP�ݴ˾����Ƿ�ˢ�²���.
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
*	�� �� ��: FormatParamValue
*	����˵��: ������ֵת��Ϊ��ʾ�ַ���
*	��    ��: _ParamCode: ��������  P_CODE_E
*			  _str : ������ֵת��Ϊ�ַ���������APP��ʾ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void FormatParamValue(uint16_t _ParamCode, char *_str)
{	
	switch (_ParamCode)
	{
		case 1:	/* P001 ������ʱʱ�� */
			sprintf(_str, "%d.%dS", g_tParam.RiseDelay / 10, g_tParam.RiseDelay % 10);					
			break;				
		case 2:	/* P002 �½���ʱʱ�� */
			sprintf(_str, "%d.%dS", g_tParam.FallDelay / 10, g_tParam.FallDelay % 10);					
			break;
		default:
			strcpy(_str, "---");
			break;
	}
}

