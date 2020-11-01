#ifndef __BUZZ_SOUCE_H
#define	__BUZZ_SOUCE_H

#include "stm32f10x.h"
#include "bsp_led.h"

#define  QUICK              1
#define  MIDDLE             2  
#define  SLOW								3

#define  BUZZ_GPIO_PORT    	         GPIOA			              
#define  BUZZ_GPIO_CLK 	             RCC_APB2Periph_GPIOA		
#define  BUZZ_GPIO_PIN		           GPIO_Pin_8


#define  BUZZ_TOGGLE		             digitalToggle(BUZZ_GPIO_PORT,BUZZ_GPIO_PORT)
#define  BUZZ_OFF		                 digitalHi(BUZZ_GPIO_PORT,BUZZ_GPIO_PIN)
#define  BUZZ_ON			               digitalLo(BUZZ_GPIO_PORT,BUZZ_GPIO_PIN)

extern   uint8_t  buzz_frequency;
extern   uint8_t  flash_time;
extern   uint8_t  flash_finish;

#define  BUZZ(a)	if (a)	\
					GPIO_SetBits(BUZZ_GPIO_PORT,BUZZ_GPIO_PORT);\
					else		\
					GPIO_ResetBits(BUZZ_GPIO_PORT,BUZZ_GPIO_PORT)

extern	void Buzz_Init(void);
extern	void Buzz_flash_time(uint8_t time, uint8_t speed);
	
#endif

