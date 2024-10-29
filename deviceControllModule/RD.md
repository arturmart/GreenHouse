Readme yani

### feedback bit - KXXXX87654321

|ID|Command                        | bit        | Description                    | Implementation                              | devCtrModule instruction        | MainCntrl |
|--| ----------------------------- | ---------- | ------------------------------ | ------------------------------------------- | ------------------------------- |-----------|
|- | `ERROR_NONE`                  | 0          | None ERROR                     | only for declaration 1,2,3 Level feedbeck   | -                               | -
|1 | `ERROR_SYNTAX`                | 1 << 0     | ERROR Syntex                   | **NO**                                      | -                               | -
|2 | `ERROR_1L_NO_DATA`            | 1 << 1     | ERROR only 1 data for 1 Level  | **1L**  delimiters1L[0] == 0                | Don't Processing, Send again    | Send again
|3 | `ERROR_1L_TOO_MANY_DATA`      | 1 << 2     | ERROR data for 1 Level > 2     | **1L**  delimiters1L[0] > 1                 | Don't Processing, Send again    | Send again
|4 | `ERROR_INVALID_CRC`           | 1 << 3     | ERROR Wrong CRC                | Wrong **CRC**                               | Don't Processing, Send again    | Send again
|5 | `ERROR_NULL_CRC`              | 1 << 4     | ERROR NULL CRC                 | Null **CRC**                                | Don't Processing, Send again    | Send again
|6 | `ERROR_2L_NO_DATA_PACKETS`    | 1 << 5     | **Warning** only 1 data 2 Level| **2L**  delimiters2L[0] == 0                | Do                              | OK
|7 | `ERROR_2L_TOO_MANY_PACKETS`   | 1 << 6     | ERROR data for 2 Level > 8     | **2L**  delimiters2L[0] > 7                 | **Do size=8, Send next?**       | Send next?
|8 | `ERROR_3L_WRONG_DATA_PACKETS` | 1 << 7     | ERROR Wrong data for 3 Level   | **3L**  delimiters3L[0] != 2 for ROW        | Don't Processing, Send again    | Send again
|X | `PACKETS_COUNT`               | 1111 << 8  | Packets Count on 4 bits        | return **Commands** Count                   | size for loop                   | -
|K | `GET_KEYWORD`                 | 1 << 12    | handle KeyWord                 | if 1L 1 part is **KeyWord**                 | Don't Processing, do **KeyWord**| -

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
**Sucess** for devCtrModule - EXXXX00E0000 
>E - Eny
>
>XXXX is 0 < toInt() < 8
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
**Sucess** for MainCntrl - EXXXX00E0000 
>E - Eny
>
>XXXX is 0 < toInt() < 8
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## SUCSESS devCtrModule
| sended data example (M -> D)       |               |
| ---------------------------------- | ------------- |
|showall/8d984889                    | 1000000000000
|83,0,1;83,1,1;83,2,1;83,3,1/61204cfc| 0010000000000
|83,0,0;83,1,0;83,2,0;83,3,0/745ec3dc| 0010000000000
|83,0,1;83,1,0;83,2,0;83,3,0/6376a71c| 0010000000000
|83,3,1/1b4cc6ee                     | 0000100100000


## ERROR devCtrModule

|ID| sended data example (M -> D)         | feedbeck          |                       |
|--| ------------------------------------ | ----------------- | ----------------      |
|2 |83,0,1;83,1,0;83,2,0;83,3,0asfsafas   | 00100000110**1**0 | `ERROR_1L_NO_DATA`
|3 |83,0,1;83,1,1;83,2,1;83,3,1/61204cfc/a| 0010000000**1**00 | `ERROR_1L_TOO_MANY_DATA`
|4 |83,0,1;83,1,1;83,2,1;83,3,1/00000000  | 001000000**1**000 | `ERROR_INVALID_CRC`
|5 |83,0,1;83,1,1;83,2,1;83,3,1/          | 00100000**1**1000 | `ERROR_NULL_CRC`
|6 |83,3,1/1b4cc6ee                       | 0000100**1**00000 | `ERROR_2L_NO_DATA_PACKETS` (No ERROR)
|7 |1;2;3;4;5;6;7;8;9;10/1b4cc6ee         | 000011**1**001000 | `ERROR_2L_TOO_MANY_PACKETS`
|8 |1,2,3,4/1b4cc6ee                      | 0000**1**00101000 | `ERROR_3L_WRONG_DATA_PACKETS`
|X |83,0,1;83,1,1;83,2,1;83,3,1/61204cfc  | 0**0100**00000000 | `PACKETS_COUNT` (No ERROR)
|K |1,2,3,4/1b4cc6ee                      | **1**000000000000 | `GET_KEYWORD` (No ERROR)

--------------------------------------------------------------------------------------------------------

##Parsing

1 Level - `Data` / `CRC`

2 Level (on Data) - `Packet1` ; `Packet2` ; `...` ; `Packet8` (Max is 8)

3 Level (on Packet) - `Part1` , `Part2` , `Part3` (Equal 3)











--------------------------------------------------------------------------------------------------------
https://www.youtube.com/watch?v=ZvSnwyKa2C4

RS 485

MAX485

DI - Digital Transmiter Sensor - TX (D6)
RO - Digital Reciver Sensor - RX (D7)
DE - Work Resalution Transmiter - digital port (D13)
RE - Work Resalution Reciver - digital port (D13)

A - Liner Defferencial Data
B - Invert Defferencial Data

VCC - 5V
Gnd - Gnd
--------------------------------------------------------------------------------------------------------
https://www.youtube.com/watch?v=Ul03Vkg9A40
https://www.youtube.com/watch?v=1VgePUaF7R8&t=456s
https://github.com/GyverLibs/GParser?tab=readme-ov-file

https://github.com/GyverLibs/StringUtils                     //parsing for Arduino by AlexGyver

http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
https://crc32.online/
https://www.rapidtables.com/convert/number/hex-to-ascii.html

https://github.com/arduino-libraries/Arduino_CRC32             // CRC for Arduino by Alexander Entinger


R,1,1;R,4,0/1667261463
R,1,1;R,4,1/342316161

--------------------------------------------------------------------------------------------------------

https://github.com/arduino-libraries/Arduino_AVRSTL/tree/main/src            //Arduino-ի գրադարանը
https://www.youtube.com/watch?v=1VgePUaF7R8                                  //AlexGyver-ի վիդեո
https://www.nongnu.org/avr-libc/user-manual/group__avr__string.html          //cString հրամաները
https://alexgyver.ru/lessons/cstring/                                        //cString հրամաները AlexGyver
https://www.youtube.com/playlist?list=PLBzxnGCN6T8f9oT4VAgCf341qIuD1LUGY
https://www.youtube.com/playlist?list=PLBzxnGCN6T8cpuX54Uav5In5bODBmSwbs

https://docs.arduino.cc/learn/contributions/arduino-creating-library-guide/



--------------------------------------------------------------------------------------------------------
arduino to Orange pi

https://www.researchgate.net/figure/Pattern-selection-from-recorded-sample-intervals_fig2_350297410
