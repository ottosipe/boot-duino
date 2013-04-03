
#include "board.h"

const int chipSelect = 5;

ArduinoOutStream cout(Serial);

Board::Board(MPU6050 *imu_in) : imu(imu_in) {  
	//scales from datasheet don't change
	scaleAccl = 417.53; //4096 / 9.81 - already shifted for m/s^2 units
	scaleGyro = 32.8; // degrees per second

};

void Board::start_imu() {
	//init and wake up the board, this should not have to change, only one way to do it!
	imu->initialize(); 
	cout << "Initializing I2C devices..." << endl;

	imu->setSleepEnabled(false);

	imu->setIntMotionEnabled(true);
	imu->setMotionDetectionDuration(80);

	//set ranges
	imu->setFullScaleAccelRange(2); // corresponds to the scaleAccl value
	imu->setFullScaleGyroRange(2); // corresponds to the scaleGyro value

	imu->setRate(4); // 1khz / (1 + 4) = 200 Hz

	// verify connection
	cout << "Testing device connections..." << endl;
	cout << (imu->testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed") << endl;


	setup_card();
};

Board::~Board() {
	close_card();
}

void Board::loop() {
	int16_t ax,ay,az;
	int16_t gx,gy,gz;

	imu->getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
	cout 	<< ax << ", " << ay << ", " << az << ", " 
			<< gx << ", " << gy << ", " << gz << ";" << endl;

	if (Serial.available() && 'x' == Serial.read()) {
		// go to "sleep"
		wait_for_key();	
	}

	delay(50);
}

void Board::wait_for_key() {

	Serial.println("Waiting for key ... ");
	while (Serial.available() && Serial.read()); // empty buffer
	while (!Serial.available());                 // wait for data
	while (Serial.available() && Serial.read()); // empty buffer again

}

void Board::setup_card() {

	if (!sd.begin(chipSelect, SPI_FULL_SPEED)) sd.initErrorHalt();
	if (!file.open("data.txt", O_RDWR | O_CREAT | O_AT_END)) {
		sd.errorHalt("Opening data.txt failed!");
	}

}

void Board::close_card() {
	file.close();
}

void Board::read_card() {

	Serial.println("data.txt:");
	// read from the file until there's nothing else in it:
	int data;
	while ((data = file.read()) > 0) Serial.write(data);

}