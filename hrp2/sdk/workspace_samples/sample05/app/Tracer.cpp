#include "Tracer.h"

Tracer::Tracer():
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_3) {
}

void Tracer::init() {
  init_f("Tracer");
}

void Tracer::terminate() {
  msg_f("Stopped.", 1);
  leftWheel.stop();
  rightWheel.stop();
}

float Tracer::calc_prop_value() {
  const float Kp = 0.83;        // <1>
  const int target = 10;        // <2>
  const int bias = 0;

  int diff = colorSensor.getBrightness() - target; // <3>
  return (Kp * diff + bias);                       // <4>
}

void Tracer::run() {
  msg_f("running...", 1);
  float turn = calc_prop_value(); // <5>
  int pwm_l = pwm - turn;      // <6>
  int pwm_r = pwm + turn;      // <6>
  leftWheel.setPWM(pwm_l);
  rightWheel.setPWM(pwm_r);
}
