LRUcache
========

LRU cache for file caching

- COMPILE:

$ make clean
$ make debug

- TEST DATA:

$ bash +x ./testdata.sh

- EXECUTE

$ ./LRUcache


Implementation:

- A double linked list (dlqueue.h) is used to maintain cache. 
- The data in the double linked list points to (filename, filedata)
- A hash table is used for fast access to to the nodes in double linked list
- Hash table is indexed based on the filename
- Hash table nodes contain Key(filename) and Value (pointer to the corresponding node in double linked list)
