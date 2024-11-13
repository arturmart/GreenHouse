//TreeParsing.cpp
//for Compile g++ -o TreeParsing TreeParsing.cpp 

#include <iostream>
#include <string>
#include <vector>

#include "TreeParsing.h"



   TreeParsing::Node::Node(std::string val = "", Node* par = nullptr) : value(val), parent(par) {
      //std::cout<<this<<" Node("<<value<<")"<<std::endl;
   }

   TreeParsing::Node::~Node() {
      //std::cout<<this<<" ~Node"<<std::endl;

      // for (Node* child : children) {
      //       delete child;
      // }
      // children.clear();

      clearNode();
   }
   void TreeParsing::Node::clearNode(){
      
      
      for(int i = 0; i < size();i++){
         
         getChild(i)->clearNode();
      }

      children.clear();
      //delete this;
 


   }

   TreeParsing::Node* TreeParsing::Node::getChild(int index) {
      /*
      if (index < 0 || index >= children.size()) {
            return nullptr;
      }*/
      return children[index];
   }

   TreeParsing::Node* TreeParsing::Node::operator[](int index) {
      return getChild(index);
   }

 
   /*
   // Перегрузка оператора * для Node
   Node& operator*() {
      return *this;  // Возвращаем ссылку на текущий объект
   }
   */
   
   void TreeParsing::Node::setValue(std::string str){
   value = str;
}

   std::string TreeParsing::Node::getValue() const {
      return value;
   }

   int TreeParsing::Node::size() const {
      return children.size();
   }


   TreeParsing::TreeParsing(std::string str) : root(new Node(str)), cursor(root) {}

   TreeParsing::TreeParsing(const TreeParsing& other){
   delete root;
   //root = other.root;
   //cursor = other.cursor;
   }

   TreeParsing::TreeParsing(const TreeParsing&& other){
   root = other.root;
   cursor = other.cursor;

   delete(other.root);
   delete(other.cursor);
   }

   TreeParsing::~TreeParsing() {
      clearTree();
      //delete root;
   }

   /*TreeParsing& TreeParsing::operator=(const TreeParsing& other) {
        if (this == &other) return *this;

        clearTree();

        root = other.root;
        cursor = root;

        return *this;

   }*/

   TreeParsing& TreeParsing::operator=( TreeParsing&& other) {
        if (this == &other) return *this;

        //clearTree();

        root = other.root;
        cursor = root;

        other.root = nullptr;
        other.cursor = nullptr;

        return *this;

   }

   
   void TreeParsing::clearTree(){
      root->clearNode();

      delete(root);
      root = nullptr;
      cursor = nullptr;
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

   TreeParsing::Node* TreeParsing::getThis(){
   return cursor;
   }

   TreeParsing::Node* TreeParsing::goToLeaf(std::vector<int> path){
   TreeParsing::goToRoot();
   for(int it: path) TreeParsing::goToChildren(it); 
   return cursor;
   } 

   TreeParsing::Node* TreeParsing::getLeafPtrStay(std::vector<int> path){
   return TreeParsing::goToLeaf(path);
   }

   TreeParsing::Node* TreeParsing::getLeafPtr(std::vector<int> path){
   Node* trg = getLeafPtrStay(path);
   goToRoot();
   return trg;

   }

   std::string TreeParsing::getLeaf(std::vector<int> path){
   return getLeafPtrStay(path)->value;
   }

   void TreeParsing::addChild(std::string val) {
      Node* newNode = new Node(val, cursor);
      cursor->children.push_back(newNode);
   }

   void TreeParsing::goToChildren(int index) {
      if (!cursor) return;
      Node* child = cursor->getChild(index);
      if (child) {
         cursor = child;
      }   
   }

   void TreeParsing::gpToLastChildren(){
   goToChildren(cursor->size()-1);
   }

   void TreeParsing::goToParent() {
      if (cursor && cursor->parent) {
         cursor = cursor->parent;
      }
   }

   void TreeParsing::goToRoot() {
      cursor = root;
   }

   void TreeParsing::print() const {
      if (root) {
         TreeParsing::print(root, 0);  // Печать с корня
      }
   }

   void TreeParsing::print(Node* node, int level = 0) const {
      if (!node) return;

      std::cout << node << "\t";
      for (int i = 0; i < level; i++) {
         std::cout << "- ";
      }
      std::cout << node->value  << std::endl;

      for (Node* child : node->children) {
         TreeParsing::print(child, level + 1);
      }
   }

    TreeParsing::operator Node*() const {
        return cursor;
    }


/*
int main(){
   TreeParsing tp("83,0,1;83,1,1;83,2,1;83,3,1/61204cfc");
   tp.addChild("83,0,1;83,1,1;83,2,1;83,3,1");
   tp.addChild("61204cfc");

   tp.goToChildren(0);
   tp.addChild("83,0,1");
   tp.addChild("83,1,1");
   tp.addChild("83,2,1");
   tp.addChild("83,3,1");

   tp.goToChildren(0);
   tp.addChild("83");
   tp.addChild("0");
   tp.addChild("1");
   tp.goToParent();




   tp.goToChildren(1);
   tp.addChild("83");
   tp.addChild("1");
   tp.addChild("1");
   tp.goToParent();

   tp.goToChildren(2);
   tp.addChild("83");
   tp.addChild("2");
   tp.addChild("1");
   tp.goToParent();

   tp.goToChildren(3);
   tp.addChild("83");
   tp.addChild("3");
   tp.addChild("1");

   tp.goToRoot();

   tp.print();

   std::cout<< tp.getLeafPtr({})       <<" "<<tp.getLeaf({})<<std::endl;
   std::cout<< tp.getLeafPtr({0})       <<" "<<tp.getLeaf({0})<<std::endl;
   std::cout<< tp.getLeafPtr({1})       <<" "<<tp.getLeaf({1})<<std::endl;
   std::cout<< tp.getLeafPtr({0,0})       <<" "<<tp.getLeaf({0,0})<<std::endl;
   std::cout<< tp.getLeafPtr({0,0,0})       <<" "<<tp.getLeaf({0,0,0})<<std::endl;

}
*/
