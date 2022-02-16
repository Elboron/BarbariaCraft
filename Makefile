.POSIX:
.SUFFIXES: .o .c

CC = gcc
LD = ld

BIN = barbaria

OBJ =\
	 main.o\
	 input_handling.o

SRC =\
	main.c\
	input_handling.c

LDFLAGS = -lGL -lGLEW -lglfw -lm -pthread

all: $(BIN)
$(BIN): $(OBJ)
$(OBJ): $(SRC)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -Wall -g3 -c -o $@ $<

clean:
	rm -f $(BIN) $(OBJ)

.PHONY: all clean
