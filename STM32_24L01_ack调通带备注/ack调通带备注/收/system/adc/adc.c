#include "adc.h"


u16 AD_Value[CH_NUM];
float AD_float_v[CH_NUM];
//PA0--ADC_Channel_0  PA1--ADC_Channel_1  PA2--ADC_Channel_2   PA3--ADC_Channel_3   PA4--ADC_Channel_4   PA5--ADC_Channel_5   
//PA6--ADC_Channel_6  PA7--ADC_Channel_7  PB0--ADC_Channel_8   PB1--ADC_Channel_9    
//ADC_Channel_TempSensor 内部温度--ADC_Channel_16   内部参考电压 ADC_Channel_Vrefint--ADC_Channel_17
//temperate=(1.43-(float)adcx*(3.3/4096))/0.0043+25;	//转换为温度值 


//本项目开启8路，外加一路内部电压矫正
//PA0--ADC_Channel_0  PA1--ADC_Channel_1  PA4--ADC_Channel_4   PA5--ADC_Channel_5   
//PA6--ADC_Channel_6  PA7--ADC_Channel_7  PB0--ADC_Channel_8   PB1--ADC_Channel_9
//内部参考电压 ADC_Channel_Vrefint--ADC_Channel_17
void  Adc_Init(void)
{ 	
			GPIO_InitTypeDef  GPIO_InitStructure;
			ADC_InitTypeDef  ADC_InitStructure; 
	    DMA_InitTypeDef DMA_InitStructure;
	    NVIC_InitTypeDef NVIC_InitStructure;
	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB| RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE );   

			RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC最大时间不能超过14M    
																		 
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                
			GPIO_Init(GPIOB, &GPIO_InitStructure);

			ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值
			ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;        //ADC工作模式:ADC1和ADC2工作在独立模式
			ADC_InitStructure.ADC_ScanConvMode =ENABLE;        //多信道扫描模式
			ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //模数转换工作在连续转换模式
			ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //外部触发转换关闭
			ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;        //ADC数据右对齐
			ADC_InitStructure.ADC_NbrOfChannel = CH_NUM;        //此处开7个信道（可开的为1~16）
			ADC_Init(ADC1, &ADC_InitStructure);        //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器


			//ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期
			ADC_RegularChannelConfig(ADC1, ADC_Channel_0,1, ADC_SampleTime_239Cycles5 );                
			ADC_RegularChannelConfig(ADC1, ADC_Channel_1,2, ADC_SampleTime_239Cycles5 );
			ADC_RegularChannelConfig(ADC1, ADC_Channel_4,3, ADC_SampleTime_239Cycles5 );                
			ADC_RegularChannelConfig(ADC1, ADC_Channel_5,4, ADC_SampleTime_239Cycles5 );
			ADC_RegularChannelConfig(ADC1, ADC_Channel_6,5, ADC_SampleTime_239Cycles5 );
		  ADC_RegularChannelConfig(ADC1, ADC_Channel_7,6, ADC_SampleTime_239Cycles5 );                
			ADC_RegularChannelConfig(ADC1, ADC_Channel_8,7, ADC_SampleTime_239Cycles5 );
			ADC_RegularChannelConfig(ADC1, ADC_Channel_9,8, ADC_SampleTime_239Cycles5 );
			ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器，参考电压通道
			ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint,9, ADC_SampleTime_239Cycles5 );

			//开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
			ADC_DMACmd(ADC1, ENABLE);       //使能ADC1的DMA传输         
			ADC_Cmd(ADC1, ENABLE);           //使能指定的ADC1
			ADC_ResetCalibration(ADC1);        //复位指定的ADC1的校准寄存器
			while(ADC_GetResetCalibrationStatus(ADC1));        //获取ADC1复位校准寄存器的状态,设置状态则等待
			ADC_StartCalibration(ADC1);                //开始指定ADC1的校准状态
			while(ADC_GetCalibrationStatus(ADC1));                //获取指定ADC1的校准程序,设置状态则等待
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);	  //软件启动AD转换
     /* Enable the DMA Interrupt */
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;   
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 优先级设置
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);		
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
			DMA_DeInit(DMA1_Channel1);   //将DMA的通道1寄存器重设为缺省值
			DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)ADC1_DR_Address;  //DMA外设ADC基地址
			DMA_InitStructure.DMA_MemoryBaseAddr =    (u32)&AD_Value;        //DMA内存基地址
			DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //内存作为数据传输的目的地
			DMA_InitStructure.DMA_BufferSize = CH_NUM;  //DMA通道的DMA缓存的数据单元大小
			DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
			DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //数据宽度为16位
			DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
			DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //循环工作模式
			DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级 
			DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
			DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道
			DMA_ClearFlag(DMA1_Channel1_IRQn);                                 // 清除DMA所有标志 
			DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA1_Channel1, ENABLE);           //启动DMA通道
}				  


void DMA1_Channel1_IRQHandler(void)
{
		u8 i=0;
    if(DMA_GetITStatus(DMA1_FLAG_TC1))
    {
            DMA_ClearFlag(DMA1_FLAG_GL1);         // 清除标志
            for(i=0;i<CH_NUM;i++)
							 AD_float_v[i]=(float)AD_Value[i]/(float)AD_Value[8]*1.2;
			     //添加滤波函数
    }
}

//获得ADC值
u16 Get_Adc(u8 ch)  //ch:通道值 0~CH_NUM-1 
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

