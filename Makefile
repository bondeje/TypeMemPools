.POSIX:
CC = gcc
BIN_DIR = bin
OBJ_DIR = src
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

$(OBJ_DIR)/mempool.o: $(SRC_DIR)/mempool.c $(INC_DIR)/mempool.h
	$(CC) $(BLDCFLAGS) -c $(IFLAGS) $(SRC_DIR)/mempool.c -o $@ $(LFLAGS)

TEST_SRCS = $(SRC_DIR)/mempool.c $(TEST_DIR)/test_mempoolmgr.c $(TEST_DIR)/test_driver.c $(TEST_DIR)/test_utils.c

test: build_hierarchy $(TEST_SRCS)
	$(CC) $(DBGCFLAGS) $(IFLAGS) $(TEST_SRCS) -o $(BIN_DIR)/$@
	$(BIN_DIR)/$@

TEST_SPEED_SRCS = $(SRC_DIR)/mempool.c $(TEST_DIR)/test_speed.c

test_speed: build_hierarchy $(TEST_SPEED_SRCS)
	$(CC) $(CFLAGS) -O0 $(IFLAGS) -Wno-unused-variable $(TEST_SPEED_SRCS) -o $(BIN_DIR)/$@

clean:
	rm -rf $(BIN_DIR)
	rm -f $(OBJ_DIR)/*.o
