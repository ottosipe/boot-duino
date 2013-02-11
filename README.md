boot-duino
==========

Arduino-based research project at the University of Michigan. Focusing on an open-source solution to gait and posture analysis for soldiers in the field.



To Build and Upload:  
`make upload`

Program Upload (when `make upload` fails):  
`avrdude -v -p atmega328p -c arduino -P /dev/tty.bt-name -b57600
-U file.hex`


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
