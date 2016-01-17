
#include <SPI.h>
#include <WiFi101.h> 
#include <WiFiUdp.h>
#include <math.h>
#include <Servo.h>
//#include "PinChangeInt.h"


//wifi initialize
int status = WL_IDLE_STATUS;
char ssid[] = "TheySeeUsRovin";
char pass[] = "brandondang";
int keyIndex = 0;

unsigned int localPort = 8888;

char packetBuffer[255];

WiFiUDP Udp;

//arm initialize
#define CENTER_SIGNAL 1500
#define ENDE_SIGNAL 1615

const int LENGTH1 = 900;
const int LENGTH2 = 1050;

int xArm;
int yArm;
int phiE;

#define AJNT1_OUT_PIN 3
#define AJNT2_OUT_PIN 6
#define ENDE_OUT_PIN 9

Servo armJoint1;
Servo armJoint2;
Servo endeJoint;


void setup()
{
  Serial.begin(9600);

//  Udp.write("attaching pins\n");
//  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//  Udp.write("attaching pins");
//  Udp.endPacket();
  Serial.println("attaching pins");
  armJoint1.attach(AJNT1_OUT_PIN);
  armJoint2.attach(AJNT2_OUT_PIN);
  endeJoint.attach(ENDE_OUT_PIN);
//
  armJoint1.writeMicroseconds(CENTER_SIGNAL);
  armJoint2.writeMicroseconds(CENTER_SIGNAL);
  endeJoint.writeMicroseconds(ENDE_SIGNAL);

  while (!Serial){;}
  
//  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//  Udp.endPacket();

//  Udp.write("checking shield\n");
//  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//  Udp.write("checking shield");
//  Udp.endPacket();
  Serial.println("checking shield");
  if (WiFi.status() == WL_NO_SHIELD)
  {
//    Udp.write("Wifi shield not present");
//    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//    Udp.write("Wifi shield not present");
//    Udp.endPacket();
    Serial.println("Wifi shield not present");
    while(true);
  }

  while (status != WL_CONNECTED)
  {
//    Udp.write("Attempting to connect to SSID: ");
//  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//  Udp.write("Attempting to connect to SSID: ");
//  Udp.endPacket();
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
//    Udp.write(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

//  Udp.write("\nConnected to wifi");
//  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//  Udp.write("Connected to wifi");
//  Udp.endPacket();
  Serial.println("Connected to wifi");

//  Udp.write("\nStarting connection to server. . .\n");
//  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//  Udp.write("Starting connection to server . . .");
//  Udp.endPacket();
  Serial.println("Starting connection to server . . .");

  xArm = 1050;
  yArm = 1600;
  phiE = 0;

  printWifiStatus();
  
  Udp.begin(localPort);

//  Udp.write("Done with Setup\n");
//  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//  Udp.write("Done with Setup");
//  Udp.endPacket();
  Serial.println("Done with Setup");

//  Udp.endPacket();

}

bool firstLoopCycle = true;

void loop()
{
  
  static int pastX;
  static int pastY;

  static int pastTheta1;
  static int pastTheta2;

  static int usAJNT1;
  static int usAJNT2;

  int x_coord = 0;
  int y_coord = 0;

  double theta1;
  double theta2;

  double theta1Deg;
  double theta2Deg;
  
  
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    //Serial.println("packet received");
    int len = Udp.read(packetBuffer, 255);
    if(len > 0) packetBuffer[len] = 0;

    char temp[20];
    int loopCount = 0;

    //Serial.println(packetBuffer);

    for (int i = 0; i < 255; ++i)
    {
      temp[i-loopCount] = packetBuffer[i];
      if(packetBuffer[i] == '/')
      {
        if (y_coord == 0)
        {
          y_coord = atoi(temp);
        }
        else
        {
          x_coord = atoi(temp);
          break;
        }
        loopCount = i + 1;
      }
    }


//    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
 //   message.toCharArray(charBuffer,message.length());
//    Udp.write(y_coord);
//    Udp.endPacket();
//    delay(1000);

    if (firstLoopCycle)
    {
      pastY = y_coord;
      pastX = x_coord;
      //firstLoopCycle = false;
      Serial.println("firstLoopCycle");
    }

    if (y_coord == 1 && x_coord == 51)
    {
      y_coord = pastY;
      x_coord = pastX;
    }

    else
    {
     if (y_coord == 0 || y_coord > 101 || y_coord < 1)
      {
        y_coord = pastY;
      }
      if (x_coord == 0 || x_coord > 101 || x_coord < 1)
      {
        x_coord = pastX;
      }
    }

//    Serial.print(y_coord);
//    Serial.print("\t");
//    Serial.print(x_coord);
//    Serial.print("\t");
//    Serial.println(packetBuffer);

//    Udp.beginPacket(Udp.remoteIP(),Udp.remotePort());
//    Udp.write(packetBuffer);
//    Udp.endPacket();

    pastY = y_coord;
    pastX = x_coord;
//
//    if (y_coord > pastY)
//    {
//      yArm += 50;
//    }
//    else if (y_coord < pastY)
//    {
//      yArm -= 50;
//    }

    yArm = map(y_coord, 1, 101, -1950, 1950);

//    if (yArm < -145)
//    {
//      yArm = -145;
//    }
//    else if (yArm > 1950)
//    {
//      yArm = 1950;
//    }

//    if (x_coord > pastX)
//    {
//      xArm += 50;
//    }
//    else if (x_coord < pastX)
//    {
//      xArm -= 50;
//    }

    xArm = map(x_coord, 1, 101, -1950, 1950);

//    if (xArm < -985)
//    {
//      xArm = -985;
//    }
//    else if (xArm > 1950)
//    {
//      xArm = 1950;
//    }

    theta1 = solveTheta1(xArm, yArm, LENGTH1, LENGTH2);
    theta2 = solveTheta2(xArm, yArm, LENGTH1, LENGTH2);

    if (firstLoopCycle)
    {
      pastTheta1 = theta1;
      pastTheta2 = theta2;
      firstLoopCycle = false;
    }

    if (isnan(theta1))
    {
      theta1 = pastTheta1;
    }
    if (isnan(theta2))
    {
      theta2 = pastTheta2;
    }

    pastTheta1 = theta1;
    pastTheta2 = theta2;

    theta1Deg = theta1 * (180/PI);
    theta2Deg = theta2 * (180/PI);

    if (theta1Deg >= -170 && theta1Deg <= 170 && theta2Deg >= -170 && theta2Deg <= 170)
    {
      usAJNT1 = map(theta1Deg, -180, 180, 1000, 2000);
      usAJNT2 = map(theta2Deg, -180, 180, 2000, 1000);

      setSpeedJoint1(usAJNT1);
      setSpeedJoint2(usAJNT2);
      setSpeedEndE(1500);
    }

    String message = String(String(x_coord) + " x_coord\t\t" + String(y_coord) + " y_coord\t\t"
     + String(xArm) + " xPos\t" + String(yArm) + " yPos\t" + String(theta1) + " rad\t"
     + String(theta2) + " rad\t" + String(theta1Deg) + " deg\t" + String(theta2Deg)
     + " deg\t" + String(usAJNT1) + " PWM\t" + String(usAJNT2) + " PWM");
    
    char charBuffer[message.length()];
    
    message.toCharArray(charBuffer,message.length());
//    Serial.println(charBuffer);
    
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(charBuffer);
    Udp.endPacket();
//    delay(1000);

//    Serial.print(x_coord);
//    Serial.print(" x_coord\t");
//    Serial.print(y_coord);
//    Serial.print(" y_coord\t");
//    Serial.print(xArm);
//    Serial.print(" xPos\t");
//    Serial.print(yArm);
//    Serial.print(" yPos\t");
//    Serial.print(theta1);
//    Serial.print(" 1rad\t");
//    Serial.print(theta2);
//    Serial.print(" 2rad\t");
//    Serial.print(theta1Deg);
//    Serial.print(" 1deg\t");
//    Serial.print(theta2Deg);
//    Serial.print(" 2deg\t");
//    Serial.print(usAJNT1);
//    Serial.print(" 1PWM\t");
//    Serial.print(usAJNT2);
//    Serial.println(" 2PWM");

//    Udp.write(xArm);
//    Udp.write(" xPos\t");
//
//    Udp.write(yArm);
//    Udp.write(" yPos\t");
//
//    Udp.write(theta1);
//    Udp.write(" 1rad\t");
//
//    Udp.write(theta2);
//    Udp.write(" 2rad\t");
//
//    Udp.write(theta1Deg);
//    Udp.write(" 1deg\t");
//
//    Udp.write(theta2Deg);
//    Udp.write(" 2deg\t");
//
//    Udp.write(usAJNT1);
//    Udp.write(" 1PWM\t");
//
//    Udp.write(usAJNT2);
//    Udp.write(" 2PWM\n");
//    
  }
  delay(10);
}


double solveTheta1(double x, double y, double l1, double l2)
{
  double c2 = (x*x + y*y - l1*l1 - l2*l2) / (2*l1*l2*1.0f);
  double s2 = sqrt(1 - c2*c2);

  double k1 = l1 + (l2*c2);
  double k2 = l2*s2;

  double t1 = atan2(y,x) - atan2(k2,k1);

  return t1;
}

double solveTheta2(double x, double y, double l1, double l2)
{
  double c2 = (x*x + y*y - l1*l1 - l2*l2) / (2*l1*l2*1.0f);
  double s2 = sqrt(1 - c2*c2);

  double t2 = atan2(s2,c2);

  return t2;
}


void setSpeedJoint1(uint16_t val)
{
  armJoint1.writeMicroseconds(val);
}

void setSpeedJoint2(uint16_t val)
{
  armJoint2.writeMicroseconds(val);
}

void setSpeedEndE(uint16_t val)
{
  endeJoint.writeMicroseconds(val);
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}












