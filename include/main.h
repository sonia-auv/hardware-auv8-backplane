#ifndef MAIN_H
#define MAIN_H

#include "mbed.h"
#include "rtos.h"
#include "pinDef.h"
#include "adress_I2C.h"
#include "INA228/INA228.h"
#include "Utility/utility.h"
#include "PCA9531/PCA9531.h"
#include "TC74A5/TC74A5.h"
#include "RS485/RS485.h"
#include "RS485/RS485_definition.h"

#define nb_motor 8
#define nb_12v 2
#define nb_fan 2

#define turn_on_temp 65.0
#define turn_off_temp 25.0

#define CONFIG_SET (0x3FD << 4)
#define CONFIG_ADC_SET 0xFB6A
#define SHUNT_CAL_MOTOR 0x186A
#define SHUNT_CAL_12V 0xBB8
#define CURRENT_LSB_MTR 0.000048
#define CURRENT_LSB_12V 0.000029

#define NEUTRAL_PWM 1500
#define MAX_PWM 1900
#define MIN_PWM 1100

//###################################################
//             PINOUT FONCTION DEFINITION
//###################################################

PwmOut pwm[nb_motor] = {PwmOut(PWM_1), PwmOut (PWM_2), PwmOut(PWM_3), PwmOut(PWM_4), PwmOut(PWM_5), 
    PwmOut(PWM_6), PwmOut(PWM_7), PwmOut(PWM_8)};

DigitalIn Killswitch(KILLSWITCH);

DigitalIn Fault[nb_motor] = {DigitalIn(FAULT_M1), DigitalIn(FAULT_M2), DigitalIn(FAULT_M3), DigitalIn(FAULT_M4),
    DigitalIn(FAULT_M5), DigitalIn(FAULT_M6), DigitalIn(FAULT_M7), DigitalIn(FAULT_M8)};

DigitalOut enable_motor[nb_motor] = {DigitalOut(MTR_1), DigitalOut(MTR_2), DigitalOut(MTR_3), DigitalOut(MTR_4), 
    DigitalOut(MTR_5), DigitalOut(MTR_6), DigitalOut(MTR_7), DigitalOut(MTR_8)};

DigitalOut fan[nb_fan] = {DigitalOut(FAN_2), DigitalOut(FAN_1)};

DigitalOut LEDKILL(LED_KILL);
DigitalOut RESET_DRIVER(LED_RESET);

//###################################################
//             OBJECTS DEFINITION
//###################################################

RS485 rs(SLAVE_PSU0);
I2C i2c1_bus(I2C1_SDA, I2C1_SCL);
I2C i2c2_bus(I2C2_SDA, I2C2_SCL);
Mutex i2c_bus;

INA228 sensor[nb_motor+nb_12v] = {INA228(&i2c2_bus, I2C_M1), INA228(&i2c2_bus, I2C_M2), INA228(&i2c1_bus, I2C_M3), 
    INA228(&i2c1_bus, I2C_M4), INA228(&i2c2_bus, I2C_M5), INA228(&i2c2_bus, I2C_M6), INA228(&i2c1_bus, I2C_M7), 
    INA228(&i2c1_bus, I2C_M8), INA228(&i2c1_bus, I2C_12V1), INA228(&i2c2_bus, I2C_12V2)};

TC74A5 tempSensor[nb_fan] = {TC74A5(&i2c1_bus, I2C_TEMP1), TC74A5(&i2c2_bus, I2C_TEMP2)};

PCA9531 ledDriver1(&i2c2_bus, I2C_DRIVER1);
PCA9531 ledDriver2(&i2c1_bus, I2C_DRIVER2);

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
Thread faultdetection;
Thread pwmcommand;
Thread fancontroller;

//###################################################
//             VARIABLES DEFINITION
//###################################################

uint8_t enable_motor_data[nb_motor] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t fault_detection[nb_motor] = {0, 0, 0, 0, 0, 0, 0, 0};

#endif