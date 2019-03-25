/* 循环链表 */

#include <stdlib.h>
#include "list.h"

typedef struct _node
{
    void *data;
    struct _node *next;
} Node;

struct _list
{
    Node *head;
    data_free data_free;
    int size;
};

void list_init(List **list, void (*data_free)(void *))
{
    *list = malloc(sizeof(struct _list));
    (*list)->head = NULL;
    (*list)->data_free = data_free;
    (*list)->size = 0;
}

void list_destroy(List **list)
{
    if ((*list) == NULL)
        return;

    Node *node = (*list)->head;

    while ((*list)->size != 0)
    {
        Node *next_node = node->next;
        (*list)->data_free(node->data);
        node->next = NULL;
        free(node);

        node = next_node;
        (*list)->size = (*list)->size - 1;
    }

    (*list)->data_free = NULL;
    free(*list);
    *list = NULL;
}

static Node *new_node(void *data)
{
    Node *node = malloc(sizeof(struct _node));
    node->data = data;
    node->next = NULL;

    return node;
}

void list_insert(List *list, void *data)
{
    if (list == NULL)
        return;

    if (list->head == NULL)
    {
        list->head = new_node(data);
        list->head->next = list->head;
    }
    else
    {
        Node *node = new_node(data);
        node->next = list->head->next;
        list->head->next = node;
    }

    list->size = list->size + 1;
}

int list_is_circle(List *list)
{
    Node *fast = list->head;
    Node *slow = list->head;

    while (1)
    {
        fast = fast->next;
        if (fast == NULL)
            return 0;

        fast = fast->next;
        slow = slow->next;

        if (fast == slow)
            return 1;
    }
}