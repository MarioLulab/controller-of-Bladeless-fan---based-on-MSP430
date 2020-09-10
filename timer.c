/*
 * timer.c
 *
 *  Created on: 2020��7��22��
 *      Author: luqi
 */

#include "public.h"

uint TimA_num;
uint wei;

// -------------------------------------
// ��ʱ��A0����
void TIMER_A0_Init(void)//ע�⿪���ж�_EINT();
{
  TA0CTL = TASSEL_1 + MC_1 + TACLR;//ʱ�ӣ�ģʽ������������Ƶ�����㡢����ж�ʹ��
                                   //TASSEL��0��TA0XLK��1��ACLK��2��SMCLK��3��~TA0XLK
                                   //MC:0��ֹͣ��1:0~CCRn��2��CCR~FFFF-0~FFFF��3:0~CCR0~0
  TA0CCTL0 = CCIE;
  TA0CCR0 = 80;                  // 80/32768 = 2.44msʱ������һ���ж�
}

//------------------------------------------------
// ��ʱ��A�����ж�
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)//TIMER0_A0//ֻʹ�ô��ж�����ʱ��Ҫʹ��TAIEλ

{
    // �������ʾ
    if((TimA_num>100)&&(wei==0)){SMG_DEL(set_gas_pressure,current_gas_pressure); TimA_num=0;}
      SMG_XS(wei);//��ʾ
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
// ��ʼ��PWM�����տ�ʼʱ����PWM��ռ�ձ���Ϊ0%
void PWM(void)
{
  TA1CTL=TASSEL_2+MC_1+TACLR;
  TA1CCR0=1000;
  TA1CCTL2=OUTMOD_2;
  TA1CCR2=0;
  P2SEL|=BIT3;
  P2DIR|=BIT3;  // P2.3ѡ��ʱ���ܣ�����Ϊ���
}




