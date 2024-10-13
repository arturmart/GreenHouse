#include "StringLimited.h"
#include "StringLimited.cpp"

void setup() {
    Serial.begin(9600);

    StringLimited<64> str("Hello World");  // Создаём объект с начальной строкой
    Serial.println(str);  // Печатаем начальную строку
    Serial.print("size - "); Serial.println(str.size());  // Печатаем размер строки
    Serial.print("buffer size - "); Serial.println(str.bufSize());  // Печатаем размер буфера

    Serial.print("findChar('W') ptr - "); Serial.println((int)str.findChar('W'));  // Печатаем указатель на найденный символ
    Serial.print("findCharID('W') - "); Serial.println(str.findCharID('W'));  // Печатаем индекс найденного символа
    Serial.print("AddOn(\"Arduino\",11) - "); str.AddOn(" Arduino", 11); Serial.println(str);  // Добавляем строку " Arduino" с индекса 11
    Serial.print("AddEnd(\" :) ! \") - "); str.AddEnd(" :) ! "); Serial.println(str);  // Добавляем строку " :) !" в конец
    Serial.print("AddIntOn(10,11) - "); str.AddIntOn(10, 25); Serial.println(str);  // Добавляем целое число 10 с индекса 25
    Serial.print("AddIntEnd(15,HEX) - "); str.AddIntEnd(15, HEX); Serial.println(str);  // Добавляем число 15 в шестнадцатеричном формате в конец
    Serial.print("getChar(10) - "); Serial.println(str.getChar(10));  // Получаем символ по индексу 10
    Serial.print("str[9] - "); Serial.println(str[9]);  // Доступ к символу по индексу 9
    Serial.print("setChar(10,'D') - "); str.setChar(10, 'D'); Serial.println(str);  // Устанавливаем символ по индексу 10
    Serial.print("str[9] = 'L' - "); str[9] = 'L'; Serial.println(str);  // Изменяем символ по индексу 9
    Serial.print("str += \" Privet\" - "); str += " Privet"; Serial.println(str);  // Добавляем строку " Privet"
    
    StringLimited<8> str2(" Mir ");  // Создаём ещё один объект
    Serial.print("str += str2 - "); str += str2; Serial.println(str);  // Добавляем строку из второго объекта
    Serial.print("str += 123 - "); str += 123; Serial.println(str);  // Добавляем целое число 123
    Serial.print("str += str2 + \" \" + 2 - "); str += str2 + " ; " + 2; Serial.println(str);  // Добавляем строку и число
    StringLimited<64> sub = str.substring(6, 5);  // Получаем подстроку с индекса 6 длиной 5 символов
    Serial.print("substring(6, 5); - "); Serial.println(sub);  // Печатаем подстроку

    // Пример использования методов преобразования регистра
    str.toUpper();  // Преобразуем строку в верхний регистр
    Serial.print("toUpper() - "); Serial.println(str);  // Печатаем строку в верхнем регистре
    str.toLower();  // Преобразуем строку в нижний регистр
    Serial.print("toLower() - "); Serial.println(str);  // Печатаем строку в нижнем регистре
}


void loop() {
    // Ваш код
}