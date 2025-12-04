

void setup() {
    pinMode(13, OUTPUT); // initialize digital pin 13 as an output
    pinMode(12, OUTPUT); // initialize digital pin 13 as an output
}

void loop() {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    delay(2000);
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    delay(2000);
}
