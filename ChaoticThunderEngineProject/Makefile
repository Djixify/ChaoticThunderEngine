
#This is for later optimizations that we cannot expect from visual studio

dir=$(cd)
CC=g++
CFLAGS=-Wall -Wextra -Werror -Iinclude -Iobj

all: initialize hellomake

initialize:
	IF exist obj ( echo obj exists already ) ELSE ( md obj && echo obj created)

%.o: %.cpp
	g++ -o obj/$@ -c $< $(CFLAGS)

hellomake: hellomake.o hellofunc.o
	g++ -o hellomake.exe obj/hellomake.o obj/hellofunc.o

run: hellomake.exe
	./hellomake.exe
	
clean: 
	rd /S /Q obj
	
print:
	echo $(ProjectDir)