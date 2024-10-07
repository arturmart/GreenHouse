#include <Arduino.h>
#include <Arduino_CRC32.h>

#define delimiter1L "/"
#define delimiter2L ";"
#define delimiter3L ","
#define TERMINATOR '\n'

#define dataSendRowsSize 8

const int RELAY_COUNT1 = 4;
int relayPin1[RELAY_COUNT1] = {2,3,4,5};    

const unsigned int BUFFER_SIZE = 64;  // Размер буфера для хранения входящих данных
char inputBuffer[BUFFER_SIZE];        // Буфер для входящих данных

Arduino_CRC32 crc32;

class RelayController{
  public:
    RelayController(int* _relayPin,const int _RELAY_COUNT):RELAY_COUNT( _RELAY_COUNT ){
      setRelayPin(_relayPin);
      initRelay();
      //testRelay();

    }
    RelayController(){

    }


    void setRelayPin(int* _relayPin){
      relayPin = _relayPin;

    }
     
    void relayWrite(int id, bool dig){
      digitalWrite(relayPin[id], !dig);
    }
    
    void testRelay(){
      for(int i =0;i<RELAY_COUNT;i++){
        relayWrite(i,HIGH);
        delay(300);
        relayWrite(i,LOW);
        delay(100);
      }
    }

    void Log(){
      Serial.print("size:");
      Serial.println(RELAY_COUNT);

      for(int i =0;i<RELAY_COUNT;i++){
        Serial.print(relayPin[i]);

        Serial.print(" ");
      }
      Serial.println();
    }

  
    void initRelay(){
        for(int i =0;i<RELAY_COUNT;i++){
            pinMode(relayPin[i], OUTPUT);
            relayWrite(i,LOW);
          }
      }
    //private:
    int* relayPin;
    int RELAY_COUNT;
  };


class DataParseSplit{
  public:
    void printIntArr2d(byte arr[dataSendRowsSize][3],int N = dataSendRowsSize, int M = 3){
      
      for(int i = 0; i<N; i++){
        
          for(int j = 0; j<M; j++){
          Serial.print(arr[i][j]);
          Serial.print('\t');
      }
      Serial.println();
        
        

      }
    }
    void printStringArr(String* arr,int size = dataSendRowsSize){
      
      for(int i = 0; i<size; i++){
        
          Serial.print("str[");
          Serial.print(i);
          Serial.print("] = ");
          Serial.println(arr[i]);
        
        

      }
    }
    int pars(int* delimiter, String& str, String* arr, int delimiterSize=1){

    

      //Serial.print("delimiter[0]- ");
      //Serial.println(delimiter[0]);
      //Serial.print("str- ");
      //Serial.println(str);
      //Serial.print("Substring result: ");
      //Serial.println(str.substring(0, 8));

      if(delimiter[0] == 0 ) arr[0] = str;
      else if(delimiter[0] > 0 ){
        arr[0] = str.substring(0, delimiter[1]);
     

        for(int i =1;i<delimiter[0];i++){
          
          arr[i] = str.substring(delimiter[i]+delimiterSize, delimiter[i+1]);
      
        }
        



        arr[delimiter[0]] = str.substring(delimiter[delimiter[0]]+delimiterSize, str.length());

      }

      return delimiter[0]+1;
      

    }
    int pars(int* delimiter, String &str, byte* arr, int delimiterSize=1){

      
      if(delimiter[0] == 0 ) arr[0] = str.toInt();
      
      else if(delimiter[0] > 0 ){
        //arr[0] = str.substring(0, delimiter[1])[0];         // to Send Int from first Char
        arr[0] = str.substring(0, delimiter[1]).toInt();  // to Send Int

        for(int i =1;i<delimiter[0];i++){
          arr[i] = str.substring(delimiter[i]+delimiterSize, delimiter[i+1]).toInt();
        }

        arr[delimiter[0]] = str.substring(delimiter[delimiter[0]]+delimiterSize, str.length()).toInt();
      }

      return delimiter[0]+1;
      

    }
    int find(String &str, String trg){// returned [size][d1][d3]...[d8]

        static int a[dataSendRowsSize+1]; 
        int aCrr = 0;

        for(int crr = 0; crr< str.length()-trg.length()+1;crr++){

            bool flag = true;
            for(int t =0; t< trg.length();t++){
                if(str[crr+t] != trg[t]){
                  flag = false;
                  break;
                }
            }
            if(!flag) continue;

            a[aCrr+1] = crr;
            aCrr++;
            
        }
        a[0] = aCrr;
        return a;
    }
    String split(byte* arr,String delimiter,int N = dataSendRowsSize){

          String data ="";

          /*

          data += (char)(arr[0]);   //first is char
          data += delimiter;
          for(int i = 1; i<N-1; i++){
            data += (arr[i]);
            data += delimiter;
          }
          data += (arr[N-1]);
          */

            
          for(int i = 0; i<N-1; i++){   //first not char
            data += (arr[i]);
            data += delimiter;
          }
          data += (arr[N-1]);

          return data;

    }
    String split(String* arr,String delimiter,int N = dataSendRowsSize){

          String data ="";

          /*

          data += (char)(arr[0]);   //first is char
          data += delimiter;
          for(int i = 1; i<N-1; i++){
            data += (arr[i]);
            data += delimiter;
          }
          data += (arr[N-1]);
          */

            
          for(int i = 0; i<N-1; i++){   //first not char
            data += (arr[i]);
            data += delimiter;
          }
          data += (arr[N-1]);

          return data;

    }
    String split2Strings(String str1,String str2,String delimiter){

          return  str1+delimiter+str2;


    }
};

