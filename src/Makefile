CC=g++
CPPFLAGS=-c -g -std=c++0x -Wall -Wextra -pedantic
LIBS=-lsfml-graphics -lsfml-window -lsfml-system -llept -ltesseract
SOURCES=main.cpp solver.cpp solver.hpp GUI.hpp GUI.cpp Screens.hpp Screens.cpp ocr.cpp ocr.hpp
OBJECTS=main.o solver.o GUI.o Screens.o ocr.o
EXECUTABLE=ssolver

all: $(SOURCES) $(EXECUTABLE)
  
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBS) $(LDFLAGS) -o $@ 

.cc.o:
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: all
	./ssolver
