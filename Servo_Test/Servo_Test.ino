/* PROJECT: CSUF TITAN ROVER 2016
 * PROGRAM: Servo_Test
 * AUTHOR:  William Zschoche
 * DATE:    08-30-2015
 * 
 * NOTES:   360 degree servo PWM signal range is
 *          1000 to 2000.
 *  
 *          Change signal_input to desired positon.
 *          
 *          1000 == 180 degrees CCW position
 *          2000 == 180 degrees CW position
 * 
 * *****WARNING****WARNING****WARNING****WARNING****WARNING******
 * 
 * ***************   IF TESTING LONGWORTH CHUCK   ***************
 * 
 * The Longworth Chuck is limited to PWM range 1220 - 1480.
 * Exceeding these limits without removing the servo from the end
 * effector will result in damage to the end effector enclosure
 * and servo.
 */

#include <Servo.h>

/* Define a constant output pin. Doing this saves on memory, and
 * makes the code easier to read (if named well). Placing
 * constants at the top of the code make the code easier to
 * maintain if we decide to change the output pin.
 * MORE INFO: https://www.arduino.cc/en/Reference/Define
 */
#define END_OUT_PIN 7

void setup() {
  
  //Open Serial connection for debug output in Serial Monitor.
  Serial.begin(9600);

  /* Create a servo variable and set the ouput pin.
   * For more info on the servo librarty see here:
   * https://www.arduino.cc/en/Reference/Servo
   */
  Servo servoEnd;
  servoEnd.attach(END_OUT_PIN);

  Serial.println("DEBUG: END EFFECTOR SERVO TEST");
}

void loop() {

  // Flag so that messages don't repeat and clutter up the Serial
  // monitor.
  static boolean debug_flag = false;

  //Put your test value here:
  uint16_t signal_input = 1000;
  
  //Safety net in case of faulty input
  if(signal_input >= 1000 && signal_input <= 2000) {
    
    if(debug_flag == false)
      Serial.println("DEBUG: Sending signal...");
    
    digitalWrite(END_OUT_PIN, HIGH);
    delayMicroseconds(signal_input);
    digitalWrite(END_OUT_PIN, LOW);
    delayMicroseconds(20000-signal_input);

    if(debug_flag == false) {
      Serial.println("DEBUG: Moving to position and holding...");
      debug_flag = true;
    }
  }
  else {
    Serial.print("ERROR! INVALID INPUT: servo_input outside ");
    Serial.println("of range."); 
    Serial.print("Change servo_input to an integer between ");
    Serial.println("1000 - 2000. See documentation.");

    digitalWrite(END_OUT_PIN, HIGH);
    delayMicroseconds(1500);
    digitalWrite(END_OUT_PIN, LOW);
    delayMicroseconds(20000 - 1500);

    // Cause system to go into an infinite loop to "stop" the
    // program.
    while(1) { }
  }
}
