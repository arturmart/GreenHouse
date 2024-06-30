#include <Arduino_CRC32.h>


const unsigned int BUFFER_SIZE = 64;  // Размер буфера для хранения входящих данных
char inputBuffer[BUFFER_SIZE];        // Буфер для входящих данных

Arduino_CRC32 crc32;

#define DELIMITER1L '/'
#define DELIMITER2L ','
#define TERMINATOR '\n'


void setup() {
  Serial.begin(9600);  // Инициализация Serial со скоростью 9600 бод
}


void loop() {
  if (Serial.available() > 0) {
    int presentSize = Serial.readBytesUntil(TERMINATOR, inputBuffer, 64);
    inputBuffer[presentSize--] = NULL;
    

    parsing(inputBuffer, presentSize);
  }
}



void parsing(char* buf, int size) {

  Serial.print("InputBuf[");
  Serial.print(size);
  Serial.print("]=");
  Serial.println(buf);


  char* commaPosDel = strchr(buf, DELIMITER1L);

  int delPos = commaPosDel-buf;


  *commaPosDel = '\0'; 

  

  Serial.print("data[");
  Serial.print(commaPosDel-buf);
  Serial.print("] = ");
  Serial.println(buf);

  Serial.print("CRC[");
  Serial.print(size-delPos);
  Serial.print("] = ");
  Serial.println(commaPosDel+1);

  String str = commaPosDel+1;
  
  unsigned long receivedCRC = str.toInt();

  Serial.print("receivedCRC: ");
  Serial.println(receivedCRC);

  uint32_t const calculatedCRC = crc32.calc((uint8_t const *)buf,commaPosDel-buf);


  Serial.print("calculatedCRC: ");
  Serial.println(calculatedCRC);

  Serial.print("equal?: ");
  Serial.println(receivedCRC == calculatedCRC);
Serial.println();


  
/*
  //char qaq[4] = data1L[1];
  uint32_t receivedCRC = strtoul("iÃÆ\0", NULL, 16);  // Преобразование CRC из строки в число

  CRC32 crc;
  crc.update(data1L[0], strlen(data1L[0]));
  unsigned long calculatedCRC = crc.finalize();

 
  Serial.println( receivedCRC);
  Serial.println(calculatedCRC);*/
  //return receivedCRC == calculatedCRC;  // Сравнение полученного и вычисленного CRC


  //GParser data2L(data1L[0], DELIMITER2L);
  //int parseCount2L = data2L.split();

  //for (byte i = 0; i < parseCount2L; i++) Serial.println(data2L[i]);
}


