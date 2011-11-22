BUILDCHAIN=../meta/buildchain/
include $(BUILDCHAIN)/library-info.mk

INC=../include/
BIN=./bin/
LIB=../lib/

TARGETS=$(shell ls *.cc | sed s/.cc/.o/g)
LIBS=-lncurses -lpanel -lutil -lansiescape -lshibuya
CXXFLAGS=-Wall -I$(INC)
LDFLAGS=-L$(LIB) $(LIBS)

OUTPUT=$(BIN)shibuya

all: $(TARGETS) link

clean:
	rm *.o

include $(BUILDCHAIN)build.mk
