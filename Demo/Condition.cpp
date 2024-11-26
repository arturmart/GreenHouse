#include <iostream>
#include "jsonManager.h"
#include <string>

//g++ -o condition Condition.cpp jsonManager.cpp -I/usr/include/cairo -lcairo TimeManager.cpp



#include <functional>

using comparisonFunc = std::function<bool(const std::vector<double> &)> ;
class Comparison{
   
   public:
   Comparison(){
      comparisonMap[">"] = [this](const std::vector<double>& args) { return this->isGreater(args[0], args[1]); };
      comparisonMap["<"] = [this](const std::vector<double>& args) { return this->isLess(args[0], args[1]); };
      comparisonMap[">="] = [this](const std::vector<double>& args) { return this->isGreaterEqual(args[0], args[1]); };
      comparisonMap["<="] = [this](const std::vector<double>& args) { return this->isLessEqual(args[0], args[1]); };
      comparisonMap["=="] = [this](const std::vector<double>& args) { return this->isEqual(args[0], args[1]); };
      comparisonMap["!="] = [this](const std::vector<double>& args) { return this->isNotEqual(args[0], args[1]); };
      comparisonMap["on"] = [this](const std::vector<double>& args) { return this->isOn(args[0], args[1], args[2]); };

   }
   
   std::unordered_map<std::string, comparisonFunc> comparisonMap;

   private:
      bool isEqual (const double& first,const double& second) const{//==
         return first==second;
      }
      bool isOn (const double& data,const double& first,const double& second) const{//On 1 < X < 2
         return (data>=first && data<=second);
      }
      bool isNotEqual (const double& first,const double& second) const{//!=
         return first!=second;
      }
      bool isGreater(const double& first,const double& second) const{//>
         return first>second;
      }
      bool isLess(const double& first,const double& second) const{//<
         return first<second;
      }
      bool isGreaterEqual(const double& first,const double& second) const{//>=
         return first>=second;
      }
      bool isLessEqual(const double& first,const double& second) const{//<=
         return first<=second;
      }

};










class ConditionNode{
   public:
   ConditionNode(std::string title):title(title){

   }

   void setComparisons(const std::vector<std::vector<std::string>>& arr){

   }


   private:
   std::vector<comparisonFunc> comparisons;
   std::string title;

};

class ConditionParseJson{
   public:
   ConditionParseJson():conditionFile("condition.json"){

   }


   void readJson(){
      json j = conditionFile.read_json_from_file();
      parseJson(j);

      //std::cout << j.dump(4) << std::endl;
   }

   void parseJson(json j){

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