CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=test.cpp Hash.cpp CountMin.cpp
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
