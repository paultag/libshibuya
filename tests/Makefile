CXX=g++

ifdef DEBUG
DEBUG_PHRASE=-DSHIBUYA_DEBUG=YES
OPT_FLAGS=-O0 -g
else
OPT_FLAGS=-O2
endif

CFLAGS=-Wall $(OPT_FLAGS) $(DEBUG_PHRASE)
LDFLAGS=-L$(LIB) -lncurses -lpanel -lutil -lansiescape -lshibuya
BIN=./bin/
INC=../include/
LIB=../lib/

TARGETS=$(shell ls *.cc | sed s/.cc/.o/g)


all: $(TARGETS) link clean

clean:
	rm -f *.o

link:
	$(CXX) $(TARGETS) $(LDFLAGS) -o $(BIN)shibuya

.cc.o:
	$(CXX) $(CFLAGS) -I$(INC) -c -o $@ $<
