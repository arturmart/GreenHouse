
#include "Executor.h"



//g++ -o test -fsanitize=address 
//g++ -o test  TestDataParsing.cpp  DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp -lwiringPi DataCombine.cpp crcCalc.cpp -lz


   Executor::Executor() : serialComm("/dev/ttyS3", 9600) {
      
   }
   int Executor::binaryStringToInt(const std::string& binaryString) {
    return std::stoi(binaryString, nullptr, 2);  // Convert using base 2
   }
   bool Executor::aboutFeedBack(std::string str){
   int bin = binaryStringToInt(str);

      if (bin & ERROR_SYNTAX)                      std::cout<<"[ER1-SY] ";
      if (bin & ERROR_1L_NO_DATA)                  std::cout<<"[ER2-1N] ";
      if (bin & ERROR_1L_TOO_MANY_DATA)            std::cout<<"[ER3-1M] ";
      if (bin & ERROR_INVALID_CRC)                 std::cout<<"[ER4-IC] ";
      if (bin & ERROR_NULL_CRC)                    std::cout<<"[ER5-NC] ";
      if (bin & ERROR_2L_NO_DATA_PACKETS)          std::cout<<"[W6-2N] ";
      if (bin & ERROR_2L_TOO_MANY_PACKETS)         std::cout<<"[ER7-2M] ";
      if (bin & ERROR_3L_WRONG_DATA_PACKETS)       std::cout<<"[ER8-3W] "; 
      if (bin & GET_KEYWORD)                       std::cout<<"[WK-KW] "; 
      std::cout<<"[WPC-"<< ((bin & PACKETS_COUNT)>>8) <<"]\n"; 

      return(! (bin & (ERROR_SYNTAX|ERROR_1L_NO_DATA|ERROR_1L_TOO_MANY_DATA|ERROR_INVALID_CRC|ERROR_NULL_CRC|ERROR_2L_TOO_MANY_PACKETS|ERROR_3L_WRONG_DATA_PACKETS)));

   }
   bool Executor::chackFeedBack(std::string str){
    TreeParsing tp = dataParser.parse(str,delimiters,3);
    //tp.print();

    //std::cout<<(CRC.calcCRCtoHex(tp.getLeaf({0})))<<"=="<<tp.getLeaf({1})<<std::endl;
    if(CRC.calcCRCtoHex(tp.getLeaf({0})).compare(tp.getLeaf({1}))){
      std::cout<<"true CRC"<<std::endl;
      return true;
    }

    std::cout<<"false CRC"<<std::endl;
    return false;
   }

   void Executor::queuePush(std::string str){

      std::string crc = CRC.calcCRCtoHex(str);
      str = DC.combinePair(str, crc,delimiters [0]);
      messageQueue.push(str);

   }
   void Executor::queuePushTurnOnAll(){

      std::string data1 = DC.combineList({DC.combineList({"83","0","1"},delimiters [2]),
                                       DC.combineList({"83","1","1"},delimiters [2]),
                                       DC.combineList({"83","2","1"},delimiters [2]),
                                       DC.combineList({"83","3","1"},delimiters [2])},delimiters [1]);
      queuePush(data1);

   }
   void Executor::queuePushTurnOffAll(){

      std::string data1 = DC.combineList({DC.combineList({"83","0","0"},delimiters [2]),
                                       DC.combineList({"83","1","0"},delimiters [2]),
                                       DC.combineList({"83","2","0"},delimiters [2]),
                                       DC.combineList({"83","3","0"},delimiters [2])},delimiters [1]);
      queuePush(data1);

   }

   void Executor::update(){
      std::string feedback;
      while (!messageQueue.empty())
      {
         feedback = serialComm.executeCommand(messageQueue.getFront());
         if ((!feedback.empty())){
            if(chackFeedBack(feedback)){
               if(aboutFeedBack(feedback)){

                  std::cout<<messageQueue.getFront()<<" feedback:"<<feedback<<std::endl;
                  //messageQueue.removeFront(); avelacnel
               }
               messageQueue.removeFront(); // rad anel
            }
            
         }
      }
   }















/*
int main(){

   for(int i = 0; i<10;i++){
      EXE.queuePushTurnOffAll();
      EXE.queuePushTurnOnAll();
   }

   while(true){
      EXE.update();
   }
}
*/
