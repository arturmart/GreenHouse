#include <iostream>
#include <string>
#include <vector>
#include <stack>



#define BUFFER_SIZE 64
#define DELIMITER1 '/' // First delimiter
#define DELIMITER2 ';' // Second delimiter
#define DELIMITER3 ',' // Third delimiter

#define delimiterPairVector std::vector<std::pair<int,int>>



class TreeParsing {
public:
    class Node {
    public:
        Node(std::string val = "", Node* par = nullptr) : value(val), parent(par) {}


        ~Node() {
            for (Node* child : children) {
                delete child;
            }
            children.clear();
        }

        Node* getChild(int index) {
            /*
            if (index < 0 || index >= children.size()) {
                return nullptr;
            }*/
            return children[index];
        }

        Node* operator[](int index) {
            return getChild(index);
        }

        // Перегрузка оператора * для Node
        
      //   Node& operator*() {
      //       return *this;  // Возвращаем ссылку на текущий объект
      //   }
        
        void setValue(std::string str){
         value = str;
      }

        std::string getValue() const {
            return value;
        }

        int size() const {
            return children.size();
        }


        Node* parent;
        std::string value;
        std::vector<Node*> children;
    };


    TreeParsing(std::string str) : root(new Node(str)), cursor(root) {}

    TreeParsing(const TreeParsing& other){
      root = other.root;
      cursor = other.cursor;
    }

    ~TreeParsing() {
        delete root;
    }

   /*
    Node* getChild(int index) const {
        if (index < 0 || index >= cursor->size()) {
            return nullptr;
        }
        return cursor->children[index];
    }

   
    Node* operator[](int index) {
        if (cursor == nullptr || index < 0 || index >= cursor->size()) {
            return nullptr;
        }
        return cursor->children[index];
    }

    

    const Node* operator[](int index) const {
        if (cursor == nullptr || index < 0 || index >= cursor->size()) {
            return nullptr;
        }
        return cursor->children[index];
    }*/

    Node* getThis(){
      return cursor;
      }

    Node* goToLeaf(std::vector<int> path){
      goToRoot();
      for(int it: path) goToChildren(it); 
      return cursor;
    }

   

    Node* getLeafPtrStay(std::vector<int> path){
      return goToLeaf(path);
    }

    Node* getLeafPtr(std::vector<int> path){
      Node* trg = getLeafPtrStay(path);
      goToRoot();
      return trg;

    }

    std::string getLeaf(std::vector<int> path){
      return getLeafPtrStay(path)->value;
    }

    void addChild(std::string val) {
        Node* newNode = new Node(val, cursor);
        cursor->children.push_back(newNode);
    }

    void goToChildren(int index) {
        if (!cursor) return;
        Node* child = cursor->getChild(index);
        if (child) {
            cursor = child;
        }   
    }

    void gpToLastChildren(){
      goToChildren(cursor->size()-1);
    }

    void goToParent() {
        if (cursor && cursor->parent) {
            cursor = cursor->parent;
        }
    }

    void goToRoot() {
        cursor = root;
    }

    void print() const {
        if (root) {
            print(root, 0);  // Печать с корня
        }
    }

    void print(Node* node, int level = 0) const {
        if (!node) return;

        std::cout << node << "\t";
        for (int i = 0; i < level; i++) {
            std::cout << "- ";
        }
        std::cout << node->value  << std::endl;

        for (Node* child : node->children) {
            print(child, level + 1);
        }
    }

    

    operator Node*() const {
        return cursor;
    }

private:
    Node* root;
    Node* cursor;
};






class DataParsing{
public:

   void setData(std::string data){
      this->data = data;
      delimiterIndex2.clear();
      levelSize = -1;
   }

   void parse(){
     

   }

   int getParsedPartCount(){
      return delimiterIndex2.size();
   }

