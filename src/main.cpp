#include <TransmitterIR.h>
#include <mbed.h>

#include "config.h"
#include "controller.h"
#include "servo.h"
#include "stepper.h"

#define sqrt_2_exp_minus_1 0.707107  //ルート2の-1乗
//                             y
//足回り　                      ^
//          front              |
//   wheel1         wheel0     |
//                              ------>x
//   wheel2         wheel3
//          rear

Controller controller(can1, 0x334);

Servo servoL_Con(servoL, 500);
Servo servoC_Con(servoC, 1200);
Servo servoR_Con(servoR, 1100);

void onPushOpenArm(int armNum, bool state) {
  if (!state) return;
  printf("openArm\n");

  if (armNum == 0) {
    // moveServo(servoL, 500);
    servoL_Con.move(500);
  } else if (armNum == 1) {
    // moveServo(servoC, 1200);
    servoC_Con.move(1200);
  } else {
    // moveServo(servoR, 1100);
    servoR_Con.move(1100);
  }
}

void onPushCloseArm(bool state) {
  if (!state) return;

  printf("closeArm\n");

  // move3Servo(servoL, 1300, servoC, 500, servoR, 1300);
  servoL_Con.move(1550);
  servoC_Con.move(600);
  servoR_Con.move(1550);
}

void onPushUpWinch(bool state) {
  if (state) {
    winch0 = 1;
    winch1 = 0;
  } else {
    winch0 = 0;
    winch1 = 0;
  }
}

void onPushDownWinch(bool state) {
  if (state) {
    winch0 = 0;
    winch1 = 1;
  } else {
    winch0 = 0;
    winch1 = 0;
  }
}

Stepper stepper(step_dir, step_num);
bool isArmForward = false;

void onPushForwardArm(bool state) {
  if (!state) return;
  if (isArmForward) return;
  stepper.start(137, 0);
  isArmForward = true;
}

void onPushBackArm(bool state) {
  if (!state) return;
  if (!isArmForward) return;
  stepper.start(137, 1);
  isArmForward = false;
}
void onPushStartAutoRobot(bool state) {
  if (!state) return;

  printf("send IR\n");
  TransmitterIR ir(IRLED_PIN);
  for (int i = 0; i < 20; i++) {
    while (ir.getState() != TransmitterIR::State::Idle) {
    }
    uint8_t buf[] = {0x8C, 0xE0, 0x00};
    int bitcount = 20;
    ir.setData(RemoteIR::Format::NEC, buf, bitcount);
  }
}

float speedDownRatio = 1.0;
void onPushSpeedDown(bool state) {
  if (state) {
    speedDownRatio = 0.5;
  } else {
    speedDownRatio = 1.0;
  }
}

void onPushBtn(size_t btnNum, bool state) {
  printf("push button %d\n", btnNum);
  switch (btnNum) {
    case 0:  // left
      onPushOpenArm(0, state);
      break;
    case 1:  // down
      onPushOpenArm(1, state);
      break;
    case 2:  // rigt
      onPushOpenArm(2, state);
      break;
    case 3:  // up
      onPushCloseArm(state);
      break;
    case 12:  // y
      onPushUpWinch(state);
      break;
    case 13:  // a
      onPushDownWinch(state);
      break;

    case 6:  // LZ
      onPushForwardArm(state);
      break;
    case 7:  // RZ
      onPushBackArm(state);
      break;

    case 9:  // start
      onPushStartAutoRobot(state);
      break;

    case 4:
      onPushSpeedDown(state);
      break;
  }
}

class Wheel {
 public:
  float MDpulse_raw;
  Wheel(DigitalOut& _dir, PwmOut& _pwmout, float _motor_ratio)
      : MDdir(_dir), MDpulse(_pwmout), motor_ratio(_motor_ratio) {}

  void set_MDpulse() {
    if (MDpulse_raw > 0) {
      MDpulse = MDpulse_raw * motor_ratio;
      MDdir = 0;
    } else {
      MDpulse = -MDpulse_raw * motor_ratio;
      MDdir = 1;
    }
  }

 private:
  DigitalOut& MDdir;
  PwmOut& MDpulse;
  float motor_ratio;
};

float input_x = 0;  //-1~1をとる 左のjoyconを想定
float input_y = 0;  //-1~1をとる 左のjoyconを想定
float input_z =
    0;  //-1~1をとる 右のjoyconを想定（右ジョイコンのy軸は現状読み取らない予定）
float velocity_rotation_ratio = 0;  //走行中の本体の回転と移動の出力の案配
                                    // 0-1をとる 1で移動100%、 0で自転100%
float velocity = 0;

void set_velocity_rotaion_ratio() {
  velocity_rotation_ratio = 1 - abs(input_z);
}

int main() {
  // onPushCloseArm(true);

  pwmout0.period_us(100);
  pwmout1.period_us(100);
  pwmout2.period_us(100);
  pwmout3.period_us(100);

  onPushOpenArm(0, true);
  onPushOpenArm(1, true);
  onPushOpenArm(2, true);

  controller.setButtonEventListener(&onPushBtn);

  Wheel wheel0(dir0, pwmout0, 0.95);
  Wheel wheel1(dir1, pwmout1, 0.95);
  Wheel wheel2(dir2, pwmout2, 0.95);
  Wheel wheel3(dir3, pwmout3, 0.95);

  // wheel0.MDpulse_raw = 0.5;
  // wheel0.set_MDpulse();

  while (true) {
    controller.receiveData();
    // printf("axesX: %d, axesY, %d", controller.axes.x, controller.axes.y);
    set_velocity_rotaion_ratio();
    input_x = -controller.axes.x * 0.01 * speedDownRatio;
    // input_x = 1;
    input_y = controller.axes.y * 0.01 * speedDownRatio;
    input_z = controller.axes.z * 0.01 * speedDownRatio * 0.7;

    wheel0.MDpulse_raw =
        sqrt_2_exp_minus_1 * velocity_rotation_ratio * (-input_x + input_y) +
        (1 - velocity_rotation_ratio) * input_z;
    wheel1.MDpulse_raw =
        sqrt_2_exp_minus_1 * velocity_rotation_ratio * (-input_x - input_y) +
        (1 - velocity_rotation_ratio) * input_z;
    wheel2.MDpulse_raw =
        sqrt_2_exp_minus_1 * velocity_rotation_ratio * (input_x - input_y) +
        (1 - velocity_rotation_ratio) * input_z;
    wheel3.MDpulse_raw =
        sqrt_2_exp_minus_1 * velocity_rotation_ratio * (input_x + input_y) +
        (1 - velocity_rotation_ratio) * input_z;

    // printf("wheel0: %d, wheel1: %d, wheel2: %d, wheel3: %d\n",
    //        (int)(wheel0.MDpulse_raw * 100), (int)(wheel1.MDpulse_raw * 100),
    //        (int)(wheel2.MDpulse_raw * 100), (int)(wheel3.MDpulse_raw * 100));

    wheel0.set_MDpulse();
    wheel1.set_MDpulse();
    wheel2.set_MDpulse();
    wheel3.set_MDpulse();
  }
}
