#include <Servo.h>

#define MID_SIGNAL 1500
#define TEST_SIGNAL 1620

#define ESC_OUT_PIN 9

Servo motor;

void setup() {
  Serial.begin(9600);
  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");

  motor.attach(ESC_OUT_PIN);

  Serial.println("Now writing middle output.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");
  motor.writeMicroseconds(MID_SIGNAL);

  // Wait for input
  while (!Serial.available());
  Serial.read();

  // Send test output
  Serial.println("Sending minimum forward output");
  motor.writeMicroseconds(TEST_SIGNAL);
}

void loop() {  

}
