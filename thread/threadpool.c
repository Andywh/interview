#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "threadpool.h"
#include "queue.h"

struct _ThreadPool {
    pthread_t *threads;
    int numberOfThreads;
    // 补全你需要的其他变量
    pthread_mutex_t m;
    pthread_cond_t c;
    //队列
    Queue *queue;
    bool shutdown;
};

void *
thread_routine (void* arg) {
    unsigned tid = (unsigned)pthread_self();
    printf("run in thread %u\n", tid);
    ThreadPool *p = (ThreadPool *)arg;

    while(1) {
        pthread_mutex_lock(&(p->m));

        while(QueueIsEmpty(p->queue) && !p->shutdown) {
            pthread_cond_wait(&(p->c), &(p->m));
        }

        if(p->shutdown) {
            pthread_mutex_unlock(&(p->m));
            pthread_exit(NULL);
        }

        Task *t = QueueDequeue(p->queue);
        pthread_mutex_unlock(&(p->m));

        TaskRun(t);

        free(t);
        t = NULL;
    }
    pthread_exit(0);
}

ThreadPool*
ThreadPoolNew(int numberOfThreads) {
    ThreadPool *p = (ThreadPool *)malloc(sizeof(ThreadPool));

    pthread_mutex_init(&p->m, NULL);
    pthread_cond_init(&p->c, NULL);

    p->queue = QueueCreate(0);
    p->numberOfThreads = numberOfThreads;
    p->shutdown = 0;

    p->threads = (pthread_t *)malloc(sizeof(pthread_t) * numberOfThreads);

    for(int i = 0; i < numberOfThreads; i++) {
        pthread_create(&(p->threads[i]), NULL, thread_routine, p);
    }

    return p;
}

int
ThreadPoolAddTask(ThreadPool *pool, TaskCallback *callback, void *arg) {
    ThreadPool *p = pool;

    Task *t = TaskNew(callback, arg);

    pthread_mutex_lock(&(p->m));
    QueueEnqueue(p->queue, t);
    pthread_mutex_unlock(&(p->m));
    pthread_cond_signal(&(p->c));

    return 0;
}

int
ThreadPoolFree(ThreadPool *pool) {
    ThreadPool *p = pool;
    if (p->shutdown) {
        return -1;
    }


    for (int i = 0; i < p->numberOfThreads; i++) {
        pthread_join(p->threads[i], NULL);
    }
    p->shutdown = true;
    pthread_cond_broadcast(&(p->c));

    free(p->threads);

    QueueRemove(p->queue);

    pthread_mutex_destroy(&(p->m));
    pthread_cond_destroy(&(p->c));
    free(p);
    p = NULL;
    return 0;
}
