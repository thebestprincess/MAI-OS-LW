CC = gcc
CFLAGS = -I./include -Wall -Wextra -pthread -fsanitize=thread
LDFLAGS = -pthread -fsanitize=thread

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/program.out

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clear:
	rm -rf $(BUILD_DIR)

.PHONY: all clean