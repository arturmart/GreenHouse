#include <iostream>
#include "jsonManager.h"
#include <string>

//g++ -o condition Condition.cpp jsonManager.cpp -I/usr/include/cairo -lcairo TimeManager.cpp

class ConditionNode{
   public:
   ConditionNode(std::string title):title(title){

   }


   private:
   std::string title;

};

class ConditionParseJson{
   public:
   ConditionParseJson():conditionFile("condition.json"){

   }


   void readJson(){
      json j = conditionFile.read_json_from_file();
      parseJsom(j);

      //std::cout << j.dump(4) << std::endl;
   }

   void parseJsom(json j){

      for (const auto& [key, value] : j.items()) {
        std::cout << key << ": " << value.type_name() << std::endl; // Вывод ключа и типа значения
      }
   }


   jsonManager conditionFile;

};

int main(){
   ConditionParseJson cond;
   cond.readJson();

}