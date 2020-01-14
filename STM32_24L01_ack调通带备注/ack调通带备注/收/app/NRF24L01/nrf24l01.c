#include "nrf24l01.h"
#include "spi1.h"

u8 TX_ADDRESS[TX_ADR_WIDTH]={0x13,0x14,0x52,0x05,0x21}; //自己的地址
u8 RX_ADDRESS[RX_ADR_WIDTH]={0x13,0x14,0x52,0x05,0x20}; //对方的地址
u8 RX_BUF[TX_PLOAD_WIDTH];//接收缓存
u8 TX_BUF[TX_PLOAD_WIDTH];//发送缓存
 
u8 RX_ACK_NUM=0;  //接收ACK长度
u8 RX_ACK_BUF[32];//接收ACK缓存	

//初始化24L01的IO口
void NRF24L01_IO_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;			   


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_CE_CSN, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_IO|NRF24L01_CSN_IO;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
	GPIO_Init(GPIO_TypeDef_GPIOx_CE_CSN, &GPIO_InitStructure);
	GPIO_ResetBits(GPIO_TypeDef_GPIOx_CE_CSN,NRF24L01_CE_IO|NRF24L01_CSN_IO);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_IRQ, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = NRF24L01_IRQ_IO;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_TypeDef_GPIOx_IRQ, &GPIO_InitStructure);
	GPIO_ResetBits(GPIO_TypeDef_GPIOx_IRQ,NRF24L01_IRQ_IO);
						 		 
	NRF24L01_SPIx_Init();    		//初始化SPI	 
	SPI_Cmd(NRF24L01_SPIx, DISABLE); // SPI外设不使能

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI主机
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//数据捕获于第1个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(NRF24L01_SPIx, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Cmd(NRF24L01_SPIx, ENABLE); //使能SPI外设

	NRF24L01_SPIx_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）

	NRF24L01_CE=0; 			//使能24L01
	NRF24L01_CSN=1;			//SPI片选取消   	 		 	 
}

//检测24L01是否存在
//返回值:0，成功;1，失败	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i; 	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}
 
void NRF24L01_Init(void)
{
	NRF24L01_IO_Init();
	NRF24L01_CE=0;	  

	NRF24L01_Write_Reg(NRF_WRITE_REG +EN_AA     , 0x01);
	NRF24L01_Write_Reg(NRF_WRITE_REG +EN_RXADDR , 0x01);
	NRF24L01_Write_Reg(NRF_WRITE_REG +SETUP_RETR, 0x1a);
	NRF24L01_Write_Reg(NRF_WRITE_REG +RF_CH     , 0x05);
	NRF24L01_Write_Reg(NRF_WRITE_REG +RF_SETUP  , 0x0f);

	NRF24L01_CE = 1;
	delay_ms(10);
}
 
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE=0;	    
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR,   (u8*)TX_ADDRESS, TX_ADR_WIDTH);//写自己地址用于自动打包入发射帧,且使能ACK
	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0,(u8*)TX_ADDRESS, TX_ADR_WIDTH);//启用应答时通道0用于接收ACK RX_ADDR_P0必须和TX_ADDR相同，这一句必须有：第10页发送模式第二条  TX_ADDR寄存器介绍
															                   	 //关闭应答时可以省略  
	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
	//以下是开启ACK数据
	NRF24L01_Write_Reg(FLUSH_TX, 0xff);
	NRF24L01_Write_Reg(FLUSH_RX, 0xff);
	NRF24L01_ReadWriteByte(0x50);
	NRF24L01_ReadWriteByte(0x73);
	NRF24L01_Write_Reg(NRF_WRITE_REG + DYNPD, 0x01);
	NRF24L01_Write_Reg(NRF_WRITE_REG + FEATURE, 0x07);
	NRF24L01_CE=1;
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE=0;	  
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写对方节点地址用于匹配指定地址接收,且使能ACK
	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);
	//以下是开启ACK数据
	NRF24L01_Write_Reg(FLUSH_TX, 0xff);
	NRF24L01_Write_Reg(FLUSH_RX, 0xff);
	NRF24L01_ReadWriteByte(0x50);
	NRF24L01_ReadWriteByte(0x73);
	NRF24L01_Write_Reg(NRF_WRITE_REG + DYNPD, 0x01);
	NRF24L01_Write_Reg(NRF_WRITE_REG + FEATURE, 0x07);
	NRF24L01_CE = 1; //CE为高,进入接收模式 
}			
 
