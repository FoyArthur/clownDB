#ifndef clownDB_h
#define clownDB_h

#include <string>
#include "FileHandle.h"

#define LEAF 0
#define NONLEAF 1

struct Metadata {
	char check[8] = "clownDB";
	unsigned int rootPage;
	unsigned int firstPage;
	unsigned int lastPage;

	Metadata(unsigned int _rootPage, unsigned int _firstPage, unsigned int _lastPage): rootPage(_rootPage),
		firstPage(_firstPage), lastPage(_lastPage) {};
};

struct data {
	char *data;
	int length;
};

class ClownDB {
	public:
		ClownDB();
		~ClownDB();
		virtual int Get(std::string key)=0;
		virtual int Put(std::string, std::string)=0;
		virtual int Delete(std::string key)=0;
		virtual int Open(const std::string filePath)=0;
		virtual int Close()=0;
	private:
		FileHandle *fh;

		int insert(const std::string &key, const std::string &value, Metadata &m);
		int createNewFile(std::string filePath);
		int checkValidFile();
		int firstInsert(Metadata &m);
		int createNonLeafPage(void *data);
};

#endif
