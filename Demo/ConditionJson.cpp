#include "ConditionJson.h"


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

