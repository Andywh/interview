#include <pthread.h>


struct _ThreadPool;
typedef struct _ThreadPool ThreadPool;

// 函数指针 typedef
// 这种东西就是照抄我的例子抄，别想背下来
typedef void *(TaskCallback)(void *);


// 创建并返回一个 thread pool
// 参数为线程池的线程数量
ThreadPool* ThreadPoolNew(int numberOfThreads);

// 给线程池添加一个任务
// pool 是线程池
// callback 是线程要执行的函数
// arg 是传给
int ThreadPoolAddTask(ThreadPool *pool, TaskCallback *callback, void *arg);

// 删除并释放一个线程池的资源
// pool 是要销毁的线程池
int ThreadPoolFree(ThreadPool *pool);

