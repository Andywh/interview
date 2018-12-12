#include <stdio.h>
#include <pthread.h>

pthread_mutex_t m;
pthread_cond_t c;
static unsigned count = 0;

void *
increment(void *args) {
    pthread_mutex_lock(&m);
    if(count == 0) {
        printf("begin signal\n");
        pthread_cond_signal(&c);
        printf("end signal\n");
    }
    count += 1;
    pthread_mutex_unlock(&m);
    pthread_exit(0);
}

void *
decrement(void *args) {
    pthread_mutex_lock(&m);
    while(count == 0) {
        printf("begin wait\n");
        pthread_cond_wait(&c, &m);
        printf("end wait\n");
    }
    count -= 1;
    pthread_mutex_unlock(&m);
    pthread_exit(0);
}

void
multiThread() {
    pthread_t tid1, tid2;
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c, NULL);

    pthread_create(&tid1, NULL, decrement, NULL);
    pthread_create(&tid2, NULL, increment, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}

int
main() {
    multiThread();
    printf("main end\n");
    return 0;
}
