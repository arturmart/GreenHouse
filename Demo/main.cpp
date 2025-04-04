#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <curl/curl.h>
#include <cstdlib>
#include <thread> // Для работы с потоками

#include "Executor.h"
#include "DataGeter.h"
#include "Logger.h"
#include "TelegramBot.h"
#include "tokens.h"
#include "ConditionJson.h"

#include "ghconfig.h"

#if GH_SIMULATION == true
#include"ghSim.h"
#endif




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

std::string getDayHMS(){
   return unixTimestampToTimeStringDate(to_unix_timestamp(std::chrono::system_clock::now()));
}


std::vector<std::unordered_map<std::string, std::string>> getterCommandsMap = {
    {{"TG_BOT", "getTemp"} , {"DATA_GETTER", "temp"}},
    //{{"TG_BOT", "getTemp2"} , {"DATA_GETTER", "temp2"}},

    {{"TG_BOT", "getTempInBake"} , {"DATA_GETTER", "inBake"}},
    {{"TG_BOT", "getTempOutBake"} , {"DATA_GETTER", "outBake"}},

    {{"TG_BOT", "getTempOut"}, {"DATA_GETTER", "tempOut"}},

    {{"TG_BOT", "getChart"}, {"SAND_CHART", "true"}},
    {{"TG_BOT", "getLogChart"}, {"SAND_CHART", "true"}, {"SAND_LOG", "true"}},
    {{"TG_BOT", "getStates"}, {"SAND_STATES", "true"}},

    

    {{"TG_BOT", "getConditionJson"}, {"SAND_COND_JSON", "true"}},
    {{"TG_BOT", "getConditionTree"}, {"SAND_COND_TREE", "true"}},

    {{"TG_BOT", "getAllData"}, {"SAND_ALL_DATA", "true"}}

    
};

std::vector<std::unordered_map<std::string, std::string>> executeCommandsMap = {
   #if EXECUTOR_WINTER == true   
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
    {{"TG_BOT", "Falcon4Auto"}, {"MODE", "auto"},{"MODULE", "Falcon4"}},


    {{"TG_BOT", "IR1On"}, {"EXECUTOR", "IR1_ON"}, {"MODE", "on"}, {"MODULE", "IR1"}},
    {{"TG_BOT", "IR1Off"}, {"EXECUTOR", "IR1_OFF"}, {"MODE", "off"}, {"MODULE", "IR1"}},
    {{"TG_BOT", "IR1Auto"}, {"MODE", "auto"},{"MODULE", "IR1"}},

    {{"TG_BOT", "IR2On"}, {"EXECUTOR", "IR2_ON"}, {"MODE", "on"}, {"MODULE", "IR2"}},
    {{"TG_BOT", "IR2Off"}, {"EXECUTOR", "IR2_OFF"}, {"MODE", "off"}, {"MODULE", "IR2"}},
    {{"TG_BOT", "IR2Auto"}, {"MODE", "auto"},{"MODULE", "IR2"}},
    #endif
    #if EXECUTOR_SUMMER == 1
    {{"TG_BOT", "Cooler1On"}, {"EXECUTOR", "Cooler1_ON"}, {"MODE", "on"}, {"MODULE", "Cooler1"}},
    {{"TG_BOT", "Cooler1Off"}, {"EXECUTOR", "Cooler1_OFF"}, {"MODE", "off"}, {"MODULE", "Cooler1"}},
    {{"TG_BOT", "Cooler1Auto"}, {"MODE", "auto"},{"MODULE", "Cooler1"}},

    {{"TG_BOT", "Cooler2On"}, {"EXECUTOR", "Cooler2_ON"}, {"MODE", "on"}, {"MODULE", "Cooler2"}},
    {{"TG_BOT", "Cooler2Off"}, {"EXECUTOR", "Cooler2_OFF"}, {"MODE", "off"}, {"MODULE", "Cooler2"}},
    {{"TG_BOT", "Cooler2Auto"}, {"MODE", "auto"},{"MODULE", "Cooler2"}},


    #endif

    {{"TG_BOT", "Light1On"}, {"EXECUTOR", "Light1_ON"}, {"MODE", "on"}, {"MODULE", "Light1"}},
    {{"TG_BOT", "Light1Off"}, {"EXECUTOR", "Light1_OFF"}, {"MODE", "off"}, {"MODULE", "Light1"}},
    {{"TG_BOT", "Light1Auto"}, {"MODE", "auto"},{"MODULE", "Light1"}},

    {{"EXECUTOR", "AlertHighTempBake"},{"MODULE_TG", "ALERT"}, {"TEXTSEND", "AlertHighTempBake"}},
    {{"EXECUTOR", "AlertLowTemp"},{"MODULE_TG", "ALERT"}, {"TEXTSEND", "AlertLowTemp"}},
    {{"EXECUTOR", "AlertBakeBadWork"},{"MODULE_TG", "ALERT"}, {"TEXTSEND", "AlertBakeBadWork"}},
 


    {{"TG_BOT", "ReadNewJsonCond"}, {"READ_NEW_JSON_DATA", "true"}}

};

