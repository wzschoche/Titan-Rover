import com.leapmotion.leap.*;
import processing.net.*;
import hypermedia.net.*;

UDP udp;

Client arm;
Controller leap;
boolean work = false;

void setup()
{
  udp = new UDP(this, 6000);
  udp.listen(true);
  
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
  HandList hands = leap.frame().hands();
  Hand hand = hands.get(0);
  FingerList fingers = hand.fingers();
  Vector hp = hand.palmPosition();
  
  Pointable f1 = fingers.get(0);
  Pointable f2 = fingers.get(1);
  
  float ff1 = f1.tipPosition().getX();
  float ff2 = f2.tipPosition().getX();
  float sub = ff1 - ff2;
  
  float pitch = hand.direction().pitch() * 100;
  
  if(hp.getY() < 150)  hp.setY(150);
  if(hp.getY() > 445)  hp.setY(445);
  if(hp.getZ() < -180) hp.setZ(-180);
  if(hp.getZ() > 180)  hp.setZ(180);
  
  float zv = map(hp.getZ(), -180, 180, 101, 1);
  float yv = map(hp.getY(), 150, 445, 1, 101);
  
  double xv = 180 - cba(-hand.palmPosition().getX() / 1.5);
  
  float pv = map(pitch, -90, 100, 160, 6);
  float gv = map(sub, 20, 90, 145, 73);
  
  if (fingers.count() >= 2) work = true;
  else work = false;
  
  //if (work && zv <= 180 && zv >= 0 && yv <= 150 && yv >= 0 && xv <= 180 && xv >= 0 && pv >= 6 && gv <= 145 && gv >= 73)
  //{
  //  String v1 = (int)xv + "P";
  //  String v2 = (int)zv + "Q";
  //  String v3 = (int)yv + "F";
  //  String v4 = (int)pv + "K";
  //  String v5 = (int)gv + "L";
  //}
  
  background(100);
  fill(255);
  textSize(height / 8);
  text(pv, 90, 100);
  text(yv, 90, 180);
  text(zv, 90, 260);
  text((float)xv, 90, 340);
  text(gv, 90, 420);
  
  String message = str(int(yv)) + "/" + str(int(zv)) + "/";
  //println(message);
  String ip = "192.168.1.101";
  int port = 8888;
  
  udp.send(message, ip, port);
  //println(message);
  
}

void receive(byte[] data, String ip, int port)
{
  //println(data);
  data = subset(data, 0, data.length);
  String message = new String(data);
  
  //println("receive: \""+message+"\" from "+ip+" on port "+port);
  println(message);
} 