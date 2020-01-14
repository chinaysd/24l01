#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"
#include "sys.h"

//GPIO_Mode_AIN            模拟输入
//GPIO_Mode_IN_FLOATING    浮空输入
//GPIO_Mode_IPD            下拉输入
//GPIO_Mode_IPU            上拉输入
//GPIO_Mode_Out_OD         开漏输出
//GPIO_Mode_Out_PP         推挽输出
//GPIO_Mode_AF_OD          复用开漏输出
//GPIO_Mode_AF_PP          复用推挽输出


//中断优先级设置，只有抢占，不开启相应
//抢占0-15 滴答定时器默认是15,0也最好不用，可以设置的范围为：1-14

//串口
#define USART1_IRQHandler_NVIC_IRQChannelPreemptionPriority          5//USART1全局中断
//#define USART2_IRQHandler_NVIC_IRQChannelPreemptionPriority          5//USART2全局中断
//#define USART3_IRQHandler_NVIC_IRQChannelPreemptionPriority          5//USART3全局中断
//#define UART4_IRQHandler_NVIC_IRQChannelPreemptionPriority           5//UART4全局中断
//#define UART5_IRQHandler_NVIC_IRQChannelPreemptionPriority           5//UART5全局中断
////外部中断线
#define EXTI0_IRQHandler_NVIC_IRQChannelPreemptionPriority           2//EXTI线0中断
#define EXTI1_IRQHandler_NVIC_IRQChannelPreemptionPriority           2//EXTI线1中断
//#define EXTI2_IRQHandler_NVIC_IRQChannelPreemptionPriority           2//EXTI线2中断
//#define EXTI3_IRQHandler_NVIC_IRQChannelPreemptionPriority           2//EXTI线3中断
//#define EXTI4_IRQHandler_NVIC_IRQChannelPreemptionPriority           2//EXTI线4中断
//#define EXTI9_5_IRQHandler_NVIC_IRQChannelPreemptionPriority         2//EXTI线5-9中断
//#define EXTI15_10_IRQHandler_NVIC_IRQChannelPreemptionPriority       2//EXTI线10-15中断
////定时器中断（TIM1,8高级定时器，TIM2,3,4,5通用定时器，TIM6,7基本定时器）
//#define TIM1_BRK_IRQHandler_NVIC_IRQChannelPreemptionPriority        1//TIM1刹车中断
//#define TIM1_UP_IRQHandler_NVIC_IRQChannelPreemptionPriority         1//TIM1更新中断
//#define TIM1_TRG_COM_IRQHandler_NVIC_IRQChannelPreemptionPriority    1//TIM1触发和通信中断 
//#define TIM1_CC_IRQHandler_NVIC_IRQChannelPreemptionPriority         1//TIM1捕获比较中断
//#define TIM2_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//TIM2全局中断
//#define TIM3_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//TIM3全局中断
//#define TIM4_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//TIM4全局中断
//#define TIM5_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//TIM5全局中断
//#define TIM6_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//TIM6全局中断
//#define TIM7_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//TIM7全局中断
//#define TIM8_BRK_IRQHandler_NVIC_IRQChannelPreemptionPriority        1//TIM8刹车中断
//#define TIM8_UP_IRQHandler_NVIC_IRQChannelPreemptionPriority         1//TIM8更新中断
//#define TIM8_TRG_COM_IRQHandler_NVIC_IRQChannelPreemptionPriority    1//TIM8触发和通信中断
//#define TIM8_CC_IRQHandler_NVIC_IRQChannelPreemptionPriority         1//TIM8捕获比较中断
////DMA中断
//#define DMA1_Channel1_IRQHandler_NVIC_IRQChannelPreemptionPriority   1//DMA1通道1全局中断
//#define DMA1_Channel2_IRQHandler_NVIC_IRQChannelPreemptionPriority   1//DMA1通道2全局中断
//#define DMA1_Channel3_IRQHandler_NVIC_IRQChannelPreemptionPriority   1//DMA1通道3全局中断
//#define DMA1_Channel4_IRQHandler_NVIC_IRQChannelPreemptionPriority   1//DMA1通道4全局中断
//#define DMA1_Channel5_IRQHandler_NVIC_IRQChannelPreemptionPriority   1//DMA1通道5全局中断
//#define DMA1_Channel6_IRQHandler_NVIC_IRQChannelPreemptionPriority   1//DMA1通道6全局中断
//#define DMA1_Channel7_IRQHandler_NVIC_IRQChannelPreemptionPriority   1//DMA1通道7全局中断
//#define DMA2_Channel1_IRQHandler_NVIC_IRQChannelPreemptionPriority   1//DMA2通道1全局中断
//#define DMA2_Channel2_IRQHandler_NVIC_IRQChannelPreemptionPriority   1//DMA2通道2全局中断
//#define DMA2_Channel3_IRQHandler_NVIC_IRQChannelPreemptionPriority   1//DMA2通道3全局中断
//#define DMA2_Channel4_5_IRQHandler_NVIC_IRQChannelPreemptionPriority 1//DMA2通道4全局中断
////CAN USB
//#define USB_HP_CAN1_TX_IRQHandler_NVIC_IRQChannelPreemptionPriority  1//USB高优先级或CAN发送中断 
//#define CAN1_RX1_IRQHandler_NVIC_IRQChannelPreemptionPriority        1//CAN接收1中断
//#define USB_LP_CAN1_RX0_IRQHandler_NVIC_IRQChannelPreemptionPriority 1//USB低优先级或CAN接收0中断 
//#define CAN1_SCE_IRQHandler_NVIC_IRQChannelPreemptionPriority        1//CAN SCE中断 
//#define USBWakeUp_IRQHandler_NVIC_IRQChannelPreemptionPriority       1//连到EXTI的从USB待机唤醒中断
////RTC实时时钟
//#define RTC_IRQHandler_NVIC_IRQChannelPreemptionPriority             1//实时时钟(RTC)全局中断 
//#define RTCAlarm_IRQHandler_NVIC_IRQChannelPreemptionPriority        1//连到EXTI的RTC闹钟中断


