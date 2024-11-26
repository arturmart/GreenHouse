#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <thread>

#include "I2CLCD.h"
#include "RelayControlModule.h"

extern std::mutex rcmMutex; 

// Интерфейс для выполнения команд
class IExecutor {
public:
    virtual ~IExecutor() = default;
    virtual bool isInited() = 0;
    virtual void execute(const std::string& arg = "") = 0;
};

// Стратегия для работы с LCD
class I2C_LCD_Strategy : public IExecutor {
public:
    I2C_LCD_Strategy(I2CLCD& lcd);
    bool isInited() override;
    void execute(const std::string& arg = "") override;

private:
    I2CLCD& lcd;
};


// Стратегия для работы с Relay Control Module
class RCM_Strategy : public IExecutor {
public:
    RCM_Strategy(RelayControlModule& rcm, const std::string& cmd);
    void execute(const std::string& arg = "") override;
    bool isInited() override;

private:
    std::string command;
    RelayControlModule& RCM;
};

// Стратегия для работы с задержкой в Relay Control Module
class RCM_Delay_Strategy : public IExecutor {
public:
    RCM_Delay_Strategy(RelayControlModule& rcm, const std::string& cmd, int delaySeconds);
    void execute(const std::string& arg = "") override;
    bool isInited() override;

private:
    void executeCommands();
    int delay;
    std::string command;
    RelayControlModule& RCM;
};

// Класс-исполнитель для управления различными стратегиями
class Executor {
public:
    Executor();
    ~Executor();

    void execute(const std::string& strategyStr, const std::string& arg = "");

private:
    std::unordered_map<std::string, IExecutor*> executeMap;
    I2CLCD lcd; // Объект I2CLCD
    RelayControlModule RCM;
};

#endif // EXECUTOR_H