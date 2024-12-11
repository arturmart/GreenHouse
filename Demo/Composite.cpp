#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <sstream>



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
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "GreaterThan" << std::endl;
        return args.size() == 2 && args[0] > args[1];
    }
};

template <typename T>
class LessThan : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "LessThan" << std::endl;
        return args.size() == 2 && args[0] < args[1];
    }
};

template <typename T>
class EqualTo : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "EqualTo" << std::endl;
        return args.size() == 2 && args[0] == args[1];
    }
};

template <typename T>
class NotEqualTo : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "NotEqualTo" << std::endl;
        return args.size() == 2 && args[0] != args[1];
    }
};

template <typename T>
class GreaterThanOrEqual : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "GreaterThanOrEqual" << std::endl;
        return args.size() == 2 && args[0] >= args[1];
    }
};

template <typename T>
class LessThanOrEqual : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "LessThanOrEqual" << std::endl;
        return args.size() == 2 && args[0] <= args[1];
    }
};

template <typename T>
class InRange : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "InRange" << std::endl;
        return args.size() == 3 && args[0] >= args[1] && args[0] <= args[2];
    }
};

template <typename T>
class OutOfRange : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "OutOfRange" << std::endl;
        return args.size() == 3 && (args[0] < args[1] || args[0] > args[2]);
    }
};

template <typename T>
class NotZero : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "NotZero" << std::endl;
        return args.size() == 1 && args[0] != 0;
    }
};

template <typename T>
class IsZero : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "IsZero" << std::endl;
        return args.size() == 1 && args[0] == 0;
    }
};

template <typename T>
class Always : public IConditionStrategy<T> {
public:
    bool evaluate(const std::vector<T>& args) const override {
        std::cout << "Always" << std::endl;
        return true;
    }
};


// Контекст для хранения и применения стратегий
class ConditionContext {
public:

    ConditionContext() {

      
        addStrategy<double>(">D", new GreaterThan<double>);
        addStrategy<double>("<D", new LessThan<double>);
        addStrategy<double>("==D", new EqualTo<double>);
        addStrategy<double>(">=D", new GreaterThanOrEqual<double>);
        addStrategy<double>("<=D", new LessThanOrEqual<double>);
        addStrategy<double>("inRangeD", new InRange<double>);
        addStrategy<double>("OutOfRangeD", new OutOfRange<double>);


        addStrategy<long long>(">LL", new GreaterThan<long long>);
        addStrategy<long long>("<LL", new LessThan<long long>);
        addStrategy<long long>("==LL", new EqualTo<long long>);
        addStrategy<long long>(">=LL", new GreaterThanOrEqual<long long>);
        addStrategy<long long>("<=LL", new LessThanOrEqual<long long>);
        addStrategy<long long>("InRangeLL", new InRange<long long>);
        addStrategy<long long>("OutOfRangeLL", new OutOfRange<long long>);


        addStrategy<bool>("NotZeroB", new NotZero<bool>);
        addStrategy<bool>("IsZeroB", new IsZero<bool>);

        addStrategy<double>("Always", new Always<double>);
    }

    // Регистрация стратегии
    

    bool check(const std::string& key, const std::vector<std::string>& args) {
        if (strategies<double>().count(key)) {
            return check<double>(key, args);
        }
        else if (strategies<long long>().count(key)) {
            return check<long long>(key, args);
        }
        else if (strategies<bool>().count(key)) {
            return check<bool>(key, args);
        }
        else {
            std::cerr << "Strategy not found: " << key << std::endl;
            return false;
        }
    }

    template <typename T>
    bool check(const std::string& key, const std::vector<std::string>& args) {
        std::vector<T> convertedArgs;
        if (!convertArgs<T>(args, convertedArgs)) return false;
        return checkImpl(key, convertedArgs);
    }

    // Освобождение памяти стратегий

