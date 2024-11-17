
#include "TreeParsing.h"
#include "DataParsing.h"
#include <iostream>
#include <string>
#include <vector>



#define delimiterPairVector std::vector<std::pair<int,int>>



   void DataParsing::setData(std::string data){
      this->data = data;
      delimiterIndex2.clear();
      //levelsPartCounts.clear();
      levelSize = -1;
   }

   TreeParsing DataParsing::parse(std::string data, const char* delimiters, int level){
      setData(data);

      for(int i =0; i< level;i++){
         parseLevel(delimiters[i]);
      }
      //DataParsing::printVectorPair();
   

      return DataParsing::getParsedListHierarchical();

   }

   int DataParsing::getParsedPartCount(){
      return delimiterIndex2.size();
   }

   void DataParsing::calculateLevelsPartCounts(){
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

   int DataParsing::parseLevel(const char delimiter,int level){
      
     

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

   void DataParsing::printVectorPair() const{
      auto it = delimiterIndex2.cbegin();

      while(it != delimiterIndex2.cend()){
         std::cout << "(" << it->first << "," << it->second << ")" << std::endl;

         it++;

      }

   }

   TreeParsing DataParsing::getParsedListHierarchical() {

      int levelStack = -1;
      levelsPartCounts.clear(); //level counter

      TreeParsing tree(data);

      std::vector<   delimiterPairVector::const_iterator > itPre;    //create each Level Pre Iterator   

      auto it = delimiterIndex2.cbegin();

       //std::cout<<"cbegin() "<< &(*delimiterIndex2.cbegin())<<std::endl;//print
       //std::cout<<"cend() "<< &(*delimiterIndex2.cend())<<std::endl;//print

      for(levelStack = 0 ; levelStack<levelSize+1;levelStack++){         //Start Down    
         itPre.push_back(delimiterIndex2.cbegin()-1);                              //create each element of Level Pre Iterator
         levelsPartCounts.push_back(0);//level counter


         if(levelStack <= it->second){
            tree.addChild("");
            tree.gpToLastChildren(); //tree.goToChildren(0); 
         }
         //levelsPartCounts[levelStack]++;

         //tree.print(); //print 2
         //std::cout<<"DS"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] ";//print
      }  //std::cout<<"\n";//print

      

      levelStack = it->second;
      //levelStack = std::min( it->second, levelSize);

      while(it != delimiterIndex2.cend()){
         //std::cout<< &(*it) <<" "<<levelStack<<">"<<it->second-1<<" ";//print
         for( ; levelStack > it->second-1   ; levelStack--)                                  //Up
         {
            
            tree.getThis()->setValue(getParsedPartByIterotor(itPre[levelStack],it));
            tree.goToParent();

            levelsPartCounts[levelStack]++;//level counter

            //tree.print();  //print 2
            //std::cout<<"{"<<getParsedPartByIterotor(itPre[levelStack],it)<<"} ";//print
            //std::cout<<"U"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] ";//print
         }  
         
         //std::cout<<"\n"<< &(*it) <<" "<<levelStack<<"<"<< ((it+1 != delimiterIndex2.cend())?(std::max((it)->second ,(it+1)->second)):(it)->second ) <<" ";//print

         for( ; levelStack < ((it+1 != delimiterIndex2.cend())?(std::max((it)->second ,(it+1)->second)):(it)->second ); levelStack++)        //Down
         {
           tree.addChild("");
           tree.gpToLastChildren(); //tree.gpToChildren(lastNode[levelStack]);
           itPre[levelStack+1] = it;

           //tree.print(); //print 2
           //std::cout<<"D"<<levelStack<<"["<<((levelStack > 0)/* ? levelsPartCounts[levelStack] : '-'*/ )<<"]"; //print
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
            //std::cout<<"UE"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"]";//print
         }  //std::cout<<"\n";//print
      return tree;
   }


   std::vector<std::vector<std::string>> DataParsing::getParsedListLayared() {
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

   std::string DataParsing::getParsedPartLayered(int index, int Layer) const{
    

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


   std::vector<std::string> DataParsing::getParsedList() const{

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
   
   std::string DataParsing::getParsedPart(int index) const{

      auto it = delimiterIndex2.cbegin()+index;


      //std::cout<<indexBegin<<" "<<indexEnd<<std::endl;

      return getParsedPartByIterotor(it-1,it);

   }

   
   std::string DataParsing::getParsedPartByIterotor(delimiterPairVector::const_iterator from, delimiterPairVector::const_iterator to) const{
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

   


