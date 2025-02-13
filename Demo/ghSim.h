#ifndef GHSIM_H
#define GHSIM_H

#include <iostream>
#include <cmath>
#include <chrono>
#include <random>
#include <thread>

#include <fstream>
#include <string>

// Clamping function
template <typename T>
T clamp(T value, T minVal, T maxVal) {
    return std::max(minVal, std::min(value, maxVal));
}

class InertiaSimulator {
public:
    InertiaSimulator(float start_value, float max_velocity, float inertia_time);
    void update();
    float getCurrentValue() const;
    float getSimCurrentValue() const;
    void setTargetValue(float target_value);

private:
    float current_value, target_value, velocity, max_velocity, inertia_time;
    std::chrono::high_resolution_clock::time_point start_time;
    static float noise(float input);
    static bool randError();
};

class SensorSim {
public:
    SensorSim(InertiaSimulator* module);
    bool isInit();
    double getData() const;
    void update();
    double getNewData();

private:
    InertiaSimulator* inSim;
    float temperature;
};

class RelaySim {
public:
    void execute(int powerLevel);
    int getHeatingRate() const;

private:
    int heatingRate = 0;
};

class I2cLcdSim {
    public:
        I2cLcdSim(const std::string& filename);
        void execute(const std::string& text);
    
    private:
        std::string filePath;
};
    

class HeatingSystemSim {
public:
    HeatingSystemSim();
    void executeRelay(int rate);
    SensorSim* getSensor1();
    SensorSim* getSensor2();
    SensorSim* getSensor3();
    RelaySim* getRelay();
    I2cLcdSim* getLCD();

private:
    InertiaSimulator simulator1;
    InertiaSimulator simulator2;
    InertiaSimulator simulator3;
    SensorSim sn1;
    SensorSim sn2;
    SensorSim sn3;
    RelaySim rele;
    I2cLcdSim lcd;
};

#endif // GHSIM_H