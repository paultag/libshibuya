CXX=g++

ifdef DEBUG
DEBUG_PHRASE=-DSHIBUYA_DEBUG=YES
OPT_FLAGS=-O0 -g
else
OPT_FLAGS=-O2
endif

CFLAGS=-Wall $(OPT_FLAGS) $(DEBUG_PHRASE)
LDFLAGS=-lncurses -lpanel -lutil -lansiescape
BIN=../bin/
INC=../include/

TARGETS=$(shell ls *.cc | sed s/.cc/.o/g)


all: $(TARGETS) link clean

clean:
	@echo "Removing all .o files."
	@rm -f *.o

link:
	$(CXX) $(TARGETS) $(LDFLAGS) -o $(BIN)shibuya

.cc.o:
	$(CXX) $(CFLAGS) -I$(INC) -c -o $@ $<
