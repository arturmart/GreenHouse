# Название компилятора
CXX = g++

# Флаги компилятора
CXXFLAGS = -std=c++17 -Wall -O0 -I/path/to/nlohmann/json -I/usr/include/cairo

# Линковочные флаги
#LDFLAGS = -lwiringPi -lz -lcurl -lcairo -lTgBot -lboost_system -lssl -lcrypto -lpthread
LDFLAGS =  -lz -lcurl -lcairo -lTgBot -lboost_system -lssl -lcrypto -lpthread

# Исходные файлы


#SRC = main.cpp tokens.cpp ghSim.cpp Executor.cpp I2CLCD.cpp RelayControlModule.cpp DataParsing.cpp \
#      TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp DataCombine.cpp \
#      crcCalc.cpp DataGeter.cpp DS18B20.cpp WeatherAPI.cpp Logger.cpp \
#      jsonManager.cpp ChartDrawing.cpp TimeManager.cpp TelegramBot.cpp \
		
SRC = main.cpp tokens.cpp ghSim.cpp Executor.cpp \
	  DataGeter.cpp DS18B20.cpp WeatherAPI.cpp Logger.cpp \
      jsonManager.cpp ChartDrawing.cpp TimeManager.cpp TelegramBot.cpp \

# Объектные файлы (будут сгенерированы автоматически)
OBJ = $(SRC:.cpp=.o)

# Цель (исполнимый файл)
TARGET = main

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполнимого файла
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Правило для компиляции исходных файлов в объектные файлы
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка (удаляет объектные и исполнимые файлы)
clean:
	rm -f $(OBJ) $(TARGET)

# Правило для инкрементальной сборки (если добавлены новые файлы)
.PHONY: all clean
