/*
 * public.c
 *
 *  Created on: 2020��7��22��
 *      Author: luqi
 */
#include "public.h"

/*��Ҫ�����ݵ�ȫ�ֱ���*/

// ������־
int start_flag = 0;         // ���Խ���PID���ڱ�־
int fan_switch = 0;         // ���ȿ�����־
unsigned int buf_index = 0;
unsigned char receive_buf[10];

// PID���̲���
int now_error = 0;              // remember current time error
int pid_error[3] = {0,0,0}; // ��¼PID���������������е�3��error
unsigned long int target ;          // ��¼����ʱ�̵��趨ֵ����
float pid_out = 0;
int pid_out1 = 0;           // ��¼��һ������ʱ�̵�PID���������
float error_1 = 0.0;        // ��¼��һʱ�̵�error
float error_2 = 0.0;        // ��¼����ʱ�̵�error
int start_now = 0;
int OUT_pwm = 0;

// �趨��ѹ��ֵ�͵�ǰ��ѹ��ֵ
int current_gas_pressure = 0;
int set_gas_pressure = 100;


