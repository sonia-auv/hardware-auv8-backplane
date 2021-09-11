/***
 * 
 * Example code: This code is a simple program that turn on/off a LED with a button while another LED flash.
 * 
 ***/

#include "main.h"

void check_mask(INA226 sensor);

void led_feedbackFunction()
{
  double_t battery1_value = 0;
  double_t battery2_value = 0;
  uint16_t stateBattery1 = 0;
  uint16_t stateBattery2 = 0;
  uint16_t stateMotor = 0;

  RESET_DRIVER = 1;

  while(true)
  {
    if(battery1_value > 16.4)
    {
      stateBattery1 = 0b01010100;
    }
    else if (battery1_value <= 16.4 && battery1_value > 15.8)
    {
      stateBattery1 = 0b01010000;
    }
    else if (battery1_value <= 15.8 && battery1_value > 15.4)
    {
      stateBattery1 = 0b01000000;
    }
    else if (battery1_value <= 15.4 && battery1_value > 14.8)
    {
      stateBattery1 = 0b0000001;
    }
    else                                           // 14,8V est le minimum qu'on se donne
    {
      stateBattery1 = 0b00000000;
    }
    if(battery2_value > 16.4)                      // Connections pour la batterie 2 sont inversées
    {
      stateBattery2 = 0b00010101;
    }
    else if (battery2_value <= 16.4 && battery2_value > 15.8)
    {
      stateBattery2 = 0b00000101;
    }
    else if (battery2_value <= 15.8 && battery2_value > 15.4)
    {
      stateBattery2 = 0b00000001;
    }
    else if (battery2_value <= 15.4 && battery2_value > 14.8)
    {
      stateBattery2 = 0b01000000;
    }
    else                                           // 14,8V est le minimum qu'on se donne
    {
      stateBattery2 = 0b00000000;
    }

    stateMotor = 0;

    for(uint8_t i=0; i<nb_motor/2; ++i)
    {
      stateMotor += fault_detection[i];
      stateMotor = (stateMotor<<0x2);
    }

    for(uint8_t i=nb_motor-1; i>(nb_motor/2)-1; --i)
    {
      stateMotor += fault_detection[i];

      if(i != 4) stateMotor = (stateMotor<<0x2);
    }

    if(Killswitch == 0)
    {
      LEDKILL = 1;
    }
    else
    {
      LEDKILL = 0;
    }
    
    i2c_bus.lock();

    check_mask(sensor[8]);
    battery1_value = sensor[8].getBusVolt();
    check_mask(sensor[9]);
    battery2_value = sensor[9].getBusVolt();

    ledDriver1.setLEDs((stateBattery1*256) + stateBattery2);
    ledDriver2.setLEDs(stateMotor);
    
    i2c_bus.unlock();

    ThisThread::sleep_for(500);
  }
}

void readVoltage()
{
  uint8_t cmd_array[1]={CMD_VOLTAGE};
  uint8_t voltage_receive[255]={0};
  uint8_t voltage_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_sensor = nb_motor+nb_12v;
  uint8_t nb_byte_send = 4*(nb_sensor);
  double_t voltage;

  while(true)
  {
    i2c_bus.lock();
    for(uint8_t i=0; i<nb_sensor; ++i)
    {
      check_mask(sensor[i]);
      voltage = sensor[i].getBusVolt();
      putFloatInArray(voltage_send, voltage, i*4);
    }
    i2c_bus.unlock();

    rs.read(cmd_array,nb_command,voltage_receive);
    rs.write(SLAVE_PSU0, cmd_array[0], nb_byte_send, voltage_send);
  }
}

void readCurrent()
{
  uint8_t cmd_array[1]={CMD_CURRENT};
  uint8_t current_receive[255]={0};
  uint8_t current_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_sensor = nb_motor+nb_12v;
  uint8_t nb_byte_send = 4*(nb_sensor);
  double_t current;

  while(true)
  {
    i2c_bus.lock();
    for(uint8_t i=0; i<nb_sensor; ++i)
    {
      check_mask(sensor[i]);
      current = sensor[i].getCurrent();
      putFloatInArray(current_send, current, i*4);
    }
    i2c_bus.unlock();

    rs.read(cmd_array,nb_command,current_receive);
    rs.write(SLAVE_PSU0, cmd_array[0], nb_byte_send, current_send);
  }
}

