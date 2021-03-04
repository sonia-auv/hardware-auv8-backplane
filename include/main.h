#ifndef MAIN_H
#define MAIN_H

#include "mbed.h"
#include "rtos.h"
#include "utility.h"
#include "pinDef.h"
#include "INA226.h"
#include "TC74A5.h"
#include "adress_I2C.h"
#include "RS485/RS485.h"
#include "RS485/RS485_definition.h"

#define nb_motor 8
#define nb_12v 2
#define nb_fan 2

#define turn_on_temp 40.0
#define turn_off_temp 25.0

#define batt_3led 0.462
#define batt_2led 0.445
#define batt_1led 0.433

#define vref 3.3

#define BACKPLANE_ID 0

#define CMD_BP_VBATT 0
#define CMD_BP_VOLTAGE 1
#define CMD_BP_CURRENT 2
#define CMD_BP_ENABLEMOTOR 3
#define CMD_BP_READMOTOR 4


#define CONFIG      0x4527
#define CALIBRATION_MOTEUR 0xD1B
#define CALIBRATION_12V 0x15D8
#define CURRENTLSB_MOTEUR  0.000763
#define CURRENTLSB_12V 0.000458

//###################################################
//             PINOUT FONCTION DEFINITION
//###################################################

DigitalOut motor_power_good[nb_motor] = {DigitalOut(PGOOD_M1), DigitalOut(PGOOD_M2), DigitalOut(PGOOD_M3), 
    DigitalOut(PGOOD_M4), DigitalOut(PGOOD_M5), DigitalOut(PGOOD_M6), DigitalOut(PGOOD_M7), DigitalOut(PGOOD_M8)};

PwmOut pwm[nb_motor] = {PwmOut(PWM_1), PwmOut (PWM_2), PwmOut(PWM_3), PwmOut(PWM_4), PwmOut(PWM_5), 
    PwmOut(PWM_6), PwmOut(PWM_7), PwmOut(PWM_8)};

DigitalIn Killswitch(KILLSWITCH);

DigitalOut enable_motor[nb_motor] = {DigitalOut(MTR_1), DigitalOut(MTR_2), DigitalOut(MTR_3), DigitalOut(MTR_4), 
    DigitalOut(MTR_5), DigitalOut(MTR_6), DigitalOut(MTR_7), DigitalOut(MTR_8)};

DigitalOut LED3GBATT1(LED1_BATT1);
DigitalOut LED2GBATT1(LED2_BATT1);
DigitalOut LED1GBATT1(LED3_BATT1);
DigitalOut LEDRBATT1(LED4_BATT1);

DigitalOut LED3GBATT2(LED1_BATT2);
DigitalOut LED2GBATT2(LED2_BATT2);
DigitalOut LED1GBATT2(LED3_BATT2);
DigitalOut LEDRBATT2(LED4_BATT2);

DigitalOut fan[nb_fan] = {DigitalOut(FAN_1), DigitalOut(FAN_2)};

DigitalOut LEDKILL(LED_KILL);

AnalogIn INPUTBATT1(INPUT_BATT1);
AnalogIn INPUTBATT2(INPUT_BATT2);

//###################################################
//             OBJECTS DEFINITION
//###################################################

RS485 rs(BACKPLANE_ID);
I2C i2c1_bus(I2C1_SDA, I2C1_SCL);
I2C i2c2_bus(I2C2_SDA, I2C2_SCL);

INA226 sensor[nb_motor+nb_12v] = {INA226(&i2c2_bus, I2C_M1), INA226(&i2c2_bus, I2C_M2), INA226(&i2c1_bus, I2C_M3), 
    INA226(&i2c1_bus, I2C_M4), INA226(&i2c2_bus, I2C_M5), INA226(&i2c2_bus, I2C_M6), INA226(&i2c1_bus, I2C_M7), 
    INA226(&i2c1_bus, I2C_M8), INA226(&i2c1_bus, I2C_12V1), INA226(&i2c2_bus, I2C_12V2)};

TC74A5 tempSensor[nb_fan] = {TC74A5(&i2c1_bus, I2C_TEMP1), TC74A5(&i2c2_bus, I2C_TEMP2)};

//###################################################
//             THREAD DEFINITION
//###################################################

Thread ledfeedback;
Thread inputbattery;
Thread voltageread;
Thread currentread;
Thread motorenable;
Thread readmotorstate;
Thread emergencystop;
Thread pwmcommand;
Thread fancontroller;

//###################################################
//             VARIABLES DEFINITION
//###################################################

uint8_t enable_motor_data[nb_motor] = {0, 0, 0, 0, 0, 0, 0, 0};

#endif