#include "exti.h"


volatile u32 maichong_left=0;
volatile u32 maichong_right=0;


void EXTIx_Init(void)
{
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef  GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		   //输入
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
		GPIO_Init(GPIOA, &GPIO_InitStructure);					 
		GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);						 //下降沿触发输出高
	
	
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
  	EXTI_Init(&EXTI_InitStructure);	 	

    
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
  	EXTI_Init(&EXTI_InitStructure);	 	
		
    

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI0_IRQHandler_NVIC_IRQChannelPreemptionPriority;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 
  	NVIC_Init(&NVIC_InitStructure);  	


	  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			  
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI1_IRQHandler_NVIC_IRQChannelPreemptionPriority;  
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 
  	NVIC_Init(&NVIC_InitStructure);  	 
 
}


void EXTI0_IRQHandler(void)
{ 
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
     {
					maichong_left++; 
				  EXTI_ClearITPendingBit(EXTI_Line0); 
     }
}
void EXTI1_IRQHandler(void)
{ 
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
     {
					maichong_right++; 
	 	      EXTI_ClearITPendingBit(EXTI_Line1); 
     }
}


