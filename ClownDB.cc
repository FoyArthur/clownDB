#include "ClownDB.h"

ClownDB::ClownDB() {
	fh = new FileHandle();
}

ClownDB::~ClownDB() {
	delete fh;
}
