#include <iostream>
#include <string>
#include <vector>
#include <stack>


#define BUFFER_SIZE 64
#define DELIMITER1 '/' // First delimiter
#define DELIMITER2 ';' // Second delimiter
#define DELIMITER3 ',' // Third delimiter

#define delimiterPairVector std::vector<std::pair<int,int>>



class TreeParsing{
   private:
   class Node{
      public:

      Node(std::string val = NULL , Node* par = nullptr) : value(val), parent(par){
         
      }
      
      ~Node() {
         children.clear();
      }
      Node* operator[](int index)
         {
            return children[index];
         }

          // Conversion operator to std::string
      operator std::string() const {
            return value;
         }


      Node* parent;
      std::string value;
      std::vector<Node*> children;
      
   };
public:
   TreeParsing(std::string str) : root(new Node(str)){
      currsor = root;

   }

   Node* getChild(int index){
      return currsor->children[index];
   }
   Node* operator[](int index)
   {
            return getChild(index);
   }

   void newCildren(std::string val){

      Node* newNode = new Node(val,currsor);
      currsor->children.push_back(newNode);

   }

   void goToChildren(int index){
      currsor = &(currsor[index]);
   }
   void goToParrent(int index){
      currsor = currsor->parent;
   }

   void print(){
      print(currsor,0);
   }

   void print(Node* node, int level = 0) const {

            for(int i = 0; i<level;i++)std::cout << "- ";

            std::cout << currsor->value << std::endl;

            std::vector<Node*> vec = currsor->children;

            if(vec.empty()) {
       
            }
            else{
               
               for ( Node* child : vec) {
                 
                  print(child, level+1);
               }

            }

            
   }

   

   Node* root;
   Node* currsor;



};

   class Node{
      public:
         //constructors
         Node(std::string val, Node* par): value(val), parent(par){

         }
         Node(std::string val): value(val){

         }
         Node(){

         }
         ~Node() {
            deleteAllChildren();
         }

         
         Node& getChild(int index) const{
            if (index >= 0 && index < children.size()) {
               return *(children[index]);
            }else {
            throw std::out_of_range("Index out of range");
         }
            
         }
         void newChild(std::string val){
             Node* newNode = new Node(val,this);
             children.push_back(newNode);
             
         }

         void  deleteChild(int index){
            if (index >= 0 && index < children.size()) {
               Node* temp = children[index];
               children.erase(children.cbegin()+index);
               delete temp;
            }
         }
         void  deleteAllChildren(){
            while (!children.empty()) {
               deleteChild(0);
            }
            /*
            for (Node* child : children) {
            delete child;
            }
            children.clear();
            */
            
         }

          Node&  operator=(const Node& second)
         {
            // Check for self-assignment
            if (this != &second) {
                  value = second.value;
                  deleteAllChildren();
                  children = second.children;
            }
            return *this;
         }
         Node&  operator=(const std::string str)
         {
            // Check for self-assignment
           
            value = str;
            return *this;
         }
    

         Node& operator[](int index)
         {
            return getChild(index);
         }

          // Conversion operator to std::string
         operator std::string() const {
            return value;
         }

         

      private:

      Node* parent;

      std::string value;
      std::vector<Node*> children;
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
/*
   Node& getParsedListHierarchical() {
      int levelStack = -1;
      levelsPartCounts.clear(); //level counter

      Node tree(data);

      std::vector<   delimiterPairVector::const_iterator > itPre;    //create each Level Pre Iterator
      std::vector< int> lastIndexes;
      

      for(levelStack = 0 ; levelStack<levelSize+1;levelStack++){         //Start Down    

         

        
         itPre.push_back(delimiterIndex2.cbegin()-1);                              //create each element of Level Pre Iterator
         levelsPartCounts.push_back(0);//level counter

         lastIndexes.push_back(0);
      }

      

      auto it = delimiterIndex2.cbegin();

      for(int levelStack =0;levelStack<it->second+1;levelStack++){
         if(levelStack) tree.newChild(" ");
         else

      }

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
*/

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
   
TreeParsing tp("main");
tp.newCildren("level1(1)");
tp.newCildren("level1(2)");
tp.print();


}
