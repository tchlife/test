#include "stm32f10x.h"
#include "Buzz_Souce.h"
#include "Sysclock.h"

uint8_t  buzz_frequency;
uint8_t  flash_time;
uint8_t  flash_finish;


void Buzz_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(BUZZ_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BUZZ_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BUZZ_GPIO_PORT, &GPIO_InitStructure);
	
	buzz_frequency = 0;
	flash_time = 0;
	flash_finish = 1;
}


void Buzz_flash_time(uint8_t time, uint8_t speed)
{
	if(flash_time < time && flash_finish == 0)
	{
		switch(speed)
		{
			case QUICK:
				if(b50ms_base_out)
				{
					buzz_frequency++;
					if(buzz_frequency > 4)
					{
						buzz_frequency = 0;
						flash_time++;
					}
					if(buzz_frequency < 2)
					{
						BUZZ_ON;
					}
					else
					{
						BUZZ_OFF;
					}
				}
			break;
			
			case MIDDLE:
			if(b50ms_base_out)
				{
					buzz_frequency++;
					if(buzz_frequency > 10)
					{
						buzz_frequency = 0;
						flash_time++;
					}
					if(buzz_frequency < 5)
					{
						BUZZ_ON;
					}
					else
					{
						BUZZ_OFF;
					}
				}
			break;
			
			case SLOW:
			if(b50ms_base_out)
				{
					buzz_frequency++;
					if(buzz_frequency > 16)
					{
						buzz_frequency = 0;
						flash_time++;
					}
					if(buzz_frequency < 8)
					{
						BUZZ_ON;
					}
					else
					{
						BUZZ_OFF;
					}
				}
			break;
		}
	}
	else
	{
		flash_time = 0;
		flash_finish = 1;
	}
}

