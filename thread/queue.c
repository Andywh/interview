#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct _Task {
    TaskCallback *callback;
    void *arg;
    Task *next;
};
// typedef struct _Task Task;

struct _Queue {
    Task *head;
    Task *tail;
    size_t size;
};
// typedef struct _Queue Queue;

Task*
TaskNew(TaskCallback *callback, void *arg) {
    Task *t = (Task *)malloc(sizeof(Task));
    t->callback = callback;
    t->arg = arg;
    t->next = NULL;
    return t;
}

void
TaskRun(Task *task) {
    (*(task->callback))(task->arg);
}

Queue*
QueueCreate(size_t size) {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

bool
QueueIsEmpty(Queue *queue) {
    return queue->size == 0;
}

Queue*
QueueEnqueue(Queue *queue, Task *task) {
    Queue *q = queue;
    Task *t = task;
    if (QueueIsEmpty(q)) {
        q->head = q->tail = t;
    } else {
        q->tail->next = t;
        q->tail = t->next;
    }
    q->size++;
    return q;
}

Task*
QueueDequeue(Queue *queue) {
    Queue *q = queue;
    if (QueueIsEmpty(q)) {
        return NULL;
    }
    Task *t = q->head;
    if (q->size == 1) {
        q->head = q->tail = NULL;
    } else {
        q->head = t->next;
    }
    q->size--;
    return t;
}

void
QueueRemove(Queue *queue) {
    Task *t = queue->head;
    while (t != NULL) {
        Task *p = t;
        t = t->next;
        free(p);
    }
    free(queue);
    return;
}
