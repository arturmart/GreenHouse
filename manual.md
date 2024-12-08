#  Manual For Green House

## Device Control Module

For Connect to Device Control Module (Arduino Mega) Connect USB Type B to Controller. 

**Turn off Power of Main Modlue(Orange Pi)!**

![photo_2024-12-07_23-08-12](https://github.com/user-attachments/assets/dd9b4f2f-c5a1-43a2-8b79-8785d2c3cc9d)



### For Change or Add Relay PinOut
in [deviceControllModule](https://github.com/arturmart/GreenHose/blob/master/deviceControllModule/deviceControllModule.ino) change `RELAY_COUNT` and `relayPin` and **Upload**

## Main Controller

for test Relay [testRCM](https://github.com/arturmart/GreenHose/blob/master/Demo/testRCM.cpp)

--------
for test Temp Module!

for Change or Add Module 


1) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp)  getterCommandsMap add Telegram Commands in Main `getterCommandsMap = {... ,{{"TG_BOT", "getTemp"} , {"DATA_GETTER", "temp"}}, ...} ` For Telegram Command
2) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp) Logger LOG Constructor Add Names  `Logger LOG(... ,{...,"temp",...});` for Logging
3) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp) in `getterCommandTGBot(...)` change `LOG.drawChart(... , ... ,{"temp",...}, ...);` for Chart
4) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp) on Registers Initilizationa on getterRegistor add module `std::unordered_map<std::string, double> getterRegistor = {{"temp",-255},...}`  For Register
6) on [DataGetter](https://github.com/arturmart/GreenHose/blob/master/Demo/DataGeter.cpp) DataGetter::DataGetter() Constructor add Strategy `strategy["temp"] = new TempStrategy("28-0303979402d4");` 
7) on [TelegramBot](https://github.com/arturmart/GreenHose/blob/master/Demo/TelegramBot.cpp) getterCommands() `{{"TEXT", "get Temp In"}, {"CALL_BACK_DATA", "getTemp"}},` For Button on Telegram *same as point 1*. There must be odd
   
   
|ID|For                        | Name |
|--|--                        | --|
|28-0303979402d4| main | temp|
|28-030397942cf4| secondory|temp2|
|28-030397946349| input Bake |inBake|
|28-04175013faff| Output Bake | outBake|

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
