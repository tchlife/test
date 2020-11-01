#ifndef __KEY_SCAN_H
#define	__KEY_SCAN_H

#include "stm32f10x.h"

#define   KEY_OFF                   0
#define   KEY_ON                    1

#define   KEY1                      1
#define   KEY2                      2
#define   KEY3                      3

#define   KEY1_GPIO_PORT    	      GPIOA			              
#define   KEY1_GPIO_CLK 	          RCC_APB2Periph_GPIOA		
#define   KEY1_GPIO_PIN		          GPIO_Pin_0

#define   KEY2_GPIO_PORT    	      GPIOC		              
#define   KEY2_GPIO_CLK 	          RCC_APB2Periph_GPIOC		
#define   KEY2_GPIO_PIN		          GPIO_Pin_13	

#define   _TESTBIT(var, bit) ((var) &   (1 << (bit)))
#define   _SETBIT(var, bit)  ((var) |=  (1 << (bit)))
#define   _CLRBIT(var, bit)  ((var) &= ~(1 << (bit)))

extern void Key_Init(void);
extern void Key_Check(void);
extern void Keycan_deal(void);

#endif


