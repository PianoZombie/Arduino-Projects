int debug1 = 3; // Lights up when a dot is registered. When the button is held it'll turn off and switch to debug2, since a dash is registered instead.
int debug2 = 4; // lights up when a dash is registered
int input = 5;
char symbol = "";
bool write = false; // by default the arduino shouldn't print anything because the user hasnt pressed the button yet
String message = ""; // the cummalative dots and dashes

unsigned long time = 0;
int buttonPrevState = 0;
bool buttonReleased = true; // by default this is true because the user won't be holding the button when the program starts

void setup() {
    pinMode(input, INPUT);
    pinMode(debug1, OUTPUT);
    pinMode(debug2, OUTPUT);
    Serial.begin(9600);
}

void loop() {
  /*
    https://morsecode.ninja/reference/
    
    In morse code, a dot and a dash are both represented by units of time.
    A dot is 1 unit
    A dash is 3 units

    The unit of time is an aribtrary value, so for this project one unit = 200ms
  */

  int state = digitalRead(input);
  if (state != buttonPrevState){ // When the state changes
    if (state == HIGH){ 
      // If the state changed to HIGH,
      buttonReleased = false; // then that means the button is pressed
      time = millis(); // and we'll need to keep track of how long the button is held.
    }
    else {
      buttonReleased = true; // Otherwise the button got released.
    }
    buttonPrevState = state;
  }
  if (buttonReleased){
    digitalWrite(debug1, LOW);
    digitalWrite(debug2, LOW);

    // If the button was released right after a hold,
    if (write){
      message += symbol;
      Serial.println(message); // print the registered symbol
      Serial.println(symbol);
      write = false; // then prevent the arduino from printing anything again.
    }
  }
  // The char to be printed will be determined while the button is being held.
  if (!buttonReleased){
    if (millis() - time >= 200){
      digitalWrite(debug1, HIGH);
      digitalWrite(debug2, LOW);
      symbol = "-"; // If it was held for over 200ms then it will register a dash. 
    }
    else{
      digitalWrite(debug1, LOW);
      digitalWrite(debug2, HIGH);
      symbol = "."; // Otherwise a dot will be registered.
    }
    write = true; // Since the button is held, it is guarenteed that a write should be made the next time input is found to be LOW.
  }
  delay(50); // slows loop down to prevent accidental double clicks from doing anything
}