void enableMotor()
{
  uint8_t cmd_array[1]={CMD_ACT_MOTOR};
  uint8_t motor_receive[255]={0};
  uint8_t motor_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = nb_motor;

  while(true)
  {
    if(rs.read(cmd_array, nb_command, motor_receive) == 8)
    {
      for(uint8_t i=0; i<nb_motor; ++i)
      {
        enable_motor_data[i] = motor_receive[i];
        motor_send[i] = motor_receive[i];
      }
      rs.write(SLAVE_PSU0, cmd_array[0], nb_byte_send, motor_send);
    }
  }  
}

void readmotor()
{
  uint8_t cmd_array[1]={CMD_READ_MOTOR};
  uint8_t motor_receive[255]={0};
  uint8_t motor_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = nb_motor;

  while(true)
  {
    rs.read(cmd_array, nb_command, motor_receive);
    for(uint8_t i=0; i<nb_motor; ++i)
    {
      motor_send[i] = fault_detection[i];
    }
    rs.write(SLAVE_PSU0, cmd_array[0], nb_byte_send, motor_send);
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
    ThisThread::sleep_for(250);
  }
}

void fault_detection_read()
{
  while(true)
  {
    for(uint8_t i=0; i<nb_motor; ++i)
    {
      if(enable_motor[i] && Fault[i])
      {
        fault_detection[i] = 1;
      }
      else if(enable_motor[i] && !Fault[i])
      {
        fault_detection[i] = 0; // 2 is right value. Need to fix the hardware
      }
      else
      {
        fault_detection[i] = 0;
      }
    }
    ThisThread::sleep_for(1000);
  }
}

void function_pwm()
{    
  uint8_t cmd_array[1] = {CMD_PWM};
  uint8_t buffer_receiver_pwm[255];
  uint8_t nb_command = 1;
  uint8_t size_command = 16;
  uint16_t data_pwm;

  while(true)
  {
    if(rs.read(cmd_array, nb_command, buffer_receiver_pwm) == size_command)
    {
      for(uint8_t i=0; i<nb_motor; ++i)
      {
        if(Killswitch == 0)
        {
          data_pwm = buffer_receiver_pwm[(2*i)+1]+buffer_receiver_pwm[2*i]*256;
          if(data_pwm >= 1100 && data_pwm <= 1900)
          {
            pwm[i].pulsewidth_us(data_pwm);
          }
        }
        else
        {
          pwm[i].pulsewidth_us(1500);
        }
      }
    }
  }
}

void function_fan()
{
  double_t temp;

  while(true)
  {
    i2c_bus.lock();
    for(uint8_t i=0; i<nb_fan; ++i)
    {
      temp = tempSensor[i].getTemp();
      if(temp >= turn_on_temp && fan[i] == 0)
      {
        fan[i] = 1;
      }
      else if(temp <= turn_off_temp && fan[i] == 1)
      {
        fan[i] = 0;
      }
    }
    i2c_bus.unlock();

    ThisThread::sleep_for(10000);
  }
}

void check_mask(INA226 sensor)
{
  uint8_t data_ready = 0;

  while(data_ready == 0)
  {
    data_ready = ((sensor.getMaskEnable()>>3) & 0x01);
    ThisThread::sleep_for(1);
  }
}

int main()
{
  RESET_DRIVER = 0;

  for(uint8_t i=0; i<nb_motor; ++i)
  {
    pwm[i].period_us(2000);
    pwm[i].pulsewidth_us(1500);
    enable_motor_data[i] = 0;
  }

  for(uint8_t i=0; i<nb_fan; ++i)
  {
    fan[i] = 0;
  }

  for(uint8_t i=0; i<nb_12v+nb_motor; ++i)
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

  ledDriver2.setPrescaler(151, 0);
  ledDriver2.setDutyCycle(64,0);

  ledfeedback.start(led_feedbackFunction);
  ledfeedback.set_priority(osPriorityAboveNormal);

  voltageread.start(readVoltage);
  voltageread.set_priority(osPriorityHigh);

  /*currentread.start(readCurrent);
  currentread.set_priority(osPriorityHigh);*/

  motorenable.start(enableMotor);
  motorenable.set_priority(osPriorityHigh);

  readmotorstate.start(readmotor);
  readmotorstate.set_priority(osPriorityHigh);

  emergencystop.start(killswitchreadout);
  emergencystop.set_priority(osPriorityHigh);

  faultdetection.start(fault_detection_read);
  faultdetection.set_priority(osPriorityAboveNormal7);

  pwmcommand.start(function_pwm);
  pwmcommand.set_priority(osPriorityHigh);

  fancontroller.start(function_fan);
  fancontroller.set_priority(osPriorityAboveNormal7);
}