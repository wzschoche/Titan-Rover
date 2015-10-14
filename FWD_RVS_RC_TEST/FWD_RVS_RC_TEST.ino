#include <Servo.h>
#include <PinChangeInt.h>
#include <Math.h>

#define JLY_IN_PIN 10
#define JLX_IN_PIN 11

#define ESCFL_OUT_PIN 7
#define ESCFR_OUT_PIN 8
#define ESCBL_OUT_PIN 2
#define ESCBR_OUT_PIN 3

#define JLY_FLAG 2
#define JLX_FLAG 1

#define CAL_SIGNAL 1500

volatile uint8_t bUpdateFlagsShared;

volatile uint16_t usJLYInShared;
volatile uint16_t usJLXInShared;

uint32_t ulJLYStart;
uint32_t ulJLXStart;

uint32_t ulTimeStart;

uint32_t cycles;

float x_coord;
float y_coord;
float angleR;
float angleL;

Servo motorFL;
Servo motorFR;
Servo motorBL;
Servo motorBR;

void setup() {
  //initialize Serial output
  Serial.begin(9600);

  Serial.println("attaching pins...");
  //attach output pins
  motorFL.attach(ESCFL_OUT_PIN);
  motorFR.attach(ESCFR_OUT_PIN);
  motorBL.attach(ESCBL_OUT_PIN);
  motorBR.attach(ESCBR_OUT_PIN);

  Serial.println("attaching interrupts");
  PCintPort::attachInterrupt(JLY_IN_PIN, calcJLY, CHANGE);
  PCintPort::attachInterrupt(JLX_IN_PIN, calcJLX, CHANGE);

  ulTimeStart = micros();
  Serial.println("Calibrating...");
  do {
    setSpeedAll(CAL_SIGNAL);
  } while((micros() - ulTimeStart) <= 4000000);
  Serial.println("Done Calibrating");

  //initialize timer
  ulTimeStart = micros();

  //initiliaze cycle counter
  cycles = 0;
  Serial.println("Done with setup");
}

