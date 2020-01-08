#include "app.h"

TIMEOUT_PARA TimeOut_Para[2];
uint8_t data_receive_buf[10];
u8 rx_buf[10]; // 接收数据变量,数据一次最大只能发32个字节
u8 tx_buf[10];

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

void App_Init(void)
{
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    TimeOutDet_Init();
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);
    Timer_Init();
    Uart_Init();
    // GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
    Init_NRF24L01();
    enableInterrupts();
    while (NRF24L01_Check()) //检测不到NRF24L01
    {
        Debug_Cfg("=========================success============================\n");
        delay_ms(100);
    }
    //接收模式
    RX_Mode();
}

void App_Handle(void)
{
    if (NRF24L01_RxPacket(rx_buf) == 0) //一旦接收到信息,则显示出来.
    {
        // printf("\r\n接收到数据为:%s", rx_buf);
        Debug_Cfg("=========================success1============================\n");
        if (rx_buf[0] == 0x01)
        {

            GPIO_WriteReverse(GPIOC, GPIO_PIN_7);
            UART1_SendData8(rx_buf[0]);
        }
        memcpy(tx_buf, rx_buf, 10);
        // delay_ms(250);
        //发送模式
        TX_Mode();
        if (NRF24L01_TxPacket(tx_buf) == TX_OK) //发送完成
        {
            // printf("发送数据：%s\r\n", tx_buf);
            Debug_Cfg("=========================success2============================\n");
        }

        RX_Mode();
    }
}
