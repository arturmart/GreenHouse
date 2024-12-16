#ifndef CONDITIONJSON_H
#define CONDITIONJSON_H

#include "jsonManager.h"
#include "Composite.h"
#include <iostream>
#include <unordered_map>
#include <string>

extern jsonManager jmCond;



nlohmann::json toJSONRecursively(Composite::Node* node);
void executorPrint(std::string str);
Composite::Node* fromJSONRecursively(const nlohmann::json& nodeJson);

bool validateJsonSyntax(const std::string& filePath, Composite& composite);
void renameAndProcessFiles(const std::function<void(const std::string&)>& sendFile);


#endif
