#include "stm32f10x.h"
#include "SPI.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);


void SPI_init(void)
{
		SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	
		SPI_APBxClock_FUN(SPI_CLK, ENABLE);
		SPI_CS_APBxClock_FUN(SPI_CS_CLK | SPI_SCK_CLK | SPI_MISO_CLK | SPI_MOSI_CLK, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
		GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
		GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
	
		FLASH_SPI_CS_HIGH();
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPIx, &SPI_InitStructure);
		
		SPI_Cmd(SPIx, ENABLE);	
}


uint8_t SPI_SendByte(uint8_t byte)
{
		SPITimeout = SPIT_FLAG_TIMEOUT;
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
		{
				if((SPITimeout--) == 0)
					return SPI_TIMEOUT_UserCallback(0);
		}
		SPI_I2S_SendData(SPIx, byte);
		SPITimeout = SPIT_FLAG_TIMEOUT;
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
		{
				if((SPITimeout--) == 0)
					return SPI_TIMEOUT_UserCallback(1);
		}
		return SPI_I2S_ReceiveData(SPIx);
}

static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}





























