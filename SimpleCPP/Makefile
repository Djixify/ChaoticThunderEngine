CC=g++
CFLAGS=-I$(INCLUDES)
DEPS=hellomake.h

all: hellomake clean

%.o: %.cpp $(DEPS)
	g++ -o $@ -c $< -Iinclude

hellomake: hellomake.o hellofunc.o
	g++ -o hellomake hellomake.o hellofunc.o

clean: 
	rm hellomake.o hellofunc.o