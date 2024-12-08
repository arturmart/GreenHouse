#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <string>
#include <chrono>

std::string unixTimestampToTimeString(time_t timestamp);
std::string unixTimestampToDateString(time_t timestamp);
// Функция для выполнения задачи
void execute_task();

// Функция для ожидания времени до заданного (часы, минуты, секунды)
void wait_until_time(int target_hour, int target_minute, int target_second);

// Функция для преобразования времени в формат ISO 8601
std::string to_iso8601(std::chrono::system_clock::time_point time_point);

// Функция для преобразования времени в Unix timestamp
long long to_unix_timestamp(std::chrono::system_clock::time_point time_point);

int getDaySecondsFromTimePoint(std::chrono::system_clock::time_point time_point);

int getDaySecondsFrom3Int(int h, int m, int s);

#endif // TIME_UTILS_H