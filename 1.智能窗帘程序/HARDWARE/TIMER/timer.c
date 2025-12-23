#include "stdio.h"
#include "timer.h"
#include "ds1302.h"
#include "led.h"
#include "OLED_I2C.h"
#include "key.h"
#include "delay.h"
#include "dht11.h"


u8 DDD=0;
u8 Flag_caidan=0;
u8 Flag_shoudong=0;
u8 Flag_dakai=0,dakai=0,Flag_bankai=0,bankai=0,Flag_dingshi=0;
extern u8 Num_wendu1,Num_wendu2,Num_shidu1,Num_shidu2;
extern u16 adcx;



void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}

//定时器3中断服务程序，定时1ms
void TIM3_IRQHandler(void)//
{
	u8 key=0;
	//检查TIM3更新中断发生与否
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//
	{
		DDD++;
		if(DDD==5)
		{
			caidan();
			DDD=0;
		}
		key=KEY_Scan(0);
		switch(key)
		{
			case 1://+1
			{
				if(Flag_dingshi==0)
				{	
					OLED_clear(0x00);
					Flag_caidan++;
					if(Flag_caidan==5)
						{
							Flag_caidan=0;
						}
				}
				if(Flag_dingshi==3&&Flag_dingshi!=0)
				{
					Flag_dingshi++;				
				}
			key=0;
			}break;
			case 2://-1
			{
				if(Flag_caidan==2)
        {
           dakai=1;
        }
				if(Flag_caidan==3&&Flag_dingshi==1)
				{
					TimeRAM.hour_kai++;
					if(TimeRAM.hour_kai==24)
					{
						TimeRAM.hour_kai=0;
					}
					ds1032_wirteRAM();
				}
				if(Flag_caidan==3&&Flag_dingshi==2)
				{
					TimeRAM.minute_kai++;
					if(TimeRAM.minute_kai==60)
					{
						TimeRAM.minute_kai=0;
					}
					ds1032_wirteRAM();
				}
				if(Flag_caidan==3&&Flag_dingshi==4)
				{
					TimeRAM.hour_guan++;
					if(TimeRAM.hour_guan==24)
					{
						TimeRAM.hour_guan=0;
					}
					ds1032_wirteRAM();
				}
				if(Flag_caidan==3&&Flag_dingshi==5)
				{
					TimeRAM.minute_guan++;
					if(TimeRAM.minute_guan==60)
					{
						TimeRAM.minute_guan=0;
					}
					ds1032_wirteRAM();
				}
				if(Flag_dingshi==3)
				{
					TimeRAM.kai=1;
					ds1032_wirteRAM();
				}
				if(Flag_dingshi==6)
				{
					TimeRAM.guan=1;
					ds1032_wirteRAM();
				}
				key=0;
			}break;
			case 3://下一个
			{
				if(Flag_caidan==2)
        {
           dakai=0;
        }
				if(Flag_caidan==3&&Flag_dingshi==1)
				{
					TimeRAM.hour_kai--;
					if(TimeRAM.hour_kai>24)
					{
						TimeRAM.hour_kai=0;
					}
					ds1032_wirteRAM();
				}
				if(Flag_caidan==3&&Flag_dingshi==2)
				{
					TimeRAM.minute_kai--;
					if(TimeRAM.minute_kai>60)
					{
						TimeRAM.minute_kai=0;
					}
					ds1032_wirteRAM();
				}
				if(Flag_caidan==3&&Flag_dingshi==4)
				{
					TimeRAM.hour_guan--;
					if(TimeRAM.hour_guan>24)
					{
						TimeRAM.hour_guan=0;
					}
					ds1032_wirteRAM();
				}
				if(Flag_caidan==3&&Flag_dingshi==5)
				{
					TimeRAM.minute_guan--;
					if(TimeRAM.minute_guan>60)
					{
						TimeRAM.minute_guan=0;
					}
					ds1032_wirteRAM();
				}
				if(Flag_dingshi==3)
				{
					TimeRAM.kai=0;
					ds1032_wirteRAM();
				}
				if(Flag_dingshi==6)
				{
					TimeRAM.guan=0;
					ds1032_wirteRAM();
				}
				key=0;
      }break;
			case 4://进入定时
			{
				if(Flag_caidan==3)
				{
					OLED_CLS();
					Flag_dingshi++;
				}
			key=0;
			}break;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
	
}
 /*宠(0) 物(1) 喂(2) 食(3) 器(4) 闹(5) 钟(6) 设(7) 置(8) ①(9) ②(10) ③(11) 星(12) 期(13) 一(14) 二(15)
 三(16) 四(17) 五(18) 六(19) 天(20) 开(21) 关(22) 年(23) 月(24) 日(25)  自(30) 动(31) 模(32) 式(33) 手(34) 动(35) 定(36)
时(37) 打(38) 开(39) 关(40) 闭(41) 时(42) 间(43)
*/
char st[50];	
void caidan(void)
{
    char ch[]={127};   
		switch(Flag_caidan)
		{
			case 0:
			{
				Flag_caidan=1;
			}break;
			case 1://主界面
			{
				ds1032_read_realTime();    //读取此时时刻
				sprintf(st,"%d  ",TimeData.year);
				OLED_ShowStr(8,0,st,2);
				OLED_ShowCN(40,0,23);//年
				sprintf(st,"%d  ",TimeData.month);
				OLED_ShowStr(56,0,st,2);
				OLED_ShowCN(72,0,24);//月
				sprintf(st,"%d  ",TimeData.day);
				OLED_ShowStr(88,0,st,2);
				OLED_ShowCN(104,0,25);//日
				sprintf(st,"%d  ",TimeData.hour);
				OLED_ShowStr(48+8,2,st,2);
				OLED_ShowStr(48+8+16,2,"-",2);
				sprintf(st,"%d  ",TimeData.minute);
				OLED_ShowStr(48+8+16+8,2,st,2);
				OLED_ShowStr(48+8+16+16+8,2,"-",2);
				sprintf(st,"%d  ",TimeData.second);
				OLED_ShowStr(48+8+16+16+16,2,st,2);
				OLED_ShowCN(0,2,12);//星期
				OLED_ShowCN(16,2,13);
				
				OLED_ShowCN(0,4,26);//温度：
				OLED_ShowCN(16,4,27);
				OLED_ShowStr(32,4,":",2);
				OLED_ShowStr(56,4,ch,2);
				OLED_ShowCN(64,4,28);//湿度：
				OLED_ShowCN(80,4,29);
				OLED_ShowStr(96,4,":",2);
				OLED_ShowStr(120,4,"%",2);
				
				DHT11_Read_Data(&Num_wendu1,&Num_wendu2,&Num_shidu1,&Num_shidu2);    
				sprintf(st,"%d",Num_wendu1);
				OLED_ShowStr(40,4,st,2);
				sprintf(st,"%d",Num_shidu1);
				OLED_ShowStr(104,4,st,2);
				switch(TimeData.week)
				{
					case 1:
					{
						OLED_ShowCN(32,2,14);
					}break;
					case 2:
					{
						OLED_ShowCN(32,2,15);
					}break;
					case 3:
					{
						OLED_ShowCN(32,2,16);
					}break;
					case 4:
					{
						OLED_ShowCN(32,2,17);
					}break;
					case 5:
					{
						OLED_ShowCN(32,2,18);
					}break;
					case 6:
					{
						OLED_ShowCN(32,2,19);
					}break;
					case 7:
					{
						OLED_ShowCN(32,2,20);
					}break;
				}
					Flag_bankai=0;
					OLED_ShowCN(32,6,30);//自动模式
					OLED_ShowCN(48,6,31);
					OLED_ShowCN(64,6,32);
					OLED_ShowCN(80,6,33);
					OLED_ShowStr(112,6,"  ",2);
			}break;
			case 2:
			{
				OLED_ShowCN(32,6,34);//手动模式
        OLED_ShowCN(48,6,35);
        OLED_ShowCN(64,6,32);
        OLED_ShowCN(80,6,33);	
	
			}break;
			case 3:
			{
				if(Flag_dingshi==1)
				{
					ds1032_readRAM();
					OLED_ShowCN(32,0,38);
					OLED_ShowCN(48,0,39);//打开时间
					OLED_ShowCN(64,0,42);
					OLED_ShowCN(80,0,43);
					sprintf(st,"%d  ",TimeRAM.hour_kai);
					OLED_ShowStr(32,4,st,2);
					OLED_ShowCN(48,4,42);//时
					sprintf(st,"%d  ",TimeRAM.minute_kai);
					OLED_ShowStr(64,4,st,2);
					OLED_ShowCN(80,4,44);//分
					if(TimeRAM.kai==0)
					OLED_ShowCN(112,6,22);
					else
					OLED_ShowCN(112,6,21);	
					delay_ms(100);
					OLED_ShowStr(32,4,"  ",2);
				}
				else if(Flag_dingshi==2)
				{
					ds1032_readRAM();
					ds1032_readRAM();
					OLED_ShowCN(32,0,38);
					OLED_ShowCN(48,0,39);//打开时间
					OLED_ShowCN(64,0,42);
					OLED_ShowCN(80,0,43);
					sprintf(st,"%d  ",TimeRAM.hour_kai);
					OLED_ShowStr(32,4,st,2);
					OLED_ShowCN(48,4,42);//时
					sprintf(st,"%d  ",TimeRAM.minute_kai);
					OLED_ShowStr(64,4,st,2);
					OLED_ShowCN(80,4,44);//分
					if(TimeRAM.kai==0)
					OLED_ShowCN(112,6,22);
					else
					OLED_ShowCN(112,6,21);	
					delay_ms(100);
					OLED_ShowStr(64,4,"  ",2);
				}
				else if(Flag_dingshi==3)
				{
					ds1032_readRAM();
					ds1032_readRAM();
					OLED_ShowCN(32,0,38);
					OLED_ShowCN(48,0,39);//打开时间
					OLED_ShowCN(64,0,42);
					OLED_ShowCN(80,0,43);
					sprintf(st,"%d  ",TimeRAM.hour_kai);
					OLED_ShowStr(32,4,st,2);
					OLED_ShowCN(48,4,42);//时
					sprintf(st,"%d  ",TimeRAM.minute_kai);
					OLED_ShowStr(64,4,st,2);
					OLED_ShowCN(80,4,44);//分
					if(TimeRAM.kai==0)
					OLED_ShowCN(112,6,22);
					else
					OLED_ShowCN(112,6,21);	
					delay_ms(100);
					OLED_ShowStr(112,6,"  ",2);
				}
				else if(Flag_dingshi==4)
				{
					ds1032_readRAM();
					OLED_ShowCN(32,0,40);//
					OLED_ShowCN(48,0,41);//关闭时间
					OLED_ShowCN(64,0,42);
					OLED_ShowCN(80,0,43);
					sprintf(st,"%d  ",TimeRAM.hour_guan);
					OLED_ShowStr(32,4,st,2);
					OLED_ShowCN(48,4,42);//时
					sprintf(st,"%d  ",TimeRAM.minute_guan);
					OLED_ShowStr(64,4,st,2);
					OLED_ShowCN(80,4,44);//分
					if(TimeRAM.guan==0)
					OLED_ShowCN(112,6,22);
					else
					OLED_ShowCN(112,6,21);	
					delay_ms(100);
					OLED_ShowStr(32,4,"  ",2);
				}
				else if(Flag_dingshi==5)
				{
					ds1032_readRAM();
					OLED_ShowCN(32,0,40);//
					OLED_ShowCN(48,0,41);//关闭时间
					OLED_ShowCN(64,0,42);
					OLED_ShowCN(80,0,43);
					sprintf(st,"%d  ",TimeRAM.hour_guan);
					OLED_ShowStr(32,4,st,2);
					OLED_ShowCN(48,4,42);//时
					sprintf(st,"%d  ",TimeRAM.minute_guan);
					OLED_ShowStr(64,4,st,2);
					OLED_ShowCN(80,4,44);//分
					if(TimeRAM.guan==0)
					OLED_ShowCN(112,6,22);
					else
					OLED_ShowCN(112,6,21);	
					delay_ms(100);
					OLED_ShowStr(64,4,"  ",2);
				}
				else if(Flag_dingshi==6)
				{
					ds1032_readRAM();
					OLED_ShowCN(32,0,40);//
					OLED_ShowCN(48,0,41);//关闭时间
					OLED_ShowCN(64,0,42);
					OLED_ShowCN(80,0,43);
					sprintf(st,"%d  ",TimeRAM.hour_guan);
					OLED_ShowStr(32,4,st,2);
					OLED_ShowCN(48,4,42);//时
					sprintf(st,"%d  ",TimeRAM.minute_guan);
					OLED_ShowStr(64,4,st,2);
					OLED_ShowCN(80,4,44);//分
					if(TimeRAM.guan==0)
					OLED_ShowCN(112,6,22);
					else
					OLED_ShowCN(112,6,21);	
					delay_ms(100);
					OLED_ShowStr(112,6,"  ",2);
				}
			else
			{
				ds1032_read_realTime();    //读取此时时刻
				sprintf(st,"%d  ",TimeData.year);
				OLED_ShowStr(8,0,st,2);
				OLED_ShowCN(40,0,23);//年
				sprintf(st,"%d  ",TimeData.month);
				OLED_ShowStr(56,0,st,2);
				OLED_ShowCN(72,0,24);//月
				sprintf(st,"%d  ",TimeData.day);
				OLED_ShowStr(88,0,st,2);
				OLED_ShowCN(104,0,25);//日
				sprintf(st,"%d  ",TimeData.hour);
				OLED_ShowStr(48+8,2,st,2);
				OLED_ShowStr(48+8+16,2,"-",2);
				sprintf(st,"%d  ",TimeData.minute);
				OLED_ShowStr(48+8+16+8,2,st,2);
				OLED_ShowStr(48+8+16+16+8,2,"-",2);
				sprintf(st,"%d  ",TimeData.second);
				OLED_ShowStr(48+8+16+16+16,2,st,2);
				OLED_ShowCN(0,2,12);//星期
				OLED_ShowCN(16,2,13);
				
				OLED_ShowCN(0,4,26);//温度：
				OLED_ShowCN(16,4,27);
				OLED_ShowStr(32,4,":",2);
				OLED_ShowStr(56,4,ch,2);
				OLED_ShowCN(64,4,28);//湿度：
				OLED_ShowCN(80,4,29);
				OLED_ShowStr(96,4,":",2);
				OLED_ShowStr(120,4,"%",2);
				
				DHT11_Read_Data(&Num_wendu1,&Num_wendu2,&Num_shidu1,&Num_shidu2);    
				sprintf(st,"%d",Num_wendu1);
				OLED_ShowStr(40,4,st,2);
				sprintf(st,"%d",Num_shidu1);
				OLED_ShowStr(104,4,st,2);
				switch(TimeData.week)
				{
					case 1:
					{
						OLED_ShowCN(32,2,14);
					}break;
					case 2:
					{
						OLED_ShowCN(32,2,15);
					}break;
					case 3:
					{
						OLED_ShowCN(32,2,16);
					}break;
					case 4:
					{
						OLED_ShowCN(32,2,17);
					}break;
					case 5:
					{
						OLED_ShowCN(32,2,18);
					}break;
					case 6:
					{
						OLED_ShowCN(32,2,19);
					}break;
					case 7:
					{
						OLED_ShowCN(32,2,20);
					}break;
				}
				Flag_dingshi=0;
				OLED_ShowCN(32,6,36);//定时模式
        OLED_ShowCN(48,6,37);
        OLED_ShowCN(64,6,32);
        OLED_ShowCN(80,6,33);OLED_ShowStr(112,6,"  ",2);
			}


				
			}break;
			case 4:
			{
        OLED_ShowCN(32,6,47);//遥控模式
        OLED_ShowCN(48,6,48);
        OLED_ShowCN(64,6,32);
        OLED_ShowCN(80,6,33);
        OLED_ShowStr(112,6,"  ",2);
			}break;
		}
}







