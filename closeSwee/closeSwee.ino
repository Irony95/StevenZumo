#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <ZumoMotors.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>
#include <Adafruit_SoftServo.h>

#define TRIGGER_PIN  6 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A0
#define ECHO_PINL     2  
#define ECHO_PINR     11
#define SERVO_PIN 13
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define PUSH_DISTANCE 10

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  
NewPing sonarL(TRIGGER_PIN, ECHO_PINL, MAX_DISTANCE); 
NewPing sonarR(TRIGGER_PIN, ECHO_PINR, MAX_DISTANCE);

#define QTR_THRESHOLD  1000 // my robot 300
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEEDL     810
#define FORWARD_SPEEDR     300
#define FORWARD_PUSH       300
#define REVERSE_DURATION  20 // ms
#define FORWARD_DURATION  100 // ms
#define TURN_DURATION     40 // ms
 
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
unsigned int var;
 
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
   
long cm, cmL, cmR;
Adafruit_SoftServo flipper;
int restingAngle = 60, flippedAngle=100, pos = restingAngle;




void waitForButtonAndCountDown()
{
  button.waitForButton();
   
  // play audible countdown
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);  
  delay(1000);
}

void setup() 
{
  Serial.begin(115200);
  waitForButtonAndCountDown();

  flipper.attach(SERVO_PIN); 
}

void loop() 
{  
  //After pwering up, Waits for user to press start button
  if (button.isPressed())
  {
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    waitForButtonAndCountDown();//Execute countdown
  }
  motors.setSpeeds(100, 100); 
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    flipper.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
    Serial.println(pos);
    flipper.refresh();
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    flipper.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
    Serial.println(pos);
    flipper.refresh();
  }
//  cm = sonar.ping_cm();
//  Serial.println(cm);
//  if (cm < 10 && cm != 0)
//  {
//    Serial.println("flippin");
//    pos = flippedAngle;
//    flipper.write(pos);
//  }
//  else if (cm > 10 || cm == 0)
//  {
//    Serial.println("resting");
//    pos = restingAngle;
//    flipper.write(pos);
//  }
}
