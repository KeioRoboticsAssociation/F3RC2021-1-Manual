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
  volatile int _stepNum = 0;
  volatile int stepCounter = 0;

  void tickFunc();
};
