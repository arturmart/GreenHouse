#ifndef MESSAGEQUEUING_H
#define MESSAGEQUEUING_H

#include <iostream>
#include <queue>
#include <string>
#include <utility>



class MessageQueuing{
   public:
   MessageQueuing();

   void push(const std::string& str);
   std::string  getFront();
   void removeFront();
   bool empty();
   void printAll();
   

   private:
   
   std::queue<std::string> CommandList;



};

#endif // MESSAGEQUEUING_H