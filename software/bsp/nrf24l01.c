#include "nRF24L01P.h"

u8 channal = 25;
u8 Rf_Power = HighPowerMode;

/** ******************************************************** */
/** 功能:模拟SPI通讯 */
/** 入口参数：MOSI写入的值 */
/** 返回值：MISO读到的值 */
/** ******************************************************** */

static u8 SPI_RW(u8 byte)
{
    u8 bit_ctr;
    for (bit_ctr = 0; bit_ctr < 8; bit_ctr++)
    {
        // MOSI = (byte & 0x80);
        MOSI_SET(byte & 0x80);
        byte = (byte << 1);
        // SCK = 1;
        SCK_SET(1);
        // byte |= MISO;
        byte |= MISO_READ();
        // SCK = 0;
        SCK_SET(0);
    }
    return (byte);
}

/** ******************************************************** */
/** 函数功能：nRF24L01+引脚初始化                 */
/** 入口参数：无 */
/** 返回  值：无 */
/** ********************************************************* */
void nRF24L01P_Init(void)
{
    GPIO_Init(IRQ_PORT, IRQ_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(MISO_PORT, MISO_PIN, GPIO_MODE_IN_FL_NO_IT);

    GPIO_Init(CE_PORT, CE_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(CSN_PORT, CSN_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(SCK_PORT, SCK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(MOSI_PORT, MOSI_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);

    SCK_SET(0);
    CSN_SET(1);
    CE_SET(0);
    MOSI_SET(0);
    IRQ_SET(1);

#if 0
    SCK = 0; //SPI时钟线拉低
    CSN = 1;
    CE = 0;
    MOSI = 0;
    IRQ = 1;
#endif
}

/** ******************************************************** */
/** 函数功能：写寄存器的值（单字节）                 */
/** 入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg） */
/** value:寄存器的值 */
/** 返回  值：状态寄存器的值 */
/** ********************************************************* */
u8 nRF24L01P_Write_Reg(u8 reg, u8 value)
{
    u8 status;

    // CSN = 0;
    CSN_SET(0);
    status = SPI_RW(reg);
    SPI_RW(value);

    // CSN = 1;
    CSN_SET(1);

    return (status);
}
/** ******************************************************** */
/** 函数功能：写寄存器的值（多字节）                   */
/** 入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg） */
/** pBuf:写数据首地址 */
/** bytes:写数据字节数 */
/** 返回  值：状态寄存器的值 */
/** ********************************************************* */
u8 nRF24L01P_Write_Buf(u8 reg, const u8 *pBuf, u8 bytes)
{
    u8 status, byte_ctr;

    // CSN = 0;
    CSN_SET(0);
    status = SPI_RW(reg);
    for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        SPI_RW(*pBuf++);

    // CSN = 1;
    CSN_SET(1);

    return (status);
}
/** ******************************************************** */
/**   函数功能：读取寄存器的值（单字节）                   */
/**   入口参数：reg:寄存器映射地址（格式：READ_REG｜reg） */
/**   返回  值：寄存器值 */
/** ********************************************************* */
u8 nRF24L01P_Read_Reg(u8 reg)
{
    u8 value;

    // CSN = 0;
    CSN_SET(0);

    SPI_RW(reg);
    value = SPI_RW(0);

    // CSN = 1;
    CSN_SET(1);
    return (value);
}
/** ******************************************************** */
/** 函数功能：读取寄存器的值（多字节）                   */
/** 入口参数：reg:寄存器映射地址（READ_REG｜reg） */
/** pBuf:接收缓冲区的首地址 */
/** bytes:读取字节数 */
/** 返回  值：状态寄存器的值 */
/** ********************************************************* */
u8 nRF24L01P_Read_Buf(u8 reg, u8 *pBuf, u8 bytes)
{
    u8 status, byte_ctr;

    // CSN = 0;
    CSN_SET(0);
    status = SPI_RW(reg);
    for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        pBuf[byte_ctr] = SPI_RW(0); //读取数据，低字节在前

    // CSN = 1;
    CSN_SET(1);
    return (status);
}

/********************************************************
  函数功能：nRF24L01+接收模式初始化                      
  入口参数：无
  返回  值：无
 *********************************************************/
void nRF24L01P_RX_Mode(u8 *address)
{
    // CE = 0;
    CSN_SET(0);

    nRF24L01P_Write_Buf(WRITE_REG + RX_ADDR_P0, address, RX_ADR_WIDTH); // 写接收地址
    nRF24L01P_Write_Reg(WRITE_REG + EN_AA, 0x01);                       // 使能接收通道0自动应答
    nRF24L01P_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);                   // 使能接收通道0
    nRF24L01P_Write_Reg(WRITE_REG + RF_CH, channal);                    // 频道选择初值为0
    nRF24L01P_Write_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);          // 接收通道0选择和发送通道相同有效数据宽度
    nRF24L01P_Write_Reg(WRITE_REG + RF_SETUP, Rf_Power);                // 2Mbps，0dBm，低噪声放大器增益
    nRF24L01P_Write_Reg(WRITE_REG + CONFIG, 0x0f);                      // CRC使能，16位CRC校验，上电，接收模式
    nRF24L01P_Write_Reg(WRITE_REG + STATUS, 0xff);                      //清除所有的中断标志位
    nRF24L01P_Write_Reg(FLUSH_RX, 0xff);                                //清除RX FIFO寄存器

    // CE = 1; // 拉高CE启动接收设备
    CSN_SET(1);
}

/********************************************************
  函数功能：nRF24L01+发送模式初始化                      
  入口参数：无
  返回  值：无
 *********************************************************/
void nRF24L01P_TX_Mode(u8 *address)
{
    // CE = 0;
    CSN_SET(0);

    nRF24L01P_Write_Buf(WRITE_REG + TX_ADDR, address, TX_ADR_WIDTH);    // 写入发送地址
    nRF24L01P_Write_Buf(WRITE_REG + RX_ADDR_P0, address, RX_ADR_WIDTH); //接收通道0地址和发送地址相同
    nRF24L01P_Write_Reg(WRITE_REG + EN_AA, 0x01);                       // 使能接收通道0自动应答
    nRF24L01P_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);                   // 使能接收通道0
    nRF24L01P_Write_Reg(WRITE_REG + RF_CH, channal);                    // 频道选择，初值为10
    nRF24L01P_Write_Reg(WRITE_REG + SETUP_RETR, 0x03);                  // 自动重发3次
    nRF24L01P_Write_Reg(WRITE_REG + RF_SETUP, Rf_Power);                //2Mbps，低噪声放大器增益 0x09 -18db  0x0b -12db  0x0d -6db  0x0f 0db
    nRF24L01P_Write_Reg(WRITE_REG + CONFIG, 0x0e);                      // send mode CRC使能，16位CRC校验，上电

    // CE = 1;
    CSN_SET(1);
}

/********************************************************
  函数功能：读取接收数据                       
  入口参数：rxbuf:接收数据存放首地址
  返回  值：0:接收到数据
1:没有接收到数据
 *********************************************************/
u8 nRF24L01P_RxPacket(u8 *rxbuf)
{
    u8 state;
    state = nRF24L01P_Read_Reg(STATUS);             //读取状态寄存器的值,1设置读取目标寄存器地址，通过写0，读出值
    nRF24L01P_Write_Reg(WRITE_REG + STATUS, state); //清除RX_DS中断标志，当有数据时，写1清除。

    if (state & RX_DR) //接收到数据
    {
        nRF24L01P_Read_Buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH); //读取数据
        nRF24L01P_Write_Reg(FLUSH_RX, 0xff);                    //清除RX FIFO寄存器
        return 0;
    }

    return 1; //没收到任何数据
}

