#include "MessageQueuing.h"

#include <iostream>
#include <string>


   MessageQueuing::MessageQueuing(){

   }

   void MessageQueuing::push(const std::string& str){
      CommandList.push(str);
   }

   std::string  MessageQueuing::getFront(){
      if(!CommandList.empty())
         return CommandList.front();
      else return NULL;
   }


   void MessageQueuing::removeFront(){
      if(!CommandList.empty())
         CommandList.pop();
   }

   bool MessageQueuing::empty(){
      return CommandList.empty();
   }

   void MessageQueuing::printAll(){
      std::cout<<"Q[";
      for (size_t i = 0; i < CommandList.size(); i++)
      {
         std::cout << CommandList.front() << " ";
         CommandList.push(CommandList.front());
         CommandList.pop();
      }
      std::cout<<"]"<<std::endl;
   }
   




/*
int main(){


}
*/