# GreenHose


//to download OS ubuntu focal desktop linux 5.10.75

https://etcher.balena.io/#download-etcher
https://docs.viam.com/installation/prepare/orange-pi-3-lts/

--------------------------------------------------------------------------------------------------------

Включение SSH на Orange Pi 3 LTS
Способ 1: Включение SSH через командную строку (если доступен монитор и клавиатура)
Откройте терминал на Orange Pi.
Введите следующую команду, чтобы убедиться, что SSH установлен:

sudo apt update
sudo apt install openssh-server

После установки SSH-сервера убедитесь, что он запущен:

sudo systemctl enable ssh
sudo systemctl start ssh

Проверьте статус SSH:

sudo systemctl status ssh

--------------------------------------------------------------------------------------------------------

Advanced IP Scanner
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

# apt-get install -y git
# git clone https://github.com/orangepi-xunlong/wiringOP.git

# cd wiringOP
# ./build clean
# ./build 

gpio readall

--------------------------------------------------------------------------------------------------------

sudo nano /boot/orangepiEnv.txt
param_w1_pin=22

sudo reboot

sudo modprobe w1-gpio
sudo modprobe w1-therm

lsmod | grep w1

ls /sys/bus/w1/devices/

--------------------------------------------------------------------------------------------------------

pip3 install --upgrade w1thermsensor



http://www.sunshine2k.de/coding/javascript/crc/crc_js.html

https://crc32.online/

https://www.rapidtables.com/convert/number/hex-to-ascii.html

R,1,1,R,4,0/1667261463
R,1,1,R,4,1/342316161

https://github.com/GyverLibs/GParser?tab=readme-ov-file


https://www.youtube.com/watch?v=Ul03Vkg9A40
