# Compiler and flags
CC = gcc
CFLAGS = -I include -Wall -Werror

# Directories
SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/main

# Source files
SRC_FILES = \
    $(SRC_DIR)/main.c \
    $(SRC_DIR)/transport/tty.c \
    $(SRC_DIR)/board/commands.c \
    $(SRC_DIR)/packet/controlPacket.c \

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRC_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@
# $^: All prerequisites (SRC_FILES)
# $@: Target (BIN_DIR/main)

# Clean target
clean:
	rm -rf $(TARGET)

.PHONY: all clean