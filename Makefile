CC=gcc

all:	testdebug testqueue testhash testdlqueue LRUcache

#debug:	CC += -DDEBUG -DTESTHASH -DTESTDLQUEUE -g 
debug:	CC += -DDEBUG -DTESTHASH -DLRUCACHE -g 
debug:	all

testdebug:	testdebug.o
	$(CC) -o testdebug testdebug.o

testdebug.o:	testdebug.c debug.h
	$(CC) -o testdebug.o -c testdebug.c	

testqueue:	testqueue.o queue.o
	$(CC) -o testqueue testqueue.o queue.o

testqueue.o:	testqueue.c queue.h debug.h
	$(CC) -o testqueue.o -c testqueue.c

queue.o:	queue.c queue.h debug.h
	$(CC) -o queue.o -c queue.c

testhash:	testhash.o hashtable.o
	$(CC) -o testhash testhash.o hashtable.o

testhash.o:	testhash.c debug.h hashtable.h
	$(CC) -o testhash.o -c testhash.c

hashtable.o:	hashtable.c debug.h hashtable.h
	$(CC) -o hashtable.o -c hashtable.c

testdlqueue:	testdlqueue.o dlqueue.o
	$(CC) -o testdlqueue testdlqueue.o dlqueue.o

testdlqueue.o:	testqueue.c dlqueue.h debug.h
	$(CC) -o testdlqueue.o -c testdlqueue.c

dlqueue.o:	dlqueue.c dlqueue.h debug.h
	$(CC) -o dlqueue.o -c dlqueue.c

LRUcache:	LRUcacheread.o dlqueue.o hashtable.o
	$(CC) -o LRUcache LRUcacheread.o dlqueue.o hashtable.o

LRUcacheread.o:	LRUcacheread.c dlqueue.h debug.h hashtable.h
	$(CC) -o LRUcacheread.o -c LRUcacheread.c

clean:	
	rm testdebug testdebug.o testqueue testqueue.o queue.o testhash testhash.o hashtable.o testdlqueue testdlqueue.o dlqueue.o LRUcacheread.o LRUcache
