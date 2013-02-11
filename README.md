boot-duino
==========

Arduino-based research project at the University of Michigan. We are working on an open-source solution to gait and posture analysis for soldiers in the field.


```
sudo perl -MCPAN -e shell  
	install YAML  
	install Device::SerialPort  
```

`avrdude -v -p atmega328p -c arduino -P /dev/tty.FireFly-C566-SPP -b57600
-U file.hex`

Resources:
 
- [Arduino-Makefile](https://github.com/mjoldfield/Arduino-Makefile)