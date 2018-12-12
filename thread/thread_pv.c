#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// sem_t *s;
// sem_wait()
// sem_post()
// 创建有名信号量
// sem_open(&c, O_CREAT, 0644, 1);

static unsigned balance = 0;
sem_t *s;

void *
deposit(void *args) {
    sem_wait(s);
    balance += 10;
    sem_post(s);
    return NULL;
}

void *
withdrawal(void *args) {
    sem_wait(s);
    balance -= 10;
    sem_post(s);
    return NULL;
}

void
multiThread() {
    int n = 100;
    pthread_t tid1[n];
    pthread_t tid2[n];
    for (int i = 0; i < n; i++) {
        pthread_create(&tid1[i], NULL, deposit, NULL);
        pthread_create(&tid2[i], NULL, withdrawal, NULL);
    }
    for (int i = 0; i < n; i++) {
        pthread_join(tid1[i], NULL);
        pthread_join(tid2[i], NULL);
    }
}

int
main() {
    const char c = 'c';
    s = sem_open(&c, O_CREAT, 0644, 1);
    multiThread();

    printf("main end, %u\n", balance);
    return 0;
}
