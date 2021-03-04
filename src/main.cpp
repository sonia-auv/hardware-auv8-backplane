/***
 * 
 * Example code: This code is a simple program that turn on/off a LED with a button while another LED flash.
 * 
 ***/

#include "main.h"

void led_feedbackFunction()
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
      ThisThread::sleep_for(20ms);
    }

    battery1_value = battery1_value / total_data;
    battery2_value = battery2_value / total_data;

    if(battery1_value > batt_3led)                               // Full - 16,4V
    {
      LED3GBATT1 = 0;
      LED2GBATT1 = 0;
      LED1GBATT1 = 0;
      LEDRBATT1 = 1;
    }
    else if (battery1_value <= batt_3led && battery1_value > batt_2led)       // 16,4V - 15,8V 
    {
      LED3GBATT1 = 1;
      LED2GBATT1 = 0;
      LED1GBATT1 = 0;
      LEDRBATT1 = 1;
    }
    else if (battery1_value <= batt_2led && battery1_value > batt_1led)      // 15,8V - 15,4V
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
    if(battery2_value > batt_3led)                               // Full - 16,4V
    {
      LED3GBATT2 = 0;
      LED2GBATT2 = 0;
      LED1GBATT2 = 0;
      LEDRBATT2 = 1;
    }
    else if (battery2_value <= batt_3led && battery2_value > batt_2led)       // 16,4V - 15,8V 
    {
      LED3GBATT2 = 1;
      LED2GBATT2 = 0;
      LED1GBATT2 = 0;
      LEDRBATT2 = 1;
    }
    else if (battery2_value <= batt_2led && battery2_value > batt_1led)      // 15,8V - 15,4V
    {
      LED3GBATT2 = 1;
      LED2GBATT2 = 1;
      LED1GBATT2 = 0;
      LEDRBATT2 = 1;
    }
    else                                           // 15,4V - 0V
    {
      LED3GBATT2 = 1;
      LED2GBATT2 = 1;
      LED1GBATT2 = 1;
      LEDRBATT2 = 0;
    }
    for(uint8_t i=0; i<nb_motor; ++i)
    {
      if(enable_motor_data[i] == 1)
      {
        motor_power_good[i] = 0;
      }
      else
      {
        motor_power_good[i] = 1;
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
}

void voltageBattery()
{
  uint8_t cmd_array[1] = {CMD_BP_VBATT};
  uint8_t battery_receive[255]= {0};
  uint8_t battery_send[255]= {0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 8;

  while(true)
  {
    rs.read(cmd_array,nb_command,battery_receive);
    putFloatInArray(battery_send,readfromAnalog(INPUTBATT1));
    putFloatInArray(battery_send,readfromAnalog(INPUTBATT2), 4);
    rs.write(BACKPLANE_ID,cmd_array[0],nb_byte_send,battery_send);
  }
}

void readVoltage()
{
  uint8_t cmd_array[1]={CMD_BP_VOLTAGE};
  uint8_t voltage_receive[255]={0};
  uint8_t voltage_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_sensor = nb_motor+nb_12v;
  uint8_t nb_byte_send = 4*(nb_sensor);
  double_t voltage;

  while(true)
  {
    rs.read(cmd_array,nb_command,voltage_receive);
    for(uint8_t i=0; i<nb_sensor; ++i)
    {
      voltage = sensor[i].getBusVolt();
      putFloatInArray(voltage_send, voltage, i*4);
    }
    rs.write(BACKPLANE_ID, cmd_array[0], nb_byte_send, voltage_send);
  }
}

void readCurrent()
{
  uint8_t cmd_array[1]={CMD_BP_CURRENT};
  uint8_t current_receive[255]={0};
  uint8_t current_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_sensor = nb_motor+nb_12v;
  uint8_t nb_byte_send = 4*(nb_sensor);
  double_t current;

  while(true)
  {
    rs.read(cmd_array,nb_command,current_receive);
    for(uint8_t i=0; i<nb_sensor; ++i)
    {
      current = sensor[i].getBusVolt();
      putFloatInArray(current_send, current, i*4);
    }
    rs.write(BACKPLANE_ID, cmd_array[0], nb_byte_send, current_send);
  }
}

void enableMotor()
{
  uint8_t cmd_array[1]={CMD_BP_ENABLEMOTOR};
  uint8_t motor_receive[255]={0};
  uint8_t motor_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 8;

  while(true)
  {
    if(rs.read(cmd_array, nb_command, motor_receive) == 8)
    {
      for(uint8_t i=0; i<nb_motor; ++i)
      {
        enable_motor_data[i] = motor_receive[i];
        motor_send[i] = motor_receive[i];
      }
      rs.write(BACKPLANE_ID, cmd_array[0], nb_byte_send, motor_send);
    }
  }  
}

void readmotor()
{
  uint8_t cmd_array[1]={CMD_BP_READMOTOR};
  uint8_t motor_receive[255]={0};
  uint8_t motor_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 8;

  while(true)
  {
    rs.read(cmd_array, nb_command, motor_receive);
    for(uint8_t i=0; i<nb_motor; ++i)
    {
      motor_send[i] = enable_motor_data[i];
    }
    rs.write(BACKPLANE_ID, cmd_array[0], nb_byte_send, motor_receive);
  }
}

void killswitchreadout()
{
  while(true)
  {
    for(uint8_t i=0; i<nb_motor; ++i)
    {
      if(Killswitch == 1)
      {
        enable_motor[i] = 0;
      }
      else if(Killswitch == 0 && enable_motor_data[i] == 1)
      {
        enable_motor[i] = 1;
      }
      else
      {
        enable_motor[i] = 0;
      }
    }
    ThisThread::sleep_for(200ms);
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

  for(uint8_t i=0; i<nb_fan; ++i)
  {
    fan[i] = 0;
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

  for(uint8_t i=0; i<(nb_motor+nb_12v); ++i)
  {
    sensor[i].setConfig(CONFIG);
  }

  for(uint8_t i=0; i<nb_motor; ++i)
  {
    sensor[i].setCalibration(CALIBRATION_MOTEUR);
    sensor[i].setCurrentLSB(CURRENTLSB_MOTEUR);
  }

  for(uint8_t i=0; i<nb_12v; ++i)
  {
    sensor[i+nb_motor].setCalibration(CALIBRATION_12V);
    sensor[i+nb_motor].setCurrentLSB(CURRENTLSB_12V);
  }

  ledfeedback.start(led_feedbackFunction);
  ledfeedback.set_priority(osPriorityAboveNormal);

  inputbattery.start(voltageBattery);
  inputbattery.set_priority(osPriorityAboveNormal1);

  voltageread.start(readVoltage);
  voltageread.set_priority(osPriorityAboveNormal1);

  currentread.start(readCurrent);
  currentread.set_priority(osPriorityAboveNormal2);

  motorenable.start(enableMotor);
  motorenable.set_priority(osPriorityAboveNormal2);

  readmotorstate.start(readmotor);
  readmotorstate.set_priority(osPriorityAboveNormal1);

  emergencystop.start(killswitchreadout);
  emergencystop.set_priority(osPriorityAboveNormal);
}