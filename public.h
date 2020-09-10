/*
 * public.h
 *
 *  Created on: 2020��7��22��
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

extern unsigned char receive_buf[10];  // ���뻺����
extern unsigned int buf_index;         // ���뻺�����±�

extern int start_flag;         // ���Խ���PID���ڱ�־
extern int fan_switch;         // ���ȿ�����־
extern unsigned long int target ;          // ��¼����ʱ�̵��趨ֵ����

extern int now_error;              // remember current time error
extern int pid_error[3]; // ��¼PID���������������е�3��error
//unsigned long int target ;          // ��¼����ʱ�̵��趨ֵ����
extern float pid_out;           // ��¼��һ����ʱ��PID���������
extern int pid_out1;           // ��¼��һ������ʱ�̵�PID���������
extern float error_1;        // ��¼��һʱ�̵�error
extern float error_2;        // ��¼����ʱ�̵�error

extern int start_now;   // pid���ں�Ӧ�����PWM���ļ�������ֵ
extern int OUT_pwm;     // ��ǰ���PWM���ļ�������ֵ


// �趨��ѹ��ֵ�͵�ǰ��ѹ��ֵ
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
extern uint set_air,mea_air,voltage;//������ѹֵ��������ѹֵ
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
