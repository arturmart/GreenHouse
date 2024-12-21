#include "TelegramBot.h"
#include <nlohmann/json.hpp> // Для работы с JSON

#define REPOSITORY_PATH "/home/orangepi/greenhouse/Demo/"

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

    {{"TEXT", "IR1 On"}, {"CALL_BACK_DATA", "IR1On"}},
    {{"TEXT", "IR1 Off"}, {"CALL_BACK_DATA", "IR1Off"}},
    {{"TEXT", "IR1 Auto"}, {"CALL_BACK_DATA", "IR1Auto"}},

    {{"TEXT", "IR2 On"}, {"CALL_BACK_DATA", "IR2On"}},
    {{"TEXT", "IR2 Off"}, {"CALL_BACK_DATA", "IR2Off"}},
    {{"TEXT", "IR2 Auto"}, {"CALL_BACK_DATA", "IR2Auto"}},

    {{"TEXT", "Light1 On"}, {"CALL_BACK_DATA", "Light1On"}},
    {{"TEXT", "Light1 Off"}, {"CALL_BACK_DATA", "Light1Off"}},
    {{"TEXT", "Light1 Auto"}, {"CALL_BACK_DATA", "Light1Auto"}},

    {{"TEXT", "Read New Json Condition"}, {"CALL_BACK_DATA", "ReadNewJsonCond"}}

};

std::vector<std::unordered_map<std::string, std::string>> getterCommands = {
    {{"TEXT", "get Temp In"}, {"CALL_BACK_DATA", "getTemp"}},
    {{"TEXT", "get Temp In 2"}, {"CALL_BACK_DATA", "getTemp2"}},

    {{"TEXT", "get Temp Bake In"}, {"CALL_BACK_DATA", "getTempInBake"}},
    {{"TEXT", "get Temp Bake Out"}, {"CALL_BACK_DATA", "getTempOutBake"}},

    {{"TEXT", "get Temp Out API"}, {"CALL_BACK_DATA", "getTempOut"}},
    
    {{"TEXT", "get Chart"}, {"CALL_BACK_DATA", "getChart"}},
    {{"TEXT", "get Log and Chart"}, {"CALL_BACK_DATA", "getLogChart"}},
    {{"TEXT", "get States"}, {"CALL_BACK_DATA", "getStates"}},

    {{"TEXT", "get Condition Json"}, {"CALL_BACK_DATA", "getConditionJson"}},
    {{"TEXT", "get Condition Tree"}, {"CALL_BACK_DATA", "getConditionTree"}}
};



TgBot::InlineKeyboardMarkup::Ptr TelegramBot::createKeyboard(const std::vector<std::unordered_map<std::string, std::string>>& commands, int columns) {
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
            if(i+j <buttons.size())
                row.push_back(buttons[i + j]);
        }
        keyboard->inlineKeyboard.push_back(row);
    }

    return keyboard;
}

