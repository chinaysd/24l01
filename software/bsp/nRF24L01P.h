#ifndef __nRF24L01P__
#define __nRF24L01P__

#include "stm8s.h"

//nRF24L01+ PIN DEFINITION

/*79F166A  2.4G模块的定义*/

#define IRQ_PORT GPIOD
#define IRQ_PIN GPIO_PIN_3
#define IRQ_SET(X) ((X) ? (GPIO_WriteHigh(IRQ_PORT, IRQ_PIN)) : (GPIO_WriteLow(IRQ_PORT, IRQ_PIN)))
#define IRQ_READ() GPIO_ReadInputPin(IRQ_PORT, IRQ_PIN)

#define MISO_PORT GPIOD
#define MISO_PIN GPIO_PIN_2
#define MISO_SET(X) ((X) ? (GPIO_WriteHigh(MISO_PORT, MISO_PIN)) : (GPIO_WriteLow(MISO_PORT, MISO_PIN)))
#define MISO_READ() GPIO_ReadInputPin(MISO_PORT, MISO_PIN)

#define CE_PORT GPIOD
#define CE_PIN GPIO_PIN_4
#define CE_SET(X) ((X) ? (GPIO_WriteHigh(CE_PORT, CE_PIN)) : (GPIO_WriteLow(CE_PORT, CE_PIN)))

#define CSN_PORT GPIOA
#define CSN_PIN GPIO_PIN_1
#define CSN_SET(X) ((X) ? (GPIO_WriteHigh(CSN_PORT, CSN_PIN)) : (GPIO_WriteLow(CSN_PORT, CSN_PIN)))

#define SCK_PORT GPIOA
#define SCK_PIN GPIO_PIN_2
#define SCK_SET(X) ((X) ? (GPIO_WriteHigh(SCK_PORT, SCK_PIN)) : (GPIO_WriteLow(SCK_PORT, SCK_PIN)))

#define MOSI_PORT GPIOA
#define MOSI_PIN GPIO_PIN_3
#define MOSI_SET(X) ((X) ? (GPIO_WriteHigh(MOSI_PORT, MOSI_PIN)) : (GPIO_WriteLow(MOSI_PORT, MOSI_PIN)))

#define TX_ADR_WIDTH 5  // 5字节宽度的发送地址
#define RX_ADR_WIDTH 5  // 5字节宽度的接收地址
#define NRF_ADR_WIDTH 5 // 5字节宽度的接收地址

#define TX_PLOAD_WIDTH 5 // 发送数据通道有效数据宽度
#define RX_PLOAD_WIDTH 5 // 发送数据通道有效数据宽度

#define HighPowerMode 0x0f
#define LowPowerMode 0x09

//********************************************************************************************************************//
// SPI(nRF24L01+) commands
#define READ_REG 0x00           // Define read command to register
#define WRITE_REG 0x20          // Define write command to register
#define RD_RX_PLOAD 0x61        // Define RX payload register address
#define WR_TX_PLOAD 0xA0        // Define TX payload register address
#define FLUSH_TX 0xE1           // Define flush清除 TX register command
#define FLUSH_RX 0xE2           // Define flush清除 RX register command
#define REUSE_TX_PL 0xE3        // Define reuse TX payload register command
#define R_RX_PL_WID 0x60        // Define Read RX payload width for the R_RX_PAYLOAD in the RX FIFO
#define W_ACK_PAYLOAD 0xA8      // Define Write Payload to be transmitted together with ACK packet on PIPExxx xxx valid in the range from 000 to 101)
#define W_TX_PAYLOAD_NOACK 0xB0 // Define Write TX Disables AUTOACK on this specific packet
#define NOP 0xFF                // Define No Operation, might be used to read status register

//********************************************************************************************************************//
// SPI(nRF24L01+) registers(addresses)
#define CONFIG 0x00      // 'Config' register address
#define EN_AA 0x01       // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR 0x02   // 'Enabled RX addresses' register address
#define SETUP_AW 0x03    // 'Setup address width' register address
#define SETUP_RETR 0x04  // 'Setup Auto. Retrans' register address
#define RF_CH 0x05       // 'RF channel' register address
#define RF_SETUP 0x06    // 'RF setup' register address
#define STATUS 0x07      // 'Status' register address
#define OBSERVE_TX 0x08  // 'Observe TX' register address
#define RPD 0x09         // 'Received Power Detector' register address
#define RX_ADDR_P0 0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1 0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2 0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3 0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4 0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5 0x0F  // 'RX address pipe5' register address
#define TX_ADDR 0x10     // 'TX address' register address
#define RX_PW_P0 0x11    // 'RX payload width, pipe0' register address
#define RX_PW_P1 0x12    // 'RX payload width, pipe1' register address
#define RX_PW_P2 0x13    // 'RX payload width, pipe2' register address
#define RX_PW_P3 0x14    // 'RX payload width, pipe3' register address
#define RX_PW_P4 0x15    // 'RX payload width, pipe4' register address
#define RX_PW_P5 0x16    // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17 // 'FIFO Status Register' register address
#define DYNPD 0x1C       // '使能动态负载长度
#define FEATURE 0x1D     // 'b0=使能命令W_TX_PAYLOAD_NOACK
//********************************************************************************************************************//
// STATUS Register
#define RX_DR 0x40 /**/
#define TX_DS 0x20
#define MAX_RT 0x10

//********************************************************************************************************************//
//                                        FUNCTION's PROTOTYPES                                                       //
//********************************************************************************************************************//
//nRF24L01+ API Functions
void nRF24L01P_Init(void);
u8 nRF24L01P_Write_Reg(u8 reg, u8 value);
u8 nRF24L01P_Write_Buf(u8 reg, const u8 *pBuf, u8 bytes);
u8 nRF24L01P_Read_Reg(u8 reg);
u8 nRF24L01P_Read_Buf(u8 reg, u8 *pBuf, u8 bytes);

u8 Spi_Test(void);

void nRF24L01P_RX_Mode(u8 *address);
void nRF24L01P_TX_Mode(u8 *address);
u8 nRF24L01P_RxPacket(u8 *rxbuf);
u8 nRF24L01P_TxPacket(u8 *txbuf);

extern u8 NRF_ADDRESS[];
extern u8 channal;
extern u8 Rf_Power;

//********************************************************************************************************************//
#endif
