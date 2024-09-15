CC = gcc
CFLAGS = -pthread -g -I./includes
# CFLAGS += -fsanitize=thread

SRC_DIR = src
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/manager.c $(SRC_DIR)/new_async.c $(SRC_DIR)/start_async.c $(SRC_DIR)/stop_async.c $(SRC_DIR)/queue.c
OBJ = $(SRC:.c=.o)
TARGET = async_program

all: $(TARGET)

run:  re
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)
#	./$(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

re: clean all

.PHONY: all clean run re