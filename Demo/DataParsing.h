
// DataParsing.h
#ifndef DATAPARSING_H
#define DATAPARSING_H


#include "TreeParsing.h"


#include <iostream>
#include <string>
#include <vector>

//g++ -o DataParsing DataParsing.cpp TreeParsing.cpp
//g++ -o DataParsing -fsanitize=address DataParsing.cpp TreeParsing.cpp




#define delimiterPairVector std::vector<std::pair<int,int>>

class DataParsing{
public:

   void setData(std::string data);
   
   TreeParsing parse(std::string data, const char* delimiters, int level);

   int getParsedPartCount();
   void calculateLevelsPartCounts();
   int parseLevel(const char delimiter,int level = -1);

   void printVectorPair() const;

   TreeParsing getParsedListHierarchical();

   std::vector<std::vector<std::string>> getParsedListLayared();
   std::string getParsedPartLayered(int index, int Layer = 0) const;

   std::vector<std::string> getParsedList() const;
   std::string getParsedPart(int index) const;

   std::string getParsedPartByIterotor(delimiterPairVector::const_iterator from,
                                       delimiterPairVector::const_iterator to) const;
   
   
int levelSize = -1;
std::string data;
delimiterPairVector delimiterIndex2;
std::vector<int> levelsPartCounts;


};

#endif //DATAPARSING_H