   void calculateLevelsPartCounts(){
      int levelStack = -1;
      levelsPartCounts.clear();
      

      for(levelStack = 0 ; levelStack<levelSize+1;levelStack++){                        //Start Down
         levelsPartCounts.push_back(0);      //std::cout<<"D"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] ";//print
         
      }     //std::cout<<"\n";//print

      auto it = delimiterIndex2.cbegin();
      levelStack = std::min( it->second, levelSize);

      while(it != delimiterIndex2.cend()){
         
               //std::cout<<levelStack<<">"<<it->second-1<<" ";//print
         for( ; (levelStack > it->second-1)  ; levelStack--)                            //Up
         {
            levelsPartCounts[levelStack]++;     //std::cout<<"U"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] ";//print
 
         }     //std::cout<<"\n"<<levelStack<<"<"<<std::max((it)->second,(it+1)->second)<<" ";//print
         for( ; levelStack < std::max((it)->second,(it+1)->second) ; levelStack++)   //Down
         {
               //std::cout<<"D"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] "; //print
         }     //std::cout << "(" << it->first << "," << it->second << ")" << std::endl;//print
         


         it++;
        
      }
      for(; levelStack >=0; levelStack--)//up
      {
         levelsPartCounts[levelStack]++;     //std::cout<<"U"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] ";//print
      }     //std::cout<<"\n";//print
     
   }

   int parseLevel(const char delimiter,int level = -1){
      
     

      int findIndex = -1;
      int count = 0;

      if(level == -1){
         levelSize ++;
         level = levelSize;
        
      }

      while (true){
         findIndex = data.find(delimiter,findIndex+1);
         if (findIndex != std::string::npos)
         {
         

            if(delimiterIndex2.empty()){
               delimiterIndex2.push_back(std::pair<int,int>(findIndex,level));
               
            }
            else{
               auto it = delimiterIndex2.cbegin();
               bool isAdded = false;

               while(it != delimiterIndex2.cend()){
                  if(it->first > findIndex){
                     isAdded = true;
                     delimiterIndex2.insert(it,std::pair<int,int>(findIndex,level));
                     //if((it-1)->first)
                     
                     break;
                  }
                  it++;
               }

               if(!isAdded){
                  delimiterIndex2.push_back(std::pair<int,int>(findIndex,level));
                  
               }
               
            }
            count++;
         }
         else break;

         

      }
      return(count);
   }

   void printVectorPair() const{
      auto it = delimiterIndex2.cbegin();

      while(it != delimiterIndex2.cend()){
         std::cout << "(" << it->first << "," << it->second << ")" << std::endl;

         it++;

      }

   }

   TreeParsing getParsedListHierarchical() {

      int levelStack = -1;
      levelsPartCounts.clear(); //level counter

      TreeParsing tree(data);

      std::vector<   delimiterPairVector::const_iterator > itPre;    //create each Level Pre Iterator   

      

      for(levelStack = 0 ; levelStack<levelSize+1;levelStack++){         //Start Down    
         itPre.push_back(delimiterIndex2.cbegin()-1);                              //create each element of Level Pre Iterator
         levelsPartCounts.push_back(0);//level counter

         tree.addChild("");
         tree.gpToLastChildren(); //tree.goToChildren(0); 
         //levelsPartCounts[levelStack]++;

         //tree.print(); //print 2
         //std::cout<<"D"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] ";//print
      }  //std::cout<<"\n";//print

      auto it = delimiterIndex2.cbegin();

      levelStack = it->second;
      while(it != delimiterIndex2.cend()){
         //std::cout<<levelStack<<">"<<it->second-1<<" ";//print
         for( ; levelStack > it->second-1   ; levelStack--)                                  //Up
         {
            
            tree.getThis()->setValue(getParsedPartByIterotor(itPre[levelStack],it));
            tree.goToParent();

            levelsPartCounts[levelStack]++;//level counter

            //tree.print();  //print 2
            //std::cout<<"{"<<getParsedPartByIterotor(itPre[levelStack],it)<<"} ";//print
            //std::cout<<"U"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] ";//print
         }  //std::cout<<"\n"<<levelStack<<"<"<<std::max((it)->second,(it+1)->second)<<" ";//print

         for( ; levelStack < (std::max((it)->second ,(it+1)->second)) ; levelStack++)        //Down
         {
           tree.addChild("");
           tree.gpToLastChildren(); //tree.gpToChildren(lastNode[levelStack]);
           itPre[levelStack+1] = it;

           //tree.print(); //print 2
           //std::cout<<"D"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"]"<<"["<<lastNode[levelStack]<<"] "; //print
         } //std::cout << "(" << it->first << "," << it->second << ")" << std::endl;//print
         it++;
      }
      for(; levelStack >=0; levelStack--)                      // End Up
         {
            
            tree.getThis()->setValue(getParsedPartByIterotor(itPre[levelStack],it));
            tree.goToParent();

            levelsPartCounts[levelStack]++;//level counter

            //tree.print(); //print 2
            //std::cout<<"{"<<getParsedPartByIterotor(itPre[levelStack],it)<<"} ";//print
            //std::cout<<"U"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"]"<<"["<<lastNode[levelStack]<<"] ";//print
         }  //std::cout<<"\n";//print
      return tree;
   }


