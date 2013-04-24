#include "feed_conn.h"
#include "http_head.h"
#include <stdio.h>
#include <sys/socket.h>
#include <accept_list.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "fd_pair.h"
#include <pthread.h>

int feed_conn(int fd)
{
    char buffer[BUFSIZ];
    int retval;                 //return value;
    if ((retval = recv(fd, buffer, BUFSIZ, 0)) <= 0)
        return retval;
    struct http_head request_head;
    parser_head(buffer, &request_head, retval);

    if (request_head.method == GET)
    {
        int path_length;
        extern char dir[];
        char filepath[PATH_MAX + 1] = "";
        int retval;
        strncpy(filepath, dir, PATH_MAX);
        path_length = strlen(filepath);
        strncat(filepath, request_head, PATH_MAX - path_length);
        if (request_head.URL[strlen(request_head.URL)] == '/')
            strcat(request_head.URL, "index.html");
        path_length = strlen(filepath);
        strncat(filepath, request_head.URL, PATH_MAX - path_length);
        retval = open(request_head.URL, O_RDONLY);
        if (retval)
        {
            extern int errno;
            char head[HEAD_MAX_LEN];
            if (errno == EACCES)
            {
                gen_http_head(head, 403, NULL);
                strcpy(head, "<h1>403 Forbidden</h1>");
                send(fd, head, strlen(head), 0);
            }
            else
            {
                gen_http_head(head, 404, NULL);
                strcpy(head, "<h1>404 Not Found</h1>");
                send(fd, head, strlen(head), 0);
            }
            return -1;
        }
        else
        {
            extern struct fd_pair *FP;
            extern pthread_mutex_t fp_mutex;
            pthread_mutex_lock(&fp_mutex);
            fd_pair_add(FP, fd, retval);
            pthread_mutex_unlock(&fp_mutex);
            return 0;
        }
    }
    else if (request_head.method = HEAD)
    {
        char head[HEAD_MAX_LEN];
        gen_http_head(head, 200,NULL);
        send(fd, head, strlen(head), 0);
        return 1;
    }
}
