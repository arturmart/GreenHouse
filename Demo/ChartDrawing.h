#ifndef CHART_DRAWING_H
#define CHART_DRAWING_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <cairo/cairo.h>
#include "TimeManager.h"  // Assuming you have this header for the unixTimestampToTimeString function


typedef struct {
    double r, g, b;
} rgb_color;


extern rgb_color colorsCario[16];

class ChartDrawing {
public:
    // Public member methods
    ChartDrawing(const std::vector<std::unordered_map<std::string, std::string>>& dataMapIn);
    std::vector<long long> getVectorLongLong(const std::string& logType);
    std::vector<double> getVectorDouble(const std::string& logType);
    std::vector<std::pair<long long, double>> getVectorPairDouble(const std::string& logType1, const std::string& logType2);

    std::vector<std::pair<long long, bool>> getVectorPairBool(const std::string& logType1, const std::string& logType2);

    template <typename T>
    std::pair<T, T> getSmallAndLarge(const std::vector<T>& vec);

    bool createImageFromJSON(const std::string& title, const std::string& time, const std::vector<std::string>& charts, const std::vector<std::string>& bools);
    
private:
    // Private member methods for drawing parts of the chart
    void DrawGrid(
        cairo_t* cr, int width = 1280, int height = 640,
        int marginTop = 200,int marginBottom = 80,int marginLeft = 100,int marginRight = 60,
        int Xlines = 20, int Ylines = 10);
    void DraWChart(
        cairo_t* cr, const std::vector<std::pair<long long, double>>& vec,
        long long minX, long long maxX, double minY, double maxY,
        int width = 1280, int height = 640,
        int marginTop = 200,int marginBottom = 80,int marginLeft = 100,int marginRight = 60);
    void DraWBoolChartEvent(
        cairo_t* cr, const std::vector<std::pair<long long, bool>>& vec,
        long long minX, long long maxX,
        int Ylines = 10, int Yid = 0,
        int width = 1280, int height = 640,
        int marginTop = 200,int marginBottom = 80,int marginLeft = 100,int marginRight = 60);

    void DraWBoolChart(
        cairo_t* cr, const std::vector<std::pair<long long, bool>>& vec,
        long long minX, long long maxX,
        int Ylines = 10, int Yid = 0,
        int width = 1280, int height = 640,
        int marginTop = 200,int marginBottom = 80,int marginLeft = 100,int marginRight = 60);
    
    
    void DrawChartXLabels(
        cairo_t* cr,
        long long minX, long long maxX, double minY, double maxY,
        int Xlines = 20,
        int width = 1280, int height = 640,
        int marginBottom = 80,int marginLeft = 100, int marginRight = 60);
    void DrawChartYLabels(cairo_t* cr,
        double minY, double maxY,
        int Ylines = 10,
        int width = 1280, int height = 640, 
        int marginTop = 200,int marginBottom = 80,int marginLeft = 100);
    void DrawYLabels(
        cairo_t* cr,
        const std::vector<std::string>& strs,
        int Ylines = 20,
        int width = 1280, int height = 640,
        int marginTop = 200,int marginBottom = 80,int marginLeft = 100);

    void DrawYLabelsByStepColored(
        cairo_t* cr,
        const std::vector<std::string>& strs,
        int Ylines = 20,
        int YStep = 20,
        int width = 1280, int height = 640,
        int marginTop = 200,int marginLeft = 100);

    std::vector<std::unordered_map<std::string, std::string>> dataMap;  // Reference to dataMap
};



#endif // CHART_DRAWING_H
