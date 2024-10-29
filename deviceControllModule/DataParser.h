#ifndef DATAPARSER_H
#define DATAPARSER_H

#include "StringLimited.h" // Ensure this header file exists
#include "Parser.h" // Include the Parser header

#define BUFFER_SIZE 64
#define DELIMITER1 '/' // First delimiter
#define DELIMITER2 ';' // Second delimiter
#define DELIMITER3 ',' // Third delimiter
#define MAX_DATA_ROWS 8 // Maximum number of data rows

class DataParser {
public:

    const char* KEYWORD_SHOWALL = "showall";


     const int ERROR_NONE = 0;               
     const int ERROR_SYNTAX = 1 << 0;       
     const int ERROR_1L_NO_DATA = 1 << 1;   
     const int ERROR_1L_TOO_MANY_DATA = 1 << 2;   
     const int ERROR_INVALID_CRC = 1 << 3; 
     const int ERROR_NULL_CRC = 1 << 4;   
     const int ERROR_2L_NO_DATA_PACKETS = 1 << 5; 
     const int ERROR_2L_TOO_MANY_PACKETS = 1 << 6; 
     const int ERROR_3L_WRONG_DATA_PACKETS = 1 << 7; 
     const int PACKETS_COUNT = 1111 << 8;
     const int GET_KEYWORD = 1 << 12;

    DataParser()= default;

    DataParser(const StringLimited<BUFFER_SIZE>& inputStr) 
        : parser(inputStr) { // Initialize parser with inputStr
    }

    void setStr(StringLimited<BUFFER_SIZE>* inputStr) {
        parser.setStr(*inputStr);
       
    }

    void setStr(StringLimited<BUFFER_SIZE>& inputStr) {
        parser.setStr(inputStr);
   
    }

    


    void setCRC(StringLimited<BUFFER_SIZE> (*_crcCheck) (StringLimited<BUFFER_SIZE>&,int) ) {
        crcClac = _crcCheck;
    }
    
 

    int parse( byte data[8][3],void (*doAction) (int)) { // Return type: pointer to an array of 3 bytes
        int feedbeck = ERROR_NONE;

        if(!parseLevel1(feedbeck)){
          return feedbeck;
        }
        else if(!crcCheck(feedbeck)){
          return feedbeck;
        }

        //Serial.println(parser.getPartCharPtr(delimiters1L, 0));
        //Serial.println(parser.getPartCharPtr(delimiters1L, 1));

        if(strcmp(parser.getPartCharPtr(delimiters1L, 0),KEYWORD_SHOWALL)==0){      //get KEYWORD_SHOWALL
          Serial.println("KEYWORD_SHOWALL");
          feedbeck |= GET_KEYWORD;
          return feedbeck;
        }
        
        else{
          if(!parseLevel2(feedbeck)){
            return feedbeck;
          }
          else if(!parseLevel3(data,feedbeck)){
            return feedbeck;
          }
          //Do Action
          //Serial.print("FEEDBECK ");Serial.println(feedbeck,BIN);

          doAction((feedbeck & PACKETS_COUNT)>>8);


          return feedbeck;
        }
        
        
        
        
    }

private:
    Parser<BUFFER_SIZE> parser; // Parser object
   
    //StringLimited<32> crc; // Store CRC

    StringLimited<BUFFER_SIZE> (*crcClac)  (StringLimited<BUFFER_SIZE>&,int) = nullptr;
   
   
    int delimiters1L[2]; // Array for level 1 delimiters
    
    int delimiters2L[8]; // Array for level 2 delimiters
    int delimiters3L[3]; // Array for level 3 delimiters
    StringLimited<8> part;
    byte data[MAX_DATA_ROWS][3]; // Array to store parsed byte data

    bool parseLevel1(int& feedback) {

        

        parser.parseCut(delimiters1L, DELIMITER1, 2); // Split string by first delimiter

        //Serial.println("Parts Count: ");  Serial.println(delimiters1L[0]); // Print number of parts

        // Print parts
        //for (int i = 0; i <= delimiters1L[0]; i++) Serial.println(parser.getPartCharPtr(delimiters1L, i));

        if(delimiters1L[0] == 0 ){
           feedback |= ERROR_1L_NO_DATA;
           return(false);
        }
        else if(delimiters1L[0] > 1 ){ 
          feedback |= ERROR_1L_TOO_MANY_DATA;
          return(false);
        }
        else{
          return(true);

        }

        
        

    }

    bool crcCheck(int& feedback){
  

      StringLimited<BUFFER_SIZE> data = parser.getPartCharPtr(delimiters1L, 0);
      data = crcClac(data,HEX);

      StringLimited<BUFFER_SIZE> crc = parser.getPartCharPtr(delimiters1L, 1);

      data.toLower();
      crc.toLower();
      //Serial.print("CRC "); Serial.print(data);Serial.print(" == "); Serial.print(crc); Serial.print(" = "); Serial.println(data == crc);
      if(crc == "" || crc=="\0" || crc==0 ) {
              //Serial.print("Null CRC");
              feedback |= ERROR_NULL_CRC | ERROR_INVALID_CRC;
              return(false);
      }
      else if(data != crc){
         feedback |= ERROR_INVALID_CRC;
         return(false);
      }
      else{
         return(true);
      }


      

    }

    bool parseLevel2(int& feedback) {
       

        parser.parseCut(delimiters2L, DELIMITER2); // Split string by second delimiter

        //Print packages
        //Serial.println("Data Packages Count: ");  Serial.println(delimiters2L[0]); // Print number of packages
        //for (int i = 0; i <= delimiters2L[0]; i++) {Serial.println(parser.getPartCharPtr(delimiters2L, i));}

        if(delimiters2L[0] == 0 ){
           feedback |= ERROR_2L_NO_DATA_PACKETS;
           return(true);
           //This is Not Error
        }
        else if(delimiters2L[0] > 7 ){
           feedback |= ERROR_2L_TOO_MANY_PACKETS;
           return(false);
        }
        else{
          return true;
        }

        


    }

    bool parseLevel3(byte data[8][3], int& feedback) { // Метод для парсинга третьего уровня с массивом байтов
    
        for (int i = 0; i <= delimiters2L[0]; i++) {
            parser.parseCut(delimiters3L, DELIMITER3, 2); // Split by third delimiter
            //Serial.println(parser.getStr()[23]);
            //Serial.print("3L[");Serial.print(i);Serial.print("] - ");Serial.print(delimiters3L[0]);Serial.print(" - "); // Number of parts in current package

            if(delimiters3L[0] != 2){
                  feedback |= ERROR_3L_WRONG_DATA_PACKETS;
                  return(false);
                }
            
            //Serial.print("DEL3 - (");
            //for (int j = 0; j <= delimiters3L[0]; j++) { Serial.print(delimiters3L[j]); Serial.print(" ");}
            //Serial.print(") ");
            for (int j = 0; j <= delimiters3L[0]; j++) {
                
                part = parser.getSubstringFromPart<8>(delimiters3L, j, 8); // Get part as StringLimited
                

                //Serial.print("part - ");  Serial.print(part); 

                int value = part.toInt(); // Convert to int
                data[i][j] = static_cast<byte>(value); // Store value as byte
                //Serial.print(" ");Serial.print(data[i][j]);     // Print value
            }


            //Serial.println();
            
             
        }
        
        feedback |= (((delimiters2L[0]+1)<<8) & PACKETS_COUNT);
        return(true);
    }
};

#endif // DATAPARSER_H
