// SerialComm.h
#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <string>
#include <chrono>

class SerialComm {
private:
    int serialPort;
    const char* port;
    int baudRate;

public:
    SerialComm();
    SerialComm(const char* port, int baudRate);
    ~SerialComm();

    void writeLine(const std::string& str);
    std::string readLine();
    std::string executeCommand(const std::string& str);
};

#endif // SERIALCOMM_H
