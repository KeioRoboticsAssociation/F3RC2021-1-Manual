#pragma once

#include <mbed.h>

class Stepper {
 public:
  Stepper(DigitalOut& dir, DigitalOut& step,
          chrono::duration<int, micro> stepPeriod = 4ms)
      : _dir(dir), _step(step) {}
  void start(int stepNum, bool dir);

 private:
  volatile bool isMoving = false;
  Ticker stepTick;
  DigitalOut& _dir;
  DigitalOut& _step;
  int _stepNum = 0;
  int stepCounter = 0;

  void tickFunc();
};
