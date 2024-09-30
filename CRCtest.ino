#include <Arduino.h>
#include <StringUtils.h>
#include <Arduino_CRC32.h>


Arduino_CRC32 crc32;


// тест всех Text парсеров и разделителей

void setup() {
    Serial.begin(9600);
    Serial.println();

    
    Text txt("R,1,0;R,2,0;R,3,1/EC502f15");

    Serial.print("TextList: ");
    su::TextList listL1(txt, "/");                      //pars by 1 level separator ("/") [main Data][CRC]

    for (int i = 0; i < listL1.length(); i++) {
            Serial.print(listL1[i]);
            Serial.print(" ");
            
    }
  Serial.print('\n');

    String data = listL1[0].toString();  // Получение строки типа String
    uint32_t crc32_calc = crc32.calc(data.c_str(), data.length());  // Преобразование String в const char* и расчет CRC32

    String sendedCRC = listL1[1].toString();  // Получение строки типа String

    

    Serial.print("CRC32 Result: ");
    Serial.println(crc32_calc);  // Вывод в шестнадцатеричном формате


    Serial.println();
}

void loop() {
}