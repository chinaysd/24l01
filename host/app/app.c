#include "app.h"
#include "stdarg.h"

TIMEOUT_PARA TimeOut_Para[2];
u8 channal_buf[6] = {10, 30, 40, 60, 80, 100}; // 跳频频道

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

void App_Init(void)
{
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    TimeOutDet_Init();
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);
    Timer_Init();
    Uart_Init();
    // GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
    Init_NRF24L01();
    TX_Mode();
    enableInterrupts();
    if (NRF24L01_Check())
    {
        GPIO_WriteLow(GPIOC, GPIO_PIN_7);
        // Debug_Cfg("===================  Si24R1 Tx TEST  ===============\n");
        // Debug_Cfg("===================        NO ACK       ===============\n");
        // Debug_Cfg("=======================================================\n");
    }
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
#define Max 2                      //接收或发送失败后重试的最大次数
u8 Tx_Cnt = 0;                     //发送次数计数
u8 Rx_Cnt = 0;                     //接收次数计数
u8 Mode = 1;                       //Mode为1表示发送模式，0表示接收模式
u8 tmp_buf_Tx[32], tmp_buf_Rx[32]; //发送接收缓冲数组

void App_Handle(void)
{
    static uint8_t j, i;
    if (GPIO_ReadInputPin(GPIOB, GPIO_PIN_4) == 0)
    {
        Channal_Change(10);
    }
    if (GPIO_ReadInputPin(GPIOB, GPIO_PIN_5) == 0)
    {
        Channal_Change(30);
    }
    // if (TimeOutDet_Check(&TimeOut_Para[0]))
    {
        // TimeOut_Record(&TimeOut_Para[0], 20);
        if (Mode == 1) //发送模式下
        {
            tmp_buf_Tx[0] = 0xa5;
            tmp_buf_Tx[1] = 0x01;
            tmp_buf_Tx[2] = 0x02;
            tmp_buf_Tx[3] = (uint8_t)(tmp_buf_Tx[0] + tmp_buf_Tx[1] + tmp_buf_Tx[2]);
            tmp_buf_Tx[4] = 0xfb;
            if (NRF24L01_TxPacket(tmp_buf_Tx) == TX_OK)
            {
                Tx_Cnt = 0;
                Mode = 0;
                RX_Mode(); //一旦发送成功则变成接收模式；
                Debug_Cfg("===================  Si24R1 Tx TEST1  ===============\n");
                // delay_ms(5);
                // GPIO_WriteHigh(GPIOC, GPIO_PIN_7);
            }

            Tx_Cnt++;
            if (Tx_Cnt == Max) //如果连续发送Max次都失败，则切换为接收模式
            {
                Tx_Cnt = 0;
                Mode = 0;
                RX_Mode();
                // Debug_Cfg("===================  Si24R1 Tx TEST2  ===============\n");
                // delay_ms(20);
            }
        }
        else //接收模式下
        {
            if (NRF24L01_RxPacket(tmp_buf_Rx) == 0) //一旦接收成功则变成发送模式；
            {
                Debug_Cfg("===================  Si24R1 Tx TEST3  ===============\n");
                // GPIO_WriteLow(GPIOC, GPIO_PIN_7);
                Rx_Cnt = 0;
                Mode = 1;
                TX_Mode();
                // delay_ms(5);
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
                    // GPIO_WriteReverse(GPIOC, GPIO_PIN_7);
                    // Debug_Cfg("===================  Si24R1 Tx TEST5  ===============\n");
                }
            }
            Rx_Cnt++;
            if (Rx_Cnt == Max) //如果连续接收Max次都失败，则切换为发送模式
            {
                // delay_ms(20);
                // Debug_Cfg("===================  Si24R1 Tx TEST4  ===============\n");
                Rx_Cnt = 0;
                Mode = 1;
                TX_Mode();
            }
        }
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
