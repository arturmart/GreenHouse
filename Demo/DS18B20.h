#ifndef DS18B20_H
#define DS18B20_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>

const std::string basePath = "/sys/bus/w1/devices/";

class DS18B20{
   public:
   DS18B20(std::string sensorID);
   float readTemperature();

   private:
   std::string sensorID;

};

#endif