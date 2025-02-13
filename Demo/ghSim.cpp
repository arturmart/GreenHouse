#include "ghSim.h"

InertiaSimulator::InertiaSimulator(float start_value, float max_velocity, float inertia_time)
    : current_value(start_value), target_value(start_value),
      max_velocity(max_velocity), inertia_time(inertia_time), velocity(0.0f) {
    start_time = std::chrono::high_resolution_clock::now();
}

void InertiaSimulator::update() {
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(now - start_time).count();
    float alpha = 1 - std::exp(-dt / inertia_time);
    current_value += (target_value - current_value) * alpha;
    start_time = now;
}

float InertiaSimulator::getCurrentValue() const { 
    return current_value; 
}

float InertiaSimulator::getSimCurrentValue() const {
    return (randError() ? -1 : noise(current_value));
}

void InertiaSimulator::setTargetValue(float target_value) {
    this->target_value = target_value;
}

float InertiaSimulator::noise(float input) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(-0.1, 0.1);
    return input + dist(gen);
}

bool InertiaSimulator::randError() {
    return (rand() % 100 < 5);
}

SensorSim::SensorSim(InertiaSimulator* module) : inSim(module) { 
    update(); 
}

bool SensorSim::isInit() { 
    return temperature != -1; 
}

double SensorSim::getData() const { 
    return temperature; 
}

void SensorSim::update() { 
    inSim->update(); 
    temperature = inSim->getSimCurrentValue(); 
}

double SensorSim::getNewData() { 
    update(); 
    return temperature; 
}

void RelaySim::execute(int powerLevel) { 
    heatingRate = clamp(powerLevel, 0, 4); 
}

int RelaySim::getHeatingRate() const { 
    return heatingRate; 
}


I2cLcdSim::I2cLcdSim(const std::string& filename) : filePath(filename) {}
        
void I2cLcdSim::execute(const std::string& text) {
    std::ofstream outFile(filePath, std::ios::trunc);
    if (outFile.is_open()) {
        outFile << text;
        outFile.close();
    }
}
    

 
    

HeatingSystemSim::HeatingSystemSim()
    : simulator1(-10.0f, 5.0f, 5.0f), simulator2(-10.0f, 4.0f, 7.5f), simulator3(-10.0f, 3.0f, 10.0f),
      sn1(&simulator1), sn2(&simulator2), sn3(&simulator3), lcd("GH_SIM_lcd_output.txt") {}



void HeatingSystemSim::executeRelay(int rate) {
    float targetValue = -10 + rele.getHeatingRate() * 10;
    simulator1.setTargetValue(targetValue);
    simulator2.setTargetValue(targetValue);
    simulator3.setTargetValue(targetValue);
}

SensorSim* HeatingSystemSim::getSensor1() { return &sn1; }
SensorSim* HeatingSystemSim::getSensor2() { return &sn2; }
SensorSim* HeatingSystemSim::getSensor3() { return &sn3; }
RelaySim* HeatingSystemSim::getRelay() { return &rele; }
I2cLcdSim* HeatingSystemSim::getLCD() { return &lcd; }



