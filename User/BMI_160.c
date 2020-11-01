#include "stm32f10x.h"
#include "Sysclock.h"
#include "bsp_led.h"
#include "BMI_160.h"
#include "delay.h"
#include "IIC.h"

uint8_t BMI_160_Gyro_data[3];
uint8_t BMI_160_ACC_data[3];
float   BMI_160_ANGLE[3];
float   BMI_160_GRAVITY[3];

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;    
static uint8_t I2C_TIMEOUT_UserCallback(void);

void BMI_160_IIC_INIT(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/*STM32F103C8T6芯片的硬件I2C: PB6 -- SCL; PB7 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C1);//使用I2C1
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x0A;//主机的I2C地址,随便写的
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);	
}

uint8_t BMI_160_WriteReg(u8 Reg_Adress, u8 Red_Data)
{
		#if 0
		IIC_Start();
		IIC_SendByte(BMI_160_SLAVE_ADDRESS);
		IIC_WaitAck();
		IIC_SendByte(Reg_Adress);
		IIC_WaitAck();
		IIC_SendByte(Red_Data);
		IIC_WaitAck();
	  IIC_Stop();
		#else
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
		I2C_GenerateSTART(I2C1, ENABLE);
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
		} 
		
		I2C_Send7bitAddress(I2C1, BMI_160_SLAVE_ADDRESS, I2C_Direction_Transmitter);
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
		} 
		
		I2C_SendData(I2C1, Reg_Adress);
	  I2CTimeout = I2CT_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
		}
		
		I2C_SendData(I2C1, Red_Data); 
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))	
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
		} 
		I2C_GenerateSTOP(I2C1, ENABLE);
		#endif
		return 1;
}

uint8_t BMI_160_ReadRegData(u8* pBuffer, u8 Reg_Adress, u16 NumByteToRead)
{
		#if 0
		uint8_t i;
	  IIC_Start();
		IIC_SendByte(BMI_160_SLAVE_ADDRESS);
		IIC_WaitAck();
		IIC_SendByte(Reg_Adress);
		IIC_WaitAck();
		
		IIC_Start();
		IIC_SendByte(BMI_160_SLAVE_ADDRESS + 1);
		IIC_WaitAck();
		
		for(i=0;i<(NumByteToRead-1);i++)
		{
			*pBuffer=IIC_Read_Byte(1);
			pBuffer++;
		}
		*pBuffer=IIC_Read_Byte(0);
		IIC_Stop();
		#else
		I2CTimeout = I2CT_LONG_TIMEOUT;
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) 
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
		}
    I2C_GenerateSTART(I2C1, ENABLE);
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
		}
		I2C_Send7bitAddress(I2C1, BMI_160_SLAVE_ADDRESS, I2C_Direction_Transmitter);
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
		}
		I2C_Cmd(I2C1, ENABLE);
		I2C_SendData(I2C1, Reg_Adress); 
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
		}
		I2C_GenerateSTART(I2C1, ENABLE);
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
		}
		I2C_Send7bitAddress(I2C1, BMI_160_SLAVE_ADDRESS, I2C_Direction_Receiver);
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
		}
		
		while(NumByteToRead)  
		{
			if(NumByteToRead == 1)
			{
				I2C_AcknowledgeConfig(I2C1, DISABLE);
				I2C_GenerateSTOP(I2C1, ENABLE);
			}
			if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
			{      
				*pBuffer = I2C_ReceiveData(I2C1);
				pBuffer++; 
				NumByteToRead--;        
			}   
		}
		I2C_AcknowledgeConfig(I2C1, ENABLE);
		#endif
		return 1;
}


void BMI_160_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{
	unsigned char i;
	
	IIC_Start();
	IIC_SendByte(BMI_160_SLAVE_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(reg_add);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(BMI_160_SLAVE_ADDRESS + 1);
	IIC_WaitAck();
	
	for(i=0;i<(num-1);i++)
	{
		*Read=IIC_Read_Byte(1);
		Read++;
	}
	*Read=IIC_Read_Byte(0);
	IIC_Stop();
}


void BIM_160_Init(void)
{
		uint8_t a;
		for(a = 0; a < 3; a++)
		{
				BMI_160_Gyro_data[a] = 0;
				BMI_160_ACC_data[a] = 0;
				BMI_160_ANGLE[a] = 0;
				BMI_160_GRAVITY[a] = 0;
		}
		DelayMs(100);
		WriteReg(BMI_160_CMD, 0xB6);
		WriteReg(BMI_160_CMD, ACC_NORMAL_WORK_MODE);
	  WriteReg(BMI_160_CMD, GYR_NORMAL_WORK_MODE);
		WriteReg(BMI_160_ACC_RANGE, 0X03);
		WriteReg(BMI_160_GYR_RANGE, 0X00);
}


void Read_BMI_160_Gyro_data(void)
{
		uint8_t i;
		uint8_t buf[6];
	  #if 0
		buf[0] = BMI_160_ReadRegData(BMI_160_GYR_X_L);
		buf[1] = BMI_160_ReadRegData(BMI_160_GYR_X_H);
		BMI_160_Gyro_data[0]  = (buf[1] << 8) | buf[0];
	
		buf[2] = BMI_160_ReadRegData(BMI_160_GYR_Y_L);
		buf[3] = BMI_160_ReadRegData(BMI_160_GYR_Y_H);
		BMI_160_Gyro_data[1]  = (buf[3] << 8) | buf[2];
	
		buf[4] = BMI_160_ReadRegData(BMI_160_GYR_Z_L);
		buf[5] = BMI_160_ReadRegData(BMI_160_GYR_Z_H);
		BMI_160_Gyro_data[2]  = (buf[5] << 8) | buf[4];
	
		for(i = 0; i < 3; i++)
		{
				BMI_160_ANGLE[i] = (float)((BMI_160_Gyro_data[i])*2000)/0x8000;
		}
	
		#else
		ReadData(BMI_160_GYR_X_L, buf, 6);
    BMI_160_Gyro_data[0] = (buf[0] << 8) | buf[1];
    BMI_160_Gyro_data[1] = (buf[2] << 8) | buf[3];
    BMI_160_Gyro_data[2] = (buf[4] << 8) | buf[5];
		for(i = 0; i < 3; i++)
		{
				BMI_160_ANGLE[i] = (float)((BMI_160_Gyro_data[i])*2000)/0x8000;
		}
		
		#endif
}


void Read_BMI_160_ACC_data(void)
{
		uint8_t i;
		uint8_t buf[6];
	  
		#if 0
		buf[0] = BMI_160_ReadRegData(BMI_160_ACC_X_L);
		buf[1] = BMI_160_ReadRegData(BMI_160_ACC_X_H);
		BMI_160_ACC_data[0]  = (buf[1] << 8) | buf[0];
	
		buf[2] = BMI_160_ReadRegData(BMI_160_ACC_Y_L);
		buf[3] = BMI_160_ReadRegData(BMI_160_ACC_Y_H);
		BMI_160_ACC_data[1]  = (buf[3] << 8) | buf[2];
	
		buf[4] = BMI_160_ReadRegData(BMI_160_ACC_Z_L);
		buf[5] = BMI_160_ReadRegData(BMI_160_ACC_Z_H);
		BMI_160_ACC_data[2]  = (buf[5] << 8) | buf[4];
	
		for(i = 0; i < 3; i++)
		{
				BMI_160_GRAVITY[i] = (float)((BMI_160_ACC_data[i])*9.8)/(0x8000/2);
		}
		#else
		
		ReadData(BMI_160_ACC_X_L, buf, 6);
    BMI_160_ACC_data[0] = (buf[0] << 8) | buf[1];
    BMI_160_ACC_data[1] = (buf[2] << 8) | buf[3];
    BMI_160_ACC_data[2] = (buf[4] << 8) | buf[5];
		
		for(i = 0; i < 3; i++)
		{
				BMI_160_GRAVITY[i] = (float)((BMI_160_ACC_data[i])*9.8)/(0x8000/2);
		}
		
		#endif
}

uint8_t BMI_160_ReadID(void)
{
	unsigned char Re = 0;
/*  Re = BMI_160_ReadRegData(BMI_160_ID);
	if(bsec_base_out)
		{
			  printf("Reg Data = 0x%02X \n", Re); 
		}
	if(Re == 0xD1)
	{
		return 1;
	}
	else
	{
		return 0;
	}	
	*/
	ReadData(BMI_160_ID, &Re, 6);    //读器件地址
	if(b100ms_base_out)
	{
			printf("Reg Data = 0x%02X \n", Re); 
	}
	if(Re != 0xD1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


void Get_BMI_160_Data(void)
{
	  BMI_160_ReadID();
		if(BMI_160_ReadID())
		{
				LED2_ON;
				Read_BMI_160_Gyro_data();
				Read_BMI_160_ACC_data();
			  Sent_BMI_160_USART();
		}
		else
		{
				LED2_TOGGLE;
		}
}


void Sent_BMI_160_USART(void)
{
		if(b100ms_base_out)
		{
		   	printf("Gyro X-axis data = 0x%02X \n", BMI_160_Gyro_data[0]); 
				printf("Gyro X-axis conversion angle = %f\n",BMI_160_ANGLE[0]);
				printf("Gyro Y-axis data = 0x%02X \n", BMI_160_Gyro_data[1]); 
				printf("Gyro Y-axis conversion angle = %f\n",BMI_160_ANGLE[1]);
				printf("Gyro Z-axis data = 0x%02X \n", BMI_160_Gyro_data[2]); 
				printf("Gyro Z-axis conversion angle = %f\n",BMI_160_ANGLE[2]);
			  printf("\n");
			
				printf("Acc X-axis data = 0x%02X \n", BMI_160_ACC_data[0]); 
				printf("Acc X-axis conversion Acceleration = %f\n",BMI_160_GRAVITY[0]);
				printf("Acc Y-axis data = 0x%02X \n", BMI_160_ACC_data[1]); 
				printf("Acc Y-axis conversion Acceleration = %f\n",BMI_160_GRAVITY[1]);
				printf("Acc Z-axis data = 0x%02X \n", BMI_160_ACC_data[2]); 
				printf("Acc Z-axis conversion Acceleration = %f\n",BMI_160_GRAVITY[2]);
				printf("\n");
		}
}

void WriteReg(u8 reg_add,u8 reg_dat)
{
	BMI_160_WriteReg(reg_dat,reg_add); 
}

void ReadData(u8 reg_add,unsigned char* Read,u8 num)
{
	BMI_160_ReadRegData(Read,reg_add,num);
}


static  uint8_t I2C_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
  MPU_ERROR("I2C Timeout error!"); 
  
  return 0;
}























