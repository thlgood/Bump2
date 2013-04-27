#include "accept_list.h"
#include "http_head.h"
#include <stdio.h>
#include "feed_conn.h"
#include <linux/limits.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

inline void get_gmtime(char *ptr)
{
    time_t time_value = time(NULL);
    struct tm *tm_ptr = gmtime(&time_value);
    strftime(ptr, 64, "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", tm_ptr);
}

inline int get_file_size(char *filepath)
{
    struct stat file_stat;
    if (lstat(filepath, &file_stat) < 0)
    {
        file_stat.st_size = 0;
        perror("lstat");
    }
    return (int)file_stat.st_size;

}

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
        if (strlen(buffer) != (unsigned)str_len)
            fprintf(stderr, "%s str_len != the length of buffer\n", __FUNCTION__);
        return -1;
    }

    char method[METHOD_NAME_LEN] = "";
    char filepath[PATH_MAX + 1] = "";

    sscanf(buffer, "%s", method);
    if (!strncmp(method, "GET", 3))
    {
        extern char dir[];
        hh->method = GET;
        sscanf(buffer, "%s%s", filepath, filepath);
        if (!strcmp(filepath, "/")) {
            strcat(filepath, "index.html");
        }
        strcpy(hh->URL, dir);
        strcat(hh->URL, filepath);
        printf("hh->url:%s", hh->URL);
        return 0;
    }
    else if(!strncmp(method, "HEAD", 4))
    {
        hh->method = HEAD;
        return 1;
    }
    else
    {
        hh->method = UNKNOW;
        return 1;
    }
}

void gen_http_head(char *head_str, int return_code, char* filepath)
{
    char *flag = head_str;
    int tmp;
    (void)filepath;
    if (head_str == NULL)
    {
        fprintf(stderr, "%s, head_str shouldn't be NULL", __FUNCTION__);
    }
    if(200 == return_code)
    {
        //HTTP/1.1 200 ok\r\n
        tmp = sprintf(flag, "HTTP/1.1 200 OK\r\n");
        flag += tmp;

        //Server: Bump Web Server/0.1\r\n
        tmp = sprintf(flag, "Server: Bump Web Server/0.1\r\n");
        flag += tmp;

        //Date: Tue, 23 Apr 2013 07:03:49 GMT
        char time_string[64];
        get_gmtime(time_string);
        tmp = sprintf(flag, "%s", time_string);
        flag += tmp;
        if (filepath != NULL)
        {
            //Content-Length: 8775\r\n
            tmp = sprintf(flag, "Content-Length: %d\r\n", get_file_size(filepath));
            flag += tmp;
        }

        //Content-Type: application/octet-stream\r\n
        tmp = sprintf(flag, "Content-Type: application/octet-stream\r\n");
        flag += tmp;

        //Connection: keep-alive
        tmp = sprintf(flag, "Connection: close\r\n");
        flag += tmp;
    }
    else if (403 == return_code)
    {
        //HTTP/1.1 200 ok\r\n
        tmp = sprintf(flag, "HTTP/1.1 403 Forbidden\r\n");
        flag += tmp;

        //Server: Bump Web Server/0.1\r\n
        tmp = sprintf(flag, "Server: Bump Web Server/0.1\r\n");
        flag += tmp;

        //Date: Tue, 23 Apr 2013 07:03:49 GMT
        char time_string[64];
        get_gmtime(time_string);
        tmp = sprintf(flag, "%s", time_string);
        flag += tmp;
    }

    else if(404 == return_code)
    {
        //HTTP/1.1 200 ok\r\n
        tmp = sprintf(flag, "HTTP/1.1 404 Not Found\r\n");
        flag += tmp;

        //Server: Bump Web Server/0.1\r\n
        tmp = sprintf(flag, "Server: Bump Web Server/0.1\r\n");
        flag += tmp;

        //Date: Tue, 23 Apr 2013 07:03:49 GMT
        char time_string[64];
        get_gmtime(time_string);
        tmp = sprintf(flag, "%s", time_string);
        flag += tmp;
    }
    else
    {
        //HTTP/1.1 400 Bad Request\r\n
        tmp = sprintf(flag, "HTTP/1.1 404 Not Found\r\n");
        flag += tmp;

        //Server: Bump Web Server/0.1\r\n
        tmp = sprintf(flag, "Server: Bump Web Server/0.1\r\n");
        flag += tmp;

        //Date: Tue, 23 Apr 2013 07:03:49 GMT
        char time_string[64];
        get_gmtime(time_string);
        tmp = sprintf(flag, "%s", time_string);
        flag += tmp;
    }

    sprintf(flag, "\r\n");
    flag[2] = '\0';
}
