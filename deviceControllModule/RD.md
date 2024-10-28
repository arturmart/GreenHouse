Readme yani

|Command                        | bit        | Description                  | Implementation                              | devCtrModule instruction 
| ----------------------------- | ---------- | ---------------------------- | ------------------------------------------- | --------------------------- |
| `ERROR_NONE`                  | 0          | None ERROR                   | only for declaration 1,2,3 Level feedbeck   | -
| `ERROR_SYNTAX`                | 1 << 0     | ERROR Syntex                 | **NO**                                      |
| `ERROR_1L_NO_DATA`            | 1 << 1     | ERROR no data for 1 Level    | **1L**  delimiters1L[0] == 0                |
| `ERROR_1L_TOO_MANY_DATA`      | 1 << 2     | ERROR data for 1 Level > 2   | **1L**  delimiters1L[0] > 1                 |
| `ERROR_INVALID_CRC`           | 1 << 3     | ERROR Wrong CRC              | Wrong **CRC**                               |
| `ERROR_NULL_CRC`              | 1 << 4     | ERROR NULL CRC               | Null **CRC**                                |
| `ERROR_2L_NO_DATA_PACKETS`    | 1 << 5     | ERROR no data for 2 Level    | **2L**  delimiters2L[0] == 0                |
| `ERROR_2L_TOO_MANY_PACKETS`   | 1 << 6     | ERROR data for 2 Level > 2   | **2L**  delimiters2L[0] > 7                 |
| `ERROR_3L_WRONG_DATA_PACKETS` | 1 << 7     | ERROR Wrong data for 3 Level | **3L**  delimiters3L[0] != 2 for ROW        |
| `PACKETS_COUNT`               | 1111 << 8  | Packets Count on 4 bits      | return **Commands** Count                   |
| `GET_KEYWORD`                 | 1 << 12    | handle KeyWord               | if 1L 1 part is **KeyWord**                 |













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
