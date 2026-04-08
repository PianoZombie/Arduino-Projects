String morse[] = {".-", "-...", "-.-.", "-..",".", "..-.", "--.", "....", "..", ".---",
                   "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
                   "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--",
                   "....-", ".....", "-....", "--...", "---..", "----.", "-----", ".-.-.-",
                   "--..--", "..--.."};
String letter[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
                    "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
                    "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4",
                    "5", "6", "7", "8", "9", "0", ".", ",", "?"};

// The pin connected to the speaker
#define outpin 8

// Lights up when a dot is registered. When the button is held it'll turn off and switch to debug2 since a dash is registered instead.
#define debug1 3

// Lights up when a dash is registered.
#define debug2 4

// Detects when a dot or dash is being inputted.
#define input 5

// Detects when you press the translate button.
#define translate 6

String symbol = ""; 
bool write = false; // by default the arduino shouldn't print anything because the user hasnt pressed the button yet
String message = ""; // the cummalative dots and dashes
// int length = 0; // Tracks the length of the morse code.

unsigned long time = 0; // Will be used to track how long a button is held for.
int buttonPrevState = 0; // The current state will constantly be compared to the previous state (this variable) in order to know when to keep track of how long the button has been held.
bool buttonReleased = true; // by default this is true because the user won't be holding the button when the program starts

void setup() {
    pinMode(input, INPUT);
    pinMode(translate, INPUT);
    pinMode(debug1, OUTPUT);
    pinMode(debug2, OUTPUT);
    Serial.begin(9600); // The baud rate is higher than the usual 9600 because the "."s and "-"s won't render properly at 9600 for some reason.
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
      //length += 1;
      //Serial.println(length);
      write = false; // then prevent the arduino from printing anything again
    }
  }
  // When the button is pressed
  if (!buttonReleased){
    tone(outpin, 500); // play a 500 hz tone

    if (millis() - time >= 1000){
      digitalWrite(debug1, HIGH);
      digitalWrite(debug2, HIGH);
      symbol = " N "; // denotes a space between words.
    }
    else if (millis() - time >= 500){
      digitalWrite(debug1, LOW);
      digitalWrite(debug2, LOW);
      symbol = " "; // denotes the end of a letter.
    }
    else if (millis() - time >= 150){
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
  
  if(digitalRead(translate) == HIGH){
    message += " ";
    String code = "";
    String letters = "";
    for (int i = 0; i < message.length(); i++){
      if (message[i] == ' '){ // Spaces will denote that there is a letter that needs to be translated.
        Serial.println(code);
        for (int j = 0; j < 39; j++){
          // Compares the current set of dots and dashes against every valid code.
          // If it finds a match it'll add it's corresponding letter. Otherwise nothing will be added.
          if (code == morse[j]){
            letters += letter[j];
            break;
          }
        }
        code = ""; // resets the variable so it doesn't accidentally grab dots and dashes from a previous letter
      }
      else if (message[i] == 'N'){
        letters += " ";
      }
      else {
        code += message[i];
      }
    }
    Serial.println(letters);
    message = "";
    delay(500);
  }
  
  delay(25); // slows loop down to prevent the arduino from registering something twice by accident
}
