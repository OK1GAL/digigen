# Digigen<br/>

Simple digital modes generator for RPI Pico<br/>
Originaly made for testing RTTY on R-154-2(M) receiver<br/>
Currently supports CW, RTTY and hellschreiber<br/>

| Mode | Mode ID | LEDs |
| :----: | :----: | :----: |
| Carier | 0 | 00 |
| CW | 1 | 01 |
| RTTY | 2 | 10 |
| HELL | 3 | 11 |

<br/>

![Image of the PCB.](/images/PCB_render.png)

<details>

<summary>HW connections</summary>
Both EEPROM and SI5351 share the same bus.<br/>
SDA: 4<br/>
DCL: 5<br/>
ERROR LED: 2<br/>
TTY_TX_PIN: 8<br/>
TTY_RX_PIN: 9<br/>
MODE0_LED_PIN: 10<br/>
MODE1_LED_PIN: 11<br/>
RUNNING_LED_PIN: 13<br/>
SPACE_LED_PIN: 15<br/>
MARK_LED_PIN: 14<br/>
PROG1_LED_PIN: 16<br/>
PROG2_LED_PIN: 17<br/>
PROG3_LED_PIN: 18<br/>
PROGRAM_BTN_PIN: 20<br/>
TEST_BTN_PIN: 21<br/>
RUN_FROM_EEPROM_BTN_PIN: 22<br/>
</details>

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
| baud xxx | sets baudrate | `baud 050` |
| settext | sets custom tx message | `settext` |
| txtext | transmits custom tx message with current mode | `txtext` |
| txmode x | sets the tx mode, 0:carier 1:CW 2:RTTY  3:HELL | `txmode 1` |
| mancwtx | starts direct CW transmition | `mancwtx` |
| manrttytx | starts direct RTTY transmision | `manrttytx` |
| savep x | saves current config to location x in EEPROM | `savep 7` |
| loadp x | loads config from preset x | `loadp 7` |
| txtext | transmits saved message | `txtext` |
| default | memory formating | `default` |
| hellspeed xxx | sets the hell baudrate | `hellspeed 125` |
| manhelltx | starts direct HELL transmision | `manhelltx` |

> [!IMPORTANT]
> To exit any direct transmition use ctrl+c.

> [!TIP]
> You can run preset from memory using "RUN EEPROM" button after choosing the preset using "CYCLE EEPROM".

> [!WARNING]
> Hellschreiber was not tested yet, so it might be a bit off.


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
<br/>
Preset organisation:<br/>
Each preset is 64 bytes long<br/>

| value | offset | type | size |
| ---- | ---- | ---- | ---- |
| mode | 0 | uint8 | 1B |
| cfreq | 1 | uint64 | 8B |
| sfreq | 9 | uint64 | 8B |
| baudrate | 17 | uint16 | 2B |
| cw speed | 19 | uint8 | 1B |
| drive strenght | 20 | uint8 | 1B |
| custom text length | 31 | uint8 | 1B |
| custom text | 32 | char array | 32B |

</details>

## TODO<br/>

- [x] Make it posible o load preset from console (curently posible only from buttons)
- [x] Add posibility to set drive strenght (2mA/4mA/6mA/8mA)
- [ ] Add calibration procedure
- [ ] Add detection of uninitiated memory and automatic initialization
- [x] Add command to enter custom message for preset
- [ ] Add beacon mode
- [x] Add Hellschreiber
- [ ] Complete Hellschreiber LUT
- [x] Add suport for TTY/HELL interface. (3.5mm jack)
- [ ] Optimize memory usage. (no need for 64bit uint for frequency etc.)
