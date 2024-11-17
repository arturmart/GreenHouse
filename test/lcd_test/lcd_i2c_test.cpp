#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <unistd.h>

#define LCD_ADDR 0x3f  // Адрес вашего I²C LCD, можно узнать через i2cdetect
#define LCD_CHR  1     // Режим передачи символов
#define LCD_CMD  0     // Режим передачи команд
#define LINE_1   0x80  // Адрес для 1 строки на LCD
#define LINE_2   0xC0  // Адрес для 2 строки на LCD

int fd;

void lcd_init();
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);
void lcd_display_string(const char *string, int line);

int main() {
    // Инициализация I2C
    fd = wiringPiI2CSetup(LCD_ADDR);
    if (fd == -1) {
        std::cerr << "Ошибка инициализации I2C!" << std::endl;
        return 1;
    }

    // Инициализация LCD
    lcd_init();

    // Вывод строки на первую и вторую строку дисплея
    lcd_display_string("Hello, Orange Pi!", 1);
    lcd_display_string("I2C LCD Example", 2);

    return 0;
}

void lcd_init() {
    // Начальная инициализация дисплея
    lcd_byte(0x33, LCD_CMD);
    lcd_byte(0x32, LCD_CMD);
    lcd_byte(0x06, LCD_CMD);
    lcd_byte(0x0C, LCD_CMD);
    lcd_byte(0x28, LCD_CMD);
    lcd_byte(0x01, LCD_CMD);
    usleep(500);
}

void lcd_byte(int bits, int mode) {
    int bits_high = mode | (bits & 0xF0) | 0x08;
    int bits_low  = mode | ((bits << 4) & 0xF0) | 0x08;

    wiringPiI2CWrite(fd, bits_high);
    lcd_toggle_enable(bits_high);

    wiringPiI2CWrite(fd, bits_low);
    lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits) {
    usleep(500);
    wiringPiI2CWrite(fd, (bits | 0x04));
    usleep(500);
    wiringPiI2CWrite(fd, (bits & ~0x04));
    usleep(500);
}

void lcd_display_string(const char *string, int line) {
    if (line == 1) {
        lcd_byte(LINE_1, LCD_CMD);
    } else if (line == 2) {
        lcd_byte(LINE_2, LCD_CMD);
    }

    while (*string) {
        lcd_byte(*string++, LCD_CHR);
    }
}