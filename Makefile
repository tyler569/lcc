
CC = clang
CFLAGS = -Wall -std=c90

OBJECTS = lcc.o

.PHONY: all clean

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o lcc

clean:
	rm lcc *.o


