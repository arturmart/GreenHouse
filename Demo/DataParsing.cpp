#include <iostream>
#include <string>
#include <vector>
#include <stack>


#define BUFFER_SIZE 64
#define DELIMITER1 '/' // First delimiter
#define DELIMITER2 ';' // Second delimiter
#define DELIMITER3 ',' // Third delimiter



class DataParsing{
public:

   void setData(std::string data){
      this->data = data;
      delimiterIndex2.clear();
   }

   void parse(){

   }

   int getParsedPartCount(){
      return delimiterIndex2.size();
   }

   std::vector<int> calculateLevelsPartCounts(int levelCount = 0){
      int levelStack = -1;
      levelsPartCounts.clear();
      

      for(levelStack = 0 ; levelStack<levelCount;levelStack++){                        //Start Down
         levelsPartCounts.push_back(0);      std::cout<<"D"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] ";//print
         
      }     std::cout<<"\n";//print

      auto it = delimiterIndex2.cbegin();
      levelStack = std::min( it->second, levelCount-1);

      while(it != delimiterIndex2.cend()){
         
               std::cout<<levelStack<<">"<<it->second-1<<" ";//print
         for( ; (levelStack > it->second-1)  ; levelStack--)                            //Up
         {
            levelsPartCounts[levelStack]++;     std::cout<<"U"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] ";//print
 
         }     std::cout<<"\n"<<levelStack<<"<"<<std::max((it)->second,(it+1)->second)<<" ";//print
         for( ; levelStack < std::max((it)->second,(it+1)->second) ; levelStack++)   //Down
         {
               std::cout<<"D"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] "; //print
         }     std::cout << "(" << it->first << "," << it->second << ")" << std::endl;//print
         


         it++;
        
      }
      for(; levelStack >=0; levelStack--)//up
      {
         levelsPartCounts[levelStack]++;     std::cout<<"U"<<levelStack<<"["<<levelsPartCounts[levelStack]<<"] ";//print
      }     std::cout<<"\n";//print
      return levelsPartCounts;
   }

   int parseLevel(const char delimiter,int level = 0){
      
     

      int findIndex = -1;
      int count = 0;

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

   void printVectorPair(std::vector<std::pair<int,int>> vecPair) const{
      auto it = vecPair.cbegin();

      while(it != vecPair.cend()){
         std::cout << "(" << it->first << "," << it->second << ")" << std::endl;

         it++;

      }



   }

   
   std::vector<std::vector<std::string>> getParsedListLayared(int levelCount = 0) {
      int levelStack = -1;
      levelsPartCounts.clear(); //level counter

      std::vector<std::vector<std::string>> packets;                             //create Packets List

      std::vector<   std::vector<std::pair<int,int>>::const_iterator > itPre;    //create each Level Pre Iterator
      

      for(levelStack = 0 ; levelStack<levelCount;levelStack++){         //Start Down    

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

   
   
   std::string getParsedPart(int index) const{

      auto it = delimiterIndex2.cbegin()+index;


      //std::cout<<indexBegin<<" "<<indexEnd<<std::endl;

      return getParsedPartByIterotor(it-1,it);

   }

   
   std::string getParsedPartByIterotor(std::vector<std::pair<int,int>>::const_iterator from, std::vector<std::pair<int,int>>::const_iterator to) const{
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

   
   

   

std::string data;
std::vector<std::pair<int,int>> delimiterIndex2;
std::vector<int> levelsPartCounts;


};

int main(){
   DataParsing parser;

   parser.setData("83,0,1;83,1,1;83,2,1;83,3,1/61204cfc");
   //parser.setData("83CC0CC1BB83CC1CC1BB83CC2CC1BB83CC3CC1AA61204cfc");
   //parser.setData("1,2,3,4;5,6,7,8;9,10,11,12;13,14,15,16/a,b,c,d;e,f,g,h;i,j,k,l;m,n,o,p/A,B,C,D;E,F,G,H;I,J,K,L;M,N,O,P");
   
   parser.parseLevel(DELIMITER1);
   parser.parseLevel(DELIMITER2,1);
   parser.parseLevel(DELIMITER3,2);

   parser.calculateLevelsPartCounts(3);
   

   std::cout<< parser.getParsedPartByIterotor(parser.delimiterIndex2.cbegin()+11,parser.delimiterIndex2.cbegin()+58)<< std::endl;
   std::cout<< parser.getParsedPart(12)<< std::endl;
   std::cout<< std::endl;

   /*
   std::vector<std::vector<std::string>> dt = parser.getParsedListLayared(3);
   for(int i = 0; i<3;i++){
      for(int j = 0; j < parser.levelsPartCounts[i];j++)
            std::cout<<dt[i][j]<<std::endl;
   }*/

   
   /*
   std::cout<<"level1 count: "<< parser.levelsPartCounts[0]<< std::endl;
   for(int i =0;i<parser.levelsPartCounts[0];i++){
      std::cout<< parser.getParsedPartLayered(i,0)<< std::endl;

   }
   std::cout<<"level2 count: "<< parser.levelsPartCounts[1]<< std::endl;
   for(int i =0;i<parser.levelsPartCounts[1];i++){
      std::cout<< parser.getParsedPartLayered(i,1)<< std::endl;

   }
   std::cout<<"level3 count: "<< parser.levelsPartCounts[2]<< std::endl;
   for(int i =0;i<parser.levelsPartCounts[2];i++){
      std::cout<< parser.getParsedPartLayered(i,2)<< std::endl;

   }
   std::cout<< std::endl;
   */

   

   //for(int i = 0; i< parser.getParsedPartCount()+1; i++){std::cout<< parser.getParsedPart(i)<< std::endl;}
   //std::cout<< parser.getParsedPartLayered(3,1)<< std::endl;

   //std::cout<< parser.getParsedPart(100)<< std::endl;

   //parser.printVectorPair(parser.delimiterIndex2);






}

