
import com.leapmotion.leap.*;
import processing.net.*;
import hypermedia.net.*;

final float LENGTH1 = 900.0;
final float LENGTH2 = 1050.0;

final String IP = "192.168.1.20";
//final String IP = "192.168.1.101";
final int PORT = 8888;

float xArm;
float yArm;
float wrist;
float gripper;

static float pastX;
static float pastY;
static float pastWrist;
static float pastGripper;

float theta1;
float theta2;
float theta1Deg;
float theta2Deg;

static float pastTheta1;
static float pastTheta2;

static float joint1PWM;
static float joint2PWM;

boolean firstLoopCycle = true;
boolean oneToOne = true;

UDP udp;

Client arm;
Controller leap;
boolean work = false;

void setup()
{
  udp = new UDP(this, 6000);
  udp.listen(true);
  
  frameRate(15);
  
  size(500,500);
  leap = new Controller();
}

double cba(double a)
{
  float n = 100 * 3;
  a = 1.5 + 2 * a / n;
  double angle = 90 + Math.cos(a) * 90;
  return angle;
}

void draw()
{
  /***********************************************
  
      Leap Motion Calculations
      
  ************************************************/
  
  HandList hands = leap.frame().hands();
  Hand handR = hands.get(0);
  Hand handL = hands.get(1);
  FingerList fingersR = handR.fingers();
  FingerList fingersL = handL.fingers();
  Vector hpR = handR.palmPosition();
  Vector hpL = handL.palmPosition();
  
  Pointable f1R = fingersR.get(0);
  Pointable f2R = fingersR.get(1);
  
  Pointable f1L = fingersL.get(0);
  Pointable f2L = fingersL.get(1);
  
  float ff1 = f1R.tipPosition().getX();
  float ff2 = f2R.tipPosition().getX();
  float sub = ff1 - ff2;
  
  float ff1L = f1L.tipPosition().getX();
  
  float pitchR = handR.direction().pitch() * 100;
  //float rollR = handR.direction().roll() * 100;
  
  if(hpR.getY() < 150)  hpR.setY(150);
  if(hpR.getY() > 445)  hpR.setY(445);
  if(hpR.getZ() < -180) hpR.setZ(-180);
  if(hpR.getZ() > 180)  hpR.setZ(180);
  
  float zv = map(hpR.getZ(), -180, 180, 101, 1);
  float yv = map(hpR.getY(), 150, 445, 1, 101);
  
  double xv = 180 - cba(-handR.palmPosition().getX() / 1.5);
  
  float pv = map(pitchR, -90, 100, 160, 6);
  float gv = map(sub, 20, 90, 145, 73);
  
  if (fingersR.count() >= 2) work = true;
  else work = false;
  
  //if (work && zv <= 180 && zv >= 0 && yv <= 150 && yv >= 0 && xv <= 180 && xv >= 0 && pv >= 6 && gv <= 145 && gv >= 73)
  //{
  //  String v1 = (int)xv + "P";
  //  String v2 = (int)zv + "Q";
  //  String v3 = (int)yv + "F";
  //  String v4 = (int)pv + "K";
  //  String v5 = (int)gv + "L";
  //}
  
  /***********************************************
  
      Inverse Kinematic Calculations
      
  ************************************************/ 
if (oneToOne)
{
  yArm = map(int(yv), 1, 101, -1950, 1950);
  xArm = map((int)zv, 1, 101, -1950, 1950);
  wrist = map((int)pv, -30, 200, 1000, 2000); 
  //if (gv < 160)
  //  gv = 160;
  //else if (gv > 320)
  //  gv = 320;
  //gripper = map((int)gv, 160, 320, 1, 101);
  wrist = checkPWMBounds(wrist);
  
  if (firstLoopCycle)
  {
    pastY = yArm;
    pastX = xArm;
    pastWrist = wrist;
    theta1 = PI/2;
    theta2 = PI/2;
    pastTheta1 = theta1;
    pastTheta2 = theta2;
    firstLoopCycle = false;
  }
  
  if ((int)yv == 1 && (int)zv == 51 && (int)pv == -167)
  {
    yArm = pastY;
    xArm = pastX;
    wrist = pastWrist;
    theta1 = pastTheta1;
    theta2 = pastTheta2;
  }
  
  else
  {
    theta1 = solveTheta1(xArm, yArm, LENGTH1, LENGTH2);
    theta2 = solveTheta2(xArm, yArm, LENGTH1, LENGTH2);
  }
  
  if (Float.isNaN(theta1) || Float.isNaN(theta2))
  {
    theta1 = pastTheta1;
    theta2 = pastTheta2;
  }
  //if (Float.isNaN(theta2))
  //{
  //  theta2 = pastTheta2;
  //}
  
  theta1Deg = theta1 * (180/PI);
  theta2Deg = theta2 * (180/PI);
  
  //debug th1 and th2
  if (theta1Deg > 180 || theta1Deg < -180 || theta2Deg > 180 || theta2Deg < -180)
  {
    print("theta OoB: ");
    print(theta1Deg);
    print(" ");
    print(theta2Deg);
    print(" ");
  }
  
  joint1PWM = map(theta1Deg, -180, 180, 1000, 2000);
  joint2PWM = map(theta2Deg, -180, 180, 2000, 1000);
  
  joint1PWM = checkPWMBounds(joint1PWM);
  joint2PWM = checkPWMBounds(joint2PWM);
  
  pastY = yArm;
  pastX = xArm;
  pastWrist = wrist;
  pastTheta1 = theta1;
  pastTheta2 = theta2;
}

else
{
  if (firstLoopCycle)
  {
    joint1PWM = 1500;
    joint2PWM = 1500;
    wrist = 1500;
    firstLoopCycle = false;
  }
  
  if ((int)yv == 1 && (int)zv == 51 && (int)pv == -167)
  {
    yv = 50;
    zv = 50;
    pv = 50;
  }
  
  if ((int)yv > 81)
  {
    joint1PWM += 20;
  }
  else if ((int)yv < 21)
  {
    joint1PWM -= 20;
  }
  
  if ((int)zv > 81)
  {
    joint2PWM += 20;
  }
  else if ((int)zv < 21)
  {
    joint2PWM -= 20;
  }
  
  if ((int)pv > 140)
  {
    wrist += 20;
  }
  else if ((int)pv < 10)
  {
    wrist -= 20;
  }
  
  joint1PWM = checkPWMBounds(joint1PWM);
  joint2PWM = checkPWMBounds(joint2PWM);
  wrist = checkPWMBounds(wrist);
  
}
  
  /***********************************************
  
      Output and Final Results
      
  ************************************************/  
  
  background(100);
  fill(255);
  textSize(height / 8);
  text((int)pv, 90, 100);
  text((int)yv, 90, 180);
  text((int)zv, 90, 260);
  text((float)xv, 90, 340);
  text((int)gv, 90, 420);
  text((int)ff1L, 90, 500);
  
  String message = str(int(joint1PWM)) + "/" + str(int(joint2PWM)) + "/" + str(int(wrist)) + "/";
  
  //println(message);
  //String ip = "192.168.1.105";
  //int port = 8888;
  
  udp.send(message, IP, PORT);
  //println(message);
  //print(int(zv));
  //print(" x\t");
  //print(int(yv));
  //print(" y\t");
  //print(int(pv));
  //print(" wrist\t");
  //print(theta1);
  //print(" th1R\t");
  //print(theta2);
  //print(" th2R\t");
  //print(theta1Deg);
  //print(" th1D\t");
  //print(theta2Deg);
  //print(" th2D\t");
  //print(joint1PWM);
  //print(" j1PWM\t");
  //print(joint2PWM);
  //print(" j2PWM\t");
  //print(wrist);
  //println(" wrPWM");
  //delay(100);
}

  /***********************************************
  
      Receive Info from Arduino
      
  ************************************************/ 

void receive(byte[] data, String ip, int port)
{
  //println(data);
  data = subset(data, 0, data.length);
  String message = new String(data);
  
  //println("receive: \""+message+"\" from "+ip+" on port "+port);
  println(message);
}

  /***********************************************
  
      Math Functions
      
  ************************************************/ 

float solveTheta1(float x, float y, float l1, float l2)
{
  float c2 = (x*x + y*y - l1*l1 - l2*l2) / (2*l1*l2*1.0f);
  float s2 = sqrt(1 - c2*c2);
  
  float k1 = l1 + (l2*c2);
  float k2 = l2*s2;
  
  float t1 = atan2(y,x) - atan2(k2,k1);
  
  return t1;
}

float solveTheta2(float x, float y, float l1, float l2)
{
  float c2 = (x*x + y*y - l1*l1 - l2*l2) / (2*l1*l2*1.0f);
  float s2 = sqrt(1 - c2*c2);
  
  float t2 = atan2(s2,c2);
  
  return t2;
}

float checkPWMBounds(float PWMSignal)
{
  if (PWMSignal < 1000)
  {
    PWMSignal = 1000;
  }
  else if (PWMSignal > 2000)
  {
    PWMSignal = 2000;
  }
  
  return PWMSignal;
}