CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
SOURCES=test.cpp CountMin_basic.cpp Hash.cpp CountSketch_basic.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test
RM=rm -rf 

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o *.*~ $(EXECUTABLE) Ma*~
