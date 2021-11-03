/***
 * 
 * Example code: This code is a simple program that turn on/off a LED with a button while another LED flash.
 * 
 ***/

#include "main.h"

int main()
{
  out_value1 = 1;
  out_value2 = 1;
  mtr_enable = 0;

  pwm.period_us(2000);
  pwm.pulsewidth_us(NEUTRAL_PWM);

  while (true)
  {
    if(kill)
    {
      mtr_enable = 1;
      pwm.pulsewidth_us(1600);
      led3 = 1;
      led2 = 1;
    }
    else
    {
      pwm.pulsewidth_us(NEUTRAL_PWM);
      mtr_enable = 1;
      led2 = 0;
      led3 = 0;
    }
  }
  return 0;
}