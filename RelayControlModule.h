
#ifndef RELAYCONTROLMODULE_H
#define RELAYCONTROLMODULE_H


#include "DataParsing.h"
#include "MessageQueuing.h"
#include "SerialComm.h"
#include "DataCombine.h"
#include "crcCalc.h"





#include <iostream>
#include <string>
#include <thread>


//g++ -o test -fsanitize=address 
//g++ -o test  TestDataParsing.cpp  DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp -lwiringPi DataCombine.cpp crcCalc.cpp -lz


const char delimiters [] = {'/',';',','};

const int ERROR_NONE = 0;               
const int ERROR_SYNTAX = 1 << 0;       
const int ERROR_1L_NO_DATA = 1 << 1;   
const int ERROR_1L_TOO_MANY_DATA = 1 << 2;   
const int ERROR_INVALID_CRC = 1 << 3; 
const int ERROR_NULL_CRC = 1 << 4;   
const int ERROR_2L_NO_DATA_PACKETS = 1 << 5; // error chi
const int ERROR_2L_TOO_MANY_PACKETS = 1 << 6; 
const int ERROR_3L_WRONG_DATA_PACKETS = 1 << 7; 
const int PACKETS_COUNT = 1111 << 8;
const int GET_KEYWORD = 1 << 12;

class RelayControlModule{
   public:
   RelayControlModule();
   int binaryStringToInt(const std::string& binaryString);
   bool checkFeedBack(std::string str);
   bool chackCRC(std::string str);
   bool isInited();

   bool hotKayInited();

   void queuePush(std::string str);
   void queuePushTurnOnAll();
   void queuePushTurnOffAll();

   void update();
   void sandCommand();
   bool sandInited();

   private:
   DataParsing dataParser;
   MessageQueuing messageQueue;
   SerialComm serialComm;
   DataCombine DC;
   crcCalc CRC;




};

#endif //RELAYCONTROLMODULE_H
