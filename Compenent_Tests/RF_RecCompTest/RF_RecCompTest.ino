/*
 * Island City Explorers
 * RF Link Receiver Component Test (To Be used with dual AVR system, RF_TransCompTest code and Transmitter Component)
 * V 1.0
 * Release: 2/17/21
 * S. Shaw
 * 
 */

#include <VirtualWire.h>


// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!

const int receive_pin = 13;
char temperatureChar[10];
char humidityChar[10];


struct package
{
  float temperature = 0.0;
  float humidity = 0.0;
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
  int tnew;
  uint8_t buf[sizeof(data)];
  uint8_t buflen = sizeof(data);

  if (vw_have_message())  // Is there a packet for us?
  {
    vw_get_message(buf, &buflen);
    memcpy(&data, &buf, buflen);

    String temperatureString = String(data.temperature, 1);
    temperatureString.toCharArray(temperatureChar, 10);

    String humidityString = String(data.humidity, 1);
    humidityString.toCharArray(humidityChar, 10);

    Serial.println("\n      New Data:");
    Serial.print("Temperature (F): ");
    Serial.println(data.temperature);
    Serial.print("Humidity (%): ");
    Serial.println(data.humidity);
  }

}