    ~ConditionContext() {
        // Очистка стратегий для типа `double`
        clearStrategies<double>();
        // Очистка стратегий для типа `long long`
        clearStrategies<long long>();
        // Очистка стратегий для типа `bool`
        clearStrategies<bool>();

    
    }

    
    void printStrtegies() {
        
        std::cout << "Strategies for double:" << std::endl;
        for (const auto& it : strategies<double>()) {
            std::cout << it.first << std::endl;
        }

        std::cout << "Strategies for long long:" << std::endl;
        for (const auto& it : strategies<long long>()) {
            std::cout << it.first << std::endl;
        }

        std::cout << "Strategies for bool:" << std::endl;
        for (const auto& it : strategies<bool>()) {
            std::cout << it.first << std::endl;
        }


    }


private:
    template <typename T>
    bool checkImpl(const std::string& key, const std::vector<T>& args) {
        auto& strategy_map = strategies<T>();
       
        auto it = strategy_map.find(key);
        if (it != strategy_map.end() && it->second) {
            return it->second->evaluate(args);
        }
        
        std::cerr << "Strategy not found: " << key << std::endl;
        return false;
    }

    // Преобразование строковых аргументов в нужный тип
    template <typename T>
    bool convertArgs(const std::vector<std::string>& args, std::vector<T>& convertedArgs) {
        try {
            for (const auto& arg : args) {
                
                    std::cout << arg << " ";
                    std::istringstream iss(arg);
                    T value;
                    iss >> value;
                    if (iss.fail()) {
                        std::cerr << "Failed to convert string to type T: " << arg << std::endl;
                        return false;
                    }
                    convertedArgs.push_back(value);
                
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error during conversion: " << e.what() << std::endl;
            return false;
        }
        return true;
    }

 
    template <typename T>
    void addStrategy(const std::string& key, IConditionStrategy<T>* strategy) {
        strategies<T>()[key] = strategy;
    }

    template <typename T>
    void clearStrategies() {
        auto& strategy_map = strategies<T>();
        for (auto it = strategy_map.begin(); it != strategy_map.end(); ++it) {
            delete it->second; // Удаляем значение
        }
        strategy_map.clear();
       
    }




    // Хранение стратегий для разных типов в хеш-таблице
    template <typename T>
    std::unordered_map<std::string, IConditionStrategy<T>*>& strategies() {
        static std::unordered_map<std::string, IConditionStrategy<T>*> strategy_map;
        return strategy_map;
    }
};





class Composite {
public:
    class Node {
    public:
        Node(const std::string& title,
            const std::string& cnodition,
            const std::vector <std::string>& conditionArgs,
            const std::function<void()>& exe,
            Node* parent = nullptr
            )
            : parent(parent), title(title), cnodition(cnodition), conditionArgs(conditionArgs), executing(exe) {
            std::cout << "Node Constructor. Ndoe Title (" << title << ") Cond (" << cnodition << ")" << std::endl;
            std::cout << "condArg "; for (auto it : conditionArgs) std::cout << it << " "; std::cout << std::endl;
        }

        void addChild(Node* child) {
            children.push_back(child);
        }

        void removeChild(size_t childID) {
            if (childID < children.size())
                children.erase(children.begin() + childID);
            else
                std::cerr << "Node does not have child " << childID << std::endl;
        }

        Node* getParent() {
            if (parent) return parent;
            std::cerr << "Node does not have a parent" << std::endl;
            return nullptr;
        }

        Node* getChild(size_t childID = 0) {
            if (childID < children.size()) {
                return children[childID];
            }
            else {
                std::cerr << "Invalid child ID: " << childID << std::endl;
                return nullptr;
            }
        }


        size_t getChildSize() const {
            return children.size();
        }

        void execute() {
            if (executing) executing();
        }

        std::string getTitle() const {
            return title;
        }
        std::string getCondition() const {
            return cnodition;
        }

        std::string getCondArgVar() const {
            if (conditionArgs.size() > 0)
                return conditionArgs[0];
            else return (NULL);
        }

        std::vector<std::string> getCondArgs() const{

            if(conditionArgs.size()>=2)

                return std::vector<std::string>(conditionArgs.begin() + 1, conditionArgs.end());
            else
                return std::vector<std::string>();
        }

        

    private:
        std::string title;

        std::string cnodition;
        std::vector <std::string> conditionArgs;

        std::function<void()> executing;

        std::vector<Node*> children;
        Node* parent;
    };

    Composite(
        std::unordered_map<
            std::string, std::string>& dataGetter,
            const std::string& title, 
            const std::string& condition,
            const std::vector<std::string>& condArg, 
            const std::function<void()>& exe = NULL
        ): dataGetter(dataGetter){

        std::cout << "Composite Constructor. Ndoe Title (" << title<<") Cond ("<<condition<<")" << std::endl;

        root = new Node(title, condition, condArg, exe);

        
        current = root;
    }

