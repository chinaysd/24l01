#include "wireless.h"
#include "nRF24L01P.h"
#include "TimeOut.h"

u8 Tx_Data[5] = {0};
u8 Rx_Buf[5] = {0};
u8 match_buf[5] = {0};
u8 pid = 0;
u8 channal_buf[6] = {1, 25, 49, 73, 97, 121};
u8 FIXED_ADDRESS[TX_ADR_WIDTH] = {0X11, 0X25, 0X52, 0X12, 0X10};

u8 NRF_ADDRESS[NRF_ADR_WIDTH] = {0x11, 0x02, 0x03, 0x04, 0x05};

u8 Hopping_Tx(u8 *address, u8 *txbuf);

void delay_us(short num) // num(us)
{

    int i = 12 * num; // 粗延迟，一条指令80ns

    while (i--)
        ;
}

void Key_Send(uint8_t key)
{
    Tx_Data[1] = key;
    Tx_Data[2] = pid++;

    Tx_Data[0] = 0xA5;
    Tx_Data[3] = (u8)(Tx_Data[1] + Tx_Data[2]);
    Tx_Data[4] = 0xFB;

    if (Hopping_Tx(NRF_ADDRESS, Tx_Data))
    {
#if 0
        domain_zero = Rx_Buf[0];
        domain_one = Rx_Buf[1];
        domain_two = Rx_Buf[2];
        domain_three = Rx_Buf[3];
        domain_four = Rx_Buf[4];
#endif
        nRF24L01P_TX_Mode(NRF_ADDRESS);
    }
}

void Address_Confirm_Mode(uint8_t key)
{

    TIMEOUT_PARA Confirme_timer;
    TIMEOUT_PARA match_timer;
    // P2_5 = 1;

    channal = 25;

    nRF24L01P_TX_Mode(FIXED_ADDRESS);

    delay_us(10);

    TimeOut_Record(&Confirme_timer, 6000);
    while (1)
    {

        // RSTSTAT = 0x03;

        if (nRF24L01P_TxPacket(NRF_ADDRESS) == 0x20)
        {
            // CE = 0;
            CE_SET(0);
            nRF24L01P_Write_Reg(FLUSH_TX, 0xff);
            // CE = 1;
            CE_SET(1);

            nRF24L01P_RX_Mode(FIXED_ADDRESS);
            TimeOut_Record(&match_timer, 10);
            while (nRF24L01P_RxPacket(match_buf))
            {
                u8 cs1, cs2, i;
                for (i = 0; i < 5; i++)
                {
                    cs1 += match_buf[i];
                    cs2 += NRF_ADDRESS[i];
                }

                if (cs1 == cs2)
                {
                    // P2_5 = 0;
                    break;
                }

                if (TimeOutDet_Check(&match_timer))
                {
                    nRF24L01P_TX_Mode(NRF_ADDRESS);
                    break;
                }
            }
        }

        if (TimeOutDet_Check(&Confirme_timer))
        {

            // CE = 0;
            CE_SET(0);
            nRF24L01P_Write_Reg(FLUSH_TX, 0xff);
            // CE = 1;
            CE_SET(1);
            while (1)
                ;
        }

        delay_us(10);
    }
}

static void Channal_Change(u8 num)
{
    channal = num;
    // CE = 0;
    CE_SET(0);
    nRF24L01P_Write_Reg(WRITE_REG + RF_CH, channal);
    // CE = 1;
    CE_SET(1);
}

u8 Hopping_Tx(u8 *address, u8 *txbuf)
{
    TIMEOUT_PARA Rx_timer, Tx_timer;

    u8 flag;
    static u8 j = 0;

    TimeOut_Record(&Tx_timer, 100);

    while (1)
    {

        if (TimeOutDet_Check(&Tx_timer))
            return 0;

        if (j > 5)
            j = 0;

        if (nRF24L01P_TxPacket(txbuf) == 0x20)
        {
            flag = 0;

            nRF24L01P_RX_Mode(address);

            TimeOut_Record(&Rx_timer, 5);
            while (nRF24L01P_RxPacket(Rx_Buf))
            {
                if (TimeOutDet_Check(&Rx_timer))
                {
                    flag = 1;
                    nRF24L01P_TX_Mode(address);
                    break;
                }
            }

            if (!flag)
                return 1;
        }
        else
            Channal_Change(channal_buf[j++]);

        delay_us(10);
    }
}

