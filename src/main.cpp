/***
 * 
 * Example code: This code is a simple program that turn on/off a LED with a button while another LED flash.
 * 
 ***/

#include "main.h"

int main()
{
  mtr_enable = 0;

  pwm.period_us(2000);
  pwm.pulsewidth_us(NEUTRAL_PWM);

  while (true)
  {
    if(kill)
    {
      led2 = 1;
      if(sw1)
      {
        mtr_enable = 1;
        pwm.pulsewidth_us(1600);
        led3 = 1;
      }
      else if(sw2)
      {
        mtr_enable = 1;
        pwm.pulsewidth_us(MAX_PWM);
        led3 = 1;
      }
      else if(sw3)
      {
        mtr_enable = 1;
        pwm.pulsewidth_us(MIN_PWM);
        led3 = 1;
      }
      else if(sw4)
      {
        mtr_enable = 1;
        pwm.pulsewidth_us(1400);
        led3 = 1;
      }
      else
      {
        mtr_enable = 1;
        pwm.pulsewidth_us(NEUTRAL_PWM);
        led3 = 0;
      }
    }
    else
    {
      pwm.pulsewidth_us(NEUTRAL_PWM);
      mtr_enable = 0;
      led2 = 0;
      led3 = 0;
    }
  }
}