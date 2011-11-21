all:
	cd src/ && make

clean:
	cd src/ && make clean
	rm -rf bin/*

all-dbg:
	cd src/ && make DEBUG=YES

test: all-dbg
	cd tests && ./run.sh

debug: all-dbg
	gdb ./bin/shibuya
	./meta/alloc.py < ./shibuya.debug.log
memleak: all-dbg
	valgrind --tool=memcheck --leak-check=full ./bin/shibuya 2>valgrind.debug.log
	./meta/alloc.py < ./shibuya.debug.log
