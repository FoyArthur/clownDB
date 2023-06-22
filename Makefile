all: test

test: test.cc ClownDB.cc FileHandle.cc
	g++ -Wall -Werror -Wpedantic -o test test.cc ClownDB.cc FileHandle.cc

clean:
	rm -rf test
