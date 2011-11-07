all:
	cd src/ && make
clean:
	cd src/ && make clean
	rm -rf bin/*
test: all
	./bin/boothby 2>log
