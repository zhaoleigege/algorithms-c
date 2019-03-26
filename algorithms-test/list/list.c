#include <stdlib.h>
#include <stdio.h>

#include "list.h"

typedef struct _node
{
   Element ele;
   struct _node* next;
} Node;


struct _list{
    struct _node* head;

    int size;
    void (*ele_free)(Element);
};

void init(List** list,Ele_free ele_free){
    printf("传递的值%p\n", list);
    printf("*p->%p\n", *list);
    *list = malloc(sizeof(struct _list));

    (*list)->head = NULL;
    (*list)->size = 0;
    (*list)->ele_free = ele_free;
}