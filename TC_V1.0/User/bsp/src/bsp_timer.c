/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : bsp_timer.c
*	��    �� : V1.3
*	˵    �� : ����systick��ʱ����Ϊϵͳ�δ�ʱ����ȱʡ��ʱ����Ϊ1ms��
*
*				ʵ���˶�������ʱ����������ʹ��(����1ms)�� ����ͨ���޸� TMR_COUNT ������ʱ������
*				ʵ����ms�����ӳٺ���������1ms�� ��us���ӳٺ���
*				ʵ����ϵͳ����ʱ�亯����1ms��λ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		
*
*	Copyright (C), 2015-2016, 
*
*********************************************************************************************************
*/

#include "bsp.h"

#define USE_TIM0
//#define USE_TIM1
//#define USE_TIM2
//#define USE_TIM3

#ifdef USE_TIM0
	#define TIM_HARD		TIMER0
	#define TIM_HARD_IRQn	TMR0_IRQn
#endif
/* ��2��ȫ�ֱ���ת���� bsp_DelayMS() ���� */
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

/* ���������ʱ���ṹ����� */
static SOFT_TMR s_tTmr[TMR_COUNT];

/*
	ȫ������ʱ�䣬��λ1ms
	����Ա�ʾ 24.85�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
*/
__IO int32_t g_iRunTime = 0;

static void bsp_SoftTimerDec(SOFT_TMR *_tmr);
/* ���� TIM��ʱ�жϵ���ִ�еĻص�����ָ�� */
static void (*s_TIM_CallBack0)(void);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitTimer
*	����˵��: ����systick�жϣ�����ʼ�������ʱ������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	uint8_t i;

	/* �������е������ʱ�� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* ȱʡ��1���Թ���ģʽ */
	}

	/*
		����systic�ж�����Ϊ1ms��������systick�жϡ�

    	SystemCoreClock �ǹ̼��ж����ϵͳ�ں�ʱ�ӣ�����STM32F4XX,һ��Ϊ 168MHz

    	SysTick_Config() �������βα�ʾ�ں�ʱ�Ӷ��ٸ����ں󴥷�һ��Systick��ʱ�ж�.
	    	-- SystemCoreClock / 1000  ��ʾ��ʱƵ��Ϊ 1000Hz�� Ҳ���Ƕ�ʱ����Ϊ  1ms
	    	-- SystemCoreClock / 500   ��ʾ��ʱƵ��Ϊ 500Hz��  Ҳ���Ƕ�ʱ����Ϊ  2ms
	    	-- SystemCoreClock / 2000  ��ʾ��ʱƵ��Ϊ 2000Hz�� Ҳ���Ƕ�ʱ����Ϊ  500us

    	���ڳ����Ӧ�ã�����һ��ȡ��ʱ����1ms�����ڵ���CPU���ߵ͹���Ӧ�ã��������ö�ʱ����Ϊ 10ms
    */
	SysTick_Config(SystemCoreClock/1000);
	bsp_InitHardTimer();
}

