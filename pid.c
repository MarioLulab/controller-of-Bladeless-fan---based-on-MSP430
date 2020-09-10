/*
 * pid.c
 *
 *  Created on: 2020��7��22��
 *      Author: luqi
 */

#include "public.h"

// -------------------------------------
// ����PID����
// ����PID����ʱ���Ѹöδ�����ڶ�ʱ���ж���
void pid(void)
{
  float du;

  // ����PID����
  float kp=3;
  float ki=0.1;
  float kd=0;

  target = set_gas_pressure;

  // ����du
  du =  kp*pid_error[0] + kd*pid_error[1] + ki*pid_error[2];
//  du = 2*pid_error[0] - 2.4*pid_error[1];
//  du = -2.4*error_1+2*error_2;

  // ����timesʱ�̵�pid���������
  pid_out = pid_out1 + du;

//   �����PID����������޷�����
  if (pid_out>=1000)
          pid_out = 1000;

  if (pid_out<=0)
          pid_out = 0;

  // ������ȱ�־Ϊ 0 ����pid���������Ϊ0
  if (fan_switch == 0) pid_out = 0;


  // ��pid�����������ֵ����start_now
  start_now = pid_out;

  // ��ʱһ��ʱ�䣬��PWM�������������ڵ�������ú��ٲ���
  delay_ms(10); // delay 100ms

  ADS7950();

//  error[times] = set_gas_pressure - current_gas_pressure;
  now_error = set_gas_pressure - current_gas_pressure;

  // ����pid_out1����ȫ�ֱ�����¼��һ��PID�����������
  pid_out1 = pid_out;

  // ����������
  pid_error[0] = now_error - error_1;
  pid_error[1] = now_error - 2*error_1 + error_2;
  pid_error[2] = now_error;
  error_2 = error_1;
  error_1 = now_error;
}
