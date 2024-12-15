#include "jsonManager.h"
#include "TimeManager.h"

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>


namespace fs = std::filesystem;

//тест



// Упростим доступ к пространству имен библиотеки

jsonManager::jsonManager(const std::string& filename): filename(filename){}

// Пример функции чтения JSON из файла

json jsonManager::read_json_from_file(const std::string &otherfilename) {
    std::ifstream file(otherfilename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + otherfilename);
    }
    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка парсинга JSON: " + std::string(e.what()));
    }
    return j;
}

json jsonManager::read_json_from_file() {
    return read_json_from_file(filename);
}

    // Пример функции записи JSON в файл
void jsonManager::write_json_to_file(const json& j) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }
    try {
        file << j.dump(4); // Запись с отступом в 4 пробела
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка записи JSON: " + std::string(e.what()));
    }
}
bool jsonManager::create_backup(const std::string& filename) {
    try {
        // Формируем имя резервной копии
        std::string backup_filename = filename + "-" +
            unixTimestampToTimeString(to_unix_timestamp(std::chrono::system_clock::now())) + ".bak";

        // Копируем файл
        std::filesystem::copy(filename, backup_filename, std::filesystem::copy_options::overwrite_existing);
        std::cout << "Резервная копия файла создана: " << backup_filename << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Не удалось создать резервную копию: " << e.what() << std::endl;
        return false;
    }
}


bool jsonManager::file_exists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

bool jsonManager::appendToJsonArray(const nlohmann::json& new_element) {
    std::ifstream input_file(filename);
    nlohmann::json json_array;

    // Проверка на существование файла
    if (!file_exists(filename)) {
        std::cerr << "Файл не найден. Создаю новый массив..." << std::endl;
        json_array = nlohmann::json::array(); // Создаем новый массив
    } else {
        try {
            input_file >> json_array;

            // Проверка, является ли содержимое файла массивом
            if (!json_array.is_array()) {
                std::cerr << "Ошибка: JSON не является массивом. Исправляю..." << std::endl;
                json_array = nlohmann::json::array(); // Преобразуем в новый массив
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка парсинга: " << e.what() << ". Создаю новый массив..." << std::endl;

            // Создаем бекап поврежденного файла
            if (!create_backup(filename)) {
                std::cerr << "Не удалось создать резервную копию." << std::endl;
            }

            json_array = nlohmann::json::array(); // Если ошибка парсинга, создаем новый массив
        }
    }

    // Добавляем новый элемент в массив
    json_array.push_back(new_element);

    // Перезаписываем файл с новым массивом
    std::ofstream output_file(filename);
    if (output_file.is_open()) {
        output_file << json_array.dump(4); // Запись с форматированием
    } else {
        std::cerr << "Ошибка записи в файл!" << std::endl;
        return false;
    }

    return true;
}

/*bool jsonManager::appendToJsonArray( const nlohmann::json& new_element) {
    // Открываем файл в режиме чтения и записи
    std::ifstream input_file(filename);
    nlohmann::json json_array;

    // Читаем файл
    if (input_file.is_open()) {
        try {
            input_file >> json_array;

            // Если структура неправильная, исправляем её
            if (!json_array.is_array()) {
                std::cerr << "Ошибка: JSON не является массивом. Исправляю..." << std::endl;
                json_array = nlohmann::json::array();
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка парсинга: " << e.what() << ". Создаю новый массив..." << std::endl;
            json_array = nlohmann::json::array();
        }
    } else {
        std::cerr << "Файл не найден. Создаю новый массив..." << std::endl;
        json_array = nlohmann::json::array();
    }
    input_file.close();

    // Добавляем новый элемент
    json_array.push_back(new_element);

    // Записываем обратно в файл
    std::ofstream output_file(filename);
    if (output_file.is_open()) {
        output_file << json_array.dump(4); // Запись с форматированием
    } else {
        std::cerr << "Ошибка записи в файл!" << std::endl;
        return false;
    }
    output_file.close();
    return true;
}*/

void jsonManager::changeName(const std::string& newfilename){
        filename = newfilename;
}








/*
int main() {

    jsonManager LogJson("example.json");

    try {
        // Создаем JSON объект
        json j = {
            {"name", "Иван"},
            {"age", 30},
            {"languages", {"C++", "Python", "JavaScript"}},
            {"is_developer", true},
            {"arr", {1,2,3,4,5,6,7,8,9,10}},
            {"mapArr", {{"a",1},{"b",2}}}
        };

        // Сохраняем JSON в файл

        LogJson.write_json_to_file(j);

        std::cout << "JSON успешно записан в файл: " << std::endl;

        // Читаем JSON из файла
        json loaded_json = LogJson.read_json_from_file();
        std::cout << "Прочитанный JSON:\n" << loaded_json.dump(4) << std::endl;

        // Доступ к элементам JSON
        std::string name = loaded_json["name"];
        int age = loaded_json["age"];
        bool is_developer = loaded_json["is_developer"];
        std::cout << "Имя: " << name << "\nВозраст: " << age
                  << "\nРазработчик: " << (is_developer ? "Да" : "Нет") << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}

*/