/*
*********************************************************************************************************
*	�� �� ��: SysTick_ISR
*	����˵��: SysTick�жϷ������ÿ��1ms����1��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
extern void bsp_RunPer1ms(void);
extern void bsp_RunPer10ms(void);
void SysTick_ISR(void)
{
	static uint8_t s_count = 0;
	uint8_t i;
	
	/* ÿ��1ms����1�� �������� bsp_DelayMS�� */
	if (s_uiDelayCount > 0)
	{
		if (--s_uiDelayCount == 0)
		{
			s_ucTimeOutFlag = 1;
		}
	}

	/* ÿ��1ms���������ʱ���ļ��������м�һ���� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		bsp_SoftTimerDec(&s_tTmr[i]);
	}

	/* ȫ������ʱ��ÿ1ms��1 */
	g_iRunTime++;
	if (g_iRunTime == 0x7FFFFFFF)	/* ��������� int32_t ���ͣ������Ϊ 0x7FFFFFFF */
	{
		g_iRunTime = 0;
	}
	bsp_RunPer1ms();		/* ÿ��1ms����һ�δ˺������˺����� bsp.c */
	
	if (++s_count >= 10)
	{
		s_count = 0;
		bsp_RunPer10ms();	/* ÿ��10ms����һ�δ˺������˺����� bsp.c */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SoftTimerDec
*	����˵��: ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
*	��    ��:  _tmr : ��ʱ������ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* �����ʱ����������1�����ö�ʱ�������־ */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* ������Զ�ģʽ�����Զ���װ������ */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DelayMS
*	����˵��: ms���ӳ٣��ӳپ���Ϊ����1ms
*	��    ��:  n : �ӳٳ��ȣ���λ1 ms�� n Ӧ����2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
	if (n == 0)
	{
		return;
	}
	else if (n == 1)
	{
		n = 2;
	}

	DISABLE_INT();  			/* ���ж� */

	s_uiDelayCount = n;
	s_ucTimeOutFlag = 0;

	ENABLE_INT();  				/* ���ж� */

	while (1)
	{
		bsp_Idle();				/* CPU����ִ�еĲ����� �� bsp.c �� bsp.h �ļ� */

		/*
			�ȴ��ӳ�ʱ�䵽
			ע�⣺��������Ϊ s_ucTimeOutFlag = 0�����Կ����Ż�������� s_ucTimeOutFlag ������������Ϊ volatile
		*/
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
}

/*
*********************************************************************************************************
*    �� �� ��: bsp_DelayUS
*    ����˵��: us���ӳ١� ������systick��ʱ����������ܵ��ô˺�����
*    ��    ��:  n : �ӳٳ��ȣ���λ1 us
*    �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_DelayUS(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;
       
	reload = SysTick->LOAD;                
    ticks = n * (SystemCoreClock/1000000);	 /* ��Ҫ�Ľ����� */  
    
    tcnt = 0;
    told = SysTick->VAL;             /* �ս���ʱ�ļ�����ֵ */

    while (1)
    {
        tnow = SysTick->VAL;    
        if (tnow != told)
        {    
            /* SYSTICK��һ���ݼ��ļ����� */    
            if (tnow < told)
            {
                tcnt += told - tnow;    
            }
            /* ����װ�صݼ� */
            else
            {
                tcnt += reload - tnow + told;    
            }        
            told = tnow;

            /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            if (tcnt >= ticks)
            {
            	break;
            }
        }  
    }
} 


