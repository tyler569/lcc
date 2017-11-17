
CC = clang
CFLAGS = -Wall -std=c90

OBJECTS = lcc.o errors.o token.o

.PHONY: all clean

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o lcc

clean:
	rm -f lcc *.o a.out


