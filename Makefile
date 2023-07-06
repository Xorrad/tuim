all: build run clean

build:
	@g++ -c test.cpp
	@g++ test.o -o test

run:
	@./test

clean:
	@rm -f test.o test