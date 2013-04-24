#include <linux/limits.h>
#include <pthread.h>
#include "accept_list.h"
#include "fd_pair.h"
int port;
int thread_count;
char dir[PATH_MAX];
pthread_mutex_t accept_mutex;
pthread_mutex_t fp_mutex;
struct accept_list *AL = NULL;
struct fd_pair *FP = NULL;
char server_name[] = "Server: Bump Web Server/0.1\r\n";
