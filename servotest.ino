#include <Servo.h>
#include <Wire.h>

Servo myservo;

int servoDir = 0;
int servoPin = 3;
int startingPos = 90;
int pos = 90;

void setup() {
  // put your setup code here, to run once:
myservo.attach(servoPin);
Serial.begin(9600);
Wire.begin();
myservo.write(startingPos);
}

void loop() {
  // put your main code here, to run repeatedly:

 if(servoDir == 0)
  {
    pos++;
    if(pos >= 110)
    {
      servoDir = 1;
    }
  }
  else
  {
    pos--;
    if(pos <= 70)
    {
      servoDir = 0;
    }
  }

  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(10); 


}

