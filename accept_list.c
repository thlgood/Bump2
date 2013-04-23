#include "accept_list.h"
#include <stdio.h>
#include <stdlib.h>

int accept_list_length(struct accept_list *al)
{

    if (al)
    {
        int tmp = 1;
        while((al = al->next) != NULL) tmp++;
        return tmp;
    }
    return 0;
}

struct accept_list* accept_list_remove(struct accept_list *al, int val)
{
    struct accept_list *temp;

    //if al == NULL
    if(NULL == al)
    {
        return NULL;
    }

    //al at first
    if (al->node == val)
    {
        temp = al->next;
        if (temp)
        {
            temp->prev = NULL;
        }
        free(al);
        return temp;
    }
    while((temp != NULL) && (temp->node != val))
    {
        temp = temp->next;
    }
    if (temp->node == val)
    {
        struct accept_list *pre = temp->prev;
        struct accept_list *nex = temp->next;
        if (pre)
        {
            pre->next = temp->next;
        }
        if (nex)
        {
            nex->prev = temp->prev;
        }
        free(temp);
        return al;
    }
    else
    {
        fprintf(stderr, "Error! can't find the node, val=%d, %s", val, __FUNCTION__);
        return al;
    }
}

struct accept_list* accept_list_add(struct accept_list *al, int val)
{
    struct accept_list *temp = (struct accept_list*)malloc(sizeof(struct accept_list));
    if (al)
    {
        temp->next = al;
        temp->prev = NULL;
        al->prev = temp;
    }
    temp->node = val;
    al = temp;
    return al;
}

int accept_list_get(struct accept_list *al)
{
    if (al)
    {
        int first_node = al->node;
        return first_node;
    }
    return 0;
}

int accept_list_max(struct accept_list *al)
{
    if (al)
    {
        int temp_max = al->node;
        while(al->next)
        {
            if (al->node > temp_max)
            {
                temp_max = al->node;
            }
        }
        return temp_max;
    }
    //else al == NULL
    else
    {
        return -1;
    }
}
