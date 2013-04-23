#ifndef ACCEPT_LIST_H
#define ACCEPT_LIST_H

struct accept_list
{
    int node;
    struct accept_list *prev;
    struct accept_list *next;
};

int accept_list_length(struct accept_list *al);
struct accept_list* accept_list_add(struct accept_list *al, int val);
struct accept_list* accept_list_remove(struct accept_list *al, int val);
int accept_list_get(struct accept_list *);
int accept_list_max(struct accept_list *);
#endif // ACCEPT_LIST_H