std::vector<std::unordered_map<std::string, std::string>> masterCommandsMap = {
    {{"TG_BOT", "getJournal"}},
    {{"TG_BOT", "getIP"}},
    {{"TG_BOT", "getLogDir"}}, 
    {{"TG_BOT", "getStatus"}}
 

};

std::mutex Mutex;

void getterCommandTGBot(const std::string& arg);
void executorCommandTGBot(const std::string& arg);
void masterCommandsMapBot(const std::string& arg);




//Registers Initilization

std::unordered_map<std::string, std::string> getterRegistor = {
    {"date","0"},
    {"dateDaily","0"},//Daily Time
    {"temp","-255"},
    //{"temp2","-255"},

    {"inBake","-255"},
    {"outBake","-255"},

    {"tempOut","-255"},
};

std::unordered_map<std::string, bool> executorRegister = {

   #if EXECUTOR_WINTER == true 
    {"Bake", false},
    {"Pump", false},
    {"Falcon1", false},
    {"Falcon2", false},
    {"Falcon3", false},
    {"Falcon4", false},
    
    {"IR1", false},
    {"IR2", false},

    #endif
    #if EXECUTOR_SUMMER == true 
    {"Cooler1", false},
    {"Cooler2", false},
    #endif

    {"Light1", false}

};

std::unordered_map<std::string, bool> autoModeExecutorRegister = {
   
   #if EXECUTOR_WINTER == true 
    {"Bake", true},
    {"Pump", true},
    {"Falcon1", true},
    {"Falcon2", true},
    {"Falcon3", true},
    {"Falcon4", true},
    
    {"IR1", true},
    {"IR2", true},

    #endif
    #if EXECUTOR_SUMMER == true 
    {"Cooler1", true},
    {"Cooler2", true},
    #endif

    {"Light1", true}

};


void doExecuteAuto(const std::string& exe); //init

std::string thisDay = getDay();
int thisHMS = stoi(getDayHMS());
bool isIntr = false;

#if GH_SIMULATION == false
Executor executor;
DataGetter dataGetter;
#else

HeatingSystemSim sim;
Executor executor( &sim );
DataGetter dataGetter( &sim );

#endif

Logger LOG(
   "Log/Log"+thisDay,
   {"date","temp",/*"temp2",*/"inBake", "outBake","tempOut",
      #if EXECUTOR_WINTER == true 
      "Bake","Pump","Falcon1","Falcon2","Falcon3","Falcon4","IR1","IR2","Light1"
      #endif
      #if EXECUTOR_SUMMER == true
      "Cooler1","Cooler2","Light1"
      #endif 
   });

TelegramBot bot(
#if GH_SIMULATION == false
"7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c"/*TOKEN_TELEGRAM*/
#else
"7943156657:AAEtMkcU_96y-8BatCoaOhIbOtfZ58R021Q"/*TOKEN_TELEGRAM_Test*/
#endif
,getterCommandTGBot,executorCommandTGBot,masterCommandsMapBot);

#if AUTOMATIZATION
   Composite  MainPattern(getterRegistor,doExecuteAuto, "main", "Always", {}, {});
#endif

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t totalSize = size * nmemb;
    buffer->append((char*)contents, totalSize);
    return totalSize;
}

bool isInternet(){
   return system("ping -c 1 api.telegram.org > /dev/null 2>&1") == 0;

}

/*
bool isInternet() {
    try {
        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Не удалось инициализировать CURL" << std::endl;
            return false;
        }

        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_URL, "https://google.com"); // Публичный URL
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);             // Таймаут в 5 секунд
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);      // Следование за редиректами
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);           // Отключение сигналов

        CURLcode res = curl_easy_perform(curl); // Выполняем запрос

        if (res == CURLE_OK) {
            curl_easy_cleanup(curl);
            return true;
        } else {
            std::cerr << "Ошибка при проверке интернета: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Исключение при проверке интернета: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Неизвестная ошибка при проверке интернета." << std::endl;
        return false;
    }
}
*/




