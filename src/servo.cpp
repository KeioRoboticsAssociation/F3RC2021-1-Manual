#include "servo.h"

#include <mbed.h>



void Servo::move(int microSec) { pulseMicroSec = microSec; }

void Servo::riseFunc() {
  pin = 1;
  servoTick.detach();
  servoTick.attach_us(callback(this, &Servo::fallFunc), pulseMicroSec);
}
void Servo::fallFunc() {
  pin = 0;
  servoTick.detach();
  servoTick.attach_us(callback(this, &Servo::riseFunc), 20000 - pulseMicroSec);
}
