Storing length as 4 bytes, then char * inside database.

Values are only stored inside the leaf pages.


First page is metadata page;

Root/Non-Leaf pages are of the form
1 byte to indicate non-leaf. 4 bytes for freespace offset then 4 bytes for page pointer, key, 4 bytes for page pointer.

Leaf Pages are of the form
1 byte to indciate its a leaf. 4 bytes for pointer to prev, 4 bytes for pointer to next. key then value pairs
