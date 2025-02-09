
#include "crcCalc.h"

   
   unsigned long crcCalc::calcCRC(const std::string& data){
      return crc32(crc, reinterpret_cast<const unsigned char*>(data.c_str()), data.length());;
   }

   std::string crcCalc::calcCRCtoHex(const std::string& data){
      std::stringstream ss;
      ss << std::hex << calcCRC(data); 
      return ss.str();
   }

