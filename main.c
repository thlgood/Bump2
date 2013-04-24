#include <sys/socket.h>
#include <stdio.h>
#include "config.h"
#include "setup_tcp.h"
#include "thread_handle.h"
#include "accept_list.h"
#include "fd_pair.h"
#include <arpa/inet.h>
#include <fcntl.h>

int main(void)
{
    extern pthread_mutex_t accept_mutex;
    extern pthread_mutex_t fp_mutex;
    extern struct accept_list *AL;

    if (read_config() < 0)
    {
        return 1;
    }
    pthread_mutex_init(&accept_mutex, NULL);
    pthread_mutex_init(&fp_mutex, NULL);

    int sockfd;
    sockfd = setup_tcp();

    if (sockfd < 0)
    {

        return 1;
    }

    int i;
    for(i = 0; i < thread_count; i++)
    {
        pthread_t pthread;
        pthread_create(&pthread, NULL, thread_handle, NULL);
    }
    int fd;
    struct sockaddr_in sock_addr;
    socklen_t len = sizeof(sock_addr);
    while(1)
    {
        len = sizeof(sock_addr);
        fd = accept(sockfd, (struct sockaddr*)&sock_addr, &len);
        if (fd > 0)
        {
            int flag;
            flag = fcntl(fd, F_GETFL, 0);
            flag |= O_NONBLOCK;
            fcntl(fd, F_SETFL, flag);
            pthread_mutex_lock(&accept_mutex);
            AL = accept_list_add(AL, fd);
            pthread_mutex_unlock(&accept_mutex);
        }
    }
    return 0;
}
