#include "I2CLCD.h"
#include <thread>
#include <chrono> // Для std::chrono

I2CLCD::I2CLCD(const int& lcdAddress) : address(lcdAddress), fd(-1) {}

bool I2CLCD::initialize() {
    fd = wiringPiI2CSetup(address);
    if (fd == -1) {
        std::cerr << "Ошибка инициализации I2C!" << std::endl;
        return false;
    }

    // Инициализация LCD
    sendByte(0x33, LCD_CMD);
    sendByte(0x32, LCD_CMD);
    sendByte(0x06, LCD_CMD);
    sendByte(0x0C, LCD_CMD);
    sendByte(0x28, LCD_CMD);
    sendByte(0x01, LCD_CMD);
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Задержка 500 мс

    return true;
}

void I2CLCD::sendByte(int bits, int mode) {
    int bits_high = mode | (bits & 0xF0) | 0x08;
    int bits_low  = mode | ((bits << 4) & 0xF0) | 0x08;

    wiringPiI2CWrite(fd, bits_high);
    toggleEnable(bits_high);

    wiringPiI2CWrite(fd, bits_low);
    toggleEnable(bits_low);
}

void I2CLCD::toggleEnable(int bits) {
    std::this_thread::sleep_for(std::chrono::microseconds(500)); // Задержка 500 мкс
    wiringPiI2CWrite(fd, (bits | 0x04));
    std::this_thread::sleep_for(std::chrono::microseconds(500)); // Задержка 500 мкс
    wiringPiI2CWrite(fd, (bits & ~0x04));
    std::this_thread::sleep_for(std::chrono::microseconds(500)); // Задержка 500 мкс
}

void I2CLCD::displayString(const char *string, int line) {
    if (line == 1) {
        sendByte(LINE_1, LCD_CMD);
    } else if (line == 2) {
        sendByte(LINE_2, LCD_CMD);
    }

    while (*string) {
        sendByte(*string++, LCD_CHR);
    }
}

void I2CLCD::clear() {
    sendByte(0x01, LCD_CMD); // Команда очистки дисплея
    std::this_thread::sleep_for(std::chrono::milliseconds(2)); // Задержка 2 мс
}
