LIBNAME=libshibuya
LIBMINOR=1.0
LIBMAJOR=1
LIB=./lib/

all: build link

build:
	cd src/ && make

clean:
	cd src/ && make clean
	rm -rf bin/*

install:
	@cd src   && make install

link:
	ln -sf $(LIBNAME).so.$(LIBMINOR)  $(LIB)$(LIBNAME).so.$(LIBMAJOR)
	ln -sf $(LIBNAME).so.$(LIBMINOR)  $(LIB)$(LIBNAME).so

manual-test: all
	cd tests && make
