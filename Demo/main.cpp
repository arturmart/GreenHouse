#include <iostream>
#include <string>
#include "Executor.h"
#include "DataGeter.h"
#include "Logger.h"

// g++ -o main main.cpp Executor.cpp -pthread I2CLCD.cpp -lwiringPi RelayControlModule.cpp DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp DataCombine.cpp crcCalc.cpp -lz DataGeter.cpp DS18B20.cpp WeatherAPI.cpp -lcurl Logger.cpp jsonManager.cpp -I /path/to/nlohmann/json ChartDrawing.cpp -I/usr/include/cairo -lcairo TimeManager.cpp 
// g++ -o executor Executor.cpp -pthread I2CLCD.cpp -lwiringPi RelayControlModule.cpp DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp DataCombine.cpp crcCalc.cpp -lz 
// g++ -o datagetter DataGeter.cpp DS18B20.cpp WeatherAPI.cpp -lcurl
// g++ -o logger Logger.cpp jsonManager.cpp -I /path/to/nlohmann/json ChartDrawing.cpp -I/usr/include/cairo -lcairo TimeManager.cpp 

Executor executor;
DataGetter dataGetter;
Logger LOG("Log3",{"date","temp","tempOut","R1","R2"});


int main(){

   
   executor.execute("lcd", "Hello!\nGreenHouse");
   while(true){

      
      

      LOG.setData("temp",std::to_string( dataGetter.getNewData("temp")));
      executor.execute("lcd",std::to_string(dataGetter.getData("temp")));
      LOG.setData("tempOut", std::to_string(dataGetter.getNewData("tempOut")));
      LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
      LOG.log();

      
      executor.execute("R1_ON");
      LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
      LOG.setData("R1","on");
      LOG.log();
      //std::this_thread::sleep_for(std::chrono::seconds(1));
      executor.execute("R1_OFF");
      LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
      LOG.setData("R1","off");
      LOG.log();

      LOG.drawChart("mainChart","date",{"temp","tempOut"},{"R1","R2","R3","R4","R5","R6","R7","R8"});
      
      

      
   }
 

    std::this_thread::sleep_for(std::chrono::seconds(6));

    std::cout << "Завершение программы" << std::endl;
    return 0;

}