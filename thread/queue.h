#ifndef __Queue_H__
#define __Queue_H__

#include <stdbool.h>

// 函数指针
typedef void *(TaskCallback)(void *);

struct _Task;
typedef struct _Task Task;

struct _Queue;
typedef struct _Queue Queue;

Task* TaskNew(TaskCallback *callback, void *arg);

void TaskRun(Task *task);

Queue* QueueCreate(size_t size);

bool QueueIsEmpty(Queue *queue);

Queue* QueueEnqueue(Queue *queue, Task *task);

Task* QueueDequeue(Queue *queue);

void QueueRemove(Queue *queue);

#endif
