#ifndef __SPI_H_
#define	__SPI_H_

#include "stm32f10x.h"
#include <stdio.h>

#define  SPIx                          SPI1
#define  SPI_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define  SPI_CLK                       RCC_APB2Periph_SPI1

#define  SPI_CS_APBxClock_FUN          RCC_APB2PeriphClockCmd
#define  SPI_CS_CLK                    RCC_APB2Periph_GPIOC
#define  SPI_CS_PORT                   GPIOC
#define  SPI_CS_PIN                    GPIO_Pin_0

#define  SPI_SCK_APBxClock_FUN         RCC_APB2PeriphClockCmd
#define  SPI_SCK_CLK                   RCC_APB2Periph_GPIOA
#define  SPI_SCK_PORT                  GPIOA
#define  SPI_SCK_PIN                   GPIO_Pin_5

#define  SPI_MISO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define  SPI_MISO_CLK                  RCC_APB2Periph_GPIOA
#define  SPI_MISO_PORT                 GPIOA
#define  SPI_MISO_PIN                  GPIO_Pin_6

#define  SPI_MOSI_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define  SPI_MOSI_CLK                  RCC_APB2Periph_GPIOA
#define  SPI_MOSI_PORT                 GPIOA
#define  SPI_MOSI_PIN                  GPIO_Pin_7

#define  FLASH_SPI_CS_LOW()             GPIO_ResetBits(SPI_CS_PORT, SPI_CS_PIN)
#define  FLASH_SPI_CS_HIGH()            GPIO_SetBits(SPI_CS_PORT, SPI_CS_PIN)

/*等待超时时间*/
#define  SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define  SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

extern   void SPI_init(void);
extern   uint8_t SPI_SendByte(uint8_t byte);

#endif


