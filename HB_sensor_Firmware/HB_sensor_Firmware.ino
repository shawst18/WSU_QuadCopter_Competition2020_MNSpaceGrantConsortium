/*
   Island City Explorers
   Home Base Ambient Sensor Data Receiver Firmware
   V 1.1
   Release: 2/20/21
   S. Shaw

   Version History:
   V 1.1 -> added magentic and temperature probing sensor code

*/

#include <VirtualWire.h>

const int receive_pin = 8;
char probe_tempChar[10];
char magChar[10];

struct package
{
  float probe_temp = 0.0;
  float mag = 0.0;
};

typedef struct package Package;
Package data;

///////////////////////////////////////////////////////////////////////////////////

void setup()
{
  delay(1000);
  Serial.begin(9600);
  // Initialise the IO and ISR
  vw_set_rx_pin(receive_pin);
  vw_setup(500);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
}

///////////////////////////////////////////////////////////////////////////////////

void loop()
{
  int tnew;
  uint8_t buf[sizeof(data)];
  uint8_t buflen = sizeof(data);

  if (vw_have_message())  // Is there a packet for us?
  {
    vw_get_message(buf, &buflen);
    memcpy(&data, &buf, buflen);

    String probe_tempString = String(data.probe_temp, 1);
    probe_tempString.toCharArray(probe_tempChar, 10);

    String magString = String(data.mag, 1);
    magString.toCharArray(magChar, 10);

    Serial.println("\n      New Data:");
    Serial.print("Temperature Probe reading (F): ");
    Serial.println(data.probe_temp);
    Serial.print("Magnetic Reading (uT): ");
    Serial.println(data.mag);
  }
}
