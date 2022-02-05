// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------
#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <ZumoMotors.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A0
#define ECHO_PINL     2  
#define ECHO_PINR     11
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  
NewPing sonarL(TRIGGER_PIN, ECHO_PINL, MAX_DISTANCE); 
NewPing sonarR(TRIGGER_PIN, ECHO_PINR, MAX_DISTANCE);

long cm, cmL, cmR, cmB;

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  cm = sonar.ping_cm();
  delay(50); 
 
  cmR = sonarR.ping_cm();
  delay(50);
 
  cmL = sonarL.ping_cm();
  delay(50);    
  
  delay(50);   
  Serial.print("Front: ");
  Serial.print(cm); 
    Serial.print("cm");

  Serial.print("  Right ");
  Serial.print(cmR); 
  Serial.print("cm   ");

  Serial.print("  Left ");
  Serial.print(cmL); 
  Serial.println("cm");
}
