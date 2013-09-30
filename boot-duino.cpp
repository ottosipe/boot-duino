// use verbose logging
//#define DEBUG

#include "board.h"

MPU6050 imu; 
Board board(&imu);;

void setup() {
	Serial.begin(38400);

	// wait to start the read
	board.wait_for_key();
	board.start_imu();
}

void loop() {
	// start the fun
	board.loop();
}

