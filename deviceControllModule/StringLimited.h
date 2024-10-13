#ifndef StringLimited_h
#define StringLimited_h

#include <Arduino.h>

template <unsigned int strN>
class StringLimited {
public:
    StringLimited(); // Конструктор по умолчанию
    StringLimited(const char* str); // Конструктор с строкой
    operator const char*() const; // Преобразование в строку
    void copy(const char* str); // Копирование строки
    int size() const; // Получение размера строки
    size_t bufSize() const; // Получение размера буфера
    char* findChar(char fnd) const; // Поиск символа
    int findCharID(char fnd) const; // Поиск символа по ID
    void AddOn(const char* str, int id); // Добавление строки
    void AddIntOn(int value, int index);
    void AddEnd(const char* str); // Добавление строки в конец
    void AddIntEnd(const int& val, int base); // Добавление целого числа
    void setChar(int index, char value);
    const char& getChar(int id) const; // Получение символа по ID
    const char* getStr() const; // Получение строки
    StringLimited<strN> substring(int start, int length) const; // Получение подстроки
    void toUpper(); // Преобразование в верхний регистр
    void toLower(); // Преобразование в нижний регистр
    StringLimited<strN>& operator+=(const StringLimited<strN>& second); // Оператор += для другой строки
    StringLimited<strN>& operator+=(const char* second); // Оператор += для строки
    StringLimited<strN> operator+(const StringLimited<strN>& second) const; // Оператор + для конкатенации

    char& operator[](int index); // Индексатор для доступа к символам

private:
    char _str[strN + 1]; // Массив символов
    unsigned int _size; // Текущий размер строки
};

//#include "StringLimited.cpp" // Включаем реализацию шаблона

#endif