void getterRegisterUpdate(){
   //Maded for same types 
   std::lock_guard<std::mutex> lock(Mutex);
   for(auto& pair  : getterRegistor){
      //std::cout<<pair.first <<" "<< dataGetter.getNewData(pair.first)<<std::endl;
      if(pair.first == "date"){
      
         pair.second =  std::to_string(to_unix_timestamp(std::chrono::system_clock::now()));
      }
      else if(pair.first == "dateDaily" ){
         pair.second =  getDayHMS();//Daily Time
      }
      else
         pair.second =  std::to_string(dataGetter.getNewData(pair.first));
   }

}


void executorReigisterLog(){

   std::lock_guard<std::mutex> lock(Mutex);
   //LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));

   LOG.setData("date", getterRegistor["date"]);

   LOG.setData("Bake", executorRegister["Bake"]?"on" : "off");
   LOG.setData("Pump", executorRegister["Pump"]?"on" : "off");

   #if EXECUTOR_WINTER == true 

   LOG.setData("Falcon1", executorRegister["Falcon1"]?"on" : "off");
   LOG.setData("Falcon2", executorRegister["Falcon2"]?"on" : "off");
   LOG.setData("Falcon3", executorRegister["Falcon3"]?"on" : "off");
   LOG.setData("Falcon4", executorRegister["Falcon4"]?"on" : "off");

   LOG.setData("IR1", executorRegister["IR1"]?"on" : "off");
   LOG.setData("IR2", executorRegister["IR2"]?"on" : "off");
   #endif
   #if EXECUTOR_SUMMER == true
   LOG.setData("Cooler1", executorRegister["Cooler1"]?"on" : "off");
   LOG.setData("Cooler2", executorRegister["Cooler2"]?"on" : "off");
   #endif

   LOG.setData("Light1", executorRegister["Light1"]?"on" : "off");

   LOG.log();
}


void getterRegisterLog(){

   std::lock_guard<std::mutex> lock(Mutex);
   //LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
   
   LOG.setData("date", getterRegistor["date"]);
   LOG.setData("dateDaily", getterRegistor["dateDaily"]);
   LOG.setData("temp", getterRegistor["temp"]);
   //LOG.setData("temp2", getterRegistor["temp2"]);
   LOG.setData("inBake", getterRegistor["inBake"]);
   LOG.setData("outBake", getterRegistor["outBake"]);
   if(isIntr && getterRegistor["tempOut"] != "-255.000000") LOG.setData("tempOut", getterRegistor["tempOut"]);

   LOG.log();
}
void LogUnion(const std::string& first, const std::string& second){
   LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));//poxel
   LOG.setData(first,second);
   LOG.log();
}
void GetterAndLog(){
   
      getterRegisterUpdate();
      getterRegisterLog();
      //[*]std::this_thread::sleep_for(std::chrono::milliseconds(10000));
   
}
void GetterAndLogLoop(){ 
   while(true){
#if DEBUG_LOG 
      std::cout<<"[main][GetterAndLog_Thread] ITER"<<std::endl; 
#endif
        try {
             GetterAndLog();
        }
        catch (const std::exception& e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }
     
   }
}


void doExecuteManual(const std::string& exe){
   std::lock_guard<std::mutex> lock(Mutex);

   for(auto it : executeCommandsMap){
      if(it["EXECUTOR"]  == exe) {
         
                 
                  std::cout<<"Try Do Manual "<<it["MODULE"]<<" "<<it["MODE"]<<" reg("<<executorRegister[it["MODULE"]]<<")"<<std::endl;
                  if(executorRegister[it["MODULE"]] != (it["MODE"] == "on")){

                     
                     executor.execute(exe);
                     LogUnion(it["MODULE"],it["MODE"]);
                     executorRegister[it["MODULE"]] = (it["MODE"] == "on");
                     std::cout<<"Do Manual "<<it["MODULE"]<<" "<<it["MODE"]<<" reg("<<executorRegister[it["MODULE"]]<<")"<<std::endl;
                  }
                  autoModeExecutorRegister[it["MODULE"]] = false;
             

         
      }
   }
    
}
void doExecuteAuto(const std::string& exe){
   
   for(auto it : executeCommandsMap){
      if(it["EXECUTOR"] == exe) {
         
         if(autoModeExecutorRegister[it["MODULE"]]){
                 
                  
                  if(executorRegister[it["MODULE"]] != (it["MODE"] == "on")){
                     executor.execute(exe);
                     LogUnion(it["MODULE"],it["MODE"]);
                     executorRegister[it["MODULE"]] = (it["MODE"] == "on");
                  }

         }
         else if(it["MODULE_TG"] == "ALERT"){
            if(isIntr)bot.alertAllUser(it["TEXTSEND"]);

         }
         
      }

   }

}

