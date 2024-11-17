#ifndef MESSAGEQUEUING_H
#define MESSAGEQUEUING_H

#include <iostream>
#include <queue>
#include <string>



class MessageQueuing{
   public:
   MessageQueuing();

   void push(std::string str);

   std::string getFront();

   void removeFront();

   bool empty();
   

   private:
   
   std::queue<std::string> CommandList;



};

#endif // MESSAGEQUEUING_H