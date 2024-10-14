#ifndef PARSER_H
#define PARSER_H

#include "StringLimited.h" // Включаем заголовочный файл для StringLimited

// Определяем размер буфера
#define PARSER_BUFFER_SIZE 64

// Шаблонный класс Parser для разбора строк
template <unsigned int strN>
class Parser {
public:
    // Конструктор класса Parser

    Parser() = default;

    Parser(const StringLimited<strN>& str) {
        setStr(str); // Инициализация строки
    }

    StringLimited<strN> getStr() const{
        return _str; 
    }

    // Установка строки
    void setStr(const StringLimited<strN>& str) {
        _str = str; // Присваиваем значение строке
    }

    // Получение указателя на часть строки
    char* getPartCharPtr(int* delimiter, int id, int start = 0) {
        // Проверка на корректность индекса
        if (id < 0 || id > delimiter[0]) return nullptr; 
        return _str.getStr() + (id == 0 ? start : delimiter[id] + 1); // Возвращаем указатель на часть строки
    }

    // Разделение строки по разделителю
    void parseCut(int* delimiter, char trg, int size = 8) {
        find(delimiter, trg, size); // Находим разделители
        for (int i = 1; i <= delimiter[0] ; i++) {
            _str.setChar(delimiter[i], '\0'); // Завершаем строку нулем
        }
    }

    // Поиск индексов разделителей
    void find(int* delimiter, char trg, int size = 8) {
        delimiter[0] = 0; // Обнуляем счетчик
        int f = -1; // Индекс для поиска
        for (int crr = 1; crr <= size; crr++) {
            f = _str.findCharID(trg, f + 1); // Находим индекс разделителя
            if (f == -1) break; // Если не найдено, выходим
            delimiter[crr] = f; // Сохраняем индекс
            delimiter[0]++; // Увеличиваем счетчик
        }
    }

    // Получение подстроки как char*
    char* getSubstringFromPart(int* delimiter, const int id, int length = PARSER_BUFFER_SIZE) const { // Изменено на PARSER_BUFFER_SIZE
        // Проверка на корректность индекса
        if (id < 0 || id > delimiter[0]) {
            return nullptr; // Возвращаем nullptr для некорректных индексов
        }
        int startIndex = getPartCharPtr(delimiter, id) - _str.getStr(); // Вычисляем начальный индекс
        return _str.substringToChar(startIndex, length); // Получаем подстроку как char*
    }

    // Получение подстроки как StringLimited
    template <unsigned int targetSize>
    StringLimited<targetSize> getSubstringFromPart(int* delimiter, const int id, int length = PARSER_BUFFER_SIZE) const { // Изменено на PARSER_BUFFER_SIZE
        // Проверка на корректность индекса
        if (id < 0 || id > delimiter[0]) {
            return StringLimited<targetSize>(""); // Возвращаем пустую строку, если индекс некорректен
        }
        int startIndex = getPartCharPtr(delimiter, id) - _str.getStr(); // Вычисляем начальный индекс
        StringLimited<64> temp = _str.substringToStringLimited(startIndex, length); // Получаем временную строку
        return StringLimited<targetSize>(temp.getStr()); // Конвертируем в нужный размер и возвращаем
    }

private:
    StringLimited<strN> _str; // Используем strN для соответствия шаблону
};

#endif // PARSER_H
