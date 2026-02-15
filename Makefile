CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = app.exe
SOURCES = main.cpp value.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
