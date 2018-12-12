#include <stdio.h>
#include <pthread.h>

void *
run(void *args) {
    int n = 10;
    unsigned pid = (unsigned)pthread_self();
    while(n > 0) {
        printf("pid: %u num: %d\n", pid, n);
        n--;
    }
    return NULL;
}

void
multiThread() {
    int n = 10;
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
