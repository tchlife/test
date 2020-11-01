#include "stm32f10x.h"
#include "Timer.h"
#include "Sysclock.h"

uint16_t timer;
uint16_t tpad_default_val;

void Timer_init(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStucture;
		BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
		TIM_TimeBaseStucture.TIM_Period = 1000;
		TIM_TimeBaseStucture.TIM_Prescaler = 71;
		TIM_TimeBaseStucture.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStucture.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStucture.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStucture);
		TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
		TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);
		TIM_Cmd(BASIC_TIM, ENABLE);
		BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, DISABLE);
		timer = 0;
		tpad_default_val = 0;
		BASIC_TIM_NVIC_Config();
}

void BASIC_TIM_NVIC_Config(void)
{
		NVIC_InitTypeDef NVIC_IniStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		NVIC_IniStructure.NVIC_IRQChannel = BASIC_TIM_IRQ;
		NVIC_IniStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_IniStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_IniStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_IniStructure);
//		BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
}

void TPAD_TIM_GPIO_init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(TPAD_TIM_CH_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
}

void TPAD_TIM_MODE_Init(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStucture;
		TIM_ICInitTypeDef TIM_ICInitStructure;
		TPAD_TIM_APBxClock_FUN(TPAD_TIM_CLK, ENABLE);
		TIM_TimeBaseStucture.TIM_Period = TPAD_TIM_Period;
		TIM_TimeBaseStucture.TIM_Prescaler = TPAD_TIM_Prescler;
		TIM_TimeBaseStucture.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStucture.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStucture.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TPAD_TIM, &TIM_TimeBaseStucture);
	
		TIM_ICInitStructure.TIM_Channel = TPAD_TIM_CH_CHANNEL;
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0;
		TIM_ICInit(TPAD_TIM, &TIM_ICInitStructure);
	
		TIM_Cmd(TPAD_TIM, ENABLE);
}


void TPAD_Reset(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(TPAD_TIM_CH_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
	
		GPIO_ResetBits(TPAD_TIM_CH_PORT, TPAD_TIM_CH_PIN);
	
		Delay_ms(5);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
}


uint16_t TPAD_Get_Val(void)
{
		TPAD_Reset();
		TIM_SetCounter(TPAD_TIM, 0);
		TIM_ClearITPendingBit(TPAD_TIM, TPAD_TIM_IT_CCX | TIM_IT_Update);
		while(TIM_GetFlagStatus(TPAD_TIM, TPAD_TIM_IT_CCX) == RESET)
		{
				if(TIM_GetCounter(TPAD_TIM) > TPAD_TIM_Period - 100)
				{
						return TIM_GetCounter(TPAD_TIM);
				}
		}
		return TPAD_TIM_GetCapturex_FUN(TPAD_TIM);
}


uint8_t TPAD_Init(void)
{
	uint16_t temp;
	
	TPAD_TIM_GPIO_init();
	TPAD_TIM_MODE_Init();	
	
	temp = TPAD_Get_Val();
	if((TPAD_DEAULT_VAL_MIN < temp) && (temp < TPAD_DEAULT_VAL_MAX) )
	{
		tpad_default_val = temp;
		return 0;  
	}
	else
  {
		return 1; 
  }
} 

uint16_t TPAD_Get_MaxVal(uint8_t num)
{
		uint16_t temp = 0, res = 0;
		while(num--)
		{
				temp = TPAD_Get_Val();
				if(temp > res)
						res = temp;
		}
		return res;
}

uint8_t TPAD_Scan(void)
{
		static uint8_t keyen = 0;
		uint8_t res = 0, sample = 3;
		uint16_t scan_val;
		scan_val = TPAD_Get_MaxVal(sample);
		if(scan_val > (tpad_default_val + TPAD_GATE_VAL))
		{
			scan_val = TPAD_Get_MaxVal(sample);
			if((keyen == 0) && (scan_val > (tpad_default_val + TPAD_GATE_VAL)))
				res = 1;
			
			keyen = 2;
		}
			
		if(keyen > 0)
			keyen--;
		
		return res;
}

void BASIC_TIM_IRQHandler(void)
{
		if(TIM_GetFlagStatus(BASIC_TIM, TIM_IT_Update) != RESET)
			timer++;
			TIM_ClearITPendingBit(BASIC_TIM, TIM_FLAG_Update);
}





