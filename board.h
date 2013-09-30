#ifndef __Board__
#define __Board__

#include "Arduino.h"
#include "Wire.h"

#include <MPU6050_6Axis_MotionApps20.h>

#include <SdFat.h>
//#include <avr/sleep.h>
//#include <avr/interrupt.h>

class Board {
	MPU6050 *imu;
	SdFat sd;
	SdFile file; // todo: switch to ostream

	VectorInt16 accel;
	VectorInt16 gyro;

	// scale to proper units
	double scaleAccl;
	double scaleGyro;

	bool blinkState;
	bool isFirst;

	// MPU control/status vars
	bool dmpReady;  // set true if DMP init was successful
	uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
	uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint16_t fifoCount;     // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64]; // FIFO storage buffer

	// orientation/motion vars
	Quaternion q;           // [w, x, y, z]         quaternion container
	VectorInt16 aa;         // [x, y, z]            accel sensor measurements
	VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
	VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
	VectorFloat gravity;    // [x, y, z]            gravity vector

public: 
	Board(MPU6050 *imu_in);
	~Board();

	void start_imu();
	void wait_for_key();
	void loop();

	void setup_card();
	void close_card();
	void read_card();
};


#endif /* defined(__Board__) */