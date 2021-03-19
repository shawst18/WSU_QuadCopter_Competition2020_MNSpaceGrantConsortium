/*
   Island City Explorers
   Mapping Firmware File
   V 1.0
   Release: 2/20/21
   S. Shaw & M. Pelissero

   Notes: After testing, it was found that the code cannot be time sensitive (commented out tdel, delay at end of loop).
   Either BME (at low tdel's 500 or less) or RF link (at higher tdel's 1000) would not work.

   3/6/21 BME & RF link removed from code/ sensor suite.  Altimeter gave more accurate reading.  Sd care

*/

#include <VirtualWire.h>
#include <HCSR04.h>
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <SD.h>  //SD card library

const int transmit_pin = 13;

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

#define SDchipSelect BUILTIN_SDCARD   //SD Pin Definition


UltraSonicDistanceSensor distanceSensor(16, 17);  //echo 15, trig 16

//For SD card
File datalog;                      //File Object for datalogging
char filename[] = "DroneB00.csv";   //Template for file name to save data in SD-card
bool SDactive = false;             //Used to check for SD card before attempting to log data

void setup() {
  Serial.begin(9600);
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(500);
  Serial.println("Adafruit_MPL3115A2 test!");

  //SD Card Setup
  pinMode(10, OUTPUT);
  pinMode(SDchipSelect, OUTPUT);
  Serial.print("Initializing SD card...");
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

  //Prints header everytime on startup
  String headerLOG = "Time(ms), Feature height (m), Altitude(m), Ultrasonic (m)";  //Defines "headerLOG" as a string that contains the inscribed text.
  //Serial.println(headerLOG);  //Prints the "headerLOG" to the serial monitor
  if (SDactive) {
    datalog.println(headerLOG);  //Prints the "headerLOG" to the SD-card
    datalog.close();
  }
  delay(500);  //waits 500 ms
}

void loop() {
  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
  }
  // put your main code here, to run repeatedly:
  float ultra = distanceSensor.measureDistanceCm() / 100;
  float altm = baro.getAltitude();
  float featHeight =  altm - ultra;
 

  //vw_send((uint8_t *)&data, sizeof(data));
  //vw_wait_tx(); // Wait until the whole message is gone

  Serial.print(altm); Serial.println(" Alt meters");
  Serial.print(featHeight); Serial.println(" Feat height m");
  Serial.print(ultra); 
  Serial.println(" Ultra m");

  delay(250);

  //Creates the string "dataLOG" and includes the device measurements listed, which is then printed to the serial monitor and the SD-card.
  String dataLOG = String (millis()) + ", " + String(featHeight) + ", " + String(altm) + ", " + String(ultra) ;
  if (SDactive) {
    datalog = SD.open(filename, FILE_WRITE);
    datalog.println(dataLOG);  //Prints the "dataLOG" to the SD-card
    datalog.close();
  }
}
