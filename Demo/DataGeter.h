#ifndef DATA_GETER_H
#define DATA_GETER_H

#include "ghconfig.h"

#include <string>
#include <unordered_map>
#include "WeatherAPI.h"

#if GH_SIMULATION == false
#include "DS18B20.h"
#else
#include "ghSim.h"
#endif




class IDataGetter {//Abstrac Intrface
public:
    virtual ~IDataGetter() = default; // Виртуальный деструктор
    virtual double getData() const = 0;
    virtual bool isInited() const = 0;
    virtual void update() = 0;
    virtual double getNewData() = 0;
};

#if GH_SIMULATION == false
class TempStrategy : public IDataGetter {//Temp Module
public:
    TempStrategy(const std::string& address);
    double getData() const override;
    bool isInited() const override;
    void update() override;
    double getNewData() override;

private:
    DS18B20 temp;
    double tempData;
};
#else
class TempStrategySim : public IDataGetter {//Temp Module
    public:
        TempStrategySim(SensorSim* sens);
        double getData() const override;
        bool isInited() const override;
        void update() override;
        double getNewData() override;
    
    private:
        SensorSim* temp;
        double tempData;
};
#endif

class OutTempStrategy : public IDataGetter {//API Temp
public:
    OutTempStrategy(WeatherAPI& weather);
    double getData() const override;
    bool isInited() const override;
    void update() override;
    double getNewData() override;

private:
    
    WeatherAPI& weather;
    std::unordered_map<std::string, std::string> dataMap;
};



class DataGetter{
   public:
   DataGetter(
    #if GH_SIMULATION == true
    HeatingSystemSim* HS
    #endif
    );
   ~DataGetter();

   double getData(const std::string& strategyStr) ;//const
   bool isInited(const std::string& strategyStr) ;//const
   void update(const std::string& strategyStr);
   double getNewData(const std::string& strategyStr);



   private:


   #if GH_SIMULATION == true
   HeatingSystemSim* HS;
   #endif
  
   WeatherAPI weather;
   std::unordered_map<std::string, IDataGetter*> strategy;
};



#endif // DATA_GETER_H
