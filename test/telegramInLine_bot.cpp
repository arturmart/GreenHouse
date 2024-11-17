
#include <tgbot/tgbot.h>
#include <iostream>

//g++ -std=c++17 -o test testExeTeleg.cpp -L/usr/local/include  -lTgBot -lboost_system -lssl -lcrypto -lpthread Executor.cpp  DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp -lwiringPi DataCombine.cpp crcCalc.cpp -lz

//Executor EXE;

int main() {
    // Замените "YOUR_BOT_TOKEN" на ваш реальный токен бота
    std::string token = "7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c";
    TgBot::Bot bot(token);

    



    // Обработчик команды "/start"
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

        TgBot::InlineKeyboardButton::Ptr button1(new TgBot::InlineKeyboardButton);
        TgBot::InlineKeyboardButton::Ptr button2(new TgBot::InlineKeyboardButton);
        TgBot::InlineKeyboardButton::Ptr button3(new TgBot::InlineKeyboardButton);
        TgBot::InlineKeyboardButton::Ptr button4(new TgBot::InlineKeyboardButton);

        button1->text = "Rele 1";
        button1->callbackData = "button_click_Rele1";

        button2->text = "Rele 2";
        button2->callbackData = "button_click_Rele2";

        button3->text = "Rele 3";
        button3->callbackData = "button_click_Rele3";

        button4->text = "Rele 4";
        button4->callbackData = "button_click_Rele4";

        keyboard->inlineKeyboard.push_back({button1,button2,button3,button4});

        bot.getApi().sendMessage(message->chat->id, "Добро пожаловать в бота! Напишите /help для списка команд.", nullptr, 0, keyboard);
    });

    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query) {
        if (query->data == "button_click_Rele1") {
            bot.getApi().sendMessage(query->message->chat->id, "RL1");
        } 
        else if (query->data == "button_click_Rele2") {
            bot.getApi().sendMessage(query->message->chat->id, "RL2");
        } 
        else if (query->data == "button_click_Rele3") {
            bot.getApi().sendMessage(query->message->chat->id, "RL3");
        } 
        else if (query->data == "button_click_Rele4") {
            bot.getApi().sendMessage(query->message->chat->id, "RL4");
        } 
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





            //EXE.queuePush(message->text);
            //EXE.update();




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




/*
#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

int main() {
    string token(getenv("7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c"));
    printf("Token: %s\n", token.c_str());

    Bot bot(token);

    // Thanks Pietro Falessi for code
    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0;
    InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
    checkButton->text = "check";
    checkButton->callbackData = "check";
    row0.push_back(checkButton);
    keyboard->inlineKeyboard.push_back(row0);

    bot.getEvents().onCommand("start", [&bot, &keyboard](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!", nullptr, nullptr, keyboard);
    });
    bot.getEvents().onCommand("check", [&bot, &keyboard](Message::Ptr message) {
        string response = "ok";
        bot.getApi().sendMessage(message->chat->id, response, nullptr, nullptr, keyboard, "Markdown");
    });
    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "check")) {
            string response = "ok";
            bot.getApi().sendMessage(query->message->chat->id, response, nullptr, nullptr, keyboard, "Markdown");
        }
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (exception& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}
*/