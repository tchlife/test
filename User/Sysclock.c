#include "Sysclock.h"
#include "stm32f10x.h"
#include "delay.h"


uint16_t Sysclock_1ms;
uint16_t Sysclock_10ms;
uint16_t Sysclock_50ms;
uint16_t Sysclock_100ms;
uint8_t Sysclock_sec;
uint16_t b1ms_base_out;
uint16_t b10ms_base_out;
uint16_t b50ms_base_out;
uint16_t b100ms_base_out;
uint16_t bsec_base_out;

void Base_time_init(void)
{
	Sysclock_1ms = 0;
	Sysclock_10ms = 0;
	Sysclock_sec = 0;
	Sysclock_50ms = 0;
	Sysclock_100ms = 0;
	b1ms_base_out = 0;
  b10ms_base_out = 0;
	b50ms_base_out = 0;
  b100ms_base_out = 0;
  bsec_base_out = 0;
}

static void Interrupt_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Usart_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRETE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Tx;
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	Interrupt_init();
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	USART_Cmd(DEBUG_USARTx, ENABLE);
}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	 USART_SendData(pUSARTx,ch);	
	 while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


/// 重定向c库函数printf到macUSARTx
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到macUSARTx */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/// 重定向c库函数scanf到macUSARTx
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}

void SysTick_Init(void)
{
	if (SysTick_Config(SystemCoreClock / 100000))	
	{ 
		while (1);
	}
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}

void time_count(void)
{
	Sysclock_1ms++;
	Sysclock_10ms++;
	Sysclock_50ms++;
}

void Time_Check(void)
{
	if(Sysclock_1ms >= B1MS_BASE)
	{
		b1ms_base_out = 1;
		Sysclock_1ms = 0;
	}
	if(Sysclock_10ms >= B10MS_BASE)
	{
		Sysclock_10ms = 0;
		b10ms_base_out = 1;
		Sysclock_100ms++;
		if(Sysclock_100ms >= B100MS_BASE)
		{
			b100ms_base_out = 1;
			Sysclock_100ms = 0;
		}
	}
	if(Sysclock_50ms >= B50MS_BASE)
	{
		Sysclock_50ms = 0;
		b50ms_base_out = 1;
		Sysclock_sec++;
		if(Sysclock_sec >= BSEC_BASE)
		{
			Sysclock_sec = 0;
			bsec_base_out = 1;
		}
	}
}


void Time_Clear(void)
{
	b1ms_base_out = 0;
	b10ms_base_out = 0;
	b50ms_base_out = 0;
  b100ms_base_out = 0;
  bsec_base_out = 0;
}

void Delay_ms(uint16_t time_ms)
{
	 uint16_t i=0;  
   while(time_ms--)
   {
      i=12000; 
      while(i--) ;    
   }
}

void Sent_ID(void)
{
	Usart_SendByte(DEBUG_USARTx, 0x01);
	Delay_ms(1);
	Usart_SendByte(DEBUG_USARTx, 0x33);
	Delay_ms(1);
	Usart_SendByte(DEBUG_USARTx, 0xAB);
	Delay_ms(1);
	Usart_SendByte(DEBUG_USARTx, 0xCC);
	Delay_ms(1);
}

void IWDG_init(uint8_t prv, uint16_t rlv)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prv);
	IWDG_SetReload(rlv);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void Clear_IWDG(void)
{
	IWDG_ReloadCounter();
}


