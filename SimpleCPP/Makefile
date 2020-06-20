CC=g++
CFLAGS=-Wall -Wextra -Werror -Iinclude

all: hellomake clean

%.o: %.cpp
	g++ -o $@ -c $< $(CFLAGS)

hellomake: hellomake.o hellofunc.o
	g++ -o hellomake hellomake.o hellofunc.o

clean: 
	rm hellomake.o hellofunc.o