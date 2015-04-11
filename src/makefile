CC = g++
PROG = netmap
CFLAGS = -Wall
SRCS = *.cpp
BIN_DIR = ../bin

all: bin netmap

bin:
	mkdir -p ${BIN_DIR}

netmap:
	$(CC) $(SRCS) -o $(BIN_DIR)/$(PROG) $(CFLAGS)

clean:
	@rm -f $(BIN_DIR)/$(PROG) $(BIN_DIR)/*.o core
