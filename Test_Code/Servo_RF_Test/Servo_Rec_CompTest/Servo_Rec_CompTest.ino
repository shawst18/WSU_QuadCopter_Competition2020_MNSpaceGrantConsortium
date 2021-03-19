/*
 * Island City Explorers
 * RF Link Transmitter Component Test code, to be used with dual AVR system, receiver code RF_RecCompTest & servo motor
 * V 1.0
 * Release: 3/12/21
 * M.Pelissero 
 * 
 */


#include <VirtualWire.h>


const int receive_pin = 8;
char servoChar[10];



struct package
{
  float servo = 0.0;
};


typedef struct package Package;
Package data;

#define servoPin 9

void setup() {
  pinMode(servoPin, OUTPUT);
  delay(1000);
  Serial.begin(9600);
  // Initialise the IO and ISR
  vw_set_rx_pin(receive_pin);
  vw_setup(500);   // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
}

void loop() {

  // Reciever link
  int tnew;
  uint8_t buf[sizeof(data)];
  uint8_t buflen = sizeof(data);

  if (vw_have_message())  // Is there a packet for us?
  {
    vw_get_message(buf, &buflen);
    memcpy(&data, &buf, buflen);

    String servoString = String(data.servo, 1);
    servoString.toCharArray(servoChar, 10);

    Serial.println("\n      New Data:");
    Serial.print("Servo (degrees): ");
    Serial.println(data.servo);
  }
  
  //Servo A pulse each 20ms
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