/*
*********************************************************************************************************
*	�� �� ��: bsp_StartTimer
*	����˵��: ����һ����ʱ���������ö�ʱ���ڡ�
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT();  			/* ���ж� */

	s_tTmr[_id].Count = _period;		/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */

	ENABLE_INT();  				/* ���ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StartAutoTimer
*	����˵��: ����һ���Զ���ʱ���������ö�ʱ���ڡ�
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ10ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT();  		/* ���ж� */

	s_tTmr[_id].Count = _period;			/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* �Զ�����ģʽ */

	ENABLE_INT();  			/* ���ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StopTimer
*	����˵��: ֹͣһ����ʱ��
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT();  	/* ���ж� */

	s_tTmr[_id].Count = 0;				/* ʵʱ��������ֵ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* �Զ�����ģʽ */

	ENABLE_INT();  		/* ���ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_CheckTimer
*	����˵��: ��ⶨʱ���Ƿ�ʱ
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ���� 0 ��ʾ��ʱδ���� 1��ʾ��ʱ��
*********************************************************************************************************
*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (s_tTmr[_id].Flag == 1)
	{
		s_tTmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetRunTime
*	����˵��: ��ȡCPU����ʱ�䣬��λ1ms������Ա�ʾ 24.85�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
*	��    ��:  ��
*	�� �� ֵ: CPU����ʱ�䣬��λ1ms
*********************************************************************************************************
*/
int32_t bsp_GetRunTime(void)
{
	int32_t runtime;

	DISABLE_INT();  	/* ���ж� */

	runtime = g_iRunTime;	/* ���������Systick�ж��б���д�������Ҫ���жϽ��б��� */

	ENABLE_INT();  		/* ���ж� */

	return runtime;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_CheckRunTime
*	����˵��: ���㵱ǰ����ʱ��͸���ʱ��֮��Ĳ�ֵ�������˼�����ѭ����
*	��    ��:  _LastTime �ϸ�ʱ��
*	�� �� ֵ: ��ǰʱ��͹�ȥʱ��Ĳ�ֵ����λ1ms
*********************************************************************************************************
*/
int32_t bsp_CheckRunTime(int32_t _LastTime)
{
	int32_t now_time;
	int32_t time_diff;

	DISABLE_INT();  	/* ���ж� */

	now_time = g_iRunTime;	/* ���������Systick�ж��б���д�������Ҫ���жϽ��б��� */

	ENABLE_INT();  		/* ���ж� */
	
	if (now_time >= _LastTime)
	{
		time_diff = now_time - _LastTime;
	}
	else
	{
		time_diff = 0x7FFFFFFF - _LastTime + now_time;
	}

	return time_diff;
}

/*
*********************************************************************************************************
*	�� �� ��: SysTick_Handler
*	����˵��: ϵͳ��શ�ʱ���жϷ�����������ļ��������˸ú�����
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SysTick_Handler(void)
{
	SysTick_ISR();
}

/*
*********************************************************************************************************
*	�� �� ��: TIM_OperatModeConfig
*	����˵��: ���ö�ʱ������ģʽ	
*	��    ��: u32Mode:
*			  TIMER_ONESHOT_MODE
*		      TIMER_PERIODIC_MODE
*		  	  TIMER_TOGGLE_MODE
*			  TIMER_CONTINUOUS_MODE
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM_OperatModeConfig(TIMER_T *timer, uint32_t u32Mode)
{
	timer->TCSR = u32Mode;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitHardTimer
*	����˵��: ���� TIMx������us����Ӳ����ʱ��TIMx���������У�����ֹͣ.
*			TIMx������TIM2 - TIM5 ֮���TIM, ��ЩTIM��4��ͨ��, ���� APB1 �ϣ�����ʱ��=SystemCoreClock / 2
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitHardTimer(void)
{
	/*����TIMER0ʱ��*/
	CLK_EnableModuleClock(TMR0_MODULE);
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, 0);
	
    TIM_OperatModeConfig(TIM_HARD,TIMER_PERIODIC_MODE);
	TIMER_SET_PRESCALE_VALUE(TIM_HARD,5);
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_StartHardTimer
*	����˵��: ʹ��TIM�����ζ�ʱ��ʹ��, ��ʱʱ�䵽��ִ�лص�������
*             ��ʱ��������10us ����Ҫ�ķ��ڵ��ñ�������ִ��ʱ�䣬�����ڲ������˲�����С��
*	��    ��: 
*             _uiTimeOut : ��ʱʱ��, ��λ 1us.       ����16λ��ʱ������� 65.5ms; ����32λ��ʱ������� 4294��
*             _pCallBack : ��ʱʱ�䵽�󣬱�ִ�еĺ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartHardTimer(uint32_t _uiTimeOut, void * _pCallBack)
{
	uint32_t cnt_now;
    uint32_t cnt_tar;

	/*
        ִ�����������䣬ʱ�� = 18us (ͨ���߼������ǲ���IO��ת)
        bsp_StartTimer2(3, 500, (void *)test1);
    */
    if (_uiTimeOut < 5)
    {
        ;
    }
    else
    {
        _uiTimeOut -= 5;
    }

    cnt_now = TIMER_GetCounter(TIM_HARD);    	/* ��ȡ��ǰ�ļ�����ֵ */
    cnt_tar = cnt_now + _uiTimeOut;			/* ���㲶��ļ�����ֵ */
	cnt_tar = cnt_tar/2;				    /*ת����Compare Value*/
	
	s_TIM_CallBack0 = (void (*)(void))_pCallBack;	
	TIMER_SET_CMP_VALUE(TIM_HARD,cnt_tar);
	TIMER_EnableInt(TIM_HARD);
	NVIC_EnableIRQ(TIM_HARD_IRQn);
	TIMER_Start(TIM_HARD);

}
/*
*********************************************************************************************************
*	�� �� ��: TIMx_IRQHandler
*	����˵��: TIM �жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TMR0_IRQHandler(void)
{
	if(TIMER_GetIntFlag(TIM_HARD) == 1)
    {
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIM_HARD);
		TIMER_DisableInt(TIM_HARD);
		NVIC_DisableIRQ(TIM_HARD_IRQn);
		TIMER_Stop(TIM_HARD);
        s_TIM_CallBack0();
    }
}
