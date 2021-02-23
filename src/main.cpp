/***
 * 
 * Example code: This code is a simple program that turn on/off a LED with a button while another LED flash.
 * 
 ***/

#include "main.h"

#define batt_16V4 0.462
#define batt_15V8 0.445
#define batt_15V4 0.433

void led_feedbacFunction(void)
{
  double_t total_data = 10.0;
  double_t battery1_value = 0;
  double_t battery2_value = 0;

  while(true)
  {
    for(uint8_t i = 0; i < total_data; ++i)
    {
      battery1_value += INPUTBATT1.read();                  // Valeur de la batterie donnée avec un test pratique (voir Excel)
      battery2_value += INPUTBATT2.read();
      ThisThread::sleep_for(battery_delay);
    }

    battery1_value = battery1_value / total_data;
    battery2_value = battery2_value / total_data;

    if(battery1_value > batt_16V4)                               // Full - 16,4V
    {
      LED3GBATT1 = 0;
      LED2GBATT1 = 0;
      LED1GBATT1 = 0;
      LEDRBATT1 = 1;
    }
    else if (battery1_value <= batt_16V4 && battery1_value > batt_15V8)       // 16,4V - 15,8V 
    {
      LED3GBATT1 = 1;
      LED2GBATT1 = 0;
      LED1GBATT1 = 0;
      LEDRBATT1 = 1;
    }
    else if (battery1_value <= batt_15V8 && battery1_value > batt_15V4)      // 15,8V - 15,4V
    {
      LED3GBATT1 = 1;
      LED2GBATT1 = 1;
      LED1GBATT1 = 0;
      LEDRBATT1 = 1;
    }
    else                                           // 15,4V - 0V
    {
      LED3GBATT1 = 1;
      LED2GBATT1 = 1;
      LED1GBATT1 = 1;
      LEDRBATT1 = 0;
    }
    if(battery1_value > batt_16V4)                               // Full - 16,4V
    {
      LED3GBATT1 = 0;
      LED2GBATT1 = 0;
      LED1GBATT1 = 0;
      LEDRBATT1 = 1;
    }
    else if (battery1_value <= batt_16V4 && battery1_value > batt_15V8)       // 16,4V - 15,8V 
    {
      LED3GBATT1 = 1;
      LED2GBATT1 = 0;
      LED1GBATT1 = 0;
      LEDRBATT1 = 1;
    }
    else if (battery1_value <= batt_15V8 && battery1_value > batt_15V4)      // 15,8V - 15,4V
    {
      LED3GBATT1 = 1;
      LED2GBATT1 = 1;
      LED1GBATT1 = 0;
      LEDRBATT1 = 1;
    }
    else                                           // 15,4V - 0V
    {
      LED3GBATT1 = 1;
      LED2GBATT1 = 1;
      LED1GBATT1 = 1;
      LEDRBATT1 = 0;
    }
    for(uint8_t i=0; i<nb_motor; ++i)
    {
      if(enable_motor[i] == 1)
      {
        motor_power_good[i] == 0;
      }
      else
      {
        motor_power_good[i] == 1;
      }
    }
    if(Killswitch == 0)
    {
      LEDKILL = 1;
    }
    else
    {
      LEDKILL = 0;
    }
}

int main() 
{
  for(uint8_t i=0; i<nb_motor; ++i)
  {
    motor_power_good[i] = 1;
    pwm[i] = 1500;
    enable_motor[i] = 0;
  }

  LED3GBATT1 = 1;
  LED2GBATT1 = 1;
  LED1GBATT1 = 1;
  LEDRBATT1 = 1;
  LED3GBATT2 = 1;
  LED2GBATT2 = 1;
  LED1GBATT2 = 1;
  LEDRBATT2 = 1;
  LEDKILL = 1;

  ledfeedback.start(led_feedbacFunction);
  ledfeedback.set_priority(osPriorityAboveNormal);



    
}