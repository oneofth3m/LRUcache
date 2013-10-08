#include "dlqueue.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

struct tst_data {
	char *tst_data;
	int tst_datalen;
};

typedef struct tst_data tst_data_t;

static void
tst_enqueue(dlq_hdl_t *hdl) {

	tst_data_t *q_data;
	tst_data_t *ret_data = NULL;

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

	dlq_enqueue(hdl, (void *) q_data, (void **)&ret_data);

	if(ret_data != NULL) {
		free(ret_data->tst_data);
		ret_data->tst_data = NULL;
		free(ret_data);	
	}
	
}

static void
tst_dequeue(dlq_hdl_t *hdl) {
	tst_data_t *q_data;
	
	q_data = (tst_data_t *)dlq_dequeue(hdl);

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

void tst_bringfront(dlq_hdl_t *hdl) {
	dlq_node_t *nodea[100];
	int ch, j;

	dlq_node_t *temp = hdl->first;
	
	int nnodes = 0;
	while(temp != NULL) {
		nodea[nnodes] = temp;
		temp = temp->next;
		nnodes++;
	}

	for(j = 0; j < nnodes; j++) {
		printf("%d = (%p)\n", j, nodea[j]);
	}
	dlq_printstring(hdl);
	printf("Select node to print:");
	scanf("%d", &ch);
	if(ch < 0 || ch >= nnodes) {
		return;
	}
	dlq_bringfront(hdl, nodea[ch]);
	dlq_printstring(hdl);
	return;
}

static void
tst_print(dlq_hdl_t *hdl) {
	dlq_printstring(hdl);
}

static void
tst_size(dlq_hdl_t *hdl) {
	printf("Size of queue is %d\n", dlq_size(hdl));
}

int 
main() {
	int ch;
	dlq_hdl_t *q_hdl;

	q_hdl = dlq_create(5);
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
		printf("5 - Bringfront.\n");	
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
		case 5:
			tst_bringfront(q_hdl);
			break;
		default:
			dlq_destroy(q_hdl);
			q_hdl = NULL;
			return 0;
			break;
		}
	} while (ch > 0 && ch < 6);

	return 1;
}

	
