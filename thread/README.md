# 线程间同步方式
- 互斥量
- 信号量
- 条件变量
线程间通信的目的主要用于线程同步，所以线程没有像进程通信中的用于数据交换的通信机制

## 互斥量
cc -lpthread thread_mutex.c && ./a.out
```
pthread_mutex_t m;
pthread_mutex_init(&m, NULL); // 动态方式创建一个锁
pthread_mutex_lock(&m);
pthread_mutex_unlock(&m);
```
## 信号量（pv 信号量）
cc -lpthread thread_pv.c && ./a.out
```
sem_t *s;
const char c = 'c';
s = sem_open(&c, O_CREAT, 0644, 1);
sem_wait(s);
sem_post(s);
```
## 条件变量
```

```
