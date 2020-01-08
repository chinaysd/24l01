#ifndef _APP_H_
#define _APP_H_

#include "stm8s.h"
#include "TimeOut.h"
#include "stdio.h"
#include "NRF24L01.h"
#include "string.h"

typedef enum
{
  UART0_ID,
  UART1_ID1
} UART_DEV_t;

void App_Init(void);
void App_Handle(void);

#endif
