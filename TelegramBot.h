#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <tgbot/tgbot.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>


class TelegramBot {
private:
   const std::string usersPath = "./tgBot/admin.json";
   
    // Объект бота
    TgBot::Bot bot;
    std::vector<std::unordered_map<std::string, std::string>> users;

    // Функции для команд
    std::function<void(const std::string&)> funcGetter;
    std::function<void(const std::string&)> funcManual;
    std::function<void(const std::string&)> funcMaster;

    // Метод для создания клавиатуры
    TgBot::InlineKeyboardMarkup::Ptr createKeyboard(const std::vector<std::unordered_map<std::string, std::string>>& commands, int columns);

    // Настройка команд
    void setupCommands();

   

    

    //метод для чтения пользователей
    std::vector<std::unordered_map<std::string, std::string>> readUsersFromFile(const std::string& filename);
    // Метод для сохранения пользователей
    void saveUsersToFile(const std::vector<std::unordered_map<std::string, std::string>>& users, const std::string& filename);

public:

    // Конструктор с токеном
    //TelegramBot(const std::string& token);

    // Конструктор с функциями
    TelegramBot(const std::string& token, const std::function<void(const std::string&)>& functionGetter, const std::function<void(const std::string&)>& functionExecutor, const std::function<void(const std::string&)>& functionMaster);

    // Установка функций
    void setFunc(const std::function<void(const std::string&)>& functionGetter, const std::function<void(const std::string&)>& functionExecutor, const std::function<void(const std::string&)>& functionMaster);

    // Метод для запуска бота
    void run();

    // Отправка текстового сообщения
   void sendMessage(int64_t chatId, const std::string& message);
   void sendMessageHTML(int64_t chatId, const std::string& message);
    // Отправка фото
    void sendPhoto(int64_t chatId, const std::string& photoPath, const std::string& caption = "");
    // Отправка документа
    void sendDocument(int64_t chatId, const std::string& documentPath, const std::string& caption = "");

    // Отправка фотографии всем пользователям
   void sendAllUserPhoto(const std::string& photoPath, const std::string& caption = "");

   // Отправка сообщения всем пользователям
   void sendAllUserMessage(const std::string& message);
   void sendAllUserMessageHTML(const std::string& message);

   // Отправка документа всем пользователям
   void sendAllUserDocument(const std::string& filePath, const std::string& caption = "");


    // Отправка предупреждения
    void alertAllUser( const std::string& caption = "");
};

#endif // TELEGRAM_BOT_H
