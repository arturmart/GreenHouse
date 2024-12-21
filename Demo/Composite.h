#ifndef COMPOSITE_H
#define COMPOSITE_H


#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <algorithm>



template <typename T>
class IConditionStrategy {
public:
    virtual ~IConditionStrategy() = default;
    virtual bool evaluate(const std::vector<T>& args) const = 0;
};

// Реализация стратегий
template <typename T>
class GreaterThan : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class LessThan : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class EqualTo : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class NotEqualTo : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class GreaterThanOrEqual : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class LessThanOrEqual : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class InRange : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class OutOfRange : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class NotZero : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class IsZero : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class Always : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class Never : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};

template <typename T>
class ModulusPart:public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override;
};


// Контекст для хранения и применения стратегий
class ConditionContext {
public:

    ConditionContext();
    ~ConditionContext();// Освобождение памяти стратегий
    // Регистрация стратегии

    bool check(const std::string& key, const std::vector<std::string>& args);

    template <typename T>
    bool check(const std::string& key, const std::vector<std::string>& args);
    
    void printStrtegies();

private:
    template <typename T>
    bool checkImpl(const std::string& key, const std::vector<T>& args);

    // Преобразование строковых аргументов в нужный тип
    template <typename T>
    bool convertArgs(const std::vector<std::string>& args, std::vector<T>& convertedArgs);

 
    template <typename T>
    void addStrategy(const std::string& key, IConditionStrategy<T>* strategy);

    template <typename T>
    void clearStrategies();


    // Хранение стратегий для разных типов в хеш-таблице
    template <typename T>
    std::unordered_map<std::string, IConditionStrategy<T>*>& strategies();
};





class Composite {
public:
    class Node {
    public:
        Node(const std::string& title,
            const std::string& cnodition,
            const std::vector <std::string>& conditionArgs,
            const std::string& exe,
            Node* parent = nullptr
            );

        void addChild(Node* child);
        void removeChild(size_t childID);
        Node* getParent();
        Node* getChild(size_t childID = 0);
        size_t getChildSize() const;
        void execute(std::function<void(std::string)> executor);
        std::string getTitle() const;
        std::string getCondition() const;
        std::string getExe() const;

        std::vector<std::string> getCondArgs() const;

    private:
        std::string title;

        std::string cnodition;
        std::vector <std::string> conditionArgs;
        std::string exe;

        std::vector<Node*> children;
        Node* parent;
    };

    Composite(
            std::unordered_map<std::string, std::string>& dataGetter, 
            const std::function<void(std::string)>& executor,
            const std::string& title, 
            const std::string& condition,
            const std::vector<std::string>& condArg, 
            const std::string& exe
        );

    Composite(Composite&& other);
    Composite& operator=(Composite&& other) noexcept; 
    void setRoot(Composite::Node* newRoot) ;
    



    

    void addChild(
        const std::string& title,
        const std::string& condition,
        const std::vector<std::string>& condArg,
        const std::string& exe
    );

    void goToRoot();
    void goToParent();
    void goToChild(size_t childID);
    void printTree();
    std::string getTreeString();
    void executeAll();
    Node* getCurrent();

    std::string getDataGetterString(const std::string& condArgVar);

    bool checkNode(Node* node);

    void printStrtegies();
private:
    std::vector<Node*> getLevelOrderPtr();

    std::vector<std::string> addFirstElement(const std::vector<std::string>& vec, const std::string& value);
    void printTree(Node* node, const std::string& prefix = "", bool isLast = true);
    void getTreeString(Node* node,  std::string& reciveString ,const std::string& prefix = "", bool isLast = true);

    ConditionContext cond;
    std::unordered_map<std::string, std::string>& dataGetter;
    std::function<void(std::string)> executor;
    Node* root;
    Node* current;
};


#endif