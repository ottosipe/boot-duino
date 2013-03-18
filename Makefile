ARDUINO_DIR   			= /Applications/Arduino.app/Contents/Resources/Java
ARDMK_DIR     			= ./arduino-mk
USER_LIB_PATH 			= ./libraries
ARDUINO_LIBS  			= SdFat Wire I2CDev MPU6050
AVRDUDE_ARD_BAUDRATE 	= 38400
ARDUINO_PORT 			= /dev/tty.FireFly*

#MCU = atmega328p
#F_CPU = 8000000

BOARD_TAG    = pro328

include arduino-mk/Arduino.mk

progam: all
	avrdude -v -p m328p -c avrisp2 -P usb -b57600 -U build-pro328/boot-duino.hex

bootload:
	avrdude -b19200 -P usb -c avrisp2 -p m328p -v -e -U flash:w:bootloader.hex -U lock:w:0x0F:m

fuses: 
	avrdude -b19200 -P usb -c avrisp2 -p m328p -U lfuse:w:0xff:m -U hfuse:w:0xda:m -U efuse:w:0x05:m