/********************************************************
  函数功能：发送一个数据包                      
  入口参数：txbuf:要发送的数据
  返回  值：0x10:达到最大重发次数，发送失败 
0x20:发送成功            
0xff:发送失败                  
 *********************************************************/
u8 nRF24L01P_TxPacket(u8 *txbuf)
{
    u8 state;

    // CE = 0;                                                  //CE拉低，使能24L01配置
    CSN_SET(0);
    nRF24L01P_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); //写数据到TX FIFO,32个字节
    // CE = 1;                                                  //CE置高，使能发送
    CSN_SET(1);

    while (IRQ_READ() != 0)
        ; //等待发送完成

    state = nRF24L01P_Read_Reg(STATUS);             //读取状态寄存器的值
    nRF24L01P_Write_Reg(WRITE_REG + STATUS, state); //清除TX_DS或MAX_RT中断标志

    if (state & MAX_RT) //达到最大重发次数
    {
        nRF24L01P_Write_Reg(FLUSH_TX, 0xff); //清除TX FIFO寄存器
        return MAX_RT;
    }
    else if (state & TX_DS) //发送成功
    {
        return TX_DS;
    }
    else
        return 0XFF; //发送失败

    return 0xFF;
}

/** ******************************************************* */
/** 功能：SPI检测 */
/** 参数：无 */
/** 返回值： 1：SPI通讯错误  0:SPI通讯成功 */
/** ******************************************************* */
u8 Spi_Test(void)
{
    u8 buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
    u8 i;
    nRF24L01P_Write_Buf(WRITE_REG + TX_ADDR, buf, 5); //写入5个字节的地址. */
    nRF24L01P_Read_Buf(TX_ADDR, buf, 5);              //读出写入的地址 */
    for (i = 0; i < 5; i++)
        if (buf[i] != 0XA5)
            break;
    if (i != 5)
        return 1; //检测24L01错误 */
    return 0;     //检测到24L01 */
}
