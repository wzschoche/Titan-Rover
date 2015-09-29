#include <Servo.h>

#define ESC1_OUT_PIN 9
#define ESC2_OUT_PIN 10
#define ESC3_OUT_PIN 11
#define ESC4_OUT_PIN 12

#define CAL_SIGNAL 1500
#define TEST_SIGNAL 1650

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

uint32_t ulTimeStart;

void setup() {
  motor1.attach(ESC1_OUT_PIN);
  motor2.attach(ESC2_OUT_PIN);
  motor3.attach(ESC3_OUT_PIN);
  motor4.attach(ESC4_OUT_PIN);

  setSpeedAll(CAL_SIGNAL);

  ulTimeStart = micros();
}

void loop() {
  if ((micros() - ulTimeStart) <= 2000000) {
    setSpeedAll(CAL_SIGNAL);
  }
  else {
    setSpeedAll(TEST_SIGNAL);
  }
}

void setSpeedAll(uint16_t val) {
    motor1.writeMicroseconds(val);
    motor2.writeMicroseconds(val);
    motor3.writeMicroseconds(val);
    motor4.writeMicroseconds(val);
}

