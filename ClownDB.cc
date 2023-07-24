#include "ClownDB.h"



ClownDB::ClownDB() {
	fh = new FileHandle();
}

ClownDB::~ClownDB() {
	delete fh;
}
/*
int ClownDB::Open(const std::string filePath) {
	int rc = fh->open(filePath);

	// File doesnt exist, create new btree file
	if(rc == 1) {
		rc = createNewFile(filePath);
		return rc;
	}

	//File Exists, check if valid db file.
	return checkValidFile();
}

int ClownDB::Close() {
	return fh->close();
}

int ClownDB::Put(std::string key, std::string value) {
	// Read metadata page
	void *data = malloc(PAGE_SIZE);
	int rc = fh->read(0, data);
	if(rc) {
		free(data);
		return 1;
	}
	Metadata m(0, 0, 0);
	memcpy(&m, data, sizeof(Metadata));
//	int rootPage = m.rootPage;
//	int firstPage = m.firstPage;
	return insert(key, value, m);
}

int ClownDB::insert(const std::string &key, const std::string &value, Metadata &m) {
	if(m.firstPage == m.rootPage) {
		return firstInsert(key, value, m);
	}

//	return recursiveInsert()
	return 0;
}

int ClownDB::createNonLeafPage(void *data) {
	// free space offset will be 5, 1 byte to indicate page type, and 4 for free space offset.
	int freeSpaceOffset = 5;
	char c = NONLEAF;
	memcpy(data, &c, sizeof(char));
	memcpy((char *) data + sizeof(char), &freeSpaceOffset, sizeof(int));
	return 0;
}

int ClownDB::createLeafPage(void *data) {
	// free space offset will be 13 bytes, 1 bytes for page type, 4 for free space offset, 4 for prev, 4 for next page due to linked list of leaf pages.
	int freeSpaceOffset = 1 + 4 + 4 + 4;
	int noPage = -1;
	char c = LEAF;
	int dataOffset = 1;

	memcpy(data, &c, sizeof(char));
	memcpy((char *) data + dataOffset, &freeSpaceOffset, sizeof(int));
	dataOffset += sizeof(int);
	memcpy((char *) data + dataOffset, &noPage, sizeof(int));
	dataOffset += sizeof(int);
	memcpy((char *) data + dataOffset, &noPage, sizeof(int));
	return 0;
}

//On first insert create new root with 2 leaf pages
int ClownDB::firstInsert(Metadata &m) {
	void *root = malloc(PAGE_SIZE);
	int rc = createNonLeafPage(root);
	if(rc) {
		free(root);
		return 1;
	}

	void *leafLeft = malloc(PAGE_SIZE);
	void *leafRight = malloc(PAGE_SIZE);
	rc = createLeafPage(leafLeft);
	rc = createLeafPage(leafRight);

	rc = fh->append(leafLeft);
	if(rc) {
		free(root);
		free(leafLeft);
		free(leafRight);
		return 1;
	}

	rc = fh->append(leafRight);
	if(rc) {
		free(root);
		free(leafLeft);
		free(leafRight);
		return 1;
	}

	rc = fh->write(m.rootPage, data);
	return 0;
}

//int ClownDB::recursiveInsert(

//Metadata stored in page 0.
int ClownDB::createNewFile(std::string filePath) {
	int rc = fh->create(filePath);
	if(rc == 1) return 1;
	Metadata m(1, 1, 1);
	void *data = malloc(PAGE_SIZE);
	memcpy(data, &m, sizeof(Metadata));
	rc = fh->append(data);
	free(data);
	return rc;
}

int ClownDB::checkValidFile() {
	void *data = malloc(PAGE_SIZE);
	int rc = fh->read(0, data);
	if(rc) {
		free(data);
		return 1;
	}
	Metadata m(1, 1, 1);
	memcpy(&m, data, sizeof(Metadata));
	free(data);

	std::string check = m.check;
	if(check != "clownDB") {
		fh->close();
		return 1;
	}
	return 0;
}
*/

