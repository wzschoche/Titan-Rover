
#include <SPI.h>
#include <WiFi101.h> 
#include <WiFiUdp.h>
#include <math.h>
#include <Servo.h>
//#include "PinChangeInt.h"


//wifi initialize
//IPAddress ip(192, 168, 1, 102);
int status = WL_IDLE_STATUS;
char ssid[] = "CSUF_TR";
char pass[] = "titanrover";
int keyIndex = 0;

unsigned int localPort = 8888;

char packetBuffer[255];

WiFiUDP Udp;

//arm initialize
#define CENTER_SIGNAL 1500
#define ENDE_SIGNAL 1615

#define AJNT1_OUT_PIN 3
#define AJNT2_OUT_PIN 6
#define ENDE_OUT_PIN 9
#define WRIST_OUT_PIN 11

Servo armJoint1;
Servo armJoint2;
Servo endeJoint;
Servo wristJoint;


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
  wristJoint.attach(WRIST_OUT_PIN);
//
  armJoint1.writeMicroseconds(CENTER_SIGNAL);
  armJoint2.writeMicroseconds(CENTER_SIGNAL);
  endeJoint.writeMicroseconds(ENDE_SIGNAL);
  wristJoint.writeMicroseconds(CENTER_SIGNAL);

  while (!Serial){;}
  
  Serial.println("checking shield");
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("Wifi shield not present");
    while(true);
  }

  //WiFi.config(ip);

  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  Serial.println("Connected to wifi");

  Serial.println("Starting connection to server . . .");

  printWifiStatus();
  
  Udp.begin(localPort);

  Serial.println("Done with Setup");
}


void loop()
{

  int usAJNT1 = 0;
  int usAJNT2 = 0;
  int usWRIST = 0;

  
  
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
        if (usAJNT1 == 0)
        {
          usAJNT1 = atoi(temp);
          strcpy(temp,"");
        }
        else if (usAJNT2 == 0)
        {
          usAJNT2 = atoi(temp);
          strcpy(temp,"");
        }
        else
        {
          usWRIST = atoi(temp);
          break;
        }
        loopCount = i + 1;
      }
    }
    
    Serial.print(usAJNT1);
    Serial.print(" joint1\t");
    Serial.print(usAJNT2);
    Serial.print(" joint2\t");
    Serial.print(usWRIST);
    Serial.print(" wrist\t");
    Serial.println(packetBuffer);




      setSpeedJoint1(usAJNT1);
      setSpeedJoint2(usAJNT2);
      setSpeedEndE(1500);
      setSpeedWrist(usWRIST);
    

    String message = String(usAJNT1) + " PWM\t" + String(usAJNT2) + " PWM\t" + String(usWRIST) + " PWM";
    
    char charBuffer[message.length()];

    //Serial.println(message);
    
    message.toCharArray(charBuffer,message.length());
//    Serial.println(charBuffer);
    
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(charBuffer);
    Udp.endPacket();
//    delay(1000);

  }
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

void setSpeedWrist(uint16_t val)
{
  wristJoint.writeMicroseconds(val);
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
