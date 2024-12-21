

#include "Composite.h"


// Реализация стратегий
template <typename T>
bool GreaterThan<T>::evaluate(const std::vector<T>& args) const  {
    //std::cout << "GreaterThan" << std::endl;
    return args.size() == 2 && args[0] > args[1];
}

template <typename T>
bool  LessThan<T>::evaluate(const std::vector<T>& args) const  {
    //std::cout << "LessThan" << std::endl;
    return args.size() == 2 && args[0] < args[1];
}

template <typename T>
bool  EqualTo<T>::evaluate(const std::vector<T>& args) const  {
        //std::cout << "EqualTo" << std::endl;
        return args.size() == 2 && args[0] == args[1];
}

template <typename T>
bool NotEqualTo<T>::evaluate(const std::vector<T>& args) const {
    //std::cout << "NotEqualTo" << std::endl;
    return args.size() == 2 && args[0] != args[1];
}

template <typename T>
bool GreaterThanOrEqual<T>::evaluate(const std::vector<T>& args) const {
    //std::cout << "GreaterThanOrEqual" << std::endl;
    return args.size() == 2 && args[0] >= args[1];
}

template <typename T>
bool LessThanOrEqual<T>::evaluate(const std::vector<T>& args) const {
    //std::cout << "LessThanOrEqual" << std::endl;
    return args.size() == 2 && args[0] <= args[1];
}

template <typename T>
bool InRange<T>::evaluate(const std::vector<T>& args) const  {
    //std::cout << "InRange" << std::endl;
    return args.size() == 3 && args[0] >= args[1] && args[0] <= args[2];
}

template <typename T>
bool OutOfRange<T>::evaluate(const std::vector<T>& args) const {
    //std::cout << "OutOfRange" << std::endl;
    return args.size() == 3 && (args[0] < args[1] || args[0] > args[2]);
}

template <typename T>
bool NotZero<T>::evaluate(const std::vector<T>& args) const  {
    //std::cout << "NotZero" << std::endl;
    return args.size() == 1 && args[0] != 0;
}

template <typename T>
bool IsZero<T>::evaluate(const std::vector<T>& args) const {
    //std::cout << "IsZero" << std::endl;
    return args.size() == 1 && args[0] == 0;
}

template <typename T>
bool Always<T>::evaluate(const std::vector<T>& args) const {
    //std::cout << "Always" << std::endl;
    return true;
}

template <typename T>
bool Never<T>::evaluate(const std::vector<T>& args) const {
    //std::cout << "Always" << std::endl;
    return false;
}

template <typename T>
bool ModulusPart<T>::evaluate(const std::vector<T>& args) const {
    //arg0 is number we comper
    //arg1 is part
    //arg2 is part counts
    //arg3 is which part we check

    //     ((1734789429  % (600*4))/600) == 1
    //      (data % (part*partCnt))/part == which part we check


    return args.size() == 4 && (((args[0] % (args[1]*args[2]))/args[1]) == args[3]);
}



