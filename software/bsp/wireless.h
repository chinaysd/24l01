#ifndef WIRELESS_H_
#define WIRELESS_H_

#include "stm8s.h"

void Key_Send(uint8_t key);
void Address_Confirm_Mode(uint8_t key);
static void Channal_Change(u8 num);
u8 Hopping_Tx(u8 *address, u8 *txbuf);
void Power_Low_Mode();

#endif /* ifndef WIRELESS_H_ */
