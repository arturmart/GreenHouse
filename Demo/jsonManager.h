#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <filesystem>


// Упростим доступ к пространству имен библиотеки
using json = nlohmann::json;

class jsonManager{
    public:
    jsonManager(const std::string& filename);

// Пример функции чтения JSON из файла
    
    json read_json_from_file(const std::string &otherfilename);
    json read_json_from_file();
    bool file_exists(const std::string& filename);
    bool create_backup(const std::string& filename);

    // Пример функции записи JSON в файл
    void write_json_to_file(const json& j);
    bool appendToJsonArray( const nlohmann::json& new_element);
    //void changeName(const std::string& newfilename);
    bool create_empty_json_file();
    void setFileName(const std::string& newFileName);

    std::vector<std::string> getFilesInDir(const std::string& path);//new Version TEST001

    private:
    std::string filename;
    
};

#endif //JSONMANAGER_H
