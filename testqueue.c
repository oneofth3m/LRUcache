#include "queue.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

struct tst_data {
	char *tst_data;
	int tst_datalen;
};

typedef struct tst_data tst_data_t;

static void
tst_enqueue(queue_hdl_t *hdl) {

	tst_data_t *q_data;

	int in_datalen;
	char *in_data;

	printf("Enter datalength:");
	scanf("%d", &in_datalen);
	
	in_data = (char *) malloc(sizeof(char)*in_datalen);
	printf("Enter data:");
	scanf("%s", in_data);

	q_data = (tst_data_t *) malloc(sizeof(tst_data_t));
	q_data->tst_datalen = in_datalen;
	q_data->tst_data = in_data;

	queue_enqueue(hdl, (void *) q_data);
	
}

static void
tst_dequeue(queue_hdl_t *hdl) {
	tst_data_t *q_data;
	
	q_data = (tst_data_t *)queue_dequeue(hdl);

	if(q_data == NULL) {
		printf("Queue is empty\n");
		return;
	}

	printf("Dequeue data => %s\n", q_data->tst_data);
	free(q_data->tst_data);
	q_data->tst_data = NULL;
	free(q_data);
	q_data = NULL;
}

static void
tst_print(queue_hdl_t *hdl) {
	queue_printstring(hdl);
}

static void
tst_size(queue_hdl_t *hdl) {
	printf("Size of queue is %d\n", queue_size(hdl));
}

int 
main() {
	int ch;
	queue_hdl_t *q_hdl;

	q_hdl = queue_create();
	if(q_hdl == NULL) {
#ifdef DEBUG
		info("Queue creation failed.\n");
#endif
		return 1;
	}
	
	do {
		printf("1 - Enqueue data.\n");
		printf("2 - Dequeue data.\n");
		printf("3 - Print queue.\n");	
		printf("4 - Print queue size.\n");	
		scanf("%d", &ch);

		switch(ch) {
		case 1:
			tst_enqueue(q_hdl);
			break;
		case 2:
			tst_dequeue(q_hdl);
			break;
		case 3:
			tst_print(q_hdl);
			break;
		case 4:
			tst_size(q_hdl);
			break;
		default:
			queue_destroy(q_hdl);
			q_hdl = NULL;
			return 0;
			break;
		}
	} while (ch > 0 && ch < 5);

	return 1;
}

	
