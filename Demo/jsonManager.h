#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <cairo.h>
#include <cstdio> 

// Упростим доступ к пространству имен библиотеки
using json = nlohmann::json;

class jsonManager{
    public:
    jsonManager(const std::string& filename);

// Пример функции чтения JSON из файла
    
    json read_json_from_file(const std::string &otherfilename);
    json read_json_from_file();

    // Пример функции записи JSON в файл
    void write_json_to_file(const json& j);

    bool appendToJsonArray( const nlohmann::json& new_element);

    void changeName(const std::string& newfilename);

    bool createImageFromJSON(
    const std::string& jsonData,
    const std::string& outputFile,
    const std::string& title ,
    const std::string& xlabel ,
    const std::string& ylabel );

    private:
    std::string filename;
};

#endif //JSONMANAGER_H

