# Arduino-CPU-Meter
Host and device code for external hardware CPU meter based on Arduino.

Linux GCC Makefile and Windows MSVC project files are provided.
Specify your serial port when launching program:
    - Windows default: **COM7**
    - Linux default: **/dev/ttyS7**

Connect a 5V voltmeter to Arduino between **Pin9** and **GND**.
See [Arduino.ino](Arduino.cpp) file for Arduino code.

<img src="img/CPU_METER.jpg" width="500">
