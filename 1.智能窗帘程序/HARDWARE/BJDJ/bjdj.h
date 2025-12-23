#ifndef __bjdj_H
#define __bjdj_H	 
#include "sys.h"

//ULN2003驱动

#define uint unsigned int

void Delay_xms(uint x);//延时函数
void Moto_Init(void);  //步进电机初始化
void Motorcw(uint speed);    //步进电机正转函数
void Motorccw(int speed);   //步进电机反转函数
void Motorcw_angle(int angle,int speed);  //步进电机正转角度函数
void Motorccw_angle(int angle,int speed); //步进电机反转角度函数
void MotorStop(void);  //步进电机停止函数

#endif
