#include <Arduino_CRC32.h>
#include <GParser.h>

const unsigned int BUFFER_SIZE = 64;  // Размер буфера для хранения входящих данных
char inputBuffer[BUFFER_SIZE];        // Буфер для входящих данных

Arduino_CRC32 crc32;

#define DELIMITER1L '/'
#define DELIMITER2L ','
#define TERMINATOR '\n'

const int RELAY_COUNT2 = 8;
int relayPin2[RELAY_COUNT2] = {2,3,4,5,6,7,8,9};                 // Светодиод подключенный к вход/выходу 13


class RelayController{
  public:
    RelayController(int* _relayPin,const int _RELAY_COUNT):RELAY_COUNT( _RELAY_COUNT ){
      setRelayPin(_relayPin);
      initRelay();
      //testRelay();

    }
    RelayController(){

    }

    RelayController(const RelayController& source): RELAY_COUNT (source.RELAY_COUNT ){
      
      relayPin = new int[RELAY_COUNT];
      for(int i =0;i<RELAY_COUNT;i++){
        relayPin[i]=source.relayPin[i];
      }

    }
    ~RelayController(){
      delete(relayPin);
    }
    RelayController& operator=(const RelayController& source){
      RELAY_COUNT = source.RELAY_COUNT;

      relayPin = new int[RELAY_COUNT];
      for(int i =0;i<RELAY_COUNT;i++){
        relayPin[i]=source.relayPin[i];
      }

      return *this;
    }

    void setRelayPin(int* _relayPin){
      relayPin = _relayPin;

    }
     
    void relayWrite(int id, bool dig){
      digitalWrite(relayPin[id], !dig);
    }
    void initRelay(){
      for(int i =0;i<RELAY_COUNT;i++){
          pinMode(relayPin[i], OUTPUT);
          relayWrite(i,LOW);
        }
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

  private:
    int* relayPin;
    int RELAY_COUNT;
  };

bool getBit(unsigned char byte, int position) {
    return (byte >> position) & 0x1;
  }


RelayController RC;


void setup() {
  RC =  RelayController(relayPin2,RELAY_COUNT2);
  Serial.begin(9600);  // Инициализация Serial со скоростью 9600 бод
}


void loop() {
  if (Serial.available() > 0) {
    int presentSize = Serial.readBytesUntil(TERMINATOR, inputBuffer, 64);
    inputBuffer[presentSize--] = NULL;
    

    if(checkCRC(inputBuffer, presentSize)){
        parsing(inputBuffer, presentSize);
    }
  }
}


bool checkCRC(char* buf, int size){
  //Serial.print("InputBuf[");Serial.print(size);Serial.print("]=");Serial.println(buf);

  char* commaPosDel = strchr(buf, DELIMITER1L);
  int delPos = commaPosDel-buf;

  *commaPosDel = '\0'; 

  

  //Serial.print("data[");  Serial.print(commaPosDel-buf);  Serial.print("] = "); Serial.println(buf);
  //Serial.print("CRC[");   Serial.print(size-delPos);      Serial.print("] = "); Serial.println(commaPosDel+1);

  String str = commaPosDel+1;
  
  unsigned long receivedCRC = str.toInt();
  //Serial.print("receivedCRC: ");    Serial.println(receivedCRC);

  uint32_t const calculatedCRC = crc32.calc((uint8_t const *)buf,commaPosDel-buf);
  //Serial.print("calculatedCRC: ");  Serial.println(calculatedCRC);

  //Serial.print("equal?: ");   Serial.println(receivedCRC == calculatedCRC); Serial.println();

  return receivedCRC == calculatedCRC;
}

void parsing(char* buf, int size) {

    Serial.print("InputBuf[");Serial.print(size);Serial.print("]=");Serial.println(buf);
    
    GParser data(buf, DELIMITER2L);
    int dataSize = data.split();
  
    //for(int i =0 ;i<dataSize;i++)Serial.println(data[i]);
  for(int i =0;i<dataSize;i++){
    if(data.equals(i,"R")) {
        RC.relayWrite(data.getInt(i+1),data.getInt(i+2));
        i+=2;
        }
    
      }
    

    

  }


