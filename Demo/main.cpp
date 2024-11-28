#include <iostream>
#include <string>
#include <unordered_map>

#include "Executor.h"
#include "DataGeter.h"
#include "Logger.h"
#include "TelegramBot.h"

#include <thread> // Для работы с потоками

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
std::string getDay(){
   return unixTimestampToDateString(to_unix_timestamp(std::chrono::system_clock::now()));
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
    {{"TG_BOT", "BakeAuto"},{"MODE", "auto"}, {"MODULE", "Bake"}},
    {{"TG_BOT", "PumpOn"}, {"EXECUTOR", "Pump_ON"}, {"MODE", "on"}, {"MODULE", "Pump"}},
    {{"TG_BOT", "PumpOff"}, {"EXECUTOR", "Pump_OFF"}, {"MODE", "off"}, {"MODULE", "Pump"}},
    {{"TG_BOT", "PumpAuto"}, {"MODE", "auto"},{"MODULE", "Pump"}},
    {{"TG_BOT", "Falcon1On"}, {"EXECUTOR", "Falcon1_ON"}, {"MODE", "on"}, {"MODULE", "Falcon1"}},
    {{"TG_BOT", "Falcon1Off"}, {"EXECUTOR", "Falcon1_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon1"}},
    {{"TG_BOT", "Falcon1Auto"}, {"MODE", "auto"},{"MODULE", "Falcon1"}},
    {{"TG_BOT", "Falcon2On"}, {"EXECUTOR", "Falcon2_ON"}, {"MODE", "on"}, {"MODULE", "Falcon2"}},
    {{"TG_BOT", "Falcon2Off"}, {"EXECUTOR", "Falcon2_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon2"}},
    {{"TG_BOT", "Falcon2Auto"}, {"MODE", "auto"},{"MODULE", "Falcon2"}},
    {{"TG_BOT", "Falcon3On"}, {"EXECUTOR", "Falcon3_ON"}, {"MODE", "on"}, {"MODULE", "Falcon3"}},
    {{"TG_BOT", "Falcon3Off"}, {"EXECUTOR", "Falcon3_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon3"}},
    {{"TG_BOT", "Falcon3Auto"}, {"MODE", "auto"},{"MODULE", "Falcon3"}},
    {{"TG_BOT", "Falcon4On"}, {"EXECUTOR", "Falcon4_ON"}, {"MODE", "on"}, {"MODULE", "Falcon4"}},
    {{"TG_BOT", "Falcon4Off"}, {"EXECUTOR", "Falcon4_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon4"}},
    {{"TG_BOT", "Falcon4Auto"}, {"MODE", "auto"},{"MODULE", "Falcon4"}}

};

std::unordered_map<std::string, bool> autoModeRegister = {
    {"Bake", true},
    {"Pump", true},
    {"Falcon1", true},
    {"Falcon2", true},
    {"Falcon3", true},
    {"Falcon4", true}

};




double temp;
double tempOut;

void getterCommandTGBot(const std::string& arg);
void executorCommandTGBot(const std::string& arg);

Executor executor;
DataGetter dataGetter;
Logger LOG("Log"+getDay(),{"date","temp","tempOut","Bake","Pump","Falcon1","Falcon2","Falcon3","Falcon4"});
TelegramBot bot("7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c",getterCommandTGBot,executorCommandTGBot);


void getterCommandTGBot(const std::string& arg){
   for(auto it : getterCommandsMap){
      if(arg == it["TG_BOT"]) {
         if(it.find("DATA_GETTER") != it.end()){
            double dt = dataGetter.getNewData(it["DATA_GETTER"]);
            bot.sendAllUserMessage(doubleToString(dt));
         }
         if(it.find("SAND_CHART") != it.end()){
            LOG.drawChart("Chart"+getDay(),"date",{"temp","tempOut"},{"Bake","Pump","Falcon1","Falcon2","Falcon3","Falcon4"});
            bot.sendAllUserPhoto("Chart"+getDay()+".png", getDay()+" Chart");

         }
         if(it.find("SAND_LOG") != it.end()){
            bot.sendAllUserDocument("Log"+getDay()+".json",getDay() + "Log");

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

            //std::cout<<"BAKE FALSE"<<std::endl;

            autoModeRegister[it["MODULE"]] = false;
            
         }
         if(it.find("MODE") != it.end()){
            if(it["MODE"] == "auto"){
               autoModeRegister[it["MODULE"]] = true;
            }
            
         }
         
      }
   }
}



int main(){

   
   
   std::thread botThread(&TelegramBot::run, &bot);

   while(true){

      executor.execute("lcd", "T="+doubleToString(dataGetter.getNewData("temp")));

      LOG.setData("temp",std::to_string( dataGetter.getData("temp")));
      //executor.execute("lcd",std::to_string(dataGetter.getData("temp")));
      LOG.setData("tempOut", std::to_string(dataGetter.getNewData("tempOut")));
      LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
      LOG.log();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if(autoModeRegister["Bake"]){
          executor.execute("Bake_ON");
          LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
          LOG.setData("Bake","on");
          LOG.log();
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if(autoModeRegister["Bake"]){
          executor.execute("Bake_OFF");
          LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
          LOG.setData("Bake","off");
          LOG.log();
      }

      if(dataGetter.getData("temp") > 24){
         if(autoModeRegister["Pump"]){
            executor.execute("Pump_ON");
            LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
            LOG.setData("Pump","on");
            LOG.log();
         }
      }

      if(dataGetter.getData("temp") < 22){
         if(autoModeRegister["Pump"]){
            executor.execute("Pump_OFF");
            LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
            LOG.setData("Pump","off");
            LOG.log();
         }
      }
      

   }

   if (botThread.joinable()) {
        botThread.join();
    }

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