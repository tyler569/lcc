
CC = clang
CFLAGS = -Wall -std=c89

CFILES = lcc.c errors.c token.c
TARGET = lcc

.PHONY: all clean

$(TARGET): $(CFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(CFILES)

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

