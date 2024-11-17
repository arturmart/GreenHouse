#include <zlib.h>
#include <iostream>
#include <string>

int main() {
    // Входные данные
    std::string data = "Hello, CRC!";
    
    // Инициализация CRC32
    unsigned long crc = crc32(0L, Z_NULL, 0);
    
    // Вычисление CRC32 для строки
    crc = crc32(crc, reinterpret_cast<const unsigned char*>(data.c_str()), data.length());
    
    // Вывод результата
    std::cout << "CRC32: " << std::hex << crc << std::endl;
    
    return 0;
}
