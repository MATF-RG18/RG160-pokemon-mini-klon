PROGRAM   = Pokemon
CC        = gcc
CFLAGS    = -g -Wall
LDFLAGS   = -lGL -lGLU -lglut -lopenal -lalut

$(PROGRAM): main.o pokemon.o image.o
	$(CC) -o $(PROGRAM) image.o pokemon.o main.o $(LDFLAGS)

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)


