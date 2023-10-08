#include "BTree.h"

#define METADATA_PAGE 0

BTree::BTree() {
	fh = new FileHandle();
}

BTree::~BTree() {
	delete fh;
}

int BTree::Open(const std::string filePath) {
	int rc = fh->open(filePath);

	// File doesnt exist, create new btree file
	if(rc == 1) {
		rc = createNewFile(filePath);
		return rc;
	}

	//File Exists, check if valid db file.
	return checkValidFile();
}

int BTree::Close() {
	return fh->close();
}

int BTree::Get(std::string key, std::string *value) {
	void *data = malloc(PAGE_SIZE);
	Metadata m(0, 0, 0);
	int rc = fh->read(METADATA_PAGE, data);
	if(rc) {
		free(data);
		return rc;
	}
	memcpy(&m, data, sizeof(Metadata));
	std::cout << "root page: " << m.rootPage << std::endl;
	rc = fh->read(m.rootPage, data);
	if(rc) {
		free(data);
		return rc;
	}
	rc = getFromPage(key, value, m.rootPage);
		
	return rc;
}

int BTree::getFromPage(std::string key, std::string *value, int pageNum) {
	std::cout << "Page NUm " << pageNum << std::endl;
	char *data = (char *) malloc(PAGE_SIZE);
	int rc = fh->read(pageNum, data);
	if(rc) {
		free(data);
		return rc;
	}
	LeafMetaData n;
	memcpy(&n, data, sizeof(LeafMetaData));
	if(n.pageType == LEAF) {
		std::cout << "Leaf : " << pageNum << std::endl;
		rc = findInLeaf(key, value, data);
		free(data);
		return rc;
	} else {
		std::cout << "Non Leaf: " << pageNum << std::endl;
		int nextPage;
		rc = getNextPage(key, data, &nextPage);
		if(rc) {
			free(data);
			return rc;
		}
		return getFromPage(key, value, nextPage);
	}
	return 0;
}

int BTree::getNextPage(std::string key, char *data, int *nextPage) {
	LeafMetaData l;
	int offset = sizeof(LeafMetaData);
	memcpy(&l, data, sizeof(LeafMetaData));
	int freeSpaceOffset = l.freeSpaceOffset;
	int page = -1;
	int keySize;
	while(offset < freeSpaceOffset) {
		if(page == -1) {
			memcpy(&page, data+offset, sizeof(int));
			offset += 4;
		}
		memcpy(&keySize, data+offset, sizeof(int));
		offset+=4;
		std::string keyData(data+offset, keySize);
		std::cout << keyData << std::endl;
		offset+=keySize;
		
		if(key < keyData) break;

		memcpy(&page, data+offset, sizeof(int)); 
		
		offset += sizeof(int);
	}
	*nextPage = page;
	return 0;
}

int BTree::findInLeaf(std::string key, std::string *value, char *data) {
	LeafMetaData l;
	int offset = sizeof(LeafMetaData);
	memcpy(&l, data, sizeof(LeafMetaData));
	int freeSpaceOffset = l.freeSpaceOffset;
	while(offset < freeSpaceOffset) {
		int keySize, valueSize;
		memcpy(&keySize, data+offset, sizeof(int));
		offset += sizeof(int);

		std::string keyCompare(data+offset, keySize);
		std::cout << keyCompare << std::endl;
		offset += keySize;

		memcpy(&valueSize, data+offset, sizeof(int));
		offset += sizeof(int);
	
		std::string valueCompare(data+offset, valueSize);
		offset += valueSize;
		if(keyCompare == key) {
			*value = valueCompare;
			break;
		}
	}
	return 0;
}

int BTree::Delete(std::string key){
	return 1;
}


int BTree::Put(std::string key, std::string value) {
	// Read metadata page
	void *data = malloc(PAGE_SIZE);
	int rc = fh->read(0, data);
	if(rc) {
		free(data);
		return 1;
	}
	Metadata m(0, 0, 0);
	memcpy(&m, data, sizeof(Metadata));
	return insert(key, value, m);
}

int BTree::insert(const std::string &key, const std::string &value, Metadata &m) {
	if(m.firstPage == m.rootPage) {
		return firstInsert(key, value, m);
	}

	return recursiveInsert(key, value, m.rootPage, *returned, &returnedPageNum)
}

int BTree::recursiveInsert(std::string key, std::string *value, int pageNum, std::string *returnedKey, int *returnedPageNum) {
	void *data = malloc(PAGE_SIZE);
	int rc = fh->read(pageNum, data);
	LeafMetaData l;
	memcpy(&l, data, sizeof(LeafMetaData));
	if(l.pageType == NONLEAF) {

	} else {
		
	}
}

int BTree::createNonLeafPage(void *data) {
	// free space offset will be 5, 1 byte to indicate page type, and 4 for free space offset.
	int freeSpaceOffset = sizeof(LeafMetaData);
	char c = NONLEAF;
	LeafMetaData n(c, freeSpaceOffset, -1, -1);
	memcpy(data, &n, sizeof(LeafMetaData));
	return 0;
}

int BTree::createLeafPage(void *data) {
	// free space offset will be 13 bytes, 1 bytes for page type, 4 for free space offset, 4 for prev, 4 for next page due to linked list of leaf pages.
	int freeSpaceOffset = sizeof(LeafMetaData);
	int noPage = -1;
	char c = LEAF;
	LeafMetaData l(c, freeSpaceOffset, noPage, noPage);

	memcpy(data, &l, sizeof(LeafMetaData));
	return 0;
}

