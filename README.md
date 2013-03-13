boot-duino
==========

Arduino-based research project at the University of Michigan. Focusing on an open-source solution to gait and posture analysis for soldiers in the field.

Fuses: [arduino fuses](http://www.codingwithcody.com/2011/04/arduino-default-fuse-settings/) [fuse calc](http://www.engbedded.com/fusecalc/)
`avrdude -b19200 -P usb -c avrisp2 -p m328p -U lfuse:w:0xff:m -U hfuse:w:0xda:m -U efuse:w:0x05:m`

Bootloader:
`avrdude -b19200 -P usb -c avrisp2 -p m328p -v -e -U flash:w:bootloader.hex -U lock:w:0x0F:m`

To Build and Upload:  
`make upload`

Program Upload via BT (when `make upload` fails):
`avrdude -v -p atmega328p -c arduino -P /dev/tty.FireFly* -b57600 -U build-pro328/boot-duino.hex`

Program Upload via AVRisp mkII:
`avrdude -v -p m328p -c avrisp2 -P usb -b57600 -U build-pro328/boot-duino.hex`

Bluetooth Settings:
`ST,255` 	- command timer  
`SU,57` 	- baud rate 57000  
`S~,3` 		- Set Modem Profile   

----

Resources and Libraries:
 
- [I2CDev](https://github.com/jrowberg/i2cdevlib)
- [SDFat](http://code.google.com/p/sdfatlib/downloads/list)
- [Arduino-Makefile](https://github.com/mjoldfield/Arduino-Makefile)

Install Arduino-Makefile:  

```
sudo perl -MCPAN -e shell
	install YAML
	install Device::SerialPort
```
