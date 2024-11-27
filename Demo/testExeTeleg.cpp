
//g++ -std=c++17 -o test testExeTeleg.cpp -L/usr/local/include  -lTgBot -lboost_system -lssl -lcrypto -lpthread 

#include <tgbot/tgbot.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

// Глобальные переменные
std::vector<std::unordered_map<std::string, std::string>> manualCommands = {
    {{"TEXT", "Bake On"}, {"CALL_BACK_DATA", "BakeOn"}},
    {{"TEXT", "Bake Off"}, {"CALL_BACK_DATA", "BakeOff"}},
    {{"TEXT", "Bake Auto"}, {"CALL_BACK_DATA", "BakeAuto"}},
    {{"TEXT", "Pump On"}, {"CALL_BACK_DATA", "PumpOn"}},
    {{"TEXT", "Pump Off"}, {"CALL_BACK_DATA", "PumpOff"}},
    {{"TEXT", "Pump Auto"}, {"CALL_BACK_DATA", "PumpAuto"}},
    {{"TEXT", "Falcon1 On"}, {"CALL_BACK_DATA", "Falcon1On"}},
    {{"TEXT", "Falcon1 Off"}, {"CALL_BACK_DATA", "Falcon1Off"}},
    {{"TEXT", "Falcon1 Auto"}, {"CALL_BACK_DATA", "Falcon1Auto"}},
    {{"TEXT", "Falcon2 On"}, {"CALL_BACK_DATA", "Falcon2On"}},
    {{"TEXT", "Falcon2 Off"}, {"CALL_BACK_DATA", "Falcon2Off"}},
    {{"TEXT", "Falcon2 Auto"}, {"CALL_BACK_DATA", "Falcon2Auto"}},
    {{"TEXT", "Falcon3 On"}, {"CALL_BACK_DATA", "Falcon3On"}},
    {{"TEXT", "Falcon3 Off"}, {"CALL_BACK_DATA", "Falcon3Off"}},
    {{"TEXT", "Falcon3 Auto"}, {"CALL_BACK_DATA", "Falcon3Auto"}},
    {{"TEXT", "Falcon4 On"}, {"CALL_BACK_DATA", "Falcon4On"}},
    {{"TEXT", "Falcon4 Off"}, {"CALL_BACK_DATA", "Falcon4Off"}},
    {{"TEXT", "Falcon4 Auto"}, {"CALL_BACK_DATA", "Falcon4Auto"}},
};

std::vector<std::unordered_map<std::string, std::string>> getterCommands = {
    {{"TEXT", "get Temp In"}, {"CALL_BACK_DATA", "getTemp"}},
    {{"TEXT", "get Temp Out API"}, {"CALL_BACK_DATA", "getTempOut"}},
    {{"TEXT", "get Temp Water In"}, {"CALL_BACK_DATA", "getTempWaterIn"}},
    {{"TEXT", "get Temp Water Out"}, {"CALL_BACK_DATA", "getTempWaterOut"}},
    {{"TEXT", "get Chart"}, {"CALL_BACK_DATA", "getChart"}},
    {{"TEXT", "get Log and Chart"}, {"CALL_BACK_DATA", "getLogChart"}},
};

class TelegramBot {
private:
    TgBot::Bot bot;

    std::function<void(const std::string &)> funcGetter;
    std::function<void(const std::string &)> funcManual;
   

    TgBot::InlineKeyboardMarkup::Ptr createKeyboard(const std::vector<std::unordered_map<std::string, std::string>>& commands, int columns) {
        TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
        std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;

        for (const auto& hTable : commands) {
            TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
            button->text = hTable.at("TEXT");
            button->callbackData = hTable.at("CALL_BACK_DATA");
            buttons.push_back(button);
        }

        for (size_t i = 0; i < buttons.size(); i += columns) {
            std::vector<TgBot::InlineKeyboardButton::Ptr> row;
            for (size_t j = 0; j < columns && i + j < buttons.size(); ++j) {
                row.push_back(buttons[i + j]);
            }
            keyboard->inlineKeyboard.push_back(row);
        }

        return keyboard;
    }

