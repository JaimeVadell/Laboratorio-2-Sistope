CC = gcc
CFLAGS = -o
SRC_DIR = src
OUTPUT_DIR = output

all: broker worker lab1

broker: $(SRC_DIR)/broker.c $(SRC_DIR)/funciones.c
	$(CC) $(CFLAGS) $(OUTPUT_DIR)/broker $^ -lm

worker: $(SRC_DIR)/worker.c $(SRC_DIR)/funciones.c
	$(CC) $(CFLAGS) $(OUTPUT_DIR)/worker $^ -lm

lab1: $(SRC_DIR)/lab1.c
	$(CC) $(CFLAGS) $(OUTPUT_DIR)/lab1 $(SRC_DIR)/lab1.c

clean:
	rm -f $(OUTPUT_DIR)/*

.PHONY: all clean