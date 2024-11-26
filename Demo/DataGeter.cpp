#include "DataGeter.h"


//g++ -o datageter DataGeter.cpp DS18B20.cpp WeatherAPI.cpp -lcurl 

// Temp class implementation
TempStrategy::TempStrategy(const std::string& address) : temp(address), tempData(-255) {}

double TempStrategy::getData() const {
    return tempData;
}
bool TempStrategy::isInited() const{
    return temp.isInited();
}
void TempStrategy::update() {
    tempData = temp.readTemperature();
}
double TempStrategy::getNewData(){
   update();
   return getData();
}



// OutTemp class implementation
OutTempStrategy::OutTempStrategy(WeatherAPI& weather):weather(weather){}

double OutTempStrategy::getData() const {
    return std::stod(((weather.getWeather()) ["temp"]));
}
bool OutTempStrategy::isInited() const{
    return weather.isInited()&&weather.isInternetAvailable();
}
void OutTempStrategy::update() {
    weather.updateWeather();
}
double OutTempStrategy::getNewData(){
   update();
   return getData();
}


DataGetter::DataGetter(): weather("fcb989e5668460983b3cb819569b8c1d", 40.059456, 44.474210){

      strategy["temp"] = new TempStrategy("28-0303979433f8");
      
      strategy["tempOut"] = new OutTempStrategy(weather); 

   }

DataGetter::~DataGetter() {
        strategy.clear();
    }
double DataGetter::getData(const std::string& strategyStr) {//const
   return ((strategy[strategyStr])->getData());
   
}
bool DataGetter::isInited(const std::string& strategyStr) {//const
   return ((strategy[strategyStr])->isInited());
}
void DataGetter::update(const std::string& strategyStr){
   (strategy[strategyStr])->update();
}
double DataGetter::getNewData(const std::string& strategyStr){
   return ((strategy[strategyStr])->getNewData());
   
}


/*
int main() {
   DataGetter dataGetter;

   for(int i = 0; i<1000;i++){

      std::cout<<dataGetter.getNewData("temp")<<" ";
      //std::cout<<dataGetter.getNewData("outTemp")<<" ";
      std::cout<<std::endl;


      //std::cout<<dataGetter.getNewData("temp")<<" "<<dataGetter.getNewData("outTemp")<<std::endl;
   }
}
*/
