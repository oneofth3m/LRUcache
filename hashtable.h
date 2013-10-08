#ifndef HASHTABLE_H
#define HASHTABLE_H

struct hash_node {
	struct hash_node *next;
	void *key;
	void *data;
};

typedef struct hash_node hash_node_t;

struct hash_hdl {
	int M;	// No. of buckets;
	int size; // No. of elements;
	hash_node_t **array;
	int (*comparefn)(void *key1, void *key2);
	int (*hashfn)(struct hash_hdl *, void *key);
};

typedef struct hash_hdl hash_hdl_t;

hash_hdl_t *hash_create(int , int (*)(void *, void *), int (*)(hash_hdl_t *, void *));

void hash_destroy(hash_hdl_t *);

void hash_put(hash_hdl_t *, void *key, void *data);

void *hash_get(hash_hdl_t *, void *key);

void hash_delete(hash_hdl_t *, void *key, void **retkey, void **retdata);

int hash_size(hash_hdl_t *);

int hash_buckets(hash_hdl_t *);

void hash_printstring(hash_hdl_t *);

#endif
