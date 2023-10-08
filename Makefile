all: test

test: test.cc ClownDB.cc FileHandle.cc BTree.cc Unordered.cc
	g++ -Wall -Werror -Wpedantic -g -o test test.cc ClownDB.cc BTree.cc FileHandle.cc Unordered.cc

clean:
	rm -rf test