u8   NRF24L01_TxPacket(u8*txbuf,u8 txlen,u8*rxbuf,u8 *rxlen)//发送数据包，用于发送模式2/4
{
	u8 sta;
	u8 return_temp=0;
	NRF24L01_CE=0;	
	NRF24L01_Write_Buf(NRF_WRITE_REG | TX_ADDR,   (u8*) TX_ADDRESS, RX_ADR_WIDTH);	//写自己地址用于自动打包入发射帧,且使能ACK
	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8*)TX_ADDRESS, TX_ADR_WIDTH);  //启用应答时通道0用于接收ACK RX_ADDR_P0必须和TX_ADDR相同，这一句必须有：第10页发送模式第二条  TX_ADDR寄存器介绍
															                   	    //关闭应答时可以省略  
	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,txlen); 
	NRF24L01_CE=1;	
	delay_us(10);
	while(NRF24L01_IRQ!=0){};//等待发送完成
	sta=NRF24L01_Read_Reg(NRF_READ_REG+STATUS);//读状态寄存器
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta);//?清除RX_DS中断标志
	//printf("sta:%x\r\n",sta);

	if(sta&TX_OK)
	{        
		//printf("TX_OK\r\n");
		return_temp|=TX_OK;
	}
	if(sta&MAX_TX)
	{
		printf("MAX_TX\r\n");

		return_temp|=MAX_TX;		 
	}
	if(sta&RX_OK)
	{
		//printf("RX_ACK_OK\r\n");
		*rxlen=NRF24L01_Read_Reg(NRF_READ_REG+R_RX_PL_WID);
		if(*rxlen<33)
		{
			NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,*rxlen);//从RXFIFO读出数据
		}
		else
		{
			*rxlen=0;
			NRF24L01_Write_Reg(FLUSH_RX, 0xff);
		}	
		return_temp|=RX_OK;
	}
	return return_temp;
}

void NRF24L01_PacketAckData(u8*pBuf,u8 len)//发送数据包，用于发送模式2/4
{
	NRF24L01_CE=0;	
	NRF24L01_Write_Buf(W_ACK_PAYLOAD,pBuf,len);
	NRF24L01_CE=1;	
}

u8 NRF24L01_RxPacket(u8 *channel,u8 *rxbuf,u8 *rxlen)
{
	u8 sta;
	sta=NRF24L01_Read_Reg(NRF_READ_REG+STATUS);//读状态寄存器
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta);//清除RX_DS中断标志
	//printf("sta:%x\r\n",sta);
	if(sta&RX_OK)
	{
		//printf("RX_OK\r\n");
		*channel = (sta & 0x0e) >> 1;
		*rxlen=NRF24L01_Read_Reg(NRF_READ_REG+R_RX_PL_WID);
		if(*rxlen<33)
		{
			NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,*rxlen);//从RXFIFO读出数据
		}
		else
		{
			*rxlen=0;
			NRF24L01_Write_Reg(FLUSH_RX, 0xff);
		}
		return RX_OK;
	}
	return 0; 
}








u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
	NRF24L01_CSN=0;                 //使能SPI传输
	status =NRF24L01_ReadWriteByte(reg);//发送寄存器号 
	NRF24L01_ReadWriteByte(value);      //写入寄存器的值
	NRF24L01_CSN=1;                 //禁止SPI传输	   
	return(status);       			//返回状态值
}

u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
	NRF24L01_CSN = 0;          //使能SPI传输		
	NRF24L01_ReadWriteByte(reg);   //发送寄存器号
	reg_val=NRF24L01_ReadWriteByte(0XFF);//读取寄存器内容
	NRF24L01_CSN = 1;          //禁止SPI传输		    
	return(reg_val);           //返回状态值
}	

u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
	NRF24L01_CSN = 0;           //使能SPI传输
	status=NRF24L01_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=NRF24L01_ReadWriteByte(0XFF);//读出数据
	NRF24L01_CSN=1;       //关闭SPI传输
	return status;        //返回读到的状态值
}

u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
	NRF24L01_CSN = 0;          //使能SPI传输
	status = NRF24L01_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)NRF24L01_ReadWriteByte(*pBuf++); //写入数据	 
	NRF24L01_CSN = 1;       //关闭SPI传输
	return status;          //返回读到的状态值
}		
