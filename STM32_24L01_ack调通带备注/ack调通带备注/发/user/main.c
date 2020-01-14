
#include "sys.h"
#include "delay.h"
#include "usart1.h"
#include "nRF24L01.h"
#include <stdlib.h> //包含函数rand（）声明的头文件

int main()
{
	u8 i, sta, tx_len;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置NVIC中断分组4:  16位抢占优先级0,15最好不用
	delay_init();
	USART1_Init(115200);
	printf("OK...\r\n\r\n\r\n");

	NRF24L01_Init(); //初始化NRF24L01

	while (NRF24L01_Check())
	{
		printf("NRF24L01 Error\r\n");
		delay_ms(1000);
	}
	printf("TX NRF24L01 OK\r\n");

	NRF24L01_TX_Mode();

	while (1)
	{
		tx_len++;
		if (tx_len > 32)
			tx_len = 1; //测试变数据长度发送，每次长度加1，最大32，变为1
		for (i = 1; i < tx_len; i++)
			TX_BUF[i] = i;
		TX_BUF[0]++;
		TX_BUF[tx_len - 1] = rand() % 10; //赋值发送的数据	，第一个数每次加1改变，最后一个数随机，中间的1 2 3、、、

		sta = NRF24L01_TxPacket(TX_BUF, tx_len, RX_BUF, &RX_ACK_NUM); //发送数据

		if (sta & TX_OK) //显示要发送的数据
		{
			printf("TX_OK       TX_BUF:");
			for (i = 0; i < tx_len; i++)
				printf("%d,", TX_BUF[i]);
			printf("\r\n");
		}

		if (sta & RX_OK) //显示接收到的ack数据
		{
			printf("RX_ACK_OK   RX_BUF:");
			for (i = 0; i < RX_ACK_NUM; i++)
				printf("%d,", RX_BUF[i]);
			printf("\r\n");
		}
		printf("\r\n");

		delay_ms(10);
	}
}
