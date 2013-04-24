#ifndef HTTP_HEAD_H
#define HTTP_HEAD_H
#include <linux/limits.h>
enum http_method{GET,POST,HEAD, UNKNOW};

#define METHOD_NAME_LEN 10
#define HEAD_MAX_LEN   1024

struct http_head
{
    enum http_method method;
    char URL[PATH_MAX];
};

//return 0 success
//return <0 for bad request
//return >0 for argument error
int parser_head(char* buffer, struct http_head*, int str_len);


void gen_http_head(char* head_str, int return_code, char* filepath);

#endif // HTTP_HEAD_H
