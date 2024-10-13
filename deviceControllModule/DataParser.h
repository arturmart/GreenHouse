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
    DataParser(const StringLimited<BUFFER_SIZE>& inputStr) 
        : parser(inputStr) { // Initialize parser with inputStr
    }

    void setStr(StringLimited<BUFFER_SIZE>* inputStr) {
        parser.setStr(*inputStr);
    }



    void setCRC(const StringLimited<32>& externalCRC) {
        crc = externalCRC;
    }

    void parse( byte data[8][3]) { // Return type: pointer to an array of 3 bytes
        parseLevel1(); // First level parsing
        parseLevel2(); // Second level parsing
        parseLevel3(data); // Third level parsing and return byte matrix
    }

private:
    Parser<BUFFER_SIZE> parser; // Parser object
    StringLimited<32> crc; // Store CRC
    int delimiters1L[2]; // Array for level 1 delimiters
    int delimiters2L[8]; // Array for level 2 delimiters
    int delimiters3L[3]; // Array for level 3 delimiters
    byte data[MAX_DATA_ROWS][3]; // Array to store parsed byte data

    void parseLevel1() {
        parser.parseCut(delimiters1L, DELIMITER1, 2); // Split string by first delimiter
        Serial.println("Parts Count: ");
        Serial.println(delimiters1L[0]); // Print number of parts

        // Print parts
        for (int i = 0; i <= delimiters1L[0]; i++) {
            Serial.println(parser.getPartCharPtr(delimiters1L, i));
        }

        // Get CRC
        crc = parser.getPartCharPtr(delimiters1L, 1);
        Serial.print("CRC: ");
        Serial.println(crc);
        
        // Compare CRC
        crc.toLower();
        Serial.print(crc);
        Serial.print(" == ");
        Serial.print("745ec3dc");
        Serial.print(" -> ");
        Serial.println(crc == "745ec3dc");
    }

    void parseLevel2() {
        parser.parseCut(delimiters2L, DELIMITER2); // Split string by second delimiter
        Serial.println("Data Packages Count: ");
        Serial.println(delimiters2L[0]); // Print number of packages

        // Print packages
        for (int i = 0; i <= delimiters2L[0]; i++) {
            Serial.println(parser.getPartCharPtr(delimiters2L, i));
        }
    }

    void parseLevel3(byte data[8][3]) { // Метод для парсинга третьего уровня с массивом байтов
        for (int i = 0; i <= delimiters2L[0]; i++) {
            parser.parseCut(delimiters3L, DELIMITER3, 2); // Split by third delimiter
            Serial.print("3L[");
            Serial.print(i);
            Serial.print("] - ");
            Serial.println(delimiters3L[0]); // Number of parts in current package

            for (int j = 0; j <= delimiters3L[0]; j++) {
                StringLimited<8> part = parser.getSubstringFromPart<8>(delimiters3L, j, 8); // Get part as StringLimited
                int value = part.toInt(); // Convert to int
                data[i][j] = static_cast<byte>(value); // Store value as byte
                Serial.print(data[i][j]); // Print value
                Serial.print(" ");
            }
            Serial.println(); // New line
        }
    }
};

#endif // DATAPARSER_H
