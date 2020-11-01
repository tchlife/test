#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp_led.h"
#include "Sysclock.h"
#include "Buzz_Souce.h"
#include "Key_Scan.h"
#include "Timer.h"
#include "Adc.h"
#include "delay.h"
#include "IIC.h"
#include "BMI_160.h"
#include "stdio.h"
#include "SPI.h"
#include "OLED.h"
//#include "codetab.h"

int main(void)
{	
    IWDG_init(IWDG_Prescaler_64, 625);
		Usart_init();
		SysTick_Init();
		Timer_init();
		LED_GPIO_Config();
		Base_time_init();
		Buzz_Init();
		Key_Init();
	  ADC_init();
	  IIC_Init();
	  SPI_init();
	  DelayInit();
	  BIM_160_Init();
		BMI_160_IIC_INIT();
//	while( TPAD_Init());
		OLED_GPIO_INIT();
 	  OLED_Init();
	  Sent_ID();
	while (1)
	{
		  Clear_IWDG();
			
			/**********************************
			Change this Yes it is Very good
		  ************************************/
		
			Time_Check();
		
			Get_BMI_160_Data();
		
		  Time_Clear();
	}
}

/*********************************************END OF FILE**********************/
