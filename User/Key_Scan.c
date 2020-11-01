#include "stm32f10x.h"
#include "Key_Scan.h"
#include "Buzz_Souce.h"
#include "Sysclock.h"
#include "Timer.h"
#include "OLED.h"

uint8_t Keysacn[3];

void Key_Init(void)
{
		uint8_t i;
	
		GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd( KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);
	
	  GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);	
	
		GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
		for (i=0;i<5;i++)
		{
			Keysacn[i] = 0;
		}
}

void Key_Check_Press(uint8_t key)
{
		uint8_t Key_temp;
		uint8_t Key_temp_num;
		Key_temp = Keysacn[key];
		Key_temp_num = 0x1f & Key_temp;
	if(_TESTBIT(Key_temp, 7) == 0)
		{
				if(Key_temp_num == 5)
				{
						_SETBIT(Key_temp,7);
						_SETBIT(Key_temp,6);
				}
				else
				{
					Key_temp_num++;
				}

		}
		else
		{
			Key_temp_num = 5;
		}
		
		if(_TESTBIT(Key_temp, 7) != 0)
				_SETBIT(Key_temp_num,7);
		else
				_CLRBIT(Key_temp_num,7);
		
		if(_TESTBIT(Key_temp, 6) != 0)
				_SETBIT(Key_temp_num,6);
		else
				_CLRBIT(Key_temp_num,6);
		
		if(_TESTBIT(Key_temp, 5) != 0)
				_SETBIT(Key_temp_num,5);
		else
				_CLRBIT(Key_temp_num,5);
		
		Keysacn[key] = Key_temp_num;
}


void Key_Check_Release(uint8_t key)
{
	  uint8_t Key_temp;
		uint8_t Key_temp_num;
		Key_temp = key;
		Key_temp_num = 0x1f & Key_temp;
	
		if(_TESTBIT(Key_temp, 7) == 0)
		{
				Key_temp_num = 0;
		}
		else
		{
			Key_temp_num--;
			if(Key_temp_num == 0)
			{
					_CLRBIT(Key_temp,7);
					_SETBIT(Key_temp,5);
			}
		}
		
		if(_TESTBIT(Key_temp, 7) != 0)
				_SETBIT(Key_temp_num,7);
		else
				_CLRBIT(Key_temp_num,7);
		
		if(_TESTBIT(Key_temp, 6) != 0)
				_SETBIT(Key_temp_num,6);
		else
				_CLRBIT(Key_temp_num,6);
		
		if(_TESTBIT(Key_temp, 5) != 0)
				_SETBIT(Key_temp_num,5);
		else
				_CLRBIT(Key_temp_num,5);

		Keysacn[key] = Key_temp_num;
}


void Key_Check(void)
{
		if(GPIO_ReadInputDataBit(GPIOA, KEY1_GPIO_PIN) == KEY_ON)
			Key_Check_Press(KEY1);
		else
			Key_Check_Release(KEY1);
		if(GPIO_ReadInputDataBit(GPIOC, KEY2_GPIO_PIN) == KEY_ON)
			Key_Check_Press(KEY2);
		else
			Key_Check_Release(KEY2);
		if(TPAD_Scan() == TPAD_ON)
		{
			flash_finish = 0;
			Key_Check_Press(KEY3);
		}
		else
			Key_Check_Release(KEY3);		
}

void First_press(uint8_t key)
{
		switch(key)
		{
			case KEY1:
				 flash_finish = 0;
				 	OLED_ShowStr(0,2,(unsigned char*)"liang",1);
					LED2_ON;
			
			break;
			
			case KEY2:
				  OLED_ShowStr(0,3,(unsigned char*)"fei fan",1);

				
			break;
			
			case KEY3:
					flash_finish = 0;
				
			break;
		}
}

void Keep_press(uint8_t key)
{
		switch(key)
		{
			case KEY1:
				
			break;
			
			case KEY2:
				
			break;
			
			case KEY3:
				
			break;
		}
}

void Release_press(uint8_t key)
{
	switch(key)
		{
			case KEY1:
				LED2_OFF;
			
			break;
			
			case KEY2:
				  
				
			break;
			
			case KEY3:
				
			break;
		}
	key = 0;
}

void Keycan_deal(void)
{
		uint8_t Keys_temp;
	  uint8_t i;
		for(i = 0; i < 5; i++)
		{
			 Keys_temp = Keysacn[i];
			 if(_TESTBIT(Keys_temp, 7) != 0)
			 {
					if(_TESTBIT(Keys_temp, 6) != 0)
					{
						 _CLRBIT(Keys_temp, 6);
						 First_press(i);
					}
					Keep_press(i);
			 }
			 if(_TESTBIT(Keys_temp, 5) != 0)
			 {
				  _CLRBIT(Keys_temp, 5);
					Release_press(i);
			 }
			 Keysacn[i] = Keys_temp;
	 }
}







