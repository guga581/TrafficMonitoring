C = gcc
INC_DIR = ./include
SRC_DIR = ./src
BIN_DIR = ./bin
DST_DIR = ./dst
LOG_DIR = ./log
all: client server

client:	$(SRC_DIR)/client_tcp.c
	$(CC) -o $(DST_DIR)/client $(SRC_DIR)/client_tcp.c -lpthread

server: $(SRC_DIR)/server_tcp.c
	$(CC) -o $(DST_DIR)/server $(SRC_DIR)/server_tcp.c -lpthread

clean:
	rm $(LOG_DIR)/*.csv
	rm $(DST_DIR)/*
