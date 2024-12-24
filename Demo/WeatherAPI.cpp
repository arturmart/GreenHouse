#include "WeatherAPI.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

// Constructor
WeatherAPI::WeatherAPI(const std::string& apiKey, double lat, double lon)
    : apiKey(apiKey), latitude(lat), longitude(lon) {}

// Function to get current weather data
//std::vector<std::string> WeatherAPI::getWeather(const std::vector<std::string>& requestedData) const{
void WeatherAPI::updateWeather()  {
    std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=" + std::to_string(latitude) +
                      "&lon=" + std::to_string(longitude) +
                      "&appid=" + apiKey + "&units=metric";
    std::string response = makeRequest(url);

    std::unordered_map<std::string, std::string> weatherData;

    if (!response.empty()) {
        try {
            json jsonData = json::parse(response);

            // Извлечение данных из JSON
            weatherData["temp"] = std::to_string(jsonData["main"]["temp"].get<double>()) ;//+ "°C"
            weatherData["humidity"] = std::to_string(jsonData["main"]["humidity"].get<int>()) ;//+ "%"
            weatherData["weather"] = jsonData["weather"][0]["description"];
            weatherData["pressure"] = std::to_string(jsonData["main"]["pressure"].get<int>()) ;//+ "hPa"
            weatherData["windspeed"] = std::to_string(jsonData["wind"]["speed"].get<double>()) ;//+ "m/s"
        } catch (json::exception& e) {
            weatherData["error"] = "Error parsing JSON: " + std::string(e.what());
        }
    } else {
        weatherData["error"] = "Empty response.";
    }

    weather = weatherData;

}

// Function to get weather forecast data
//std::vector<std::vector<std::string>> WeatherAPI::getForecast(const std::vector<std::string>& requestedData) const{
void WeatherAPI::updateForecast()  {
    std::string url = "https://api.openweathermap.org/data/2.5/forecast?lat=" + std::to_string(latitude) +
                      "&lon=" + std::to_string(longitude) +
                      "&appid=" + apiKey + "&units=metric";
    std::string response = makeRequest(url);

    std::vector<std::unordered_map<std::string, std::string>> forecastData;

    if (!response.empty()) {
        try {
            json jsonData = json::parse(response);

            if (jsonData.contains("list") && !jsonData["list"].empty()) {
                for (const auto& forecast : jsonData["list"]) {
                    std::unordered_map<std::string, std::string> forecastEntry;

                    // Добавляем дату в формате строки
                    forecastEntry["date"] = forecast["dt_txt"].get<std::string>();

                    // Добавляем временную метку в формате UNIX
                    forecastEntry["dateUnix"] = std::to_string(forecast["dt"].get<int>());

                    // Добавляем другие параметры прогноза
                    forecastEntry["temp"] = std::to_string(forecast["main"]["temp"].get<double>()) ;//+ "°C"
                    forecastEntry["humidity"] = std::to_string(forecast["main"]["humidity"].get<int>()) ;//+ "%"
                    forecastEntry["weather"] = forecast["weather"][0]["description"];
                    forecastEntry["pressure"] = std::to_string(forecast["main"]["pressure"].get<int>()) ;//+ "hPa"
                    forecastEntry["windspeed"] = std::to_string(forecast["wind"]["speed"].get<double>()) ;//+ "m/s"

                    // Добавляем запись прогноза в итоговый массив
                    forecastData.push_back(forecastEntry);
                }
            } else {
                forecastData.push_back({{"error", "Forecast data not found."}});
            }
        } catch (json::exception& e) {
            forecastData.push_back({{"error", "Error parsing JSON: " + std::string(e.what())}});
        }
    } else {
        forecastData.push_back({{"error", "Empty response."}});
    }

    forecast = forecastData;

}

std::unordered_map<std::string, std::string> WeatherAPI::getWeather() const{
    return weather;
}

std::vector<std::unordered_map<std::string, std::string>> WeatherAPI::getForecast() const{
    return forecast;
}

// Function to make HTTP requests using libcurl
std::string WeatherAPI::makeRequest(const std::string& url) const{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

// Callback function for processing data
size_t WeatherAPI::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t totalSize = size * nmemb;
    buffer->append((char*)contents, totalSize);
    return totalSize;
}

bool WeatherAPI::isInited() const {
    std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=" + std::to_string(latitude) +
                      "&lon=" + std::to_string(longitude) +
                      "&appid=" + apiKey + "&units=metric";
    std::string response = makeRequest(url);
    

    if (!response.empty()) {
        try {
            json jsonData = json::parse(response);
            // Проверка, если API возвращает код состояния "cod" со значением 200
            if (jsonData.contains("cod") && jsonData["cod"] == 200) {
                return true;  // API инициализирован и отвечает успешно
            }
        } catch (json::exception& e) {
            // Если произошла ошибка при разборе ответа, возвращаем false
            std::cerr << "Ошибка при разборе JSON в isInited: " << e.what() << std::endl;
        }
    }
    return false;  // Если ответ пустой или неправильный, возвращаем false
}


bool WeatherAPI::isInternetAvailable() const {
    try {
        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Не удалось инициализировать CURL" << std::endl;
            return false;
        }

        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_URL, "https://google.com"); // Публичный URL
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);             // Таймаут в 5 секунд
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);      // Следование за редиректами
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);           // Отключение сигналов

        CURLcode res = curl_easy_perform(curl); // Выполняем запрос

        if (res == CURLE_OK) {
            curl_easy_cleanup(curl);
            return true;
        } else {
            std::cerr << "Ошибка при проверке интернета: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Исключение при проверке интернета: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Неизвестная ошибка при проверке интернета." << std::endl;
        return false;
    }
}
