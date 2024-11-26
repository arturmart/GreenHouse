#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "jsonManager.h"
#include "ChartDrawing.h"

class Logger {
public:
    // Constructor
    Logger(const std::string& filename, const std::vector<std::string>& table = {});

    // Member functions
    void addTable(const std::string& str);
    void setTable(const std::vector<std::string>& table);
    void setData(const std::string& key, const std::string& data);
    void log();
    void setAndLog(const std::unordered_map<std::string, std::string>& table);
    void drawChart(const std::string& title, const std::string& x, const std::vector<std::string>& y, const std::vector<std::string>& bools);
    void printLogElements(const std::vector<std::unordered_map<std::string, std::string>>& dataMap);
    std::vector<std::unordered_map<std::string, std::string>> getLogElements();

private:
    std::string filename;                          // Filename for logging
    jsonManager LogJson;                           // JSON manager object
    std::vector<std::string> logType;              // Columns for logging
    std::unordered_map<std::string, std::string> dataTable;  // Data table for logging
};

#endif // LOGGER_H
