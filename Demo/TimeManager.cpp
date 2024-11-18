#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

void execute_task() {
    std::cout << "Задача выполнена!" << std::endl;
}

void wait_until_time(int target_hour, int target_minute, int target_second) {
    using namespace std::chrono;

    // Текущее время
    auto now = system_clock::now();
    std::time_t now_time_t = system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time_t);

    // Буфер для вывода времени
    char time_buffer[9]; // ЧЧ:ММ:СС

    // Выводим текущее время каждую секунду
    while (true) {
        now = system_clock::now();
        now_time_t = system_clock::to_time_t(now);
        now_tm = *std::localtime(&now_time_t);
        
        // Форматируем время
        std::strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", &now_tm);
        std::cout << "Текущее время: " << time_buffer << std::endl;
        
        std::this_thread::sleep_for(seconds(1));

        // Когда текущее время совпадает с заданным
        if (now_tm.tm_hour == target_hour && now_tm.tm_min == target_minute && now_tm.tm_sec == target_second) {
            execute_task();
            break;
        }
    }
}

int main() {
    int hour, minute, second;

    std::cout << "Введите время для выполнения (часы, минуты, секунды): ";
    std::cin >> hour >> minute >> second;

    wait_until_time(hour, minute, second);

    return 0;
}
