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


## for Change or Add Getter 


1) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp)  getterCommandsMap add Telegram Commands in Main `getterCommandsMap = {... ,{{"TG_BOT", "getTemp"} , {"DATA_GETTER", "temp"}}, ...} ` For Telegram Command
2) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp) Logger LOG Constructor Add Names  `Logger LOG(... ,{...,"temp",...});` for Logging
3) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp) in `getterCommandTGBot(...)` change `LOG.drawChart(... , ... ,{"temp",...}, ...);` for Chart
4) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp) on Registers Initilizationa on getterRegistor add module `std::unordered_map<std::string, double> getterRegistor = {{"temp",-255},...}`  For Register
6) on [DataGetter](https://github.com/arturmart/GreenHose/blob/master/Demo/DataGeter.cpp) DataGetter::DataGetter() Constructor add Strategy `strategy["temp"] = new TempStrategy("28-0303979402d4");` 
7) on [TelegramBot](https://github.com/arturmart/GreenHose/blob/master/Demo/TelegramBot.cpp) getterCommands() `{{"TEXT", "get Temp In"}, {"CALL_BACK_DATA", "getTemp"}},` For Button on Telegram *same as point 1*. There must be odd
   
## for test Temp Module!  

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

--------

## for Add Executor
1) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp) executeCommandsMap add Telegram Commands in Main `getterCommandsMap = {... ,{{"TG_BOT", "IR1On"}, {"EXECUTOR", "IR1_ON"}, {"MODE", "on"}, {"MODULE", "IR1"}}, ...} ` For Telegram Command
2)  on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp)  on Registers Initilizationa on executorRegister and autoModeExecutorRegister add module `std::unordered_map<std::string, double> autoModeExecutorRegister and executorRegister = {"IR1", false},`  For Register
3) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp) Logger LOG Constructor Add Names  `Logger LOG(... ,{...,"IR1",...});` for Logging
4) on [main](https://github.com/arturmart/GreenHose/blob/master/Demo/main.cpp) in `getterCommandTGBot(...)` change `LOG.drawChart(... , ... ,..., {"IR1",...});` for Chart
5) on [Executor](https://github.com/arturmart/GreenHouse/blob/master/Demo/Executor.cpp) in Executor::Executor() add New Strategy `executeMap["IR1_ON"] = new RCM_Strategy(RCM, "83,6,1");` `executeMap["IR1_OFF"] = new RCM_Strategy(RCM, "83,6,0");`
6) on [TelegramBot](https://github.com/arturmart/GreenHose/blob/master/Demo/TelegramBot.cpp) manualCommands() `{{"TEXT", "IR1 On"}, {"CALL_BACK_DATA", "IR1On"}},
    {{"TEXT", "IR1 Off"}, {"CALL_BACK_DATA", "IR1Off"}},
    {{"TEXT", "IR1 Auto"}, {"CALL_BACK_DATA", "IR1Auto"}},`
   For Button on Telegram *same as point 1*. There must be divide to 3



