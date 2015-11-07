#include <Servo.h>
#include <PinChangeInt.h>
#include <Math.h>

/*
 *  Input pins from X8R receiver
 */
#define JOINT1_IN_PIN 10  //ch. 7
#define JOINT2_IN_PIN 11  //ch. 6
#define WRIST_IN_PIN  12  //ch. 4
#define ENDE_IN_PIN   13  //ch. 3

/*
 *  Output pins to arm joints (AJNT)
 */
//valid PWM output pins: 3,5,6,9,10,11
#define AJNT1_OUT_PIN 3   //flag 1
#define AJNT2_OUT_PIN 5   //flag 2
#define WRIST_OUT_PIN 6   //flag 3
#define ENDE_OUT_PIN  9   //flag 4

/*
 *  Binary "flag" values for interrupts
 */

 #define JOINT1_FLAG 1
 #define JOINT2_FLAG 2
 #define WRIST_FLAG  4
 #define ENDE_FLAG   8

 /*
  *   Calibration signal for AJNT servo motors; 1500 = 0 output
  */
#define CENTER_SIGNAL 1500

 /*
  *   Signal limiting constant.  The servos will move at a constant
  *   speed; so they are either on or off
  */

//#define MAX_ON 1750

  /*
   * 
   */
volatile uint8_t bUpdateFlagsShared;

volatile uint16_t usAJNT1InShared;
volatile uint16_t usAJNT2InShared;
volatile uint16_t usWRISTInShared;
volatile uint16_t usENDEInShared;

uint32_t ulAJNT1Start;
uint32_t ulAJNT2Start;
uint32_t ulWRISTStart;
uint32_t ulENDEStart;

uint32_t ulTimeStart;

uint32_t cycles;

float joint1;
float joint2;
float wrist;
float ende;

Servo armJoint1;
Servo armJoint2;
Servo wristJoint;
Servo endeJoint;

void setup() {
  Serial.begin(9600);

  //attach output pins
  Serial.println("attaching pins...");
  armJoint1.attach(AJNT1_OUT_PIN);
  armJoint2.attach(AJNT2_OUT_PIN);
  wristJoint.attach(WRIST_OUT_PIN);
  endeJoint.attach(ENDE_OUT_PIN);

  armJoint1.writeMicroseconds(CENTER_SIGNAL);
  armJoint2.writeMicroseconds(CENTER_SIGNAL);
  wristJoint.writeMicroseconds(CENTER_SIGNAL);
  endeJoint.writeMicroseconds(CENTER_SIGNAL);

  //attach interrupts
  Serial.println("attaching interrupts");
  PCintPort::attachInterrupt(JOINT1_IN_PIN, calcAJNT1, CHANGE);
  PCintPort::attachInterrupt(JOINT2_IN_PIN, calcAJNT2, CHANGE);
  PCintPort::attachInterrupt(WRIST_IN_PIN, calcWRIST, CHANGE);
  PCintPort::attachInterrupt(ENDE_IN_PIN, calcENDE, CHANGE);

  ulTimeStart = micros();
//  Serial.println("Calibrating...");
//  do {
//    setSpeedAll(CAL_SIGNAL);
//  } while ((micros() - ulTimeStart) <= 4000000);
//  Serial.println("Done Calibrating");

  //initialize timer
  ulTimeStart = micros();

  //initialize  cycle counter
  cycles = 0;

  Serial.println("Done with Setup");

}


