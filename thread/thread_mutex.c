#include <stdio.h>
#include <pthread.h>

static int balance = 0;
pthread_mutex_t m;

void *
deposit(void *args) {
    pthread_mutex_lock(&m);
    balance += 10;
    pthread_mutex_unlock(&m);
}

void *
withdrawal(void *args) {
    pthread_mutex_lock(&m);
    balance -= 10;
    pthread_mutex_unlock(&m);
}

void
multiThread(void) {
    int n = 1000;
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
    pthread_mutex_init(&m, NULL);
    multiThread();

    printf("main end, %d\n", balance);
    return 0;
}
