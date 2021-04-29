#include <SD.h> //Load SD card library
#include <SPI.h> //Load SPI Library


#include "Wire.h"    // imports the wire library for talking over I2C 
#include "Adafruit_BMP085.h"  // import the Pressure Sensor Library We are using Version one of Adafruit API for this sensor

Adafruit_BMP085 Sensor;  // create sensor object called mySensor

// Ok everyone so this is the base that we will have for our data output to sd card coding.
float tempC;  // Variable for holding temp in C
float pressure; //Variable for holding pressure reading
float altitude; //Variable Fo altitude
float sealevelpressure;
float al;

const int greenPin = 7; //this is here for us to activate our pin warning system.
const int redPin = 8; 

int chipSelect = 4; //chipSelect pin for the SD card Reader
File SensorData; //Data object you will write your sesnor data to

void setup(){
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  Serial.begin(9600); //turn on serial monitor
  Sensor.begin();   //initialize pressure sensor mySensor
  if (!SD.begin(4)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
    
  }
   Serial.println("card initialized.");
   PrintHeader();
  

  
}

void PrintHeader() // this function prints header to SD card
{
   File dataFile = SD.open("home.txt", FILE_WRITE);
   if (dataFile)
   {
      String header = "Temperature (C), Pressure (Pa), Real Altitude (m), Altitude (m), Sea Level Pressure (m)";
      dataFile.println(header);
      dataFile.close();
      Serial.println("Header Printed");
   }
   else
   {
     Serial.println("ERROR: Datafile or SD card unavailable");
   }
}

void loop() {

  if (!Sensor.begin()) 
  {
        digitalWrite(redPin, HIGH); // turns on red warning LED
        digitalWrite(greenPin, LOW); // turns off green LED
	Serial.println("DANGER! DANGER! Check wiring, Check code, Check well... I don't know just look busy");
  }
    else 
  {
  (Sensor.begin());
  SensorData = SD.open("Ein_Data.txt", FILE_WRITE);
  
  if (!SensorData) 
  {
  
        digitalWrite(redPin, HIGH); // turns on red warning LED
        digitalWrite(greenPin, LOW); // turns off green LED
	Serial.println("Check SD Card, Then Press Reset");
        delay(2000);
  }
  
  else 
  {
  (SensorData); 
  digitalWrite(greenPin, HIGH); // turns on the green pin
  digitalWrite(redPin, LOW); // turns off red warning LED
  tempC = Sensor.readTemperature(); //Read Temperature from BMP180
  pressure = Sensor.readPressure(); //Read Pressure
  altitude = Sensor.readAltitude(101820); //Altitude
  al = Sensor.readAltitude();
  sealevelpressure = Sensor.readSealevelPressure();
  Serial.print("The Temp is: "); //Print Your results
  Serial.print(tempC);
  Serial.println(" degrees C");
  Serial.print("The Pressure is: ");
  Serial.print(pressure);
  Serial.println(" Pa.");
  Serial.print("The Real Altitude is: ");
  Serial.print(altitude);
  Serial.println(" meters");
  Serial.print("The Altitude is: ");
  Serial.print(al);
  Serial.println("Meters");
  Serial.print("The Sea Level Pressure is: ");
  Serial.print(sealevelpressure);
  Serial.println(" meters (calculated)");
  Serial.println("");

delay(500); //for the test I have it at a 500 ms delay for our actual launch I believe a higher delay would be more ideal, and save battery power.


SensorData.print(tempC);                             //write temperature data to card
SensorData.print(",");                               //write a commma
SensorData.print(pressure);
SensorData.print(",");
SensorData.print(altitude);                        //write pressure and end the line (println)
SensorData.print(",");
SensorData.println(sealevelpressure);
SensorData.close();                                  //close the file
}
}

}

 
  
