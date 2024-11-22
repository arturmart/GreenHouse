#include "MessageQueuing.h"

#include <iostream>
#include <string>


   MessageQueuing::MessageQueuing(){

   }

   void MessageQueuing::push(std::string str){
      CommandList.push(str);
   }

   std::string MessageQueuing::getFront(){
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
   




/*
int main(){


}
*/