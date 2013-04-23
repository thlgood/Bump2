#include "feed_conn.h"
#include "http_head.h"
#include <stdio.h>
#include <sys/socket.h>
#include <accept_list.h>
int feed_conn(int fd)
{
    char buffer[BUFSIZ];
    int retval;                 //return value;
    if ((retval = recv(fd, buffer, BUFSIZ, 0)) <= 0)
        return retval;
    struct http_head request_head;
    parser_head(buffer, &request_head, retval);


}
