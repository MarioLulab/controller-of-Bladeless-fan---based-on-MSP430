/*
 * SMG.c
 *
 *  Created on: 2020年7月23日
 *      Author: luqi
 */

/*--------------------------------------
数码管显示函数在SMG_XS();中任意屏蔽位选
---------------------------------------*/

#include "public.h"
#define uint unsigned int
#define uchar unsigned char
/******数码管位选宏定义*******/
#define SMG_W0_0 P3OUT&=~BIT0
#define SMG_W0_1 P3OUT|=BIT0
#define SMG_W1_0 P3OUT&=~BIT5
#define SMG_W1_1 P3OUT|=BIT5
#define SMG_W2_0 P3OUT&=~BIT4
#define SMG_W2_1 P3OUT|=BIT4
#define SMG_W3_0 P11OUT&=~BIT0
#define SMG_W3_1 P11OUT|=BIT0
#define SMG_W4_0 P11OUT&=~BIT1
#define SMG_W4_1 P11OUT|=BIT1
#define SMG_W5_0 P11OUT&=~BIT2
#define SMG_W5_1 P11OUT|=BIT2
#define SMG_W6_0 P1OUT&=~BIT6
#define SMG_W6_1 P1OUT|=BIT6
#define SMG_W7_0 P1OUT&=~BIT7
#define SMG_W7_1 P1OUT|=BIT7
#define SMG_DUAN P8OUT
#define SMG_XS_DELAY SMG_DELAY(50)//<200
uchar SMG_XS_table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,
0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};

uint Smg_Data[10];
/*****************************/
void SMG_DELAY(uint z)
{
  uint o,p=100;
  for(o=0;o<z;o++)
    while(p)p--;
}
void SMG_GPIO(void)
{
  P3SEL &= ~(BIT0+BIT4+BIT5);
  P3DIR |= BIT0+BIT4+BIT5;
  P3OUT |= BIT0+BIT4+BIT5;
  P11SEL &= ~(BIT0+BIT1+BIT2);
  P11DIR |= BIT0+BIT1+BIT2;
  P11OUT |= BIT0+BIT1+BIT2;
  P1SEL &= ~(BIT6+BIT7);
  P1DIR |= BIT6+BIT7;
  P1OUT |= BIT6+BIT7;
  P8SEL = 0X00;
  P8OUT = 0XFF;
  P8DIR = 0XFF;
}
void SMG_XS(uchar weizhi)
{
  switch(weizhi)
  {
    case 0:
      SMG_W7_1;  SMG_DUAN=0xff;
      SMG_W0_0;
      break;
    case 1:
      SMG_W0_1;  SMG_DUAN=0xff;
      SMG_W1_0;
      break;
    case 2:
      SMG_W1_1;  SMG_DUAN=0xff;
      SMG_W2_0;
      break;
    case 3:
      SMG_W2_1;  SMG_DUAN=0xff;
      SMG_W3_0;
      break;
    case 4:
      SMG_W3_1;  SMG_DUAN=0xff;
      SMG_W4_0;
      break;
    case 5:
      SMG_W4_1;  SMG_DUAN=0xff;
      SMG_W5_0;
      break;
    case 6:
      SMG_W5_1;  SMG_DUAN=0xff;
      SMG_W6_0;
      break;
    case 7:
      SMG_W6_1;  SMG_DUAN=0xff;
      SMG_W7_0;
      break;
    default: break;

  }

  SMG_DUAN=SMG_XS_table[Smg_Data[weizhi]];

  /*SMG_W0_0;
  SMG_DUAN=0x86;//SMG_XS_table[(num0/1000)%10];
  SMG_XS_DELAY;
  SMG_W0_1;
  SMG_DUAN=0xff;*/
  /*SMG_W4_0;
  SMG_DUAN=SMG_XS_table[(num1/1000)%10];
  SMG_XS_DELAY;
  SMG_W4_1;
  SMG_DUAN=0xff;*/
}

void SMG_DEL(uint num0,uint num1)
{
  SMG_W7_1;
  SMG_DUAN=0xff;

  Smg_Data[0] = 16;
  Smg_Data[1] = (num0/100)%10;
  Smg_Data[2] = (num0/10)%10;
  Smg_Data[3] = num0%10;
  Smg_Data[4] = 16;
  Smg_Data[5] = (num1/100)%10;
  Smg_Data[6] = (num1/10)%10;
  Smg_Data[7] = num1%10;

}


