/*
   Island City Explorers
   Code for the ambient sensors sensor suite (BME280 Barometric Pressure, Relative Humidity, & Temperature and a Gas Sensor)
   S. Shaw
   Release: 2/17/21
   V 1.1
   Version History:
   V 1.0 => Primary libraries, BME interfacing and serial printing
   V 1.1 => added gas sensor reading code, fixed up data stype storage for other sensory parameters

*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h> // extra library for BME280
#include <Adafruit_BME280.h> // library for BME280
#include <SD.h>  //SD card library

#define SEALEVELPRESSURE_HPA (1013.25) // pressure @ Sea Level def
#define SDchipSelect BUILTIN_SDCARD   //SD Pin Definition

float temp, pres, alt, hum, gas; // Define data variable

Adafruit_BME280 bme;

int tdel = 1000; //defines time delays between taking ambient measurements

//For SD card
File datalog;                      //File Object for datalogging
char filename[] = "DroneAmbient.csv";   //Template for file name to save data in SD-card
bool SDactive = false;             //Used to check for SD card before attempting to log data


void setup() {

  Serial.begin(9600); // Begin serial communcation between teensy and comp
  while (!Serial);   // time to get serial running

  unsigned status;

  // Startup for BME
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    while (1) delay(10);
  }
  pinMode(SDchipSelect, OUTPUT);
  if (!SD.begin(SDchipSelect)) {                             //Attempt to start SD communication
    Serial.println("Card not inserted or the card is broken.");          //Print out error if failed; remind user to check card
  }
  else {                                                    //If successful, attempt to create file
    Serial.println("Card initialized successfully.\nCreating File...");
    for (byte i = 0; i < 100; i++) {                        //Can create up to 100 files with similar names, but numbered differently
      filename[6] = '0' + i / 10;
      filename[7] = '0' + i % 10;
      if (!SD.exists(filename)) {                           //If a given filename doesn't exist, it's available
        datalog = SD.open(filename, FILE_WRITE);            //Create file with that name
        SDactive = true;                                    //Activate SD logging since file creation was successful
        Serial.println("Logging to: " + String(filename));  //Tell user which file contains the data for this run of the program
        break;                                              //Exit the for loop now that we have a file
      }
    }
    if (!SDactive) {
      Serial.println("No available file names; clear SD card to enable logging");
    }
  }
}

void loop() {

  temp = bme.readTemperature();
  pres = bme.readPressure();
  alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  hum = bme.readHumidity();
  gas = (analogRead(1) / 1023.0) * 3.3; //Read Gas value from analog 1
  
  //Creates the string "dataLOG" and includes the device measurements listed, which is then printed to the serial monitor and the SD-card.
  String dataLOG = String(millis()) + ", " + String(temp) + ", " + String(pres) + ", " + String(alt) + ", " + String(hum) + ", " + String(gas);
  if (SDactive) {
    datalog = SD.open(filename, FILE_WRITE);
    datalog.println(dataLOG);  //Prints the "dataLOG" to the SD-card
    datalog.close();
  }
  
  Serial.print("Gas = ");
  Serial.println(gas, DEC); //Print the value to serial port

  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");

  Serial.print("Pressure = ");

  Serial.print(pres / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(alt);
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.println();

  delay(tdel);
}
