#ifndef __KEY_H
#define __KEY_H	 

#include "sys.h"
volatile extern u8 Key;

//开发板选择
#define Stm32f103zet6_key		1														    
#define Stm32f103c8t6_key   0



#if  (Stm32f103zet6_key==1)
		#define KEY_LEFT  PEin(2) //PE2
		#define KEY_DOWN  PEin(3)	//PE3 
		#define KEY_RIGHT PEin(4)	//PE4
		#define KEY_UP    PAin(0)	//PA0


		#define KEY_NO_PRES 	    0
		#define KEY_LEFT_PRES 	  1	
		#define KEY_DOWN_PRES     2
		#define KEY_RIGHT_PRES	  3
		#define KEY_UP_PRES	      4
#endif
#if  (Stm32f103c8t6_key==1)
		#define KEY_UP    PAin(0)	//PA0
		#define KEY_DOWN  PEin(3)	//PE3 

		#define KEY_NO_PRES 	    0	
		#define KEY_UP_PRES	      4	
		#define KEY_DOWN_PRES     2
#endif



void Key_Scan_Init(void);//IO初始化
u8   KEY_Scan(u8);  	   //按键扫描函数

#endif
