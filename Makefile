# ================================================================
# CHIP-8 Emulator Makefile
# Author: Abderrahmane benchikh
# ================================================================

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -Iinclude `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lm

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# Source files (automatically detect all .c files)
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Output executable
TARGET = chip8

# Colors for output
GREEN = \033[0;32m
YELLOW = \033[1;33m
RED = \033[0;31m
NC = \033[0m

# Default target
all: $(TARGET)

# Create object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Created object directory$(NC)"

# Link executable
$(TARGET): $(OBJ_DIR) $(OBJECTS)
	@echo "$(GREEN)Linking: $@$(NC)"
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "$(GREEN)Build successful!$(NC)"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "$(YELLOW)Compiling: $<$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Run with test ROM
run: $(TARGET)
	@echo "$(GREEN)Running emulator...$(NC)"
	@./$(TARGET) roms/Brick.ch8 2>/dev/null || echo "$(RED)Usage: ./$(TARGET) <rom_file>$(NC)"

# Clean build files
clean:
	@echo "$(RED)Cleaning...$(NC)"
	@rm -rf $(OBJ_DIR) $(TARGET)

# Show help
help:
	@echo "$(GREEN)CHIP-8 Emulator Makefile$(NC)"
	@echo "Available targets:"
	@echo "  all      - Build the emulator (default)"
	@echo "  run      - Build and run emulator"
	@echo "  clean    - Remove build files"
	@echo "  help     - Show this help"
