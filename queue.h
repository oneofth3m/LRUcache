#ifndef QUEUE_H
#define QUEUE_H

struct queue_node {
	struct queue_node *next;
	void *data;
};

typedef struct queue_node queue_node_t;

struct queue_hdl {
	struct queue_node *first;
	struct queue_node *last;
	int size;
};

typedef struct queue_hdl queue_hdl_t;

queue_hdl_t *queue_create();

void queue_destroy(queue_hdl_t *);

void queue_enqueue(queue_hdl_t *, void *data);

void *queue_dequeue(queue_hdl_t *);

int queue_size(queue_hdl_t *);

void queue_printstring(queue_hdl_t *);

#endif
