//Program to read QTR Value 0 or 5 using serial port
//Comment out Serialport statements after verifying QTR values
//White <100 Black >500-700
//QTR Threshold White 200 using battery Black 2000
#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
 
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  400 // microseconds
 
ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
 
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
 
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

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
  Serial.begin(9600);
  waitForButtonAndCountDown();
}

void loop()
{
  if (button.isPressed())
  {
    // if button is pressed, stop and wait for another press to go again
    button.waitForRelease();
    waitForButtonAndCountDown();
  }
   
  sensors.read(sensor_values);

  Serial.print("Left = ");
  Serial.print(sensor_values[0]);//Left Sensor
  Serial.print(" ");

  Serial.print("Right = ");
  Serial.println(sensor_values[5]);//Right Sensor

  if (sensor_values[0] > QTR_THRESHOLD)//Left Sensor
  {
  
  }
  else if (sensor_values[5] > QTR_THRESHOLD)//Right Sensor
  {
   
  }
  else
  {
  
  }
}
