/*
   Island City Explorers
   Servo Motor Testing Code, obtained from source below
   V 1.0
   Release: 2/17/21
   S. Shaw

*/
/*
     Servo Motor Control - 50Hz Pulse Train Generator
           by Dejan, https://howtomechatronics.com
*/
#define servoPin 9
void setup() {
  pinMode(servoPin, OUTPUT);
}
void loop() {
  // A pulse each 20ms
  for (int i = 0; i < 20; i++) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(600); // Duration of the pusle in microseconds
    digitalWrite(servoPin, LOW);
    delayMicroseconds(19400); // 20ms - duration of the pusle
  }
  for (int i = 0; i < 20; i++) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(1450); // Duration of the pusle in microseconds
    digitalWrite(servoPin, LOW);
    delayMicroseconds(18550); // 20ms - duration of the pusle
  }
  for (int i = 0; i < 20; i++) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(2300); // Duration of the pusle in microseconds
    digitalWrite(servoPin, LOW);
    delayMicroseconds(17700); // 20ms - duration of the pusle
    // Pulses duration: 600 - 0deg; 1450 - 90deg; 2300 - 180deg
  }
}
