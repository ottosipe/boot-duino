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
	SdFile file;

	VectorInt16 accel;
	VectorInt16 gyro;

	// scale to proper units
	double scaleAccl;
	double scaleGyro;

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