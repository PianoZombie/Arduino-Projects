#define outpin 8

int debug1 = 3; // Lights up when a dot is registered. When the button is held it'll turn off and switch to debug2, since a dash is registered instead.
int debug2 = 4; // lights up when a dash is registered
int input = 5;
String symbol = "";
bool write = false; // by default the arduino shouldn't print anything because the user hasnt pressed the button yet
String message = ""; // the cummalative dots and dashes

unsigned long time = 0;
int buttonPrevState = 0;
bool buttonReleased = true; // by default this is true because the user won't be holding the button when the program starts

void setup() {
    pinMode(input, INPUT);
    pinMode(debug1, OUTPUT);
    pinMode(debug2, OUTPUT);
    Serial.begin(115200);
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
      Serial.println("\n\n\n\n\n\n\n\n\n\n\n"); // <- clears the screen
      Serial.println(message); // print the registered symbol
      write = false; // then prevent the arduino from printing anything again.
    }
  }
  // The char to be printed will be determined while the button is being held.
  if (!buttonReleased){
    freqout(1000, 1);
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
  delay(25); // slows loop down to prevent accidental double clicks from doing anything
}

void freqout(int freq, int t)  // freq in hz, t in ms
{
  int hperiod;                               //calculate 1/2 period in us
  long cycles, i;
  pinMode(outpin, OUTPUT);                   // turn on output pin

  hperiod = (500000 / freq) - 7;             // subtract 7 us to make up for digitalWrite overhead

  cycles = ((long)freq * (long)t) / 1000;    // calculate cycles
 // Serial.print(freq);
 // Serial.print((char)9);                   // ascii 9 is tab - you have to coerce it to a char to work
 // Serial.print(hperiod);
 // Serial.print((char)9);
 // Serial.println(cycles);

  for (i=0; i<= cycles; i++){              // play note for t ms
    digitalWrite(outpin, HIGH);
    delayMicroseconds(hperiod);
    digitalWrite(outpin, LOW);
    delayMicroseconds(hperiod - 1);     // - 1 to make up for digitaWrite overhead
  }
pinMode(outpin, INPUT);                // shut off pin to avoid noise from other operations

}
