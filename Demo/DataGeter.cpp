#include "DataGeter.h"
#include "tokens.h"


//g++ -o datageter DataGeter.cpp DS18B20.cpp WeatherAPI.cpp -lcurl 

// Temp class implementation

#if GH_SIMULATION == false
TempStrategy::TempStrategy(const std::string& address) : temp(address), tempData(-255) {}

double TempStrategy::getData() const {
    return tempData;
}
bool TempStrategy::isInited() const{
    return temp.isInited();
}
void TempStrategy::update() {
   float buf = 0;
    for(int i = 0; i < 4;i++){//filtr
      buf = temp.readTemperature();
      if(buf != -1) break;
    }
    tempData = buf;
}
double TempStrategy::getNewData(){
   update();
   return getData();
}
#else
TempStrategySim::TempStrategySim(SensorSim* sens) : temp(sens), tempData(-255) {}

double TempStrategySim::getData() const {
   
    return tempData;
}
bool TempStrategySim::isInited() const{
    return temp->isInit();
}
void TempStrategySim::update() {
    tempData = temp->getNewData();
}
double TempStrategySim::getNewData(){
   update();
   return getData();
}
#endif



// OutTemp class implementation
OutTempStrategy::OutTempStrategy(WeatherAPI& weather):weather(weather){}

double OutTempStrategy::getData() const {
    return std::stod(((weather.getWeather()) ["temp"]));
}
bool OutTempStrategy::isInited() const{
   if(weather.isInternetAvailable()){
    return weather.isInited()&&weather.isInternetAvailable();
   }
}
void OutTempStrategy::update() {
   if(weather.isInternetAvailable()){
    weather.updateWeather();
   }
}
double OutTempStrategy::getNewData(){
   if(weather.isInternetAvailable()){
      update();
      return getData();
   }
   else return(-255);
}

#if GH_SIMULATION == false
DataGetter::DataGetter():  weather("fcb989e5668460983b3cb819569b8c1d"/*TOKEN_WEATHERAPI*/, 40.059456, 44.474210)
#else
DataGetter::DataGetter(   HeatingSystemSim* HS): weather("fcb989e5668460983b3cb819569b8c1d"/*TOKEN_WEATHERAPI*/, 40.059456, 44.474210), HS(HS)
#endif
   {
      
   #if GH_SIMULATION == false
      strategy["temp"] = new TempStrategy("28-0303979402d4");
      //strategy["temp2"] = new TempStrategy("28-030397942cf4");
      strategy["inBake"] = new TempStrategy("28-030397946349");
      strategy["outBake"] = new TempStrategy("28-04175013faff");
   #else
      strategy["temp"] = new TempStrategySim(HS->getSensor1());
      //strategy["temp2"] = new TempStrategy("28-030397942cf4");
      strategy["inBake"] = new TempStrategySim(HS->getSensor2());
      strategy["outBake"] = new TempStrategySim(HS->getSensor3());
   #endif
      
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
