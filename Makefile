all:
	cd src/ && make

clean:
	cd src/ && make clean
	rm -rf bin/*

test: all
	cd tests && ./run.sh

debug: all
	valgrind ./bin/shibuya 2>valgrind.debug.log
	./meta/alloc.py < ./shibuya.debug.log
