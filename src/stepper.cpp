#include "stepper.h"

void Stepper::start(int stepNum, bool dir) {
  if (isMoving) return;
  printf("start stepper\n");
  isMoving = true;
  _dir = dir;
  _stepNum = stepNum;
  stepTick.attach(callback(this, &Stepper::tickFunc), 4ms);
}

void Stepper::tickFunc() {
  // pc.write("tick\n", 5);
  stepCounter++;
  _step = stepCounter % 2;
  if (stepCounter >= _stepNum * 2) {
    stepTick.detach();
    isMoving = false;
    _stepNum = 0;
    stepCounter = 0;
  }
}
