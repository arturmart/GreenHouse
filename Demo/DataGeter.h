#ifndef DATA_GETER_H
#define DATA_GETER_H

#include <string>
#include <unordered_map>
#include "DS18B20.h"
#include "WeatherAPI.h"


class IDataGetter {//Abstrac Intrface
public:
    virtual ~IDataGetter() = default; // Виртуальный деструктор
    virtual double getData() const = 0;
    virtual bool isInited() const = 0;
    virtual void update() = 0;
    virtual double getNewData() = 0;
};

class TempStrategy : public IDataGetter {
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

class OutTempStrategy : public IDataGetter {
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
   DataGetter();
   ~DataGetter();

   double getData(const std::string& strategyStr) ;//const
   bool isInited(const std::string& strategyStr) ;//const
   void update(const std::string& strategyStr);
   double getNewData(const std::string& strategyStr);



   private:

   WeatherAPI weather;
   std::unordered_map<std::string, IDataGetter*> strategy;
};



#endif // DATA_GETER_H
