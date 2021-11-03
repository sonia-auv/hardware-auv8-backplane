#ifndef MAIN_H
#define MAIN_H

#include "mbed.h"
#include "rtos.h"
#include "pinDef.h"
#include "adress_I2C.h"

#define NEUTRAL_PWM 1500
#define MAX_PWM 1900
#define MIN_PWM 1100

//###################################################
//             PINOUT FONCTION DEFINITION
//###################################################

PwmOut pwm(PWM_1);

DigitalOut mtr_enable(MTR_1);
DigitalOut out_value1(OUT_VALUE1);
DigitalOut out_value2(OUT_VALUE2);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

DigitalIn on_mtr(ON_MTR);
DigitalIn kill(KILLSWITCH);

#endif