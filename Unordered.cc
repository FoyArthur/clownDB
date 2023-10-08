#include "Unordered.h"

Unordered::Unordered() {
	fh = new FileHandle();
}

Unordered::~Unordered() {
	delete fh;
}

int Unordered::Open(std::string filePath) {
	int rc = fh->open(filePath);
	if(rc == 1) {
		rc = createNewFile(filePath);
		return rc;
	}
	return checkValidFile();
}

int Unordered::Close() {
	int rc = fh->close();
	return rc;
}

int Unordered::Get(std::string key, std::string *value) {
	unsigned int numPages;
	int rc = fh->getPages(&numPages);
	if(rc) return rc;
	rc = searchAndDelete(key, numPages, value, NO_DELETE);
	return 0;
}

int Unordered::Put(std::string key, std::string value) {
	//Read in last page. Check if enough space
	// If enough space, append key value pair, else add new page.

	//Search pages for existing key
	//If exists delete it.
	unsigned int numPages;
	int rc = fh->getPages(&numPages);
	if(rc) return 1;
	
	std::string valueReturn;
	rc = searchAndDelete(key, numPages, &valueReturn, DELETE);
	if(rc) return 1;

	int enough = enoughSpace(key, value, numPages);

	if(enough) {
		return insertAtEnd(key, value, numPages);
	} else {
		return insertNewPage(key, value);
	}
}

int Unordered::Delete(std::string key) {
	unsigned int numPages;
	int rc = fh->getPages(&numPages);
	if(rc) return 1;
	
	std::string valueReturn;
	rc = searchAndDelete(key, numPages, &valueReturn, DELETE);
	return rc;
}

// 8 bytes for length of key and length of value
int Unordered::enoughSpace(std::string key, std::string value, int numPages) {
	char *data = (char *) malloc (PAGE_SIZE);
	int rc = fh->read(numPages-1, data);
	if(rc) {
		free(data);
		return rc;
	}

	int offset;
	memcpy(&offset, data, sizeof(int));

	int keyValueSize = key.size() + value.size() + 8;
	free(data);
	return PAGE_SIZE - offset >= keyValueSize;
}

void *Unordered::insert(std::string key, std::string value) {
	char *data = (char *) malloc(key.size()+value.size()+8);
	int keySize = key.size();
	int valueSize = value.size();
	int dataOffset = 0;

	memcpy(data, &keySize, sizeof(int));
	dataOffset += sizeof(int);
	memcpy(data + dataOffset, key.data(), keySize);
	dataOffset += keySize;

	memcpy(data + dataOffset, &valueSize, sizeof(int));
	dataOffset += sizeof(int);
	memcpy(data + dataOffset, value.data(), valueSize);
	
	return (void *) data;
}

//insert keylength, key, valuelength, value, at freespaceoffset, then update the offset.
int Unordered::insertAtEnd(std::string key, std::string value, int numPages) {
	void *data = malloc(PAGE_SIZE);
	int rc = fh->read(numPages-1, data);
	if(rc) {
		free(data);
		return rc;
	}
	int offset;
	memcpy(&offset, data, sizeof(int));
	
	int insertedDataSize = key.size() + value.size() + 8;
	void *insertedData = insert(key, value);
	memcpy((char *) data + offset, insertedData, insertedDataSize);
	offset += insertedDataSize;
	memcpy(data, &offset, sizeof(int));

	free(insertedData);
	rc = fh->write(numPages-1, data);
	free(data);

	return rc;
}

int Unordered::insertNewPage(std::string key, std::string value) {
	void *data = createNewPage();
	void *insertedData = insert(key, value);

	int insertedDataSize = key.size() + value.size() + 8;
	memcpy((char *)data+4, insertedData, insertedDataSize);
	insertedDataSize += 4;
	memcpy(data, &insertedDataSize, sizeof(int));

	int rc = fh->append(data);
	free(data);
	free(insertedData);
	return rc;
}

//Searches all pages for key and deletes it if it exists
int Unordered::searchAndDelete(std::string key, int numPages, std::string *value, int del) {
	void *data = malloc(PAGE_SIZE);

	for(int i = 0; i < numPages; i++) {
		int rc = fh->read(i, data);
		if(rc) {
			free(data);
			return rc;
		}

		int offset;
		memcpy(&offset, data, sizeof(int));
		int current = 4;

		// Iterate through page, if key matches, delete key value pair
		while(current < offset) {
			int pairStart = current;
			int keySize;
			memcpy(&keySize, (char *) data + current, sizeof(int));
			current += sizeof(int);
			char *keyData = (char *) calloc(1, keySize+1);
			memcpy(keyData, (char *) data + current, keySize);
			std::string keyString(keyData);
			current += keySize;
			free(keyData);

			int valueSize;
			memcpy(&valueSize, (char *) data + current, sizeof(int));
			current += sizeof(int);

			char *valueData = (char *) calloc(1, valueSize+1);
			memcpy(valueData, (char *) data + current, valueSize);
			current += valueSize;

			//std::cout << keySize << " " << valueSize << std::endl;

			if(key == keyString) {
				//go to end of key, value pair and shift left overwriting it and deleting it
				if(del == DELETE) {
					int pairSize = keySize + valueSize + sizeof(int) * 2;
					memmove((char *) data + pairStart, (char *) data + current, offset-current);
					offset -= pairSize;
					memcpy(data, &offset, sizeof(int));
					fh->write(i, data);
				} else {
					*value = valueData;
				}

				free(valueData);
				free(data);
				return 0;
			}

			free(valueData);
		}
	}
	free(data);

	return 0;
}

void *Unordered::createNewPage() {
	void *data = malloc(PAGE_SIZE);
	int offset = 4;
	memcpy(data, &offset, sizeof(int));
	return data;
}

int Unordered::createNewFile(std::string filePath) {
	int rc = fh->create(filePath);
	if(rc) return 1;
	void *data = createNewPage();
	rc = fh->append(data);
	free(data);
	return rc;
}

int Unordered::checkValidFile() {
	return 0;
}
