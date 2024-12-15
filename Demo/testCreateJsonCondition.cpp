
#include "ConditionJson.h"
#include <unordered_map>
#include <functional>


// g++ -std=c++17 testCreateJsonCondition.cpp ConditionJson.cpp jsonManager.cpp TimeManager.cpp Composite.cpp -o bin -I/path/to/nlohmann/json


void  executorPrint(std::string str){
    std::cout<<"exe "<<str<<std::endl;
}


int main() {
    std::unordered_map<std::string, std::string> dataGetter;
    dataGetter["temp"] = "5";
    dataGetter["temp2"] = "10";

    // Создаем объект Composite
    Composite testPattern(dataGetter,executorPrint, "main", "Always", {}, "main");

    // Добавляем дочерние элементы
    testPattern.addChild("Child1", ">D", {"temp", "20"}, "Bake_ON" );
    testPattern.addChild("Child2", "<D", {"temp", "20"}, "Bake_OFF" );
    testPattern.addChild("Child3", "==D", {"temp", "temp2"}, "doChild3" );
    /*
    testPattern.goToChild(0);
    testPattern.addChild("Child1.1", ">D", {"temp", "20"}, "doChild1.1" );

    testPattern.goToParent();
    testPattern.goToChild(1);
    testPattern.addChild("Child2.1", ">D", {"temp", "30"}, "doChild2.1" );

    testPattern.goToRoot();
    */
    // Печатаем дерево
    testPattern.printTree();

    // Записываем дерево в файл
    jmCond.write_json_to_file(toJSONRecursively(testPattern.getCurrent()));

    // Чтение из файла
    nlohmann::json readJson = jmCond.read_json_from_file();  // Читаем JSON из файла

    // Восстановление дерева
    Composite::Node* rootNode = fromJSONRecursively(readJson);

    // Создаем новый объект Composite, используя восстановленное дерево
    Composite restoredPattern(dataGetter,executorPrint, "restoredMain", "Always",{}, "revMain");
    restoredPattern.setRoot(rootNode);
    //restoredPattern.setRoot(fromJSONRecursively(readJson));
    

    // Печатаем восстановленное дерево
    restoredPattern.printTree();
    restoredPattern.executeAll();

    std::cout<<restoredPattern.getTreeString();

    return 0;
}