void TelegramBot::setupCommands() {
    bot.getEvents().onCommand("manual", [this](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Մանուալ գործիքների ցուցակ", nullptr, 0, createKeyboard(manualCommands, 3));
    });

    bot.getEvents().onCommand("getter", [this](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Սենսորների գործիքների ցուցակ", nullptr, 0, createKeyboard(getterCommands, 2));
    });
    /*

    bot.getEvents().onCommand("menu", [this](TgBot::Message::Ptr message) {
        // Создаем инлайн-клавиатуру
        TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

        // Первая кнопка меню
        TgBot::InlineKeyboardButton::Ptr button1(new TgBot::InlineKeyboardButton);
        button1->text = "Getter";
        button1->callbackData = "getter";

        // Вторая кнопка меню
        TgBot::InlineKeyboardButton::Ptr button2(new TgBot::InlineKeyboardButton);
        button2->text = "Executor";
        button2->callbackData = "manual";

        // Добавляем кнопки в строку
        std::vector<TgBot::InlineKeyboardButton::Ptr> row = {button1, button2};

        // Добавляем строку в клавиатуру
        keyboard->inlineKeyboard.push_back(row);

        // Отправляем сообщение с клавиатурой
        bot.getApi().sendMessage(message->chat->id, "Выберите опцию:", nullptr, 0, keyboard);
    });
    */

    bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {

        /*

        TgBot::ReplyKeyboardMarkup::Ptr keyboard(new TgBot::ReplyKeyboardMarkup);
        keyboard->resizeKeyboard = true;

        std::vector<TgBot::KeyboardButton::Ptr> row1;
        TgBot::KeyboardButton::Ptr button1(new TgBot::KeyboardButton);
        button1->text = "Getter";
        row1.push_back(button1);

        TgBot::KeyboardButton::Ptr button2(new TgBot::KeyboardButton);
        button2->text = "Executor";

        
        row1.push_back(button2);

        keyboard->keyboard.push_back(row1);
        */

        bot.getApi().sendMessage(message->chat->id, "/manual մանուալ գործիքներ \n /getter Սենսորների գործիքներ", nullptr, 0, nullptr/*keyboard*/);
    });

    bot.getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) {
        for (const auto& hTable : manualCommands) {
            if (query->data == hTable.at("CALL_BACK_DATA")) {
                //bot.getApi().sendMessage(query->message->chat->id, "/manual " + hTable.at("TEXT"));
                funcManual(hTable.at("CALL_BACK_DATA"));
                //alertAllUser("test");
            }
        }
        for (const auto& hTable : getterCommands) {
            if (query->data == hTable.at("CALL_BACK_DATA")) {
                funcGetter(hTable.at("CALL_BACK_DATA"));
                //bot.getApi().sendMessage(query->message->chat->id, "/getter " + hTable.at("TEXT"));
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

    #include <fstream> // Для записи файла

bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
    if (message->document) {
        if (message->document->mimeType == "application/json" || 
            message->document->fileName.substr(message->document->fileName.find_last_of('.') + 1) == "json") {
            
            std::string fileName = message->document->fileName;

            if (fileName.find("Condition_New.json") == 0) {
                try {
                    // Получаем ID файла
                    std::string fileId = message->document->fileId;

                    // Получаем информацию о файле
                    TgBot::File::Ptr file = bot.getApi().getFile(fileId);

                    // Загружаем содержимое файла
                    std::string fileContent = bot.getApi().downloadFile(file->filePath);

                    // Путь для сохранения файла
                    std::string downloadPath = REPOSITORY_PATH + fileName;

                    // Сохраняем файл на диск
                    std::ofstream outFile(downloadPath, std::ios::binary);
                    if (outFile.is_open()) {
                        outFile << fileContent;
                        outFile.close();

                        // Подтверждаем успешное сохранение
                        bot.getApi().sendMessage(message->chat->id, "JSON файл успешно получен и сохранён: " + fileName);
                        funcManual("ReadNewJsonCond");
                    } else {
                        bot.getApi().sendMessage(message->chat->id, "Не удалось сохранить файл на диск: " + downloadPath);
                    }
                } catch (const std::exception& e) {
                    // Обрабатываем ошибки
                    bot.getApi().sendMessage(message->chat->id, "Не удалось скачать JSON файл. Ошибка: " + std::string(e.what()));
                }
            } else {
                bot.getApi().sendMessage(message->chat->id, "Имя файла не соответствует требованиям (должно начинаться с 'Condition_New.json').");
            }
        } else {
            bot.getApi().sendMessage(message->chat->id, "Пожалуйста, отправьте файл в формате JSON.");
        }
    }
});

}

/*TelegramBot::TelegramBot(const std::string& token) : bot(token) {
    setupCommands();
    users = readUsersFromFile(usersPath);
}*/

TelegramBot::TelegramBot(const std::string& token, const std::function<void(const std::string&)>& functionGetter, const std::function<void(const std::string&)>& functionExecutor) : bot(token) {
    users = readUsersFromFile(usersPath);
    setupCommands();
    setFunc(functionGetter, functionExecutor);
    
}

void TelegramBot::setFunc(const std::function<void(const std::string&)>& functionGetter, const std::function<void(const std::string&)>& functionExecutor) {
    funcGetter = functionGetter;
    funcManual = functionExecutor;
}
void TelegramBot::sendMessage(int64_t chatId, const std::string& message) {
    try {
        bot.getApi().sendMessage(chatId, message);
        std::cout << "Сообщение отправлено в чат " << chatId << std::endl;
    } catch (const TgBot::TgException& e) {
        std::cerr << "Ошибка отправки сообщения: " << e.what() << std::endl;
    }
}
void TelegramBot::sendMessageHTML(int64_t chatId, const std::string& message) {
    try {
        bot.getApi().sendMessage(chatId, message,nullptr,nullptr,nullptr, "HTML");
        std::cout << "Сообщение отправлено в чат " << chatId << std::endl;
    } catch (const TgBot::TgException& e) {
        std::cerr << "Ошибка отправки сообщения: " << e.what() << std::endl;
    }
}

void TelegramBot::sendPhoto( int64_t chatId, const std::string& photoPath, const std::string& caption) {
    try {
        std::ifstream file(photoPath);
        if (!file.is_open()) {
            std::cerr << "Ошибка: файл не найден или не может быть открыт: " << photoPath << std::endl;
            return;  // Завершаем выполнение функции, если файл не существует
        }

        bot.getApi().sendPhoto(chatId, TgBot::InputFile::fromFile(photoPath, "image/png"), caption);
        std::cout << "Картинка отправлена в чат " << chatId << std::endl;
    } catch (const TgBot::TgException& e) {
        std::cerr << "Ошибка отправки картинки: " << e.what() << std::endl;
    }
}

