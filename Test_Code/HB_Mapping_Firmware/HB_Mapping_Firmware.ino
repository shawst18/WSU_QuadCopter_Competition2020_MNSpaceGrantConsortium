/*
   Island City Explorers
   Home Base Mapping Data Receiver Firmware
   V 1.0
   Release: 2/20/21
   S. Shaw

*/

#include <VirtualWire.h>

const int receive_pin = 13;
char fhChar[10];

struct package
{
  float fh = 0.0;
};


typedef struct package Package;
Package data;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  // Initialise the IO and ISR
  vw_set_rx_pin(receive_pin);
  vw_setup(500);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
  uint8_t buf[sizeof(data)];
  uint8_t buflen = sizeof(data);

  if (vw_have_message())  // Is there a packet for us?
  {
    vw_get_message(buf, &buflen);
    memcpy(&data, &buf, buflen);

    String fhString = String(data.fh, 1);
    fhString.toCharArray(fhChar, 10);
    Serial.println("\n      New Data:");
    Serial.print("feat Height: ");
    Serial.print(data.fh);
  }

}
