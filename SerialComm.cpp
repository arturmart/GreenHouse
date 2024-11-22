// SerialComm.cpp
#include "SerialComm.h"
#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>
#include <unistd.h>
#include <thread>

#define WAIT_UNTIL_SEC 5

SerialComm::SerialComm(){
    
}

SerialComm::SerialComm(const char* port, int baudRate) : port(port), baudRate(baudRate) {
    // Инициализация WiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "Ошибка инициализации WiringPi!" << std::endl;
        exit(1);
    }

    // Открытие последовательного порта
    serialPort = serialOpen(port, baudRate);
    if (serialPort < 0) {
        std::cerr << "Не удалось открыть последовательный порт!" << std::endl;
        exit(1);
    }
}

SerialComm::~SerialComm() {
    serialClose(serialPort);  // Закрытие порта при уничтожении объекта
}

void SerialComm::writeLine(const std::string& str) {
    serialPrintf(serialPort, str.c_str());  // Отправка команды
}

std::string SerialComm::readLine() {
    std::string read;
    while (serialDataAvail(serialPort)) {
        char incomingByte = serialGetchar(serialPort);
        if (incomingByte == '\n' || incomingByte == '\0') {
            break;  // Выход при получении конца строки
        }
        read += incomingByte;  // Сохранение байта
    }
    return read;
}

std::string SerialComm::executeCommand(const std::string& str) {
    auto start = std::chrono::system_clock::now();
    writeLine(str);

    std::string read;
    while (true) {
        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

        std::this_thread::sleep_for(std::chrono::microseconds(50000));  // Задержка на 500 мс
        read = readLine();

        if (elapsed >= WAIT_UNTIL_SEC || !read.empty()) {
            break;
        }
    }
    return read;
}


// g++ -o SerialComm SerialComm.cpp -lwiringPi

/*
int main(int argc, char* argv[]) {
    SerialComm serialComm("/dev/ttyS3", 9600);  // Создание объекта с нужным портом и скоростью

    std::string feedback;
    if (argc == 1) {
        std::cout << "-h for help" << std::endl;
        feedback = serialComm.executeCommand("showall/8d984889\n");  // Выполнение команды
    }
    else if (argc == 2) {
        std::string command = std::string(argv[1]) + "\n";
        std::cout << command << std::endl;

        if (command == "-h\n") {
            std::cout << "[Helping] Usage: sct [arg1]\n"
                      << "[arg1]: Argument to send.\n"
                      << "Without [arg1], sends showall/8d984889 to show all modes of Relay control module!\n"
                      << "Prints received response.\n";
            return 0;
        }
        else {
            feedback = serialComm.executeCommand(command.c_str());
        }
    }
    else {
        std::cerr << "[E1] Error! Wrong count of arguments!" << std::endl;
    }

    if (!feedback.empty()) {
        std::cout << "Received Data: " << feedback << std::endl;
    }
    else {
        std::cout << "[W2] No received data!" << std::endl;
    }

    return 0;
}
*/
