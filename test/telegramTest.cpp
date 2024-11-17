#include <tgbot/tgbot.h>
#include <iostream>

//g++ -std=c++17 -o telegram_bot telegramTest.cpp -L/usr/local/include  -lTgBot -lboost_system -lssl -lcrypto -lpthread

int main() {
    // Замените "YOUR_BOT_TOKEN" на ваш реальный токен бота
    std::string token = "7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c";
    TgBot::Bot bot(token);

    // Обработчик команды "/start"
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Добро пожаловать в бота! Напишите /help для списка команд.");
    });

    // Обработчик команды "/help"
    bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id,
            "Доступные команды:\n"
            "/start - Запустить бота\n"
            "/help - Показать справку\n"
            "/echo [текст] - Повторить ваше сообщение"
        );
    });

    // Обработчик команды "/echo"
    bot.getEvents().onCommand("echo", [&bot](TgBot::Message::Ptr message) {
        std::string reply = "Вы сказали: ";
        if (message->text.size() > 6) { // "/echo " - это 6 символов
            reply += message->text.substr(6);
        } else {
            reply += "(ничего)";
        }
        bot.getApi().sendMessage(message->chat->id, reply);
    });

    // Обработчик неизвестных команд или обычных сообщений
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        if (!message->text.empty() && message->text[0] == '/') {
            bot.getApi().sendMessage(message->chat->id, "Неизвестная команда. Напишите /help для списка доступных команд.");
        } else {
            std::cout << "Получено сообщение: " << message->text << std::endl;
        }
    });

    try {
        std::cout << "Бот запущен..." << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}