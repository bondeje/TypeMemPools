CC = gcc
BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
INC_DIR = include
TEST_DIR = test
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -Wno-unused-parameter -Wno-unused-but-set-variable -DNDEBUG -fPIC
BLDCFLAGS = $(CFLAGS) -O2
IFLAGS = -I$(INC_DIR)
LFLAGS = 
DBGCFLAGS = $(CFLAGS) -g3 -fsanitize=address,undefined -UNDEBUG -O0 -std=c11

all: build_hierarchy $(OBJ_DIR)/mempool.o

build_hierarchy: 
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

$(OBJ_DIR)/mempool.o: $(SRC_DIR)/mempool.c
	$(CC) $(BLDCFLAGS) -c $(IFLAGS) $^ -o $@ $(LFLAGS)

TEST_SRCS = $(SRC_DIR)/mempool.c $(TEST_DIR)/test_mempoolmgr.c $(TEST_DIR)/test_driver.c $(TEST_DIR)/test_utils.c

$(BIN_DIR)/test_mempool: $(TEST_SRCS)
	$(CC) $(DBGCFLAGS) $(IFLAGS) $(TEST_SRCS) -o $(BIN_DIR)/test_mempool

test: build_hierarchy $(BIN_DIR)/test_mempool
	$(BIN_DIR)/test_mempool

TEST_SPEED_SRCS = $(SRC_DIR)/mempool.c $(TEST_DIR)/test_speed.c

$(BIN_DIR)/test_speed: $(TEST_SPEED_SRCS)
	$(CC) $(CFLAGS) -O0 $(IFLAGS) -Wno-unused-variable $(TEST_SPEED_SRCS) -o $(BIN_DIR)/test_speed

test_speed: build_hierarchy $(BIN_DIR)/test_speed 
	$(BIN_DIR)/test_speed

clean:
	rm -rf bin
	rm -rf obj