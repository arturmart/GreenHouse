# GreenHose

![CompositJSONpng](https://github.com/user-attachments/assets/c590008d-2f42-499d-9e2c-39f2f6f5866a)

![GHK](https://github.com/user-attachments/assets/0f8482ab-1b7b-4ad1-9608-adaa316a2270)


![scheme](https://github.com/user-attachments/assets/f6e4ce55-6043-4926-9fc6-b2317afb2b4b)


### github tutorial
about github: [get-started](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/quickstart-for-writing-on-github)

--------------------------------------------------------------------------------------------------------
### orangePI OS
to download OS ubuntu focal desktop linux 5.10.75

[Etcher for IoT](https://etcher.balena.io/#download-etcher)

[Orange PI OS](https://docs.viam.com/installation/prepare/orange-pi-3-lts/)
[official OS](http://www.orangepi.org/orangepiwiki/index.php/Orange_Pi_3_LTS)

--------------------------------------------------------------------------------------------------------
### connecting with SSH

Включение SSH на Orange Pi 3 LTS
Включение SSH через командную строку (если доступен монитор и клавиатура)
Откройте терминал на Orange Pi.
Введите следующую команду, чтобы убедиться, что SSH установлен:

>sudo apt update

>sudo apt install openssh-server


После установки SSH-сервера убедитесь, что он запущен:

>sudo systemctl enable ssh

>sudo systemctl start ssh


Проверьте статус SSH:

>sudo systemctl status ssh

--------------------------------------------------------------------------------------------------------
## connecting with SSH from vscode 
https://code.visualstudio.com/docs/remote/ssh

https://www.youtube.com/watch?v=BkQ2hL0fkWM

https://help.rc.ufl.edu/doc/SSH_Using_VS_Code

https://code.visualstudio.com/docs/remote/ssh-tutorial

https://www.ssh.com/academy/ssh/command

>ctrl + shift + P

>vs code> Remote-SSH: Connect to Host

>Add Host> ssh orangepi@<IP-адрес Orange Pi>

--------------------------------------------------------------------------------------------------------

Advanced IP Scanner

![IP Scanner](https://github.com/user-attachments/assets/0c3c928a-3faf-49d0-be9f-774e61a1bae6)


192.168.10.1-254
PUTTY

--------------------------------------------------------------------------------------------------------

sudo apt-get update
sudo apt-get upgrade


sudo apt install python3-pip
sudo apt-get install python3-dev

sudo pip3 install Orangepi.GPIO

-sudo apt-get install wiringpi
-sudo apt-get remove wiringpi

WiringPi-ն չի աշխատում OrangePI 3 LTS -ի համար պետք ա wiringOP
--------------------------------------------------------------------------------------------------------
https://github.com/orangepi-xunlong/wiringOP/blob/next/README.md
https://blog.regimov.net/orange-pi-lite2-1-wire/
https://micro-pi.ru/wiringop-wiringpi-%D0%BD%D0%B0-orange-pi-pc/
https://micro-pi.ru/%D1%81%D1%85%D0%B5%D0%BC%D0%B0-%D0%BF%D0%BE%D0%B4%D0%BA%D0%BB%D1%8E%D1%87%D0%B5%D0%BD%D0%B8%D1%8F-hc-sr04-%D0%BA-orange-pi-pc/#__HC-SR04_Orange_Pi_PCBanana_Pi_Raspberry_Pi_GPIO_40
https://orangepi.su/content.php?p=112&c=OrangePi%20PC%20rabotaem%20s%20GPIO,%20UART%20i%20i2c

http://www.orangepi.org/orangepiwiki/index.php/Orange_Pi_3_LTS

https://uelectronics.com/wp-content/uploads/2022/04/OrangePi-3-LTS-H6-User-manual-v1.0.pdf?srsltid=AfmBOooDsc42U6c_YOJfpQFX2p3PilGZzoPxofqVLSiIuqRE7INaIOAZ
page 84(79)

https://psenyukov.ru/%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%B0-%D1%81-gpio-%D1%80%D0%B0%D0%B7%D1%8A%D0%B5%D0%BC%D0%B0%D0%BC%D0%B8-%D0%B2-orange-pi/
https://raspberrypi.ru/578-wiring-pi-biblioteka-raboty-s-gpio-chast-1



apt-get install -y git
git clone https://github.com/orangepi-xunlong/wiringOP.git

cd wiringOP
./build clean
./build 

gpio readall

--------------------------------------------------------------------------------------------------------
http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/orange-pi-3-LTS.html
https://linux-sunxi.org/1-Wire
https://www.raspberrypi-spy.co.uk/2013/03/raspberry-pi-1-wire-digital-thermometer-sensor/
http://mypractic.ru/ds18b20-datchik-temperatury-s-interfejsom-1-wire-opisanie-na-russkom-yazyke.html
https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf
https://orangepi.su/content.php?p=194&chpu=Stavim%20datchik%20temperatury%20k%20Orange%20Pi%20/%20Raspberry%20Pi
https://github.com/leech001/Temp_control
https://f1atb.fr/temperature-ds18b20-and-orange-pi-zero/



>sudo nano /boot/orangepiEnv.txt
```bush
overlays=w1-gpio uart3 i2c0 i2c1
#if theree exist i2c then overlays=w1-gpio i2c0 i2c1 ...
#if theree exist serial then overlays=uart1 ...
param_w1_pin=PD22
param_w1_pin_int_pullup=1
```

or

>sudo orangepi-config > System > Bootenv

>sudo reboot

>sudo modprobe w1-gpio

>sudo modprobe w1-therm

//gpio mode 2 out ?

>gpio readall
```bash
 +------+-----+----------+--------+---+   OPi 3  +---+--------+----------+-----+------+
 | GPIO | wPi |   Name   |  Mode  | V | Physical | V |  Mode  | Name     | wPi | GPIO |
 +------+-----+----------+--------+---+----++----+---+--------+----------+-----+------+
 |      |     |     3.3V |        |   |  1 || 2  |   |        | 5V       |     |      |
 |  122 |   0 |    SDA.0 |   ALT2 | 0 |  3 || 4  |   |        | 5V       |     |      |
 |  121 |   1 |    SCL.0 |   ALT2 | 0 |  5 || 6  |   |        | GND      |     |      |
 |  118 |   2 |    PWM.0 |     IN | 1 |  7 || 8  | 0 | OFF    | PL02     | 3   | 354  |
 |      |     |      GND |        |   |  9 || 10 | 0 | OFF    | PL03     | 4   | 355  |
 |  120 |   5 |    RXD.3 |   ALT4 | 0 | 11 || 12 | 0 | OFF    | PD18     | 6   | 114  |
 |  119 |   7 |    TXD.3 |   ALT4 | 0 | 13 || 14 |   |        | GND      |     |      |
 |  362 |   8 |     PL10 |    OFF | 0 | 15 || 16 | 0 | OFF    | PD15     | 9   | 111  |
 |      |     |     3.3V |        |   | 17 || 18 | 0 | OFF    | PD16     | 10  | 112  |
 |  229 |  11 |   MOSI.1 |   ALT4 | 0 | 19 || 20 |   |        | GND      |     |      |
 |  230 |  12 |   MISO.1 |   ALT4 | 0 | 21 || 22 | 0 | OFF    | PD21     | 13  | 117  |
 |  228 |  14 |   SCLK.1 |    OFF | 0 | 23 || 24 | 0 | OFF    | CE.1     | 15  | 227  |
 |      |     |      GND |        |   | 25 || 26 | 0 | OFF    | PL08     | 16  | 360  |
 +------+-----+----------+--------+---+----++----+---+--------+----------+-----+------+
 | GPIO | wPi |   Name   |  Mode  | V | Physical | V |  Mode  | Name     | wPi | GPIO |
 +------+-----+----------+--------+---+   OPi 3  +---+--------+----------+-----+------+
```

>lsmod | grep w1
```bash
w1_therm               28672  0
w1_gpio                16384  0
wire                   36864  2 w1_gpio,w1_therm
```

>ls /sys/bus/w1/devices/
```
w1_bus_master1  28-XXXXXXXXXXXX
```



![Rectangle 741](https://github.com/user-attachments/assets/9c7eace9-507e-4ed3-bf48-2cedfa4ee452)
![Rectangle 741](https://github.com/user-attachments/assets/9022c55d-a769-40f4-890f-462ed39d795b)

--------------------------------------------------------------------------------------------------------

pip3 install --upgrade w1thermsensor

--------------------------------------------------------------------------------------------------------
sudo apt-get update
sudo apt-get install i2c-tools

sudo nano /boot/orangepiEnv.txt
overlays=i2c0 i2c1
param_i2c1_baudrate=100000 


sudo i2cdetect -y 0  # Для I2C-0
sudo i2cdetect -y 1  # Для I2C-1





cat <dir> //for visualisation
--------------------------------------------------------------------------------------------------------

OpenWeatherMap API
https://openweathermap.org/price#weather        //main

https://openweathermap.org/api                  //patetner
https://openweathermap.org/api/one-call-3
https://home.openweathermap.org/api_keys
https://openweathermap.org/weather-conditions    //icons

https://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={API key}&units=metric
api.openweathermap.org/data/2.5/forecast?lat={lat}&lon={lon}&appid={API key}&units=metric

marmrashen lat{40.059456} lon{44.474210}
my appid {fcb989e5668460983b3cb819569b8c1d}

https://api.openweathermap.org/data/2.5/weather?lat=40.059456&lon=44.474210&appid=fcb989e5668460983b3cb819569b8c1d&units=metric
https://api.openweathermap.org/data/2.5/forecast?lat=40.059456&lon=44.474210&appid=fcb989e5668460983b3cb819569b8c1d&units=metric

https://openweathermap.org/bulk
https://openweathermap.org/forecast5

--------------------------------------------------------------------------------------------------------
JSON for Modern C++ (Niels Lohmann)  :JSON-ի հետ աշխատելու համար
   sudo apt-get install nlohmann-json3-dev

libcurl                              :библиотеку для выполнения HTTP-запросов
    sudo apt-get install libcurl4-openssl-dev


--------------------------------------------------------------------------------------------------------

##  UART(Serial) communication with OrangePi and Arduino

https://uelectronics.com/wp-content/uploads/2022/04/OrangePi-3-LTS-H6-User-manual-v1.0.pdf?srsltid=AfmBOooDsc42U6c_YOJfpQFX2p3PilGZzoPxofqVLSiIuqRE7INaIOAZ
page 89(84)

1) activate uart 
>sudo nano /boot/orangepiEnv.txt
```bush
overlays=uart3
```
>sudo reboot

2) Screen is a tool to analys UART(Serial) communication, testing uart with screen

>sudo apt-get install screen

>sudo apt-get upgrade screen

>screen /dev/ttyS1 9600

to exit >ctrl+A then K then Y

### for testing we have 
[serialCommunicationTest](https://github.com/arturmart/GreenHose/tree/master/test/serialCommunicationTest)

for Arduino
>sct.ino

for OrangePi
>sct.cpp



arduino to Orange pi

*https://www.programmingelectronics.com/serial-read/

https://www.researchgate.net/figure/Pattern-selection-from-recorded-sample-intervals_fig2_350297410

https://www.arducoding.com/2018/05/serial-debug-pada-orange-pi-menggunakan.html

https://micro-pi.ru/rpi-pi4j-%D1%83%D1%80%D0%BE%D0%BA-6-%D1%88%D0%B8%D0%BD%D0%B0-spi/

*https://roboticsbackend.com/raspberry-pi-arduino-serial-communication/

*https://psenyukov.ru/%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%B0-%D1%81-gpio-%D1%80%D0%B0%D0%B7%D1%8A%D0%B5%D0%BC%D0%B0%D0%BC%D0%B8-%D0%B2-orange-pi/

https://f1atb.fr/orange-pi-gpio/

https://www.amazon.com/SparkFun-Logic-Level-Converter-Bi-Directional/dp/B01N30ZCW9/ref=sr_1_1?dib=eyJ2IjoiMSJ9.qrzfvZOtn4CIylDG2bI-VZHxiJZkcBVgtb9gS1lu7s5IKVMoyg7MFJySEeBTWM3xrZEpWQSxSit2T8eRnBpXxrzDk2qphxGCPGfOf3hfQvS0mw12tWJ_-tW6ddzlrbdLBeQL--T21PelzQUKha1IiBkPjfc97AHP_qNKGe4_6-LZjzHgBDLa3SmsSdNe_qgFBqMiTubgt201ywSuMsG8cgPVh2b1JjHY8XASEPGEf8k._UkYar4XbircUF7dRs_yiUslKo3eLcuC1Z4ykHcsUhA&dib_tag=se&keywords=logic+level+converter&qid=1729101691&sr=8-1


------------------------------------------------------------------------
## Telegram
[TGBot](https://github.com/reo7sp/tgbot-cpp )
[Telegram API](https://core.telegram.org/bots/api)
[for True Build](https://hub.docker.com/r/reo7sp/tgbot-cpp/)





### for install
>sudo apt install g++ make binutils cmake libboost-system-dev libssl-dev zlib1g-dev libcurl4-openssl-dev

>sudo apt install libboost-test-dev doxygen



>git clone https://github.com/reo7sp/tgbot-cpp

>cd tgbot-cpp

>cmake .

>make -j4

>sudo make install


sudo dpkg-reconfigure tzdata // to change Time


[JSON to chart](https://jsontochart.com/)



