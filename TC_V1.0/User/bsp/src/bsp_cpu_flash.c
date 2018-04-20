/*
*********************************************************************************************************
*
*	模块名称 : cpu内部falsh操作模块(for F4)
*	文件名称 : bsp_cpu_flash.c
*	版    本 : V1.0
*	说    明 : 提供读写CPU内部Flash的函数
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"


/*
*********************************************************************************************************
*	函 数 名: bsp_ReadCpuFlash
*	功能说明: 读取CPU Flash的内容
*	形    参:  _ucpDst : 目标缓冲区
*			 _ulFlashAddr : 起始地址
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值: 0=成功，1=失败
*********************************************************************************************************
*/
uint8_t bsp_ReadCpuFlash(uint32_t _ulFlashAddr, uint32_t *_ucpDst, uint32_t _ulSize)
{
	uint32_t i;

	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}

	/* 长度为0时不继续操作,否则起始地址为奇地址会出错 */
	if (_ulSize == 0)
	{
		return 1;
	}
	/* Unlock protected registers */
    SYS_UnlockReg();
	/* Enable FMC ISP function */
    FMC_Open();
	for (i = 0; i < _ulSize/4; i++)
	{
//		*_ucpDst++ = *(uint32_t *)_ulFlashAddr++;
		*_ucpDst++ = FMC_Read(_ulFlashAddr);
		_ulFlashAddr += 4;
	}
	/* Disable FMC ISP function */
    FMC_Close();
    /* Lock protected registers */
    SYS_LockReg();
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_CmpCpuFlash
*	功能说明: 比较Flash指定地址的数据.
*	形    参: _ulFlashAddr : Flash地址
*			 _ucpBuf : 数据缓冲区
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值:
*			FLASH_IS_EQU		0   Flash内容和待写入的数据相等，不需要擦除和写操作
*			FLASH_REQ_WRITE		1	Flash不需要擦除，直接写
*			FLASH_REQ_ERASE		2	Flash需要先擦除,再写
*			FLASH_PARAM_ERR		3	函数参数错误
*********************************************************************************************************
*/
uint8_t bsp_CmpCpuFlash(uint32_t _ulFlashAddr, uint32_t *_ucpBuf, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucIsEqu;	/* 相等标志 */
	uint32_t ucByte;

	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return FLASH_PARAM_ERR;		/*　函数参数错误　*/
	}

	/* 长度为0时返回正确 */
	if (_ulSize == 0)
	{
		return FLASH_IS_EQU;		/* Flash内容和待写入的数据相等 */
	}

	ucIsEqu = 1;			/* 先假设所有字节和待写入的数据相等，如果遇到任何一个不相等，则设置为 0 */
	/* Unlock protected registers */
    SYS_UnlockReg();
	/* Enable FMC ISP function */
    FMC_Open();
	for (i = 0; i < _ulSize/4; i++)
	{
//		ucByte = *(uint32_t *)_ulFlashAddr;
		ucByte = FMC_Read(_ulFlashAddr);

		if (ucByte != *_ucpBuf)
		{
			if (ucByte != 0xFFFF)
			{
				return FLASH_REQ_ERASE;		/* 需要擦除后再写 */
			}
			else
			{
				ucIsEqu = 0;	/* 不相等，需要写 */
			}
		}

		_ulFlashAddr += 4;
		_ucpBuf++;
	}
	/* Disable FMC ISP function */
    FMC_Close();
    /* Lock protected registers */
    SYS_LockReg();

	if (ucIsEqu == 1)
	{
		return FLASH_IS_EQU;	/* Flash内容和待写入的数据相等，不需要擦除和写操作 */
	}
	else
	{
		return FLASH_REQ_WRITE;	/* Flash不需要擦除，直接写 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_WriteCpuFlash
*	功能说明: 写数据到CPU 内部Flash。
*	形    参: _ulFlashAddr : Flash地址
*			 _ucpSrc : 数据缓冲区
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值: 0-成功，1-数据长度或地址溢出，2-写Flash出错(估计Flash寿命到)
*********************************************************************************************************
*/
uint8_t bsp_WriteCpuFlash(uint32_t _ulFlashAddr, uint32_t *_ucpSrc, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucRet;

	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}

	/* 长度为0时不继续操作  */
	if (_ulSize == 0)
	{
		return 0;
	}

	ucRet = bsp_CmpCpuFlash(_ulFlashAddr, _ucpSrc, _ulSize);

	if (ucRet == FLASH_IS_EQU)
	{
		return 0;
	}

	DISABLE_INT();  			/* 关中断 */		
	/* Unlock protected registers */
    SYS_UnlockReg();
	/* Enable FMC ISP function */
    FMC_Open();
	/* 需要擦除 */
	if (ucRet == FLASH_REQ_ERASE)
	{
		// Erase page
		FMC_Erase(_ulFlashAddr);
	}

	/* 按字节模式编程（为提高效率，可以按字编程，一次写入4字节） */
	for (i = 0; i < _ulSize/4; i++)
	{
		FMC_Write(_ulFlashAddr, *_ucpSrc++);
		_ulFlashAddr += 4;
	}

	/* Disable FMC ISP function */
    FMC_Close();
    /* Lock protected registers */
    SYS_LockReg();

  	ENABLE_INT();  				/* 开中断 */

	return 0;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
