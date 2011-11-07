all:
	cd src/ && make
clean:
	cd src/ && make clean
	rm -rf bin/*
test: all
	./bin/shibuya 2>log
