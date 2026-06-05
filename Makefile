
CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = sfml-app
SRC = main.cpp board.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

