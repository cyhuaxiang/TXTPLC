#include "bsp.h"

/* 定义每个串口结构体变量 */
#if UART0_FIFO_EN == 1
	static UART_TypeDef g_tUart0;
	static uint8_t g_TxBuf0[UART0_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf0[UART0_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif



static void UartVarInit(void);

static void InitHardUart(void);
static void UartSend(UART_TypeDef *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_TypeDef *_pUart, uint8_t *_pByte);
static void UartIRQ(UART_TypeDef *_pUart);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化串口硬件，并对串口全局变量赋初值。上电执行一次。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	UartVarInit();		/* 必须先初始化全局变量,再配置硬件 */
	InitHardUart();		/* 配置串口的硬件参数(波特率等) */

	
}
/*
*********************************************************************************************************
*	函 数 名: ComToUart
*	功能说明: 将COM端口号转换为UART_TypeDef针
*	形    参: _ucPort: 端口号(COM1 - COM2)
*	返 回 值: UART_TypeDef针
*********************************************************************************************************
*/
UART_TypeDef *ComToUart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM0)
	{
		#if UART0_FIFO_EN == 1
			return &g_tUart0;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return &g_tUart1;
		#else
			return 0;
		#endif
	}
	else
	{
		/* 不做任何处理 */
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ComToUart
*	功能说明: 将COM端口号转换为 UART_T
*	形    参: _ucPort: 端口号(COM1 - COM2)
*	返 回 值: UART_T*,  UART0, USART1
*********************************************************************************************************
*/
UART_T *ComToUARTx(COM_PORT_E _ucPort)
{
	if (_ucPort == COM0)
	{
		#if UART0_FIFO_EN == 1
			return UART0;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return UART1;
		#else
			return 0;
		#endif
	}
	else
	{
		/* 不做任何处理 */
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: comSendBuf
*	功能说明: 向串口发送一组数据。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
*	形    参: _ucPort: 端口号(COM1 - COM5)
*			  _ucaBuf: 待发送的数据缓冲区
*			  _usLen : 数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_TypeDef *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	if (pUart->SendBefor != 0)
	{
		pUart->SendBefor();		/* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	}

	UartSend(pUart, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	函 数 名: comSendChar
*	功能说明: 向串口发送1个字节。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送。
*	形    参: _ucPort: 端口号(COM1 - COM5)
*			  _ucByte: 待发送的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	comSendBuf(_ucPort, &_ucByte, 1);
}

/*
*********************************************************************************************************
*	函 数 名: comGetChar
*	功能说明: 从接收缓冲区读取1字节，非阻塞。无论有无数据均立即返回。
*	形    参: _ucPort: 端口号(COM1 - COM5)
*			  _pByte: 接收到的数据存放在这个地址
*	返 回 值: 0 表示无数据, 1 表示读取到有效字节
*********************************************************************************************************
*/
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_TypeDef *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return 0;
	}

	return UartGetChar(pUart, _pByte);
}

/*
*********************************************************************************************************
*	函 数 名: comClearTxFifo
*	功能说明: 清零串口发送缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM5)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearTxFifo(COM_PORT_E _ucPort)
{
	UART_TypeDef *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usTxWrite = 0;
	pUart->usTxRead = 0;
	pUart->usTxCount = 0;
}

/*
*********************************************************************************************************
*	函 数 名: comClearRxFifo
*	功能说明: 清零串口接收缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM5)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearRxFifo(COM_PORT_E _ucPort)
{
	UART_TypeDef *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usRxWrite = 0;
	pUart->usRxRead = 0;
	pUart->usRxCount = 0;
}

/*
*********************************************************************************************************
*	函 数 名: comSetBaud
*	功能说明: 设置串口的波特率
*	形    参: _ucPort: 端口号(COM1 - COM5)
*			  _BaudRate: 波特率，0-4500000， 最大4.5Mbps
*	返 回 值: 无
*********************************************************************************************************
*/
void comSetBaud(COM_PORT_E _ucPort, uint32_t _BaudRate)
{
	UART_T* UARTx;
	
	UARTx = ComToUARTx(_ucPort);
	if (UARTx == 0)
	{
		return;
	}
	UART_Open(UARTx, _BaudRate);
}
/*
*********************************************************************************************************
*	函 数 名: UartVarInit
*	功能说明: 初始化串口相关的变量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART0_FIFO_EN == 1
	g_tUart0.uart = UART0;	
	g_tUart0.pTxBuf = g_TxBuf0;					/* 发送缓冲区指针 */
	g_tUart0.pRxBuf = g_RxBuf0;					/* 接收缓冲区指针 */
	g_tUart0.usTxBufSize = UART0_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart0.usRxBufSize = UART0_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart0.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart0.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart0.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart0.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart0.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart0.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart0.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart0.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart0.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

}

/*
*********************************************************************************************************
*	函 数 名: InitHardUart
*	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）适合于STM32-F4开发板
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitHardUart(void)
{

#if UART0_FIFO_EN == 1		
	/* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set P3 multi-function pins for UART0 RXD and TXD */
    SYS->P3_MFP &= ~(SYS_MFP_P30_Msk | SYS_MFP_P31_Msk);
    SYS->P3_MFP |= (SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0);
	/* Reset IP */
    SYS_ResetModule(UART0_RST);
    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART0, 115200);
	UART_EnableInt(UART0, UART_IER_RDA_IEN_Msk);
