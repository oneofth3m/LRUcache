#include "hashtable.h"
#include "debug.h"

#include <stdlib.h>
#include <stdio.h>

hash_hdl_t *
hash_create(int M, int (*comparefn)(void *, void *), int (*hashfn)(hash_hdl_t *, void *)) {
	
	hash_hdl_t *hdl;
	int i;
	
	hdl = (hash_hdl_t *)malloc(sizeof(hash_hdl_t));
	if(hdl == NULL) {
#ifdef DEBUG
		info("Invalid handle\n");
#endif
		return NULL;
	}

	hdl->size = 0;
	hdl->comparefn = comparefn;
	hdl->hashfn = hashfn;

	hdl->array = (hash_node_t **)malloc(sizeof(hash_node_t *)*M);
	if(hdl->array == NULL) {
#ifdef DEBUG
		info("Hash array creation failed\n");
#endif
		free(hdl);
		hdl = NULL;
		return NULL;
	}

	hdl->M = M;
	for(i = 0; i < M; i++) {
		hdl->array[i] = NULL;
	}

	return hdl;
}

void
hash_destroy(hash_hdl_t *hdl) {
	int i;
	for(i = 0; i < hdl->M; i++) {
		hash_node_t *temp1, *temp2;
		temp1 = hdl->array[i];
		while(temp1 != NULL) {
			temp2 = temp1;
			temp1 = temp1->next;
			free(temp2);
			temp2 = NULL;
		}
	}

	free(hdl->array);
	hdl->array = NULL;
	free(hdl);
	hdl = NULL;
	return;
}

void
hash_put(hash_hdl_t *hdl, void *key, void *data) {
	int hash;

	if(hdl == NULL || key == NULL || data == NULL) {
#ifdef DEBUG
		info("Invalid handle || key || data");
#endif
		return;
	}

	hash = hdl->hashfn(hdl, key);
	if(hash < 0) {
#ifdef DEBUG
		log("Invalid hash for key : %s(%d)", (char *)key, hash);
#endif
		return;
	}

	/* Resize code */

	/* Deal with duplicate keys later */
	hash_node_t *newnode;
	newnode = (hash_node_t *)malloc(sizeof(hash_node_t));
	if(newnode == NULL) {
#ifdef DEBUG
		info("malloc failed");
#endif
		return;
	}
	newnode->next = NULL;
	newnode->key = key;
	newnode->data = data;

	newnode->next = hdl->array[hash];
	hdl->array[hash] = newnode;

	hdl->size++;

	return;
}

void *
hash_get(hash_hdl_t *hdl, void *key) {

	int hash;

	if(hdl == NULL || key == NULL) {
#ifdef DEBUG
		info("Invalid handle || key");
#endif
		return NULL;
	}

	hash = hdl->hashfn(hdl, key);
	if(hash < 0) {
#ifdef DEBUG
		info("Invalid hash of key");
#endif
		return NULL;
	}

	hash_node_t *bucket;
	bucket = hdl->array[hash];

	while(bucket != NULL) {
		int cmp;
		cmp = hdl->comparefn(key, bucket->key);

		if(cmp == 0) {
			return bucket->data;
		}

		bucket = bucket->next;
	}

	return NULL;
}

void
hash_delete(hash_hdl_t *hdl, void *key, void **retkey, void **retdata) {

	int hash;

	if(hdl == NULL || key == NULL) {
#ifdef DEBUG
		info("Invalid handle or key");
#endif
		return;
	}

	hash = hdl->hashfn(hdl, key);
	if(hash < 0) {
#ifdef DEBUG
		info("Invalid hash for key");
#endif
		return;
	}

	hash_node_t *bucket;
	bucket = hdl->array[hash];

	if(bucket == NULL) {
		return;
	}

	if(hdl->comparefn(key, bucket->key) == 0) {
		hdl->array[hash] = bucket->next;
		hdl->size--;
		*retkey = bucket->key;
		*retdata = bucket->data;
		/* Resize code */
		free(bucket);
		bucket = NULL;
		return;
	}	

	while(bucket->next != NULL) {
		int cmp;
		cmp = hdl->comparefn(key, bucket->next->key);

		if(cmp == 0) {
			hash_node_t *temp;
			temp = bucket->next;
			bucket->next = bucket->next->next;
			hdl->size--;
			/* Resize code */
			*retkey = temp->key;
			*retdata = temp->data;
			free(temp);
			temp = NULL;
			return;
		}
		
		bucket = bucket->next;
	}

	return;
}
			
int
hash_size(hash_hdl_t *hdl) {
	return hdl->size;
}

int
hash_buckets(hash_hdl_t *hdl) {
	return hdl->M;
}

void
hash_printstring(hash_hdl_t *hdl) {
	int i;

	for(i = 0; i < hdl->M; i++) {
		hash_node_t *bucket;
		bucket = hdl->array[i];
		
		printf("hash[%d] => ", i);
		while(bucket != NULL) {
#ifdef TESTHASH
			struct tst_key {
				char *tst_key;
				int tst_keylen;
			} *key;
			key = bucket->key;
			printf("%s(%d) =>", key->tst_key, key->tst_keylen);
#elif LRUCACHE
			struct LRU_hash_key {
				char *key;
				int keylen;
			} *key;
			key = bucket->key;
			printf("%s(%d) =>", key->key, key->keylen);
#endif
			bucket = bucket->next;
		}
		printf("\n");
	}
}