/*


#include <iostream>
#include <cmath>
#include <chrono>
#include <random>
#include <thread>



template <typename T>
T clamp(T value, T minVal, T maxVal) {
    return std::max(minVal, std::min(value, maxVal));
}


class InertiaSimulator {
public:
    //new acceleration to inertia_time
    InertiaSimulator(float start_value, float max_velocity, float inertia_time)
        : current_value(start_value), target_value(start_value),
          max_velocity(max_velocity), inertia_time(inertia_time), velocity(0.0f) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    void update() {

        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(now - start_time).count();

        // Экспоненциальное приближение изменения температуры
        float alpha = 1 - std::exp(-dt / inertia_time);
        current_value += (target_value - current_value) * alpha;

        start_time = now;
        //old with acc
        // auto now = std::chrono::high_resolution_clock::now();
        // float difftime = std::chrono::duration<float>(now - start_time).count();
        // float delta_x = target_value - current_value;

        // float new_velocity = velocity + acceleration * delta_x * difftime;
        // velocity = clamp(new_velocity, -max_velocity, max_velocity);
        // current_value += velocity * difftime;

        // start_time = now;
        
    }

    float getCurrentValue() const { return current_value; }
    float getSimCurrentValue() const {
        return (randError() ? -1 : noise(current_value));
    }

    void setTargetValue(float target_value) { this->target_value = target_value; }

private:
    float current_value, target_value, velocity, max_velocity, inertia_time;
    std::chrono::high_resolution_clock::time_point start_time;

    static float noise(float input) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dist(-0.1, 0.1);
        return input + dist(gen);
    }

    static bool randError() {
        return (rand() % 100 < 5);
    }
};

class Sensor {
public:
    Sensor(InertiaSimulator* module) : inSim(module) { update(); }
    bool isInit() { return temperature != -1; }
    double getData() const { return temperature; }
    void update() { inSim->update(); temperature = inSim->getSimCurrentValue(); }
    double getNewData() { update(); return temperature; }

private:
    InertiaSimulator* inSim;
    float temperature;
};

class Relay {
public:
    void execute(int powerLevel) { heatingRate = clamp(powerLevel, 0, 4); }
    int getHeatingRate() const { return heatingRate; }

private:
    int heatingRate = 0;
};

int main() {
    InertiaSimulator simulator1(-10.0f, 5.0f, 5.0f);
    InertiaSimulator simulator2(-10.0f, 4.0f, 7.5f);
    InertiaSimulator simulator3(-10.0f, 3.0f, 10.0f);

    Sensor sn1(&simulator1);
    Sensor sn2(&simulator2);
    Sensor sn3(&simulator3);

    Relay rele;
    rele.execute(2);

    while (true) {
        float temp1 = sn1.getNewData();
        float temp2 = sn2.getNewData();
        float temp3 = sn3.getNewData();

        float targetValue = -10 + rele.getHeatingRate() * 10;

        simulator1.setTargetValue(targetValue);
        simulator2.setTargetValue(targetValue);
        simulator3.setTargetValue(targetValue);

        std::cout << temp1 << '\t' << temp2 << '\t' << temp3 << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

*/
/*
#include <iostream>
#include <cmath>
#include <chrono>

class InertiaSimulator {
public:
    // Конструктор, принимающий все параметры
    InertiaSimulator(float start_value, float target_value, float max_velocity, float acceleration)
        : current_value(start_value), target_value(target_value), max_velocity(max_velocity),
          acceleration(acceleration), velocity(0.0f){
        start_time = std::chrono::high_resolution_clock::now();  // Сохраняем начальное время
    }

    // Метод для обновления значений
    void update() {
        // Получаем текущее время
        auto now = std::chrono::high_resolution_clock::now();
        float difftime = std::chrono::duration<float>(now - start_time).count();  // Время с начала симуляции в секундах

        // Вычисляем разницу
        float delta_x = target_value - current_value;

        // Обновляем скорость на основе ускорения, ограниченной максимальной скоростью
        velocity = velocity + acceleration * delta_x * difftime;

        // Ограничиваем скорость максимальной величиной
        if (std::abs(velocity) > max_velocity) {
            velocity = sign(velocity) * max_velocity;
        }

        // Обновляем текущее значение
        current_value = current_value + velocity * difftime;

        // Выводим текущее значение
        //std::cout << "Current Value: " << current_value << std::endl;
        
        // Обновляем start_time для следующего вычисления
        start_time = now;
    }

    // Проверка, достигнуто ли целевое значение

    // Геттер для текущего значения
    float getCurrentValue() const {
        return current_value;
    }
    float getSimCurrentValue() const {
        if(randError()){
            return -1;
        }
        else{
            return noise(current_value);
        }
    }



    
    // Сеттеры для изменения параметров
    void setStartValue(float start_value) {
        current_value = start_value;
    }

    void setTargetValue(float target_value) {
        this->target_value = target_value;
    }

    void setMaxVelocity(float max_velocity) {
        this->max_velocity = max_velocity;
    }

    void setAcceleration(float acceleration) {
        this->acceleration = acceleration;
    }

    

private:
    float current_value;       // Текущее значение
    float target_value;        // Целевое значение
    float velocity;            // Скорость изменения
    float max_velocity;       // Максимальная скорость
    float acceleration;        // Ускорение
    std::chrono::high_resolution_clock::time_point start_time;  // Время начала симуляции

    // Функция для определения знака числа
    float sign(float value) const {
        return (value > 0) - (value < 0);  // Возвращает знак числа (1 для положительного, -1 для отрицательного, 0 для нуля)
    }
    float noise(float input) const{
        return (input+(rand() % 200 - 100) / 1000.0); // шум в пределах -0.1 до 0.1
    }
    bool randError() const{
        return (rand() % 100 < 5); // шум в пределах -0.1 до 0.1
    }
};

class Sensor {
    public:
        // Добавим дефолтный конструктор
        Sensor(InertiaSimulator* module)
            : inSim(module){
                update();
            }
    
        bool isInit() {
            return temperature != -1;
        }
    
        double getData() const{
            return temperature;
        }

        void update() {
            inSim->update();
            temperature = inSim->getSimCurrentValue();
        }

        double getNewData() {
            update();
            return temperature;
        }
    
        
    
    private:
        InertiaSimulator* inSim;
        float temperature;
    };

class Relay {
    public:
        bool isInit() {
            return relayStatus != -1;
        }
    
        void execute(bool rele1, bool rele2, bool rele3, bool rele4) {
            // Усиливаем скорость нагрева в зависимости от количества включенных реле
            heatingRate = rele1 + rele2 + rele3 + rele4;
        }
    
        int getHeatingRate() const{
            return heatingRate;
        }
    
    private:
        int heatingRate = 0; // Степень нагрева
        int relayStatus = 0;    // Статус реле (-1 если не инициализировано)
};

int main() {
    // Инициализация симулятора с начальным значением 0, целевым 100, максимальной скоростью 10, ускорением 0.1 и погрешностью 0.01
    InertiaSimulator simulator1(-10.0f, 100.0f, 5.0f, 0.1f);
    InertiaSimulator simulator2(-10.0f, 100.0f, 4.0f, 0.075f);
    InertiaSimulator simulator3(-10.0f, 100.0f, 3.0f, 0.05f);
    
    Sensor sn1(&simulator1);
    Sensor sn2(&simulator2);
    Sensor sn3(&simulator3);

    Relay rele;
    rele.execute(1,0,0,0);
    int heatingRate = rele.getHeatingRate();

    float targetValue = -10;

    

    // Симуляция с обновлением каждую секунду
    while (true) {
        float temp1 = sn1.getNewData();
        float temp2 = sn2.getNewData();
        float temp3 = sn3.getNewData();

        

        targetValue = -10+((float)heatingRate)/4.0*35;

        simulator1.setTargetValue(targetValue);  
        simulator2.setTargetValue(targetValue);  
        simulator3.setTargetValue(targetValue);  
        

        std::cout<<temp1<<'\t'<<temp2<<'\t'<<temp3<<std::endl;
        
    }

    std::cout << "Target reached!" << std::endl;

    return 0;
}
*/


