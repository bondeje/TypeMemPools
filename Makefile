.POSIX:
# because what the hell is going on with FreeBSD make
.OBJDIR: .
.SUFFIXES:

CC = gcc
BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
TEST_DIR = test
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -Wno-unused-parameter -Wno-unused-but-set-variable -DNDEBUG -fPIC
BLDCFLAGS = $(CFLAGS) -O2
IFLAGS = -I$(INC_DIR)
DBGLFLAGS =
LFLAGS = 
DBGCFLAGS = $(CFLAGS) -g3 -fsanitize=address,undefined -UNDEBUG -O0 -std=c11

all: build_hierarchy $(SRC_DIR)/mempool.o

build_hierarchy: 
	mkdir -p $(BIN_DIR)

TEST_SRCS = $(SRC_DIR)/mempool.do $(TEST_DIR)/test_mempoolmgr.do $(TEST_DIR)/test_driver.do $(TEST_DIR)/test_utils.do

test: build_hierarchy $(TEST_SRCS)
	$(CC) $(DBGCFLAGS) $(TEST_SRCS) -o $(BIN_DIR)/$@ $(DBGLFLAGS)
	$(BIN_DIR)/$@

TEST_SPEED_SRCS = $(SRC_DIR)/mempool.c $(TEST_DIR)/test_speed.c

test_speed: build_hierarchy $(TEST_SPEED_SRCS)
	$(CC) $(CFLAGS) -O0 -Wno-unused-variable $(IFLAGS) $(TEST_SPEED_SRCS) -o $(BIN_DIR)/$@ $(LFLAGS)

clean:
	rm -rf $(BIN_DIR)
	rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*.do
	rm -f $(TEST_DIR)/*.o $(TEST_DIR)/*.do

.SUFFIXES: .c .o .do
.c.o:
	$(CC) $(BLDCFLAGS) -c $(IFLAGS) $< -o $@

.c.do:
	$(CC) $(DBGCFLAGS) -c $(IFLAGS) $< -o $@
