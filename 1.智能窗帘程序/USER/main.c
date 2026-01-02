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
	KEY_Init(); 
	OLED_Init();                                  //屏幕初始化
 
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
            
             
//            MQ_res = MQ_Scan();
            if(MQ==0)
        {
            LED0 = !LED0;
            delay_ms(100);
            LED0 = !LED0;
            delay_ms(100);
        }
    }
    
		 
  }
}
 
 


