
#include "key.h"

volatile u8 Key; 
						    
//按键初始化函数
void Key_Scan_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 #if  (Stm32f103zet6_key==1)
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
#endif
#if  (Stm32f103c8t6_key==1)
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;//KEY1-KEY3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
#endif

}

//注意此函数有响应优先级,WK_UP>KEY1>KEY2>KEY3
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//-1，没有任何按键按下
//0，WK_UP按下 WK_UP
//1，KEY1按下
//2，KEY2按下
//3，KEY3按下 
u8 KEY_Scan(u8 mode)
{	 
	static u8 flag=0;
	static s8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按	
#if  (Stm32f103zet6_key==1)
  if(flag==0)
  {
			if(key_up&&(KEY_UP==1||KEY_LEFT==0||KEY_DOWN==0||KEY_RIGHT==0))
			    flag=1;
			else if(KEY_UP==0&&KEY_LEFT==1&&KEY_DOWN==1&&KEY_RIGHT==1)
         key_up=1; 
	}
	else if(flag==1)
	{
		key_up=0;
		flag=0;
		if     (KEY_UP==1)    return KEY_UP_PRES;
		else if(KEY_LEFT==0)  return KEY_LEFT_PRES;
		else if(KEY_DOWN==0)  return KEY_DOWN_PRES;
		else if(KEY_RIGHT==0) return KEY_RIGHT_PRES; 
	}	    
 	return KEY_NO_PRES;// 无按键按下
#endif
#if  (Stm32f103c8t6_key==1)
  if(flag==0)
  {
			if(key_up&&(KEY_UP==0||KEY_DOWN==0))
			    flag=1;
			else if(KEY_UP==1&&KEY_DOWN==1)
         key_up=1; 
	}
	else if(flag==1)
	{
		key_up=0;
		flag=0;
		if     (KEY_UP==0)    return KEY_UP_PRES;
		else if(KEY_DOWN==0)  return KEY_DOWN_PRES;
	}	    
 	return KEY_NO_PRES;// 无按键按下
#endif
}


