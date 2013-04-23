#include <linux/limits.h>
#include <pthread.h>
#include "accept_list.h"
int port;
int thread_count;
char dir[PATH_MAX];
pthread_mutex_t accept_mutex;
struct accept_list *AL = NULL;

char server_name[] = "Bump Web Server/0.1";