    void setupCommands() {
        bot.getEvents().onCommand("manual", [this](TgBot::Message::Ptr message) {
            bot.getApi().sendMessage(message->chat->id, "Մանուալ գործիքների ցուցակ", nullptr, 0, createKeyboard(manualCommands, 3));
        });

        bot.getEvents().onCommand("getter", [this](TgBot::Message::Ptr message) {
            bot.getApi().sendMessage(message->chat->id, "Սենսորների գործիքների ցուցակ", nullptr, 0, createKeyboard(getterCommands, 2));
        });

        bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
            bot.getApi().sendMessage(message->chat->id, "/manual մանուալ գործիքներ \n /getter Սենսորների գործիքներ", nullptr, 0, nullptr);
        });

        bot.getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) {
            for (const auto& hTable : manualCommands) {
                if (query->data == hTable.at("CALL_BACK_DATA")) {
                    bot.getApi().sendMessage(query->message->chat->id, "/manual " + hTable.at("TEXT"));
                    funcManual(hTable.at("CALL_BACK_DATA"));
                    alert(bot, query->message->chat->id, "test");
                }
            }
            for (const auto& hTable : getterCommands) {
                if (query->data == hTable.at("CALL_BACK_DATA")) {
                    funcGetter(hTable.at("CALL_BACK_DATA"));
                    bot.getApi().sendMessage(query->message->chat->id, "/getter " + hTable.at("TEXT"));
                }
            }
        });

        bot.getEvents().onCommand("help", [this](TgBot::Message::Ptr message) {
            bot.getApi().sendMessage(message->chat->id,
                                     "Доступные команды:\n"
                                     "/start - Запустить бота\n"
                                     "/help - Показать справку\n"
                                     "/echo [текст] - Повторить ваше сообщение\n"
                                     "/manual - մանուալ գործիքներ\n"
                                     "/getter - Սենսորների գործիքների ցուցակ\n");
        });

        bot.getEvents().onCommand("echo", [this](TgBot::Message::Ptr message) {
            std::string reply = "Вы сказали: ";
            if (message->text.size() > 6) { // "/echo " - это 6 символов
                reply += message->text.substr(6);
            } else {
                reply += "(ничего)";
            }
            bot.getApi().sendMessage(message->chat->id, reply);
        });

        bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
            if (!message->text.empty() && message->text[0] == '/') {
                std::string command = message->text.substr(0, message->text.find(' '));
                std::string args = message->text.substr(message->text.find(' ') + 1);

                // Проверяем, является ли команда известной
                if (command != "/start" && command != "/help" && command != "/manual" && command != "/getter" && command != "/echo") {
                    bot.getApi().sendMessage(message->chat->id, "Неизвестная команда. Напишите /help для списка доступных команд.");
                } else {
                    std::cout << "Получена команда: " << command << ", аргументы: " << args << std::endl;
                }
            } else {
                std::cout << "Получено сообщение: " << message->text << std::endl;
            }
        });
    }

public:
    TelegramBot(const std::string& token) : bot(token) {
        setupCommands();
    }
    TelegramBot(const std::string& token, const std::function<void(const std::string &)>& functionGetter, const std::function<void(const std::string &)>& functionExecutor) : bot(token) {
        setupCommands();
        setFunc(functionGetter, functionExecutor);
    }

    void setFunc(const std::function<void(const std::string &)>& functionGetter, const std::function<void(const std::string &)>& functionExecutor) {
        funcGetter = functionGetter;
        funcManual = functionExecutor;
    }

    void sendPhoto(TgBot::Bot& bot, int64_t chatId, const std::string& photoPath, const std::string& caption = "") {
        try {
            bot.getApi().sendPhoto(chatId, TgBot::InputFile::fromFile(photoPath, "image/png"), caption);
            std::cout << "Картинка отправлена в чат " << chatId << std::endl;
        } catch (const TgBot::TgException& e) {
            std::cerr << "Ошибка отправки картинки: " << e.what() << std::endl;
        }
    }

    void alert(TgBot::Bot& bot, int64_t chatId, const std::string& caption = ""){
        sendPhoto(bot,chatId, "./tgBot/alert.png", caption);

    }

    void run() {
        try {
            std::cout << "Бот запущен..." << std::endl;
            TgBot::TgLongPoll longPoll(bot);
            while (true) {
                longPoll.start();
            }
        } catch (TgBot::TgException& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }
};

void foo(const std::string& dt){
    std::cout<<"foo("<<dt<<")"<<std::endl;
}

int main() {
    std::string token = "7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c";
    TelegramBot bot(token,foo,foo);
    
    bot.run();
    return 0;
}
