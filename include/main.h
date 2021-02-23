#ifndef MAIN_H
#define MAIN_H

#include "mbed.h"
#include "rtos.h"
#include "pinDef.h"
#include "INA226.h"
#include "adress_I2C.h"
#include "RS485/RS485.h"
#include "RS485/RS485_definition.h"

#define delay 300
#define battery_delay 20
#define nb_motor 8
#define nb_12v 2

#define BACKPLANE_ID 0

//###################################################
//             PINOUT FONCTION DEFINITION
//###################################################

DigitalOut PGOODM1(PGOOD_M1);
DigitalOut PGOODM2(PGOOD_M2);
DigitalOut PGOODM3(PGOOD_M3);
DigitalOut PGOODM4(PGOOD_M4);
DigitalOut PGOODM5(PGOOD_M5);
DigitalOut PGOODM6(PGOOD_M6);
DigitalOut PGOODM7(PGOOD_M7);
DigitalOut PGOODM8(PGOOD_M8);
DigitalOut motor_power_good[nb_motor] = {PGOODM1, PGOODM2, PGOODM3, PGOODM4, PGOODM5, PGOODM6, PGOODM7, PGOODM8};

PwmOut PWM1(PWM_1);
PwmOut PWM2(PWM_2);
PwmOut PWM3(PWM_3);
PwmOut PWM4(PWM_4);
PwmOut PWM5(PWM_5);
PwmOut PWM6(PWM_6);
PwmOut PWM7(PWM_7);
PwmOut PWM8(PWM_8);
PwmOut pwm[nb_motor] = {PWM1, PWM2, PWM3, PWM4, PWM5, PWM6, PWM7, PWM8};

DigitalIn Killswitch(KILLSWITCH);

DigitalOut MTR1(MTR_1);
DigitalOut MTR2(MTR_2);
DigitalOut MTR3(MTR_3);
DigitalOut MTR4(MTR_4);
DigitalOut MTR5(MTR_5);
DigitalOut MTR6(MTR_6);
DigitalOut MTR7(MTR_7);
DigitalOut MTR8(MTR_8);
DigitalOut enable_motor[nb_motor] = {MTR1, MTR2, MTR3, MTR4, MTR5, MTR6, MTR7, MTR8};

DigitalOut LED3GBATT1(LED1_BATT1);
DigitalOut LED2GBATT1(LED2_BATT1);
DigitalOut LED1GBATT1(LED3_BATT1);
DigitalOut LEDRBATT1(LED4_BATT1);

DigitalOut LED3GBATT2(LED1_BATT2);
DigitalOut LED2GBATT2(LED2_BATT2);
DigitalOut LED1GBATT2(LED3_BATT2);
DigitalOut LEDRBATT2(LED4_BATT2);

DigitalOut LEDKILL(LED_KILL);

AnalogIn INPUTBATT1(INPUT_BATT1);
AnalogIn INPUTBATT2(INPUT_BATT2);

//###################################################
//             OBJECTS DEFINITION
//###################################################

RS485 rs(BACKPLANE_ID);
I2C i2c1_bus(I2C1_SDA, I2C1_SCL);
I2C i2c2_bus(I2C2_SDA, I2C2_SCL);

INA226 senm1(&i2c2_bus, I2C_M1);
INA226 senm2(&i2c2_bus, I2C_M2);
INA226 senm3(&i2c1_bus, I2C_M3);
INA226 senm4(&i2c1_bus, I2C_M4);
INA226 senm5(&i2c2_bus, I2C_M5);
INA226 senm6(&i2c2_bus, I2C_M6);
INA226 senm7(&i2c1_bus, I2C_M7);
INA226 senm8(&i2c1_bus, I2C_M8);

INA226 sen12V1(&i2c1_bus, I2C_12V1);
INA226 sen12V2(&i2c2_bus, I2C_12V2);

INA226 sensor[nb_motor+nb_12v] = {senm1, senm2, senm3, senm4, senm5, senm6, senm7, senm8, sen12V1, sen12V2};

//###################################################
//             THREAD DEFINITION
//###################################################

Thread ledfeedback;
Thread inputbattery1;
Thread inputbattery2;
Thread voltageread;
Thread currentread;
Thread motorallenable;
Thread motorenable;
Thread emergencystop;
Thread readmotorstate;

//###################################################
//             VARIABLES DEFINITION
//###################################################

uint8_t enablemotor1;
uint8_t enablemotor2;
uint8_t enablemotor3;
uint8_t enablemotor4;
uint8_t enablemotor5;
uint8_t enablemotor6;
uint8_t enablemotor7;
uint8_t enablemotor8;
uint8_t enablemotor[nb_motor] = {enablemotor1, enablemotor2, enablemotor3, enablemotor4, enablemotor5, enablemotor6, enablemotor7, enablemotor8};

#endif