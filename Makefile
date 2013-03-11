ARDUINO_DIR   = /Applications/Arduino.app/Contents/Resources/Java
ARDMK_DIR     = ./arduino-mk

AVRDUDE_ARD_BAUDRATE = 57600
ARDUINO_PORT = /dev/tty.FireFly*

#MCU = atmega328p
#F_CPU = 8000000

BOARD_TAG    = pro328

include arduino-mk/Arduino.mk


install:
	avrdude -v -p atmega328p -c arduino -P $(ARDUINO_PORT) -b57600 -U build-pro328/boot-duino.hex

