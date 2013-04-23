#include "accept_list.h"
#include "http_head.h"
#include <stdio.h>
#include "feed_conn.h"
#include <linux/limits.h>
#include <string.h>

int parser_head(char* buffer, struct http_head* hh, int str_len)
{
    if (buffer == NULL || hh == NULL ||str_len <= 0)
    {
        if (buffer == NULL)
            fprintf(stderr, "%s buffer == NULL", __FUNCTION__);
        if (NULL == hh)
            fprintf(stderr, "%s struct http_head == NULL", __FUNCTION__);
        if (str_len <= 0)
            fprintf(stderr, "%s str_len should big than zero\n", __FUNCTION__);
        if (strlen(buffer) != str_len)
            fprintf(stderr, "%s str_len != the length of buffer\n", __FUNCTION__);
        return -1;
    }

    char method[METHOD_NAME_LEN] = "";
    char filepath[PATH_MAX + 1] = "";

    sscanf(buffer, "%s", method);
    if (!strncmp(method, "GET", 3))
    {
        hh->method = GET;
        sscanf(buffer, "%s%s", filepath, filepath);
        return 0;
    }
    else if(!strncmp(method, "HEAD", 4))
    {
        hh->method = HEAD;
        return 1;
    }
}

int gen_http_head(char *head_str, int return_code, char* filepath)
{
    char *flag = head_str;
    if (head_str == NULL)
    {
        fprintf(stderr, "%s, head_str shouldn't be NULL", __FUNCTION__);
    }
    if(200 == return_code)
    {
        extern char server_name[];
        strcpy(head_str, "HTTP/1.1 200 OK\r\n");
        flag += strlen("HTTP/1.1 200 OK\r\n");
        strcpy(flag, server_name);
        flag += strlen(server_name);

    }

}
