#ifndef __IIC_H_
#define	__IIC_H_

#include "stm32f10x.h"

#define IIC_WR	                 0		
#define IIC_RD	                 1	

#define	IIC_GPIO_PORT	           GPIOB
#define	IIC_RCC_PORT	           RCC_APB2Periph_GPIOB
#define	IIC_SCL_PIN	             GPIO_Pin_6
#define	IIC_SDA_PIN	             GPIO_Pin_7

#if  0

#define	IIC_SCL_1()	             GPIO_SetBits(IIC_GPIO_PORT, IIC_SCL_PIN)
#define	IIC_SCL_0()	             GPIO_ResetBits(IIC_GPIO_PORT, IIC_SCL_PIN)

#define	IIC_SDA_1()	             GPIO_SetBits(IIC_GPIO_PORT, IIC_SDA_PIN)
#define	IIC_SDA_0()	             GPIO_ResetBits(IIC_GPIO_PORT, IIC_SDA_PIN)

#define	IIC_SDA_READ()	         GPIO_ReadInputDataBit(IIC_GPIO_PORT, IIC_SDA_PIN)

#else

#define IIC_SCL_1()              IIC_GPIO_PORT->BSRR = IIC_SCL_PIN				
#define IIC_SCL_0()              IIC_GPIO_PORT->BRR = IIC_SCL_PIN			
	
#define IIC_SDA_1()              IIC_GPIO_PORT->BSRR = IIC_SDA_PIN				
#define IIC_SDA_0()              IIC_GPIO_PORT->BRR = IIC_SDA_PIN				
	
#define IIC_SDA_READ()           ((IIC_GPIO_PORT->IDR & IIC_SDA_PIN) != 0)	


#endif



extern   void    IIC_Init(void);
extern   void	   IIC_GPIO_Init(void);
extern   void 	 IIC_Start(void);
extern   void 	 IIC_Stop(void);
extern   void    IIC_ACK(void);
extern   void    IIC_NACK(void);
extern   void    IIC_SendByte(uint8_t data);
extern   uint8_t IIC_Read_Byte(uint8_t ack);
extern   uint8_t IIC_WaitAck(void);
extern   uint8_t IIC_CheckDevice(uint8_t Address);

#endif

