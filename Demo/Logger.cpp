#include "Logger.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
#include <thread>

//g++ -o logger Logger.cpp jsonManager.cpp ChartDrawing.cpp TimeManager.cpp -I/path/to/headers -lcairo

Logger::Logger(const std::string& filename, const std::vector<std::string>& table)
    : filename(filename), LogJson(filename + ".json"), logType(table) {
    setTable(table);  // Initialize the table with the provided column names
}

void Logger::addTable(const std::string& str) {
    dataTable[str] = "";  // Add new table column with an empty value
}

void Logger::setTable(const std::vector<std::string>& table) {
    dataTable.clear();  // Clear existing data
    for (const std::string& str : table) {
        dataTable[str] = "";  // Initialize each column with an empty value
    }
}

void Logger::setData(const std::string& key, const std::string& data) {
    dataTable[key] = data;  // Set the value for a specific column
}

void Logger::log() {
    json newDict = json::object();

    // Iterate through the data table and add the key-value pairs to newDict
    for (const auto& pair : dataTable) {
        if (!pair.second.empty()) {  // Only add non-empty values
            newDict[pair.first] = pair.second;
            dataTable[pair.first] = "";  // Clear the data after logging
        }
    }

    // Try appending the new data to the JSON file
    if (LogJson.appendToJsonArray(newDict)) {
        // std::cout << "Элемент успешно добавлен в JSON!" << std::endl;
    } else {
        std::cerr << "Не удалось добавить элемент." << std::endl;
    }
}

void Logger::setAndLog(const std::unordered_map<std::string, std::string>& table) {
    for (const auto& line : table) {
        setData(line.first, line.second);  // Set multiple data points
    }
    log();  // Log the data
}

void Logger::drawChart(const std::string& title, const std::string& x, const std::vector<std::string>& y, const std::vector<std::string>& bools) {
    // Create and draw a chart using data from the log
    std::vector<std::unordered_map<std::string, std::string>> logElements = getLogElements();
    if (logElements.empty()) {
        std::cerr << "Ошибка: Логовые элементы пусты!" << std::endl;
        return;  // Прерываем выполнение, если данных нет
    }

    // Создаем объект для рисования графика
    try {
        ChartDrawing chartDrawing(logElements);
        // Создаем изображение на основе данных
        chartDrawing.createImageFromJSON(title, x, y, bools);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при создании графика: " << e.what() << std::endl;
    }
}

void Logger::printLogElements(const std::vector<std::unordered_map<std::string, std::string>>& dataMap) {
    for (const auto& dataLine : dataMap) {
        for (const auto& tp : dataLine) {
            std::cout << tp.first << ":" << tp.second << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::unordered_map<std::string, std::string>> Logger::getLogElements() {
    json root = LogJson.read_json_from_file();
    std::vector<std::unordered_map<std::string, std::string>> dataMap;

    if (root.is_null()) {
        std::cerr << "Ошибка: не удалось загрузить или распарсить JSON файл." << std::endl;
        return {};  // Возвращаем пустой вектор, если данные не были загружены
    }

    // Iterate through the JSON data and map it to the log columns
    for (auto& point : root) {
        std::unordered_map<std::string, std::string> dataLine;
        for (const auto& tp : logType) {
            if (point.contains(tp)) {
                dataLine[tp] = point[tp].get<std::string>();  // Add the value if the key exists
            }
        }
        dataMap.push_back(dataLine);
    }

    return dataMap;  // Return the collected log data
}




//g++ -o logger Logger.cpp jsonManager.cpp -I/usr/include/cairo -lcairo TimeManager.cpp DS18B20.cpp
/*
int main(){
   Logger LOG("Log2",{"data","temp","tempOut","R1","R2"});
   LOG.drawChart("Chart", "data",{"temp","tempOut"},{"R1","R2"});

   
   //DS18B20 temp1("28-0303979433f8");

   while (true) {
        float temperature = temp1.readTemperature();
        if (temperature != -1) {

            
            LOG.setData("temp", std::to_string(temperature));

            LOG.setData("data", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())) );
            LOG.log();
            //std::cout << "time: " << unixTimestampToTimeString(to_unix_timestamp(std::chrono::system_clock::now())) << ", Temp: " << temperature << std::endl;
            LOG.LogJson.createImageFromJSON("Log1.json","qaq.png","Real Time Temperature","data","temp");
        }
        usleep(100000); // Update every second
    }
    
    
   




   
   for(int i = 0; i<100;i++){
   LOG.setData("data", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())+i));
   LOG.setData("temp", std::to_string(i/2+i%10));
   LOG.log();
   }

   LOG.LogJson.createImageFromJSON("Log1.json","qaq.png","maq","data","temp");
   
}
*/