IDIR=/include

CC=g++
CFLAGS=-I$(IDIR)
DEPS = hellomake.h

%.o: %.cpp $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

hellomake: hellomake.o hellofunc.o
	g++ -o hellomake hellomake.o hellofunc.o