#ifndef C_UTIL_QUEUE_H
#define C_UTIL_QUEUE_H

struct Queue {
	struct List *list;
};

struct Queue *QueueNew(void);
void QueueEnqueue(struct Queue *, void *val);
void *QueueDequeue(struct Queue *);
int QueueSize(struct Queue *);
void QueueFree(struct Queue *);

#endif //C_UTIL_QUEUE_H
