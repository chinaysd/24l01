
#include "sys.h"
#include "delay.h"
#include "usart1.h"
#include "nRF24L01.h"
#include<stdlib.h>   //包含函数rand（）声明的头文件


int main()
{	
	u8 i,sta,channel,rx_len,ack_len;	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置NVIC中断分组4:  16位抢占优先级0,15最好不用
	delay_init();

	USART1_Init(115200);
	printf("OK...\r\n\r\n\r\n");
 

    NRF24L01_Init();    		//初始化NRF24L01 

	while(NRF24L01_Check())
	{
		printf("NRF24L01 Error\r\n");
 		delay_ms(1000);
	}
	printf("RX NRF24L01 OK\r\n");
  
	NRF24L01_RX_Mode();
	
	while(1)
	{	  
		sta=NRF24L01_RxPacket(&channel,RX_BUF,&rx_len);
		if(sta&RX_OK)              
		{	
			printf("RX_OK       RX_BUF:");
			for(i=0;i<rx_len;i++)printf("%d,",RX_BUF[i]);printf("\r\n");    //如果接收到数据，显示数据
 
			//也可以随时填充ACK数据收到直接就返回了，不用等收到数据填充，下次返回了
			ack_len=10;                                                     //定义ack数据长度为10
			for(i=0;i<ack_len;i++)
				TX_BUF[i]=RX_BUF[i];
			TX_BUF[ack_len-1]=rand()%10;//赋值ack数据，前9个是接收的数据，最后一个随机
				
			printf("TX_ACK_BUF  TX_BUF:");                                  //显示一下
			for(i=0;i<ack_len;i++)
				printf("%d,",TX_BUF[i]);
			printf("\r\n");
			printf("\r\n");	
			NRF24L01_PacketAckData(TX_BUF,ack_len);//送入ACK发送缓存
		}
	}
}
