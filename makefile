CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = iscas
SOURCES = iscas.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
		$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
		rm -f $(TARGET)

