#include <Servo.h>
Servo myServo; // create a servo object called myServo
const int potPin = A0; // set the name of pin A0 to potPin
const int buttonPin = 7; // the button pin
int potValue = 0; // variable to read the value from the potentiometer
int servoAngle = 0;

void setup() {
    myServo.attach(9);
    Serial.begin(9600);
}

void loop() {
    if (digitalRead(buttonPin) == HIGH){
      myServo.write(179);
    }
    else{
      potValue = analogRead(potPin);
      servoAngle = map(potValue, 0 ,1023, 0, 179); // scale potentiometer value to the servo
      myServo.write(servoAngle);
    }
    delay(100);
}
