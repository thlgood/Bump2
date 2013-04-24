#ifndef FD_PAIR_H
#define FD_PAIR_H

struct fd_pair
{
    int client_fd;
    int server_fd;
    struct fd_pair *prev;
    struct fd_pair *next;
};

struct fd_pair* fd_pair_remove(int key);
struct fd_pair* fd_pair_add(struct fd_pair *fpl, int client_fd, int server_fd);

#endif // FD_PAIR_H