void loop() {

  static uint16_t usAJNT1In;
  static uint16_t usAJNT2In;
  static uint16_t usWRISTIn;
  static uint16_t usENDEIn;

  static uint16_t usAJNT1;
  static uint16_t usAJNT2;
  static uint16_t usWRIST;
  static uint16_t usENDE;

  static uint8_t bUpdateFlags;

  if (bUpdateFlagsShared)
  {
    noInterrupts();

    bUpdateFlags = bUpdateFlagsShared;

    if (bUpdateFlags & JOINT1_FLAG)
    {
      usAJNT1In = usAJNT1InShared;
    }
    if (bUpdateFlags & JOINT2_FLAG)
    {
      usAJNT2In = usAJNT2InShared;
    }
    if (bUpdateFlags & WRIST_FLAG)
    {
      usWRISTIn = usWRISTInShared;
    }
    if (bUpdateFlags & ENDE_FLAG)
    {
      usENDEIn = usENDEInShared;
    }

    bUpdateFlagsShared = 0;

    interrupts();
  }

  if (bUpdateFlags & (JOINT1_FLAG || JOINT2_FLAG || WRIST_FLAG || ENDE_FLAG))
  {
    if (usAJNT1In < 1000)
      usAJNT1In = 1000;
    else if (usAJNT1In > 2000)
      usAJNT1In = 2000;
      
    if (usAJNT2In < 1000)
      usAJNT2In = 1000;
    else if (usAJNT2In > 2000)
      usAJNT2In = 2000;
      
    if (usWRISTIn < 1000)
      usWRISTIn = 1000;
    else if (usWRISTIn > 2000)
      usWRISTIn = 2000;

    if (usENDEIn < 1000)
      usENDEIn = 1000;
    else if (usENDEIn > 2000)
      usENDEIn = 2000;  

    
    joint1 = map(usAJNT1In, 1000, 2000, 1350, 1650);
    joint2 = map(usAJNT2In, 1000, 2000, 1350, 1650);
    wrist = map(usWRISTIn, 1000, 2000, 1350, 1650);
    //wrist = 1500;
    //Shovel End Effector: Collision imminent with j1 when j2 is at_____ and
    //end effector is at 1820. Conditions should prefent this.
    ende = map(usENDEIn, 1000, 2000, 1276, 1950);
    //ende = usENDEIn;

//    Serial.print("joint1: ");
//    Serial.println(joint1);
//    Serial.print("joint2: ");
//    Serial.println(joint2);
//    Serial.print("wrist: ");
//    Serial.println(wrist);
//    Serial.print("ende: ");
//    Serial.println(ende);

//    if (fabs(joint1) < )
//      joint1 = 1500;
//    else if (joint1 >= 20)
//      joint1 = 1650;
//    else if (joint1 <= -20)
//      joint1 = 1350;
          
//    if (fabs(joint2) < 20)
//      joint2 = 1500;
//    else if (joint2 >= 20)
//      joint2 = 1650;
//    else if (joint2 <= -20)
//      joint2 = 1400;  
    
//    Serial.print("joint1: ");
//    Serial.println(joint1);
//    Serial.println("");
//    Serial.print("joint2: ");
//    Serial.println(joint2);
//    Serial.println("");

    setSpeedJoint1(joint1);
    setSpeedJoint2(joint2);
    setSpeedWrist(wrist);
    setSpeedEnde(ende);

    bUpdateFlags = 0;
                
  }
  //delay(1000);

}

void setSpeedJoint1(uint16_t val)
{
  armJoint1.writeMicroseconds(val);
}

void setSpeedJoint2(uint16_t val)
{
  Serial.println(val);
  armJoint2.writeMicroseconds(val);
}

void setSpeedWrist(uint16_t val)
{
  wristJoint.writeMicroseconds(val);
}

void setSpeedEnde(uint16_t val)
{
  endeJoint.writeMicroseconds(val);
}

void calcAJNT1()
{
  if (digitalRead(JOINT1_IN_PIN) == HIGH)
  {
    ulAJNT1Start = micros();  
  }
  else
  {
    usAJNT1InShared = (uint16_t)(micros() - ulAJNT1Start);
    bUpdateFlagsShared |= JOINT1_FLAG;
  }
}
void calcAJNT2()
{
  if (digitalRead(JOINT2_IN_PIN) == HIGH)
  {
    ulAJNT2Start = micros();
  }
  else
  {
    usAJNT2InShared = (uint16_t)(micros() - ulAJNT2Start);
    bUpdateFlagsShared |= JOINT2_FLAG;
  }
}
void calcWRIST()
{
  //Serial.println("WRIST IS READ");
  if (digitalRead(WRIST_IN_PIN) == HIGH)
  {
    ulWRISTStart = micros();  
  }
  else
  {
    usWRISTInShared = (uint16_t)(micros() - ulWRISTStart);
    bUpdateFlagsShared |= WRIST_FLAG;
  }
}
void calcENDE()
{
  //Serial.println("ENDE IS READ");
  if (digitalRead(ENDE_IN_PIN) == HIGH)
  {
    ulENDEStart = micros();  
  }
  else
  {
    usENDEInShared = (uint16_t)(micros() - ulENDEStart);
    bUpdateFlagsShared |= ENDE_FLAG;
  }
}
