#include "led.h"

	    
//LED IO初始化
void LED_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	#if  (Stm32f103zet6_led==1)
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//LED0-7-->PC.0-7 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					
	 GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);				  //GPIOC0-7 输出高

#endif
#if  (Stm32f103c8t6_led==1)
	 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE);	 //使能PC端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//LED--PC13
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC0-7
	 GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //GPIOC0-7 输出高


	 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);	 //使能PC端口时钟

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC0-7
	 GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);	
#endif	
}

