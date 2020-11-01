#include "stm32f10x.h"
#include "delay.h"
#include "IIC.h"

static void IIC_Delay(void)
{
	uint8_t i;
	for (i = 0; i < 10; i++);
}


void IIC_Init(void)
{
		IIC_GPIO_Init();
}

void	IIC_GPIO_Init(void)
{
		GPIO_InitTypeDef   GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(IIC_RCC_PORT, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	
		GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	
		GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStructure);
	
		IIC_Stop();
}

void 	IIC_Start(void)
{
		IIC_SDA_1();
		IIC_SCL_1();
		IIC_Delay();
		IIC_SDA_0();
		IIC_Delay();
		IIC_SCL_0();
		IIC_Delay();
}


void IIC_Stop(void)
{
		IIC_SDA_0();
		IIC_SCL_1();
		IIC_Delay();
		IIC_SDA_1();
}


void IIC_ACK(void)
{
		IIC_SDA_0();
		IIC_Delay();
		IIC_SCL_1();
		IIC_Delay();
		IIC_SCL_0();
		IIC_Delay();
		IIC_SDA_1();
}



void IIC_NACK(void)
{
		IIC_SDA_1();
		IIC_Delay();
		IIC_SCL_1();
		IIC_Delay();
		IIC_SCL_0();
		IIC_Delay();
}


void IIC_SendByte(uint8_t data)
{
		uint8_t i;
		for(i = 0; i < 8; i++)
	  {
				if(data & 0x80)
				{
						IIC_SDA_1();
				}
				else
				{
						IIC_SDA_0();
				}
				IIC_Delay();
				IIC_SCL_1();
				IIC_Delay();
				IIC_SCL_0();
				IIC_Delay();
				if(i == 7)
				{
						IIC_SDA_1();
				}
				data <<= 1;
				IIC_Delay();		
	  }
}


uint8_t IIC_Read_Byte(uint8_t ack)
{
		uint8_t i;
		uint8_t value;
		value = 0;
		for(i = 0; i < 8; i++)
	  {
				value <<= 1;
				IIC_SCL_1();
				IIC_Delay();
				if(IIC_SDA_READ())
				{
//						value |= 0x01;
 					value++;
				}
				IIC_SCL_0();
				IIC_Delay();
		}
		if(ack == 0)
			IIC_NACK();
		else	
			IIC_ACK();
		return value;
}


uint8_t IIC_WaitAck(void)
{
		uint8_t re;
		IIC_SDA_1();
		IIC_Delay();
		IIC_SCL_1();
		IIC_Delay();
		if(IIC_SDA_READ())
		{
				re = 1;
		}
		else
		{
				re = 0;
		}
		IIC_SCL_0();
		IIC_Delay();
		return re;
}

uint8_t IIC_CheckDevice(uint8_t Address)
{
		uint8_t ID;
	
		IIC_GPIO_Init();
		IIC_Start();
	
		IIC_SendByte(Address | IIC_WR);
		ID = IIC_WaitAck();
	
		IIC_Stop();
	
		return ID;
}







































