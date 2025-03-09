#include "Executor.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>


//g++ -o executor Executor.cpp -pthread I2CLCD.cpp -lwiringPi  RelayControlModule.cpp  DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp -lwiringPi DataCombine.cpp crcCalc.cpp -lz
// Стратегия для работы с LCD

std::mutex rcmMutex;


#if GH_SIMULATION == false
I2C_LCD_Strategy::I2C_LCD_Strategy(I2CLCD& lcd) : lcd(lcd) {
    if (!isInited()) {
        std::cerr << "Ошибка инициализации LCD!" << std::endl;
    }
}

bool I2C_LCD_Strategy::isInited() {
    return lcd.initialize(); // Проверка инициализации
}

void I2C_LCD_Strategy::execute(const std::string& arg) {
    lcd.clear();

    std::size_t found = arg.find('\n');

    if (found == std::string::npos) {
        lcd.displayString(arg.c_str(), 1);
    } else {
        lcd.displayString(arg.substr(0, found).c_str(), 1);
        lcd.displayString(arg.substr(found + 1).c_str(), 2);
    }
}
#else
I2C_LCD_Strategy::I2C_LCD_Strategy(I2cLcdSim* lcd) : lcd(lcd) {
    
}

bool I2C_LCD_Strategy::isInited() {
    return true; 
}

void I2C_LCD_Strategy::execute(const std::string& arg) {
    
    lcd->execute(arg);
}
#endif

// Стратегия для работы с Relay Control Module
#if GH_SIMULATION == false
RCM_Strategy::RCM_Strategy(RelayControlModule& rcm, const std::string& cmd)
    : RCM(rcm), command(cmd) {}

void RCM_Strategy::execute(const std::string& arg) {
    rcmMutex.lock();
    std::cout << "Lock" << std::endl;
    RCM.queuePush(command);
    RCM.update();
    rcmMutex.unlock();
    std::cout << "UnLock" << std::endl;
}

bool RCM_Strategy::isInited() {
    return RCM.sandInited();
}
#else
RCM_Strategy::RCM_Strategy(RelaySim* rcm, const std::string& cmd)
    : RCM(rcm), command(cmd) {}

void RCM_Strategy::execute(const std::string& arg) {

    RCM->execute(4);

}

bool RCM_Strategy::isInited() {
    return true;
}
#endif

// Стратегия с задержкой для работы с Relay Control Module
/*
RCM_Delay_Strategy::RCM_Delay_Strategy(RelayControlModule& rcm, const std::string& cmd, int delaySeconds)
    : RCM(rcm), command(cmd), delay(delaySeconds) {}

void RCM_Delay_Strategy::execute(const std::string& arg) {
    std::thread t(&RCM_Delay_Strategy::executeCommands, this);
    t.detach(); // Запуск в отдельном потоке
}

bool RCM_Delay_Strategy::isInited() {
    return RCM.sandInited();
}

void RCM_Delay_Strategy::executeCommands() {
    std::this_thread::sleep_for(std::chrono::seconds(delay)); // Задержка

    rcmMutex.lock();
    std::cout << "Lock" << std::endl;
    RCM.queuePush(command);
    RCM.update();
    rcmMutex.unlock();
    std::cout << "UnLock" << std::endl;
}
*/

