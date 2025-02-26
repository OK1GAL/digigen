# Digigen<br/>
Simple digital modes generator for RPI Pico<br/>
Originaly made for testing RTTY on R-154-2(M) receiver<br/>
Currently supports CW and RTTY<br/>
<br/>
## HW connections:<br/>
Both EEPROM and SI5351 share the same bus.<br/>
SDA: 4<br/>
DCL: 5<br/>
## Commands:

| Command | description | example syntax |
| ---- | ---- | ---- |
| help | prints help | help |
| config | prints current config | config |
| baud xxx | sets paudrare | baud 050 |
| cfreq xxxxxxxxx | sets center frequency | cfreq 007100000|

<br/>
## EEPROM memory map:<br/>
Each preset is 64 bytes long (20 bytes used)<br/>
|mode 1B|cfreq 8B|sfreq 8B|baudrate 2B|CW_speed 1B|<br/>
0x0000-0x003f last config<br/>
0x0040-0x007f preset 0<br/>
0x0080-0x00bf preset 1<br/>
0x00c0-0x00ff preset 2<br/>
0x0100-0x013f preset 3<br/>
0x0140-0x017f preset 4<br/>
0x0180-0x01bf preset 5<br/>
0x01c0-0x01ff preset 6<br/>
0x0200-0x023f preset 7<br/>
0x0240-0x027f Calibration data<br/>