/*
 */
int readValue = 0;
int writeValue = 0;
void setup() {
    pinMode(3, OUTPUT); // Red light
    pinMode(4, OUTPUT); // Yellow light
    pinMode(5, OUTPUT); // Green light
    Serial.begin(9600);
}

void loop() {
    readValue = analogRead(A0); 
    writeValue = readValue / 4;
    analogWrite(3, writeValue); // set LED 1 brightness to writeValue
    analogWrite(4, writeValue); // set LED 2 brightness to writeValue
    analogWrite(5, writeValue); // set LED 3 brightness to writeValue
    Serial.print(readValue);
    Serial.print("  :  ");
    Serial.println(writeValue);
    delay(100);
}
