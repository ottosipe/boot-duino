// use verbose logging
//#define DEBUG
#define SD_WRITE

#include "board.h"

MPU6050 imu; 
Board board(&imu);;

void setup() {
  
	Serial.begin(38400);
	board.start_imu();
	// wait to start the read
	//board.wait_for_key();

}

void loop() {
	// start the fun
	board.loop();
}

