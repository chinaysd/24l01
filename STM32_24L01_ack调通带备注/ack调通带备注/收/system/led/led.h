#ifndef __LED_H
#define __LED_H	 

#include "sys.h"
//开发板选择
#define Stm32f103zet6_led		1														    
#define Stm32f103c8t6_led   0


#if  (Stm32f103zet6_led==1)
	#define LED1 PCout(0)// PC0
	#define LED2 PCout(1)// PC1
	#define LED3 PCout(2)// PC2
	#define LED4 PCout(3)// PC3
	#define LED5 PCout(4)// PC4
	#define LED6 PCout(5)// PC5
	#define LED7 PCout(6)// PC6
	#define LED8 PCout(7)// PC7
#endif
#if  (Stm32f103c8t6_led==1)
	#define LED13 PCout(13)// PC13
	#define LED8  PBout(8)
	#define LED9  PBout(9)
#endif


void LED_Init(void);//初始化
		 				    
#endif
