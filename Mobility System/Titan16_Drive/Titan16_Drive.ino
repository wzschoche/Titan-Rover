/*
    Project:        Titan Rover 2016
    Program:        Drive Program
    Date Created:   10/21/2015

    Mobility Team:  William Zschoche
                    Paul Ishizaki
                    Justin Stewart
                    Bastian Awischus

    Description:    Preliminary drive sketch directing Rover Mobility sub-system.
                    Single joystick control over of rover in differential steering
                    configuration.

                    Two PWM signal inputs are into -100, 100 cartesian coordinate
                    format. Calculations result in proportional power signals to
                    drive 4 to 6 electronic speed controller (ESCs).
*/


#include <Servo.h>
#include <PinChangeInt.h>
#include <Math.h>
#include <TR16_DiffAlg.h>

/*
     Input pins from X8R radio receiver.
*/
#define JLX_IN_PIN 10     //ch. 1
#define JLY_IN_PIN 11     //ch. 2
#define DRV_IN_PIN 12     //ch. 3

/*
    Output pins to ESCs.

    valid PWM output pins(MEGA 2560): 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                                      44, 45, 46
*/
#define ESC_FL_OUT_PIN 2   //Front Left ESC
#define ESC_FR_OUT_PIN 3   //Front Right ESC
#define ESC_BL_OUT_PIN 4   //Back Left ESC
#define ESC_BR_OUT_PIN 5   //Back Right ESC
#define ESC_ML_OUT_PIN 6   //Mid Left ESC
#define ESC_MR_OUT_PIN 7   //Mid Right ESC

/*
    Binary "flag" values for interrupts.
*/
#define JLY_FLAG  2
#define JLX_FLAG  1
#define DRV_FLAG  4

/*
    Calibration signal for ESCs; 1500 = 0 throttle.
*/
#define CAL_SIGNAL 1500

/*
    Signal limiting constants. Maximum allowable signals for forward and
    reverse throttle.

    Note: Throttle is further limited in ESC firmware profile TITAN_ROVER. to a maximum of 25%.

*/
//#define MAX_FWD 1750      //Max forward throttle
//#define MAX_REV 1250      //Max reverse throttle

/*

*/
volatile uint8_t bUpdateFlagsShared;

volatile uint16_t usJLYInShared;
volatile uint16_t usJLXInShared;
volatile uint16_t usDRVInShared;

uint32_t ulJLYStart;
uint32_t ulJLXStart;
uint32_t ulDRVStart;

uint32_t ulTimeStart;

//uint32_t cycles;  //DEBUG

//float x_coord;
//float y_coord;

Servo motorFL;
Servo motorFR;
Servo motorBL;
Servo motorBR;
Servo motorML;
Servo motorMR;

TR16_DiffAlg diff;

void setup() {
  //initialize Serial output
  Serial.begin(9600);

  Serial.println("attaching pins...");
  //attach output pins
  motorFL.attach(ESC_FL_OUT_PIN);
  motorFR.attach(ESC_FR_OUT_PIN);
  motorBL.attach(ESC_BL_OUT_PIN);
  motorBR.attach(ESC_BR_OUT_PIN);
  motorML.attach(ESC_ML_OUT_PIN);
  motorMR.attach(ESC_MR_OUT_PIN);

  Serial.println("attaching interrupts");
  PCintPort::attachInterrupt(JLY_IN_PIN, calcJLY, CHANGE);
  PCintPort::attachInterrupt(JLX_IN_PIN, calcJLX, CHANGE);
  PCintPort::attachInterrupt(DRV_IN_PIN, calcDRV, CHANGE);

  //initialize timer
  ulTimeStart = micros();

  //Ensure ESCs are calibrated by sending idle signal for a
  //minimum of 4 seconds.
  Serial.println("Calibrating...");
  do {
    setSpeedAll(CAL_SIGNAL);
  } while ((micros() - ulTimeStart) <= 4000000);
  Serial.println("Done Calibrating");

  //  //initiliaze cycle counter      //DEBUG
  //  cycles = 0;
  Serial.println("Done with setup");
}

