#include "conn_manage.h"
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include "fd_pair.h"

void feed_conn(int fd)
{
    (void)fd;
}

void conn_manage(struct accept_list *al)
{
    int epoll_fd;
    struct epoll_event ev;
    struct epoll_event events[MAX_CONN_PER_THREAD];

    int max_event = accept_list_length(al);
    int ret_val;

    //the argument of epoll_create is unused
    //but must big than zero
    epoll_fd = epoll_create(1);
    if (epoll_fd < 0)
    {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }
    struct accept_list *temp = al;
    for(; temp != NULL; temp = temp->next)
    {
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = temp->node;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, temp->node, &ev) < 0)
        {
            perror("epoll_ctl");
            continue;
        }
    }

    ret_val = epoll_wait(epoll_fd, events, max_event, -1);
    if (ret_val != max_event)
    {
        fprintf(stderr, "epoll_wait return %d, but max_event = %d\n",
                ret_val, max_event);
    }

    int i;
    for(i = 0; i < ret_val; i++)
    {
        if (events[i].events & EPOLLIN)
        {
            int retval = feed_conn(events[i].data.fd);
            if (0 != retval)
            {
                accept_list_remove(al, events[i].data.fd);
                close(events[i].data.fd);
            }
        }
    }

    //send start
    extern struct fd_pair FP;
    struct accept_list *head = al;
    struct fd_pair     *fdps = FP;
    for(; head != NULL; head = head->next)
    {

    }
}
