#ifndef CONFIG_H
#define CONFIG_H
#include <glib.h>
#include <linux/limits.h>
#ifndef _CONFIG_FILE_PATH_
#define _CONFIG_FILE_PATH_ "./http-download.conf"
#endif
/*
 *read config file 'config' file
 *return -1 for error
 */
int read_config();

extern int port;
extern int thread_count;
extern char dir[];
#endif // CONFIG_H
