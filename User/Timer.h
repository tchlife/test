#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"

#define   BASIC_TIM6

#ifdef 		BASIC_TIM6
#define		BASIC_TIM						              TIM6
#define		BASIC_TIM_APBxClock_FUN						RCC_APB1PeriphClockCmd
#define		BASIC_TIM_CLK						          RCC_APB1Periph_TIM6
#define		BASIC_TIM_IRQ						          TIM6_IRQn
#define		BASIC_TIM_IRQHandler              TIM6_IRQHandler

#else
#define		BASIC_TIM						              TIM7
#define		BASIC_TIM_APBxClock_FUN						RCC_APB1PeriphClockCmd
#define		BASIC_TIM_CLK						          RCC_APB1Periph_TIM7
#define		BASIC_TIM_IRQ						          TIM7_IRQn
#define		BASIC_TIM_IRQHandler              TIM7_IRQHandler
#endif

#define   TPAD_TIM                          TIM5
#define		TPAD_TIM_APBxClock_FUN						RCC_APB1PeriphClockCmd
#define		TPAD_TIM_CLK						          RCC_APB1Periph_TIM5
#define		TPAD_TIM_Period						        0XFFFF
#define		TPAD_TIM_Prescler						      71

#define		TPAD_TIM_CH_GPIO_CLK						  RCC_APB2Periph_GPIOA
#define		TPAD_TIM_CH_PORT						      GPIOA
#define		TPAD_TIM_CH_PIN						        GPIO_Pin_1
#define		TPAD_TIM_CH_CHANNEL						    TIM_Channel_2

#define		TPAD_TIM_IT_CCX						        TIM_IT_CC2
#define		TPAD_TIM_IRQ						          TIM_IT_IRQn
#define		TPAD_TIM_INT_FUN						      TIM5_IRQHandler

#define		TPAD_TIM_GetCapturex_FUN				  TIM_GetCapture2
#define		TPAD_TIM_OCxPolartyConfig_FUN		  TIM_OC2PolarityConfig

#define		TPAD_GATE_VAL				              70
#define		TPAD_DEAULT_VAL_MIN				        70
#define		TPAD_DEAULT_VAL_MAX				        80

#define		TPAD_ON				                    1
#define		TPAD_OFF				                  0


extern    uint16_t timer;

extern    void TIMx_CAPTURE_GPIO_Config(void);
extern    void Timer_init(void);
extern    void BASIC_TIM_NVIC_Config(void);
extern    void TPAD_TIM_GPIO_init(void);
extern    void TPAD_TIM_MODE_Init(void);
extern    void TPAD_Reset(void);
extern    uint16_t TPAD_Get_Val(void);
extern    uint16_t TPAD_Get_MaxVal(uint8_t num);
extern    uint8_t TPAD_Scan(void);
extern    uint8_t TPAD_Init(void);


#endif