    void addChild(
        const std::string& title,
        const std::string& condition,
        const std::vector<std::string>& condArg,
        const std::function<void()> & exe = NULL
    ) {

        current->addChild(new Node(title, condition, condArg, exe, current));
    }

    void goToRoot() {
        current = root;
    }

    void goToParent() {
        Node* tmp = current->getParent();
        if (tmp)
            current = tmp;
    }

    void goToChild(size_t childID) {
        Node* tmp = current->getChild(childID);
        if (tmp)
            current = tmp;
    }

    void printTree() {
        printTree(root);
    }

    void executeAll() {
        std::vector<Node*> iterators = getLevelOrderPtr();

        for (auto it : iterators) {
            if (checkNode(it))
                std::cout << "\033[32m";
            else
                std::cout << "\033[31m";
            it->execute();
            std::cout << "\033[0m";
        }
    }


    std::string getDataGetterString(const std::string& condArgVar) {

        if (dataGetter.find(condArgVar) != dataGetter.end()) {
            return dataGetter[condArgVar];
        }
        else {
            return "";
        }

        

    }

    bool checkNode(Node* node) {
        return(cond.check(
            node->getCondition(), addFirstElement(node->getCondArgs(), getDataGetterString(node->getCondArgVar()))));



    }

    
    void printStrtegies() {
        cond.printStrtegies();
    }

private:
    std::vector<Node*> getLevelOrderPtr() {
        std::vector<Node*> iterators;

        if (root == nullptr)
            return iterators;

        std::queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            iterators.push_back(node);

            for (size_t i = 0; i < node->getChildSize(); i++) {
                q.push(node->getChild(i));
            }
        }

        return iterators;
    }

    std::vector<std::string> addFirstElement(const std::vector<std::string>& vec, const std::string& value) {
        // Создаем новый вектор, в котором сначала добавляем новый элемент
        if (value.empty() || vec.empty()) return std::vector<std::string>();
        std::vector<std::string> newVec;
        newVec.push_back(value);  // Добавляем элемент в начало

        // Копируем все элементы из исходного вектора
        newVec.insert(newVec.end(), vec.begin(), vec.end());

        return newVec;
    }

    void printTree(Node* node, const std::string& prefix = "", bool isLast = true) {
        if (!node) return;

        std::cout << prefix;

        if (!prefix.empty()) {
            std::cout << (isLast ? "+-- " : "|-- ");
        }

        std::cout << "[" << node->getTitle() << "]" << std::endl;

        std::string childPrefix = prefix + (isLast ? "    " : "|   ");
        for (size_t i = 0; i < node->getChildSize(); ++i) {
            bool lastChild = (i == node->getChildSize() - 1);
            printTree(node->getChild(i), childPrefix, lastChild);
        }
    }



    ConditionContext cond;
    std::unordered_map<std::string, std::string>& dataGetter;
    Node* root;
    Node* current;
};



int main() {
    std::unordered_map<std::string,  std::string> dataGetter;
    dataGetter["var1"] = "5";


    std::cout << "1" << "Always" << std::endl;
    Composite testPattern(dataGetter, "main", "Always", {"0"}, []() { std::cout << "Executing main" << "\n"; });
    
    
    testPattern.printStrtegies();

   

    testPattern.addChild("Child1", ">D", {"var1", "0"}, []() { std::cout << "Executing Child" << 10 << "\n"; });
    testPattern.addChild("Child2", "<D", { "var1", "0" }, []() { std::cout << "Executing Child2\n"; });

    testPattern.goToChild(0); // Переходим к Child1
    testPattern.addChild("Child1.1", ">D", { "var1", "20" }, []() { std::cout << "Executing Child1.1\n"; });

    testPattern.goToParent(); // Возвращаемся к корню
    testPattern.goToChild(1); // Переходим к Child2
    testPattern.addChild("Child2.1", ">D", { "var1", "30" }, []() { std::cout << "Executing Child2.1\n"; });

    testPattern.goToRoot();

    // Печатаем дерево
    testPattern.printTree();

    while (true)
    {
        std::cout << "get var1: ";
        std::cin >> dataGetter["var1"];
        

        testPattern.executeAll();

    }
    

    return 0;
}


