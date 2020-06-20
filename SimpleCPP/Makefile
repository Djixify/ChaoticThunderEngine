CC=g++
CFLAGS=-Wall -Wextra -Werror -Iinclude -Iobj

all: initialize hellomake

initialize:
	@if [ ! -d obj ]; then\
    mkdir obj;\
	fi

%.o: %.cpp
	g++ -o obj/$@ -c $< $(CFLAGS)

hellomake: hellomake.o hellofunc.o
	g++ -o hellomake.exe obj/hellomake.o obj/hellofunc.o

clean: 
	rm -r obj