#include "app.h"

TIMEOUT_PARA TimeOut_Para[2];
u8 Mode = 0;                                   //模式标志设置为0 接收端
u8 tmp_buf_Tx[5], tmp_buf_Rx[5];               //发送接收缓冲数组
u8 channal_buf[6] = {10, 30, 40, 60, 80, 100}; // 跳频频道
uint8_t t;

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

// void delay_ms(uint16_t time)
// {
//     static uint16_t i, j;
//     for (i = time; i > 0; i--)
//     {
//         for (j = 200; j > 0; j--)
//             ;
//     }
// }

void App_Init(void)
{
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    TimeOutDet_Init();
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);
    Timer_Init();
    Uart_Init();
    Init_NRF24L01();
    enableInterrupts();
    while (NRF24L01_Check()) //检测不到NRF24L01
    {
        Debug_Cfg("=========================success============================\n");
        delay_ms(100);
    }
    //接收模式
    RX_Mode();

    OLED_Init();
    OLED_Clear();
    OLED_ShowString(30, 0, "EMOMO TEST");
    OLED_ShowString(8, 2, "ZHONGJINGYUAN");
    OLED_ShowString(20, 4, "2020/01/13");
    OLED_ShowString(0, 6, "ASCII:");
    OLED_ShowString(63, 6, "CODE:");
    t = ' ';
    OLED_ShowCHinese(0, 0, 0);
    OLED_ShowCHinese(18, 0, 1);
    OLED_ShowCHinese(36, 0, 2);
    OLED_ShowCHinese(54, 0, 3);
    OLED_ShowCHinese(72, 0, 4);
    OLED_ShowCHinese(90, 0, 5);
    OLED_ShowCHinese(108, 0, 6);

    t = ' ';
}

void App_Handle(void)
{
    static uint8_t j;
    if (Mode == 1)
    {
        /*这里可以更新要发送的数据*/
        tmp_buf_Tx[0] = 0xa5;
        tmp_buf_Tx[1] = 0x01;
        tmp_buf_Tx[2] = 0x02;
        tmp_buf_Tx[3] = (uint8_t)(tmp_buf_Tx[0] + tmp_buf_Tx[1] + tmp_buf_Tx[2]);
        tmp_buf_Tx[4] = 0xfb;
        if (NRF24L01_TxPacket(tmp_buf_Tx) == TX_OK) //发送数据成功
        {
            Mode = 0;  //转变为接收模式
            RX_Mode(); //一旦发送成功则变成接收模式；
            Debug_Cfg("=========================success1============================\n");
        }
    }
    else
    {
        if (NRF24L01_RxPacket(tmp_buf_Rx) == 0) //一旦接收成功则变成发送模式；
        {
            Mode = 1;
            TX_Mode();
            Debug_Cfg("=========================success2============================\n");
            if (tmp_buf_Tx[0] != 0xa5)
            {
                return;
            }
            if (tmp_buf_Tx[4] != 0xfb)
            {
                return;
            }
            if (tmp_buf_Tx[1] == 0x01)
            {
                Debug_Cfg("=========================success3============================\n");
            }
        }
    }
    // if (TimeOutDet_Check(&TimeOut_Para[0]))
    // {
    //     TimeOut_Record(&TimeOut_Para[0], 12);
    //     Channal_Change(channal_buf[j++]);
    //     if (j > 5)
    //     {
    //         j = 0;
    //     }
    // }

    OLED_ShowCHinese(0, 0, 0);
    OLED_ShowCHinese(18, 0, 1);
    OLED_ShowCHinese(36, 0, 2);
    OLED_ShowCHinese(54, 0, 3);
    OLED_ShowCHinese(72, 0, 4);
    OLED_ShowCHinese(90, 0, 5);
    OLED_ShowCHinese(108, 0, 6);
    OLED_ShowString(0, 2, "EMOMO WELCOM U");
    // OLED_ShowString(8,2,"ZHONGJINGYUAN");
    OLED_ShowString(20, 4, "2020/01/13");
    OLED_ShowString(0, 6, "ASCII:");
    OLED_ShowString(63, 6, "CODE:");
    OLED_ShowChar(48, 6, t);
    t++;
    if (t > '~')
        t = ' ';
    OLED_ShowNum(103, 6, t, 3, 16);
}
