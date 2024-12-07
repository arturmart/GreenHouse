#include "RelayControlModule.h"
#include <iostream>
#include <string>

//g++ -o test testRCM.cpp  RelayControlModule.cpp  DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp -lwiringPi DataCombine.cpp crcCalc.cpp -lz


RelayControlModule EXE;

int main(){

 

   std::string input;

   while(true){
      std::cout<<"sand Message:";
      std::cin>>input;

      EXE.queuePush(input);
      


      EXE.update();
   }
}