bool crcCheck(String data, String crc){
 
    //Serial.println(data);
    //Serial.println(crc);
    //Serial.println(String(crc32.calc(data.c_str(), data.length()),HEX));
    //Serial.println(String(crc32.calc(data.c_str(), data.length()),HEX) == crc);
    return String(crc32.calc(data.c_str(), data.length()),HEX) == crc;

}
String crcCalk(String data){
    return String(crc32.calc(data.c_str(), data.length()),HEX);
}
class DataReadSned{
  public:
  DataReadSned(String &str){
    setStr(str);
    
  }
  DataReadSned(){
    
  }
  void setStr(String &str){
    _str = str;
  }

  int parse(String &str, byte (&pars3L)[dataSendRowsSize][3]){
    setStr(str);
    return parse(pars3L);
  }
  int parse(byte (&pars3L)[dataSendRowsSize][3]){
    //[parses is 2][crc check][One data row][more data row][wrrong Number][8 bitRow Count]
    int feedbeck = 0;

    String pars1L[2];
    feedbeck = parse1L(crcCheck,pars1L)<<11;//[parses is 2][crc check][][][][8nit]

    if(feedbeck & (0b1 << 11)){
      Serial.println("CRC is Correct!");
    }
    else{
      Serial.println("CRC is Wrong!");
    }

    String pars2L[dataSendRowsSize];
    
    feedbeck+=(parse2L(pars2L,pars1L)<<8);//[][][One data row][more data row][wrrong Number][8bit]


    //byte pars3L[dataSendRowsSize][3];
    int rowCount = parse3L(pars3L,pars2L);
    feedbeck+= rowCount;//[][][][][][8 bitRow Count]
    dm.printIntArr2d(pars3L,rowCount);
     

    return(feedbeck);


    
  }
  int split(byte (&pars3L)[dataSendRowsSize][3],byte size3L){ 
    splitDataAndCRC( pars3L,crcCalk, size3L);
  }
  private:
  int splitDataAndCRC(byte pars3L[dataSendRowsSize][3],String _crcCalc (String),byte size3L){
    Serial.println();
    Serial.println();

    //spliting

    String sendData1L;
    String sendData2L;
    String sendData3L[dataSendRowsSize];

    for(int i =0;i<size3L;i++){
      sendData3L[i] = dm.split(pars3L[i],delimiter3L,3);
    }

    sendData2L = dm.split(sendData3L,delimiter2L,size3L);
    sendData1L = dm.split2Strings(sendData2L,_crcCalc(sendData2L),delimiter1L);

    Serial.println(sendData1L);
  }
  byte parse1L(bool _crcCheck (String&,String&), String* pars1L){
    byte feedbeck = 0;
    
    //1L array
    //String pars1L[2];   
    int* delPoint1L = dm.find(_str,delimiter1L);
    byte size1L = dm.pars(delPoint1L, _str, pars1L);
    feedbeck = ((size1L==2) << 1)+ _crcCheck(pars1L[0],pars1L[1]);  //0b000000[parses is 2][crc check]

    Serial.println("Parse 1 level");
    dm.printStringArr(pars1L,2);
    Serial.println();

    return feedbeck;

    

  }
  byte parse2L( String* pars2L, String* pars1L){
    byte feedbeck = 0;

    //2L array
    //String pars2L[dataSendRowsSize];
    int* delPoint2L = dm.find(pars1L[0],delimiter2L);
    byte size2L =dm.pars(delPoint2L, pars1L[0], pars2L);
    feedbeck = ((size2L==0) << 2)+ ((size2L>8) << 1)+(size2L <= 0);  //0b00000[One data row][more data row][wrrong Number]

    for(int i = 0; i <size2L;i++){
      Serial.print(delPoint2L[i]);
      Serial.print('\t');
    }
    Serial.println();

    Serial.print("Parse 2 level: size(");
    Serial.print(size2L);
    Serial.println(")");
    dm.printStringArr(pars2L,size2L);
    Serial.println();

    return feedbeck;



    

  }
  byte parse3L(byte (&pars3L)[dataSendRowsSize][3], String* pars2L){

    //3L array
    
    //byte pars3L[dataSendRowsSize][3];
    int* delPoint3L;
    byte size3L = 0;
    for(int i =0;i<dataSendRowsSize;i++){
        if(pars2L[i] == NULL) break;
        size3L++;

        delPoint3L = dm.find(pars2L[i],delimiter3L);
        dm.pars(delPoint3L, pars2L[i], pars3L[i]);

    }

    Serial.print("size3L - ");
     Serial.println(size3L);
    return size3L;  //0bXXXXXXXX real Data Row Count

 

    //Serial.println(size3L);
    //Serial.println();
    //dm.printIntArr2d(pars3L,size3L);




  }

 





