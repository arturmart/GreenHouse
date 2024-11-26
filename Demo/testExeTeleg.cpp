#include <tgbot/tgbot.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>


//g++ -std=c++17 -o test testExeTeleg.cpp -L/usr/local/include  -lTgBot -lboost_system -lssl -lcrypto -lpthread 

std::vector<std::unordered_map<std::string,std::string>> manualCommands={
    {{"TEXT","Bake On"},{"CALL_BACK_DATA","BakeOn"}},
    {{"TEXT","Bake Off"},{"CALL_BACK_DATA","BakeOff"}},
    {{"TEXT","Bake Auto"},{"CALL_BACK_DATA","BakeAuto"}},

    {{"TEXT","Pump On"},{"CALL_BACK_DATA","PumpOn"}},
    {{"TEXT","Pump Off"},{"CALL_BACK_DATA","PumpOff"}},
    {{"TEXT","Pump Auto"},{"CALL_BACK_DATA","PumpAuto"}},

    {{"TEXT","Falcon1 On"},{"CALL_BACK_DATA","Falcon1On"}},
    {{"TEXT","Falcon1 Off"},{"CALL_BACK_DATA","Falcon1Off"}},
    {{"TEXT","Falcon1 Auto"},{"CALL_BACK_DATA","Falcon1Auto"}},

    {{"TEXT","Falcon2 On"},{"CALL_BACK_DATA","Falcon2On"}},
    {{"TEXT","Falcon2 Off"},{"CALL_BACK_DATA","Falcon2Off"}},
    {{"TEXT","Falcon2 Auto"},{"CALL_BACK_DATA","Falcon2Auto"}},

    {{"TEXT","Falcon3 On"},{"CALL_BACK_DATA","Falcon3On"}},
    {{"TEXT","Falcon3 Off"},{"CALL_BACK_DATA","Falcon3Off"}},
    {{"TEXT","Falcon3 Auto"},{"CALL_BACK_DATA","Falcon3Auto"}},

    {{"TEXT","Falcon4 On"},{"CALL_BACK_DATA","Falcon4On"}},
    {{"TEXT","Falcon4 Off"},{"CALL_BACK_DATA","Falcon4Off"}},
    {{"TEXT","Falcon4 Auto"},{"CALL_BACK_DATA","Falcon4Auto"}}
    
};

