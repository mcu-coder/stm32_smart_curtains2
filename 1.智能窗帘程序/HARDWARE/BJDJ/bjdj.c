#include "bjdj.h"
#include "delay.h"

#define uint unsigned int
	
//步进电机正反转数组1
//uint16_t phasecw[4] ={0x0200,0x0100,0x0080,0x0040};// D-C-B-A    9 8 7 6 5 4 3 2 1 0
//uint16_t phaseccw[4]={0x0040,0x0080,0x0100,0x0200};// A-B-C-D.

void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}

void Moto_Init(void)
{
	//电机1
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 ;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 GPIO_ResetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 );
// IN4: P89   d
// IN3: PB8   c 
// IN2: PB5   b
// IN1: PB4   a
}

void Motorcw(uint speed)  
{  
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
	delay_ms(speed);  
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
	delay_ms(speed);  
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
	delay_ms(speed);  
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
	delay_ms(speed);  
}



void Motorccw(int speed)  
{  
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
	delay_ms(speed);  
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
	delay_ms(speed);  
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
	delay_ms(speed);  
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
	delay_ms(speed);  
}


void MotorStop(void)  
{ 
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
}



//由于   *一个脉冲*   *输出轴*  转0.08789度（电机实转0.08789*64=5.625度），即步进角为5.625度。
//则转完A-B-C-D为  *8个脉冲*  ，即0.08789*8=0.70312度。若称A-B-C-D为一个周期，则j为需要的转完angle角度所需的周期数。
void Motorcw_angle(int angle,int speed)
{
	int i,j;
	j=angle;
	for(i=0;i<j;i++)
	{
		Motorcw(5);
	}
}



void Motorccw_angle(int angle,int speed)
{
	int i,j;
	j=angle;
	for(i=0;i<j;i++)
	{
		Motorccw(5);
	}
		MotorStop(); 
}

