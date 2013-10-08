#ifndef DLQUEUE_H
#define DLQUEUE_H

struct dlq_node {
	struct dlq_node *next;
	struct dlq_node *prev;
	void *data;
};

typedef struct dlq_node dlq_node_t;

struct dlq_hdl {
	dlq_node_t *first;
	dlq_node_t *last;
	int capacity;
	int size;
};

typedef struct dlq_hdl dlq_hdl_t;

dlq_hdl_t *dlq_create(int capacity);

void dlq_destroy(dlq_hdl_t *);

/* 
 * Return enqueued node pointer 
 * if, this causes dequeue, dqd_data points to dequeued data
 */
dlq_node_t *dlq_enqueue(dlq_hdl_t *, void *data, void **dqd_data);

/* Return dequeued data pointer */
void *dlq_dequeue(dlq_hdl_t *);

void dlq_bringfront(dlq_hdl_t *, dlq_node_t *);

int dlq_capacity(dlq_hdl_t *);

int dlq_size(dlq_hdl_t *);

void dlq_printstring(dlq_hdl_t *);

#endif
