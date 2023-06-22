#include "ClownDB.h"
#include "FileHandle.h"

#include <cassert>
#include <iostream>

/*
FileHandle Tests
	1. Open Empty File, should fail
	2. Create New File, should succeed
	3. Create File again with same name, should fail
	4. Close New File, open file should succeed.
	5. Close file then destroy, should succeed.
	6. Read page that doesnt exist, should fail.
	7. Write page, should succeed.
	8. Read page, should succeed.
*/
void generateRandom() {
	
}

void FileHandleTests() {
	FileHandle fh;
	fh.destroy("db");

	int rc = fh.open("db");
	assert(rc == 1);

	rc = fh.create("db");
	assert(rc == 0);

	//Create again should fail
	rc = fh.create("db");
	assert(rc == 1);

	
	rc = fh.close();
	assert(rc == 0);

	rc = fh.open("db");
	assert(rc == 0);

	rc = fh.close();
	assert(rc == 0);

	rc = fh.destroy("db");
	assert(rc == 0);

	rc = fh.create("db");
	assert(rc == 0);

	void *data = malloc(PAGE_SIZE);
	rc = fh.read(0, data);
	assert(rc == 1);

	free(data);
}

int main() {
	FileHandleTests();
	std::cout << "Tests Passed" << std::endl;
}
