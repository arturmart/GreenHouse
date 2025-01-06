#include <Arduino.h>
#include "DataParser.h" // Include the DataParser class
#include "StringLimited.h" // Ensure this header file exists
#include "RelayController.h"
#include <Arduino_CRC32.h>
Arduino_CRC32 crc32;

#define BUFFER_SIZE 64
char inputBuffer[BUFFER_SIZE];
#define TERMINATOR '\n'

const int ERROR_NONE = 0;               
const int ERROR_SYNTAX = 1 << 0;       
const int ERROR_1L_NO_DATA = 1 << 1;   
const int ERROR_1L_TOO_MANY_DATA = 1 << 2;   
const int ERROR_INVALID_CRC = 1 << 3; 
const int ERROR_NULL_CRC = 1 << 4;   
const int ERROR_2L_NO_DATA_PACKETS = 1 << 5; // error chi
const int ERROR_2L_TOO_MANY_PACKETS = 1 << 6; 
const int ERROR_3L_WRONG_DATA_PACKETS = 1 << 7; 
const int PACKETS_COUNT = 1111 << 8;
const int GET_KEYWORD = 1 << 12;



const int RELAY_COUNT = 16;
int relayPin[RELAY_COUNT] = {30,32,34,36,38,40,42,44,22,24,26,28,4,5,6,7};  

RelayController relayController(relayPin, RELAY_COUNT); 

StringLimited<BUFFER_SIZE> str;   //83,0,1;83,1,1;83,2,1;83,3,1/61204cfc //83,0,0;83,1,0;83,2,0;83,3,0/745ec3dc
int feedback;
byte data[8][3];
DataParser parser;


 


StringLimited<BUFFER_SIZE> crcCalk(StringLimited<BUFFER_SIZE>& data,int base = HEX){
   
  
    uint32_t crc1 (crc32.calc(data.c_str(), data.size())); 
    char buf[1 + 8 * sizeof(unsigned long)];
	  ultoa(crc1, buf, base);
    StringLimited<BUFFER_SIZE> crc2(buf);
  
    return (crc2);
}

void serialWriteKeyWord(int feedback, enum DataParser::KEYWORD key){

      //Serial.print("feedbeck ");
      StringLimited<BUFFER_SIZE> write = "";

      switch (key){
        case DataParser::KEYWORD::KEYWORD_INITED:
            write = "dcmIsInited";
            //write += "/";
            //write.AddIntEnd(feedback,BIN);
       
            //Serial.println(write);
            Serial1.println(write+"/"+crcCalk(write));
          break;
        case DataParser::KEYWORD::KEYWORD_SHOWALL:
            write = "";
            write.AddIntEnd(relayController.getStates(),BIN);
            
            //write.AddIntEnd(feedback,BIN);
            //Serial.println(write);
            Serial1.println(write+"/"+crcCalk(write));
          break;
      }
      
      
   
    
}

void serialWriteFeedBack(int feedback){

      
      StringLimited<BUFFER_SIZE> write = "";
      
      write.AddIntEnd(feedback,BIN);
      


      Serial1.println(write+"/"+crcCalk(write));
}

void doAction(int size){

  //Serial.println("Do Action");

  for (int i = 0; i < size; i++) {
          //Serial.print("Row "); Serial.print(i); Serial.print(": ");for (int j = 0; j < 3; j++) {Serial.print(data[i][j]);Serial.print(" ");}Serial.println();

          if(data[i][0] == 83) {
            relayController.relayWrite(data[i][1],data[i][2]);
            //Serial.print("Servo "); Serial.print(data[i][1]); Serial.print(" is "); Serial.println(data[i][2]);
            }
      }

      
}

void doAll(){
 relayController.relayWriteAll();
}


//  83,0,0/6e0d4821
//  83,0,1/190a78b7

//  83,1,0/6fcf2216
//  83,1,1/18c81280

//  83,2,0/6d899c4f 
//  83,2,1/1a8eacd9

//  83,3,0/6c4bf678
//  83,3,1/1b4cc6ee

//  83,4,0/6904e0fd
//  83,4,1/1e03d06b

//  83,5,0/68c68aca
//  83,5,1/1fc1ba5c

//  83,12,0/f0af7675
//  83,12,1/87a846e3

void setup() {
    Serial1.begin(57600);
    pinMode(13, OUTPUT);
    digitalWrite(13,HIGH);
    parser.setCRC(crcCalk);
  
}

void loop() {
    if (Serial1.available() > 0) {
      int presentSize = Serial1.readBytesUntil(TERMINATOR, inputBuffer, 64);
      inputBuffer[presentSize--] = NULL;
      str = inputBuffer;
      //Serial.println(str);
      

      parser.setStr(str);
      //int newFeedback = parser.parse( data);

      //if(parser.parse( data))
      
      feedback = parser.parse( data, doAction, serialWriteKeyWord);
      doAll();
      //Serial.print("feedbeck "); Serial.println(feedback,BIN);
      if(!(feedback & GET_KEYWORD))serialWriteFeedBack(feedback);
      
      //showall/8d984889
      //83,0,1;83,1,1;83,2,1;83,3,1/61204cfc
      //83,0,0;83,1,0;83,2,0;83,3,0/745ec3dc
      //83,0,1;83,1,0;83,2,0;83,3,0/6376a71c
      //83,3,1/1b4cc6ee

    }


      

}