#ifndef THREAD_HANDLE_H
#define THREAD_HANDLE_H

#include <pthread.h>

void *thread_handle(void *arg);
#define MAX_CONN_PER_THREAD 1000

#endif // THREAD_HANDLE_H