  String _str;
  DataParseSplit dm;
};



DataReadSned DS;
RelayController RC;


void erorrhandler(int& feedbeck){
  //[parses is 2][crc check][One data row][more data row][wrrong Number][8 bitRow Count]

  if(feedbeck == 0){
    Serial.println("[E1] Wrong Data!");
  }
  if((feedbeck>>12) == 0){
    Serial.println("[E2] Wrong type of Level1");
  }
  if(((feedbeck>>11) & 0b1) != 1){
    Serial.println("[E3] Wrong CRC!");
  }
  if(((feedbeck>>10) & 0b1) == 1){
    Serial.println("[W] One data Row");
  }
  else if(((feedbeck>>9) & 0b1) == 1){
    Serial.println("[E4] data Row OwerFlow1");
  }
  else if(((feedbeck>>8) & 0b1) == 1){
    Serial.println("[E5] Wrong data Row!");
  }
  if((feedbeck & 0b11111111) > 8){
    Serial.println("[E6] data Row OwerFlow!");
  }
  else if((feedbeck & 0b11111111) <0){
    Serial.println("[E7] Wrong data Row!");
  }
          
         

}
void relayCheck(const byte (&data)[dataSendRowsSize][3],int& rowSize){
  for(int i = 0; i<rowSize; i++){

      switch(data[i][0]) {
        case 83:
          //digitalWrite(RC.relayPin[pars3L[i][1]], pars3L[i][2]);
          RC.relayWrite(data[i][1], data[i][2]);
          
          Serial.print("R ");
          Serial.print(data[i][1]);
          Serial.print(" ");
          Serial.println(data[i][2]);
          
          break;
      }
    }

}

int check(byte (&pars3L)[dataSendRowsSize][3],String& str){
    
    //byte pars3L[dataSendRowsSize][3];
    
    
    int feedbeck = DS.parse(str,pars3L);
    Serial.print("feedbeck - ");
    Serial.println(feedbeck,BIN);
    int rowSize = feedbeck & 0b11111111;

    relayCheck(pars3L, rowSize);

    
    return feedbeck;
}


byte data[dataSendRowsSize][3];
String str;

void setup() {

    // parsing
    Serial.begin(9600);
    Serial.println();

    RC =  RelayController(relayPin1,RELAY_COUNT1);
    str="83,0,0;83,1,0;83,2,0;83,3,0/745ec3dc";

    int feedbeck = check(data,str);
    erorrhandler(feedbeck);
    
}


void loop() {

   if (Serial.available() > 0) {
    int presentSize = Serial.readBytesUntil(TERMINATOR, inputBuffer, 64);
    inputBuffer[presentSize--] = NULL;
      Serial.println(inputBuffer);

      str = inputBuffer;
      //int feedbeck = check(data,str);
      //erorrhandler(feedbeck);
      Serial.println();
  }
}
