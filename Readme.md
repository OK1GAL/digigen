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
| ---- | :----: | ---- |
| help | prints help | `help` |
| config | prints current config | `config` |
| txen x | sets the carier tx state | `txen 1` |
| drives x | sets the SI5351 drive strenght 0=2mA 1=4mA 2=6mA 3=8mA | `drives 3` |
| cfreq xxxxxxxxx | sets center frequency | `cfreq 007100000` |
| sfreq xxxxxxxxx | sets frequency deviation | `sfreq 000000125` |
| cwspeed xx | sets CW speed in WPM | `cwspeed 025` |
| baud xxx | sets baudrare | `baud 050` |
| txmode x | sets the tx mode, 0:carier 1:CW 2:RTTY | `txmode 1` |
| mancwtx | starts direct CW transmition | `mancwtx` |
| manrttytx | starts direct RTTY transmision | `manrttytx` |
| savep x | saves current config to location x in EEPROM | `savep 7` |
| loadp x | loads config from preset x | `loadp 7` |
| default | memory formating | `default` |
> [!IMPORTANT]
> To exit any direct transmition use ctrl+c.


<details>

<summary>EEPROM memory map</summary>
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

| value | offset | type | size |
| ---- | ---- | ---- | ---- |
| mode | 0 | uint8 | 1B |
| cfreq | 1 | uint64 | 8B |
| sfreq | 9 | uint64 | 8B |
| baudrate | 17 | uint16 | 2B |
| cw speed | 19 | uint8 | 1B |
| drive strenght | 20 | uint8 | 1B |

</details>

## Preset organisation:<br/>
Each preset is 64 bytes long<br/>
### Memory offsets:<br/>

| value | offset | type | size |
| ---- | ---- | ---- | ---- |
| mode | 0 | uint8 | 1B |
| cfreq | 1 | uint64 | 8B |
| sfreq | 9 | uint64 | 8B |
| baudrate | 17 | uint16 | 2B |
| cw speed | 19 | uint8 | 1B |
| drive strenght | 20 | uint8 | 1B |


## TODO<br/>
- [x] Make it posible o load preset from console (curently posible only from buttons)
- [x] Add posibility to set drive strenght (2mA/4mA/6mA/8mA)
- [ ] Add calibration procedure
- [ ] Add detection of uninitiated memory and automatic initialization
- [ ] Add command to enter custom message for preset
- [ ] Add beacon mode
