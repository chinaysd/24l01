#ifndef __DELAY_H
#define __DELAY_H 	

#include "sys.h"  

extern volatile u32 xitong_haomiao;

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























