#include <Random16.h> // Import rng library
Random16 rnd;
/*
 */


void setup(){
  pinMode(2, OUTPUT); // green
  pinMode(3, OUTPUT); // red 1
  pinMode(4, OUTPUT); // white 1
  pinMode(5, OUTPUT); // white 2
  pinMode(6, OUTPUT); // white 3
  pinMode(7, OUTPUT); // red 2
  pinMode(8, INPUT);  // button
  Serial.begin(9600);
}

void loop() {
  int num = analogRead(A0); // The potentiometer will determine how long the delays are

  if (digitalRead(8) == LOW){ // If button not pressed
    // Lights up lights from left to right
    for (int i = 2; i < 8; i++){
      digitalWrite(i, HIGH); 
      delay(num);
    }
    
    // Dims lights from right to left
    for (int i = 7; i > 1; i--){
      digitalWrite(i, LOW);
      delay(num);
    }
  }
  else {
    int random = rnd.get(2, 8); // generates a random number from 2 - 7

    // The loop will cycle through all the lights (represented by i)
    for (int i = 2; i < 8; i++){
      
      // if i is the same as the random number it will light up
      if (i == random){
        digitalWrite(i, HIGH);
      }
      // otherwise just leave it off
      else {
        digitalWrite(i, LOW);
      }
    }
    delay(num);
  }
}
