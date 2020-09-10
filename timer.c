/*
 * timer.c
 *
 *  Created on: 2020年7月22日
 *      Author: luqi
 */

#include "public.h"

uint TimA_num;
uint wei;

// -------------------------------------
// 定时器A0设置
void TIMER_A0_Init(void)//注意开总中断_EINT();
{
  TA0CTL = TASSEL_1 + MC_1 + TACLR;//时钟；模式；计数器、分频器清零、溢出中断使能
                                   //TASSEL：0：TA0XLK、1：ACLK、2：SMCLK、3：~TA0XLK
                                   //MC:0：停止、1:0~CCRn、2：CCR~FFFF-0~FFFF、3:0~CCR0~0
  TA0CCTL0 = CCIE;
  TA0CCR0 = 80;                  // 80/32768 = 2.44ms时，触发一次中断
}

//------------------------------------------------
// 定时器A触发中断
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)//TIMER0_A0//只使用此中断向量时不要使能TAIE位

{
    // 数码管显示
    if((TimA_num>100)&&(wei==0)){SMG_DEL(set_gas_pressure,current_gas_pressure); TimA_num=0;}
      SMG_XS(wei);//显示
      wei = (wei+1)%8;
      TimA_num++;

    if (fan_switch == 1){
    if(OUT_pwm!=start_now)//start_old
        {
          if(OUT_pwm<start_now)
          {
            OUT_pwm++;
          }
          else
          {
            if(OUT_pwm>0)  OUT_pwm--;
            else OUT_pwm=0;

          }
        }
    }
    else if (fan_switch == 0)   OUT_pwm = 0;

    if (current_gas_pressure == 800) OUT_pwm = 0;
    TA1CCR2 = OUT_pwm;
}

// -------------------------------------
// 初始化PWM波，刚开始时，将PWM波占空比拉为0%
void PWM(void)
{
  TA1CTL=TASSEL_2+MC_1+TACLR;
  TA1CCR0=1000;
  TA1CCTL2=OUTMOD_2;
  TA1CCR2=0;
  P2SEL|=BIT3;
  P2DIR|=BIT3;  // P2.3选择定时功能，设置为输出
}




