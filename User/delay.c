#include "stm32f10x.h"
#include "delay.h"

static __IO u32 ms_counter=0;
void DelayInit()
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	
	if (SysTick_Config(SystemCoreClock / 1000))        // ST3.5.0库版本
	{
					/* Capture error */
					while (1);
	}
	          
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;// 使能滴答定时器  

}
		    								   

void DelayMs(unsigned int nms)
{
	ms_counter=nms;
	while(ms_counter);
}

void DelayS(unsigned int ns)//延时秒
{
	unsigned char i;
	for(i=0;i<ns;i++)
	{
		DelayMs(1000);
	}
}

void TimingDelay_Decrement(void)
{
	if (ms_counter != 0x00)
	{ 
		ms_counter--;
	}
}
