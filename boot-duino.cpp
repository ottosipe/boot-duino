#include <Arduino.h>
#include <SdFat.h>
#include <Wire.h>
#include <I2Cdev.h>


const int chipSelect = 5;
SdFat sd;
SdFile myFile;

void setup() {
  Serial.begin(38400);

  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) sd.initErrorHalt();
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("Opening test.txt failed!");
  }
  
  Serial.print("Writing to test.txt...");
  myFile.println("Hello World!");

  // close the file:
  myFile.close();
  Serial.println("done.");

  // re-open the file for reading:
  if (!myFile.open("test.txt", O_READ)) {
    sd.errorHalt("opening test.txt for read failed");
  }
  Serial.println("test.txt:");

  // read from the file until there's nothing else in it:
  int data;
  while ((data = myFile.read()) > 0) Serial.write(data);

  myFile.close();
}

void loop() {
  // nothing happens after setup
}

