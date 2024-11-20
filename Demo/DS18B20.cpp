//also Using WiringPi/WiringOP with Custom GPIO Control
//g++ -o DS18B20 DS18B20.cpp

/*
Pins: vin-Out5v
      gnd-gndi
      DQ-pd22/pwm0

connection test
   sudo modprobe w1-gpio
   sudo modprobe w1-therm
   lsmod | grep w1
   ls /sys/bus/w1/devices/

*/

#include "DS18B20.h"



   DS18B20::DS18B20(std::string sensorID):sensorID(sensorID){
   }

   float DS18B20::readTemperature() {

   std::string sensorPath = basePath + sensorID;

    std::ifstream file(sensorPath + "/w1_slave");
    if (!file.is_open()) {
        std::cerr << "Failed to open sensor file." << std::endl;
        return -1;
    }

    std::string line1, line2;
    std::getline(file, line1);
    std::getline(file, line2);
    file.close();

    // Check CRC
    if (line1.find("YES") == std::string::npos) {
        std::cerr << "CRC check failed." << std::endl;
        return -1;
    }

    // Find temperature
    size_t pos = line2.find("t=");
    if (pos == std::string::npos) {
        std::cerr << "Failed to find temperature data." << std::endl;
        return -1;
    }

    std::string tempStr = line2.substr(pos + 2);
    float temperature = std::stof(tempStr) / 1000.0;
    return temperature;
}


/*
int main() {

   DS18B20 temp1("28-0303979433f8");
    

    while (true) {
        float temperature = temp1.readTemperature();
        if (temperature != -1) {
            std::cout << "Temperature: " << temperature << "°C" << std::endl;
        }
        sleep(1); // Update every second
    }

    return 0;
}
*/