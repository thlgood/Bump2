#include "fd_pair.h"

struct fd_pair *fd_pair_remove(struct fd_pair *fpl, int key)
{
    struct fd_pair *temp;

    //if fpl == NULL
    if(NULL == fpl)
    {
        return NULL;
    }

    //fpl at first
    if (fpl->client_fd == key)
    {
        temp = fpl->next;
        if (temp)
        {
            temp->prev = NULL;
        }
        free(fpl);
        return temp;
    }
    while((temp != NULL) && (temp->client_fd != key))
    {
        temp = temp->next;
    }
    if (temp->client_fd == key)
    {
        struct fd_pair *pre = temp->prev;
        struct fd_pair *nex = temp->next;
        if (pre)
        {
            pre->next = temp->next;
        }
        if (nex)
        {
            nex->prev = temp->prev;
        }
        free(temp);
        return fpl;
    }
    else
    {
        fprintf(stderr, "Error! can't find the node, val=%d, %s", val, __FUNCTION__);
        return fpl;
    }
}

struct fd_pair* fd_pair_add(struct fd_pair *fpl, int client_fd, int server_fd)
{
    struct fd_pair *temp = (struct fd_pair*)malloc(sizeof(struct fd_pair));
    if (fpl)
    {
        temp->next = fpl;
        temp->prev = NULL;
        fpl->prev = temp;
    }
    temp->client_fd = client_fd;
    temp->server_fd = server_fd;
    return temp;
}
