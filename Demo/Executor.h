#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <thread>

#include "ghconfig.h"

#if GH_SIMULATION == false
#include "I2CLCD.h"
#include "RelayControlModule.h"

#else
#include"ghSim.h"

#endif

extern std::mutex rcmMutex; 

// Интерфейс для выполнения команд
class IExecutor {
public:
    virtual ~IExecutor() = default;
    virtual bool isInited() = 0;
    virtual void execute(const std::string& arg = "") = 0;
};

// Стратегия для работы с LCD

#if GH_SIMULATION == false
class I2C_LCD_Strategy : public IExecutor {
    public:
        I2C_LCD_Strategy(I2CLCD& lcd);

        bool isInited() override;
        void execute(const std::string& arg = "") override;
    
    private:
        I2CLCD& lcd;
};
#else

class I2C_LCD_Strategy : public IExecutor {
    public:
        I2C_LCD_Strategy(I2cLcdSim* lcd);

        bool isInited() override;
        void execute(const std::string& arg = "") override;
    
    private:
        I2cLcdSim* lcd;
};
#endif



// Стратегия для работы с Relay Control Module
#if GH_SIMULATION == false
class RCM_Strategy : public IExecutor {
    public:
        RCM_Strategy(RelayControlModule& rcm,const std::string& cmd);
        void execute(const std::string& arg = "") override;
        bool isInited() override;
    
    private:
        std::string command;
        RelayControlModule& RCM;
};
#else

class RCM_Strategy : public IExecutor {
    public:
        RCM_Strategy(RelaySim* rcm,const std::string& cmd);
        void execute(const std::string& arg = "") override;
        bool isInited() override;
    
    private:
        std::string command;
        RelaySim* RCM;
};
#endif

// Стратегия для работы с задержкой в Relay Control Module
/*
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
*/
// Класс-исполнитель для управления различными стратегиями
class Executor {
public:
    
#if GH_SIMULATION == false
Executor();
#else
Executor(HeatingSystemSim* sim);
#endif

    ~Executor();

    void execute(const std::string& strategyStr, const std::string& arg = "");

private:
    std::unordered_map<std::string, IExecutor*> executeMap;
    
#if GH_SIMULATION == false
    RelayControlModule RCM;
    I2CLCD lcd; // Объект I2CLCD
#else
    HeatingSystemSim* sim;
#endif
};


#endif // EXECUTOR_H
