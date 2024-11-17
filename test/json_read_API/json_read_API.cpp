#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Подключение пространства имен для удобства
using json = nlohmann::json;

// Функция для обработки данных, получаемых через libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t totalSize = size * nmemb;
    buffer->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Инициализация libcurl
    curl = curl_easy_init();
    if (curl) {
        // Установка URL для запроса
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/forecast?lat=40.059456&lon=44.474210&appid=fcb989e5668460983b3cb819569b8c1d&units=metric");

        // Настройка функции обратного вызова для получения данных
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Выполнение запроса
        res = curl_easy_perform(curl);

        // Проверка на наличие ошибок
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Вывод полученного JSON-ответа
            std::cout << "Received data: " << readBuffer << std::endl;

            // Парсинг JSON
            try {
                json jsonData = json::parse(readBuffer);

                // Пример чтения значений из JSON
                std::string city = jsonData["location"]["city"];
                double temperature = jsonData["weather"]["temperature"];

                std::cout << "City: " << city << std::endl;
                std::cout << "Temperature: " << temperature << " °C" << std::endl;
            } catch (json::parse_error& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        // Завершение работы с libcurl
        curl_easy_cleanup(curl);
    }

    return 0;
}