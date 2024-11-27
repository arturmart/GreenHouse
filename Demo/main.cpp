#include <iostream>
#include <string>
#include <unordered_map>

#include "Executor.h"
#include "DataGeter.h"
#include "Logger.h"
#include "TelegramBot.h"

// g++ -o main main.cpp Executor.cpp -pthread I2CLCD.cpp -lwiringPi RelayControlModule.cpp DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp DataCombine.cpp crcCalc.cpp -lz DataGeter.cpp DS18B20.cpp WeatherAPI.cpp -lcurl Logger.cpp jsonManager.cpp -I /path/to/nlohmann/json ChartDrawing.cpp -I/usr/include/cairo -lcairo TimeManager.cpp TelegramBot.cpp -I /path/to/nlohmann/json -lTgBot -lboost_system -lssl -lcrypto -lpthread  
// g++ -o executor Executor.cpp -pthread I2CLCD.cpp -lwiringPi RelayControlModule.cpp DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp DataCombine.cpp crcCalc.cpp -lz 
// g++ -o datagetter DataGeter.cpp DS18B20.cpp WeatherAPI.cpp -lcurl
// g++ -o logger Logger.cpp jsonManager.cpp -I /path/to/nlohmann/json ChartDrawing.cpp -I/usr/include/cairo -lcairo TimeManager.cpp 
//g++ -std=c++17 -o test TelegramBot.cpp -I /path/to/nlohmann/json -lTgBot -lboost_system -lssl -lcrypto -lpthread 


std::string doubleToString(double value) {
    char buffer[50];  // Буфер для хранения отформатированной строки
    sprintf(buffer, "%.2f", value);  // Форматируем число с 2 знаками после запятой
    return std::string(buffer);  // Возвращаем строку
}


std::vector<std::unordered_map<std::string, std::string>> getterCommandsMap = {
    {{"TG_BOT", "getTemp"} , {"DATA_GETTER", "temp"}},
    {{"TG_BOT", "getTempOut"}, {"DATA_GETTER", "tempOut"}},
    {{"TG_BOT", "getTempWaterIn"}},
    {{"TG_BOT", "getTempWaterOut"}},
    {{"TG_BOT", "getChart"}, {"SAND_CHART", "true"}},
    {{"TG_BOT", "getLogChart"}, {"SAND_CHART", "true"}, {"SAND_LOG", "true"}}
};

std::vector<std::unordered_map<std::string, std::string>> executeCommandsMap = {
    {{"TG_BOT", "BakeOn"} , {"EXECUTOR", "Bake_ON"}, {"MODE", "on"}, {"MODULE", "Bake"}},
    {{"TG_BOT", "BakeOff"}, {"EXECUTOR", "Bake_OFF"}, {"MODE", "off"}, {"MODULE", "Bake"}},
    {{"TG_BOT", "BakeAuto"}, {"AUTO", "Bake"}},
    {{"TG_BOT", "PumpOn"}, {"EXECUTOR", "Pump_ON"}, {"MODE", "on"}, {"MODULE", "Pump"}},
    {{"TG_BOT", "PumpOff"}, {"EXECUTOR", "Pump_OFF"}, {"MODE", "off"}, {"MODULE", "Pump"}},
    {{"TG_BOT", "PumpAuto"}, {"AUTO", "Pump"}},
    {{"TG_BOT", "Falcon1On"}, {"EXECUTOR", "Falcon1_ON"}, {"MODE", "on"}, {"MODULE", "Falcon1"}},
    {{"TG_BOT", "Falcon1Off"}, {"EXECUTOR", "Falcon1_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon1"}},
    {{"TG_BOT", "Falcon1Auto"}, {"AUTO", "Falcon1"}},
    {{"TG_BOT", "Falcon2On"}, {"EXECUTOR", "Falcon2_ON"}, {"MODE", "on"}, {"MODULE", "Falcon2"}},
    {{"TG_BOT", "Falcon2Off"}, {"EXECUTOR", "Falcon2_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon2"}},
    {{"TG_BOT", "Falcon2Auto"}, {"AUTO", "Falcon2"}},
    {{"TG_BOT", "Falcon3On"}, {"EXECUTOR", "Falcon3_ON"}, {"MODE", "on"}, {"MODULE", "Falcon3"}},
    {{"TG_BOT", "Falcon3Off"}, {"EXECUTOR", "Falcon3_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon3"}},
    {{"TG_BOT", "Falcon3Auto"}, {"AUTO", "Falcon3"}},
    {{"TG_BOT", "Falcon4On"}, {"EXECUTOR", "Falcon4_ON"}, {"MODE", "on"}, {"MODULE", "Falcon4"}},
    {{"TG_BOT", "Falcon4Off"}, {"EXECUTOR", "Falcon4_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon4"}},
    {{"TG_BOT", "Falcon4Auto"}, {"AUTO", "Falcon4"}}

};

void getterCommandTGBot(const std::string& arg);
void executorCommandTGBot(const std::string& arg);

Executor executor;
DataGetter dataGetter;
Logger LOG("Log4",{"date","temp","tempOut","Bake","Pump","Falcon1","Falcon2","Falcon3","Falcon4"});
TelegramBot bot("7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c",getterCommandTGBot,executorCommandTGBot);


void getterCommandTGBot(const std::string& arg){
   for(auto it : getterCommandsMap){
      if(arg == it["TG_BOT"]) {
         if(it.find("DATA_GETTER") != it.end()){
            double dt = dataGetter.getNewData(it["DATA_GETTER"]);
            bot.sendAllUserMessage(doubleToString(dt));
         }
         if(it.find("SAND_CHART") != it.end()){
            LOG.drawChart("mainChart","date",{"temp","tempOut"},{"Bake","Pump","Falcon1","Falcon2","Falcon3","Falcon4"});
            bot.sendAllUserPhoto("mainChart.png","toDayChart");

         }
         if(it.find("SAND_LOG") != it.end()){
            bot.sendAllUserDocument("Log4.json","toDayChart");

         }
      }
      
   }
}
void executorCommandTGBot(const std::string& arg){
   for(auto it : executeCommandsMap){
      if(arg == it["TG_BOT"]) {
         if(it.find("EXECUTOR") != it.end()){
            executor.execute(it["EXECUTOR"]);
            LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
            LOG.setData(it["MODULE"],it["MODE"]);
            LOG.log();
            
         }
      }
   }
}



int main(){

   
   executor.execute("lcd", "Hello!\nGreenHouse");
   bot.run();
   /*
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
    */

    std::cout << "Завершение программы" << std::endl;
    return 0;

}