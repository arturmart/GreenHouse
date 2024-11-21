#include <iostream>
#include "DS18B20.h"
#include "WeatherAPI.h"


//g++ -o datageter DataGeter.cpp DS18B20.cpp WeatherAPI.cpp -lcurl -std=c++17
// sudo modprobe w1-gpio
// sudo modprobe w1-therm

class DataGeter{
   public:
   virtual double getData() const = 0;

};

class Temp: public DataGeter{
   

   public:
   Temp(const std::string& addres):temp(addres){//"28-0303979433f8"

   }
   double getData() const override {
        return temp.readTemperature();
    }

   private:
   DS18B20 temp;
};

class OutTemp: public DataGeter{
   

   public:
   OutTemp(const std::string& apiKey, double lat, double lon): weather(apiKey,lat,lon){//"28-0303979433f8"

   }
   double getData() const override {
        return std::stod(weather.getWeather({"temperature"})[0]);
    }

   private:
    WeatherAPI weather;
};

int main(){
   Temp temp("28-0303979433f8");
   OutTemp outTemp("fcb989e5668460983b3cb819569b8c1d", 40.059456, 44.474210);

   //DataGeter temp = (new Temp("28-0303979433f8"));
   //DataGeter outTemp = new OutTemp("fcb989e5668460983b3cb819569b8c1d", 40.059456, 44.474210);

   std::cout<<temp.getData()<<std::endl;
   std::cout<<outTemp.getData()<<std::endl;

}