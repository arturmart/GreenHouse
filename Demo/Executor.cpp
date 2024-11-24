#include <iostream>
#include <string>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <mutex>  // Для использования мьютекса

#include "I2CLCD.h"
#include "RelayControlModule.h"
//g++ -o executor Executor.cpp -pthread I2CLCD.cpp -lwiringPi  RelayControlModule.cpp  DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp -lwiringPi DataCombine.cpp crcCalc.cpp -lz

// Интерфейс для выполнения команд
class IExecutor {
public:
    virtual ~IExecutor() = default;
    virtual bool isInited() = 0;
    virtual void execute(const std::string& arg="") = 0;
};

// Стратегия для работы с LCD
class I2C_LCD_Strategy : public IExecutor {
public:
    I2C_LCD_Strategy(I2CLCD& lcd) : lcd(lcd) {
     
        if (!isInited()) {
            std::cerr << "Ошибка инициализации LCD!" << std::endl;
        }
    }

    bool isInited() override {
        return lcd.initialize(); // Проверка инициализации
    }

    void execute(const std::string& arg="") override {
      //   if (!isInited()) {
      //       std::cerr << "LCD не инициализирован!" << std::endl;
      //       return;
      //   }
        lcd.clear();

        std::size_t found = arg.find('\n');

        if (found==std::string::npos){
          lcd.displayString(arg.c_str(), 1);
        }
        else{
         lcd.displayString(arg.substr(0,found).c_str(),1);
         lcd.displayString(arg.substr(found+1).c_str(),2);
        }
        
        
    }

private:

    I2CLCD& lcd;
};


class RCM_Strategy: public IExecutor{
   public:
   RCM_Strategy(RelayControlModule& rcm,const std::string& cmd) : RCM(rcm), command(cmd){
    //   if (!isInited()) {
    //         std::cerr << "Ошибка инициализации RCM!" << std::endl;
    //   }
   }
   void execute(const std::string& arg="") override {
      std::lock_guard<std::mutex> lock(rcmMutex);
      RCM.queuePush(command);
      RCM.update();

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }
   bool isInited() override {
        return RCM.sandInited();
   }

   private:

   std::string command;
   RelayControlModule& RCM;
   static std::mutex rcmMutex;

};

std::mutex RCM_Strategy::rcmMutex;

class RCM_Delay_Strategy: public IExecutor{
   public:
   RCM_Delay_Strategy(RelayControlModule& rcm,const std::string& cmd, int delaySeconds) : 
   RCM(rcm), command(cmd), delay(delaySeconds){
    //   if (!isInited()) {
    //         std::cerr << "Ошибка инициализации RCM!" << std::endl;
    //   }
   }
   void execute(const std::string& arg="") override {

       std::thread t(&RCM_Delay_Strategy::executeCommands, this);
       t.detach(); 
   }

   
   bool isInited() override {
        return RCM.sandInited();
   }

   private:

   void executeCommands() {
       

        
        std::this_thread::sleep_for(std::chrono::seconds(delay)); // Задержка
        
        std::lock_guard<std::mutex> lock(rcmMutex);
        RCM.queuePush(command); // Вторая команда
        RCM.update();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //usleep(100);
    }

   int delay;
   std::string command;

   RelayControlModule& RCM;

   static std::mutex rcmMutex;
};
std::mutex RCM_Delay_Strategy::rcmMutex;

// Класс-исполнитель для управления различными стратегиями
class Executor {
public:
    Executor() : lcd(0x3f), RCM(){ // Инициализация дисплея с адресом 0x3f
        // Добавление стратегий для различных линий
        executeMap["lcd"] = new I2C_LCD_Strategy(lcd);
        executeMap["R1_ON"] = new RCM_Strategy(RCM,"83,0,1");
        executeMap["R1_OFF"] = new RCM_Strategy(RCM,"83,0,0");
        executeMap["R2_ON"] = new RCM_Strategy(RCM,"83,1,1");
        executeMap["R2_OFF"] = new RCM_Strategy(RCM,"83,1,0");
        executeMap["R3_ON"] = new RCM_Strategy(RCM,"83,2,1");
        executeMap["R3_OFF"] = new RCM_Strategy(RCM,"83,2,0");
        executeMap["R4_ON"] = new RCM_Strategy(RCM,"83,3,1");
        executeMap["R4_OFF"] = new RCM_Strategy(RCM,"83,3,0");

        executeMap["R1_OFF_5s"] = new RCM_Delay_Strategy(RCM,"83,0,0",5);
        executeMap["R2_OFF_5s"] = new RCM_Delay_Strategy(RCM,"83,1,0",5);
        executeMap["R3_OFF_5s"] = new RCM_Delay_Strategy(RCM,"83,2,0",5);
        executeMap["R4_OFF_5s"] = new RCM_Delay_Strategy(RCM,"83,2,0",5);
        
    }

    ~Executor() {
        for (auto& pair : executeMap) {
            delete pair.second; // Очистка выделенной памяти
        }
    }

    void execute(const std::string& strategyStr, const std::string& arg = "") {
        if (executeMap.find(strategyStr) != executeMap.end()) {
            executeMap[strategyStr]->execute(arg);
        } else {
            std::cerr << "Стратегия \"" << strategyStr << "\" не найдена." << std::endl;
        }
    }
  

private:
    std::unordered_map<std::string, IExecutor*> executeMap;
    I2CLCD lcd; // Объект I2CLCD
    RelayControlModule RCM;

    
};


int main() {
    Executor exe;

    for(int i = 0;i<10;i++){

        exe.execute("R1_ON");
        exe.execute("R1_OFF_5s");
        exe.execute("R2_ON");
        exe.execute("R2_OFF_5s");
        exe.execute("R3_ON");
        exe.execute("R3_OFF_5s");
        exe.execute("R4_ON");
        exe.execute("R4_OFF_5s");
    }

    // Выполнение стратегий

    /*
    exe.execute("lcd", "Barev \nApper");
    exe.execute("R1_ON");
    exe.execute("lcd", "Hajox \nApper");
    exe.execute("R1_OFF");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    exe.execute("R1_ON");
    exe.execute("R1_OFF_5s");
    exe.execute("R2_ON");
    exe.execute("R2_OFF_5s");
    exe.execute("R3_ON");
    exe.execute("R3_OFF_5s");


    for(int i = 0; i<10;i++){
    exe.execute("R4_ON");
    exe.execute("R4_OFF");}
    */

   
    

    
    
    


    std::this_thread::sleep_for(std::chrono::seconds(10)); // Задержка


    return 0;
}