void TelegramBot::sendDocument( int64_t chatId, const std::string& documentPath, const std::string& caption) {

    try {
        std::ifstream file(documentPath);
        if (!file.is_open()) {
            std::cerr << "Ошибка: файл не найден или не может быть открыт: " << documentPath << std::endl;
            return;  // Завершаем выполнение функции, если файл не существует
        }

        bot.getApi().sendDocument(chatId, TgBot::InputFile::fromFile(documentPath, "application/octet-stream"), caption);
        std::cout << "Документ отправлен в чат " << chatId << std::endl;
    } catch (const TgBot::TgException& e) {
        std::cerr << "Ошибка отправки документа: " << e.what() << std::endl;
    }
}


void TelegramBot::alertAllUser( const std::string& caption) {
   for (const auto& user : users) {  // Предполагаем, что список пользователей хранится в переменной `users`
        int64_t chatId = std::stoll(user.at("chatId"));
        sendPhoto(chatId, "./tgBot/alert.png", caption);  // Используем уже существующий метод sendPhoto
    }
}

void TelegramBot::sendAllUserPhoto(const std::string& photoPath, const std::string& caption) {
    for (const auto& user : users) {  // Предполагаем, что список пользователей хранится в переменной `users`
        int64_t chatId = std::stoll(user.at("chatId"));
        sendPhoto(chatId, photoPath, caption);  // Используем уже существующий метод sendPhoto
    }
}
void TelegramBot::sendAllUserMessage(const std::string& message) {
    for (const auto& user : users) {  // Предполагаем, что список пользователей хранится в переменной `users`
        int64_t chatId = std::stoll(user.at("chatId"));
        sendMessage(chatId, message);  // Используем уже существующий метод sendMessage
    }
}
void TelegramBot::sendAllUserMessageHTML(const std::string& message) {
    for (const auto& user : users) {  // Предполагаем, что список пользователей хранится в переменной `users`
        int64_t chatId = std::stoll(user.at("chatId"));
        sendMessageHTML(chatId, message);  // Используем уже существующий метод sendMessage
    }
}
void TelegramBot::sendAllUserDocument(const std::string& filePath, const std::string& caption) {
    for (const auto& user : users) {  // Предполагаем, что список пользователей хранится в переменной `users`
        int64_t chatId = std::stoll(user.at("chatId"));
        sendDocument(chatId, filePath, caption);  // Используем уже существующий метод sendDocument
    }
}

void TelegramBot::run() {
    try {
        std::cout << "Бот запущен..." << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            try {
            longPoll.start();
        }
        catch (const std::exception& e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }
            
        }
    } catch (TgBot::TgException& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

std::vector<std::unordered_map<std::string, std::string>> TelegramBot::readUsersFromFile(const std::string& filename) {
    //std::cout << "usersPath: " << filename << std::endl;
    std::ifstream file(filename);
    std::vector<std::unordered_map<std::string, std::string>> users;

    

    if (!file.is_open()) {
        std::cerr << "Ошибка при открытии файла: " << filename << std::endl;
        return users; // Возвращаем пустой список, если файл не существует
    }

    nlohmann::json j;
    file >> j;
    file.close();

    for (const auto& item : j) {
        std::unordered_map<std::string, std::string> user;
        user["chatId"] = std::to_string(item["chatId"].get<int64_t>());
        user["Name"] = item["Name"].get<std::string>();
        //std::cout<<user["Name"]<<" "<<user["chatId"] <<std::endl;
        users.push_back(user);
    }

    return users;
}

void TelegramBot::saveUsersToFile(const std::vector<std::unordered_map<std::string, std::string>>& users, const std::string& filename) {
    nlohmann::json j;
    for (const auto& user : users) {
        nlohmann::json userJson;
        userJson["chatId"] = std::stoll(user.at("chatId"));
        userJson["Name"] = user.at("Name");
        j.push_back(userJson);
    }

    std::ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4); // Запись в файл с отступами
        file.close();
    } else {
        std::cerr << "Ошибка при записи в файл: " << filename << std::endl;
    }
}




/*
void foo(const std::string& dt){
    std::cout<<"foo("<<dt<<")"<<std::endl;
}

//g++ -std=c++17 -o test TelegramBot.cpp -I /path/to/nlohmann/json -lTgBot -lboost_system -lssl -lcrypto -lpthread 

int main() {
    std::string token = "7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c";
    TelegramBot bot(token,foo,foo);
    
    bot.run();
    return 0;
}

*/