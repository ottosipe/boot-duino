#include "Wire.h"

#include <MPU6050.h>
#include <I2Cdev.h>
#include <helper_3dmath.h>

#include <SdFat.h>
//#include <avr/sleep.h>
//#include <avr/interrupt.h>
ArduinoOutStream cout(Serial);






MPU6050 *imu = new MPU6050; 

const int chipSelect = 5;
SdFat sd;
SdFile myFile;

void setup() {
  Serial.begin(38400);

  //init and wake up the board, this should not have to change, only one way to do it!
  imu->initialize(); 
  imu->setSleepEnabled(false);

  imu->setIntMotionEnabled(true);
  imu->setMotionDetectionDuration(80);

  //set ranges
  imu->setFullScaleAccelRange(2); // corresponds to the scaleAccl value
  imu->setFullScaleGyroRange(2); // corresponds to the scaleGyro value

  imu->setRate(4); // 1khz / (1 + 4) = 200 Hz


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
  int16_t ax,ay,az;
  int16_t gx,gy,gz;

  imu->getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
  cout << ax << ", " << ay << ", " << az << ", " << gx << ", " << gy << ", " << gz << ";" << endl;
  delay(100);

}