/*
class Relay {
public:
    bool isInit() {
        return relayStatus != -1;
    }

    void execute(bool rele1, bool rele2, bool rele3, bool rele4) {
        // Усиливаем скорость нагрева в зависимости от количества включенных реле
        int activeRelays = rele1 + rele2 + rele3 + rele4;
        heatingRate = activeRelays * 0.5; // Можно настроить коэффициент нагрева
    }

    double getHeatingRate() {
        return heatingRate;
    }

private:
    double heatingRate = 0; // Степень нагрева
    int relayStatus = 0;    // Статус реле (-1 если не инициализировано)
};



class TemperatureSystem {
public:
    TemperatureSystem() {
        // Инициализация датчиков с разными инерциями
        sensor1 = Sensor(-5.001, 10.0); // датчик около нагревателя (самая высокая инерция)
        sensor2 = Sensor(-5.001, 5.0);  // датчик в комнате (средняя инерция)
        sensor3 = Sensor(-5.001, 2.0);  // самый медленный датчик
    }

    void update(double elapsedTime, bool rele1, bool rele2, bool rele3, bool rele4) {
        relay.execute(rele1, rele2, rele3, rele4);

        sensor1.update(elapsedTime, relay);
        sensor2.update(elapsedTime, relay);
        sensor3.update(elapsedTime, relay);
    }

    void printSensors() {
        std::cout << "S1-" << sensor1.getData();
        std::cout << " \tS2-" << sensor2.getData();
        std::cout << " \tS3-" << sensor3.getData() << std::endl;
    }

private:
    Sensor sensor1, sensor2, sensor3;
    Relay relay;
};



class GHSystemsimulation{
    

    public: 
    GHSystemsimulation(){
        start = std::time(nullptr);

    }
    void update(){
        std::time_t end = std::time(nullptr);
        double diff = std::difftime(end, start);

        double random_noise = (rand() % 200 - 100) / 1000.0;


        realTemp+=(target-realTemp)/10;
    }

    private:
    Sensor sensor1, sensor2, sensor3;
    Relay relay;

    double realTemp=0;
    double inertia =10; // acseleration
    double power =10; // acseleration
    std::time_t start;

};

int main() {
    TemperatureSystem system;
    
    system.printSensors();

  float sign(float value) {
    return (value > 0) - (value < 0); // Возвращает знак числа (1 для положительного, -1 для отрицательного, 0 для нуля)
}  // Пример обновления системы на протяжении времени
    while(true){
        system.update(1.0, true, false, true, false); // 1 секунда, включены реле 1 и 3
        system.printSensors();
    }

    return 0;
}

*/


/*



#include<iostream>
#include<ctime>
#include<cstdlib>


class GHSimulator{
    public:

    private:
    

};

int main(){

    double x =0;
    double alpha = 10e-8;

    std::time_t start = std::time(nullptr);

    std::srand(std::time(nullptr));

    while(true){
        std::time_t end = std::time(nullptr);
        double diff = std::difftime(end, start);

        double random_noise = (static_cast<double>(std::rand()) / RAND_MAX)-0.5 ;

        bool random_err = (std::rand() % 100) <= 1;

        x+=(20+random_noise-x)*alpha*diff;

        if(!random_err){
            
            std::cout<<x<<std::endl;
        }
        else{
            
            std::cout<<-1<<std::endl;
        }

    }

    return 0;
}
*/