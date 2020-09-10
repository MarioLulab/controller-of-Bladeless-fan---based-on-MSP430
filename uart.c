/*
 * uart.c
 *
 *  Created on: 2020��7��22��
 *      Author: luqi
 */

#include "public.h"

void send_current(void){
    char current_buf[4];
    sprintf(current_buf,"%d",current_gas_pressure);
    send_buf(current_buf);
    send_buf("\n");
}

void send_buf(unsigned char *ptr)    //�����ַ���
{
    while(*ptr != '\0')
    {
        while(!(UCA1IFG & UCTXIFG));    // �����ж�
        UCA1TXBUF = *ptr;
        ptr++;
        delay(50);
    }
}

static char* left(char *dest,const char *src ,int n){
    char *p=dest;
    char *q=(char *)src;
    int len=strlen(src);

    if(n>len){
        n=len;
    }
    while(n--) {
        if (*q != ';') *(p++)=*(q++);   //������Ƿֺţ���ô��ȡֵ����
    }
    *(p++)='\0';
    return dest;
}

void delay(int ms)        //��ʱ����
{
    int i=0;
    int j=0;
    for(i=0;i<ms;i++)
    for(j=0;j<240;j++);
}


void parse_data(unsigned char *ptr){
    unsigned char instruct[4];
    left(instruct, ptr, 3);

    if (strcmp(instruct, "set") == 0) {
        // ��������ָ����set���ã�������ȡset������ݣ�������set_gas_pressure;
        send_buf("ok!I will set the gas pressure !!!");
        unsigned char data_input[3];
        left(data_input, receive_buf+4, 3);
        send_buf(data_input);
        int set_data;
//        sprintf(set_data,"%s",data_input);
        set_data = atoi(data_input);
        set_gas_pressure = set_data;
    }
    else if(strcmp(instruct, "stp") == 0){
        // ����������stpָ���ô��ͣ����
//        send_buf(receive_buf);
    	send_buf("fan is stop!");
        start_flag = 0;
        fan_switch = 0;

    }
    else if(strcmp(instruct, "run") == 0){
        // ����������run����ָ���ô��������
//        send_buf(receive_buf);
    	send_buf("fan is running!");
        start_flag = 1;
        fan_switch = 1;
    }
    else if(strcmp(instruct, "now") == 0){
        unsigned char char_buf[4];
        sprintf(char_buf, "%d", current_gas_pressure);
        send_buf(char_buf);
    }
    else send_buf("please input the correct instructs!");

}


void save_data(unsigned char *ptr){
    if (*ptr == ';')    {receive_buf[buf_index] = '\0';buf_index = 0;parse_data(receive_buf);}  //ÿ��ָ���ԡ�;����β
    else receive_buf[buf_index++] = *ptr;
    }
void UART_RS232_Init(void) //RS232�ӿڳ�ʼ������
{
    P5SEL    |=  BIT6+BIT7 ;                    // P5.6,5.7 = USCI_A1 TXD/RXD
    UCA1CTL1 |=  UCSWRST;                       // ��λUSCI
    UCA1CTL1 |=  UCSSEL_1;                      // ACLK
    UCA1BR0   =  0x03;                          // 32768Hz 9600 ������Ϊ9600
    UCA1BR1   =  0x00;                          // 32768Hz 9600
    UCA1MCTL |=  UCBRS_3 + UCBRF_0;             // Modulation UCBRSx=1, UCBRFx=0
    UCA1CTL1 &= ~UCSWRST;                       // ������λ
    UCA1IE   |=  UCRXIE ;                       // Enable USCI_A1 RX interrupt ʹ���ж�
}


#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_ISR()
{
    switch(_even_in_range(UCA1IV,4))
    {
    case 0:break;                             // Vector 0 - No interrupt
        case 2:                                   // Vector 2 - RXIFG
//                UCA1TXBUF = UCA1RXBUF;                // ���ݻ���
                save_data((unsigned char *)&UCA1RXBUF);                            // ����Է�����Ϣ���д����޸Ĵ˴�
            break;
     case 4:break;                             // Vector 4 - TXIFG
     default: break;
    }
}


