#include "public.h"

unsigned long int AD_times = 0;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  TIMER_A0_Init();
  ADS7950_PORT_Init();
  UART_RS232_Init();
  SMG_GPIO();
  PWM();
  _EINT();  // 打开全局中断
  delay(50);    //delay 50ms
  send_buf("let's start!\ninput:run;->run the fan!\ninput:stp;->stop the fan!\ninput:now;->check out the current gas pressure\ninput:set xxx;->set the gas pressure you wish xxx");
  send_buf("\nremember end the instruct with ;");
  send_buf("\nenjoy yourself!");
  while(1)
  {
      AD_times++;
      if(AD_times==1000)
      {		ADS7950();
            if (start_flag) {
                pid();
//                send_current();
//                char current_buf[4];
//                sprintf(current_buf,"%d",current_gas_pressure);
//                send_buf(current_buf);
//                send_buf(",");
//                sprintf(current_buf,"%d",OUT_pwm);
//                send_buf(current_buf);
//                send_buf("\n");
//                char current_buf[4];
//                sprintf(current_buf, "%d", buf_index);
//                send_buf(receive_buf);
            }
          AD_times = 0;
      }
  }
}

