CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC_DIR = src
OBJ_DIR = build
BIN = assembler

# find all .c files recursively in src/
SRC = $(shell find $(SRC_DIR) -name "*.c")
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# rule to compile each .c into .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN)
