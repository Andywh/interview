#ifndef __ThreadPool_H__
#define __ThreadPool_H__


#include <pthread.h>
#include "queue.h"

struct _ThreadPool;
typedef struct _ThreadPool ThreadPool;

void *thread_routine (void* arg);

ThreadPool* ThreadPoolNew(int numberOfThreads);
// 给线程添加一个任务
// pool 是线程池
// callback 是线程要执行的函数
// arg 是传给要执行的函数的参数
int ThreadPoolAddTask(ThreadPool *pool, TaskCallback *callback, void *arg);

// 删除并释放一个线程池的资源
// pool 是要销毁的线程池
int ThreadPoolFree(ThreadPool *pool);

#endif
