#ifndef CRCCALC_H
#define CRCCALC_H

#include <zlib.h>

#include <string>

#include <sstream>
#include <iomanip>  // For std::hex, std::setw, std::setfill

class crcCalc{
   public:
   unsigned long calcCRC(const std::string& data);
   std::string calcCRCtoHex(const std::string& data);
   private:
   unsigned long crc = crc32(0L, Z_NULL, 0);
};

#endif //CRCCALC_H