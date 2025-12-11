#include <Random16.h> // Import rng library
Random16 rnd;
/*
 */

// Function used to only light
void lightUpOnly(int x){
  // Loops through all the lights
  for (int i = 2; i < 8; i++){
    // if the pin number matches the one inputted in the function, it will light up
    if (i == x){
      digitalWrite(i, HIGH);
    }
    // otherwise keep it off
    else{
      digitalWrite(i, LOW);
    }
  }
}


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
  int num = analogRead(A5); // The potentiometer will determine how long the delays are

  // If button not pressed
  if (digitalRead(8) == LOW){ 
    // Lights up lights from left to right
    for (int i = 2; i < 8; i++){
      digitalWrite(i, HIGH); 
      delay(num);
      Serial.print(num);
    }
    
    // Dims lights from right to left
    for (int i = 7; i > 1; i--){
      digitalWrite(i, LOW);
      delay(num);
    }
  }
  else {
    // If the potentiometer is all the way down
    if (num < 300){ 
      int random = rnd.get(2, 8); // generates a random number from 2 - 7
      lightUpOnly(random);
      delay(1000);
    }
    else {
      // These are intervals. Individual lights will light up depending on how much you turn it
      if (num >= 900){
        lightUpOnly(8);
      }else if (num >= 800){
        lightUpOnly(7);
      }else if (num >= 700){
        lightUpOnly(6);
      }else if (num >= 600){
        lightUpOnly(5);
      }else if (num >= 500){
        lightUpOnly(4);
      }else if (num >= 400){
        lightUpOnly(3);
      }else if (num >= 300){
        lightUpOnly(2);
      }
    }
  } 
}
