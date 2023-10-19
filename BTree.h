#include "ClownDB.h"

/*

struct NonLeafMetaData {
	char pageType;
	int freeSpaceOffset;
	NonLeafMetaData(char _pageType, int _freeSpaceOffset): pageType(_pageType), freeSpaceOffset(_freeSpaceOffset){};
	NonLeafMetaData():pageType(0), freeSpaceOffset(0){};
};
*/

struct LeafMetaData {
	char pageType;
	int freeSpaceOffset;
	int previousPageNumber;
	int nextPageNumber;
	LeafMetaData(char _pageType, int _freeSpaceOffset, int _previousPageNumber, int _nextPageNumber): pageType(_pageType), freeSpaceOffset(_freeSpaceOffset),
		previousPageNumber(_previousPageNumber), nextPageNumber(_nextPageNumber) {};

	LeafMetaData():pageType(0), freeSpaceOffset(0), previousPageNumber(0), nextPageNumber(0){};
};

class BTree: public ClownDB {
	public:
		BTree();
		~BTree();
		int Get(std::string key, std::string *value) override;
		int Put(std::string key, std::string value) override;
		int Delete(std::string key) override;
		int Open(const std::string filePath) override;
		int Close() override;
	private:
		FileHandle *fh;

		int createNonLeafPage(void *data);
		int createLeafPage(void *data);
		int firstInsert(const std::string &key, const std::string &value, Metadata &m);
		int createNewFile(std::string filePath);
		int checkValidFile();
		int insert(const std::string &key, const std::string &value, Metadata &m);
		int insertAtLeaf(const std::string &key, const std::string &value, int pageNum, std::string *returnedKey, int *returnedPageNum);
		int updateRoot(const std::string &key, int rootPageNum, int leftLeaf, int rightLeaf);
		int getFromPage(std::string key, std::string *value, int pageNum);
		int findInLeaf(std::string key, std::string *value, char *data);
		int getNextPage(std::string key, char *data, int *nextPage);
		int recursiveInsert(const std::string key, const std::string &value, int pageNum, std::string *returnedKey, int *returnedPageNum);
		int insertAndShift(const std::string key, const std::string value, char *data, int insertOffset);
		int insertSplit(const std::string key, const std::string value, int pageNum, int insertOffset, std::string *returnedKey, int *returnedPage);
		int findPage(const std::string key, const std::string &value, int pageNum, int *page);
};
