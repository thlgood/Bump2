#include "conn_manage.h"
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include "fd_pair.h"
#include <unistd.h>
#include <sys/socket.h>

void conn_manage(struct accept_list **al, int* epoll_fd)
{
    extern struct fd_pair *FP;
    struct epoll_event ev;
    struct epoll_event events[MAX_CONN_PER_THREAD];

    //extern struct accept_list AL;
    int max_event = accept_list_length(*al);
    int ret_val;

    if (*epoll_fd == 0)
    {
        //the argument of epoll_create is unused
        //but must big than zero
        *epoll_fd = epoll_create(1);
        if (*epoll_fd < 0)
        {
            perror("epoll_create");
            exit(EXIT_FAILURE);
        }
    }
    struct accept_list *temp = *al;
    for(; temp != NULL; temp = temp->next)
    {
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = temp->node;
        if (epoll_ctl(*epoll_fd, EPOLL_CTL_ADD, temp->node, &ev) < 0)
        {
            perror("epoll_ctl");
            continue;
        }
    }

    if (max_event > 0)
    {
        ret_val = epoll_wait(*epoll_fd, events, max_event, -1);
        if (ret_val != max_event)
        {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
    }

    int i;
    for(i = 0; i < ret_val; i++)
    {
        if (events[i].events & EPOLLIN)
        {
            int retval = feed_conn(events[i].data.fd);
            if (0 != retval)
            {
                *al = accept_list_remove(*al, events[i].data.fd);
                FP = fd_pair_remove(FP, events[i].data.fd);
                close(events[i].data.fd);
            }
        }
    }

    //send start
    struct accept_list *head = *al;
    //struct fd_pair     *fdps = FP;
    char buffer[BUFSIZ];
    for(; head != NULL; head = head->next)
    {
        int local_file_fd = fd_pair_get_val(FP, head->node);
        if (local_file_fd < 0)
        {
            fprintf(stderr, "%s local file fd should big than zero\n", __FUNCTION__);
            FP = fd_pair_remove(FP, head->node);
            *al = accept_list_remove(*al, head->node);
        }
        ret_val = read(local_file_fd, buffer, BUFSIZ);
        if (ret_val < 0)
        {
            FP = fd_pair_remove(FP, head->node);
            *al = accept_list_remove(*al, head->node);
        }
        else if (ret_val < BUFSIZ)
        {
            send(head->node, buffer, ret_val, 0);
            FP = fd_pair_remove(FP, head->node);
            *al = accept_list_remove(*al, head->node);
        }
        else
        {
            ret_val = send(head->node, buffer, ret_val, 0);
            if (ret_val <= 0)
            {
                FP = fd_pair_remove(FP, head->node);
                *al = accept_list_remove(*al, head->node);
            }
        }
    }
}
