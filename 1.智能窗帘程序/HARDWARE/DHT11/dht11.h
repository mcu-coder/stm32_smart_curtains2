#ifndef __DHT11_H__
#define	__DHT11_H__
#include "stm32f10x.h"
#include "delay.h"

u8 DHT11_Init(void);
u8 DHT11_Read_Data(u8 *temp,u8 *temp1,u8 *humi2,u8 *humi);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_check(void);
void DHT11_rst(void);

#endif
