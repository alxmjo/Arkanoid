# Author: Alexander Johnson
# Date: 2017/02/22
CXX = g++
CXXFLAGS = -std=c++11 -Wall -pedantic-errors
EXECUTABLE = main
SOURCES = main.cpp
HEADERS =

all: $(SOURCES) $(HEADERS)
	$(CXX) $(SOURCES) -o $(EXECUTABLE) $(CXXFLAGS)

clean:
	rm -f a.out *.o $(EXECUTABLE)