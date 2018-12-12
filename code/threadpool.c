#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "threadpool.h"


struct _Task;
typedef struct _Task Task;


struct _TaskQueue;
typedef struct _TaskQueue TaskQueue;

// TaskQueue 的定义和接口你自己来根据需要补全
struct _Task {
    TaskCallback *callback;
    void *arg;                      //回调函数参数
    Task *next;
};


struct _ThreadPool {
    int numberOfThreads;        // 线程池中的线程数量  
    pthread_t *threads;         // 线程池中所有线程的 id
    // 补全你需要的其他变量
    pthread_mutex_t m;   // 锁
    pthread_cond_t con;  // cond 量
};


// 线程池里面的单个线程的入口函数
void *
_ThreadPoolThreadNew(void *arg) {
	pthread_t tid;
	pthread_create(&tid, NULL, threadFunc, NULL);
	return tid;
}


ThreadPool* 
ThreadPoolNew(int numberOfThreads) {
	ThreadPool *p = (ThreadPool *)malloc(sizeof(ThreadPool));

	pthread_mutex_init(&(pool->m), NULL);
	pthread_cond_init(&(pool->c), NULL);

	p->threads = (pthread_t *)malloc(sizeof(pthread_t))
	for(int i = 0; i < numberOfThreads; i++) {
		p->threads[i] = _ThreadPoolThreadNew()
	}

	return p;
}


int
ThreadPoolAddTask(ThreadPool* pool, TaskCallback * callback, void *arg) {


}

void *
_ThreadPoolThreadNew(void* arg) {
}

int
ThreadPoolFree(ThreadPool *pool) {
}