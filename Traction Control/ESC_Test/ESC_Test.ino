#include <Servo.h>

#define MID_SIGNAL 1500
#define TEST_SIGNAL0 1650
#define TEST_SIGNAL1 1700
#define TEST_SIGNAL2 1750
#define TEST_SIGNAL3 1800
#define TEST_SIGNAL4 1850
#define TEST_SIGNAL5 1900
#define TEST_SIGNAL6 1350
#define TEST_SIGNAL7 1300
#define TEST_SIGNAL8 1250
#define TEST_SIGNAL9 1200
#define TEST_SIGNAL10 1150
#define TEST_SIGNAL11 1100


#define ESC1_OUT_PIN 9
#define ESC2_OUT_PIN 10
#define ESC3_OUT_PIN 11
#define ESC4_OUT_PIN 12

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

void setup() {
  Serial.begin(9600);
  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");

  motor1.attach(ESC1_OUT_PIN);
  motor2.attach(ESC2_OUT_PIN);
  motor3.attach(ESC1_OUT_PIN);
  motor4.attach(ESC2_OUT_PIN);

  Serial.println("Now writing middle output.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");

  
  /*delay(2000);

  // Wait for input
  while (!Serial.available());
  Serial.read();

  // Send test output
  Serial.println("Sending minimum forward output");
  motor1.writeMicroseconds(TEST_SIGNAL);

  Serial.println("...");
  delay(5000);

  Serial.println("Start Motor 2 test? (Press Enter)");
   // Wait for input
  while (!Serial.available());
  Serial.read();

  Serial.println("Now writing middle output.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");
  motor2.writeMicroseconds(MID_SIGNAL);
  delay(2000);

  // Wait for input
  while (!Serial.available());
  Serial.read();

  // Send test output
  Serial.println("Sending minimum forward output");
  motor2.writeMicroseconds(TEST_SIGNAL);*/
}

void loop() {

  static uint32_t ulTimeStart = micros();
  uint32_t ulTimeNow = (micros() - ulTimeStart);
  
  if (ulTimeNow <= 3000000) {
    motor1.writeMicroseconds(MID_SIGNAL);
    motor2.writeMicroseconds(MID_SIGNAL);
  }
  else {
    motor1.writeMicroseconds(TEST_SIGNAL0);
    motor2.writeMicroseconds(TEST_SIGNAL0);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL1);
    motor2.writeMicroseconds(TEST_SIGNAL1);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL2);
    motor2.writeMicroseconds(TEST_SIGNAL2);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL3);
    motor2.writeMicroseconds(TEST_SIGNAL3);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL4);
    motor2.writeMicroseconds(TEST_SIGNAL4);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL5);
    motor2.writeMicroseconds(TEST_SIGNAL5);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL4);
    motor2.writeMicroseconds(TEST_SIGNAL4);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL3);
    motor2.writeMicroseconds(TEST_SIGNAL3);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL2);
    motor2.writeMicroseconds(TEST_SIGNAL2);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL1);
    motor2.writeMicroseconds(TEST_SIGNAL1);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL0);
    motor2.writeMicroseconds(TEST_SIGNAL0);
    delay(1000);
    motor1.writeMicroseconds(MID_SIGNAL);
    motor2.writeMicroseconds(MID_SIGNAL);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL6);
    motor2.writeMicroseconds(TEST_SIGNAL6);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL7);
    motor2.writeMicroseconds(TEST_SIGNAL7);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL8);
    motor2.writeMicroseconds(TEST_SIGNAL8);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL9);
    motor2.writeMicroseconds(TEST_SIGNAL9);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL10);
    motor2.writeMicroseconds(TEST_SIGNAL10);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL11);
    motor2.writeMicroseconds(TEST_SIGNAL11);
        delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL10);
    motor2.writeMicroseconds(TEST_SIGNAL10);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL9);
    motor2.writeMicroseconds(TEST_SIGNAL9);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL8);
    motor2.writeMicroseconds(TEST_SIGNAL8);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL7);
    motor2.writeMicroseconds(TEST_SIGNAL7);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL6);
    motor2.writeMicroseconds(TEST_SIGNAL6);
  }
}