////不常用
//窗口看门狗
//#define WWDG_IRQHandler_NVIC_IRQChannelPreemptionPriority            14//窗口看门狗
////SPI  IIC SDIO中断
//#define SPI1_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//SPI1全局中断
//#define SPI2_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//SPI2全局中断
//#define SPI3_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//SPI3全局中断
//#define I2C1_EV_IRQHandler_NVIC_IRQChannelPreemptionPriority         1//I2C1事件中断
//#define I2C1_ER_IRQHandler_NVIC_IRQChannelPreemptionPriority         1//I2C1错误中断
//#define I2C2_EV_IRQHandler_NVIC_IRQChannelPreemptionPriority         1//I2C2事件中断
//#define I2C2_ER_IRQHandler_NVIC_IRQChannelPreemptionPriority         1//I2C2错误中断
//#define SDIO_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//SDIO全局中断
////ADC
//#define ADC1_2_IRQHandler_NVIC_IRQChannelPreemptionPriority          1//ADC1和ADC2全局中断
//#define ADC3_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//ADC3全局中断

//#define FSMC_IRQHandler_NVIC_IRQChannelPreemptionPriority            1//FSMC全局中断
//#define FLASH_IRQHandler_NVIC_IRQChannelPreemptionPriority           1//闪存全局中断 
//#define RCC_IRQHandler_NVIC_IRQChannelPreemptionPriority             1//复位和时钟控制(RCC)中断
//#define PVD_IRQHandler_NVIC_IRQChannelPreemptionPriority             1//连到EXTI的电源电压检测(PVD)中断 
//#define TAMPER_IRQHandler_NVIC_IRQChannelPreemptionPriority          1//侵入检测中断 



//位带操作,实现51类似的GPIO控制功能,具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口! 确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

//以下为汇编函数
void WFI_SET(void);		  //执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址
#define _BIN8(a,b,c,d,e,f,g,h) ((a<<7)+(b<<6)+(c<<5)+(d<<4)+(e<<3)+(f<<2)+(g<<1)+(h<<0))
#endif
