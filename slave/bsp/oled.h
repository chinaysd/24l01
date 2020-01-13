#ifndef __OLED_H
#define __OLED_H
#include "stm8s.h"

#define OLED_CMD 0
#define OLED_DATA 1
#define OLED_MODE 0

#define OLED_SCLK_PORT (GPIOC)
#define OLED_SCLK_PINS (GPIO_PIN_7)

#define OLED_SDIN_PORT (GPIOC)
#define OLED_SDIN_PINS (GPIO_PIN_6)

#define OLED_RST_PORT (GPIOC)
#define OLED_RST_PINS (GPIO_PIN_5)

#define OLED_DC_PORT (GPIOC)
#define OLED_DC_PINS (GPIO_PIN_4)

#define OLED_CS_PORT (GPIOC)
#define OLED_CS_PINS (GPIO_PIN_3)

#define OLED_SCLK_Clr() GPIO_WriteLow(OLED_SCLK_PORT, OLED_SCLK_PINS)
#define OLED_SCLK_Set() GPIO_WriteHigh(OLED_SCLK_PORT, OLED_SCLK_PINS)

#define OLED_SDIN_Clr() GPIO_WriteLow(OLED_SDIN_PORT, OLED_SDIN_PINS)
#define OLED_SDIN_Set() GPIO_WriteHigh(OLED_SDIN_PORT, OLED_SDIN_PINS)

#define OLED_RST_Clr() GPIO_WriteLow(OLED_RST_PORT, OLED_RST_PINS)
#define OLED_RST_Set() GPIO_WriteHigh(OLED_RST_PORT, OLED_RST_PINS)

#define OLED_DC_Clr() GPIO_WriteLow(OLED_DC_PORT, OLED_DC_PINS)
#define OLED_DC_Set() GPIO_WriteHigh(OLED_DC_PORT, OLED_DC_PINS)

#define OLED_CS_Clr() GPIO_WriteLow(OLED_CS_PORT, OLED_CS_PINS)
#define OLED_CS_Set() GPIO_WriteHigh(OLED_CS_PORT, OLED_CS_PINS)

#define SIZE 16
#define XLevelL 0x02
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

void delay_ms(uint16_t ms);
void Delay(uint16_t nCount);

void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *p);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);
#endif
