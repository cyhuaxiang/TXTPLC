/*
*********************************************************************************************************
*
*	ģ������ : cpu�ڲ�falsh����ģ��
*	�ļ����� : bsp_cpu_flash.h
*	��    �� : V1.0
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_CPU_FLASH_H_
#define _BSP_CPU_FLASH_H_

#define FLASH_BASE_ADDR	(FMC->DFBADR)			/* Flash����ַ */
#define	FLASH_SIZE		(FMC->DFBADR + 0x1000)	/* Flash ���� */

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     (FMC->DFBADR) /* Base @ of Sector 0, 512bytes */
#define ADDR_FLASH_SECTOR_1     (FMC->DFBADR + FMC_FLASH_PAGE_SIZE) /* Base @ of Sector 1, 512bytes */
#define ADDR_FLASH_SECTOR_2     (FMC->DFBADR + FMC_FLASH_PAGE_SIZE*2) /* Base @ of Sector 2, 512bytes */
#define ADDR_FLASH_SECTOR_3     (FMC->DFBADR + FMC_FLASH_PAGE_SIZE*3) /* Base @ of Sector 3, 512bytes */
#define ADDR_FLASH_SECTOR_4     (FMC->DFBADR + FMC_FLASH_PAGE_SIZE*4) /* Base @ of Sector 4, 512bytes */
#define ADDR_FLASH_SECTOR_5     (FMC->DFBADR + FMC_FLASH_PAGE_SIZE*5) /* Base @ of Sector 5, 512bytes */
#define ADDR_FLASH_SECTOR_6     (FMC->DFBADR + FMC_FLASH_PAGE_SIZE*6) /* Base @ of Sector 6, 512bytes */
#define ADDR_FLASH_SECTOR_7     (FMC->DFBADR + FMC_FLASH_PAGE_SIZE*7) /* Base @ of Sector 7, 512bytes */


#define FLASH_IS_EQU		0   /* Flash���ݺʹ�д���������ȣ�����Ҫ������д���� */
#define FLASH_REQ_WRITE		1	/* Flash����Ҫ������ֱ��д */
#define FLASH_REQ_ERASE		2	/* Flash��Ҫ�Ȳ���,��д */
#define FLASH_PARAM_ERR		3	/* ������������ */

uint8_t bsp_ReadCpuFlash(uint32_t _ulFlashAddr, uint32_t *_ucpDst, uint32_t _ulSize);
uint8_t bsp_WriteCpuFlash(uint32_t _ulFlashAddr, uint32_t *_ucpSrc, uint32_t _ulSize);
uint8_t bsp_CmpCpuFlash(uint32_t _ulFlashAddr, uint32_t *_ucpBuf, uint32_t _ulSize);

#endif


/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

