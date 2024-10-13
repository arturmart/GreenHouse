#include "StringLimited.h"

// Конструктор по умолчанию
template <unsigned int strN>
StringLimited<strN>::StringLimited() : _size(0) {
    _str[0] = '\0'; // Инициализируем пустую строку
}

// Конструктор с строкой
template <unsigned int strN>
StringLimited<strN>::StringLimited(const char* str) {
    copy(str);
}

// Преобразование в строку
template <unsigned int strN>
StringLimited<strN>::operator const char*() const {
    return _str;
}

// Копирование строки
template <unsigned int strN>
void StringLimited<strN>::copy(const char* str) {
    // Ограничиваем размер до strN
    _size = strlen(str) < strN ? strlen(str) : strN;
    strncpy(_str, str, _size);
    _str[_size] = '\0'; // Добавляем нулевой символ в конец
}

// Получение размера строки
template <unsigned int strN>
int StringLimited<strN>::size() const {
    return _size;
}

// Получение размера буфера
template <unsigned int strN>
size_t StringLimited<strN>::bufSize() const {
    return strN;
}

// Поиск символа
template <unsigned int strN>
char* StringLimited<strN>::findChar(char fnd) const {
    for (unsigned int i = 0; i < _size; i++) {
        if (_str[i] == fnd) {
            return &_str[i];
        }
    }
    return nullptr; // Если не найден
}

// Поиск символа по ID
template <unsigned int strN>
int StringLimited<strN>::findCharID(char fnd) const {
    for (unsigned int i = 0; i < _size; i++) {
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
    if (_size + len <= strN) {
        memmove(_str + len, _str, _size); // Сдвигаем текущую строку
        strncpy(_str, str, len);
        _size += len;
        _str[_size] = '\0'; // Добавляем нулевой символ
    }
}
template <unsigned int strN>
void StringLimited<strN>::AddIntOn(int value, int index) {
    if (index >= 0 && index < strN - 1) { // Проверяем границы
        int len = snprintf(_str + index, strN - index, "%d", value);
        if (len < 0 || len >= strN - index) {
            _str[index] = '\0'; // Обработка ошибок
        }
        _size = strlen(_str); // Обновляем размер
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

template <unsigned int strN>
void StringLimited<strN>::setChar(int index, char value) {
    if (index >= 0 && index < _size) {
        _str[index] = value; // Устанавливаем символ
    }
}

// Получение символа по ID
template <unsigned int strN>
const char& StringLimited<strN>::getChar(int id) const {
    if (id >= 0 && id < _size) {
        return _str[id];
    }
    static char nullChar = '\0'; // Возвращаем нулевой символ, если ID некорректен
    return nullChar;
}

template <unsigned int strN>
char& StringLimited<strN>::operator[](int index) {
    return _str[index]; // Возвращаем ссылку на символ
}

// Получение строки
template <unsigned int strN>
const char* StringLimited<strN>::getStr() const {
    return _str;
}

// Получение подстроки
template <unsigned int strN>
StringLimited<strN> StringLimited<strN>::substring(int start, int length) const {
    StringLimited<strN> result;
    if (start >= 0 && start < _size) {
        int end = start + length > _size ? _size : start + length;
        int substrLength = end - start;
        strncpy(result._str, _str + start, substrLength);
        result._str[substrLength] = '\0'; // Добавляем нулевой символ
        result._size = substrLength;
    }
    return result;
}

// Преобразование в верхний регистр
template <unsigned int strN>
void StringLimited<strN>::toUpper() {
    for (unsigned int i = 0; i < _size; i++) {
        if (_str[i] >= 'a' && _str[i] <= 'z') {
            _str[i] -= ('a' - 'A');
        }
    }
}

// Преобразование в нижний регистр
template <unsigned int strN>
void StringLimited<strN>::toLower() {
    for (unsigned int i = 0; i < _size; i++) {
        if (_str[i] >= 'A' && _str[i] <= 'Z') {
            _str[i] += ('a' - 'A');
        }
    }
}

// Оператор += для другой строки
template <unsigned int strN>
StringLimited<strN>& StringLimited<strN>::operator+=(const StringLimited<strN>& second) {
    AddEnd(second.getStr());
    return *this;
}

// Оператор += для строки
template <unsigned int strN>
StringLimited<strN>& StringLimited<strN>::operator+=(const char* second) {
    AddEnd(second);
    return *this;
}

// Оператор + для конкатенации
template <unsigned int strN>
StringLimited<strN> StringLimited<strN>::operator+(const StringLimited<strN>& second) const {
    StringLimited<strN> result(*this);
    result += second;
    return result;
}

// Не забудь включить реализацию шаблона
#include "StringLimited.h" // Это нужно, если ты используешь шаблон