#include <Arduino.h>
#include "DataParser.h" // Include the DataParser class

#define BUFFER_SIZE 64

void setup() {
    Serial.begin(9600); // Инициализация последовательного порта
    Serial.println("___________");

    // Создаем экземпляр класса DataParser
    

    // Исходная строка для парсинга
    StringLimited<BUFFER_SIZE> str("83,0,0;83,1,0;83,2,0;83,3,0/745ec3dc");
    DataParser parser(str);
    
    byte data[8][3]; // Массив для хранения распарсенных байтов
    parser.parse( data); // Парсим данные, передавая указатель на строку и массив байтов

    // Использование массива байтов
    for (int i = 0; i < MAX_DATA_ROWS; i++) {
        Serial.print("Row "); Serial.print(i); Serial.print(": ");
        for (int j = 0; j < 3; j++) {
            Serial.print(data[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
}

void loop() {
    // Ваш код в loop() (если требуется)
}
