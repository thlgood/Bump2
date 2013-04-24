#ifndef CONN_MANAGE_H
#define CONN_MANAGE_H
#include "accept_list.h"
#include "thread_handle.h"
void conn_manage(struct accept_list **al, int* epoll_fd);
#endif // CONN_MANAGE_H
