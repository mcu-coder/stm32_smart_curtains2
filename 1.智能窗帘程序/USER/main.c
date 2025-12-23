#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
#include "usart3.h"
#include "OLED_I2C.h"
#include "math.h"
#include "ds1302.h"
#include "dht11.h"
#include "bjdj.h"
#include "adc.h"
#include "stdio.h"

#define Num_i 256 //电机步数


u16 Flag_adc=0;
u8 Num_wendu1,Num_wendu2,Num_shidu1,Num_shidu2;
extern u8 Flag_caidan;
extern u8 Flag_dakai,dakai,Flag_bankai,bankai,Flag_zhanting;
extern u8	st[50];	
int MQ_res = 0;
    
int main(void)
{	
  u16 ii=0;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  uart_init(9600);          //语音模块串口初始化
	usart3_init(9600);                 //蓝牙使用
	DHT11_Init();              
	Adc_Init();
	Moto_Init();
	ds1302_gpio_init();	
	//ds1032_init();						//时间设置
  ds1032_read_realTime();   //读取实时数据 
	KEY_Init();
    MQ_Init();
    LED_Init();
	OLED_Init();                                  //屏幕初始化
  TIM3_Int_Init(719,7199); 
	printf("AF:30");//声音调到31档   最大
	delay_ms(100);
    delay_ms(1000);
	printf("A7:00003");//欢迎使用
	delay_ms(1000);
	OLED_CLS();

    
  while(1)
  {
    if(Flag_caidan==1)//自动模式
		{
			delay_ms(500);
			Flag_adc=Get_Adc_Average(0,10);
			delay_ms(500);
            
            
            if(Flag_adc>1000&&Flag_dakai==0)
            {
                printf("A7:00001");//打开
                TIM_Cmd(TIM3,DISABLE); 
                for(ii=0;ii<Num_i;ii++)
                {
                    Motorcw_angle(1,3);		
                    MotorStop(); 
                }
                TIM_Cmd(TIM3, ENABLE); 
        Flag_dakai=1;
                dakai=1;
            }
            if(Flag_adc<1000&&Flag_dakai==1)
            {
        printf("A7:00002");//关闭
        TIM_Cmd(TIM3,DISABLE); 
        for(ii=0;ii<Num_i;ii++)
        {
          Motorccw_angle(1,3);		
          MotorStop(); 
        } 
        TIM_Cmd(TIM3, ENABLE); 
        Flag_dakai=0;
                dakai=0;
        
        
            

		}
//            MQ_res = MQ_Scan();
            if(MQ==0)
        {
            LED0 = !LED0;
            delay_ms(100);
            LED0 = !LED0;
            delay_ms(100);
        }
    }
    if(Flag_caidan==2)//手动模式
    {
      if(dakai==1&&Flag_dakai==0)
      {
        printf("A7:00006");//打开
        TIM_Cmd(TIM3,DISABLE); 
        for(ii=0;ii<Num_i;ii++)
        {
          Motorcw_angle(1,3);		
          MotorStop(); 
        }
				TIM_Cmd(TIM3, ENABLE); 
        Flag_dakai=1;
      }
      if(dakai==0&&Flag_dakai==1)
      {
        printf("A7:00007");//关闭
        TIM_Cmd(TIM3,DISABLE); 
        for(ii=0;ii<Num_i;ii++)
        {
          Motorccw_angle(1,3);		
          MotorStop(); 
        }
				TIM_Cmd(TIM3, ENABLE); 
        Flag_dakai=0;
      }
    }
		if(Flag_caidan==3)//定时模式
		{
			if(TimeRAM.kai==1&&Flag_dakai==0)
			{
				if(TimeRAM.hour_kai==TimeData.hour&&TimeRAM.minute_kai==TimeData.minute)
				{
					printf("A7:00006");//打开
					TIM_Cmd(TIM3,DISABLE); 
					for(ii=0;ii<Num_i;ii++)
					{
						Motorcw_angle(1,3);		
						MotorStop(); 
					}
					TIM_Cmd(TIM3, ENABLE); 
					Flag_dakai=1;
				}
			}
			if(TimeRAM.guan==1&&Flag_dakai==1)
			{
				if(TimeRAM.hour_guan==TimeData.hour&&TimeRAM.minute_guan==TimeData.minute)
        {
					printf("A7:00007");//关闭
					TIM_Cmd(TIM3,DISABLE); 
					for(ii=0;ii<Num_i;ii++)
					{
						Motorccw_angle(1,3);		
						MotorStop(); 
					}
					TIM_Cmd(TIM3, ENABLE); 
					Flag_dakai=0;
				}
      }
		}
		
		if(Flag_caidan==4)//遥控模式
		{
	
		 if(dakai==1&&Flag_dakai==0)
      {
        printf("A7:00006");//打开
        TIM_Cmd(TIM3,DISABLE);
				ii=0;
        for(ii=0;ii<Num_i;ii++)
        {
          Motorcw_angle(1,3);		
          MotorStop(); 
        }
				TIM_Cmd(TIM3, ENABLE); 
        Flag_dakai=1;
      }
      if(dakai==0&&Flag_dakai==1)
      {
        printf("A7:00007");//关闭
        TIM_Cmd(TIM3,DISABLE); 
        for(ii=0;ii<Num_i;ii++)
        {
          Motorccw_angle(1,3);		
          MotorStop(); 
        }
				TIM_Cmd(TIM3, ENABLE); 
        Flag_dakai=0;
      }	
		}
  }
}
 
 