int BTree::insertAtLeaf(const std::string &key, const std::string &value, int pageNum, std::string *returnedKey, int *returnedPageNum) {
	char *data = (char *) malloc(PAGE_SIZE);
	int rc = fh->read(pageNum, data);
	if(rc) {
		free(data);
		return 1;
	}

	LeafMetaData l;
	memcpy(&l, data, sizeof(LeafMetaData));
	int offset = sizeOf(LeafMetaData);
	int freeSpaceOffset = l.freeSpaceOffset;
	int keySize = key.size();
	int valueSize = value.size();

	int insert = freeSpaceOffset;

	while(offset < l.freeSpaceOffset) {
		int keySize, valueSize;
		memcpy(&keySize, data+offset, sizeof(int));
		offset += sizeof(int);
		
		std::string keyData(data+offset, keySize);
		if(key < keyData) {
			insert = offset - sizeof(int);
			break;
		}
		offset += keySize;

		memcpy(&valueSize, data+offset, sizeof(int));
		offset += sizeof(int) + valueSize;
	}

	int newSize = key.size() + value.size() + sizeof(int) * 2;
	int enoughSpace = l.freeSpaceOffset + newSize <= PAGE_SIZE;
	
	if(enoughSpace) {
		//regular insert and shift
		rc = 
	} else {
		//split
	}

	rc = fh->write(pageNum, data);
	if(rc) {
		free(data);
		return 1;
	}
	return 0;
}

int BTree::insertAndShift(const std::string key, const std::string value, char *data, int insertOffset) {
	LeafMetaData l;
	memcpy(&l, data, sizeof(LeafMetaData));
	int newEntrySize = key.size() + value.size() + sizeof(int);
	l.freeSpaceOffset += newEntrySize;
	memcpy(data, &l, sizeof(LeafMetaData));

	//shift data past offset to the right to make space for new entry
	memmove(data+offset+newEntrySize, data+offset, newEntrySize);

	int keySize = key.size();
	int valueSize = value.size();

	memcpy(data+offset, &keySize, sizeof(int));
	offset += 4;

	memcpy(data+offset, key.data(), keySize);
	offset+=keySize;

	memcpy(data+offset, &valueSize, sizeof(int));
	offset += sizeof(int);

	memcpy(data+offset, value.data(), valueSize);
	offset += valueSize;
	return 0;
}

int BTree::updateRoot(const std::string &key, int rootPageNum, int leftLeafPage, int rightLeafPage) {
	char *root = (char *) malloc(PAGE_SIZE);
	int rc = fh->read(rootPageNum, root);
	if(rc) {
		free(root);
		return rc;
	}
	LeafMetaData l;
	memcpy(&l, root, sizeof(LeafMetaData));

	int offset = l.freeSpaceOffset;
	memcpy(root+offset, &leftLeafPage, sizeof(int));
	offset += 4;
	
	int keySize = key.size();
	memcpy(root+offset, &keySize, sizeof(int));
	offset += sizeof(int);
	
	memcpy(root+offset, key.data(), keySize);
	offset += keySize;
	
	memcpy(root+offset, &rightLeafPage, sizeof(int));
	offset += sizeof(int);
	l.freeSpaceOffset = offset;

	memcpy(root, &l, sizeof(LeafMetaData));

	rc = fh->write(rootPageNum, root);
	free(root);
	return rc;
}

//On first insert create new root with 2 leaf pages
//Add key-value to right leaf page.
int BTree::firstInsert(const std::string &key, const std::string &value, Metadata &m) {
	void *root = malloc(PAGE_SIZE);
	int rc = createNonLeafPage(root);
	rc = fh->append(root);
	free(root);
	if(rc) return 1;

	unsigned int rootPageNum;
	fh->getPages(&rootPageNum);
	rootPageNum--;


	void *leafLeft = malloc(PAGE_SIZE);
	void *leafRight = malloc(PAGE_SIZE);
	rc = createLeafPage(leafLeft);
	rc = createLeafPage(leafRight);

	rc = fh->append(leafLeft);
	free(leafLeft);
	if(rc) {
		free(leafRight);
		return 1;
	}

	rc = fh->append(leafRight);
	free(leafRight);
	if(rc) return 1;

	//Update metadata on page 0.
	unsigned int rightLeafPage;
	fh->getPages(&rightLeafPage);
	rightLeafPage--;
	rc = insertAtLeaf(key, value, rightLeafPage);
	if(rc) return 1;

	rc = updateRoot(key, rootPageNum, rightLeafPage-1, rightLeafPage);
	if(rc) return 1;


	m.rootPage = rootPageNum;
	m.firstPage = rightLeafPage-1;
	m.lastPage = rightLeafPage;
	
	void *metadataPage = malloc(PAGE_SIZE);
	rc = fh->read(METADATA_PAGE, metadataPage);
	if(rc) {
		free(metadataPage);
	}

	memcpy(metadataPage, &m, sizeof(Metadata));
	rc = fh->write(METADATA_PAGE, metadataPage);
	free(metadataPage);

	return rc;
}

//int ClownDB::recursiveInsert(



//Metadata stored in page 0.
int BTree::createNewFile(std::string filePath) {
	int rc = fh->create(filePath);
	if(rc == 1) return 1;
	Metadata m(0, 0, 0);
	void *data = malloc(PAGE_SIZE);
	memcpy(data, &m, sizeof(Metadata));
	rc = fh->append(data);
	free(data);
	return rc;
}

int BTree::checkValidFile() {
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


