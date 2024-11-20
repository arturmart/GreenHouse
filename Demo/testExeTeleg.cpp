#include <tgbot/tgbot.h>
#include "Executor.h"
#include <iostream>


//g++ -std=c++17 -o test testExeTeleg.cpp -L/usr/local/include  -lTgBot -lboost_system -lssl -lcrypto -lpthread Executor.cpp  DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp -lwiringPi DataCombine.cpp crcCalc.cpp -lz

Executor EXE;

int main() {
    // Замените "YOUR_BOT_TOKEN" на ваш реальный токен бота
    std::string token = "7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c";
    TgBot::Bot bot(token);

    



    // Обработчик команды "/start"
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

        TgBot::InlineKeyboardButton::Ptr button1On(new TgBot::InlineKeyboardButton);
        TgBot::InlineKeyboardButton::Ptr button2On(new TgBot::InlineKeyboardButton);
        TgBot::InlineKeyboardButton::Ptr button3On(new TgBot::InlineKeyboardButton);
        TgBot::InlineKeyboardButton::Ptr button4On(new TgBot::InlineKeyboardButton);

        TgBot::InlineKeyboardButton::Ptr button1Off(new TgBot::InlineKeyboardButton);
        TgBot::InlineKeyboardButton::Ptr button2Off(new TgBot::InlineKeyboardButton);
        TgBot::InlineKeyboardButton::Ptr button3Off(new TgBot::InlineKeyboardButton);
        TgBot::InlineKeyboardButton::Ptr button4Off(new TgBot::InlineKeyboardButton);

        button1On->text = "Rele 1 On";
        button1On->callbackData = "button_click_Rele1On";

        button2On->text = "Rele 2 On";
        button2On->callbackData = "button_click_Rele2On";

        button3On->text = "Rele 3 On";
        button3On->callbackData = "button_click_Rele3On";

        button4On->text = "Rele 4 On";
        button4On->callbackData = "button_click_Rele4On";

        button1Off->text = "Rele 1 Off";
        button1Off->callbackData = "button_click_Rele1Off";

        button2Off->text = "Rele 2 Off";
        button2Off->callbackData = "button_click_Rele2Off";

        button3Off->text = "Rele 3 Off";
        button3Off->callbackData = "button_click_Rele3Off";

        button4Off->text = "Rele 4 Off";
        button4Off->callbackData = "button_click_Rele4Off";

        keyboard->inlineKeyboard.push_back({button1On,button2On,button3On,button4On});
        keyboard->inlineKeyboard.push_back({button1Off,button2Off,button3Off,button4Off});

        bot.getApi().sendMessage(message->chat->id, "Добро пожаловать в бота! Напишите /help для списка команд.", nullptr, 0, keyboard);
    });

    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query) {
        if (query->data == "button_click_Rele1On") {
            bot.getApi().sendMessage(query->message->chat->id, "RL1 On");

            EXE.queuePush("83,0,1");
            EXE.update();
        } 
        else if (query->data == "button_click_Rele2On") {
            bot.getApi().sendMessage(query->message->chat->id, "RL2 On");

            EXE.queuePush("83,1,1");
            EXE.update();
        } 
        else if (query->data == "button_click_Rele3On") {
            bot.getApi().sendMessage(query->message->chat->id, "RL3 On");

            EXE.queuePush("83,2,1");
            EXE.update();
        } 
        else if (query->data == "button_click_Rele4On") {
            bot.getApi().sendMessage(query->message->chat->id, "RL4 On");

            EXE.queuePush("83,3,1");
            EXE.update();
        } 
        else if (query->data == "button_click_Rele1Off") {
            bot.getApi().sendMessage(query->message->chat->id, "RL1 Off");

            EXE.queuePush("83,0,0");
            EXE.update();
        } 
        else if (query->data == "button_click_Rele2Off") {
            bot.getApi().sendMessage(query->message->chat->id, "RL2 Off");

            EXE.queuePush("83,1,0");
            EXE.update();
        } 
        else if (query->data == "button_click_Rele3Off") {
            bot.getApi().sendMessage(query->message->chat->id, "RL3 Off");

            EXE.queuePush("83,2,0");
            EXE.update();
        } 
        else if (query->data == "button_click_Rele4Off") {
            bot.getApi().sendMessage(query->message->chat->id, "RL4 Off");

            EXE.queuePush("83,3,0");
            EXE.update();
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





            EXE.queuePush(message->text);
            EXE.update();




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


