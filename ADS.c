/*
 * ADS.c
 *
 *  Created on: 2020年7月22日
 *      Author: luqi
 */
#include "public.h"

float fl_in_table[5];
float last_gas_pressure = 0.0;  // last_gas_pressure为前一次采样结果


#define SCLK         (BIT5)    //sclk
#define SDO          (BIT4)    //sdo
#define SDI          (BIT7)    //sdi
#define CS           (BIT1)    //cs

#define ADS7950_SCLK_HIGH  (P5OUT |= SCLK)
#define ADS7950_SCLK_LOW   (P5OUT &= ~SCLK)
#define ADS7950_SDI_HIGH   (P3OUT |= SDI)
#define ADS7950_SDI_LOW    (P3OUT &= ~SDI)
#define ADS7950_CS_HIGH    (P1OUT |= CS)
#define ADS7950_CS_LOW     (P1OUT &= ~CS)
#define ADS7950_IN         (P5IN)

#define SET_PIN_DIR_ADS7950_SCLK_OUT (P5DIR |= SCLK)
#define SET_PIN_DIR_ADS7950_SDO_IN   (P5DIR &= ~SDO)
#define SET_PIN_DIR_ADS7950_SDI_OUT  (P3DIR |= SDI)
#define SET_PIN_DIR_ADS7950_CS_OUT   (P1DIR |= CS)

#define MODE_MANUAL_CH0        0x1800
#define MODE_MANUAL_CH1        0x1880
#define MODE_MANUAL_CH2        0x1900
#define MODE_MANUAL_CH3        0x1980

// -------------------------------------
// ADS7950采样模块
unsigned int float_int(void)
{
  uchar i,j;
  uint FL_IN;
  float data;
  // 冒泡排序，把数组fl_in_table[]由大到小进行排列
  for(i=0;i<5;i++)
  {
    for(j=i;j>0;j--)
    {
      if(fl_in_table[j]>fl_in_table[j-1])
      {
        data=fl_in_table[j];
        fl_in_table[j]=fl_in_table[j-1];
        fl_in_table[j-1]=data;
      }
    }
  }
  data=fl_in_table[2]*0.9+last_gas_pressure*0.1; // 取采样得到的中位数，与上次结果作带权运算，得到的结果设置为这次采样的结果
  last_gas_pressure=data;
  FL_IN=(int)data;
  return FL_IN;
}

void ADS7950(void)
{
  uint spi_result,i;
  float mum_data;

  // 采5次样，平均每采一次需要5.38ms
  for(i=0;i<5;i++)
  {
    spi_result = ADS7950_ReadResultAndSetNextSample(MODE_MANUAL_CH3);   // sample CH3

    // gas_pressure_convert()
    mum_data =  spi_result&0xfff;
    mum_data = mum_data/0xfff*800; //data=dat/0XFFF*545;什么转换方法？

    fl_in_table[i] = mum_data;

    delay_ms(1); // delay 1ms

  }
  current_gas_pressure=float_int(); //把处理得到的采样结果，转换为了气压值，该值为当前气压值
}

void ADS7950_PORT_Init(void)
{
    // GPIO口初始化,包含了GPIO_init()
    P1DIR |= BIT2+BIT3+BIT4+BIT5;
    P1OUT |= BIT2+BIT3+BIT4+BIT5;
    SET_PIN_DIR_ADS7950_SCLK_OUT; //set pin output,connect to ads7950 sclk
    SET_PIN_DIR_ADS7950_SDI_OUT;  //set pin output,connect to ads7950 sdi
    SET_PIN_DIR_ADS7950_SDO_IN;   //set pin input, connect to ads7950 sdo
    SET_PIN_DIR_ADS7950_CS_OUT;   //set pin output,connect to ads7950 cs
    ADS7950_ReadResultAndSetNextSample(MODE_MANUAL_CH3);
}

unsigned int ADS7950_ReadResultAndSetNextSample(unsigned int uiSendData)
{
      unsigned int uiSendTimes,result;

      result=0;

      ADS7950_CS_HIGH;
      ADS7950_SCLK_LOW;
      __delay_cycles(30);//delay 20ns at least. 30 * 1/1000000 = 30us
      ADS7950_CS_LOW;
      __delay_cycles(30);//delay 40ns at least  for the first SDO. 30 * 1/1000000 = 30us

      for(uiSendTimes=16;uiSendTimes>0;uiSendTimes--)
      {
            // 采一次样约需要 16 * 90us * 3 + 60us = 4.38ms

            //prepare one bit to send.from the top bit , sending the SDI data , to choose the mode
            // ADS7950_configure()
            if((uiSendData & 0x8000)== 0x8000) ADS7950_SDI_HIGH;
            else  ADS7950_SDI_LOW;

            uiSendData <<=1;

            //read one bit
            // ADS7950_read()
            result<<=1;
            if(( ADS7950_IN & SDO)== SDO) result |= 0x0001; //pin 'SDO' is P9.2.'ADS7950_IN' is PORT9

            //delay 20ns at least  to set sclk high
            __delay_cycles(30); // delay 30 * 1/32768 = 30us

            ADS7950_SCLK_HIGH; //rising edge of SCLK, latch one bit
            __delay_cycles(30); // delay 30 * 1/32768 = 30us
            ADS7950_SCLK_LOW;  //falling edge of SCLK, SDO clocks on one bit
            __delay_cycles(30); // delay 30 * 1/32768 = 30us
      }

      ADS7950_CS_HIGH;  // 拉高nCS
     // __delay_cycles(50);

      return result;

}
