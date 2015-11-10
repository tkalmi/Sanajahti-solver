CC=g++
CPPFLAGS=-c -g -std=c++0x -Wall -Wextra -pedantic $(INC)
LIBS=-Llibraries/SFML-2.2/lib -lsfml-graphics -lsfml-window -lsfml-system
INC=-Ilibraries/SFML-2.2/include
SOURCES=main.cpp solver.cpp solver.hpp GUI.hpp GUI.cpp Screens.hpp Screens.cpp
OBJECTS=main.o solver.o GUI.o Screens.o
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
  
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBS) $(LDFLAGS) -o $@ 

.cc.o:
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: main
	./main
