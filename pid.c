/*
 * pid.c
 *
 *  Created on: 2020年7月22日
 *      Author: luqi
 */

#include "public.h"

// -------------------------------------
// 整定PID参数
// 整定PID参数时，把该段代码放在定时器中断内
void pid(void)
{
  float du;

  // 定义PID参数
  float kp=3;
  float ki=0.1;
  float kd=0;

  target = set_gas_pressure;

  // 计算du
  du =  kp*pid_error[0] + kd*pid_error[1] + ki*pid_error[2];
//  du = 2*pid_error[0] - 2.4*pid_error[1];
//  du = -2.4*error_1+2*error_2;

  // 计算times时刻的pid调节器输出
  pid_out = pid_out1 + du;

//   可添加PID控制器输出限幅代码
  if (pid_out>=1000)
          pid_out = 1000;

  if (pid_out<=0)
          pid_out = 0;

  // 如果风扇标志为 0 ，则pid调节器输出为0
  if (fan_switch == 0) pid_out = 0;


  // 把pid调节器的输出值赋给start_now
  start_now = pid_out;

  // 延时一段时间，等PWM波生成完作用于电机起作用后，再采样
  delay_ms(10); // delay 100ms

  ADS7950();

//  error[times] = set_gas_pressure - current_gas_pressure;
  now_error = set_gas_pressure - current_gas_pressure;

  // 更新pid_out1，该全局变量记录上一次PID调节器的输出
  pid_out1 = pid_out;

  // 更新误差参数
  pid_error[0] = now_error - error_1;
  pid_error[1] = now_error - 2*error_1 + error_2;
  pid_error[2] = now_error;
  error_2 = error_1;
  error_1 = now_error;
}
