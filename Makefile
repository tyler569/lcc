
CC = clang
CFLAGS = -Wall -std=c99 -g

CFILES = lcc.c errors.c token.c typer.c
CHEADERS = errors.h token.h typer.h
TARGET = lcc
MAKEFILE = ./Makefile

.PHONY: all clean

$(TARGET): $(CFILES) $(MAKEFILE) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(CFILES)

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