// Класс-исполнитель для управления различными стратегиями
#if GH_SIMULATION == false
Executor::Executor():lcd(0x3f), RCM() 
#else
Executor::Executor(    HeatingSystemSim* sim): sim(sim)
#endif
{


#if GH_SIMULATION == true
#define lcd sim->getLCD()
#define RCM sim->getRelay()
#endif
                
    // Добавление стратегий для различных команд
    executeMap["lcd"] = new I2C_LCD_Strategy(lcd);

    

    #if EXECUTOR_WINTER == true                         
    executeMap["Bake_ON"] = new RCM_Strategy(RCM, "83,0,1");
    executeMap["Bake_OFF"] = new RCM_Strategy(RCM, "83,0,0");
    executeMap["Pump_ON"] = new RCM_Strategy(RCM, "83,1,1");
    executeMap["Pump_OFF"] = new RCM_Strategy(RCM, "83,1,0");
    executeMap["Falcon1_ON"] = new RCM_Strategy(RCM, "83,2,1");
    executeMap["Falcon1_OFF"] = new RCM_Strategy(RCM, "83,2,0");
    executeMap["Falcon2_ON"] = new RCM_Strategy(RCM, "83,3,1");
    executeMap["Falcon2_OFF"] = new RCM_Strategy(RCM, "83,3,0");
    executeMap["Falcon3_ON"] = new RCM_Strategy(RCM, "83,4,1");
    executeMap["Falcon3_OFF"] = new RCM_Strategy(RCM, "83,4,0");
    executeMap["Falcon4_ON"] = new RCM_Strategy(RCM, "83,5,1");
    executeMap["Falcon4_OFF"] = new RCM_Strategy(RCM, "83,5,0");

    executeMap["IR1_ON"] = new RCM_Strategy(RCM, "83,6,1");
    executeMap["IR1_OFF"] = new RCM_Strategy(RCM, "83,6,0");
    executeMap["IR2_ON"] = new RCM_Strategy(RCM, "83,7,1");
    executeMap["IR2_OFF"] = new RCM_Strategy(RCM, "83,7,0");
    #endif

    #if EXECUTOR_SUMMER == true                         

    executeMap["Cooler1_ON"] = new RCM_Strategy(RCM, "83,1,1");
    executeMap["Cooler1_OFF"] = new RCM_Strategy(RCM, "83,1,0");
    executeMap["Cooler2_ON"] = new RCM_Strategy(RCM, "83,0,1");
    executeMap["Cooler2_OFF"] = new RCM_Strategy(RCM, "83,0,0");

    #endif

    /*
    Bake 0
    pump 1
    Falcon1 2
    Falcon2 3
    Falcon3 4
    Falcon4 5
    IR1 6
    IR2 7
    Light 8
    */


    executeMap["Light1_ON"] = new RCM_Strategy(RCM, "83,8,1");
    executeMap["Light1_OFF"] = new RCM_Strategy(RCM, "83,8,0");

  

    //executeMap["R1_OFF_5s"] = new RCM_Delay_Strategy(RCM, "83,0,0", 5);
    //executeMap["R2_OFF_5s"] = new RCM_Delay_Strategy(RCM, "83,1,0", 5);
    //executeMap["R3_OFF_5s"] = new RCM_Delay_Strategy(RCM, "83,2,0", 5);
    //executeMap["R4_OFF_5s"] = new RCM_Delay_Strategy(RCM, "83,3,0", 5);
}

Executor::~Executor() {
    // Очистка выделенной памяти для стратегий
    for (auto& pair : executeMap) {
        delete pair.second;
    }
}

void Executor::execute(const std::string& strategyStr, const std::string& arg) {
    if (executeMap.find(strategyStr) != executeMap.end()) {
        executeMap[strategyStr]->execute(arg);
    } else {
        std::cerr << "Стратегия \"" << strategyStr << "\" не найдена." << std::endl;
    }
}

/*
int main() {
    // Создание объекта Executor
    Executor executor;

    // Проверяем выполнение стратегий
    std::cout << "Тест стратегии LCD" << std::endl;
    executor.execute("lcd", "Привет, мир!\nArduino");

    std::cout << "\nТест включения реле R1" << std::endl;
    executor.execute("R1_ON");

    std::cout << "\nТест выключения реле R1 с задержкой 5 секунд" << std::endl;
    executor.execute("R1_OFF_5s");

    // Пауза для завершения потока с задержкой
    std::this_thread::sleep_for(std::chrono::seconds(6));

    std::cout << "Завершение программы" << std::endl;
    return 0;
}*/