// Контекст для хранения и применения стратегий
    ConditionContext::ConditionContext() {

      
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

        addStrategy<long long>("%LL", new ModulusPart<long long>);



        addStrategy<bool>("NotZeroB", new NotZero<bool>);
        addStrategy<bool>("IsZeroB", new IsZero<bool>);

        addStrategy<double>("Always", new Always<double>);
        addStrategy<double>("Never", new Never<double>);
    }

    // Регистрация стратегии
    bool ConditionContext::check(const std::string& key, const std::vector<std::string>& args) {
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
    bool ConditionContext::check(const std::string& key, const std::vector<std::string>& args) {
        std::vector<T> convertedArgs;
        if (!convertArgs<T>(args, convertedArgs)) return false;
        return checkImpl(key, convertedArgs);
    }

    // Освобождение памяти стратегий
    ConditionContext::~ConditionContext() {
        // Очистка стратегий для типа `double`
        clearStrategies<double>();
        // Очистка стратегий для типа `long long`
        clearStrategies<long long>();
        // Очистка стратегий для типа `bool`
        clearStrategies<bool>();

    
    }

    void ConditionContext::printStrtegies() {
        
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


    template <typename T>
    bool ConditionContext::checkImpl(const std::string& key, const std::vector<T>& args) {
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
    bool ConditionContext::convertArgs(const std::vector<std::string>& args, std::vector<T>& convertedArgs) {
        try {
            for (const auto& arg : args) {
                
                    // std::cout << arg << " ";
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
    void ConditionContext::addStrategy(const std::string& key, IConditionStrategy<T>* strategy) {
        strategies<T>()[key] = strategy;
    }

    template <typename T>
    void ConditionContext::clearStrategies() {
        auto& strategy_map = strategies<T>();
        for (auto it = strategy_map.begin(); it != strategy_map.end(); ++it) {
            delete it->second; // Удаляем значение
        }
        strategy_map.clear();
       
    }

    // Хранение стратегий для разных типов в хеш-таблице
    template <typename T>
    std::unordered_map<std::string, IConditionStrategy<T>*>& ConditionContext::strategies() {
        static std::unordered_map<std::string, IConditionStrategy<T>*> strategy_map;
        return strategy_map;
    }







        Composite::Node::Node(const std::string& title,
            const std::string& cnodition,
            const std::vector <std::string>& conditionArgs,
            const std::vector <std::string>& exe,
            Composite::Node* parent
            )
            : parent(parent), title(title), cnodition(cnodition), conditionArgs(conditionArgs), exe(exe) {
            //std::cout << "Node Constructor. Ndoe Title (" << title << ") Cond (" << cnodition << ")" << std::endl;
            //std::cout << "condArg "; for (auto it : conditionArgs) std::cout << it << " "; std::cout << std::endl;
        }

        void Composite::Node::addChild(Node* child) {
            children.push_back(child);
        }

        void Composite::Node::removeChild(size_t childID) {
            if (childID < children.size())
                children.erase(children.begin() + childID);
            else
                std::cerr << "Node does not have child " << childID << std::endl;
        }

        Composite::Node* Composite::Node::getParent() {
            if (parent) return parent;
            //std::cerr << "Node does not have a parent" << std::endl;
            return nullptr;
        }

        Composite::Node* Composite::Node::getChild(size_t childID) {
            if (childID < children.size()) {
                return children[childID];
            }
            else {
                std::cerr << "Invalid child ID: " << childID << std::endl;
                return nullptr;
            }
        }

        size_t Composite::Node::getChildSize() const {
            return children.size();
        }

        void Composite::Node::execute(std::function<void(std::string)> executor) {
            if (!exe.empty()) {
                for(auto it : exe) {
                    executor(it);
                }
            }
        }

        std::string Composite::Node::getTitle() const {
            return title;
        }
        std::vector<std::string> Composite::Node::getExe() const {
            return exe;
        }
        std::string Composite::Node::getCondition() const {
            return cnodition;
        }
    
        std::vector<std::string> Composite::Node::getCondArgs() const{

            return conditionArgs;
            /*

            if(conditionArgs.size()>=2)

                return std::vector<std::string>(conditionArgs.begin() + 1, conditionArgs.end());
            else
                return std::vector<std::string>();
                */
        }

        

    

    Composite::Composite(
            std::unordered_map<std::string, std::string>& dataGetter, 
            const std::function<void(std::string)>& executor,
            const std::string& title, 
            const std::string& condition,
            const std::vector<std::string>& condArg, 
            const std::vector<std::string>& exe
        ): dataGetter(dataGetter), executor(executor){

        //std::cout << "Composite Constructor. Ndoe Title (" << title<<") Cond ("<<condition<<")" << std::endl;

        root = new Node(title, condition, condArg, exe);

        
        current = root;
    }
    
    Composite::Composite(Composite&& other) 
    : dataGetter((other.dataGetter)), 
      executor((other.executor)),
      root(std::move(other.root)), 
      current(std::move(other.current)), 
      cond(std::move(other.cond)) {
    // Reset the moved-from object
    other.root = nullptr;
    other.current = nullptr;
    }

Composite& Composite::operator=(Composite&& other) noexcept {
    if (this != &other) {
        // Clean up current object
        delete root;

        // Transfer ownership
        dataGetter = std::move(other.dataGetter);
        executor = std::move(other.executor);
        root = other.root;
        current = other.current;
        cond = std::move(other.cond);

        // Reset the moved-from object
        other.root = nullptr;
        other.current = nullptr;
    }
    return *this;
}
    void Composite::setRoot(Composite::Node* newRoot)  {
         if (root) {
            delete root;  // или другое освобождение ресурсов, если требуется
        }
        this->root = newRoot;
        this->current = newRoot;  // Также можно установить текущий узел в корень
    }

    void Composite::addChild(
        const std::string& title,
        const std::string& condition,
        const std::vector<std::string>& condArg,
        const std::vector<std::string>& exe
    ) {

        current->addChild(new Node(title, condition, condArg, exe, current));
    }

    void Composite::goToRoot() {
        current = root;
    }

    void Composite::goToParent() {
        Node* tmp = current->getParent();
        if (tmp)
            current = tmp;
    }

    void Composite::goToChild(size_t childID) {
        Node* tmp = current->getChild(childID);
        if (tmp)
            current = tmp;
    }

    void Composite::printTree() {
        printTree(root);
    }

    std::string Composite::getTreeString(){
        std::string str;
        getTreeString(root,str);

        return str;
    }

    void Composite::executeAll() {

        if (root == NULL) return;
        goToRoot();


        //LevelOrder

        std::vector<Node*> iterators = getLevelOrderPtr();
        std::vector<bool> iteratorsBools(iterators.size(), false);

        //for(auto it: iterators)std::cout<<it<<std::endl;
        

       

        for (int i = 0; i<iterators.size(); i++) {
            
            //local check
            iteratorsBools[i] = checkNode(iterators[i]);

             //std::cout<<(iterators[i]->getParent())<<" ";
            auto prnt = std::find(iterators.begin(), iterators.end(), iterators[i]->getParent());
            

            if(prnt != iterators.end()){
                //std::cout<<"fnd"<<std::endl;
                int indexPrnt = std::distance(iterators.begin(), prnt);

                iteratorsBools[i] = iteratorsBools[i] && iteratorsBools[indexPrnt];
                if(iteratorsBools[i]){
                    iterators[i]->execute(executor);
                }
               
            }
            else{
                //std::cout<<"nfnd"<<std::endl;
                if(iteratorsBools[i]){
                    iterators[i]->execute(executor);
                }
            }

            /*
            if(checkNode(it)){
               it->execute();
            }
            */
            
        }
        //for(auto it: iteratorsBools)std::cout<<it<<std::endl;
        
    }


    Composite::Node* Composite::getCurrent(){
        return current;
    }

    std::string Composite::getDataGetterString(const std::string& condArgVar) {

        if (dataGetter.find(condArgVar) != dataGetter.end()) {
            return dataGetter[condArgVar];
        }
        else {
            return "";
        }

        

    }

    bool Composite::checkNode(Node* node) {
        


        std::vector<std::string> varToStrArgs;
        std::string varToStrBuf;

        //std::cout<<node->getTitle()<<"(";
        for(auto it : node->getCondArgs()){
            varToStrBuf = getDataGetterString(it);

            if(varToStrBuf != "") varToStrArgs.push_back(varToStrBuf);
            else varToStrArgs.push_back(it);

            //std::cout<<it<<"{";
            //std::cout<<varToStrBuf;
            //std::cout<<"},";
        }

        //std::cout<<")"<<std::endl;


        return(cond.check(
            node->getCondition(), varToStrArgs));



    }

    
    void Composite::printStrtegies() {
        cond.printStrtegies();
    }


    std::vector<Composite::Node*> Composite::getLevelOrderPtr() {
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

    std::vector<std::string> Composite::addFirstElement(const std::vector<std::string>& vec, const std::string& value) {
        // Создаем новый вектор, в котором сначала добавляем новый элемент
        if (value.empty() || vec.empty()) return std::vector<std::string>();
        std::vector<std::string> newVec;
        newVec.push_back(value);  // Добавляем элемент в начало

        // Копируем все элементы из исходного вектора
        newVec.insert(newVec.end(), vec.begin(), vec.end());

        return newVec;
    }

    void Composite::printTree(Composite::Node* node, const std::string& prefix, bool isLast) {
        if (!node) return;

        std::cout << prefix;

        if (!prefix.empty()) {
            std::cout << (isLast ? "+-- " : "|-- ");
        }

        
        if (checkNode(node)){
               if(node->getParent()){
                    if (checkNode(node->getParent())){
                            std::cout << "\033[32m";//[+]
                    }
                    else{
                            std::cout << "\033[33m";//[~]
                    }
               }
               else{
                    std::cout << "\033[32m";//[+]
               }
        }
         else
               std::cout << "\033[31m";//[-]
         

        std::cout << "[" << node->getTitle() << "]" << std::endl;
        
        std::cout << "\033[0m";

        std::string childPrefix = prefix + (isLast ? "    " : "|   ");
        for (size_t i = 0; i < node->getChildSize(); ++i) {
            bool lastChild = (i == node->getChildSize() - 1);
            printTree(node->getChild(i), childPrefix, lastChild);
        }
    }

    void Composite::getTreeString(Composite::Node* node, std::string& reciveString, const std::string& prefix, bool isLast) {
        if (!node) return;

        reciveString += prefix;

        if (!prefix.empty()) {
           reciveString += (isLast ? "+-- " : "|-- ");
        }

        reciveString +=  "[" + node->getTitle() + "]";

        if (checkNode(node)){
               if(node->getParent()){
                    if (checkNode(node->getParent())){
                            reciveString += "[+]";
                    }
                    else{
                            reciveString += "[~]";
                    }
               }
               else{
                    reciveString += "[+]";
               }
        }
         else{
            reciveString += "[-]";
         }

        reciveString += " ("+node->getCondition() + ") { ";

        for(auto it : node->getCondArgs()){
            reciveString += it + " ";
        }
        reciveString += "};\n";
        
         
        
        

        std::string childPrefix = prefix + (isLast ? "    " : "|   ");
        for (size_t i = 0; i < node->getChildSize(); ++i) {
            bool lastChild = (i == node->getChildSize() - 1);
            getTreeString(node->getChild(i), reciveString, childPrefix, lastChild);
        }
    }






/*
int main() {
    std::unordered_map<std::string,  std::string> dataGetter;
    dataGetter["var1"] = "5";
    dataGetter["var2"] = "10";


   
    Composite testPattern(dataGetter, "main", "Always", {}, []() { std::cout << "Executing main" << "\n"; });
    
    
    //testPattern.printStrtegies();

   

    testPattern.addChild("Child1", ">D", {"var1", "0"}, []() { std::cout << "Executing Child" << 10 << "\n"; });
    testPattern.addChild("Child2", "<D", { "var1", "0" }, []() { std::cout << "Executing Child2\n"; });
    testPattern.addChild("Child3", "==D", { "var1", "var2" }, []() { std::cout << "Executing Child3\n"; });

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
        testPattern.printTree();

        testPattern.executeAll();

    }
    

    return 0;
}
*/
