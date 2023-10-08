#ifndef unordered_h
#define unordered_h

#include "ClownDB.h"

#define DELETE 0
#define NO_DELETE 1

class Unordered: public ClownDB {
	public:
		Unordered();
		~Unordered();
		int Get(std::string key, std::string *value) override;
		int Put(std::string key, std::string value) override;
		int Delete(std::string key) override;
		int Open(const std::string filePath) override;
		int Close() override;
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
#endif
