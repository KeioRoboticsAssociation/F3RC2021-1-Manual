#pragma once
#include <mbed.h>

class Servo {
 public:
  Servo(DigitalOut& dOut, int initialMicrosecond = 500) : pin(dOut) {
    pulseMicroSec = 500;
    riseFunc();
  }

  void move(int microSec);

 private:
  DigitalOut& pin;
  Timeout servoTick;
  int pulseMicroSec = 500;

  void riseFunc();
  void fallFunc();
};