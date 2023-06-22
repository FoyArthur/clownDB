#ifndef clownDB_h
#define clownDB_h
#include <string>

struct data {
	char *data;
	int length;
};

class ClownDB {
	public:
		ClownDB(){};
		int Get(std::string key);
		int Put(std::string);
		int Delete(std::string key);
		int Open(const std::string s);
	private:

};

#endif
