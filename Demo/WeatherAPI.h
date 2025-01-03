#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <string>
#include <vector>
#include <unordered_map>

class WeatherAPI {
public:
    // Constructor to initialize WeatherAPI with API key and coordinates
    WeatherAPI(const std::string& apiKey, double lat, double lon);

    // Function to get current weather data
    //std::vector<std::string>  getWeather(const std::vector<std::string>& requestedData) const;
    void updateWeather() ;
    std::unordered_map<std::string, std::string> getWeather() const;

    // Function to get weather forecast data
    //std::vector<std::vector<std::string>> getForecast(const std::vector<std::string>& requestedData) const;
    void updateForecast() ;
    std::vector<std::unordered_map<std::string, std::string>> getForecast() const;

    bool isInited() const;
    bool isInternetAvailable() const;

private:
    std::string apiKey;  // API Key for OpenWeather API
    double latitude;     // Latitude of the location
    double longitude;    // Longitude of the location

    std::unordered_map<std::string, std::string> weather;
    std::vector<std::unordered_map<std::string, std::string>> forecast;

   

    // Function to perform HTTP requests using libcurl
    std::string makeRequest(const std::string& url) const;

    // Callback function for processing data
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer);
};

#endif // WEATHER_API_H