void loop() {
  
  static uint16_t usJLYIn;
  static uint16_t usJLXIn;

  static uint16_t usPowL;
  static uint16_t usPowR;
  
  static uint8_t bUpdateFlags;

  static uint8_t mag_vec;

  float propL;
  float propR;
  
  //Ensure ESCs are calibrated by sending idles signal for a
  //minimum of 4 seconds.
  
  if(bUpdateFlagsShared) {
    
    noInterrupts();
    
    bUpdateFlags = bUpdateFlagsShared;
    //check for y-input interrupt
    if(bUpdateFlags & JLY_FLAG) {
      usJLYIn = usJLYInShared;
    }
    //check for x-input interrupt
    if(bUpdateFlags & JLX_FLAG) {
      usJLXIn = usJLXInShared;
    }
    
    bUpdateFlagsShared = 0;
    
    interrupts();
  }
  
  if(bUpdateFlags & JLY_FLAG) {
    //if(motorFL.readMicroseconds() != usJLYIn) {
      if(usJLYIn < 1000) {
        usJLYIn = 1000;
      }
      else if(usJLYIn > 2000) {
        usJLYIn = 2000;
      }
      //usJLYIn = map(usJLYIn, 1000, 2000, 1150, 1850);

      y_coord = map(usJLYIn, 1000, 2000, -100, 100);
      if(!(bUpdateFlags & JLY_FLAG)) {
        if(x_coord <= 0) {
          angleL = atan2(fabs(y_coord), fabs(x_coord));
          angleR = 180 - angleL;

          mag_vec = sqrt((pow(x_coord,2)+pow(y_coord,2)));
          propL = (angleL/180)*mag_vec;
          propR = (angleR/180)*mag_vec;

          if(y_coord <= 0) {
            usPowR = map(propR, 0, 45, 1500, 1350);
            usPowL = map(propL, 0, 45, 1500, 1350);
          }
          else {
            usPowR = map(propR, 0, 45, 1500, 1750);
            usPowL = map(propL, 0, 45, 1500, 1750);
          }
        }
        else {
          angleR = atan2(fabs(y_coord), fabs(x_coord));
          angleL = 180 - angleR;

          mag_vec = sqrt((pow(x_coord,2)+pow(y_coord,2)));
          propL = (angleL/180)*mag_vec;
          propR = (angleR/180)*mag_vec;

          if(y_coord <= 0) {
            usPowR = map(propR, 0, 45, 1500, 1350);
            usPowL = map(propL, 0, 45, 1500, 1350);
          }
          else {
            usPowR = map(propR, 0, 45, 1500, 1750);
            usPowL = map(propL, 0, 45, 1500, 1750);
          }
        }
      }
        
      Serial.println(usJLYIn);
    }
  
  if(bUpdateFlags & JLX_FLAG) {

    if(usJLXIn < 1000) {
        usJLXIn = 1000;
    }
    else if(usJLXIn > 2000) {
      usJLXIn = 2000;
    }
    //usJLYIn = map(usJLXIn, 1000, 2000, 1150, 1850);
    x_coord = map(usJLXIn, 1000, 2000, -100, 100);

    if(x_coord <= 0) {
      angleL = atan2(fabs(y_coord), fabs(x_coord));
      angleR = 180 - angleL;

      mag_vec = sqrt((pow(x_coord,2)+pow(y_coord,2)));
      propL = (angleL/180)*mag_vec;
      propR = (angleR/180)*mag_vec;

      if(y_coord <= 0) {
        usPowR = map(propR, 0, 45, 1500, 1350);
        usPowL = map(propL, 0, 45, 1500, 1350);
      }
      else {
        usPowR = map(propR, 0, 45, 1500, 1750);
        usPowL = map(propL, 0, 45, 1500, 1750);
      }
    }
    else {
      angleR = atan2(fabs(y_coord), fabs(x_coord));
      angleL = 180 - angleR;

      mag_vec = sqrt((pow(x_coord,2)+pow(y_coord,2)));
      propL = (angleL/180)*mag_vec;
      propR = (angleR/180)*mag_vec;

      if(y_coord <= 0) {
        usPowR = map(propR, 0, 45, 1500, 1350);
        usPowL = map(propL, 0, 45, 1500, 1350);
      }
      else {
        usPowR = map(propR, 0, 45, 1500, 1750);
        usPowL = map(propL, 0, 45, 1500, 1750);
      }
    }
  }

  setSpeedRight(usPowR);
  setSpeedLeft(usPowL);
  Serial.println(usJLXIn);
    
  bUpdateFlags = 0;
  
  //count cycles
  ++cycles;

  if(cycles % 1000 == 0) {
  //Serial.print("Loops: ");
  //Serial.println(cycles);
  }
}

void setSpeedAll(uint16_t val) {
  motorFL.writeMicroseconds(val);
  motorFR.writeMicroseconds(val);
  motorBL.writeMicroseconds(val);
  motorBR.writeMicroseconds(val);
}

void setSpeedLeft(uint16_t val) {
  motorFL.writeMicroseconds(val);
  motorBL.writeMicroseconds(val);
}

void setSpeedRight(uint16_t val) {
  motorFR.writeMicroseconds(val);
  motorBR.writeMicroseconds(val);
}

void calcJLY() {
  if(digitalRead(JLY_IN_PIN) == HIGH)
  { 
    ulJLYStart = micros();
  }
  else
  {
    usJLYInShared = (uint16_t)(micros() - ulJLYStart);
    bUpdateFlagsShared |= JLY_FLAG;
  }
}

void calcJLX()
{
  if(digitalRead(JLX_IN_PIN) == HIGH)
  { 
    ulJLXStart = micros();
  }
  else
  {
    usJLXInShared = (uint16_t)(micros() - ulJLXStart);
    bUpdateFlagsShared |= JLX_FLAG;
  }
}

