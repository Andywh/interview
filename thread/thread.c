#include <stdio.h>
#include <pthread.h>

void *
run(void *args) {
    unsigned pid = pthread_self();
    int n = 10;
    while (n > 0) {
        printf("%run in thread %u, %d\n", pid, n);
        n--;
    }
    return NULL;
}

void
multiThread(void) {
    int n = 12;
    pthread_t tid[n];
    for (int i = 0; i < n; i++) {
        pthread_create(&tid[i], NULL, run, NULL);
    }
    for (int i = 0; i < n; i++) {
        pthread_join(tid[i], NULL);
    }
}

int
main() {
    multiThread();
    printf("main end\n");
    return 0;
}
