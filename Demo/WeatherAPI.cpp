#include "WeatherAPI.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

// Constructor
WeatherAPI::WeatherAPI(const std::string& apiKey, double lat, double lon)
    : apiKey(apiKey), latitude(lat), longitude(lon) {}

// Function to get current weather data
std::vector<std::string> WeatherAPI::getWeather(const std::vector<std::string>& requestedData) const{
    std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=" + std::to_string(latitude) +
                      "&lon=" + std::to_string(longitude) +
                      "&appid=" + apiKey + "&units=metric";
    std::string response = makeRequest(url);

    std::vector<std::string> weatherData;

    if (!response.empty()) {
        try {
            json jsonData = json::parse(response);

            for (const auto& data : requestedData) {
                if (data == "temperature" && jsonData.contains("main") && jsonData["main"].contains("temp")) {
                    double temperature = jsonData["main"]["temp"];
                    weatherData.push_back(std::to_string(temperature) + "°C");
                }
                else if (data == "humidity" && jsonData["main"].contains("humidity")) {
                    int humidity = jsonData["main"]["humidity"];
                    weatherData.push_back(std::to_string(humidity) + "%");
                }
                else if (data == "weather" && !jsonData["weather"].empty()) {
                    std::string weatherDescription = jsonData["weather"][0]["description"];
                    weatherData.push_back(weatherDescription);
                }
                else if (data == "pressure" && jsonData["main"].contains("pressure")) {
                    int pressure = jsonData["main"]["pressure"];
                    weatherData.push_back(std::to_string(pressure) + "hPa");
                }
                else if (data == "windspeed" && jsonData["wind"].contains("speed")) {
                    double windSpeed = jsonData["wind"]["speed"];
                    weatherData.push_back(std::to_string(windSpeed) + "m/s");
                }
                else {
                    weatherData.push_back("Error: " + data + " not found.");
                }
            }
        } catch (json::exception& e) {
            weatherData.push_back("Error parsing JSON: " + std::string(e.what()));
        }
    } else {
        weatherData.push_back("Error: Empty response.");
    }

    return weatherData;
}

// Function to get weather forecast data
std::vector<std::vector<std::string>> WeatherAPI::getForecast(const std::vector<std::string>& requestedData) const{
    std::string url = "https://api.openweathermap.org/data/2.5/forecast?lat=" + std::to_string(latitude) +
                      "&lon=" + std::to_string(longitude) +
                      "&appid=" + apiKey + "&units=metric";
    std::string response = makeRequest(url);

    std::vector<std::vector<std::string>> forecastData;  // Vector of vector of strings

    if (!response.empty()) {
        try {
            json jsonData = json::parse(response);
            if (jsonData.contains("list") && !jsonData["list"].empty()) {
                for (const auto& forecast : jsonData["list"]) {
                    std::string time = forecast["dt_txt"];
                    std::vector<std::string> forecastEntry;  // Entry for one forecast

                    for (const auto& data : requestedData) {
                        if (data == "data" && forecast["main"].contains("temp")) {
                            forecastEntry.push_back(time);
                        }
                        else if (data == "dataUnix" && forecast["main"].contains("temp")) {
                            long long timeUnix = forecast["dt"];
                            forecastEntry.push_back(std::to_string(timeUnix));
                        }
                        else if (data == "temperature" && forecast["main"].contains("temp")) {
                            double temperature = forecast["main"]["temp"];
                            forecastEntry.push_back(std::to_string(temperature) + "°C");
                        }
                        else if (data == "humidity" && forecast["main"].contains("humidity")) {
                            int humidity = forecast["main"]["humidity"];
                            forecastEntry.push_back(std::to_string(humidity) + "%");
                        }
                        else if (data == "weather" && !forecast["weather"].empty()) {
                            std::string weatherDescription = forecast["weather"][0]["description"];
                            forecastEntry.push_back(weatherDescription);
                        }
                        else if (data == "pressure" && forecast["main"].contains("pressure")) {
                            int pressure = forecast["main"]["pressure"];
                            forecastEntry.push_back(std::to_string(pressure) + "hPa");
                        }
                        else if (data == "windspeed" && forecast["wind"].contains("speed")) {
                            double windSpeed = forecast["wind"]["speed"];
                            forecastEntry.push_back(std::to_string(windSpeed) + "m/s");
                        }
                        else {
                            forecastEntry.push_back(data + " not found for " + time);
                        }
                    }

                    forecastData.push_back(forecastEntry);
                }
            } else {
                std::vector<std::string> errorEntry = {"Error: Forecast data not found."};
                forecastData.push_back(errorEntry);
            }
        } catch (json::exception& e) {
            std::vector<std::string> errorEntry = {"Error parsing JSON: " + std::string(e.what())};
            forecastData.push_back(errorEntry);
        }
    } else {
        std::vector<std::string> errorEntry = {"Error: Empty response."};
        forecastData.push_back(errorEntry);
    }

    return forecastData;
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
