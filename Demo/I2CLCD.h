#ifndef I2CLCD_H
#define I2CLCD_H

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <unistd.h>

#define LCD_CHR  1     // Режим передачи символов
#define LCD_CMD  0     // Режим передачи команд
#define LINE_1   0x80  // Адрес для 1 строки на LCD
#define LINE_2   0xC0  // Адрес для 2 строки на LCD

class I2CLCD {
private:
    int fd;
    int address;

    void sendByte(int bits, int mode);
    void toggleEnable(int bits);

public:
    I2CLCD(const int& lcdAddress);

    bool initialize();
    void displayString(const char *string, int line);
    void clear();
};

#endif // I2CLCD_H
