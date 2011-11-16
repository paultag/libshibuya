all:
	cd src/ && make
clean:
	cd src/ && make clean
	rm -rf bin/*
test: all
	cd tests && ./run.sh