void TurnOffAllRCM(){
   #if EXECUTOR_WINTER == true
   executor.execute("Bake_OFF");
   executor.execute("Pump_OFF");
   executor.execute("Falcon1_OFF");
   executor.execute("Falcon2_OFF");
   executor.execute("Falcon3_OFF");
   executor.execute("Falcon4_OFF");
   executor.execute("IR1_OFF");
   executor.execute("IR2_OFF");
   #endif

   #if EXECUTOR_SUMMER == true
   executor.execute("Cooler1_OFF");
   executor.execute("Cooler2_OFF");
   #endif

   executor.execute("Light1_OFF");
   
   
}

void getterCommandTGBot(const std::string& arg){
   
   std::cout<<"from \033[34m[TG]\033[0m "<<arg<<std::endl;
   //std::lock_guard<std::mutex> lock(Mutex);
   for(auto it : getterCommandsMap){
      if(arg == it["TG_BOT"]) {
         if(it.find("DATA_GETTER") != it.end()){
            //double dt = dataGetter.getData(it["DATA_GETTER"]);
            //bot.sendAllUserMessage(doubleToString(dt));
            if(isIntr) bot.sendAllUserMessage(it["DATA_GETTER"]+": "+ getterRegistor[it["DATA_GETTER"]]);
         }
          if(it.find("SAND_ALL_DATA") != it.end()){
            if(isIntr){
               bot.sendAllUserMessage(
                  "temp: "+ getterRegistor["temp"] + 
                  //"\ntemp2: "+ getterRegistor["temp2"] + 
                  "\ninBake: "+ getterRegistor["inBake"] + 
                  "\noutBake: "+ getterRegistor["outBake"] + 
                  "\ntempOut: "+ getterRegistor["tempOut"]);
              
            }

         }
         if(it.find("SAND_CHART") != it.end()){
            #if EXECUTOR_WINTER == true
            LOG.drawChart("Chart"+thisDay,"date",{"temp",/*"temp2",*/"inBake", "outBake","tempOut"},{"Bake","Pump","Falcon1","Falcon2","Falcon3","Falcon4","IR1","IR2","Light1"});
            #endif
            #if EXECUTOR_SUMMER == true
            LOG.drawChart("Chart"+thisDay,"date",{"temp",/*"temp2",*/"inBake", "outBake","tempOut"},{"Cooler1","Cooler2","Light1"});
            #endif
            if(isIntr)bot.sendAllUserPhoto("Chart"+thisDay+".png", thisDay+" Chart");

         }
         if(it.find("SAND_LOG") != it.end()){
            if(isIntr)bot.sendAllUserDocument("Log/Log"+thisDay+".json",thisDay + "Log");

         }
         if(it.find("SAND_STATES") != it.end()){
            std::string data = "Executor:\n";
            data = "Name | State | Auto\n";

            for(auto it : executorRegister){
               data+=it.first;
               data+=" ";
               data+=it.second ? "ON" : "OFF";
               data+=" ";
               data+=autoModeExecutorRegister[it.first]? "AUTO" : "MANUAL";
               data+="\n";
               
            }
            if(isIntr)bot.sendAllUserMessage(data);
            

         }
         
         if(it.find("SAND_COND_TREE") != it.end()){
            #if AUTOMATIZATION
               if(isIntr)bot.sendAllUserMessage(MainPattern.getTreeString());
            #else
               if(isIntr) bot.sendAllUserMessage("[main] AUTOMATIZATION is off!");
               std::cout<<"[main] AUTOMATIZATION is off!"<<std::endl;
            #endif
            

         }
         if(it.find("SAND_COND_JSON") != it.end()){
            if(isIntr)bot.sendAllUserDocument("Condition.json",thisDay + " Condition");

         }
      }
      
   }
}

