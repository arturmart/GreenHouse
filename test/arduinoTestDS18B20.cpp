#include <OneWire.h>
#include <DallasTemperature.h>

// Укажите пин, к которому подключен датчик
#define ONE_WIRE_BUS 2

// Создаем объект OneWire
OneWire oneWire(ONE_WIRE_BUS);

// Передаем объект OneWire в библиотеку DallasTemperature
DallasTemperature sensors(&oneWire);

void setup() {
  // Инициализируем последовательный порт
  Serial.begin(9600);
  
  // Инициализируем библиотеку DallasTemperature
  sensors.begin();
}

void loop() {
  // Запрашиваем температуру от всех датчиков
  sensors.requestTemperatures();
  
  // Получаем температуру первого датчика
  float temperature = sensors.getTempCByIndex(0);
  
  // Выводим температуру в последовательный порт
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  // Ждем 1 секунду перед следующим чтением
  delay(1000);
}
