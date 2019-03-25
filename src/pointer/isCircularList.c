#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define LENGTH 10

int main(void)
{
    int *data;
    List *list;

    list_init(&list, free);
    for (int i = 0; i < LENGTH; i++)
    {
        data = malloc(sizeof(int));
        *data = i;
        list_insert(list, data);
    }

    printf("%d\n", list_is_circle(list));

    list_destroy(&list);

    return 0;
}
