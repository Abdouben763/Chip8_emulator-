# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

# Source files
SRC = src/main.c src/display.c src/chip8.c src/input.c src/timer.c src/config.c
OBJ = $(SRC:.c=.o)

# Output executable
TARGET = chip8

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile .c -> .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET)
