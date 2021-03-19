
/*
   Island City Explorers
   Ambient Sensor Firmware File
   V 1.3
   Release: 2/20/21
   S. Shaw

   Version Hitory:
   V 1.1 -> ss - added magentic and temperature probing sensor code
   V 1.2 -> ss - Changed RF data struct after testing, implemented SD card code
   v 1.3 -> ss - Added Ultrasonic sensor
   v 1.4 -> ss - Changed the magnetometer form the HMC5883L to the LSM9DS1 and changed the thermistor to a AMG8833 Thermal Camera

  Component list:
  Teensy 3.5
  BME temp, humid, pres, alt sensor
  Gas Sensor
  Temp Probe
  LSM9DS1 Mag Sensor
  Ultrasonic Sensor
  Spy Cam board
  RF link transmitter
  SD card

*/

// Libraries
#include <VirtualWire.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>  //SD card library
#include <Adafruit_Sensor.h> // extra library for BME280 and Mag sensor
#include <Adafruit_BME280.h> // library for BME280
#include <Arduino_LSM9DS1.h>
#include <HCSR04.h> // library for Ultrasonic
#include <Adafruit_AMG88xx.h> // Library for thermo IR cam

Adafruit_BME280 bme;
Adafruit_AMG88xx amg;

// Pin Definitions
#define transmit_pin 23 // Data pin assign for RF link
#define GasSens_pin 22 // Gas Sensor pin
#define cam_trig 21 // Camera trigger pin
#define SDchipSelect BUILTIN_SDCARD   //SD Pin Definition
#define Pow_pin 13 // Pin for power indication, connects to built in LED light
UltraSonicDistanceSensor distanceSensor(17, 16);  //trig, echo

// Define variables and constants
float temp, pres, hum, gas, ultra, alt; // Define data variable
int code;
String magData;
#define SEALEVELPRESSURE_HPA (1013.25) // pressure @ Sea Level def
float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

//for RF link
struct package
{
  float probe_temp;
  float mag;
};
typedef struct package Package;
Package data;

//For SD card
File datalog;                      //File Object for datalogging
File dataTHERM;                    //File for Therm Cam data
char filename[] = "DroneB00.csv";   //Template for file name to save data in SD-card
bool SDactive = false;             //Used to check for SD card before attempting to log data

//////////////////////////////////////////////////////////////////////////////////



void setup() {
  //Initialize Analog control/input pins
  pinMode(cam_trig, OUTPUT);
  digitalWrite(cam_trig, HIGH);
  pinMode(Pow_pin, OUTPUT); // Set power indicator to output
  pinMode(GasSens_pin, INPUT);

  // Initialize RF link
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(500);       // Bits per sec
  // Serial initialization for troubleshooting
  Serial.begin(9600); // Begin serial communcation between teensy and comp
  //while (!Serial);   // time to get serial running

  // Startup for BME
  bme.begin();
  // Startup for Magnetic Sensor
  IMU.begin();
  // Startup for IR thermo cam
  amg.begin();

  //SD Card Setup
  pinMode(10, OUTPUT);
  pinMode(SDchipSelect, OUTPUT);
  //Serial.print("Initializing SD card...");
  if (!SD.begin(SDchipSelect)) {                             //Attempt to start SD communication
    //Serial.println("Card not inserted or the card is broken.");          //Print out error if failed; remind user to check card
  }
  else {                                                    //If successful, attempt to create file
    //Serial.println("Card initialized successfully.\nCreating File...");
    for (byte i = 0; i < 100; i++) {                        //Can create up to 100 files with similar names, but numbered differently
      filename[6] = '0' + i / 10;
      filename[7] = '0' + i % 10;
      if (!SD.exists(filename)) {                           //If a given filename doesn't exist, it's available
        datalog = SD.open(filename, FILE_WRITE);            //Create file with that name
        SDactive = true;                                    //Activate SD logging since file creation was successful
        //Serial.println("Logging to: " + String(filename));  //Tell user which file contains the data for this run of the program
        break;                                              //Exit the for loop now that we have a file
      }
    }
    if (!SDactive) {
      //Serial.println("No available file names; clear SD card to enable logging");
    }
  }

  //Prints header everytime on startup
  String headerLOG = "Time(ms), Alt(m), Temp(C), Pres (atm), Humidity(%), Magnetic Field (uT), relative gas reading (V), Ultrasonic Reading (m), Thermal Cam middle pixel reading (C)";  //Defines "headerLOG" as a string that contains the inscribed text.
  //Serial.println(headerLOG);  //Prints the "headerLOG" to the serial monitor
  if (SDactive) {
    datalog.println(headerLOG);  //Prints the "headerLOG" to the SD-card
    datalog.close();
  }
  delay(100); // Allow time for everything to boot up

}

//////////////////////////////////////////////////////////////////////////////////



void loop() {


  // Take Picture
  digitalWrite(cam_trig, LOW);
  delay(200);
  digitalWrite(cam_trig, HIGH);

  // Code for Power Indication, blinks every sampling of data iteration
  digitalWrite(Pow_pin, HIGH);
  delay(50);
  digitalWrite(Pow_pin, LOW);

  // Update Environmental Data from BME
  temp = bme.readTemperature();
  pres = bme.readPressure();
  hum = bme.readHumidity();
  alt = bme.readAltitude(SEALEVELPRESSURE_HPA);

  // Read in pixels from IR thermo cam
  amg.readPixels(pixels);

  // Read in voltage on gas sensor
  gas = analogRead(GasSens_pin) * (3.3 / 1023.0);

  //Read in distance below drone from Ultrasonic sensor
  ultra = distanceSensor.measureDistanceCm() / 100;

  //Read in magnetic data, find magnitude of magnetic field vector
  if (IMU.magneticFieldAvailable()) {
    float x, y, z;
    IMU.readMagneticField(x, y, z);
    data.mag = sqrt(x * x + y * y + z * z);
  }

  data.probe_temp = pixels[32];

  //Send temp and magnetic data over RF link
  vw_send((uint8_t *)&data, sizeof(data));
  vw_wait_tx(); // Wait until the whole message is gone
  Serial.println("hello");
  //Save data to SD card
  String dataLOG = String(millis()) + ", " + String(alt) + ", " + String(temp) + ", " + String(pres) + ", " + String(hum) + ", " + String(data.mag) + ", " + String(gas) + ", " + String(ultra) + "," + String(pixels[1]);
  if (SDactive) {
    datalog = SD.open(filename, FILE_WRITE);
    datalog.println(dataLOG);  //Prints the "dataLOG" to the SD-card
    datalog.close();
  }
  /*  if (SDactive) {
      dataTHERM = SD.open(filename, FILE_WRITE);
      for (int i = 63; i >= 0; i--) {
        dataTHERM.print(pixels[i]);
        if (i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56) {
          dataTHERM.println();
        }
        else {
          dataTHERM.print(',');
        }
      }
      dataTHERM.println();
      dataTHERM.close();
    }*/
  /*//Code for troubleshooting sensors
  Serial.println(millis());
  Serial.println(temp);
  Serial.println(pres);
  Serial.println(hum);
  Serial.println(data.mag);
  Serial.println(gas);
  Serial.println(ultra);
  Serial.print("[");
  for (int i = 1; i <= AMG88xx_PIXEL_ARRAY_SIZE; i++) {
    Serial.print(pixels[i - 1]);
    Serial.print(", ");
    if ( i % 8 == 0 ) Serial.println();
  }
  Serial.println("]");
  Serial.println();
  delay(1000);*/
}
