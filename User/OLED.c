#include "OLED.h"
#include "delay.h"
#include "codetab.h"


void OLED_GPIO_INIT(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/*STM32F103C8T6芯片的硬件I2C: PB10 -- SCL; PB11 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C2);//使用I2C2
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//主机的I2C地址,随便写的
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

	I2C_Cmd(I2C2, ENABLE);
	I2C_Init(I2C2, &I2C_InitStructure);
}

void OLED_wtiteByte(uint8_t addr,uint8_t data)
{
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C2, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	I2C_Send7bitAddress(I2C2, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C2, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C2, data);//发送数据
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C2, ENABLE);//关闭I2C1总线
}

void  OLED_wtiteCmd(unsigned char I2C_Command)
{
		OLED_wtiteByte(0x00, I2C_Command);
}

void  OLED_wtiteData(unsigned char I2C_Data)
{
		OLED_wtiteByte(0x40, I2C_Data);
}

void OLED_Init(void)
{
	DelayMs(100); //这里的延时很重要
	
	OLED_wtiteCmd(0xae); //display off
	OLED_wtiteCmd(0x20); //Set Memory Addressing Mode	
	OLED_wtiteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_wtiteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
	OLED_wtiteCmd(0xc8); //Set COM Output Scan Direction
	OLED_wtiteCmd(0x00); //---set low column address
	OLED_wtiteCmd(0x10); //---set high column address
	OLED_wtiteCmd(0x40); //--set start line address
	OLED_wtiteCmd(0x81); //--set contrast control register
	OLED_wtiteCmd(0xff); //亮度调节 0x00~0xff
	OLED_wtiteCmd(0xa1); //--set segment re-map 0 to 127
	OLED_wtiteCmd(0xa6); //--set normal display
	OLED_wtiteCmd(0xa8); //--set multiplex ratio(1 to 64)
	OLED_wtiteCmd(0x3F); //
	OLED_wtiteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_wtiteCmd(0xd3); //-set display offset
	OLED_wtiteCmd(0x00); //-not offset
	OLED_wtiteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	OLED_wtiteCmd(0xf0); //--set divide ratio
	OLED_wtiteCmd(0xd9); //--set pre-charge period
	OLED_wtiteCmd(0x22); //
	OLED_wtiteCmd(0xda); //--set com pins hardware configuration
	OLED_wtiteCmd(0x12);
	OLED_wtiteCmd(0xdb); //--set vcomh
	OLED_wtiteCmd(0x20); //0x20,0.77xVcc
	OLED_wtiteCmd(0x8d); //--set DC-DC enable
	OLED_wtiteCmd(0x14); //
//	OLED_wtiteCmd(0x2f);
	OLED_wtiteCmd(0xaf); //--turn on oled panel
	
	OLED_Fill(0x00);
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	OLED_wtiteCmd(0xb0+y);
	OLED_wtiteCmd(((x&0xf0)>>4)|0x10);
	OLED_wtiteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_wtiteCmd(0xb0+m);		//page0-page1
		OLED_wtiteCmd(0x00);		//low column start address
		OLED_wtiteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_wtiteData(fill_Data);
			}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

void OLED_ON(void)
{
	OLED_wtiteCmd(0X8D);  //设置电荷泵
	OLED_wtiteCmd(0X14);  //开启电荷泵
	OLED_wtiteCmd(0XAF);  //OLED唤醒
}

void OLED_OFF(void)
{
	OLED_wtiteCmd(0X8D);  //设置电荷泵
	OLED_wtiteCmd(0X10);  //关闭电荷泵
	OLED_wtiteCmd(0XAE);  //OLED休眠
}

void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					OLED_wtiteData(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					OLED_wtiteData(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					OLED_wtiteData(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_wtiteData(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_wtiteData(F16x16[adder]);
		adder += 1;
	}
}

void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			OLED_wtiteData(BMP[j++]);
		}
	}
}


