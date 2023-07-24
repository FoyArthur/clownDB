#include "ClownDB.h"

class Unordered: public ClownDB {
	public:
		Unordered();
		int Get(std::string key);
		int Put(std::string, std::string);
		int Delete(std::string key);
		int Open(const std::string filePath);
		int Close();
	private:
		FileHandle *fh;

		int createNewFile(std::string key);
		int checkValidFile();
		int enoughSpace(std::string key, std::string value, int numPages);
		void *createNewPage();

		int insertAtEnd(std::string key, std::string value, int numPages);
		int insertNewPage(std::string key, std::string value);
		void *insert(std::string key, std::string value);

};
