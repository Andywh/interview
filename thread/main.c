#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "threadpool.h"

void *
printTask(void *arg) {
    int n = *(int *)arg;
    free(arg);
    // arg = NULL;

    printf("execute task1 %d \n", n);
    sleep(2);
    printf("leaving task %d \n", n);

    return NULL;
}

int
main(int argc, const char *argv[]) {
    const int numberOfTasks = 20;

    ThreadPool *pool = ThreadPoolNew(25);
    sleep(6);
    for (int i = 0; i < numberOfTasks; i++) {
        int *n = malloc(sizeof(int));
        *n = i;
        ThreadPoolAddTask(pool, printTask, n);
        printf("(add task %d)\n", i);
    }
    sleep(14);
    ThreadPoolFree(pool);

    return 0;
}
