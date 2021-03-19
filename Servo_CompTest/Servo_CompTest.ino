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
#define servoPin 20
void setup() {
  pinMode(servoPin, OUTPUT);
}
void loop() {
  // A pulse each 20ms
  for (int i = 0; i < 10; i++) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(900); // Duration of the pusle in microseconds
    digitalWrite(servoPin, LOW);
    delayMicroseconds(20000 - 900); // 20ms - duration of the pusle
  }
  delay(1000);
  /*for (int i = 0; i < 20; i++) {
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
  }*/
}
