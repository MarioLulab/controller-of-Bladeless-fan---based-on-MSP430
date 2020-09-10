/*
 * public.h
 *
 *  Created on: 2020年7月22日
 *      Author: luqi
 */

#ifndef PUBLIC_H_
#define PUBLIC_H_

#include <stdio.h>
#include <msp430.h>
#include "msp430x54xa.h"
#include <string.h>
#include <stdlib.h>


#define delay_ms(x) __delay_cycles((long)(1048576*(double)x/1000.0))
#define uint unsigned int
#define uchar unsigned char

extern unsigned char receive_buf[10];  // 输入缓存区
extern unsigned int buf_index;         // 输入缓存区下标

extern int start_flag;         // 可以进行PID调节标志
extern int fan_switch;         // 风扇开启标志
extern unsigned long int target ;          // 记录所有时刻的设定值输入

extern int now_error;              // remember current time error
extern int pid_error[3]; // 记录PID调节器迭代过程中的3中error
//unsigned long int target ;          // 记录所有时刻的设定值输入
extern float pid_out;           // 记录这一采样时刻PID调节器输出
extern int pid_out1;           // 记录上一个采样时刻的PID调节器输出
extern float error_1;        // 记录上一时刻的error
extern float error_2;        // 记录上上时刻的error

extern int start_now;   // pid调节后，应该输出PWM波的计数器的值
extern int OUT_pwm;     // 当前输出PWM波的计数器的值


// 设定气压初值和当前气压初值
extern int current_gas_pressure;
extern int set_gas_pressure;



void TIMER_A0_Init(void);
void pid(void);
void PWM(void);
void ADS7950(void);
void ADS7950_PORT_Init(void);
unsigned int ADS7950_ReadResultAndSetNextSample(unsigned int uiSendData);
void delay(int ms);
void send_buf(unsigned char *ptr);
void send_current(void);
void UART_RS232_Init(void);

/*
#include "msp430x54xa.h"
#define uint unsigned int
#define uchar unsigned char
//#define _NOP() __no_operation()

extern uchar start_flag,pid_flag,AD_flag;
extern uint set_air,mea_air,voltage;//设置气压值，测量气压值
extern uint Start_now;
extern int errors1,errors2,errors3;
extern float DATA_Out;

void TIMER_A0_Init(void);
void ADS7950(void);
void ADS7950_PORT_Init();
uint float_int(void);
void PWM(void);
void pid(void);

void SMG_GPIO(void);
void SMG_XS(uchar weizhi);
void SMG_DEL(uint num0,uint num1);

void KEY_GPIO(void);
uchar key_scan(void);
void key_hand(void);

*/

#endif /* PUBLIC_H_ */