void sendConditionToTgBot(const std::string& str){
   if(isIntr)bot.sendAllUserDocument(str,thisDay + " Old Condition");
}

void executorCommandTGBot(const std::string& arg){
   std::cout<<"from \033[34m[TG]\033[0m "<<arg<<std::endl;
   //std::lock_guard<std::mutex> lock(Mutex);
   for(auto it : executeCommandsMap){
      if(arg == it["TG_BOT"]) {

         if(it.find("EXECUTOR") != it.end()){
            doExecuteManual(it["EXECUTOR"]);
         }
         if(it.find("MODE") != it.end()){
            if(it["MODE"] == "auto"){
               autoModeExecutorRegister[it["MODULE"]] = true;
            }
         }
         if(it.find("READ_NEW_JSON_DATA") != it.end()){
            #if AUTOMATIZATION
               // Шаг 1: Проверка синтаксиса Condition_New.json
               if (validateJsonSyntax("Condition_New", MainPattern)) {
                  // Шаг 2: Если все правильно, продолжаем
                  // Шаг 3 и 4: Переименование и отправка
                  renameAndProcessFiles(sendConditionToTgBot);
                  std::cout << "Composite has been successfully updated." << std::endl;
               } else {
                  std::cerr << "Error: Invalid JSON syntax in Condition_New" << std::endl;
               }
               std::cout<<"Condition restored!"<<std::endl;
            #else
               if(isIntr) bot.sendAllUserMessage("[main] AUTOMATIZATION is off!");
               std::cout<<"[main] AUTOMATIZATION is off!"<<std::endl;
            #endif

         }
      }
   }
}

std::string getBushCommandOutput(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;

    // Открываем поток для выполнения команды
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("[main][getBushCommandOutput]Ошибка: не удалось выполнить команду!");
    }

    // Считываем результат команды
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    // Закрываем поток
    int returnCode = pclose(pipe);
    if (returnCode != 0) {
        throw std::runtime_error("[main][getBushCommandOutput]Команда завершилась с ошибкой!");
    }

    // Убираем лишние пробелы и символы новой строки
    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result;
}

void masterCommandsMapBot(const std::string& arg){
   std::cout<<"from \033[34m[TG]\033[0m "<<arg<<std::endl;
   //std::lock_guard<std::mutex> lock(Mutex);
   for(auto it : masterCommandsMap){
      if(arg == it["TG_BOT"]) {
     
         if(it["TG_BOT"] == "getJournal"){
            
            //std::cout<<"[main][commandMap] exists!"<<std::endl;
            std::string command = "journalctl -u greenhouse-demo.service > DebugLog.txt";
            // Выполнение команды
            int result = std::system(command.c_str());

            // Проверка результата выполнения
            if (result == 0) {
               std::cout << "[main][masterCommandsMapBot] journalctl sucsess" << std::endl;

               if(isIntr)bot.sendAllUserDocument("DebugLog.txt","journal Debug Log");
            } else {
               std::cerr << "[main][masterCommandsMapBot] journalctl err" << std::endl;
            }
         }
         else if(it["TG_BOT"] == "getIP"){
            //std::cout<<"[main][commandMap] exists!"<<std::endl;
            

            try {
               // Выполняем команду hostname -I
               std::string ipAddress = getBushCommandOutput("hostname -I");

               // Вывод результата
               if (!ipAddress.empty()) {
                     std::cout << "[main][masterCommandsMapBot] IP-адрес устройства: " << ipAddress << std::endl;
                     if(isIntr) bot.sendAllUserMessage(ipAddress);
                     
               } else {
                     std::cerr << "[main][masterCommandsMapBot] IP-адрес не найден!" << std::endl;
               }
            } catch (const std::exception& e) {
               std::cerr << "[main][masterCommandsMapBot] Ошибка: " << e.what() << std::endl;
            }
         }
         else if(it["TG_BOT"] == "getLogDir"){
            //std::cout<<"[main][commandMap] exists!"<<std::endl;
            

            try {
               
               std::string logDir = " --- Log Dir --- \n";
               for (auto i : LOG.getLogsNameList()) {
                  logDir += i;
                  logDir += '\n';
               }

               if(isIntr) bot.sendAllUserMessage(logDir);

              
            } catch (const std::exception& e) {
               std::cerr << "[main][masterCommandsMapBot] Ошибка: " << e.what() << std::endl;
            }
         }
         else if(it["TG_BOT"] == "getStatus"){
            //std::cout<<"[main][commandMap] exists!"<<std::endl;
            

            try {
               // Выполняем команду hostname -I
               std::string ipAddress = getBushCommandOutput("sudo systemctl status greenhouse-demo.service");

               // Вывод результата
               if (!ipAddress.empty()) {
                     std::cout << "[main][masterCommandsMapBot] status устройства: " << ipAddress << std::endl;
                     if(isIntr) bot.sendAllUserMessage(ipAddress);
                     
               } else {
                     std::cerr << "[main][masterCommandsMapBot] status не найден!" << std::endl;
               }
            } catch (const std::exception& e) {
               std::cerr << "[main][masterCommandsMapBot] Ошибка: " << e.what() << std::endl;
            }
         }
         
      }
   }
}



