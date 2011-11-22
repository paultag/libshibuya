CXX=g++

ifdef DEBUG
DEBUG_PHRASE=-DSHIBUYA_DEBUG=YES
OPT_FLAGS=-O0 -g
else
OPT_FLAGS=-O2
endif

LIBNAME=libshibuya
LIBMINOR=1.0
LIBMAJOR=1

CFLAGS=-Wall $(OPT_FLAGS) $(DEBUG_PHRASE) -c -fPIC
LDFLAGS=-lncurses -lpanel -lutil \
	-lansiescape -shared -Wl,-soname,$(LIBNAME).so.$(LIBMAJOR)
LIB=../lib/
INC=../include/

TARGETS=$(shell ls *.cc | sed s/.cc/.o/g)

all: $(TARGETS) link clean

clean:
	rm -f *.o

install:
	mkdir -p $(DESTDIR)/usr/include
	mkdir -p $(DESTDIR)/usr/lib
	cp -r $(INC)* $(DESTDIR)/usr/include/
	cp -r $(LIB)* $(DESTDIR)/usr/lib/

link:
	$(CXX) $(LDFLAGS) $(TARGETS) -o $(LIB)$(LIBNAME).so.$(LIBMINOR)

.cc.o:
	$(CXX) $(CFLAGS) -I$(INC) -c -o $@ $<
