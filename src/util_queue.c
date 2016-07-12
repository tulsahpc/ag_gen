#include <stdlib.h>
#include "util_list.h"
#include "util_queue.h"

struct Queue *QueueNew()
{
	struct Queue *queue = malloc(sizeof(struct Queue));
	queue->list = malloc(sizeof(struct List));
	return queue;
}

void QueueEnqueue(struct Queue *q, void *val)
{
	ListPrepend(q->list, val);
}

void *QueueDequeue(struct Queue *q)
{
	return ListPop(q->list);
}

int QueueSize(struct Queue *q)
{
	return ListSize(q->list);
}

void QueueFree(struct Queue *q)
{
	free(q->list);
	free(q);
}
