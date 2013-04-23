#include "thread_handle.h"
#include "accept_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void feed_request(struct accept_list *p)
{
    (void)p;
    //sleep(1);
}

void* thread_handle(void *arg)
{
    (void)arg;

    pthread_detach(pthread_self());
    int current_connections = 0;
    struct accept_list *thread_AL = NULL;
    extern pthread_mutex_t accept_mutex;

    while(1)
    {
        int accept_disabled = MAX_CONN_PER_THREAD / 8
                - (MAX_CONN_PER_THREAD - current_connections);

        if (accept_disabled > 0)
        {
            accept_disabled--;
        }
        else
        {
            if (!pthread_mutex_trylock(&accept_mutex))
            {
                int new_accept = 0;
                extern struct accept_list *AL;
                if ((new_accept = accept_list_get(AL)) != 0)
                {
                    AL = accept_list_remove(AL, new_accept);
                    thread_AL = accept_list_add(thread_AL, new_accept);
                }
                pthread_mutex_unlock(&accept_mutex);
            }
        }
        feed_request(thread_AL);
    }
}
