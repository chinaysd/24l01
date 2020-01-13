#include "app.h"
#include "stdarg.h"

TIMEOUT_PARA TimeOut_Para[2];
u8 channal_buf[6] = {10, 30, 40, 60, 80, 100}; //
#define Max 1                                  //接收或发送失败后重试的最大次数
u8 Tx_Cnt = 0;                                 //发送次数计数
u8 Rx_Cnt = 0;                                 //接收次数计数
u8 Mode = 1;                                   //Mode为1表示发送模式，0表示接收模式
u8 tmp_buf_Tx[5], tmp_buf_Rx[5];               //发送接收缓冲数组
uint8_t j;

void Debug_Cfg(uint8_t *u_buf)
{
    while (*u_buf != '\0')
    {
        UART1_SendData8(*u_buf);
        while (RESET == UART1_GetFlagStatus(UART1_FLAG_TXE))
            ;
        *u_buf++;
    }
}

static void Timer_Init(void)
{
#define TIM4_PERIOD 124
    /* TIM4 configuration:
    - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
    clock used is 16 MHz / 128 = 125 000 Hz
    - With 125 000 Hz we can generate time base:
        max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
        min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
    - In this example we need to generate a time base equal to 1 ms
    so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

    /* Time base configuration */
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
    /* Clear TIM4 update flag */
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    /* Enable update interrupt */
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);

    /* enable interrupts */
    // enableInterrupts();

    /* Enable TIM4 */
    TIM4_Cmd(ENABLE);
}

static void Uart_Init(void)
{
    UART1_DeInit();
    UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE);
    UART1_Cmd(ENABLE);
}

void delay_ms(uint16_t time)
{
    static uint16_t i, j;
    for (i = time; i > 0; i--)
    {
        for (j = 200; j > 0; j--)
            ;
    }
}

void NRF24L01_DataHdl(uint16_t tx_dat)
{
    if (Mode == 1) //发送模式下
    {
        tmp_buf_Tx[0] = 0xa5;
        tmp_buf_Tx[1] = tx_dat >> 8;
        tmp_buf_Tx[2] = tx_dat;
        tmp_buf_Tx[3] = (uint8_t)(tmp_buf_Tx[0] + tmp_buf_Tx[1] + tmp_buf_Tx[2]);
        tmp_buf_Tx[4] = 0xfb;
        if (NRF24L01_TxPacket(tmp_buf_Tx) == TX_OK)
        {
            Tx_Cnt = 0;
            Mode = 0;
            RX_Mode(); //一旦发送成功则变成接收模式；
            Debug_Cfg("===================  Si24R1 Tx TEST1  ===============\n");
            GPIO_WriteReverse(GPIOC, GPIO_PIN_7);
        }
        else
        {
            Mode = 0;
            RX_Mode();
        }
    }
    else //接收模式下
    {
        if (NRF24L01_RxPacket(tmp_buf_Rx) == 0) //一旦接收成功则变成发送模式；
        {
            // Debug_Cfg("===================  Si24R1 Tx TEST3  ===============\n");
            Rx_Cnt = 0;
            Mode = 1;
            TX_Mode();
            if (tmp_buf_Rx[0] != 0xa5)
            {
                return;
            }
            if (tmp_buf_Rx[4] != 0xfb)
            {
                return;
            }
            if (tmp_buf_Rx[1] == 0x01)
            {
                // Debug_Cfg("===================  Si24R1 Tx TEST5  ===============\n");
            }
        }
        else
        {
            Mode = 1;
            TX_Mode();
        }
    }
}

void App_Init(void)
{
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    TimeOutDet_Init();
    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);
    Bsp_Key_Init();
    Timer_Init();
    Uart_Init();
    Init_NRF24L01();
    TX_Mode();
    enableInterrupts();
    if (NRF24L01_Check())
    {
        GPIO_WriteLow(GPIOC, GPIO_PIN_7);
    }
}

void App_Handle(void)
{
    key_msg_t key_value;
    key_value = Bsp_Key_Scan();
    switch (key_value)
    {
    case MSG_K1_PRESS:
        NRF24L01_DataHdl(0x0101);
        break;
    case MSG_K2_PRESS:
        NRF24L01_DataHdl(0x0102);
        break;
    default:
        break;
    }

    // if (TimeOutDet_Check(&TimeOut_Para[0]))
    // {
    //     TimeOut_Record(&TimeOut_Para[0], 2);
    //     Channal_Change(channal_buf[j++]);
    //     if (j > 5)
    //     {
    //         j = 0;
    //     }
    // }
}
