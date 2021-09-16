
#include "config.h"

#include <mbed.h>

DigitalOut step_num(STEP_PULSE, 0);  //ステッピングモーターのステップ
DigitalOut step_dir(STEP_DIR, 0);  //ステップの方向

DigitalOut dir0(MOTOR_DIR0);
DigitalOut dir1(MOTOR_DIR1);
DigitalOut dir2(MOTOR_DIR2);
DigitalOut dir3(MOTOR_DIR3);

PwmOut pwmout0(MOTOR_PWM0);
PwmOut pwmout1(MOTOR_PWM1);
PwmOut pwmout2(MOTOR_PWM2);
PwmOut pwmout3(MOTOR_PWM3);

DigitalOut winch0(WINCH0, 0);
DigitalOut winch1(WINCH1, 0);

DigitalOut servoL(SERVOL_PIN);
DigitalOut servoC(SERVOC_PIN);
DigitalOut servoR(SERVOR_PIN);

DigitalOut led(LED1, false);
CAN can1(CAN_RX, CAN_TX, CAN_BAUD);