int nowSec(){
   return getDaySecondsFromTimePoint(std::chrono::system_clock::now());
}

int fromSec(int h, int m ,int s){
   return  getDaySecondsFrom3Int(h,m,s);

}


bool chackDayChanged(){
   try {
    thisHMS = std::stoi(getDayHMS());
   } catch (const std::invalid_argument& e) {
      std::cerr << "Invalid argument in stoi: getDayHMS() returned non-numeric value." << std::endl;
   } catch (const std::out_of_range& e) {
      std::cerr << "Out of range in stoi: getDayHMS() returned value too large." << std::endl;
   } catch (const std::exception& e) {
      std::cerr << "Unexpected exception in stoi: " << e.what() << std::endl;
   }
      

   if(thisHMS > 60000 && thisDay !=getDay()){   //thisDay !=getDay() for 24 h
      std::cout<<"Day Changed!"<<std::endl;
      if(isIntr){
         bot.sendAllUserMessage("Day "+thisDay+" Change!");
         #if EXECUTOR_WINTER == true
         LOG.drawChart("Chart"+thisDay,"date",{"temp",/*"temp2",*/"inBake", "outBake","tempOut"},{"Bake","Pump","Falcon1","Falcon2","Falcon3","Falcon4","IR1","IR2","Light1"});
         #endif
         #if EXECUTOR_SUMMER == true
         LOG.drawChart("Chart"+thisDay,"date",{"temp",/*"temp2",*/"inBake", "outBake","tempOut"},{"Cooler1","Cooler2","Light1"});
         #endif
         
         bot.sendAllUserPhoto("Chart"+thisDay+".png", thisDay+" Chart");
         bot.sendAllUserDocument("Log/Log"+thisDay+".json",thisDay + "Log");
      }

      thisDay = getDay();
      LOG.setFileName("Log/Log"+thisDay);
      executorReigisterLog();
      return true;
   }
   return false;
}

int main(){
  

  


   
   isIntr = isInternet();
   GetterAndLog();
   executorReigisterLog();

   std::thread logThread(&GetterAndLogLoop);

   std::thread botThread(&TelegramBot::run, &bot);

   #if AUTOMATIZATION
      MainPattern.setRoot(fromJSONRecursively(jmCond.read_json_from_file(),nullptr));
   #endif

   TurnOffAllRCM();
   if(isIntr)bot.sendAllUserMessage("Green House Lounch!");

   std::string lcdString = "";

   while(true){
#if DEBUG_LOG 
      std::cout<<"[main] ITER"<<std::endl; 
#endif
      try {
            isIntr = isInternet();
            getterRegisterUpdate();
            // Simulating code that may throw an exception
            lcdString = "T1=" + doubleToString(std::stod(getterRegistor["temp"]))+ 
                  //" T2="+ doubleToString(std::stod(getterRegistor["temp2"]))+ 
                  "\nBI="+ doubleToString(std::stod(getterRegistor["inBake"]))+ 
                  " BO="+ doubleToString(std::stod(getterRegistor["outBake"]));
    
            executor.execute("lcd",lcdString);//doubleToString
            #if AUTOMATIZATION
               MainPattern.executeAll();
            #endif
            
            chackDayChanged();
            //[*]std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        catch (const std::exception& e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
            if(isIntr)bot.alertAllUser("Total error");
            if(isIntr)bot.sendAllUserMessage(e.what());
        }

      

   }

   if (botThread.joinable()) {
        botThread.join();
    }

   if (logThread.joinable()) {
        logThread.join();
    }

   

    std::cout << "Завершение программы" << std::endl;
    return 0;

}

