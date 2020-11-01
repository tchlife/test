#ifndef __DATA_COPE_H_
#define	__DATA_COPE_H_

#include "stm32f10x.h"


extern uint8_t DataScope_OutPut_Buffer[42];	   


void DataScope_Get_Channel_Data(float Data,unsigned char Channel);    

uint8_t DataScope_Data_Generate(unsigned char Channel_Number); 

void Sent_Data(void);


#endif


