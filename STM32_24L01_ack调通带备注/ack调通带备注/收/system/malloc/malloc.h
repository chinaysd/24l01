#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h"

#define SRAMEX    0		//外部内存池 
 

//内存参数设定.
#if SRAMEX==0
		#define MEM_BLOCK_SIZE			32  	  						//内存块大小为32字节
		#define MEM_MAX_SIZE			  10*1024  						//最大管理内存 10K
		#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE 	//内存表大小
#else	
		#define MEM_BLOCK_SIZE			32  	  						//内存块大小为32字节
		#define MEM_MAX_SIZE			  960 *1024           //最大管理内存960K(963.76)    x*1024+x*1024/32*2=1024*1024 x=963.765
		#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE 	//内存表大小 
#endif	




#ifndef NULL
#define NULL 0
#endif	

//内存管理控制器
struct _m_mallco_dev
{
	void (*init)(void);				//初始化
	u8 (*perused)(void);		  	//内存使用率
	u8 	*membase;					//内存池 
	u16 *memmap; 					//内存管理状态表
	u8  memrdy; 					//内存管理是否就绪
};
extern struct _m_mallco_dev mallco_dev;	//在mallco.c里面定义

void mymemset(void *s,u8 c,u32 count);	//设置内存
void mymemcpy(void *des,void *src,u32 n);//复制内存     
void mem_init(void);					 //内存管理初始化函数(外/内部调用)
u32 mem_malloc(u32 size);		 		//内存分配(内部调用)
u8 mem_free(u32 offset);		 		//内存释放(内部调用)
u8 mem_perused(void);					//得内存使用率(外/内部调用) 


//用户调用函数
void myfree(void *ptr);  				//内存释放(外部调用)
void *mymalloc(u32 size);				//内存分配(外部调用)
void *myrealloc(void *ptr,u32 size);	//重新分配内存(外部调用)
#if SRAMEX==1
		void FSMC_SRAM_Init(void);
		void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 NumHalfwordToWrite);
		void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 NumHalfwordToRead);
		void fsmc_sram_test_write(u8 data,u32 addr);
		u8 fsmc_sram_test_read(u32 addr);
#endif	


#endif













