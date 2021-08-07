#ifndef MAIN_H
#define MAIN_H

#include "mbed.h"
#include "rtos.h"
#include "pinDef.h"
#include "adress_I2C.h"
#include "INA226.h"
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

#define CONFIG      0x4527
#define CALIBRATION_MOTEUR 0xA7C
#define CALIBRATION_12V 0x15D8
#define CURRENTLSB_MOTEUR  0.000763
#define CURRENTLSB_12V 0.000458

//###################################################
//             PINOUT FONCTION DEFINITION
//###################################################

PwmOut pwm[nb_motor] = {PwmOut(PWM_1), PwmOut (PWM_2), PwmOut(PWM_3), PwmOut(PWM_4), PwmOut(PWM_5), 
    PwmOut(PWM_6), PwmOut(PWM_7), PwmOut(PWM_8)};

DigitalIn Killswitch(KILLSWITCH);

DigitalOut enable_motor[nb_motor] = {DigitalOut(MTR_1), DigitalOut(MTR_2), DigitalOut(MTR_3), DigitalOut(MTR_4), 
    DigitalOut(MTR_5), DigitalOut(MTR_6), DigitalOut(MTR_7), DigitalOut(MTR_8)};

DigitalOut fan[nb_fan] = {DigitalOut(FAN_1), DigitalOut(FAN_2)};

DigitalOut LEDKILL(LED_KILL);
DigitalOut RESET_DRIVER(LED_RESET);

//###################################################
//             OBJECTS DEFINITION
//###################################################

RS485 rs(SLAVE_BACKPLANE);
I2C i2c1_bus(I2C1_SDA, I2C1_SCL);
I2C i2c2_bus(I2C2_SDA, I2C2_SCL);

/*INA226 sensor[nb_motor+nb_12v] = {INA226(&i2c2_bus, I2C_M1), INA226(&i2c2_bus, I2C_M2), INA226(&i2c1_bus, I2C_M3), 
    INA226(&i2c1_bus, I2C_M4), INA226(&i2c2_bus, I2C_M5), INA226(&i2c2_bus, I2C_M6), INA226(&i2c1_bus, I2C_M7), 
    INA226(&i2c1_bus, I2C_M8), INA226(&i2c1_bus, I2C_12V1), INA226(&i2c2_bus, I2C_12V2)};*/

INA226 sensor[5] = {INA226(&i2c1_bus, I2C_M3), 
    INA226(&i2c1_bus, I2C_M4), INA226(&i2c1_bus, I2C_M7), 
    INA226(&i2c1_bus, I2C_M8), INA226(&i2c1_bus, I2C_12V1)};


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
Thread pwmcommand;
Thread fancontroller;

//###################################################
//             VARIABLES DEFINITION
//###################################################

uint8_t enable_motor_data[nb_motor] = {0, 0, 0, 0, 0, 0, 0, 0};

#endif