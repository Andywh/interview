#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

typedef void *(TaskCallback)(void *);

struct _Task;
typedef struct _Task Task;
struct _Task {
    TaskCallback *callback;
    void *arg;
    Task *next;
};

struct _TaskQueue;
typedef struct _TaskQueue TaskQueue;
struct _TaskQueue {
    Task *head;
    Task *tail;
    size_t size;
};

struct _ThreadPool {
    int numberOfThreads;        // 线程池中的线程数量
    pthread_t *threads;         // 线程池中所有线程的 id
    // 补全你需要的其他变量
    pthread_mutex_t m;   // 锁
    pthread_cond_t c;  // cond 量

    // 队列
    TaskQueue *queue;
    bool shutdown;
};
typedef struct _ThreadPool ThreadPool;

TaskQueue *
TaskQueueCreate(size_t size) {
    TaskQueue *q = malloc(sizeof(TaskQueue));
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

bool
TaskQueueIsEmpty(TaskQueue *queue) {
    return queue->size == 0;
}

void
TaskQueueEnqueue(TaskQueue *queue, Task *task) {
    TaskQueue *q = queue;
    if (q->size == 0) {
        q->head = q->tail = task;
    } else {
        q->tail->next = task;
        q->tail = task;
    }

    q->size++;
}

Task*
TaskQueueDequeue(TaskQueue *queue) {
    TaskQueue *q = queue;
    if (TaskQueueIsEmpty(q)) {
        return NULL;
    }
    Task *t = q->head;
    if (q->size == 1) {
        q->tail = q->head = NULL;
    } else {
        q->head = t->next;
    }
    return t;
}

void
TaskQueueRemove(TaskQueue *queue) {
    Task *t = queue->head;
    while (t != NULL) {
        Task *p = t;
        t = t->next;
        free(p);
    }
    free(queue);
    return;
}

int
ThreadPoolFree(ThreadPool *pool) {
    if (pool->shutdown) {
        return -1;
    }

    pthread_cond_broadcast(&(pool->c));

    int i;
    for (i = 0; i < pool->numberOfThreads; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);

    TaskQueueRemove(pool->queue);

    pthread_mutex_destroy(&(pool->m));
    pthread_cond_destroy(&(pool->c));

    free(pool);

    pool = NULL;
    return 0;
}

int
ThreadPoolAddTask(ThreadPool* pool, TaskCallback *callback, void *arg) {
    Task *newTask = (Task*)malloc(sizeof(Task));
    newTask->callback = callback;
    newTask->arg = arg;
    newTask->next = NULL;

    pthread_mutex_lock(&(pool->m));

    TaskQueueEnqueue(pool->queue, newTask);
    pthread_mutex_unlock(&(pool->m));

    pthread_cond_signal(&(pool->c));
    return 0;
}

void *
threadFunc(void *arg) {
    int timeout;
    unsigned pid = pthread_self();
    printf("run in thread %lu\n", pid);
    ThreadPool *pool = (ThreadPool *)arg;

    while(1) {
        timeout = 0;
        // 访问线程池之前需要加锁
        pthread_mutex_lock(&pool->m);
        while(TaskQueueIsEmpty(pool->queue) && !pool->shutdown) {
            pthread_cond_wait(&(pool->c), &(pool->m));
        }

        if(pool->shutdown) {
            pthread_mutex_unlock(&(pool->m));
            pthread_exit(NULL);
        }

        Task *t = TaskQueueDequeue(pool->queue);
        pthread_mutex_unlock(&(pool->m));

        (*(t->callback))(t->arg);
        free(t);
        t = NULL;
    }
    pthread_exit(NULL);
}

// 线程池里面的单个线程的入口函数
void *
_ThreadPoolThreadNew(void *arg) {
    pthread_create((pthread_t *)arg, NULL, threadFunc, NULL);
}

ThreadPool*
ThreadPoolNew(int numberOfThreads) {
    ThreadPool *p = (ThreadPool *)malloc(sizeof(ThreadPool));

    pthread_mutex_init(&(p->m), NULL);
    pthread_cond_init(&(p->c), NULL);

    p->threads = (pthread_t *)malloc(sizeof(pthread_t) * numberOfThreads);

    for(int i = 0; i < numberOfThreads; i++) {
        pthread_t tid;
        // _ThreadPoolThreadNew((void *)tid);
        pthread_create(&tid, NULL, threadFunc, p);
        p->threads[i] = tid;
    }

    return p;
}

void *
printTask(void *arg) {
    int n = *(int *)arg;
    free(arg);

    printf("task %d\n", n);
    sleep(2);

    return NULL;
}

int main() {
    const int numberOfTasks = 20;
    ThreadPool *p = ThreadPoolNew(5);

    // 添加 20 个任务
    // 但是线程池只有 5 个线程
    // 每个任务要花费 1 秒
    // 总共要花费 4 秒执行完所有线程
    for (int i = 0; i < numberOfTasks; ++i) {
        int *n = malloc(sizeof(int));
        *n = i;
        ThreadPoolAddTask(p, printTask, n);

    }

    ThreadPoolFree(p);
    return 0;
}
