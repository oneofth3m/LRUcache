#include "hashtable.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

struct tst_data {
	char *tst_data;
	int tst_datalen;
};

struct tst_key {
	char *tst_key;
	int tst_keylen;
};

typedef struct tst_data tst_data_t;
typedef struct tst_key tst_key_t;

static void
tst_hash_put(hash_hdl_t *hdl) {
	int keylen, datalen;
	char *key, *data;

	tst_key_t *hash_key;	
	tst_data_t *hash_data;

	printf("Enter keylength :");
	scanf("%d", &keylen);
	key = (char *)malloc(sizeof(char)*keylen);
	printf("Enter key:");
	scanf("%s", key);

	printf("Enter datalength :");
	scanf("%d", &datalen);
	data = (char *) malloc(sizeof(char)*datalen);
	printf("Enter data:");
	scanf("%s", data);
	
	hash_key = (tst_key_t *)malloc(sizeof(tst_key_t));
	hash_key->tst_key = key;
	hash_key->tst_keylen = keylen;
		
	hash_data = (tst_data_t *)malloc(sizeof(tst_data_t));
	hash_data->tst_data = data;
	hash_data->tst_datalen = datalen;

	hash_put(hdl, (void *)hash_key, (void *)hash_data);
}

static void
tst_hash_get(hash_hdl_t *hdl) {

	int keylen;
	char *key;

	tst_key_t *hash_key;	
	tst_data_t *hash_data;

	printf("Enter keylength :");
	scanf("%d", &keylen);
	key = (char *)malloc(sizeof(char)*keylen);
	printf("Enter key:");
	scanf("%s", key);

	hash_key = (tst_key_t *)malloc(sizeof(tst_key_t));
	hash_key->tst_key = key;
	hash_key->tst_keylen = keylen;
		
	hash_data = (tst_data_t *)hash_get(hdl, (void *)hash_key);

	if(hash_data != NULL) {
		printf("Key (%s), Data (%s)\n", hash_key->tst_key,
			 hash_data->tst_data);
	}

	free(key);
	key = NULL;	
	free(hash_key);
	hash_key = NULL;
}

static void
tst_hash_delete(hash_hdl_t *hdl) {

	int keylen;
	char *key;

	tst_key_t *hash_key;	

	tst_key_t *retkey = NULL;	
	tst_data_t *retdata = NULL;	

	printf("Enter keylength :");
	scanf("%d", &keylen);
	key = (char *)malloc(sizeof(char)*keylen);
	printf("Enter key:");
	scanf("%s", key);

	hash_key = (tst_key_t *)malloc(sizeof(tst_key_t));
	hash_key->tst_key = key;
	hash_key->tst_keylen = keylen;

	hash_delete(hdl, (void *)hash_key, (void **)&retkey, 
			(void **)&retdata);

	free(hash_key);
	hash_key = NULL;
	free(key);
	key = NULL;

	if(retkey != NULL) {
		free(retkey->tst_key);
		retkey->tst_key = NULL;
		free(retkey);
		retkey = NULL;
	}	

	if(retdata != NULL) {
		free(retdata->tst_data);
		retdata->tst_data = NULL;
		free(retdata);
		retdata = NULL;
	}	
		
}

static void
tst_hash_print(hash_hdl_t *hdl) {
	hash_printstring(hdl);
}

static void
tst_hash_printsize(hash_hdl_t *hdl) {
	printf("Size of hash is %d\n", hash_size(hdl));
}

static int 
tst_hash_compare(void *key1, void *key2) {
	tst_key_t *k1, *k2;
	
	k1 = (tst_key_t *)key1;	
	k2 = (tst_key_t *)key2;	

	if(k1->tst_keylen != k2->tst_keylen) {
		return -1;
	}

	return strncmp(k1->tst_key, k2->tst_key, k1->tst_keylen);
}

static int
tst_hash(hash_hdl_t *hdl, void *key) {
	tst_key_t *k1;
	int i = 0;
	int hash = 17;
	
	k1 = (tst_key_t *)key;
	
	for(i = 0; i < k1->tst_keylen; i++) {
		hash = hash*31 + (int)k1->tst_key[i];
	}
	
	return (hash) % (hdl->M);
}

int 
main() {
	int ch;
	int capacity;
	hash_hdl_t *h_hdl;

	printf("Enter no. of buckets:");
	scanf("%d", &capacity);
	h_hdl = hash_create(capacity, tst_hash_compare, tst_hash);
	if(h_hdl == NULL) {
#ifdef DEBUG
		info("Hash creation failed.\n");
#endif
		return 1;
	}
	
	do {
		printf("1 - Add data.\n");
		printf("2 - Get data.\n");
		printf("3 - Del data.\n");
		printf("4 - Print Hash.\n");	
		printf("5 - Print Hash size.\n");	
		scanf("%d", &ch);

		switch(ch) {
		case 1:
			tst_hash_put(h_hdl);
			break;
		case 2:
			tst_hash_get(h_hdl);
			break;
		case 3:
			tst_hash_delete(h_hdl);
			break;
		case 4:
			tst_hash_print(h_hdl);
			break;
		case 5:
			tst_hash_printsize(h_hdl);
			break;
		default:
			hash_destroy(h_hdl);
			h_hdl = NULL;
			return 0;
			break;
		}
	} while (ch > 0 && ch < 6);

	return 1;
}

	
