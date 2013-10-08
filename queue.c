#include "queue.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

queue_hdl_t *
queue_create() {
	queue_hdl_t *hdl;
	
	hdl = (queue_hdl_t *)malloc(sizeof(queue_hdl_t));
	if(hdl == NULL) {
#ifdef DEBUG
		info("malloc failed");
#endif
		return NULL;
	}

	hdl->first = NULL;
	hdl->last = NULL;
	hdl->size = 0;
	return hdl;
}

void 
queue_destroy(queue_hdl_t *hdl) {
	free(hdl);
	hdl = NULL;
}

void 
queue_enqueue(queue_hdl_t *hdl, void *data) {
	
	queue_node_t *newnode;
	
	if(hdl == NULL) {
#ifdef DEBUG
		info("Invalid handle");
#endif
		return;
	}

	newnode = (queue_node_t *)malloc(sizeof(queue_node_t));
	if(newnode == NULL) {
#ifdef DEBUG
		info("malloc failed");
#endif
		return;
	}

	hdl->size++;
	newnode->data = data;
	newnode->next = NULL;
	
	/* First node */
	if(hdl->last == NULL) {
		hdl->last = newnode;
		hdl->first = newnode;
		return;
	}

	hdl->last->next = newnode;
	hdl->last = newnode;
	return;
}

void *
queue_dequeue(queue_hdl_t *hdl) {

	queue_node_t *qnodep;
	void *data;

	if(hdl == NULL) {
#ifdef DEBUG
		info("Invalid handle");
#endif
		return NULL;
	}

	qnodep = hdl->first;
	/* No nodes */
	if(qnodep == NULL) {
		return NULL;
	}

	if(qnodep == hdl->last) {
		hdl->last = NULL;
	}

	hdl->first = qnodep->next;
	hdl->size--;

	data = qnodep->data;
	free(qnodep);
	qnodep = NULL;
	return data;
}

int
queue_size(queue_hdl_t *hdl) {
	return hdl->size;
}

/* 
 * Redefine struct tst_data as needed
 */
void
queue_printstring(queue_hdl_t *hdl) {

	queue_node_t *s;
#ifdef TESTQUEUE
	struct tst_data {
		char *tst_data;
		int tst_datalen;
	} *q_data;
#endif
	
	s = hdl->first;
	while(s != NULL) {

#ifdef TESTQUEUE
		q_data = (struct tst_data *)s->data;
		printf("%s(%d) => ", q_data->tst_data, q_data->tst_datalen);
#endif
		s = s->next;
	}
	printf("\n");
}
