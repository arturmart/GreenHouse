#include "I2CLCD.h"
#include <iostream>
#include <string>


I2CLCD lcd(0x3f);

int main(){
   std::string input;

   if(lcd.initialize()){
      std::cout<<"LCD is Inited!"<<std::endl;
   }
   else{
      std::cout<<"ERROR LCD!"<<std::endl;
      return 0;
   }

   while (true)
   {
      
      std::cout<<"sand Message:";
      std::getline(std::cin,input);
      lcd.clear();
      std::cout<<">"<<input.c_str()<<std::endl;
      lcd.displayString(input.c_str(), 1);
   }
   

}