#endif

}
/*
*********************************************************************************************************
*	函 数 名: UartSend
*	功能说明: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartSend(UART_TypeDef *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;

	for (i = 0; i < _usLen; i++)
	{
		/* 如果发送缓冲区已经满了，则等待缓冲区空 */
	#if 0
		/*
			在调试GPRS例程时，下面的代码出现死机，while 死循环
			原因： 发送第1个字节时 _pUart->usTxWrite = 1；_pUart->usTxRead = 0;
			将导致while(1) 无法退出
		*/
		while (1)
		{
			uint16_t usRead;

			DISABLE_INT();
			usRead = _pUart->usTxRead;
			ENABLE_INT();

			if (++usRead >= _pUart->usTxBufSize)
			{
				usRead = 0;
			}

			if (usRead != _pUart->usTxWrite)
			{
				break;
			}
		}
	#else
		/* 当 _pUart->usTxBufSize == 1 时, 下面的函数会死掉(待完善) */
		while (1)
		{
			__IO uint16_t usCount;

			DISABLE_INT();
			usCount = _pUart->usTxCount;
			ENABLE_INT();

			if (usCount < _pUart->usTxBufSize)
			{
				break;
			}
		}
	#endif

		/* 将新数据填入发送缓冲区 */
		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];

		DISABLE_INT();
		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
		{
			_pUart->usTxWrite = 0;
		}
		_pUart->usTxCount++;
		ENABLE_INT();
	}

	UART_EnableInt(_pUart->uart, UART_IER_THRE_IEN_Msk);
}

/*
*********************************************************************************************************
*	函 数 名: UartGetChar
*	功能说明: 从串口接收缓冲区读取1字节数据 （用于主程序调用）
*	形    参: _pUart : 串口设备
*			  _pByte : 存放读取数据的指针
*	返 回 值: 0 表示无数据  1表示读取到数据
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_TypeDef *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite 变量在中断函数中被改写，主程序读取该变量时，必须进行临界区保护 */
	DISABLE_INT();
	usCount = _pUart->usRxCount;
	ENABLE_INT();

	/* 如果读和写索引相同，则返回0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* 已经没有数据 */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* 从串口接收FIFO取1个数据 */

		/* 改写FIFO读索引 */
		DISABLE_INT();
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)
		{
			_pUart->usRxRead = 0;
		}
		_pUart->usRxCount--;
		ENABLE_INT();
		return 1;
	}
}

/*
*********************************************************************************************************
*	函 数 名: UartIRQ
*	功能说明: 供中断服务程序调用，通用串口中断处理函数
*	形    参: _pUart : 串口设备
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartIRQ(UART_TypeDef *_pUart)
{
	uint8_t ch;
	uint32_t u32IntSts = _pUart->uart->ISR;
	/* 处理接收中断  */
	if(u32IntSts & UART_ISR_RDA_INT_Msk)
	{
		/* 从串口接收数据寄存器读取数据存放到接收FIFO */
		ch = UART_READ(_pUart->uart);
		_pUart->pRxBuf[_pUart->usRxWrite] = ch;
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
		{
			_pUart->usRxWrite = 0;
		}
		if (_pUart->usRxCount < _pUart->usRxBufSize)
		{
			_pUart->usRxCount++;
		}

		/* 回调函数,通知应用程序收到新数据,一般是发送1个消息或者设置一个标记 */
		//if (_pUart->usRxWrite == _pUart->usRxRead)
		//if (_pUart->usRxCount == 1)
		{
			if (_pUart->ReciveNew)
			{
				_pUart->ReciveNew(ch);
			}
		}
		
	}

	/* 处理发送缓冲区空中断 */
	if(u32IntSts & UART_ISR_THRE_INT_Msk)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
			UART_DisableInt(_pUart->uart, UART_IER_THRE_IEN_Msk);
			/* 回调函数, 一般用来处理RS485通信，将RS485芯片设置为接收模式，避免抢占总线 */
			if (_pUart->SendOver)
			{
				_pUart->SendOver();
			}
		}
		else
		{
			/* 从发送FIFO取1个字节写入串口发送数据寄存器 */
			UART_WRITE(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	功能说明: USART中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART0_FIFO_EN == 1
void UART0_IRQHandler(void)
{
	UartIRQ(&g_tUart0);
}
#endif

#if UART1_FIFO_EN == 1
void UART1_IRQHandler(void)
{
	UartIRQ(&g_tUart1);
}
#endif
