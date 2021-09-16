#include <mbed.h>

#define STEP_PULSE PB_14
#define STEP_DIR PC_4

#define MOTOR_DIR0 PB_10
#define MOTOR_DIR1 PC_7
#define MOTOR_DIR2 PA_7
#define MOTOR_DIR3 PC_9

#define MOTOR_PWM0 PA_8
#define MOTOR_PWM1 PA_9
#define MOTOR_PWM2 PB_6
#define MOTOR_PWM3 PA_6

#define WINCH0 PB_5
#define WINCH1 PB_4

#define SERVOL_PIN PB_13
#define SERVOC_PIN PB_15
#define SERVOR_PIN PB_2

#define IRLED_PIN PA_5

#define CAN_RX PA_11
#define CAN_TX PA_12
#define CAN_BAUD 500E3

#define CAN_ADRS 0x334

extern DigitalOut step_num;  //ステッピングモーターのステップ
extern DigitalOut step_dir;  //ステップの方向

extern DigitalOut dir0;
extern DigitalOut dir1;
extern DigitalOut dir2;
extern DigitalOut dir3;

extern PwmOut pwmout0;
extern PwmOut pwmout1;
extern PwmOut pwmout2;
extern PwmOut pwmout3;

extern DigitalOut winch0;
extern DigitalOut winch1;

extern DigitalOut servoL;
extern DigitalOut servoC;
extern DigitalOut servoR;

extern DigitalOut led;
extern CAN can1;