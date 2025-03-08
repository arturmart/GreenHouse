#include <iostream>
#include <thread>

//g++ -o testTelegramBot testTelegramBot.cpp TelegramBot.cpp -lTgBot -lcurl -lboost_system -lssl -lcrypto -lpthread


/*
#include "TelegramBot.h"
#include <thread>
// g++ -o testTelegramBot testTelegramBot.cpp TelegramBot.cpp -lTgBot -lssl -lcrypto -lpthread
void getterCommandTGBot(const std::string& arg){std::cout<<"[TG][getter]"<<arg<<std::endl;}
void executorCommandTGBot(const std::string& arg){std::cout<<"[TG][exe]"<<arg<<std::endl;}
void masterCommandsMapBot(const std::string& arg){std::cout<<"[TG][master]"<<arg<<std::endl;}
TelegramBot bot("7943156657:AAEtMkcU_96y-8BatCoaOhIbOtfZ58R021Q",getterCommandTGBot,executorCommandTGBot,masterCommandsMapBot);

int main(){
    std::thread botThread(&TelegramBot::run, &bot);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout<<"[main]loop"<<std::endl;
    }
    return 0;
}
*/



#include <tgbot/tgbot.h>
#include <iostream>

std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();
const int MAX_INCTIVITY = 20;


bool isInet(){
    system("ping -c 1 api.telegram.org > /dev/null 2>&1");
    return WEXITSTATUS(system("echo $?")) == 0;
}

int main() {
    std::string token = "7943156657:AAEtMkcU_96y-8BatCoaOhIbOtfZ58R021Q";  // Замените на свой Telegram API токен
    TgBot::Bot bot(token);

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Привет! Я твой Telegram-бот. Напиши что-нибудь.");
    });

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        if (message->text.empty()) return;
        bot.getApi().sendMessage(message->chat->id, "Ты сказал: " + message->text);
    });

    try {
        std::cout << "Бот запущен..." << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            if(!isInet()){
                std::cerr<<"no inet"<<std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }

            try{
                
                //TgBot::TgLongPoll longPoll(bot);
                longPoll.start();
            } catch (std::exception &e){
                std::cerr<<"Long pool err"<<e.what()<< std::endl;
                std::cerr<<"delay 5 sec"<<std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}


/*
#include <tgbot/tgbot.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <csignal>

// Глобальные переменные
std::atomic<bool> stopBot(false);
std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();
const int MAX_INACTIVITY = 30; // 30 секунд для определения зависания

// Функция проверки соединения с Telegram API
bool isInet() {
    return system("ping -c 1 api.telegram.org > /dev/null 2>&1") == 0;
}

// Функция запуска long polling в отдельном потоке
void runLongPoll(TgBot::Bot& bot) {
    while (!stopBot) {
        try {
            if (!isInet()) {
                std::cerr << "⚠️ Нет интернета! Ждём 5 секунд..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }

            std::cerr << "🟢 Запускаем longPoll..." << std::endl;
            TgBot::TgLongPoll longPoll(bot, 30);
            longPoll.start();  // Здесь может зависнуть
            lastUpdate = std::chrono::steady_clock::now();
        } catch (const std::exception& e) {
            std::cerr << "❌ Ошибка longPoll: " << e.what() << std::endl;
        }

        std::cerr << "♻️ Перезапуск longPoll через 5 сек..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

// Обработчик завершения бота (Ctrl+C)
void signalHandler(int signum) {
    std::cerr << "🔴 Получен сигнал " << signum << ". Завершаем бота..." << std::endl;
    stopBot = true;
}

int main() {
    std::string token = "7943156657:AAEtMkcU_96y-8BatCoaOhIbOtfZ58R021Q"; // Вставь свой токен
    TgBot::Bot bot(token);

    // Реакция на команду /start
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Привет! Я Telegram-бот на C++.");
    });

    // Эхо-ответ на любое сообщение
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        if (!message->text.empty()) {
            bot.getApi().sendMessage(message->chat->id, "Ты сказал: " + message->text);
            lastUpdate = std::chrono::steady_clock::now();
        }
    });

    // Устанавливаем обработчик сигнала Ctrl+C
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    std::cout << "🚀 Бот запущен..." << std::endl;

    // Запускаем longPoll в отдельном потоке
    std::thread pollThread(runLongPoll, std::ref(bot));
    pollThread.detach();

    // Основной поток проверяет зависания longPoll
    while (!stopBot) {
        auto now = std::chrono::steady_clock::now();
        int secondsSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(now - lastUpdate).count();

        if (secondsSinceLastUpdate > MAX_INACTIVITY) {
            std::cerr << "⚠️ Бот завис! Перезапускаем longPoll..." << std::endl;
            stopBot = true;
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    std::cerr << "🔴 Бот остановлен." << std::endl;
    return 0;
}

*/
