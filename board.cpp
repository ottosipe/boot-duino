
#include "board.h"

ArduinoOutStream cout(Serial);

const int chipSelect = 5;
#define LED_PIN 13

#define SD_WRITE

volatile bool mpuInterrupt = false;     
// indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

Board::Board(MPU6050 *imu_in) : imu(imu_in) {  


    pinMode(chipSelect, OUTPUT);
	//scales from datasheet don't change
	scaleAccl = 417.53; //4096 / 9.81 - already shifted for m/s^2 units
	scaleGyro = 32.8; // degrees per second

	dmpReady = false;
	blinkState = false;

	isFirst = true;
};

void Board::start_imu() {
	//init and wake up the board, this should not have to change, only one way to do it!
	imu->initialize();
	int devStatus = imu->dmpInitialize();

	if (devStatus == 0) {
        imu->setDMPEnabled(true);

        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = imu->getIntStatus();

        dmpReady = true;
        packetSize = imu->dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

	//set ranges
	imu->setFullScaleAccelRange(2); // corresponds to the scaleAccl value
	imu->setFullScaleGyroRange(2); // corresponds to the scaleGyro value

	imu->setRate(4); // 1khz / (1 + 4) = 200 Hz

	#ifdef SD_WRITE
	setup_card();
    file.println("--------------------------");
	#endif
};

Board::~Board() {
	#ifdef SD_WRITE
	close_card();
	#endif
}

void Board::loop() {

	if (!dmpReady) {
		Serial.println("Error, DMP not ready.");
		return;
	}

	// reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = imu->getIntStatus();

    // get current FIFO count
    fifoCount = imu->getFIFOCount();

	if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        imu->resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = imu->getFIFOCount();

        // read a packet from FIFO
        imu->getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        /*if(isFirst) {
			imu->dmpGetQuaternion(&q, fifoBuffer);
        	imu->dmpGetGravity(&gravity, &q);
        	isFirst = false;
        }*/

        // display initial world-frame acceleration, adjusted to remove gravity
        // and rotated based on known orientation from quaternion
        imu->dmpGetQuaternion(&q, fifoBuffer);
        imu->dmpGetAccel(&aa, fifoBuffer);
        imu->dmpGetGravity(&gravity, &q);
        imu->dmpGetLinearAccel(&aaReal, &aa, &gravity);
        imu->dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
        cout << aa.x / scaleAccl << "," 
        	<< aa.y / scaleAccl << "," 
        	<< aa.z / scaleAccl << ",";

        cout << aaWorld.x / scaleAccl << "," 
            << aaWorld.y / scaleAccl << "," 
            << aaWorld.z / scaleAccl << ";" << endl;
    
        #ifdef SD_WRITE
        file.print(aa.x / scaleAccl);
        file.print(",");
        file.print(aa.y / scaleAccl);
        file.print(",");
        file.print(aa.z / scaleAccl);
        file.print(",");

        file.print(aaWorld.x / scaleAccl);
        file.print(",");
        file.print(aaWorld.y / scaleAccl);
        file.print(",");
        file.print(aaWorld.z / scaleAccl);
        file.println(";");
        #endif

        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }

   	if (Serial.available() && 'x' == Serial.read()) {
		// go to "sleep"
		wait_for_key();	
	}

}

void Board::wait_for_key() {

	Serial.println("Waiting for key ... ");
	while (Serial.available() && Serial.read()); // empty buffer
	while (!Serial.available());                 // wait for data
	while (Serial.available() && Serial.read()); // empty buffer again

}

void Board::setup_card() {

	if (!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();
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