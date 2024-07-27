CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -Wno-unused-parameter -Wno-unused-but-set-variable -fPIC
IFLAGS = -Iinclude
LFLAGS = 
BIN_DIR = bin
OBJ_DIR = obj

all: build_hierarchy src/mempool.o

build_hierarchy: $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR): ; @mkdir -p $@

$(BIN_DIR): ; @mkdir -p $@

src/mempool.o: src/mempool.c
	$(CC) $(CFLAGS) -c $(IFLAGS) src/mempool.c -o obj/mempool.o $(LFLAGS)

clean:
	rm -rf bin
	rm -rf obj