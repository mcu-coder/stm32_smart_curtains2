#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"





#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//读取按键5
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//读取按键6
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取按键7
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//读取按键8
#define MQ    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//读取按键8

#define KEY1_PRES 	1	//KEY1按下
#define KEY2_PRES	  2	//KEY2按下
#define KEY3_PRES 	3	//KEY3按下
#define KEY4_PRES	  4	//KEY4按下

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数					    
u8 MQ_Scan(void);
void MQ_Init(void) ;
#endif
