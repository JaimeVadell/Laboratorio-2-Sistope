CC = gcc
CFLAGS = -o
SRC_DIR = src
OUTPUT_DIR = output

all: broker worker lab2

broker: $(SRC_DIR)/broker.c $(SRC_DIR)/funciones.c
	$(CC) $(CFLAGS) $(OUTPUT_DIR)/broker $^ -lm

worker: $(SRC_DIR)/worker.c $(SRC_DIR)/funciones.c
	$(CC) $(CFLAGS) $(OUTPUT_DIR)/worker $^ -lm

lab2: $(SRC_DIR)/lab2.c
	$(CC) $(CFLAGS) $(OUTPUT_DIR)/lab2 $^

clean:
	rm -f $(OUTPUT_DIR)/*

.PHONY: all clean