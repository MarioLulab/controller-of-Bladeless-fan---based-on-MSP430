/*
 * public.c
 *
 *  Created on: 2020年7月22日
 *      Author: luqi
 */
#include "public.h"

/*需要被传递的全局变量*/

// 开启标志
int start_flag = 0;         // 可以进行PID调节标志
int fan_switch = 0;         // 风扇开启标志
unsigned int buf_index = 0;
unsigned char receive_buf[10];

// PID过程参数
int now_error = 0;              // remember current time error
int pid_error[3] = {0,0,0}; // 记录PID调节器迭代过程中的3中error
unsigned long int target ;          // 记录所有时刻的设定值输入
float pid_out = 0;
int pid_out1 = 0;           // 记录上一个采样时刻的PID调节器输出
float error_1 = 0.0;        // 记录上一时刻的error
float error_2 = 0.0;        // 记录上上时刻的error
int start_now = 0;
int OUT_pwm = 0;

// 设定气压初值和当前气压初值
int current_gas_pressure = 0;
int set_gas_pressure = 100;


