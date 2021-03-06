#include <LSM303.h>
#include <Servo.h>
#include <Wire.h>

LSM303 compass;         //magnetometer constructor
Servo myservo;          //servo constructor

float d = 3.86;       //diameter of wheel in inches
float circ = (PI * d)/3;    // arc length between two magnets on wheel in inches
float desiredDirection = 0;    //Put the direction you want the robot to move towards
int servoPin = 3;       // Pin that the servomotor is connected to
int solenoidPin = 2;    // Pin that the mosfet is conected to
int switchPin = 4;      // Pin that the switch is conected to
int startingPos = 95;   // starting position of the servo (pick from 0 to 180)
int switchState;
int servoDir = 0;       // Variable that stores the servo direction value as decided by the magnetometer
int solenoidState = LOW;           
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 250;           // interval at which to blink (milliseconds)
int previousSwitchState = 0;  // Holds previous switch value to allow for comparison between current and last state
float distTraveled = 0;    //total distance traveled in inches
const int quarterTurnDist = 12;  //arc length of turn radius
const int initDist = 64;

void setup() {
  myservo.attach(servoPin);               // attaches the servo on pin 9 to the servo object
  pinMode(solenoidPin, OUTPUT);           //Sets the pin as an output
  pinMode(switchPin, INPUT_PULLUP);       //Sets the pin as an input_pullup
  Serial.begin(9600);                     // starts serial communication @ 9600 bps
  Wire.begin();
  compass.init();
  compass.enableDefault();
  while (millis() <= 15000)
  {
    //do nothing
  }
  myservo.write(startingPos);     //resets the position of the servo
                    //give enough time to upload and disconnect laptop
}

void loop() {
////////////// REED SWITCH ///////////////////////////////////////////////////
  previousSwitchState = switchState;
  switchState = digitalRead(switchPin);

  if (switchState != previousSwitchState)    // if the read switch detects any change
  {
    switch(switchState)
    {
      case 0:                                // a magnet has been detected, meaning that the wheel has turned 1/4th of its circumference
      distTraveled = distTraveled + circ;    // add 1/4 of circumference of wheel to total distance traveled since this case only runs if a new magnet is detected
      Serial.print("Distance Traveled: ");
      Serial.println(distTraveled/12);       // print the total distance traveled in feet
      break;
      case 1:                                // does nothing since we only care about a new magnet getting detected
      
      break;
    }
  }

    
////////////// MAGNETOMETER ///////////////////////////////////////////////////
compass.read();
float heading = compass.heading();  //compass.heading() returns the angle diff between north and its current direction



////////////// SERVOMOTOR ///////////////////////////////////////////////////
if (distTraveled <= initDist)
{
  myservo.write(startingPos);
}
// make servo angle turn proportionally to the difference in angle between desired and current angle
//algorithm assumes desired direction is north

else
{
  if (distTraveled <= initDist + quarterTurnDist)  //if we need to turn left into channel
  {
    myservo.write(85);    //steer to the left, change to 105 if need right turn
 }
  else
 {
    myservo.write(startingPos);
  }
  
  delay(10); 
}
////////////// SOLENOID VALVE ///////////////////////////////////////////////////
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (solenoidState == LOW) {
      solenoidState = HIGH;
    } else {
      solenoidState = LOW;
    }
    digitalWrite(solenoidPin, solenoidState);    //Switch Solenoid ON/oFF
  }


////////////// Serial Print  ///////////////////////////////////////////////////
Serial.print("Reed Switch: ");
Serial.print(switchState);
Serial.print("   Magnetometer: ");
Serial.print(heading);
Serial.print("  Servo Direction: ");
Serial.println(servoDir);

////////////// Stop Actuating ///////////////////////////////////////////////////
if (millis() >= 75000)
{
  delay(3600000);
}
}

