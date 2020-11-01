#ifndef __SYSCLOCK_H_
#define	__SYSCLOCK_H_

#include "stm32f10x.h"
#include <stdio.h>

#define B1MS_BASE                       100
#define B10MS_BASE                      1000
#define B50MS_BASE                      5000
#define B100MS_BASE                     10
#define BSEC_BASE                       20

#define DEBUG_USARTx 				            USART1
#define DEBUG_USART_CLK 				        RCC_APB2Periph_USART1
#define DEBUG_USART_APBxClkCmd 					RCC_APB2PeriphClockCmd
#define DEBUG_USART_BAUDRETE 		  			115200
#define DEBUG_USART_GPIO_CLK 		  			(RCC_APB2Periph_GPIOA)
#define DEBUG_USART_GPIO_APBxClkCmd 		RCC_APB2PeriphClockCmd
#define DEBUG_USART_TX_GPIO_PORT 				GPIOA
#define DEBUG_USART_TX_GPIO_PIN 				GPIO_Pin_9
#define DEBUG_USART_RX_GPIO_PORT 				GPIOA
#define DEBUG_USART_RX_GPIO_PIN 				GPIO_Pin_10
#define DEBUG_USART_IRQ 				        USART1_IRQn
#define DEBUG_USART_IRQHandler 				  USART1_IRQHandler

extern  uint16_t Sysclock_10ms;
extern  uint16_t Sysclock_50ms;
extern  uint16_t Sysclock_100ms;
extern  uint8_t Sysclock_sec;
extern  uint16_t b1ms_base_out;
extern  uint16_t b10ms_base_out;
extern  uint16_t b50ms_base_out;
extern  uint16_t b100ms_base_out;
extern  uint16_t bsec_base_out;

extern  void Usart_init(void);
extern  void SysTick_Init(void);
extern  void Base_time_init(void);
extern  void time_count(void);
extern  void Time_Check(void);
extern  void Time_Clear(void);
extern  void Sent_ID(void);
extern  void Clear_IWDG(void);
extern  void IWDG_init(uint8_t prv, uint16_t rlv);
extern  void Delay_ms(uint16_t time_ms);
extern  void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

#endif

