/*
*********************************************************************************************************
*
*	ģ������ : Ӧ�ó������ģ��
*	�ļ����� : param.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __PARAM_H
#define __PARAM_H

#define PARAM_ADDR		ADDR_FLASH_SECTOR_0
#define PARAM_VER			0x00000104					/* �����汾 */

/* ȫ�ֲ��� */
typedef struct
{
	uint32_t ParamVer;			/* �������汾���ƣ������ڳ�������ʱ�������Ƿ�Բ��������������� */

	uint32_t RiseDelay;/* ������ʱ */
	uint32_t FallDelay;/* ������ʱ */
	uint32_t TotalNum;/*�ӹ�����*/
	uint32_t WorkMode;/*�ӹ�ģʽ*/

}
PARAM_T;
/*                    ��Сֵ,  ���ֵ,   ȱʡֵ,  �������� */
#define P000_DEF    {       0,    00,       0, "-------"}
#define P001_DEF    {       0,    99,       0, "������ʱ"}
#define P002_DEF    {       0,    99,       0, "�½���ʱ"}


#define	P_SET_NUM	2

typedef struct
{
	int32_t Min;		/* �������ֵ */
	int32_t Max;		/* �������ֵ */
	int32_t Default;	/* ȱʡ����ֵ */
	char Name[32+1];	/* �������� */	
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
