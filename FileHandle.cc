#include "FileHandle.h"

#define EXISTS 0
#define NOT_EXISTS 1

//Returns 0 if file exists
int fileExists(const std::string filePath) {
	FILE *fd = fopen(filePath.c_str(), "r+b");
	if(fd == NULL) {
		return NOT_EXISTS;
	}
	fclose(fd);
	return EXISTS;
}

int FileHandle::getPages(unsigned int *pages) {
	if(fd == NULL) return 1;

	struct stat buf;
	int rc = stat(fileName.c_str(), &buf);
	if(rc) return 1;

	*pages = buf.st_size / PAGE_SIZE;

	return 0;
}

int FileHandle::open(const std::string filePath) {
	if(fd != NULL) return 1;
	fd = fopen(filePath.c_str(), "r+b");
	if(fd == NULL) return 1;
	fileName = filePath;
	return 0;
}

int FileHandle::create(const std::string filePath) {
	if(fileExists(filePath) == EXISTS || fd != NULL) {
		return 1;
	}
	fd = fopen(filePath.c_str(), "w+b");
	if(fd == NULL) return 1;
	fileName = filePath;
	return 0;
}

int FileHandle::destroy(const std::string filePath) {
	if(fileExists(filePath) == NOT_EXISTS) return 1;
	return remove(filePath.c_str());
}

int FileHandle::close() {
	if(fd == NULL) return 1;
	int rc = fclose(fd);
	fd = NULL;
	fileName = "";
	return rc;
}

int FileHandle::read(unsigned int pageNum, void *data) {
	//No file open
	if(fd == NULL) return 1;

	unsigned int pages;
	int rc = getPages(&pages);
	if(rc) return 1;
	if(pageNum >= pages) return 1;

	//seek to beginning of page and read PAGE_SIZE bytes.
	rc = fseek(fd, pageNum * PAGE_SIZE, SEEK_SET);
	fread(data, 1, PAGE_SIZE, fd);
	return 0;
}

int FileHandle::write(unsigned int pageNum, void *data) {
	//No file open
	if(fd == NULL) return 1;

	unsigned int pages;
	int rc = getPages(&pages);
	if(rc) return 1;
	if(pageNum >= pages) return 1;

	//seek to beginning of page and write PAGE_SIZE bytes.
	rc = fseek(fd, pageNum * PAGE_SIZE, SEEK_SET);
	fwrite(data, 1, PAGE_SIZE, fd);
	return 0;
}

int FileHandle::append(void *data) {
	if(fd == NULL) return 1;
	unsigned int pages;
	int rc = getPages(&pages);
	if(rc) return 1;
	
	rc = fseek(fd, pages * PAGE_SIZE, SEEK_SET);
	fwrite(data, 1, PAGE_SIZE, fd);
	return 0;
}


