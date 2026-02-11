// The pin connected to the speaker
#define outpin 8

int debug1 = 3; // Lights up when a dot is registered. When the button is held it'll turn off and switch to debug2 since a dash is registered instead.
int debug2 = 4; // Lights up when a dash is registered.
int input = 5; // Detects the button being pressed.
String symbol = ""; 
bool write = false; // by default the arduino shouldn't print anything because the user hasnt pressed the button yet
String message = ""; // the cummalative dots and dashes

unsigned long time = 0; // Will be used to track how long a button is held for.
int buttonPrevState = 0; // The current state will constantly be compared to the previous state (this variable) in order to know when to keep track of how long the button has been held.
bool buttonReleased = true; // by default this is true because the user won't be holding the button when the program starts

void setup() {
    pinMode(input, INPUT);
    pinMode(debug1, OUTPUT);
    pinMode(debug2, OUTPUT);
    Serial.begin(115200); // The baud rate is higher than the usual 9600 because the "."s and "-"s won't render properly at 9600 for some reason.
}

void loop() {
  /*
    https://morsecode.ninja/reference/
    
    In morse code, a dot and a dash are both represented by units of time.
    A dot is 1 unit
    A dash is 3 units

    However, coding a dot to register specifically between 50ms and 150ms would be quite annoying both to me the programmer and the user.
    Therefore, I'll be using a slightly different system where a dash is one unit of time. If the button is held for anything less than that unit of time a dot will be registered.
    The unit of time is currently 150ms.
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
    noTone(outpin); // stop playing the tone
    digitalWrite(debug1, LOW);
    digitalWrite(debug2, LOW);

    // If the button was released right after a hold,
    if (write){
      message += symbol; // add the registered symbol to the message
     // Serial.println("\n\n\n\n\n\n\n\n\n\n\n"); // <- clears the screen
      Serial.println(message); // print the new message
      write = false; // then prevent the arduino from printing anything again
    }
  }
  // When the button is pressed
  if (!buttonReleased){
    tone(outpin, 500); // play a 500 mhz tone
    if (millis() - time >= 150){
      digitalWrite(debug1, HIGH);
      digitalWrite(debug2, LOW);
      symbol = "-"; // If it was held for over 150ms then it will register a dash. 
    }
    else{
      digitalWrite(debug1, LOW);
      digitalWrite(debug2, HIGH);
      symbol = "."; // Otherwise a dot will be registered.
    }
    write = true; // Since the button is held, it is guarenteed that a write should be made the next time input is found to be LOW.
  }
  delay(25); // slows loop down to prevent the arduino from registering something twice by accident
}
