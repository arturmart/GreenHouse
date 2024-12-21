#include "jsonManager.h"
#include "TimeManager.h"

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include <mutex>

std::mutex fileMutex;


namespace fs = std::filesystem;

//тест



// Упростим доступ к пространству имен библиотеки

jsonManager::jsonManager(const std::string& filename): filename(filename){}

// Пример функции чтения JSON из файла

json jsonManager::read_json_from_file(const std::string &otherfilename) {
    std::lock_guard<std::mutex> lock(fileMutex);
    std::ifstream file(otherfilename+".json");
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
    std::lock_guard<std::mutex> lock(fileMutex);
    std::ofstream file(filename+ ".json");
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
            unixTimestampToTimeString(to_unix_timestamp(std::chrono::system_clock::now())) + ".json";

        // Копируем файл
        std::filesystem::copy(filename+ ".json", backup_filename, std::filesystem::copy_options::overwrite_existing);
        std::cout << "Резервная копия файла создана: " << backup_filename << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Не удалось создать резервную копию: " << e.what() << std::endl;
        return false;
    }
}


bool jsonManager::file_exists(const std::string& filename) {
    return std::filesystem::exists(filename+ ".json");
}





bool jsonManager::appendToJsonArray(const nlohmann::json& new_element) {
    std::lock_guard<std::mutex> lock(fileMutex); // Синхронизация доступа

    nlohmann::json json_array;

    // Проверяем, существует ли файл
    if (!file_exists(filename)) {
        std::cerr << "Файл не найден. Создаю новый массив..." << std::endl;
        json_array = nlohmann::json::array();
    } else {
        // Если файл существует, пытаемся его прочитать
        std::ifstream input_file(filename + ".json");
        try {
            input_file >> json_array;

            // Проверяем, является ли содержимое файла массивом
            if (!json_array.is_array()) {
                throw std::runtime_error("JSON не является массивом.");
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка обработки файла: " << e.what() << std::endl;

            // Создаем резервную копию поврежденного файла
            if (!create_backup(filename)) {
                std::cerr << "Не удалось создать резервную копию поврежденного файла." << std::endl;
            }

            // Создаем новый массив вместо поврежденного содержимого
            json_array = nlohmann::json::array();
        }
    }

    // Добавляем новый элемент в массив
    json_array.push_back(new_element);

    // Записываем массив обратно в файл
    std::ofstream output_file(filename + ".json"); // Перемещено объявление
    if (output_file.is_open()) {
        try {
            output_file << json_array.dump(4); // Запись с форматированием
        } catch (const std::exception& e) {
            std::cerr << "Ошибка записи в файл: " << e.what() << std::endl;
            return false;
        }
    } else {
        std::cerr << "Не удалось открыть файл для записи!" << std::endl;
        return false;
    }

    return true;
}


void jsonManager::changeName(const std::string& newfilename){
        filename = newfilename;
}