   std::vector<std::vector<std::string>> getParsedListLayared() {
      int levelStack = -1;
      levelsPartCounts.clear(); //level counter

      std::vector<std::vector<std::string>> packets;                             //create Packets List

      std::vector<   delimiterPairVector::const_iterator > itPre;    //create each Level Pre Iterator
      
      /*
      for(int i = 0 ; i<levelSize+1;i++){            

         packets.push_back(std::vector<std::string>{});                              
         itPre.push_back(delimiterIndex2.cbegin()-1);                             
         levelsPartCounts.push_back(0);//level counter
      }*/

      for(levelStack = 0 ; levelStack<levelSize+1;levelStack++){         //Start Down    

         packets.push_back(std::vector<std::string>{});                          //create each element of Packets List       
         itPre.push_back(delimiterIndex2.cbegin()-1);                              //create each element of Level Pre Iterator
         
         levelsPartCounts.push_back(0);//level counter
      }

      auto it = delimiterIndex2.cbegin();
      
      levelStack = it->second;

      while(it != delimiterIndex2.cend()){
         
         for( ; levelStack > it->second-1   ; levelStack--)//Up
         {
            
            levelsPartCounts[levelStack]++;//level counter
            packets[levelStack].push_back(getParsedPartByIterotor(itPre[levelStack],it));

             
         }

         for( ; levelStack < (std::max((it)->second ,(it+1)->second)) ; levelStack++)//Down
         {
           itPre[levelStack+1] = it;
   
         }

         it++;
      }
      for(; levelStack >=0; levelStack--)                      // End Up
         {
            levelsPartCounts[levelStack]++;//level counter
            packets[levelStack].push_back(getParsedPartByIterotor(itPre[levelStack],it));
            
         }
      return packets;
   }

   std::string getParsedPartLayered(int index, int Layer = 0) const{
    

      auto it = delimiterIndex2.cbegin();
      auto itPre = delimiterIndex2.cbegin()-1;
      
      int count = 0;
      while(it != delimiterIndex2.cend()){
         if(it->second <= Layer){

            if(count == index){
               
               break;
            }
            itPre = it;
            count++;
         }
         it++;
      }

      
      return getParsedPartByIterotor(itPre,it);

   }


   std::vector<std::string> getParsedList() const{

      std::vector<std::string> strs;
      delimiterPairVector::const_iterator it = delimiterIndex2.cbegin();

      while(it != delimiterIndex2.cend()){
         strs.push_back(getParsedPartByIterotor(it-1,it));
         it++;
      }
      strs.push_back(getParsedPartByIterotor(it-1,it));


      //std::cout<<indexBegin<<" "<<indexEnd<<std::endl;

      return strs;

   }
   
   std::string getParsedPart(int index) const{

      auto it = delimiterIndex2.cbegin()+index;


      //std::cout<<indexBegin<<" "<<indexEnd<<std::endl;

      return getParsedPartByIterotor(it-1,it);

   }

   
   std::string getParsedPartByIterotor(delimiterPairVector::const_iterator from, delimiterPairVector::const_iterator to) const{
      const int delSize = 1;

      int indexBegin = 0;
      int indexEnd = data.size();

      if(from >= delimiterIndex2.cbegin()){
            indexBegin= (from->first) + delSize;
      }
      if(to < delimiterIndex2.cend()){
            indexEnd = (to)->first;
      }

     
      return data.substr(indexBegin,indexEnd-indexBegin);

   }

   
   

   
int levelSize = -1;
std::string data;
delimiterPairVector delimiterIndex2;
std::vector<int> levelsPartCounts;


};

int main(){

   DataParsing parser;
   parser.setData("83,0,1;83,1,1;83,2,1;83,3,1/61204cfc");
   parser.parseLevel(DELIMITER1);
   parser.parseLevel(DELIMITER2);
   parser.parseLevel(DELIMITER3);

   TreeParsing tp = parser.getParsedListHierarchical();

   tp.print();



   




}
