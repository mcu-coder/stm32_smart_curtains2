#include "stm32f10x.h"
#include "delay.h"
#include "dht11.h"

void DHT11_in(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 
}

void DHT11_out(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOB,GPIO_Pin_0); 	
}

void DHT11_rst(void)
{
	DHT11_out();
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	delay_ms(19);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	delay_us(30);
	DHT11_in();
}

u8 DHT11_check(void)
{
	u8 time;
	DHT11_in();
	    while (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)&&time<100)
	{
		time++;
		delay_us(1);
	};	 
	if(time>=100)
		return 1;
	else 
		time=0;
    while (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)&&time<100)
	{
		time++;
		delay_us(1);
	};
	if(time>=100)
		return 1;	    
	return 0;
}

u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)&&retry<100)//µÈ´ý±äÎªµÍµçÆ½
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)&&retry<100)//µÈ´ý±ä¸ßµçÆ½
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//µÈ´ý40us
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))return 1;
	else return 0;		   
}

u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

u8 DHT11_Read_Data(u8 *temp,u8 *temp1,u8 *humi,u8 *humi1)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_rst();
	if(DHT11_check()==0)
	{
		for(i=0;i<5;i++)
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*humi1=buf[1];
			*temp=buf[2];
			*temp1=buf[3];
		}
	}else return 1;
	return 0;	    
}

u8 DHT11_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	DHT11_rst();
	return DHT11_check();
}





