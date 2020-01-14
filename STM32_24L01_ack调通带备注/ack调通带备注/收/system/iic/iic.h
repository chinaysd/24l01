#ifndef _IIC_H
#define _IIC_H
#include "sys.h"

//////////////////////////////////////////////////////////
#define IIC_RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOB
#define IIC_GPIO                  GPIOB
#define IIC_SCL_IO                GPIO_Pin_10 //SCL
#define IIC_SDA_IO                GPIO_Pin_11 //SDA	
#define IIC_SCL                   PBout(10) 
#define IIC_SDA                   PBout(11) 	 
#define READ_SDA                  PBin (11)  //输入SDA 
/////////////////////////////////////////////////////////



void IIC_Init(void);        //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  	  //发送IIC停止信号

u8   IIC_Wait_Ack(void); 	  //IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Send_Byte(u8 txd);		        //IIC发送一个字节
u8   IIC_Read_Byte(unsigned char ack);//IIC读取一个字节

#endif
