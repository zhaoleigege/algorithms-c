#include <stdio.h>
#include "./sort.h"

int main(void)
{
    int len = 9;
    int array[] = {5, 2, 4, 6, 1, 3, 6, 8, 0};
    sort(array, 0, len);

    for (int i = 0; i < len; i++)
        printf("%d ", array[i]);

    return 0;
}