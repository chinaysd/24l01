#include "app.h"

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
    nRF24L01P_Init();
    nRF24L01P_TX_Mode(NRF_ADDRESS);
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);
    Timer_Init();
    Uart_Init();
    enableInterrupts();
    Debug_Cfg("\n=======================================================\n");
    Debug_Cfg("===================  Si24R1 Tx TEST  ===============\n");
    Debug_Cfg("===================        NO ACK       ===============\n");
    Debug_Cfg("=======================================================\n");
}

void App_Handle(void)
{
    // Spi_Test();
    if (GPIO_ReadInputPin(GPIOB, GPIO_PIN_4) == 0)
    {
        Debug_Cfg("=========================OK============================\n");
        Address_Confirm_Mode(0x01);
    }
}
