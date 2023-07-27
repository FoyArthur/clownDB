#include "ClownDB.h"

#define DELETE 0
#define NO_DELETE 1

class Unordered: public ClownDB {
	public:
		Unordered();
		~Unordered();
		int Get(std::string key, std::string *value);
		int Put(std::string key, std::string value);
		int Delete(std::string key);
		int Open(const std::string filePath);
		int Close();
	private:
		FileHandle *fh;

		int createNewFile(std::string key);
		int checkValidFile();
		int enoughSpace(std::string key, std::string value, int numPages);
		void *createNewPage();
		int searchAndDelete(std::string key, int numPages, std::string *value, int del);

		int insertAtEnd(std::string key, std::string value, int numPages);
		int insertNewPage(std::string key, std::string value);
		void *insert(std::string key, std::string value);

};
