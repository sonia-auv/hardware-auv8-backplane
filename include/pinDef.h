#ifndef PIN_DEF_H
#define PIN_DEF_H

//###################################################
//              BOARD PIN DEFINITION
//###################################################

#define PGOOD_M1 PA_0
#define PGOOD_M2 PA_1
#define PGOOD_M3 PA_2
#define PGOOD_M4 PA_3
#define PGOOD_M5 PA_4
#define PGOOD_M6 PA_5
#define PGOOD_M7 PA_6
#define PGOOD_M8 PA_7

#define I2C1_SCL PB_6
#define I2C1_SDA PB_7
#define I2C2_SCL PA_8
#define I2C2_SDA PC_9

#define PWM_1 PD_15
#define PWM_2 PD_14
#define PWM_3 PC_6
#define PWM_4 PC_7
#define PWM_5 PD_13
#define PWM_6 PD_12
#define PWM_7 PC_8
#define PWM_8 PE_6

#define KILLSWITCH PD_2

#define FAN_1 PE_1
#define FAN_2 PE_0

#define MTR_1 PB_12
#define MTR_2 PB_13
#define MTR_3 PE_5
#define MTR_4 PE_4
#define MTR_5 PB_14
#define MTR_6 PB_15
#define MTR_7 PE_3
#define MTR_8 PE_2

#define LED1_BATT1 PE_7
#define LED2_BATT1 PE_8
#define LED3_BATT1 PE_9
#define LED4_BATT1 PE_10

#define LED1_BATT2 PE_11
#define LED2_BATT2 PE_12
#define LED3_BATT2 PE_13
#define LED4_BATT2 PE_14

#define LED_KILL PE_15

#define INPUT_BATT1 PD_9
#define INPUT_BATT2 PC_0

//###################################################
//              RS485 PIN DEFINITION
//###################################################

#define RS485_RX_PIN PA_10
#define RS485_TX_PIN PA_9

#define RS485_TE_PIN PA_12
#define RS485_DE_PIN PA_11
#define RS485_RE_PIN PB_4

#endif