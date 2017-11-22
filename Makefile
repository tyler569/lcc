
CC = clang
CFLAGS = -Wall -std=c11 -g

OPT = 0
ADDITIONAL_FLAGS = ""
CFLAGS += -O$(OPT)
CFLAGS += $(ADDITIONAL_FLAGS)

CFILES = lcc.c errors.c token.c typer.c ast.c vector.c
CHEADERS = errors.h token.h typer.h ast.h vector.h
TARGET = lcc
MAKEFILE = ./Makefile

.PHONY: all clean format msan

# I am not doing incremental compilation because this
# is still fast.  If it ever stops being, or if it
# isn't fast for you, it will be easy to move to
# an incremental approach.

$(TARGET): $(CFILES) $(MAKEFILE) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(CFILES)

all: $(TARGET)

asan:
	make ADDITIONAL_FLAGS="-fsanitize=address"

format:
	clang-format -style="{BasedOnStyle: Webkit, IndentWidth: 4}" -i *.c *.h

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf *.dSYM
	rm -f a.out

