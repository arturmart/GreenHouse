#include "ConditionJson.h"

#include <fstream>
#include <filesystem>
#include <functional>



jsonManager jmCond("Condition.json");


nlohmann::json toJSONRecursively(Composite::Node* node) {
    if (!node) return nullptr;

    nlohmann::json nodeJson;
    nodeJson["title"] = node->getTitle();
    nodeJson["condition"] = node->getCondition();
    nodeJson["conditionArgs"] = node->getCondArgs();
    nodeJson["executing"] = node->getExe();

    // Для каждого дочернего элемента вызываем рекурсивно
    std::vector<nlohmann::json> childrenJson;
    for (size_t i = 0; i < node->getChildSize(); ++i) {
        nlohmann::json childJson = toJSONRecursively(node->getChild(i));
        childrenJson.push_back(childJson);
    }

    if (!childrenJson.empty()) {
        nodeJson["children"] = childrenJson;
    }

    return nodeJson;
}



Composite::Node* fromJSONRecursively(const nlohmann::json& nodeJson) {
    if (nodeJson.is_null()) {
        return nullptr;
    }

    // Получаем заголовок, условие и аргументы
    std::string title = nodeJson["title"];
    std::string condition = nodeJson["condition"];
    std::vector<std::string> conditionArgs = nodeJson["conditionArgs"].get<std::vector<std::string>>();
    std::string exe = nodeJson["executing"];

   

    // Создаем узел
    Composite::Node* node = new Composite::Node(title, condition, conditionArgs,exe);

    // Рекурсивно добавляем дочерние элементы
    if (nodeJson.contains("children")) {
        for (const auto& childJson : nodeJson["children"]) {
            Composite::Node* childNode = fromJSONRecursively(childJson);
            node->addChild(childNode);
        }
    }

    return node;
}



namespace fs = std::filesystem;

bool validateJsonSyntax(const std::string& filePath, Composite& composite) {
    try {
        std::ifstream inputFile(filePath);
        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open the file " << filePath << std::endl;
            return false;
        }

        nlohmann::json jsonData;
        inputFile >> jsonData;

        // Преобразуем JSON в Composite
        Composite::Node* rootNode = fromJSONRecursively(jsonData); // используйте вашу функцию
        if (rootNode == nullptr) {
            std::cerr << "Error: Failed to parse the JSON into a Composite structure" << std::endl;
            return false;
        }

        // Если синтаксис правильный, добавляем в Composite
        composite.setRoot(rootNode); // или любой другой метод для добавления узла в Composite
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error: Exception while parsing JSON: " << e.what() << std::endl;
        return false;
    }
}

void renameAndProcessFiles(const std::function<void(const std::string&)>& sendFile) {
    try {
        // Переименовать Condition.json в Condition_Old.json
        fs::rename("Condition.json", "Condition_Old.json");

        // Отправить файл в бота и удалить старый файл
        sendFile("Condition_Old.json");
        fs::remove("Condition_Old.json");

        // Переименовать Condition_New.json в Condition.json
        fs::rename("Condition_New.json", "Condition.json");

        std::cout << "Files have been successfully renamed and processed." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error during file renaming and processing: " << e.what() << std::endl;
    }
}