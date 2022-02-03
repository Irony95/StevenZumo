/** Arduino Uno Timer 2 Servo Example
This example code for the Arduino Uno shows how to use Timer 2 on
the ATmega328P to control a single servo.  This can be useful for
people who cannot use the Arduino IDE's Servo library.  For
example, ZumoMotors uses the same timer as the Servo
library (Timer 1), so the two libraries conflict.
 
The SERVO_PIN macro below specifies what pin to output the 
servo on.  This pin needs to be connected to the signal input
line of the servo.  The Arduino's GND needs to be connected to
the ground pin of the servo.  The servo's ground and power pins
need to be connected to an appropriate power supply.
*/
#include <ZumoMotors.h>
#include <Adafruit_SoftServo.h>
#include <NewPing.h>
ZumoMotors motors;

#define TRIGGER_PIN  6 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A0
#define ECHO_PINL     2  
#define ECHO_PINR     11
#define SERVO_PIN 13
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define PUSH_DISTANCE 10
 
// This line specifies what pin we will use for sending the
// signal to the servo.  You can change this.
#define SERVO_PIN 13
Adafruit_SoftServo flipper;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  
 
void setup()
{
   // Set up the interrupt that will refresh the servo for us automagically
  OCR0A = 0xAF;            // any number is OK
  TIMSK0 |= _BV(OCIE0A);    // Turn on the compare interrupt (below!)

  flipper.attach(SERVO_PIN);
  Serial.begin(115200);
}
 
void loop()
{
  motors.setSpeeds(200,200);
  int cm = sonar.ping_cm();
  if (cm < 10 && cm != 0)
  {
    Serial.println("flipped");
    flipper.write(20);
  }
  else if (cm > 10 || cm == 0)
  {
    Serial.println("resting");
    flipper.write(100);
  }
}

volatile uint8_t counter = 0;
SIGNAL(TIMER0_COMPA_vect) {
  // this gets called every 2 milliseconds
  counter += 2;
  // every 20 milliseconds, refresh the servos!
  if (counter >= 20) {
    counter = 0;
    flipper.refresh();   
  }
}
