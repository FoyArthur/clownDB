#ifndef FileHandle_h
#define FileHandle_h

#include <cstdio>
#include <iostream>
#include <cstring>
#include <sys/stat.h>

#define PAGE_SIZE 4096
#include <string>

class FileHandle {
	public:
		int open(std::string filePath);
		int create(std::string filePath);
		int destroy(std::string filePath);
		int close();
		int read(unsigned int pageNum, void *data);
		int write(unsigned int pageNum, void *data);

	private:
		FILE *fd = NULL;
		std::string fileName;

		int getPages(unsigned int *pages);
};


#endif 
