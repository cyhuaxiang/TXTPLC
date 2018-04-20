#include "bsp.h"

/* ����ÿ�����ڽṹ����� */
#if UART0_FIFO_EN == 1
	static UART_TypeDef g_tUart0;
	static uint8_t g_TxBuf0[UART0_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf0[UART0_RX_BUF_SIZE];		/* ���ջ����� */
#endif



static void UartVarInit(void);

static void InitHardUart(void);
static void UartSend(UART_TypeDef *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_TypeDef *_pUart, uint8_t *_pByte);
static void UartIRQ(UART_TypeDef *_pUart);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ������Ӳ�������Դ���ȫ�ֱ�������ֵ���ϵ�ִ��һ�Ρ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	UartVarInit();		/* �����ȳ�ʼ��ȫ�ֱ���,������Ӳ�� */
	InitHardUart();		/* ���ô��ڵ�Ӳ������(�����ʵ�) */

	
}
/*
*********************************************************************************************************
*	�� �� ��: ComToUart
*	����˵��: ��COM�˿ں�ת��ΪUART_TypeDef��
*	��    ��: _ucPort: �˿ں�(COM1 - COM2)
*	�� �� ֵ: UART_TypeDef��
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
		/* �����κδ��� */
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ComToUart
*	����˵��: ��COM�˿ں�ת��Ϊ UART_T
*	��    ��: _ucPort: �˿ں�(COM1 - COM2)
*	�� �� ֵ: UART_T*,  UART0, USART1
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
		/* �����κδ��� */
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: comSendBuf
*	����˵��: �򴮿ڷ���һ�����ݡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
*			  _ucaBuf: �����͵����ݻ�����
*			  _usLen : ���ݳ���
*	�� �� ֵ: ��
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
		pUart->SendBefor();		/* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
	}

	UartSend(pUart, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	�� �� ��: comSendChar
*	����˵��: �򴮿ڷ���1���ֽڡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ��͡�
*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
*			  _ucByte: �����͵�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	comSendBuf(_ucPort, &_ucByte, 1);
}

/*
*********************************************************************************************************
*	�� �� ��: comGetChar
*	����˵��: �ӽ��ջ�������ȡ1�ֽڣ��������������������ݾ��������ء�
*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
*			  _pByte: ���յ������ݴ���������ַ
*	�� �� ֵ: 0 ��ʾ������, 1 ��ʾ��ȡ����Ч�ֽ�
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
*	�� �� ��: comClearTxFifo
*	����˵��: ���㴮�ڷ��ͻ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
*	�� �� ֵ: ��
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
*	�� �� ��: comClearRxFifo
*	����˵��: ���㴮�ڽ��ջ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
*	�� �� ֵ: ��
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
*	�� �� ��: comSetBaud
*	����˵��: ���ô��ڵĲ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
*			  _BaudRate: �����ʣ�0-4500000�� ���4.5Mbps
*	�� �� ֵ: ��
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
*	�� �� ��: UartVarInit
*	����˵��: ��ʼ��������صı���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART0_FIFO_EN == 1
	g_tUart0.uart = UART0;	
	g_tUart0.pTxBuf = g_TxBuf0;					/* ���ͻ�����ָ�� */
	g_tUart0.pRxBuf = g_RxBuf0;					/* ���ջ�����ָ�� */
	g_tUart0.usTxBufSize = UART0_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart0.usRxBufSize = UART0_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart0.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart0.usTxRead = 0;						/* ����FIFO������ */
	g_tUart0.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart0.usRxRead = 0;						/* ����FIFO������ */
	g_tUart0.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart0.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart0.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart0.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart0.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif

}

/*
*********************************************************************************************************
*	�� �� ��: InitHardUart
*	����˵��: ���ô��ڵ�Ӳ�������������ʣ�����λ��ֹͣλ����ʼλ��У��λ���ж�ʹ�ܣ��ʺ���STM32-F4������
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: UartSend
*	����˵��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ��رշ����ж�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartSend(UART_TypeDef *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;

	for (i = 0; i < _usLen; i++)
	{
		/* ������ͻ������Ѿ����ˣ���ȴ��������� */
	#if 0
		/*
			�ڵ���GPRS����ʱ������Ĵ������������while ��ѭ��
			ԭ�� ���͵�1���ֽ�ʱ _pUart->usTxWrite = 1��_pUart->usTxRead = 0;
			������while(1) �޷��˳�
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
		/* �� _pUart->usTxBufSize == 1 ʱ, ����ĺ���������(������) */
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

		/* �����������뷢�ͻ����� */
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
*	�� �� ��: UartGetChar
*	����˵��: �Ӵ��ڽ��ջ�������ȡ1�ֽ����� ��������������ã�
*	��    ��: _pUart : �����豸
*			  _pByte : ��Ŷ�ȡ���ݵ�ָ��
*	�� �� ֵ: 0 ��ʾ������  1��ʾ��ȡ������
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_TypeDef *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite �������жϺ����б���д���������ȡ�ñ���ʱ����������ٽ������� */
	DISABLE_INT();
	usCount = _pUart->usRxCount;
	ENABLE_INT();

	/* �������д������ͬ���򷵻�0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* �Ѿ�û������ */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* �Ӵ��ڽ���FIFOȡ1������ */

		/* ��дFIFO������ */
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
*	�� �� ��: UartIRQ
*	����˵��: ���жϷ��������ã�ͨ�ô����жϴ�����
*	��    ��: _pUart : �����豸
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartIRQ(UART_TypeDef *_pUart)
{
	uint8_t ch;
	uint32_t u32IntSts = _pUart->uart->ISR;
	/* ��������ж�  */
	if(u32IntSts & UART_ISR_RDA_INT_Msk)
	{
		/* �Ӵ��ڽ������ݼĴ�����ȡ���ݴ�ŵ�����FIFO */
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

		/* �ص�����,֪ͨӦ�ó����յ�������,һ���Ƿ���1����Ϣ��������һ����� */
		//if (_pUart->usRxWrite == _pUart->usRxRead)
		//if (_pUart->usRxCount == 1)
		{
			if (_pUart->ReciveNew)
			{
				_pUart->ReciveNew(ch);
			}
		}
		
	}

	/* �����ͻ��������ж� */
	if(u32IntSts & UART_ISR_THRE_INT_Msk)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
			UART_DisableInt(_pUart->uart, UART_IER_THRE_IEN_Msk);
			/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
			if (_pUart->SendOver)
			{
				_pUart->SendOver();
			}
		}
		else
		{
			/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
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
*	�� �� ��: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	����˵��: USART�жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
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
