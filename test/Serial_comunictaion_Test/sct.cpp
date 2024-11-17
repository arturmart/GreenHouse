#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>
#include <cstring>
#include <unistd.h>


//for compile
//g++ -o sct sct.cpp -lwiringPi

class SerialComm {
private:
    int serialPort;
    const char* port;
    int baudRate;

public:
    SerialComm(const char* port, int baudRate) : port(port), baudRate(baudRate) {
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

    ~SerialComm() {
        // Закрытие порта при уничтожении объекта
        serialClose(serialPort);
    }

    void writeLine(const char* command) {
        serialPrintf(serialPort, command);  // Отправка команды
    }

    std::string readLine() {
        std::string feedback;
        while (serialDataAvail(serialPort)) {
            //std::cout<<serialDataAvail(serialPort)<<std::endl;
            char incomingByte = serialGetchar(serialPort);
            if (incomingByte == '\n' || incomingByte == '\0') {
                break;  // Выход при получении конца строки
            }
            feedback += incomingByte;  // Сохранение байта
        }
        return feedback;
    }

    std::string executeCommand(const char* command) {
        writeLine(command);  // Отправка команды
        usleep(1000000);  // Ждем 1 секунду перед ожиданием фидбека

        std::string feedback = readLine();  // Получение фидбека
        

        return feedback;
    }
};

int main(int argc, char *argv[]) {
    SerialComm serialComm("/dev/ttyS3", 9600);  // Создание объекта с нужным портом и скоростью

    std::string feedback;

    

    //std::cout << "get argv["<<argc<<"]"<<std::endl;
    //for(int i = 0; i< argc;i++) std::cout <<"["<<i<<"] "<< argv[i] << std::endl;
    //std::cout <<std::endl;


    if(argc == 1){
        //std::cout << "[W1] args is 0" << std::endl;
        std::cout << "-h for help"<<std::endl;
        feedback = serialComm.executeCommand("showall/8d984889\n");  // Выполнение команды
    }
    else if(argc == 2){
        //std::cout << "[W1] args is 1" << std::endl;
        std::string sand = std::string(argv[1]) + "\n"; 
        std::cout << sand.c_str() << std::endl;
        if(sand == "-h\n"){
            std::cout << "[Helping] sct.cpp [arg1] " << std::endl <<
             "[arg1] is argument for sending"<< std::endl <<
              "w/o [arg1] sand showall/8d984889. shows all modes of Relay controll module!"<< std::endl<<
              "returns and print of recived string!"<< std::endl;
            return 0;
        }
        else feedback = serialComm.executeCommand(sand.c_str());
    }
    else{
        std::cout << "[E1] Error! Wrong count of arg!" << std::endl;
    }

    if (!feedback.empty()) {
            std::cout << "read Data: " << feedback << std::endl;  // Вывод фидбека
        }
    else    std::cout << "[W2] No recived data!" << feedback << std::endl;  // Вывод фидбека


    return 0;
}