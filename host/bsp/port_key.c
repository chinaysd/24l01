#include "stm8s.h"

#define KEY1_PORT GPIOB
#define KEY1_PIN GPIO_PIN_4

#define KEY2_PORT GPIOB
#define KEY2_PIN GPIO_PIN_5

typedef struct
{
    GPIO_TypeDef *GPIOx;
    GPIO_Pin_TypeDef PortPins;
} key_info_t;

static key_info_t key_info[] = {
    {KEY1_PORT, KEY1_PIN},
    {KEY2_PORT, KEY2_PIN},
};

void Port_Key_Init(void)
{
    char i;
    for (i = 0; i < (sizeof(key_info) / sizeof(key_info[0])); i++)
    {
        GPIO_Init(key_info[i].GPIOx, key_info[i].PortPins, GPIO_MODE_IN_PU_NO_IT);
    }
}

unsigned char Port_Get_KeyIndex(void)
{
    unsigned char value = 0, cnt = 0;

    if (!GPIO_ReadInputPin(KEY1_PORT, KEY1_PIN))
    {
        value = 1, cnt++;
    }

    if (!GPIO_ReadInputPin(KEY2_PORT, KEY2_PIN))
    {
        value = 2, cnt++;
    }

    if (cnt > 1)
    {
        value = 0;
    }
    return value;
}
