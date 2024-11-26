# Название конечного исполняемого файла
TARGET = main

# Компилятор и флаги
CXX = g++
CXXFLAGS = -pthread -Wall -Wextra
LDFLAGS = -lwiringPi -lz -lcurl

# Список исходных файлов
SRCS = main.cpp Executor.cpp I2CLCD.cpp RelayControlModule.cpp \
       DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp \
       SerialComm.cpp DataCombine.cpp crcCalc.cpp DataGeter.cpp \
       DS18B20.cpp WeatherAPI.cpp

# Автоматическое определение объектных файлов
OBJS = $(SRCS:.cpp=.o)

# Правило сборки всего проекта
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Правило компиляции каждого .cpp в .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка временных файлов
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
