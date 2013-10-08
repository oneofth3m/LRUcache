#ifndef LRUCAHCEREAD_H
#define LRUCACHEREAD_H

#include "dlqueue.h"
#include "hashtable.h"

#define NFILES 20
#define READLEN 1024*1024*1024

struct LRU_hash_key {
	char *hash_key_filename;
	int hash_key_filename_len;
};

typedef struct LRU_hash_key LRU_hash_key_t;

struct LRU_queue_data {
	char *filedata;
	int filedatalen;
	LRU_hash_key_t *filekey;
};

typedef struct LRU_queue_data LRU_queue_data_t;

	
#endif

