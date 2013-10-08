#include "LRUcacheread.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

LRU_queue_data_t *
LRU_read(int fileno, dlq_hdl_t *q_hdl, hash_hdl_t *h_hdl) {
	int hit = 0;
	LRU_queue_data_t *q_data = NULL;
	LRU_queue_data_t *dqd_data = NULL;
	char *data;
	int datalen = READLEN;
	char *filename = NULL;
	int filelen = 19;
	if(fileno > 9) {
		filelen++;
	}

	if(q_hdl == NULL) {
#ifdef DEBUG
		info("Queue handle is NULL");
#endif
		return NULL;
	}

	if(h_hdl == NULL) {
#ifdef DEBUG
		info("Queue handle is NULL");
#endif
		return NULL;
	}

	filename = (char *)malloc(sizeof(char)*filelen);
	snprintf(filename, filelen, "testdata/file%d.txt", fileno);

	LRU_hash_key_t *hkey= NULL;
	hkey = (LRU_hash_key_t *)malloc(sizeof(LRU_hash_key_t));
	hkey->hash_key_filename = filename;
	hkey->hash_key_filename_len = filelen;

	dlq_node_t *hashed = NULL;
	hashed = (dlq_node_t *)hash_get(h_hdl, (void *)hkey);
	
	if(hashed != NULL) {
		/* HIT */
		printf("---------------CACHE HIT---------------\n");

		free(filename);
		filename = NULL;
		free(hkey);
		hkey = NULL;

		dlq_bringfront(q_hdl, hashed);
	
		q_data = (LRU_queue_data_t *)hashed->data;
		return q_data;
	}

	/* MISS */
	printf("---------------CACHE MISS---------------\n");
	int fd;
	fd = open(filename, O_RDONLY); 	
	if(fd == -1) {
#ifdef DEBUG
		printf("File not found\n");
#endif
		return NULL;
	}
	
	size_t rsize;
	data = (char *)malloc(sizeof(char)*datalen);
	rsize = read(fd, data, datalen);
	if(rsize < 0) {
#ifdef DEBUG
		printf("File read failed\n");
#endif
		free(data);
		data = NULL;
		free(filename);
		filename = NULL;
		return NULL;
	}

	q_data = (LRU_queue_data_t *) malloc(sizeof(LRU_queue_data_t));
	q_data->filedata = data;
	q_data->filedatalen = rsize;
	q_data->filekey = hkey;

	dlq_node_t *new_qnode;
	new_qnode = dlq_enqueue(q_hdl, (void *)q_data, (void **)&dqd_data);
	
	/* Insert new_qnode to haseh*/
	hash_put(h_hdl, (void *)hkey, (void *)new_qnode);	

	if(dqd_data != NULL) {
		/* Remove old qnode from hash */
		void *retkey;
		void *retdata;
		hash_delete(h_hdl, (void *)dqd_data->filekey, &retkey, 
			&retdata);
		free(dqd_data->filekey->hash_key_filename);
		dqd_data->filekey->hash_key_filename = NULL;
		free(dqd_data->filekey);
		dqd_data->filekey = NULL;
		free(dqd_data->filedata);
		dqd_data->filedata = NULL;
		free(dqd_data);
		dqd_data = NULL;
	}

	return q_data;
}

void display_files() {
	int i;

	printf(" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
	for(i = 0; i < NFILES; i++) {
		printf("%d -> testdata/file%d.txt\n", i, i);
	}
	printf(" XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
}

void
read_file(dlq_hdl_t *q_hdl, hash_hdl_t *h_hdl) {
	
	int i;
	int file_ch;
	clock_t cstart, cend;
	LRU_queue_data_t *q_data = NULL;

	display_files();

	printf("Select file :");
	scanf("%d", &file_ch);

	if(file_ch < 0 && file_ch > NFILES) {
		printf("Invalid file choice\n");
		return;
	}

	cstart = clock();	
	q_data = LRU_read(file_ch, q_hdl, h_hdl);
	cend = clock();

#ifdef DEBUG
	// Print first few characters

	if(q_data == NULL) {
		printf("No data from file.\n");
		return;
	}

	printf("Filename : %s(%d)\n", q_data->filekey->hash_key_filename,
		q_data->filedatalen);
	for(i = 0; i < 5 && i < q_data->filedatalen; i++) {
		printf("%c", q_data->filedata[i]);
	}
	printf("\n");
#endif

	printf("Read time : %.3f cpu sec\n",
		 ((double)cend -(double)cstart)*1.0e-6);
}

void
print_hashtable(hash_hdl_t *hdl) {
	hash_printstring(hdl);
}

void
print_LRUcache(dlq_hdl_t *hdl) {
	dlq_printstring(hdl);
}

static int 
tst_hash_compare(void *key1, void *key2) {
	LRU_hash_key_t *k1, *k2;
	
	k1 = (LRU_hash_key_t *)key1;	
	k2 = (LRU_hash_key_t *)key2;	

	if(k1->hash_key_filename_len != k2->hash_key_filename_len) {
		return -1;
	}

	return strncmp(k1->hash_key_filename, k2->hash_key_filename, 
		k1->hash_key_filename_len);
}

static int
tst_hash(hash_hdl_t *hdl, void *key) {
	LRU_hash_key_t *k1;
	int i = 0;
	int hash = 17;
	
	k1 = (LRU_hash_key_t *)key;
	
	for(i = 0; i < k1->hash_key_filename_len; i++) {
		hash = hash*31 + (int)k1->hash_key_filename[i];
	}
	
	return (hash & 0x7ffffff) % (hdl->M);
}

int main() {
	int ch;
	int capacity;
	LRU_queue_data_t *free_q_data;	

	dlq_hdl_t *q_hdl;
	printf("Enter LRU capacity:");
	scanf("%d", &capacity);
	q_hdl = dlq_create(capacity);
	if(q_hdl == NULL) {
#ifdef DEBUG
		info("Failed to create Queue");
#endif
		return 1;
	}

	hash_hdl_t *h_hdl;
	printf("Enter hash buckets:");
	scanf("%d", &capacity);
	h_hdl = hash_create(capacity, tst_hash_compare, tst_hash);
	if(h_hdl == NULL) {
#ifdef DEBUG
		info("Failed to create Queue");
#endif
		return 1;
	}
	

	do {
		printf("1 - Read file.\n");
		printf("2 - Print hashtable \n");
		printf("3 - Print LRU Cache \n");
		scanf("%d", &ch);

		switch(ch) {
		case 1:	
			read_file(q_hdl, h_hdl);
			break;
		case 2:
			print_hashtable(h_hdl);
			break;
		case 3:
			print_LRUcache(q_hdl);
			break;
		default:
			free_q_data = NULL;
			while((free_q_data = 
			(LRU_queue_data_t *)dlq_dequeue(q_hdl)) != NULL) {
				free(free_q_data->filedata);
				free_q_data->filedata = NULL;
				free(free_q_data->filekey->hash_key_filename);
				free_q_data->filekey->hash_key_filename = NULL;
				free(free_q_data->filekey);
				free_q_data->filekey = NULL;
				free(free_q_data);
				free_q_data = NULL;
			}
			dlq_destroy(q_hdl);
			q_hdl = NULL;
			hash_destroy(h_hdl);
			h_hdl = NULL;
			return 0;
		}
	}while (ch > 0 && ch < 4);
	return 1;
}
