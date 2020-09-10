/*
 * uart.c
 *
 *  Created on: 2020年7月22日
 *      Author: luqi
 */

#include "public.h"

void send_current(void){
    char current_buf[4];
    sprintf(current_buf,"%d",current_gas_pressure);
    send_buf(current_buf);
    send_buf("\n");
}

void send_buf(unsigned char *ptr)    //发送字符串
{
    while(*ptr != '\0')
    {
        while(!(UCA1IFG & UCTXIFG));    // 发送中断
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
        if (*q != ';') *(p++)=*(q++);   //如果不是分号，那么就取值复制
    }
    *(p++)='\0';
    return dest;
}

void delay(int ms)        //延时函数
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
        // 如果输入的指令是set设置，则再提取set后的内容，并传给set_gas_pressure;
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
        // 如果输入的是stp指令，那么暂停风扇
//        send_buf(receive_buf);
    	send_buf("fan is stop!");
        start_flag = 0;
        fan_switch = 0;

    }
    else if(strcmp(instruct, "run") == 0){
        // 如果输入的是run启动指令，那么开启风扇
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
    if (*ptr == ';')    {receive_buf[buf_index] = '\0';buf_index = 0;parse_data(receive_buf);}  //每条指令以‘;’结尾
    else receive_buf[buf_index++] = *ptr;
    }
void UART_RS232_Init(void) //RS232接口初始化函数
{
    P5SEL    |=  BIT6+BIT7 ;                    // P5.6,5.7 = USCI_A1 TXD/RXD
    UCA1CTL1 |=  UCSWRST;                       // 复位USCI
    UCA1CTL1 |=  UCSSEL_1;                      // ACLK
    UCA1BR0   =  0x03;                          // 32768Hz 9600 波特率为9600
    UCA1BR1   =  0x00;                          // 32768Hz 9600
    UCA1MCTL |=  UCBRS_3 + UCBRF_0;             // Modulation UCBRSx=1, UCBRFx=0
    UCA1CTL1 &= ~UCSWRST;                       // 结束复位
    UCA1IE   |=  UCRXIE ;                       // Enable USCI_A1 RX interrupt 使能中断
}


#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_ISR()
{
    switch(_even_in_range(UCA1IV,4))
    {
    case 0:break;                             // Vector 0 - No interrupt
        case 2:                                   // Vector 2 - RXIFG
//                UCA1TXBUF = UCA1RXBUF;                // 数据回显
                save_data((unsigned char *)&UCA1RXBUF);                            // 如需对发送信息进行处理修改此处
            break;
     case 4:break;                             // Vector 4 - TXIFG
     default: break;
    }
}


