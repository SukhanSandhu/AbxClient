# File: Makefile

CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude
SRC = src/main.cpp src/TcpClient.cpp src/AbxClient.cpp src/JsonBuilder.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = abxClient

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) $(OBJ)
