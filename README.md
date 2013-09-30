boot-duino
==========

Arduino-based research project at the University of Michigan. Focusing on an open-source solution to gait and posture analysis.

## Project Status as of Sept. 30, 2013
- Hardware 3.0 is stable
- Software can collect, save, or transmit IMU data.
- Case is designed and being tested.

## Programming

- Program Upload via BT: `make upload`
- Program Upload via AVRisp mkII: `make program`
- Bootloader: `make bootload`
- Fuses: `make fuses`

----

Bluetooth Settings:  
`ST,255` 	- command timer  
`SU,38` 	- baud rate 38400  
`S~,3` 		- Set Modem Profile   

Fuse Info: [Arduino Fuses](http://www.codingwithcody.com/2011/04/arduino-default-fuse-settings/), [Fuse Calculator](http://www.engbedded.com/fusecalc/)


----

Hardware Notes

- Chip Select Pin : D5

----


Install Arduino-Makefile:  

```
sudo perl -MCPAN -e shell
	install YAML
	install Device::SerialPort
```


## Resources and Libraries:
 
- [I2CDev](https://github.com/jrowberg/i2cdevlib)
- [SDFat](http://code.google.com/p/sdfatlib/downloads/list)
- [Arduino-Makefile](https://github.com/mjoldfield/Arduino-Makefile)
