#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include "TimeManager.h"  // Подключаем заголовочный файл

std::string unixTimestampToTimeString(time_t timestamp) {
    // Преобразуем timestamp в структуру tm
    struct tm *timeInfo = localtime(&timestamp);
    
    // Создаем строку с нужным форматом (часы:минуты:секунды)
    char buffer[9];  // Формат "HH:MM:SS"
    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeInfo);
    
    return std::string(buffer);  // Возвращаем строку
}

void execute_task() {
    std::cout << "Задача выполнена!" << std::endl;
}

void wait_until_time(int target_hour, int target_minute, int target_second) {
    using namespace std::chrono;

    while (true) {
        // Текущее время
        auto now = system_clock::now();
        std::time_t now_time_t = system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time_t);

        // Буфер для вывода времени
        char time_buffer[9]; // ЧЧ:ММ:СС
        std::strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", &now_tm);
        //std::cout << "Текущее время: " << time_buffer << std::endl;

        // Проверка, если текущее время совпадает с целевым
        if (now_tm.tm_hour == target_hour &&
            now_tm.tm_min == target_minute &&
            now_tm.tm_sec == target_second) {
            execute_task();
            break;
        }

        // Если текущее время позже заданного, ждем следующего дня
        if ((now_tm.tm_hour > target_hour) ||
            (now_tm.tm_hour == target_hour && now_tm.tm_min > target_minute) ||
            (now_tm.tm_hour == target_hour && now_tm.tm_min == target_minute && now_tm.tm_sec > target_second)) {
            //std::cout << "Целевое время прошло. Ожидание следующего дня..." << std::endl;

            // Устанавливаем ожидание до следующего дня
            std::this_thread::sleep_for(seconds(24 * 60 * 60));
        } else {
            // Ждем 1 секунду
            std::this_thread::sleep_for(seconds(1));
        }
    }
}

std::string to_iso8601(std::chrono::system_clock::time_point time_point) {
    std::time_t t = std::chrono::system_clock::to_time_t(time_point);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

long long to_unix_timestamp(std::chrono::system_clock::time_point time_point) {
    return std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
}

/*
int main() {
    int hour, minute, second;

    std::cout << "Введите время для выполнения (часы, минуты, секунды): ";
    std::cin >> hour >> minute >> second;

    wait_until_time(hour, minute, second);

    return 0;
}
*/