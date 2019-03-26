#include <stdio.h>
/* 插入排序算法，包含下标为start但不包含end的数组 */
void insert_sort(int array[], int start, int end)
{
    for (int i = start + 1; i < end; i++)
    {
        int curVlaue = array[i];
        int index = i - 1;
        for (; index >= 0; index--)
        {
            if (curVlaue < array[index])
            {
                array[index + 1] = array[index];
                continue;
            }
            else
                break;
        }
        array[index + 1] = curVlaue;
    }
}