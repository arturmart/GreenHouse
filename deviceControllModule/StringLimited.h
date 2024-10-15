#ifndef STRINGLIMITED_H
#define STRINGLIMITED_H

#include <Arduino.h>

template <unsigned int strN>
class StringLimited {
public:
    // Конструкторы
    StringLimited(); // Конструктор по умолчанию
    StringLimited(const char* str); // Конструктор с C-строкой

    // Преобразование в C-строку
    operator const char*() const;

    // Методы работы со строками
    void copy(const char* str); // Копирование строки
    void copyToBuffer(char* buffer, size_t bufferSize) const; // Копирование в буфер
    int size() const; // Получение размера строки
    size_t bufSize() const; // Получение размера буфера
    char* findChar(char fnd, const int from = 0) const; // Поиск символа
    int findCharID(char fnd, const int from = 0) const; // Поиск символа по индексу
    void AddOn(const char* str, int id); // Добавление строки в начало
    void AddIntOn(int value, int index); // Добавление целого числа по индексу
    void AddEnd(const char* str); // Добавление строки в конец
    void AddIntEnd(const int& val, int base); // Добавление целого числа в конец
    void setChar(int index, char value); // Установка символа по индексу
    const char& getChar(int id) const; // Получение символа по индексу
    const char* getStr() const; // Получение строки
    const char* c_str() const;
    int toInt() const;
    char* substringToChar(int start, int length) const; // Возвращает подстроку как C-строку
    StringLimited<strN> substringToStringLimited(int start, int length) const; // Возвращает подстроку как StringLimited
    StringLimited<strN> substring(int start, int length) const; // Получение подстроки
    void toUpper(); // Преобразование в верхний регистр
    void toLower(); // Преобразование в нижний регистр
    StringLimited<strN> toLowered() const; // Возвращает строку в нижнем регистре

    // Операторы
    StringLimited<strN>& operator+=(const StringLimited<strN>& second); // Оператор += для другой строки
    StringLimited<strN>& operator+=(const char* second); // Оператор += для строки
    StringLimited<strN> operator+(const StringLimited<strN>& second) const; // Оператор + для конкатенации
    bool operator==(const char* other) const; // Оператор сравнения с C-строкой
    bool operator==(const StringLimited<strN>& other) const; // Оператор сравнения с другой строкой
    char& operator[](int index); // Индексатор для доступа к символам
    bool operator!=(const StringLimited<strN>& other) const;

private:
    char _str[strN + 1]; // Массив символов
    unsigned int _size; // Текущий размер строки
};

// Конструктор по умолчанию
template <unsigned int strN>
StringLimited<strN>::StringLimited() : _size(0) {
    _str[0] = '\0'; // Инициализируем пустую строку
}

// Конструктор с C-строкой
template <unsigned int strN>
StringLimited<strN>::StringLimited(const char* str) {
    if (str) {
        //_size = size();
        copy(str);
    } else {
        _str[0] = '\0'; // Устанавливаем пустую строку, если передан nullptr
    }
}

// Преобразование в C-строку
template <unsigned int strN>
StringLimited<strN>::operator const char*() const {
    return _str;
}

// Копирование строки
template <unsigned int strN>
void StringLimited<strN>::copy(const char* str) {
    _size = strlen(str) < strN ? strlen(str) : strN;
    strncpy(_str, str, _size);
    _str[_size] = '\0'; // Добавляем нулевой символ в конец
}
  
// Копирование строки в указанный буфер
template <unsigned int strN>
void StringLimited<strN>::copyToBuffer(char* buffer, size_t bufferSize) const {
    if (bufferSize > 0) {
        size_t lengthToCopy = (bufferSize - 1 < _size) ? (bufferSize - 1) : _size;
        strncpy(buffer, _str, lengthToCopy);
        buffer[lengthToCopy] = '\0'; // Завершаем нулем
    }
}

// Получение размера строки
template <unsigned int strN>
int StringLimited<strN>::size() const {
    return _size;
    //return strlen(_size);
}

// Получение размера буфера
template <unsigned int strN>
size_t StringLimited<strN>::bufSize() const {
    return strN;
}

// Поиск символа
template <unsigned int strN>
char* StringLimited<strN>::findChar(char fnd, const int from) const {
    for (unsigned int i = from; i < _size; i++) {
        if (_str[i] == fnd) {
            return &_str[i];
        }
    }
    return nullptr; // Если не найден
}

// Поиск символа по индексу
template <unsigned int strN>
int StringLimited<strN>::findCharID(char fnd, const int from) const {
    for (unsigned int i = from; i < _size; i++) {
        if (_str[i] == fnd) {
            return i;
        }
    }
    return -1; // Если не найден
}

// Добавление строки в начало
template <unsigned int strN>
void StringLimited<strN>::AddOn(const char* str, int id) {
    int len = strlen(str);
    if (_size + len < strN) {
        memmove(_str + len, _str, _size); // Сдвигаем текущую строку
        strncpy(_str, str, len);
        _size += len;
        _str[_size] = '\0'; // Добавляем нулевой символ
    }
}

// Добавление целого числа по индексу
template <unsigned int strN>
void StringLimited<strN>::AddIntOn(int value, int index) {
    if (index >= 0 && index < strN - 1) {
        int len = snprintf(_str + index, strN - index, "%d", value);
        if (len < 0 || len >= strN - index) {
            _str[index] = '\0'; // Обработка ошибок
        } else {
            _size = strlen(_str); // Обновляем размер
        }
    }
}

