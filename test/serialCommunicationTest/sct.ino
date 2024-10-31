
#define BUFFER_SIZE 64
#define TERMINATOR '\n'


class SerialComm {
private:
    char inputBuffer[BUFFER_SIZE];
    

public:
    SerialComm() {}  // Конструктор инициализирует индекс

    void setup() {
        Serial1.begin(9600);  // Инициализация последовательного порта
    }

    /*
    //Old

    void receiveCommand() {
        while (Serial.available()) {  // Проверка, есть ли доступные данные


            char incomingByte = Serial.read();  // Чтение байта
            if (incomingByte != '\n' && incomingByte != '\0') {
                if (index < sizeof(buffer) - 1) {  // Проверка на переполнение буфера
                    buffer[index++] = incomingByte;  // Запись байта в буфер
                }
            } else {
                buffer[index] = '\0';  // Завершение строки
                processCommand(buffer);  // Обработка команды
                index = 0;  // Сброс индекса для следующей команды
            }
        }
    }
    */
    void receiveCommand() {
        while (Serial1.available() > 0) {
              int presentSize = Serial1.readBytesUntil(TERMINATOR, inputBuffer, BUFFER_SIZE);
              inputBuffer[presentSize--] = NULL;

              processCommand(inputBuffer);
             
              //Serial.println(str);

            
        }
    }


    void processCommand(const char* command) {
        //Serial.println(command); 
        if (strcmp(command, "blink") == 0) {  // Если команда "blink"
            
                digitalWrite(LED_BUILTIN, HIGH);
                delay(250);
                digitalWrite(LED_BUILTIN, LOW);
                delay(250);
            
            Serial1.print("OK ");  // Отправка фидбека обратно
        } 
        else {
            Serial1.print("WG ");  // Фидбек для неизвестной команды
        }
        Serial1.println(command);
    }
};

SerialComm serialComm;  // Создание объекта класса SerialComm

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);  // Настройка пина светодиода как выход
    serialComm.setup();  // Инициализация последовательного порта
}

void loop() {
    serialComm.receiveCommand();  // Получение и обработка команд
}