void loop() {
  static uint8_t bUpdateFlags;

  static uint16_t usJLYIn;
  static uint16_t usJLXIn;
  static uint16_t usDRVIn;

  static uint16_t usPowL;
  static uint16_t usPowR;

  static uint16_t usPower[2];

//  float propL;
//  float propR;
//
//  float v;
//  float w;

  if (bUpdateFlagsShared) {

    noInterrupts();

    bUpdateFlags = bUpdateFlagsShared;
    //check for y-input flag
    if (bUpdateFlags & JLY_FLAG) {
      usJLYIn = usJLYInShared;
    }
    //check for x-input flag
    if (bUpdateFlags & JLX_FLAG) {
      usJLXIn = usJLXInShared;
    }
    //check drive mode flag
    if (bUpdateFlags & DRV_FLAG) {
      usDRVIn = usDRVInShared;
    }

    bUpdateFlagsShared = 0;

    interrupts();
  }

  if (bUpdateFlags & (JLX_FLAG || JLY_FLAG)) {
    //    Serial.println("x-flag and y-flag");    //DEBUG
    
//    //limit signal to the range of ESCs
//    usJLXIn = constrain(usJLXIn, 1000, 2000);
//    usJLYIn = constrain(usJLYIn, 1000, 2000);
//    
////    usJLYIn = map(usJLXIn, 1000, 2000, 1150, 1850); //DEBUG
//    x_coord = map(usJLXIn, 1000, 2000, -100, 100);
//    y_coord = map(usJLYIn, 1000, 2000, -100, 100);
//
//    //    Serial.print("x-coord: ");
//    //    Serial.println(x_coord);                //DEBUG
//    if (fabs(x_coord) < 10)
//      x_coord = 0;
//    if (fabs(y_coord) < 10)
//      y_coord = 0;
//
//    x_coord *= -1;
//
//    v = calcPosVar(x_coord, y_coord);
//
//    w = calcNegVar(x_coord, y_coord);
//
//    propR = calcRightPower(v, w);
//
//    propL = calcLeftPower(v, w);
//
//    if (propR < 0) {
//      usPowR = 1500 - ((1500 - MAX_REV) * (fabs(propR) / 100));
//    }
//    else {
//      usPowR = 1500 + ((MAX_FWD - 1500) * (propR / 100));
//    }
//
//    if (propL < 0) {
//      usPowL = 1500 - ((1500 - MAX_REV) * (fabs(propL) / 100));
//    }
//    else {
//      usPowL = 1500 + ((MAX_FWD - 1500) * (propL / 100));
//    }
      diff.calcSpeed(usJLXIn, usJLYIn);

      usPowL = diff.getPowLeft();
      usPowR = diff.getPowRight();
  }

  //  Serial.print("wheels right: ");   //DEBUG
  //  Serial.println(usPowR);
  //  Serial.print("wheels left: ");
  //  Serial.println(usPowL);

  setSpeedRight(usPowR, usDRVIn);
  setSpeedLeft(usPowL, usDRVIn);
  //Serial.println(usJLXIn);  //DEBUG

  bUpdateFlags = 0;

  //  //count cycles
  //  ++cycles;                 //DEBUG
  //
  //  if(cycles % 1000 == 0) {
  //    Serial.print("Loops: ");
  //    Serial.println(cycles);
  //  }
  //  Serial.print("y-coord: ");
  //  Serial.println(y_coord);
  //  Serial.print("x-coord: ");
  //  Serial.println(x_coord);
  //  Serial.println("\n");
  //  delay(1000);              //DEBUG
}

//float calcPosVar(float x, float y) {
//  return (100 - fabs(x)) * (y / 100) + y;
//}
//
//float calcNegVar(float x, float y) {
//  return (100 - fabs(y)) * (x / 100) + x;
//}
//
//float calcRightPower(float v, float w) {
//  return (v + w) / 2;
//}
//
//float calcLeftPower(float v, float w) {
//  return (v - w) / 2;
//}

void setSpeedAll(uint16_t val) {
    motorBL.writeMicroseconds(val);
    motorBR.writeMicroseconds(val);
    motorML.writeMicroseconds(val);
    motorMR.writeMicroseconds(val);
    motorFL.writeMicroseconds(val);
    motorFR.writeMicroseconds(val);
}

void setSpeedLeft(uint16_t val, uint16_t mode) {
  //6WD mode
  if (mode > 1500) {
    motorBL.writeMicroseconds(val);
    motorML.writeMicroseconds(val);
    motorFL.writeMicroseconds(val);
  }
  //4WD mode
  else {
    motorFL.writeMicroseconds(val);
    motorBL.writeMicroseconds(val);
  }
}

void setSpeedRight(uint16_t val, uint16_t mode) {
  //6WD mode
  if (mode > 1500) {
    motorBR.writeMicroseconds(val);
    motorMR.writeMicroseconds(val);
    motorFR.writeMicroseconds(val);
  }
  //4WD mode
  else {
    motorFR.writeMicroseconds(val);
    motorBR.writeMicroseconds(val);
  }
}

// Calculate pulse time for the y-axis of the left joystick.
void calcJLY() {
  if (digitalRead(JLY_IN_PIN) == HIGH)
  {
    ulJLYStart = micros();
  }
  else
  {
    usJLYInShared = (uint16_t)(micros() - ulJLYStart);
    bUpdateFlagsShared |= JLY_FLAG;
  }
}

// Calculate pulse time for the x-axis of the left joystick.
void calcJLX()
{
  if (digitalRead(JLX_IN_PIN) == HIGH)
  {
    ulJLXStart = micros();
  }
  else
  {
    usJLXInShared = (uint16_t)(micros() - ulJLXStart);
    bUpdateFlagsShared |= JLX_FLAG;
  }
}

// Calculate pulse time for the drive mode switch.
void calcDRV()
{
  if (digitalRead(DRV_IN_PIN) == HIGH)
  {
    ulDRVStart = micros();
  }
  else
  {
    usDRVInShared = (uint16_t)(micros() - ulDRVStart);
    bUpdateFlagsShared |= DRV_FLAG;
  }
}
