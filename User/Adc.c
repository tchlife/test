#include "stm32f10x.h"
#include "Adc.h"
#include "Sysclock.h"
#include "Buzz_Souce.h"

__IO    uint16_t   ADC_ConvertValue[5];
float    ADC_ConvertedValueLocal[5];


void ADC_init(void)
{
		uint8_t i;
		for(i = 0; i < 5; i++)
		{
			ADC_ConvertValue[i] = 0;
			ADC_ConvertedValueLocal[i] = 0;
		}
		ADCx_GPIO_Init();
		ADCx_Mode_Init();
    #ifdef    single
		ADC_NVIC_init();
		#endif
		
}

#ifdef    single
void ADCx_GPIO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = ADC_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(ADC_PORT, &GPIO_InitStructure);
}

void ADCx_Mode_Init(void)
{
		ADC_InitTypeDef ADC_InitStructure;
		ADC_APBxClock_FUN(ADC_CLK, ENABLE);
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 1;
		ADC_Init(ADCx, &ADC_InitStructure);
	
		RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
	  ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);
		ADC_Cmd(ADCx, ENABLE);
		ADC_ResetCalibration(ADCx);
		while(ADC_GetResetCalibrationStatus(ADCx));
		ADC_StartCalibration(ADCx);
		while(ADC_GetCalibrationStatus(ADCx));
		ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}


void ADC_NVIC_init(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		
		NVIC_Init(&NVIC_InitStructure);
}

void ADC_IRQHandler(void)
{
		if(ADC_GetITStatus(ADCx, ADC_IT_EOC) == SET)
		{
				ADC_ConvertValue[0] = ADC_GetConversionValue(ADCx);
		}
		ADC_ClearITPendingBit(ADCx, ADC_IT_EOC);
}

#else

void ADCx_GPIO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = ADC_PIN1 | ADC_PIN2 | ADC_PIN3 | ADC_PIN4 | ADC_PIN5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(ADC_PORT, &GPIO_InitStructure);
}

void ADCx_Mode_Init(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;
	
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		ADC_APBxClock_FUN(ADCx_CLK, ENABLE);
		DMA_DeInit(ADC_DMA_CHANNEL);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(ADCx->DR));
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertValue;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = NOFCHANEL;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
		DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
	
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;
		ADC_Init(ADCx, &ADC_InitStructure);
		RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		ADC_RegularChannelConfig(ADCx, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
		ADC_RegularChannelConfig(ADCx, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
		ADC_RegularChannelConfig(ADCx, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
		ADC_RegularChannelConfig(ADCx, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
		ADC_RegularChannelConfig(ADCx, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
		ADC_DMACmd(ADCx, ENABLE);
		ADC_Cmd(ADCx, ENABLE);
		ADC_Cmd(ADCx, ENABLE);
		ADC_ResetCalibration(ADCx);
		while(ADC_GetResetCalibrationStatus(ADCx));
		ADC_StartCalibration(ADCx);
		while(ADC_GetCalibrationStatus(ADCx));
		ADC_SoftwareStartConvCmd(ADCx, ENABLE);	
}
#endif


void ADC_SenUsart(void)
{
		#ifdef    single
		ADC_ConvertedValueLocal[0] = (float)ADC_ConvertValue[0]/4096*3.3;
		if(bsec_base_out)
		{ 
				printf("\r\n The current AD value = 0x%04X \n", ADC_ConvertValue[0]); 
				printf("\r\n The current AD value = %f V \n",ADC_ConvertedValueLocal[0]); 
				printf("\r\n");
		}
	  #else
		ADC_ConvertedValueLocal[0] = (float)ADC_ConvertValue[0]/4096*3.3;
		ADC_ConvertedValueLocal[1] = (float)ADC_ConvertValue[1]/4096*3.3;
		ADC_ConvertedValueLocal[2] = (float)ADC_ConvertValue[2]/4096*3.3;
		ADC_ConvertedValueLocal[3] = (float)ADC_ConvertValue[3]/4096*3.3;
		ADC_ConvertedValueLocal[4] = (float)ADC_ConvertValue[4]/4096*3.3;
		if(b50ms_base_out)
		{
				Usart_SendByte(DEBUG_USARTx, 0xAA);
				Usart_SendByte(DEBUG_USARTx, ADC_ConvertedValueLocal[0]/256);
				Usart_SendByte(DEBUG_USARTx, ADC_ConvertedValueLocal[0]%256);
				Usart_SendByte(DEBUG_USARTx, 0xBB);
				Usart_SendByte(DEBUG_USARTx, ADC_ConvertedValueLocal[1]/256);
				Usart_SendByte(DEBUG_USARTx, ADC_ConvertedValueLocal[1]%256);
				Usart_SendByte(DEBUG_USARTx, 0xCC);
				Usart_SendByte(DEBUG_USARTx, ADC_ConvertedValueLocal[2]/256);
				Usart_SendByte(DEBUG_USARTx, ADC_ConvertedValueLocal[2]%256);
				Usart_SendByte(DEBUG_USARTx, 0xDD);
				Usart_SendByte(DEBUG_USARTx, ADC_ConvertedValueLocal[3]/256);
				Usart_SendByte(DEBUG_USARTx, ADC_ConvertedValueLocal[3]%256);
				Usart_SendByte(DEBUG_USARTx, 0xEE);
				Usart_SendByte(DEBUG_USARTx, ADC_ConvertedValueLocal[4]/256);
				Usart_SendByte(DEBUG_USARTx, ADC_ConvertedValueLocal[4]%256);
		}
		#endif
}
	

























