#include "DataCombine.h"

#include <iostream>
#include <string>
#include <vector>




   std::string DataCombine::combineListOfLists(const std::vector<std::vector<std::string>>& strsList, const char& delimiter1, const char& delimiter2){
      std::string data;
    

      auto itStrsList = strsList.cbegin();
      while(itStrsList != strsList.end()){
         auto it = itStrsList->cbegin();

         while(it != itStrsList->cend()){
            data+= *it;
            if(it+1 != itStrsList->cend()) data+=delimiter1;
            it++;
         }
         if(itStrsList+1 != strsList.end()) data+=delimiter2;
         itStrsList++;
      }

      return data;

   }

   std::string DataCombine::combineList(std::vector<std::string> strs, const char& delimiter){
      std::string data;
    

      auto it = strs.cbegin();
      while(it != strs.end()){
         data+= *it;
         if(it+1 != strs.end()) data+=delimiter;
         it++;
      }

      return data;

   }
   std::string DataCombine::combinePair(const std::string& str1,const std::string& str2, const char& delimiter){
      std::string data;


      data = str1+delimiter+str2;

      return data;

   }
   std::string DataCombine::combinePair(std::pair<std::string,std::string> pair, const char& delimiter){
      
      return DataCombine::combinePair(pair.first,pair.second,delimiter);

   }

/*
   int main(){
      DataCombine DC;


         std::cout<<DC.combinePair(DC.combineList({DC.combineList({"a","b","c"},','),
                                             DC.combineList({"a","b","c"},','),
                                             DC.combineList({"a","b","c"},','),
                                             DC.combineList({"a","b","c"},',')},';'), "crc",'/') << std::endl;

   }

*/

