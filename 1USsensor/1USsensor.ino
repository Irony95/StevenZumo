#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <ZumoMotors.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 50 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

#define LED 13
#define QTR_THRESHOLD  300 // my robot 300
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     100 // 0 is stopped, 400 is full speed
#define TURN_SPEED        80
#define FORWARD_SPEEDL     150
#define FORWARD_SPEEDR     150
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
   
long cm;

void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
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

  void Turn() 
  {   
    int amtChecks = 100;
    byte var=0;
    while(var<amtChecks)
    {
      Serial.println("turning");
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
     delay(3000/amtChecks);

      var++;
      cm = sonar.ping_cm();
      if((cm <10)&&(cm>0))break;
    }
  }

  void Reverse() 
  {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(1000);
  }
  
  void Forward() 
  { 
    cm = sonar.ping_cm();//Read ultrasonic distance
    delay(10);// Slight delay between pings 
    
    sensors.read(sensor_values);//Read reflectance sensors for Border check
    //This section of code checks for an opponent in close proximity
    Serial.println(cm);
    if((cm <10)&&(cm>0)&&(sensor_values[0] < QTR_THRESHOLD)&&(sensor_values[5] < QTR_THRESHOLD))
    {
      Serial.println("PUSHSS");
      motors.setSpeeds(FORWARD_PUSH, FORWARD_PUSH);
      delay(250);
      motors.setSpeeds(FORWARD_SPEEDL, FORWARD_SPEEDR); 
      delay(250);
      buzzer.playNote(NOTE_G(6), 500, 15);//Buzzer tone signaling attack mode
    }

    //This section of code checks that an opponent is not in range
    if((cm >=10)||(cm==0))
    {
      Serial.println("usual");
      motors.setSpeeds(FORWARD_SPEEDL, FORWARD_SPEEDR); 
      buzzer.playNote(NOTE_G(4), 500, 10);//Buzzer tone signaling normal mode
    }  
  }

  void setup() 
  {
  pinMode(LED, HIGH); 
  Serial.begin(9600);
  waitForButtonAndCountDown();
  }

void loop() {

  //After pwering up, Waits for user to press start button
  if (button.isPressed())
  {
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    waitForButtonAndCountDown();//Execute countdown
  }

  Forward();//Forward Subroutine is repetatively executed 
            //with code below for Border detect
  if ((sensor_values[0] > QTR_THRESHOLD)||(sensor_values[5] > QTR_THRESHOLD))
  {
    Reverse();
    Turn();
  }
}
