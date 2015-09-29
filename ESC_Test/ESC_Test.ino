#include <Servo.h>

#define MID_SIGNAL 1500
#define TEST_SIGNAL0 1650
#define TEST_SIGNAL1 1750
#define TEST_SIGNAL2 1850
#define TEST_SIGNAL3 1950

#define ESC1_OUT_PIN 9
#define ESC2_OUT_PIN 10

Servo motor1;
Servo motor2;

void setup() {
  Serial.begin(9600);
  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");

  motor1.attach(ESC1_OUT_PIN);
  motor2.attach(ESC2_OUT_PIN);

  Serial.println("Now writing middle output.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");

  static int motorsig = MID_SIGNAL;
  
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
    motor1.writeMicroseconds(TEST_SIGNAL2);
    motor2.writeMicroseconds(TEST_SIGNAL2);
    delay(1000);
    motor1.writeMicroseconds(TEST_SIGNAL1);
    motor2.writeMicroseconds(TEST_SIGNAL1);
    delay(1000);
  }
}
