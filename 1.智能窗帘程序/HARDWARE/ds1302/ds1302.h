#ifndef __ds1302_H 
#define __ds1302_H  
 
#include "sys.h" 
 
#define CE_L GPIO_ResetBits(GPIOA,GPIO_Pin_5)//拉低使能位
#define CE_H GPIO_SetBits(GPIOA,GPIO_Pin_5)//拉高使能位
#define SCLK_L GPIO_ResetBits(GPIOA,GPIO_Pin_7)//拉低时钟线
#define SCLK_H  GPIO_SetBits(GPIOA,GPIO_Pin_7)//拉高时钟线
#define DATA_L  GPIO_ResetBits(GPIOA,GPIO_Pin_6)//拉低数据线
#define DATA_H  GPIO_SetBits(GPIOA,GPIO_Pin_6)//拉高数据线
 
struct TIMEData
{
	u16 year;
	u8  month;
	u8  day;
	u8  hour;
	u8  minute;
	u8  second;
	u8  week;
};//创建TIMEData结构体方便存储时间日期数据

struct TIMERAM
{
	u8  hour_kai;
	u8  minute_kai;
	u8  hour_guan;
	u8  minute_guan;
	u8  kai;
	u8  guan;
};//创建TIMEData结构体方便存储时间日期数据

extern struct TIMERAM TimeRAM;
extern struct TIMEData TimeData;//全局变量
void ds1302_gpio_init(void);//ds1302端口初始化
void ds1302_write_onebyte(u8 data);//向ds1302发送一字节数据
void ds1302_wirte_rig(u8 address,u8 data);//向指定寄存器写一字节数据
u8 ds1302_read_rig(u8 address);//从指定寄存器读一字节数据
void ds1032_init(void);//ds1302初始化函数
void ds1032_DATAOUT_init(void);//IO端口配置为输出
void ds1032_DATAINPUT_init(void);//IO端口配置为输入
void ds1032_read_time(void);//从ds1302读取实时时间（BCD码）
void ds1032_read_realTime(void);//将BCD码转化为十进制数据
void ds1032_wirteRAM(void);
void ds1032_readRAM(void);
 
#endif






