#include "NRF24L01.h"

u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //发送地址
u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //发送地址

uint8_t channal = 30;

//初始化NRF24L01IO口
//CE->PD8,CSN->PD9,SCK->PD10,MOSI->PD11,MISO->PD12,IRQ->PD13

u8 NRF24L01_Write_Buf(u8 reg_addr, u8 *pBuf, u8 data_len);
u8 NRF24L01_Read_Buf(u8 reg_addr, u8 *pBuf, u8 data_len);

void Delay(uint16_t nCount)
{
	for (; nCount != 0; nCount--)
		;
}

void Init_NRF24L01(void)
{
	GPIO_Init(MOSI_PORT, MOSI_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(SCK_PORT, SCK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(CSN_PORT, CSN_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(CE_PORT, CE_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	CE_H;	//初始化时先拉高
	CSN_H; //初始化时先拉高

	GPIO_Init(IRQ_PORT, IRQ_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(MISO_PORT, MISO_PIN, GPIO_MODE_IN_PU_NO_IT);

	GPIO_WriteHigh(IRQ_PORT, IRQ_PIN); //IRQ置高

	CE_L;	//使能NRF24L01
	CSN_H; //SPI片选取消
}

//模拟SPI读写数据函数
u8 SPI_ReadWriteByte(u8 TxData)
{
	u16 bit_ctr;
	for (bit_ctr = 0; bit_ctr < 8; bit_ctr++)
	{
		if (TxData & 0x80)
			MOSI_H;
		else
			MOSI_L;
		TxData = (TxData << 1);
		SCK_H;
		Delay(0xff);
		if (READ_MISO)
			TxData |= 0x01;
		SCK_L;
		Delay(0xff);
	}
	return (TxData);
}

//上电检测NRF24L01是否在位
//写5个数据然后再读回来进行比较，
//相同时返回值0，表示在位;
//否则返回1，表示不在位.
u8 NRF24L01_Check(void)
{
	u8 buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	u8 buf1[5];
	u8 i;
	NRF24L01_Write_Buf(SPI_WRITE_REG + TX_ADDR, buf, 5); //写入5个字节的地址.
	NRF24L01_Read_Buf(TX_ADDR, buf1, 5);								 //读出写入的地址
	for (i = 0; i < 5; i++)
		if (buf1[i] != 0XA5)
			break;
	if (i != 5)
		return 1; //NRF24L01不在位

	return 0; //NRF24L01在位
}
//通过SPI写寄存器
u8 NRF24L01_Write_Reg(u8 reg_addr, u8 data)
{
	u8 status;
	CSN_L;																//使能SPI传输
	status = SPI_ReadWriteByte(reg_addr); //发送寄存器号
	SPI_ReadWriteByte(data);							//写入寄存器的值
	CSN_H;																//禁止SPI传输
	return (status);											//返回状态值
}
//读取SPI寄存器值 ，regaddr:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg_addr)
{
	u8 reg_val;
	CSN_L;													//使能SPI传输
	SPI_ReadWriteByte(reg_addr);		//发送寄存器号
	reg_val = SPI_ReadWriteByte(0); //读取寄存器内容

	CSN_H;						//禁止SPI传输
	return (reg_val); //返回状态值
}
//在指定位置读出指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Read_Buf(u8 reg_addr, u8 *pBuf, u8 data_len)
{
	u8 status, i;
	CSN_L;																//使能SPI传输
	status = SPI_ReadWriteByte(reg_addr); //发送寄存器值(位置),并读取状态值
	for (i = 0; i < data_len; i++)
		pBuf[i] = SPI_ReadWriteByte(0); //读出数据

	CSN_H;				 //关闭SPI传输
	return status; //返回读到的状态值
}
//在指定位置写指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg_addr, u8 *pBuf, u8 data_len)
{
	u8 status, i;
	CSN_L;																//使能SPI传输
	status = SPI_ReadWriteByte(reg_addr); //发送寄存器值(位置),并读取状态值
	for (i = 0; i < data_len; i++)
		SPI_ReadWriteByte(*pBuf++); //写入数据
	CSN_H;												//关闭SPI传输
	return status;								//返回读到的状态值
}
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *tx_buf)
{
	u8 state;
	CE_L;
	NRF24L01_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); //写数据到TX BUF  32个字节
	CE_H;																										 //启动发送
	while (READ_IRQ != 0)
		;																								 //等待发送完成
	state = NRF24L01_Read_Reg(STATUS);								 //读取状态寄存器的值
	NRF24L01_Write_Reg(SPI_WRITE_REG + STATUS, state); //清除TX_DS或MAX_RT中断标志
	if (state & MAX_TX)																 //达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX, 0xff); //清除TX FIFO寄存器
		return MAX_TX;
	}
	if (state & TX_OK) //发送完成
	{
		return TX_OK;
	}
	return 0xff; //其他原因发送失败
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF24L01_RxPacket(u8 *rx_buf)
{
	u8 state;
	state = NRF24L01_Read_Reg(STATUS);								 //读取状态寄存器的值
	NRF24L01_Write_Reg(SPI_WRITE_REG + STATUS, state); //清除TX_DS或MAX_RT中断标志
	if (state & RX_OK)																 //接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD, rx_buf, RX_PLOAD_WIDTH); //读取数据
		NRF24L01_Write_Reg(FLUSH_RX, 0xff);											//清除RX FIFO寄存器
		return 0;
	}
	return 1; //没收到任何数据
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了
void RX_Mode(void)
{
	CE_L;
	//写RX节点地址
	NRF24L01_Write_Buf(SPI_WRITE_REG + RX_ADDR_P0, (u8 *)RX_ADDRESS, RX_ADR_WIDTH);

	//使能通道0的自动应答
	NRF24L01_Write_Reg(SPI_WRITE_REG + EN_AA, 0x01);
	//使能通道0的接收地址
	NRF24L01_Write_Reg(SPI_WRITE_REG + EN_RXADDR, 0x01);
	//设置RF通信频率
	NRF24L01_Write_Reg(SPI_WRITE_REG + RF_CH, channal);
	//选择通道0的有效数据宽度
	NRF24L01_Write_Reg(SPI_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
	//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	NRF24L01_Write_Reg(SPI_WRITE_REG + RF_SETUP, 0x0f);
	//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX接收模式
	NRF24L01_Write_Reg(SPI_WRITE_REG + CONFIG, 0x0f);
	//CE为高,进入接收模式
	CE_H;
}

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,
//选择RF频道,波特率和LNA HCURR PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了
//CE为高大于10us,则启动发送.
void TX_Mode(void)
{
	CE_L;
	//写TX节点地址
	NRF24L01_Write_Buf(SPI_WRITE_REG + TX_ADDR, (u8 *)TX_ADDRESS, TX_ADR_WIDTH);
	//设置TX节点地址,主要为了使能ACK
	NRF24L01_Write_Buf(SPI_WRITE_REG + RX_ADDR_P0, (u8 *)RX_ADDRESS, RX_ADR_WIDTH);

	//使能通道0的自动应答
	NRF24L01_Write_Reg(SPI_WRITE_REG + EN_AA, 0x01);
	//使能通道0的接收地址
	NRF24L01_Write_Reg(SPI_WRITE_REG + EN_RXADDR, 0x01);
	//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	NRF24L01_Write_Reg(SPI_WRITE_REG + SETUP_RETR, 0x1a);
	//设置RF通道为40
	NRF24L01_Write_Reg(SPI_WRITE_REG + RF_CH, channal);
	//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	NRF24L01_Write_Reg(SPI_WRITE_REG + RF_SETUP, 0x0f);
	//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX发送模式,开启所有中断
	NRF24L01_Write_Reg(SPI_WRITE_REG + CONFIG, 0x0e);
	// CE为高,10us后启动发送
	CE_H;
}

void Channal_Change(u8 num)
{
	channal = num;
	CE_L;
	NRF24L01_Write_Reg(SPI_WRITE_REG + RF_CH, channal); // 频道选择
	CE_H;
}
