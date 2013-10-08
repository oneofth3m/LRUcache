#include "dlqueue.h"
#include "debug.h"

#include <stdlib.h>
#include <stdio.h>

dlq_hdl_t *
dlq_create(int capacity) {
	dlq_hdl_t *hdl;

	hdl = (dlq_hdl_t *)malloc(sizeof(dlq_hdl_t));
	if(hdl == NULL) {
#ifdef DEBUG
		info("malloc failed");
#endif
		return NULL;
	}

	hdl->first = NULL;
	hdl->last = NULL;
	hdl->size = 0;
	hdl->capacity = capacity;
	
	return hdl;
}

void
dlq_destroy(dlq_hdl_t *hdl) {
	free(hdl);
	hdl = NULL; /* Rememeber to NULL in client also */
}

void *
dlq_dequeue(dlq_hdl_t *hdl) {

	void *retdata;
	
	if(hdl == NULL) {
#ifdef DEBUG
		info("Invalid handle");
#endif
		return NULL;
	}

	if(hdl->last == NULL) {
#ifdef DEBUG
		info("Empty DLQueue");
#endif
		return NULL;
	}

	retdata = hdl->last->data;	
	hdl->size--;

	if(hdl->first == hdl->last) {
		free(hdl->last);
		hdl->last = NULL;
		hdl->first = NULL;
		return retdata;
	}

	dlq_node_t *temp = hdl->last;
	hdl->last = hdl->last->prev;
	hdl->last->next = NULL;
	free(temp);
	temp = NULL;
	return retdata;
}

dlq_node_t *dlq_enqueue(dlq_hdl_t *hdl, void *data, void **dqd_data) {
	
	if(hdl == NULL) {
#ifdef DEBUG
		info("Invalid handle");
#endif
		return NULL;
	}

	if(data == NULL || dqd_data == NULL) {
#ifdef DEBUG
		info("NULL data args");
#endif
		return NULL;
	}

	*dqd_data = NULL;
	if(dlq_capacity(hdl) == dlq_size(hdl)) {
		*dqd_data = dlq_dequeue(hdl);
	}

	dlq_node_t *newnode;
	newnode = (dlq_node_t *)malloc(sizeof(dlq_node_t));
	if(newnode == NULL) {
#ifdef DEBUG
		info("Malloc failed");
#endif
		return NULL;
	}

	newnode->next = NULL;
	newnode->prev = NULL;
	newnode->data = data;
	hdl->size++;

	if(hdl->first == NULL) {
		hdl->first = newnode;
		hdl->last = newnode;
		return newnode;
	}

	newnode->next = hdl->first;
	hdl->first->prev = newnode;
	hdl->first = newnode;

	return newnode;
}
		
void dlq_bringfront(dlq_hdl_t *hdl, dlq_node_t *node) {
	if(hdl == NULL || node == NULL) {
#ifdef DEBUG
		info("Invalid handle || node");
#endif
		return;
	}

	if(hdl->first == NULL || hdl->first == node) {
		return;
	}

	if(hdl->last == node) {
		hdl->last = hdl->last->prev;
	}

	if(node->prev != NULL) {
		node->prev->next = node->next;
	}

	if(node->next != NULL) {
		node->next->prev = node->prev;
	}

	node->prev = NULL;
	node->next = hdl->first;
	hdl->first->prev = node;
	hdl->first = node;
}

int
dlq_capacity(dlq_hdl_t *hdl) {
	return hdl->capacity;
}

int
dlq_size(dlq_hdl_t *hdl) {
	return hdl->size;
}

void
dlq_printstring(dlq_hdl_t *hdl) {
	dlq_node_t *s;
	s = hdl->first;

	while(s != NULL) {
#ifdef TESTDLQUEUE
		struct tst_data {
			char *data;
			int datalen;
		} *tst_datap;
		
		tst_datap = (struct tst_data *)s->data;

		printf("%s => ", tst_datap->data);
#elif	LRUCACHE
		struct LRU_hash_key {
			char *key;
			int keylen;
		};

		struct LRU_queue_data {
			char *filedata;
			int filedatalen;
			struct LRU_hash_key *hkey;
		};

		struct LRU_queue_data *q_data;
		q_data = (struct LRU_queue_data *)s->data;

		printf("%s => ", q_data->hkey->key);
#endif
		s = s->next;
	}

	printf("\n");
	return;
}
