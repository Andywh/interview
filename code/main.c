// main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "threadpool.h"


void *
printTask(void *arg) {
    int n = *(int *)arg;
    free(arg);

    printf("task %d \n", n);
    sleep(1);

    return NULL;
}

int
main(int argc, const char *argv[]) {
    const int numberOfTasks = 20;

    ThreadPool *pool = GuaThreadPoolNew(5);

    // 添加 20 个任务
    // 但是线程池只有 5 个线程
    // 每个任务要花费 1 秒
    // 总共要花费 4 秒执行完所有线程
    for(int i = 0; i < numberOfTasks; ++i) {
        int *n = malloc(sizeof(int));
        *n = i;
        ThreadPoolAddTask(pool, printTask, n);
        printf("(add task %d)\n", i);
    }

    // GuaThreadPoolFree 要等所有的线程都退出后才会返回
    // 因为在队列中还没有执行的函数会执行后再退出
    ThreadPoolFree(pool);

    return 0;
}