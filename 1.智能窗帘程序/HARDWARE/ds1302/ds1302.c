#include "ds1302.h"
#include "sys.h"
#include "delay.h"
 
struct TIMEData TimeData;
struct TIMERAM TimeRAM;
u8 read_time[7];
 
void ds1302_gpio_init(void)//CE,SCLK端口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PC.11  CE
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOC.11
	GPIO_ResetBits(GPIOA,GPIO_Pin_7); 
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //PC.12  SCLK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOC.12
	GPIO_ResetBits(GPIOA,GPIO_Pin_5); 
}
 
void ds1032_DATAOUT_init()//配置双向I/O端口为输出态
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PC.10  DATA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOC.10
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
}

void ds1032_DATAINPUT_init()//配置双向I/O端口为输入态
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PC.10 DATA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOC.10
}
 
 
void ds1302_write_onebyte(u8 data)//向DS1302发送一字节数据
{
		u8 count=0;
		SCLK_L;
	ds1032_DATAOUT_init();


	for(count=0;count<8;count++)
		{	SCLK_L;
			if(data&0x01)
			{DATA_H;}
			else{DATA_L;}//先准备好数据再发送
			SCLK_H;//拉高时钟线，发送数据
			data>>=1;
		}
}
 
void ds1302_wirte_rig(u8 address,u8 data)//向指定寄存器地址发送数据
{
	u8 temp1=address;
	u8 temp2=data;
	CE_L;SCLK_L;delay_us(1);
	CE_H;delay_us(3);
	ds1302_write_onebyte(temp1);
	ds1302_write_onebyte(temp2);
	CE_L;SCLK_L;delay_us(3);
}
 
u8 ds1302_read_rig(u8 address)//从指定地址读取一字节数据
{
	u8 temp3=address;
	u8 count=0;
	u8 return_data=0x00;
	CE_L;SCLK_L;delay_us(3);
	CE_H;delay_us(3);
	ds1302_write_onebyte(temp3);
	ds1032_DATAINPUT_init();//配置I/O口为输入
	delay_us(3);
	for(count=0;count<8;count++)
		{
		delay_us(3);//使电平持续一段时间
		return_data>>=1;
		SCLK_H;delay_us(5);//使高电平持续一段时间
		SCLK_L;delay_us(30);//延时14us后再去读取电压，更加准确
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))
		{return_data=return_data|0x80;}
		
		}
	delay_us(2);
	CE_L;DATA_L;
	return return_data;
}
 
void ds1032_init(void)
{
	ds1302_wirte_rig(0x8e,0x00);//关闭写保护
	ds1302_wirte_rig(0x80,0x00);//seconds37秒
	ds1302_wirte_rig(0x82,0x30);//minutes58分
	ds1302_wirte_rig(0x84,0x16);//hours23时
	ds1302_wirte_rig(0x86,0x21);//date30日
	ds1302_wirte_rig(0x88,0x12);//months9月
	ds1302_wirte_rig(0x8a,0x04);//days星期日
	ds1302_wirte_rig(0x8c,0x23);//year2020年
	ds1302_wirte_rig(0x8e,0x80);//关闭写保护
}
 
void ds1032_read_time(void)
{
	read_time[0]=ds1302_read_rig(0x81);//读秒
	read_time[1]=ds1302_read_rig(0x83);//读分
	read_time[2]=ds1302_read_rig(0x85);//读时
	read_time[3]=ds1302_read_rig(0x87);//读日
	read_time[4]=ds1302_read_rig(0x89);//读月
	read_time[5]=ds1302_read_rig(0x8B);//读星期
	read_time[6]=ds1302_read_rig(0x8D);//读年
}
 
void ds1032_read_realTime(void)
{
	ds1032_read_time();  //BCD码转换为10进制
	TimeData.second=(read_time[0]>>4)*10+(read_time[0]&0x0f);
	TimeData.minute=((read_time[1]>>4)&(0x07))*10+(read_time[1]&0x0f);
	TimeData.hour=(read_time[2]>>4)*10+(read_time[2]&0x0f);
	TimeData.day=(read_time[3]>>4)*10+(read_time[3]&0x0f);
	TimeData.month=(read_time[4]>>4)*10+(read_time[4]&0x0f);
	TimeData.week=read_time[5];
	TimeData.year=(read_time[6]>>4)*10+(read_time[6]&0x0f)+2000;	
}


void ds1032_wirteRAM(void)
{
	ds1302_wirte_rig(0x8e,0x00);//关闭写保护
	ds1302_wirte_rig(0xC0,TimeRAM.hour_kai);//开时
	ds1302_wirte_rig(0xC2,TimeRAM.minute_kai);//开分
	ds1302_wirte_rig(0xC4,TimeRAM.hour_guan);//关时
	ds1302_wirte_rig(0xC6,TimeRAM.minute_guan);//关分
	ds1302_wirte_rig(0xC8,TimeRAM.kai);//关分
	ds1302_wirte_rig(0xCA,TimeRAM.guan);//关分
	ds1302_wirte_rig(0x8e,0x80);//关闭写保护
}
void ds1032_readRAM(void)
{
	TimeRAM.hour_kai=ds1302_read_rig(0xC1);//读秒
	TimeRAM.minute_kai=ds1302_read_rig(0xC3);//读分
	TimeRAM.hour_guan=ds1302_read_rig(0xC5);//读时
	TimeRAM.minute_guan=ds1302_read_rig(0xC7);//读日	
	TimeRAM.kai=ds1302_read_rig(0xC9);//读日
	TimeRAM.guan=ds1302_read_rig(0xCB);//读日
}
 




