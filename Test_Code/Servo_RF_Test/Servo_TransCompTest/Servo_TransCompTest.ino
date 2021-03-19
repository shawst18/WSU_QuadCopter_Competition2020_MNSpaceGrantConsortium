/*
 * Island City Explorers
 * RF Link Transmitter Component Test code, to be used with dual AVR system, receiver code RF_RecCompTest & Servo
 * V 1.0
 * Release: 3/12/21
 * M.Pelissero
 * 
 */

#include <VirtualWire.h>

//const int led_pin = 13;
const int transmit_pin = 13;

struct package
{
  float servo ;
 
};

typedef struct package Package;
Package data;

void setup()
{
  // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(500);       // Bits per sec
  //pinMode(led_pin, OUTPUT);
}


void loop()
{
  //digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  for (int n = 0; n<6; n++) {
  data.servo = float(n);
  vw_send((uint8_t *)&data, sizeof(data));
  vw_wait_tx(); // Wait until the whole message is gone
  //digitalWrite(led_pin, LOW);
  delay(1000);
  }
}
