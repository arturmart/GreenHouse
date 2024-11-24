
#include "RelayControlModule.h"



//g++ -o test -fsanitize=address 
//g++ -o test  ReleControlModule.cpp  DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp -lwiringPi DataCombine.cpp crcCalc.cpp -lz


   RelayControlModule::RelayControlModule() : serialComm("/dev/ttyS3", 57600) {
      
   }
   int RelayControlModule::binaryStringToInt(const std::string& binaryString) {
    return std::stoi(binaryString, nullptr, 2);  // Convert using base 2
   }
   bool RelayControlModule::checkFeedBack(std::string str){
   int bin = binaryStringToInt(str);

      if (bin & ERROR_SYNTAX)                      std::cout<<"\033[31m[ER1-SY]\033[0m ";
      if (bin & ERROR_1L_NO_DATA)                  std::cout<<"\033[31m[ER2-1N]\033[0m ";
      if (bin & ERROR_1L_TOO_MANY_DATA)            std::cout<<"\033[31m[ER3-1M]\033[0m ";
      if (bin & ERROR_INVALID_CRC)                 std::cout<<"\033[31m[ER4-IC]\033[0m ";
      if (bin & ERROR_NULL_CRC)                    std::cout<<"\033[31m[ER5-NC]\033[0m ";
      if (bin & ERROR_2L_NO_DATA_PACKETS)          std::cout<<"\033[33m[W6-2N]\033[0m ";
      if (bin & ERROR_2L_TOO_MANY_PACKETS)         std::cout<<"\033[31m[ER7-2M]\033[0m ";
      if (bin & ERROR_3L_WRONG_DATA_PACKETS)       std::cout<<"\033[31m[ER8-3W]\033[0m "; 
      if (bin & GET_KEYWORD)                       std::cout<<"\033[33m[WK-KW]\033[0m "; 
      std::cout<<"\033[33m[WPC-"<< ((bin & PACKETS_COUNT)>>8) <<"]\033[0m\n"; 

      return(! (bin & (ERROR_SYNTAX|ERROR_1L_NO_DATA|ERROR_1L_TOO_MANY_DATA|ERROR_INVALID_CRC|ERROR_NULL_CRC|ERROR_2L_TOO_MANY_PACKETS|ERROR_3L_WRONG_DATA_PACKETS)));

   }
   bool RelayControlModule::chackCRC(std::string str){
    TreeParsing tp = dataParser.parse(str,delimiters,3);
    //tp.print();

    //std::cout<<(CRC.calcCRCtoHex(tp.getLeaf({0})))<<"=="<<tp.getLeaf({1})<<std::endl;
    if(CRC.calcCRCtoHex(tp.getLeaf({0})).compare(tp.getLeaf({1}))){
      std::cout<<"\033[32m[true CRC]\033[0m"<<std::endl;
      return true;
    }

    std::cout<<"\033[31m[false CRC]\033[0m"<<std::endl;
    return false;
   }

   void RelayControlModule::queuePush(std::string str){

      std::string crc = CRC.calcCRCtoHex(str);
      str = DC.combinePair(str, crc,delimiters [0]);
      messageQueue.push(str);

   }
   void RelayControlModule::queuePushTurnOnAll(){

      std::string data1 = DC.combineList({DC.combineList({"83","0","1"},delimiters [2]),
                                       DC.combineList({"83","1","1"},delimiters [2]),
                                       DC.combineList({"83","2","1"},delimiters [2]),
                                       DC.combineList({"83","3","1"},delimiters [2])},delimiters [1]);
      queuePush(data1);

   }
   void RelayControlModule::queuePushTurnOffAll(){

      std::string data1 = DC.combineList({DC.combineList({"83","0","0"},delimiters [2]),
                                       DC.combineList({"83","1","0"},delimiters [2]),
                                       DC.combineList({"83","2","0"},delimiters [2]),
                                       DC.combineList({"83","3","0"},delimiters [2])},delimiters [1]);
      queuePush(data1);

   }

   void RelayControlModule::update(){
      std::string feedback;
      while (!messageQueue.empty())
      {
         sandCommand();
      }
   }

    void RelayControlModule::sandCommand() { 
    std::string feedback;
    
    for (int i = 0; i < 5; i++) {
        messageQueue.printAll();
        feedback = serialComm.executeCommand(messageQueue.getFront());
        
        if (!feedback.empty()) {
            if (chackCRC(feedback)) {  
                if (checkFeedBack(feedback)) {
                    std::cout << "rp(" << i << ") " << messageQueue.getFront() << " feedback: " << feedback << std::endl;
                    messageQueue.removeFront();  // Удаление команды из очереди
                    std::cout << std::endl;
                    return;
                } else {
                    std::cout << "\033[31m[WD Reapit]\033[0m " << std::endl << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Задержка между попытками
                }
            } else {
                std::cerr << "\033[31m[CRC Error]\033[0m Invalid CRC for feedback: " << feedback << std::endl;
                continue;  // Прерывание при ошибке CRC
            }
        } else {
            std::cerr << "\033[31m[Empty Feedback]\033[0m No feedback received!" << std::endl;
            continue;  // Прерывание, если нет ответа
        }
    }

    // Если не удалось получить правильный ответ после 5 попыток
    std::cerr << "\033[31m(DCM don't get Data)\033[0m Failed to get valid feedback after 5 attempts." << std::endl;
    // Можно добавить дополнительные действия, например, убрать команду из очереди, если необходимо:
    // messageQueue.removeFront();
   }

    bool RelayControlModule::sandInited() {
      
      
      
      std::string feedback = serialComm.executeCommand(DC.combinePair("inited", CRC.calcCRCtoHex("inited"),delimiters [0]));

      if ((!feedback.empty())){
         TreeParsing tp = dataParser.parse(feedback,delimiters,3);
         //tp.print();

         //std::cout<<(CRC.calcCRCtoHex(tp.getLeaf({0})))<<"=="<<tp.getLeaf({1})<<std::endl;
         if(CRC.calcCRCtoHex(tp.getLeaf({0})).compare(tp.getLeaf({1}))){
            
            if(tp.getLeaf({0}).compare("dcmIsInited") == 0){
               
               return true;
               
            }
         }
      }
      return false;
    }

   













/*
ReleControlModule EXE;

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

