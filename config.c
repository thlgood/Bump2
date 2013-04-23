#include "config.h"
#include <stdio.h>
#include <string.h>
int read_config()
{
    extern int thread_count;
    extern int port;
    extern char dir[];

    char *dst_path;
    GKeyFile *key_file = g_key_file_new();
    GError   *g_error = NULL;

    g_key_file_load_from_file(key_file, _CONFIG_FILE_PATH_, 0, &g_error);
    if (g_error)
    {
        fprintf(stderr, "%s %d:%s", __FUNCTION__, __LINE__, g_error->message);
        g_key_file_free(key_file);
        g_error_free(g_error);
        return -1;
    }
    dst_path = g_key_file_get_string(key_file, "options", "dir", &g_error);
    if (dst_path != NULL)
        strncpy(dir, dst_path, PATH_MAX);
    else
    {
        fprintf(stderr, "%s %d:%s", __FUNCTION__, __LINE__, g_error->message);
        g_key_file_free(key_file);
        g_error_free(g_error);
        return -2;
    }
    thread_count = g_key_file_get_integer(key_file, "options", "thread_count", NULL);
    if (!thread_count)
        thread_count = 2;
    port = g_key_file_get_integer(key_file, "options", "port", NULL);
    if (!port)
        port = 80;

    g_key_file_free(key_file);
    return 0;
}
