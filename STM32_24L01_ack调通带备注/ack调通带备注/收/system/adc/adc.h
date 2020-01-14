#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"


#define CH_NUM  9           //要使用的ADC通道数
#define ADC1_DR_Address    ((uint32_t)0x4001244C)

extern  u16   AD_Value[CH_NUM]; 
extern  float AD_float_v[CH_NUM];
void    Adc_Init(void);
u16     Get_Adc(u8 ch);  //ch:通道值 0~CH_NUM-1 

#endif 
