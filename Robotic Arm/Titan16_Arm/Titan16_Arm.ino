#include <Servo.h>
#include <PinChangeInt.h>

#define J1_IN_PIN   10
#define J2_IN_PIN   11
#define WR_IN_PIN   12
#define ED_IN_PIN   13

#define J1_OUT_PIN  6
#define J2_OUT_PIN  7
#define WR_OUT_PIN  8
#define ED_OUT_PIN  9

#define J1_FLAG     1
#define J2_FLAG     2
#define WR_FLAG     4
#define ED_FLAG     8

volatile uint8_t bUpdateFlagsShared;

volatile uint16_t usJ1InShared;
volatile uint16_t usJ2lInShared;
volatile uint16_t usWRInShared;
volatile uint16_t usEDlInShared;

uint32_t ulJ1Start;
uint32_t ulJ2Start;
uint32_t ulWRStart;
uint32_t ulEDStart;

Servo servoJ1;
Servo servoJ2;
Servo servoWR;
Servo servoED;

void setup() {
  Serial.begin(9600);
  
  servoJ1.attach(J1_OUT_PIN);
  servoJ2.attach(J2_OUT_PIN);
  servoWR.attach(WR_OUT_PIN);
  servoED.attach(ED_OUT_PIN);

  PCintPort::attachInterrupt(J1_IN_PIN, calcJ1, CHANGE);
  PCintPort::attachInterrupt(J2_IN_PIN, calcJ2, CHANGE);
  PCintPort::attachInterrupt(WR_IN_PIN, calcWR, CHANGE);
  PCintPort::attachInterrupt(ED_IN_PIN, calcED, CHANGE);
}

void loop() {
  static uint16_t usJ1In;
  static uint16_t usJ2In;
  static uint16_t usWRIn;
  static uint16_t usEDIn;
  
  static uint8_t bUpdateFlags;

  if(bUpdateFlagsShared) {
    noInterrupts();
    bUpdateFlags = bUpdateFlagsShared;

    if(bUpdateFlags & J1_FLAG) usJ1In = usJ1InShared;

    if(bUpdateFlags & J2_FLAG) usJ2In = usJ2InShared;

    if(bUpdateFlags & WR_FLAG) usWRIn = usWRInShared;
    
    if(bUpdateFlags & ED_FLAG) usEDIn = usEDInShared;

    bUpdateFlagsShared = 0;
    interrupts();
  }

  if(bUpdateFlags & J1_FLAG) {
    if(servoJ1.readMicroseconds() != usJ1In)
    {
      if(usJ1In < 1000)
        usJ1In = 1000;
      else if(usJ1In > 2000)
        usJ1In = 2000;
      
      usJ1In = map(usJ1In, 1000, 2000, 1220, 1480);
      
      servoJ1.writeMicroseconds(usJ1In);
    }
  }

  if(bUpdateFlags & J2_FLAG) {
    if(servoJ2.readMicroseconds() != usJ2In)
    {
      if(usJ2In < 1000)
        usJ2In = 1000;
      else if(usJ2In > 2000)
        usJ2In = 2000;
      
      usJ2In = map(usJ2In, 1000, 2000, 1220, 1480);
      
      servoJ2.writeMicroseconds(usJ2In);
    }
  }

  if(bUpdateFlags & WR_FLAG) {
    if(servoWR.readMicroseconds() != usWRIn)
    {
      if(usWRIn < 1000)
        usWRIn = 1000;
      else if(usWRIn > 2000)
        usWRIn = 2000;
      
      usWRIn = map(usWRIn, 1000, 2000, 1220, 1480);
      
      servoWR.writeMicroseconds(usWRIn);
    }
  }

  if(bUpdateFlags & ED_FLAG) {
    if(servoED.readMicroseconds() != usEDIn)
    {
      if(usEDIn < 1000)
        usEDIn = 1000;
      else if(usEDIn > 2000)
        usEDIn = 2000;
      
      usEDIn = map(usEDIn, 1000, 2000, 1220, 1480);
      
      servoED.writeMicroseconds(usEDIn);
    }
  }
  
  bUpdateFlags = 0;
}