// void Power_Low_Mode()
// {

//     MOSI = 0;
//     CSN = 0; //IO锟斤拷锟斤拷偷锟狡�
//     SCK = 0;
//     CE = 0;
//     nRF24L01P_Write_Reg(WRITE_REG + STATUS, 0xff);
//     nRF24L01P_Write_Reg(WRITE_REG + CONFIG, 0x00); // nrf锟斤拷锟斤拷凸锟斤拷锟侥Ｊ�

//     P0_2 = 0; // 锟截憋拷锟斤拷压锟斤拷路

//     P3 &= 0xf0;
//     P1 |= 0x3f; // 锟截憋拷LED

//     ET2 = 0;
//     ET5 = 0; //锟截闭讹拷时锟斤拷锟叫讹拷

//     INSCON = 0x40;
//     P5CR = 0x03;
//     KEYV1 = 0;
//     KEYV2 = 0;
//     INSCON = 0x00;
//     P1CR |= 0xC0;
//     KEYV3 = 0;
//     KEYV4 = 0; // 锟斤拷锟斤拷锟斤拷锟叫帮拷锟斤拷锟斤拷锟�

//     EXF0 = 0x40; // 锟斤拷锟斤拷锟解部锟叫讹拷通锟斤拷4锟铰斤拷锟截达拷锟斤拷
//     EXF1 = 0x00; // 锟斤拷锟斤拷獠�4锟叫断憋拷志 1锟斤拷锟叫讹拷锟斤拷锟斤拷 锟斤拷4位锟斤拷锟斤拷
//     IENC = 0X0f; //锟斤拷锟斤拷锟解部锟叫讹拷4 锟斤拷锟斤拷通锟斤拷 锟斤拷4位锟斤拷锟斤拷  锟斤拷锟斤拷位通锟斤拷选锟斤拷

//     Clock_in_to_low(); // 锟斤拷锟斤拷锟狡碉拷锟�

//     while (1)
//     {
//         IEN1 |= 0x08; // 使锟斤拷锟解部锟叫讹拷4
//         RSTSTAT = 0x00;

//         _nop_();
//         _nop_();
//         _nop_();
//         SUSLO = 0X55; // 锟斤拷锟斤拷锟斤拷锟侥Ｊ�
//         PCON = 0X02;  // b1 = PD   b0 = IDLE
//         _nop_();

//         _nop_();
//         _nop_();

//         if (power_low_flag == 0) // 锟解部锟叫断憋拷锟斤拷锟斤拷
//         {
//             RSTSTAT = 0x01;
//             Clock_In_to_high(); // 锟斤拷锟斤拷锟狡�

//             delay_ms(20);                             // 锟斤拷锟斤拷
//             if (!KEYH4 || !KEYH3 || !KEYH2 || !KEYH1) //确锟斤拷锟叫帮拷锟斤拷锟斤拷锟斤拷
//             {
//                 P3CR = Bin(01011111); // 锟街革拷IO锟斤拷 锟斤拷锟斤拷
//                 P3PCR = Bin(10100000);
//                 P3 = Bin(00001111);

//                 nRF24L01P_Write_Reg(WRITE_REG + CONFIG, 0x0e); // 锟斤拷锟斤拷模式
//                 CE = 1;

//                 P0_2 = 1; // 锟斤拷锟斤拷锟斤拷压锟斤拷路
//                 ET2 = 1;  // 锟斤拷锟斤拷锟斤拷时锟斤拷锟叫讹拷
//                 ET5 = 1;
//                 //  while(!KEYH4 || !KEYH3 || !KEYH2 || !KEYH1);  //锟饺达拷锟斤拷锟斤拷
//                 P2_5 = 0;
//                 delay_ms(20); //锟剿诧拷
//                 break;
//             }
//             else
//                 power_low_flag = 1; // 确锟斤拷为锟斤拷锟斤拷 锟斤拷锟铰的伙拷锟斤拷 锟斤拷锟铰斤拷锟斤拷凸锟斤拷锟侥Ｊ�
//         }
//     }

//     //  Key_Send(MSG_KEY_NONE);                                  //锟斤拷取状态
// }