// Добавление строки в конец
template <unsigned int strN>
void StringLimited<strN>::AddEnd(const char* str) {
    int len = strlen(str);
    if (_size + len < strN) {
        strncpy(_str + _size, str, len);
        _size += len;
        _str[_size] = '\0'; // Добавляем нулевой символ
    }
}

// Добавление целого числа в конец
template <unsigned int strN>
void StringLimited<strN>::AddIntEnd(const int& val, int base) {
    char buffer[12]; // Достаточно для целых чисел
    itoa(val, buffer, base); // Преобразуем число в строку
    AddEnd(buffer);
}

// Установка символа по индексу
template <unsigned int strN>
void StringLimited<strN>::setChar(int index, char value) {
    if (index >= 0 && index < _size) {
        _str[index] = value; // Устанавливаем символ
    }
}

// Получение символа по индексу
template <unsigned int strN>
const char& StringLimited<strN>::getChar(int id) const {
    if (id >= 0 && id < _size) {
        return _str[id];
    }
    static char nullChar = '\0'; // Возвращаем нулевой символ, если ID некорректен
    return nullChar;
}

// Индексатор для доступа к символам
template <unsigned int strN>
char& StringLimited<strN>::operator[](int index) {
    return _str[index]; // Позволяем доступ к символам
}

// Получение строки
template <unsigned int strN>
const char* StringLimited<strN>::getStr() const {
    return _str;
}
template <unsigned int strN>
const char* StringLimited<strN>::c_str() const {
    return _str; // Предположим, что _data – это массив символов
}



// Преобразование в int
template <unsigned int strN>
int StringLimited<strN>::toInt() const {
    return atoi(_str); // Преобразуем строку в целое число
}

// Возвращает подстроку как C-строку
template <unsigned int strN>
char* StringLimited<strN>::substringToChar(int start, int length) const {
    if (start >= 0 && start + length <= _size) {
        char* subStr = new char[length + 1];
        strncpy(subStr, _str + start, length);
        subStr[length] = '\0'; // Завершаем нулем
        return subStr;
    }
    return nullptr; // Возвращаем nullptr, если выход за пределы
}

// Возвращает подстроку как StringLimited
template <unsigned int strN>
StringLimited<strN> StringLimited<strN>::substringToStringLimited(int start, int length) const {
    StringLimited<strN> subStr;
    //Serial.print(" sstsl("); Serial.print(start + length); Serial.print(")");
    //Serial.print(" sstsl - s("); Serial.print(start); Serial.print(") l("); Serial.print(length); Serial.print(") sz(");Serial.print(_size);Serial.print(")");
    if (start >= 0 && start<=_size) {// if (start >= 0 && start + length <= _size)
        strncpy(subStr._str, _str + start, length);
        subStr._str[length] = '\0'; // Завершаем нулем
        subStr._size = length; // Устанавливаем новый размер
    }
    return subStr;
}

// Получение подстроки
template <unsigned int strN>
StringLimited<strN> StringLimited<strN>::substring(int start, int length) const {
    return substringToStringLimited(start, length); // Переиспользуем метод
}

// Преобразование в верхний регистр
template <unsigned int strN>
void StringLimited<strN>::toUpper() {
    for (unsigned int i = 0; i < _size; i++) {
        _str[i] = toupper(_str[i]);
    }
}

// Преобразование в нижний регистр
template <unsigned int strN>
void StringLimited<strN>::toLower() {
    for (unsigned int i = 0; i < _size; i++) {
        _str[i] = tolower(_str[i]);
    }
}

// Возвращает строку в нижнем регистре
template <unsigned int strN>
StringLimited<strN> StringLimited<strN>::toLowered() const {
    StringLimited<strN> lowerStr(*this); // Создаем новую строку
    for (unsigned int i = 0; i < _size; i++) {
        lowerStr._str[i] = tolower(lowerStr._str[i]);
    }
    return lowerStr; // Возвращаем новую строку
}

// Оператор += для другой строки
template <unsigned int strN>
StringLimited<strN>& StringLimited<strN>::operator+=(const StringLimited<strN>& second) {
    AddEnd(second._str); // Используем метод добавления
    return *this;
}

// Оператор += для строки
template <unsigned int strN>
StringLimited<strN>& StringLimited<strN>::operator+=(const char* second) {
    AddEnd(second); // Используем метод добавления
    return *this;
}

// Оператор + для конкатенации
template <unsigned int strN>
StringLimited<strN> StringLimited<strN>::operator+(const StringLimited<strN>& second) const {
    StringLimited<strN> result(*this); // Создаем новый объект
    result += second; // Используем оператор +=
    return result;
}

// Оператор сравнения с C-строкой
template <unsigned int strN>
bool StringLimited<strN>::operator==(const char* other) const {
    return strcmp(_str, other) == 0;
}

// Оператор сравнения с другой строкой
template <unsigned int strN>
bool StringLimited<strN>::operator==(const StringLimited<strN>& other) const {
    return strcmp(_str, other._str) == 0;
}

// Оператор неравенства
template <unsigned int strN>
bool StringLimited<strN>::operator!=(const StringLimited<strN>& other) const {
    return !(*this == other); // Используем оператор ==
}

#endif // STRINGLIMITED_H
