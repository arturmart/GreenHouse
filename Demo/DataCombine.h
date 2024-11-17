#ifndef DATACOMBINE_H
#define DATACOMBINE_H

#include <string>
#include <vector>

class DataCombine{
   public:

   std::string combineListOfLists(const std::vector<std::vector<std::string>>& strsList, const char& delimiter1, const char& delimiter2);
   std::string combineList(std::vector<std::string> strs, const char& delimiter);
   std::string combinePair(std::string str1,std::string str2, const char& delimiter);
   std::string combinePair(std::pair<std::string,std::string> pair, const char& delimiter);

};

#endif //DATACOMBINE_H

