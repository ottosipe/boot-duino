//#include <I2Cdev>
#include <Arduino.h>

void setup() {
  Serial.begin(57600);
}

void loop() {
  delay(1000);
  Serial.write("Hello, Josh \n");
}
