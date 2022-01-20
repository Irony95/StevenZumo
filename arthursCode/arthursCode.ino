#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <ZumoMotors.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIGGER_PIN  6 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A0
#define ECHO_PINL     2  
#define ECHO_PINR     11
#define ECHO_PINB     13
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define PUSH_DISTANCE 10

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  
NewPing sonarL(TRIGGER_PIN, ECHO_PINL, MAX_DISTANCE); 
NewPing sonarR(TRIGGER_PIN, ECHO_PINR, MAX_DISTANCE);
NewPing sonarB(TRIGGER_PIN, ECHO_PINB, MAX_DISTANCE);

#define LED 13
#define QTR_THRESHOLD  1000 // my robot 300
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     100 // 0 is stopped, 400 is full speed
#define TURN_SPEED        80
#define FORWARD_SPEEDL     310
#define FORWARD_SPEEDR     300
#define FORWARD_PUSH       800
#define REVERSE_DURATION  20 // ms
#define FORWARD_DURATION  100 // ms
#define TURN_DURATION     80 // ms
 
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
unsigned int var;
 
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
   
long cm, cmL, cmR;
Servo myservo;
Int pos = 0


void waitForButtonAndCountDown()
{
  //digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);
   
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

void TurnL() 
{   
  int amtChecks = 100;
  byte var=0;
  while(var<amtChecks)
  {    
    Serial.println("checking L");
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
   delay(3000/amtChecks);

    var++;
    cm = sonar.ping_cm();
    if((cm <PUSH_DISTANCE)&&(cm>0))break;
  }
}

void TurnR() 
{   
  int amtChecks = 100;
  byte var=0;
  while(var<amtChecks)
  {    
    Serial.println("checking R");
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
   delay(3000/amtChecks);
  
    var++;
    cm = sonar.ping_cm();
    if((cm <PUSH_DISTANCE)&&(cm>0))break;
  }
}

void Reverse() 
{
  motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  delay(1000);
}
  
void Forward() 
{ 
  cm = sonar.ping_cm();
  delay(10);
  
  sensors.read(sensor_values);
  //Charging
  if((cm <PUSH_DISTANCE)&&(cm>0) &&(sensor_values[0] < QTR_THRESHOLD)&&(sensor_values[5] < QTR_THRESHOLD))
  {
    Serial.println("PUSHSS");
    motors.setSpeeds(FORWARD_PUSH, FORWARD_PUSH);
    delay(250);
    motors.setSpeeds(FORWARD_SPEEDL, FORWARD_SPEEDR); 
    delay(250);
    buzzer.playNote(NOTE_G(6), 500, 15);//Buzzer tone signaling attack mode
  }

  //Normal forward Movement
  else if((cm >=PUSH_DISTANCE)||(cm==0))
  {
    Serial.println("usual");
    motors.setSpeeds(FORWARD_SPEEDL, FORWARD_SPEEDR); 
    buzzer.playNote(NOTE_G(4), 500, 10);//Buzzer tone signaling normal mode
  }  
}

void setup() 
{
  //pinMode(LED, HIGH); 
  Serial.begin(115200);
  waitForButtonAndCountDown();
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
  cmR = sonarR.ping_cm();  
 
  cmL = sonarL.ping_cm();  

  cmB = sonarB.ping_cm();  
  

  Forward();//Forward Subroutine is repetatively executed 
  
  //with code below for Border detect
  if ((sensor_values[0] > QTR_THRESHOLD)||(sensor_values[5] > QTR_THRESHOLD))
  {
    Serial.println("hitBorder");
    Reverse();
    TurnL();
  }
  else if ((cmL<50) && (cmL>0))
  {
   TurnL();
   Serial.println("Left");
  }
  else if ((cmR<50) && (cmR>0))
  {
   TurnR();
   Serial.println("Right");
  }
}
