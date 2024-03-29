/***
 * 
 * Author : Francis Alonzo
 * 
 * Hardware AUV8 Backplane
 * 
 ***/

#include "main.h"

void check_mask(INA228 sensor);
void led_feedbackFunction(double_t batt1, double_t batt2);

void readSensor()
{
  uint8_t cmd_array[2]={CMD_VOLTAGE, CMD_CURRENT};
  uint8_t voltage_send[255]={0};
  uint8_t current_send[255]={0};
  uint8_t nb_sensor = nb_motor+nb_12v;
  uint8_t nb_byte_send = 4*(nb_sensor);
  double_t voltage, current, batt1 = 0, batt2 = 0;

  while(true)
  {
    for(uint8_t i=0; i<nb_sensor; ++i)
    {
      check_mask(sensor[i]);

      voltage = sensor[i].getBusVolt();
      current = sensor[i].getCurrent();

      putFloatInArray(voltage_send, voltage, i*4);
      putFloatInArray(current_send, current, i*4);

      if(i == 8) batt1 = voltage;
      if(i == 9) batt2 = voltage;
    }
    led_feedbackFunction(batt1, batt2);
    rs.write(SLAVE_PSU0, cmd_array[0], nb_byte_send, voltage_send);
    rs.write(SLAVE_PSU0, cmd_array[1], nb_byte_send, current_send);
    ThisThread::sleep_for(500);
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
  uint8_t motor_send[255]={0};
  uint8_t nb_byte_send = nb_motor;

  while(true)
  {
    for(uint8_t i=0; i<nb_motor; ++i)
    {
      motor_send[i] = fault_detection[i];
    }
    rs.write(SLAVE_PSU0, cmd_array[0], nb_byte_send, motor_send);
    ThisThread::sleep_for(2500);
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
        fault_detection[i] = 2;
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
          if(data_pwm >= MIN_PWM && data_pwm <= MAX_PWM)
          {
            pwm[i].pulsewidth_us(data_pwm);
          }
        }
        else
        {
          pwm[i].pulsewidth_us(NEUTRAL_PWM);
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
    for(uint8_t i=0; i<nb_fan; ++i) // Fan digital output is inverted to account for the placement of the fan in the rack
    {
      temp = tempSensor[i].getTemp(); // TODO add the temperature of the INA228 to get a more precise activation
      if(temp >= turn_on_temp && fan[i] == 0) // Thermostat behavior
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

void check_mask(INA228 sensor)
{
  uint8_t data_ready = 0;

  while(data_ready == 0)
  {
    data_ready = ((sensor.getAlertFlags()>>1) & 0x01);
    ThisThread::sleep_for(1);
  }
}

void led_feedbackFunction(double_t batt1, double_t batt2)
{
  uint16_t stateBattery1 = 0;
  uint16_t stateBattery2 = 0;
  uint16_t stateMotor = 0;

  if(batt1 > 16.4)
  {
    stateBattery1 = 0b01010100;
  }
  else if (batt1 <= 16.4 && batt1 > 15.8)
  {
    stateBattery1 = 0b01010000;
  }
  else if (batt1 <= 15.8 && batt1 > 15.4)
  {
    stateBattery1 = 0b01000000;
  }
  else if (batt1 <= 15.4 && batt1 > 14.8)
  {
    stateBattery1 = 0b0000001;
  }
  else                                           // 14,8V is the lowest voltage we are confortable with
  {
    stateBattery1 = 0b00000000;
  }
  if(batt2 > 16.4)                      // Connection inverted on the led driver for battery 2
  {
    stateBattery2 = 0b00010101;
  }
  else if (batt2 <= 16.4 && batt2 > 15.8)
  {
    stateBattery2 = 0b00000101;
  }
  else if (batt2 <= 15.8 && batt2 > 15.4)
  {
    stateBattery2 = 0b00000001;
  }
  else if (batt2 <= 15.4 && batt2 > 14.8)
  {
    stateBattery2 = 0b01000000;
  }
  else                                            // 14,8V is the lowest voltage we are confortable with
  {
    stateBattery2 = 0b00000000;
  }

  stateMotor = 0;

  for(uint8_t i=0; i<nb_motor/2; ++i)
  {
    stateMotor += fault_detection[i];
    stateMotor = (stateMotor<<0x2);
  }

  for(uint8_t i=nb_motor-1; i>(nb_motor/2)-1; --i) // Not sure this works but the board is changing soon
  {                                                // so I won't work on it
    stateMotor += fault_detection[i];

    if(i != 4) stateMotor = (stateMotor<<0x2);
  }

  if(Killswitch.read() == 0)
  {
    LEDKILL = 1;
  }
  else
  {
    LEDKILL = 0;
  }

  ledDriver1.setLEDs((stateBattery1 << 8) | stateBattery2);
  ledDriver2.setLEDs(stateMotor);
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

  uint8_t i = 0;

  while(i<nb_12v+nb_motor)
  {
    sensor[i].setConfig(CONFIG_SET);
    sensor[i].setConfigADC(CONFIG_ADC_SET);
    if(sensor[i].getConfig() == CONFIG_SET || sensor[i].getConfigADC() == CONFIG_ADC_SET) ++i;
  }

  i = 0;

  while(i<nb_motor)
  {
    sensor[i].setShuntCal(SHUNT_CAL_MOTOR);
    sensor[i].setCurrentLSB(CURRENT_LSB_MTR);
    if(sensor[i].getShuntCal() == SHUNT_CAL_MOTOR || sensor[i].getCurrentLSB() == CURRENT_LSB_MTR) ++i;
  }

  while(i<nb_motor+nb_12v)
  {
    sensor[i].setShuntCal(SHUNT_CAL_12V);
    sensor[i].setCurrentLSB(CURRENT_LSB_12V);
    if(sensor[i].getShuntCal() == SHUNT_CAL_12V || sensor[i].getCurrentLSB() == CURRENT_LSB_12V) ++i;
  }

  RESET_DRIVER = 1;

  ledDriver2.setPrescaler(151, 0);
  ledDriver2.setDutyCycle(64,0);

  sensorread.start(readSensor);
  sensorread.set_priority(osPriorityHigh);

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