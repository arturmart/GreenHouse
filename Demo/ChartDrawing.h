#ifndef CHART_DRAWING_H
#define CHART_DRAWING_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <cairo.h>
#include "TimeManager.h"  // Assuming you have this header for the unixTimestampToTimeString function

class ChartDrawing {
public:
    // Public member methods
    ChartDrawing(const std::vector<std::unordered_map<std::string, std::string>>& dataMapIn);
    std::vector<long long> getVectorLongLong(const std::string& logType);
    std::vector<double> getVectorDouble(const std::string& logType);
    std::vector<std::pair<long long, double>> getVectorPair(const std::string& logType1, const std::string& logType2);

    template <typename T>
    std::pair<T, T> getSmallAndLarge(const std::vector<T>& vec);

    bool createImageFromJSON(const std::string& title = "Chart");

private:
    // Private member methods for drawing parts of the chart
    void DrawGrid(cairo_t* cr, int width = 1280, int height = 640, int margin = 100, int Xlines = 20, int Ylines = 10);
    void DraWChart(cairo_t* cr, const std::vector<std::pair<long long, double>>& vec, long long minX, long long maxX, double minY, double maxY, int width = 1280, int height = 640, int margin = 100);
    void DrawXLabels(cairo_t* cr, long long minX, long long maxX, double minY, double maxY, int Xlines = 20, int width = 1280, int height = 640, int margin = 100);
    void DrawYLabels(cairo_t* cr, double minY, double maxY, int Ylines = 10, int width = 1280, int height = 640, int margin = 100);

    std::vector<std::unordered_map<std::string, std::string>> dataMap;  // Reference to dataMap
};

#endif // CHART_DRAWING_H
