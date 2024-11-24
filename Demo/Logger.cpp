#include "jsonManager.h"
#include "ChartDrawing.h"
#include <iostream>
#include <string>
#include <unordered_map>

//#include "DS18B20.h"
//DS18B20.cpp
//g++ -o logger Logger.cpp jsonManager.cpp -I/usr/include/cairo -lcairo TimeManager.cpp DS18B20.cpp

class Logger{
   public:

   Logger(const std::string& filename,const std::vector<std::string>& table= {}):filename(filename),LogJson(filename+".json"),logType(table){
      setTable(table);
   }

   void addTable(const std::string& str){
      dataTable[str] = "";
   }
   void setTable(const std::vector<std::string>& table){
      dataTable.clear();
      for(std::string str : table){
         dataTable[str] = "";
      }
   }

   void setData(const std::string& key, const std::string& data){
      dataTable[key] = data;
   }

   void log(){
      //json j = LogJson.read_json_from_file();

      json newDict = json::object();

      for(const auto& pair : dataTable){
         if(pair.second != ""){
            //json localDict = {pair.first,pair.second};
            newDict[pair.first] = pair.second;
            dataTable[pair.first] = "";
         }
      }

      if (LogJson.appendToJsonArray(newDict)) {
        //std::cout << "Элемент успешно добавлен в JSON!" << std::endl;
      } else {
         std::cerr << "Не удалось добавить элемент." << std::endl;
      }


      //j.push_back(newDict);
      //LogJson.write_json_to_file(j);
   }


   void drawChart(){
      std::vector<std::unordered_map<std::string,std::string>> dt = getLogElements();
      //printLogElements(dt);
      ChartDrawing chartDrawing(dt);
      
      chartDrawing.createImageFromJSON("chart");
   }

   void printLogElements(const std::vector<std::unordered_map<std::string, std::string>>& dataMap){
      for(auto dataLine:dataMap){
         for (auto tp : dataLine) {
            std::cout<<tp.first<<":"<<tp.second<<" ";
        }
        std::cout<<std::endl;
      }
   }
   std::vector<std::unordered_map<std::string, std::string>> getLogElements() {
    json root = LogJson.read_json_from_file();
    std::vector<std::unordered_map<std::string, std::string>> dataMap;

    int counter = 0;
    for (auto point : root) {
        std::unordered_map<std::string, std::string> dataLine;
        for (auto tp : logType) {
            dataLine[tp] = point[tp].get<std::string>();
        }
        dataMap.push_back(dataLine);
        counter++;
    }

    return dataMap;  // Возвращаем по значению
   }



   
   
   jsonManager LogJson;
   std::vector<std::string> logType;//{"data","temp"}
   private:
   std::string filename;
   std::unordered_map<std::string, std::string> dataTable;
};


#include "TimeManager.h"




//g++ -o logger Logger.cpp jsonManager.cpp -I/usr/include/cairo -lcairo TimeManager.cpp DS18B20.cpp
int main(){
   Logger LOG("Log2",{"data","temp","tempOut"});
   LOG.drawChart();

   /*
   //DS18B20 temp1("28-0303979433f8");

   while (true) {
        float temperature = temp1.readTemperature();
        if (temperature != -1) {

            
            LOG.setData("temp", std::to_string(temperature));

            LOG.setData("data", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())) );
            LOG.log();
            //std::cout << "time: " << unixTimestampToTimeString(to_unix_timestamp(std::chrono::system_clock::now())) << ", Temp: " << temperature << std::endl;
            LOG.LogJson.createImageFromJSON("Log1.json","qaq.png","Real Time Temperature","data","temp");
        }
        usleep(100000); // Update every second
    }
    */
    
   




   /*
   for(int i = 0; i<100;i++){
   LOG.setData("data", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())+i));
   LOG.setData("temp", std::to_string(i/2+i%10));
   LOG.log();
   }

   LOG.LogJson.createImageFromJSON("Log1.json","qaq.png","maq","data","temp");
   */

   



   
}