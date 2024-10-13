
#include <Arduino.h> // Не забудьте добавить эту строку для работы с Arduino

#include <cstring>   // Для работы со строками
#include <utility>   // Для std::pair
#include <cctype>    // Для функций преобразования регистра

// Шаблонный класс для работы с ограниченными строками
template <size_t strN>
class StringLimited {
public:
    // Конструктор по умолчанию, инициализирует пустую строку
    StringLimited() : _size(strN) {
        _str[0] = '\0';  // Инициализируем строку пустой строкой
    }

    // Конструктор, принимающий строку
    StringLimited(const char* str) : _size(strN) {
        copy(str);  // Копируем переданную строку
    }

    // Конструктор копирования
    StringLimited(const StringLimited& other) : _size(other._size) {
        copy(other._str);  // Копируем строку из другого объекта
    }

    // Оператор присваивания для объектов одинакового размера
    StringLimited& operator=(const StringLimited& other) {
        if (this != &other) {  // Проверка на самоприсваивание
            copy(other._str);  // Копируем строку
        }
        return *this;
    }

    // Оператор присваивания для объектов разного размера
    template <size_t otherSize>
    StringLimited& operator=(const StringLimited<otherSize>& other) {
        copy(other.getStr());  // Копируем строку из другого объекта
        return *this;
    }

    // Преобразование в const char*
    operator const char*() const {
        return _str;
    }

    // Метод для копирования строки с защитой от переполнения
    void copy(const char* str) {
        strncpy(_str, str, _size - 1);  // Копируем строку
        _str[_size - 1] = '\0';         // Обеспечиваем наличие нулевого терминатора
    }

    // Возвращает текущий размер строки
    int size() const {
        return strlen(_str);  // Возвращаем длину строки
    }

    // Возвращает размер буфера
    size_t bufSize() const {
        return _size;  // Возвращаем размер буфера
    }

    // Поиск символа в строке
    char* findChar(char fnd) const {
        return strchr(_str, fnd);  // Используем стандартную функцию для поиска
    }

    // Возвращает индекс найденного символа или -1, если не найден
    int findCharID(char fnd) const {
        char* pos = findChar(fnd);
        return (pos != nullptr) ? (pos - _str) : -1;  // Проверка на nullptr
    }

    // Добавление строки в заданное место
    void AddOn(const char* str, int id) {
        if (id >= 0 && id < _size) {
            strncat(_str + id, str, _size - id - 1);  // Добавляем строку с защитой от переполнения
        }
    }

    // Добавление строки в конец
    void AddEnd(const char* str) {
        AddOn(str, size());  // Добавляем строку в конец
    }

    // Добавление строки из другого объекта в конец
    void AddEnd(const StringLimited& second) {
        AddOn(second.getStr(), size());  // Используем метод AddOn
    }

    // Добавление целого числа в заданное место
    void AddIntOn(const int& val, int id, int base = 10) {
        char buffer[16];  // Буфер для временного хранения целого числа
        itoa(val, buffer, base);  // Преобразуем целое число в строку
        AddOn(buffer, id);  // Добавляем строку
    }

    // Добавление целого числа в конец
    void AddIntEnd(const int& val, int base = 10) {
        AddIntOn(val, size(), base);  // Используем метод AddIntOn
    }

    // Возвращает символ по индексу
    const char& getChar(int id) const {
        return _str[id];  // Возвращаем символ
    }

    // Возвращает строку
    const char* getStr() const {
        return _str;  // Возвращаем указатель на строку
    }

    // Перегруженный оператор [] для доступа к символам
    const char& operator[] (int id) const {
        return getChar(id);  // Используем метод getChar
    }

    // Установка символа по индексу
    void setChar(int id, const char& chr) {
        if (id >= 0 && id < _size) {
            _str[id] = chr;  // Устанавливаем символ
        }
    }


    // Перегруженный оператор [] для изменения символов
    char& operator[](int id) {
        return _str[id];  // Возвращаем ссылку для изменения элемента
    }

    // Оператор сложения для добавления другой строки
    StringLimited& operator+= (const StringLimited& second) {
        AddEnd(second);  // Используем метод AddEnd
        return *this;  // Возвращаем ссылку на текущий объект
    }

    // Оператор сложения для добавления C-строки
    StringLimited& operator+= (const char* second) {
        AddEnd(second);  // Используем метод AddEnd
        return *this;  // Возвращаем ссылку на текущий объект
    }

    // Оператор сложения для добавления целого числа
    StringLimited& operator+= (const int& second) {
        AddIntEnd(second);  // Используем метод AddIntEnd
        return *this;  // Возвращаем ссылку на текущий объект
    }

    // Оператор сложения для объединения двух объектов
    StringLimited<strN> operator+ (const StringLimited& second) const {
        StringLimited<strN> temp(this->_str);  // Создаём временный объект
        temp.AddEnd(second.getStr());  // Добавляем строку из второго объекта
        return temp;  // Возвращаем новый объект
    }

    // Оператор сложения для объединения с C-строкой
    StringLimited<strN> operator+ (const char* second) const {
        StringLimited<strN> temp(this->_str);  // Создаём временный объект
        temp.AddEnd(second);  // Добавляем C-строку
        return temp;  // Возвращаем новый объект
    }

    // Оператор сложения для объединения с целым числом
    StringLimited<strN> operator+ (const int& second) const {
        StringLimited<strN> temp(this->_str);  // Создаём временный объект
        temp.AddIntEnd(second);  // Добавляем целое число
        return temp;  // Возвращаем новый объект
    }

    // Метод для получения подстроки
    StringLimited<strN> substring(int start, int length) const {
        if (start < 0 || length <= 0 || start >= size()) {
            return StringLimited<strN>(); // Возвращаем пустую строку, если параметры некорректны
        }

        if (start + length > size()) {
            length = size() - start; // Корректируем длину
        }

        StringLimited<strN> result;  // Создаём новый объект
        strncpy(result._str, _str + start, length);  // Копируем подстроку
        result._str[length] = '\0';  // Добавляем нулевой терминатор
        return result;  // Возвращаем подстроку
    }

    // Метод для преобразования строки в верхний регистр
    void toUpper() {
        for (int i = 0; i < size(); ++i) {
            _str[i] = toupper(_str[i]);  // Преобразуем каждый символ в верхний регистр
        }
    }

    // Метод для преобразования строки в нижний регистр
    void toLower() {
        for (int i = 0; i < size(); ++i) {
            _str[i] = tolower(_str[i]);  // Преобразуем каждый символ в нижний регистр
        }
    }

private:
    size_t _size;  // Размер буфера
    char _str[strN];  // Хранилище для строки
};

// Пример использования класса в функции setup
void setup() {
    Serial.begin(9600);  // Начинаем сериализацию
    Serial.println();

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
    // Здесь можно добавить основной код, который будет выполняться циклично
}