int main() {
    // Замените "YOUR_BOT_TOKEN" на ваш реальный токен бота
    std::string token = "7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c";
    TgBot::Bot bot(token);

    


    // Обработчик команды "/manual"
    bot.getEvents().onCommand("manual", [&bot](TgBot::Message::Ptr message) {
        TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

        std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;

        for(auto hTable : manualCommands){
            TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
            button->text = hTable["TEXT"];
            button->callbackData = hTable["CALL_BACK_DATA"];

            buttons.push_back(button);

        }

        //avelacnel keyline i mech bajanumov 3 ev heto feedbackneri mech htable ov

        //Bake
        

        TgBot::InlineKeyboardButton::Ptr button_BakeOff(new TgBot::InlineKeyboardButton);
        button_BakeOff->text = "Bake Off";
        button_BakeOff->callbackData = "BakeOff";

        TgBot::InlineKeyboardButton::Ptr button_BakeAuto(new TgBot::InlineKeyboardButton);
        button_BakeAuto->text = "Bake Auto";
        button_BakeAuto->callbackData = "BakeAuto";

        //Pump
        TgBot::InlineKeyboardButton::Ptr button_PumpOn(new TgBot::InlineKeyboardButton);
        button_PumpOn->text = "Pump On";
        button_PumpOn->callbackData = "PumpOn";

        TgBot::InlineKeyboardButton::Ptr button_PumpOff(new TgBot::InlineKeyboardButton);
        button_PumpOff->text = "Pump Off";
        button_PumpOff->callbackData = "PumpOff";

        TgBot::InlineKeyboardButton::Ptr button_PumpAuto(new TgBot::InlineKeyboardButton);
        button_PumpAuto->text = "Pump Auto";
        button_PumpAuto->callbackData = "PumpAuto";

        //Falcon 1
        TgBot::InlineKeyboardButton::Ptr button_Falcon1On(new TgBot::InlineKeyboardButton);
        button_Falcon1On->text = "Falcon1 On";
        button_Falcon1On->callbackData = "Falcon1On";

        TgBot::InlineKeyboardButton::Ptr button_Falcon1Off(new TgBot::InlineKeyboardButton);
        button_Falcon1Off->text = "Falcon 1 Off";
        button_Falcon1Off->callbackData = "Falcon1Off";

        TgBot::InlineKeyboardButton::Ptr button_Falcon1Auto(new TgBot::InlineKeyboardButton);
        button_Falcon1Auto->text = "Falcon 1 Auto";
        button_Falcon1Auto->callbackData = "Falcon1Auto";

        //Falcon 2
        TgBot::InlineKeyboardButton::Ptr button_Falcon2On(new TgBot::InlineKeyboardButton);
        button_Falcon2On->text = "Falcon 2 On";
        button_Falcon2On->callbackData = "Falcon2On";

        TgBot::InlineKeyboardButton::Ptr button_Falcon2Off(new TgBot::InlineKeyboardButton);
        button_Falcon2Off->text = "Falcon 2 Off";
        button_Falcon2Off->callbackData = "Falcon2Off";

        TgBot::InlineKeyboardButton::Ptr button_Falcon2Auto(new TgBot::InlineKeyboardButton);
        button_Falcon2Auto->text = "Falcon 2 Auto";
        button_Falcon2Auto->callbackData = "Falcon2Auto";

        //Falcon 3
        TgBot::InlineKeyboardButton::Ptr button_Falcon3On(new TgBot::InlineKeyboardButton);
        button_Falcon3On->text = "Falcon 3 On";
        button_Falcon3On->callbackData = "Falcon3On";

        TgBot::InlineKeyboardButton::Ptr button_Falcon3Off(new TgBot::InlineKeyboardButton);
        button_Falcon3Off->text = "Falcon 3 Off";
        button_Falcon3Off->callbackData = "Falcon3Off";

        TgBot::InlineKeyboardButton::Ptr button_Falcon3Auto(new TgBot::InlineKeyboardButton);
        button_Falcon3Auto->text = "Falcon 3 Auto";
        button_Falcon3Auto->callbackData = "Falcon3Auto";

        //Falcon 4
        TgBot::InlineKeyboardButton::Ptr button_Falcon4On(new TgBot::InlineKeyboardButton);
        button_Falcon4On->text = "Falcon 4 On";
        button_Falcon4On->callbackData = "Falcon4On";

        TgBot::InlineKeyboardButton::Ptr button_Falcon4Off(new TgBot::InlineKeyboardButton);
        button_Falcon4Off->text = "Falcon 4 Off";
        button_Falcon4Off->callbackData = "Falcon4Off";

        TgBot::InlineKeyboardButton::Ptr button_Falcon4Auto(new TgBot::InlineKeyboardButton);
        button_Falcon4Auto->text = "Falcon 4 Auto";
        button_Falcon4Auto->callbackData = "Falcon4Auto";

       

        // Добавляем строки кнопок в клавиатуру
        keyboard->inlineKeyboard.push_back({button_BakeOn, button_BakeOff, button_BakeAuto});
        keyboard->inlineKeyboard.push_back({button_PumpOn, button_PumpOff, button_PumpAuto});
        keyboard->inlineKeyboard.push_back({button_Falcon1On, button_Falcon1Off, button_Falcon1Auto});
        keyboard->inlineKeyboard.push_back({button_Falcon2On, button_Falcon2Off, button_Falcon2Auto});
        keyboard->inlineKeyboard.push_back({button_Falcon3On, button_Falcon3Off, button_Falcon3Auto});
        keyboard->inlineKeyboard.push_back({button_Falcon4On, button_Falcon4Off, button_Falcon4Auto});



        bot.getApi().sendMessage(message->chat->id, "Մանուալ գործիքների ցուցակ", nullptr, 0, keyboard);
    });

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

        bot.getApi().sendMessage(message->chat->id, "/manual մանուալ գործիքներ", nullptr, 0, keyboard);
    });

    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query) {
        if (query->data == "Rele1On") {
            bot.getApi().sendMessage(query->message->chat->id, "RL1 On");
            // EXE.queuePush("83,0,1");
            // EXE.update();
        } 
        else if (query->data == "Rele2On") {
            bot.getApi().sendMessage(query->message->chat->id, "RL2 On");
            // EXE.queuePush("83,1,1");
            // EXE.update();
        } 
        else if (query->data == "Rele3On") {
            bot.getApi().sendMessage(query->message->chat->id, "RL3 On");
            // EXE.queuePush("83,2,1");
            // EXE.update();
        } 
        else if (query->data == "Rele4On") {
            bot.getApi().sendMessage(query->message->chat->id, "RL4 On");
            // EXE.queuePush("83,3,1");
            // EXE.update();
        } 
        else if (query->data == "Rele1Off") {
            bot.getApi().sendMessage(query->message->chat->id, "RL1 Off");
            // EXE.queuePush("83,0,0");
            // EXE.update();
        } 
        else if (query->data == "Rele2Off") {
            bot.getApi().sendMessage(query->message->chat->id, "RL2 Off");
            // EXE.queuePush("83,1,0");
            // EXE.update();
        } 
        else if (query->data == "Rele3Off") {
            bot.getApi().sendMessage(query->message->chat->id, "RL3 Off");
            // EXE.queuePush("83,2,0");
            // EXE.update();
        } 
        else if (query->data == "Rele4Off") {
            bot.getApi().sendMessage(query->message->chat->id, "RL4 Off");
            // EXE.queuePush("83,3,0");
            // EXE.update();
        } 
        else {
            bot.getApi().sendMessage(query->message->chat->id, "Неизвестное действие.");
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


