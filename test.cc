#include "ClownDB.h"
#include "Unordered.h"
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
	7. Write page, should fail.
	8. Append Page, should succeed.
	8. Read page, should succeed.
	9. Overwrite Page with different data, should succeed.
	10. Append 10 Pages, Read 3rd and 10th make sure its correct.
*/

void *generateRandom() {
	char *data = (char *) malloc(PAGE_SIZE);
	char letter = 'a';
	int offset = 0;
	for(int i = 0; i < 4096; i++) {
		memcpy(data+offset, &letter, 1);
		letter = letter == 'z' ? 'a': letter+1;
		offset++;
	}
	return (void *) data;
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

	void *data = generateRandom();
	rc = fh.read(0, data);
	assert(rc == 1);

	rc = fh.write(0, data);
	assert(rc == 1);

	rc = fh.append(data);
	assert(rc == 0);

	void *readData = malloc(PAGE_SIZE);
	rc = fh.read(0, readData);
	assert(rc == 0);
	rc = memcmp(data, readData, PAGE_SIZE);
	assert(rc == 0);

	void *data2 = generateRandom();
	memset(data2, 0, 2);
	rc = fh.write(0, data2);
	assert(rc == 0);

	rc = fh.read(0, readData);
	assert(rc == 0);
	rc = memcmp(data, readData, PAGE_SIZE);
	assert(rc != 0);

	for(int i = 1; i < 10; i++) {
		rc = fh.append(data2);
		assert(rc == 0);
	}
	
	rc = fh.read(10, readData);
	assert(rc != 0);

	for(int i = 1; i < 10; i++) {
		rc = fh.read(i, readData);
		assert(rc == 0);
		rc = memcmp(data2, readData, PAGE_SIZE);
		assert(rc == 0);
	}

	free(readData);
	free(data2);
	free(data);
}

/* Clown DB Tests
 1. Open New File Should succeed.
 2. Close File should succeed.
 3. Open other non-db file should not succeed.
 4. Open existing File should succeed.
*/

void ClownDBTests() {
	/*
	ClownDB cdb;
	
	int rc = cdb.Open("clown.db");
	assert(rc == 0);

	rc = cdb.Close();
	assert(rc == 0);

	rc = cdb.Open("random.txt");
	assert(rc == 1);

	rc = cdb.Open("clown.db");
	assert(rc == 0);

	std::string s = "hello_world";
	rc = cdb.Put("test", s);
	assert(rc == 0);

//	int rc = cdb.Open("clown
*/
}

void UnorderedTests() {
	std::cout << "Unordered testS" << std::endl;
	
	Unordered u;
	int rc = u.Open("clown.db2");
	assert(rc == 0);

	rc = u.Close();
	assert(rc == 0);

	rc = u.Open("clown.db2");
	assert(rc == 0);

	std::string s = "hello_world";
	rc = u.Put("test", s);
	assert(rc == 0);	
}

int main() {
	FileHandleTests();
	UnorderedTests();
	//ClownDBTests();
	std::cout << "Tests Passed" << std::endl;
}
