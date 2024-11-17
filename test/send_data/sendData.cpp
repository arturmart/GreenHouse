#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

int main() {
    int serialPort = open("/dev/ttyS3", O_RDWR);  // Открываем последовательный порт (ttyS1 может варьироваться в зависимости от Orange Pi)

    if (serialPort == -1) {
        std::cerr << "Failed to open serial port" << std::endl;
        return -1;
    }

    // Настройка порта
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(serialPort, &tty) != 0) {
        std::cerr << "Error from tcgetattr" << std::endl;
        return -1;
    }

    cfsetospeed(&tty, B9600);  // Устанавливаем скорость 9600 бод
    cfsetispeed(&tty, B9600);

    tty.c_cflag |= (CLOCAL | CREAD);  // Разрешаем чтение и прием

    // Применяем настройки порта
    if (tcsetattr(serialPort, TCSANOW, &tty) != 0) {
        std::cerr << "Error from tcsetattr" << std::endl;
        return -1;
    }

    // Отправляем строку данных
    std::string dataToSend = "Hello Arduino!";
    write(serialPort, dataToSend.c_str(), dataToSend.size());

    std::cout << "Data sent: " << dataToSend << std::endl;

    close(serialPort);  // Закрываем порт
    return 0;
}