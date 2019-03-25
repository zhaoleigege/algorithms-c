#include <stdio.h>

/* 冒泡排序，包含下标为start但不包含end的数组 */
/* 从最后两位开始比较数据，如果前一个大于后一个则交换数据，一直到已经排好了序的数据末尾 */
void bubble_sort(int array[], int start, int end)
{
    for (int i = start; i < end; i++)
    {
        for (int j = end - 1; j > i; j--)
        {
            if (array[j - 1] > array[j])
            {
                int temp = array[j - 1];
                array[j - 1] = array[j];
                array[j] = temp;
            }
        }
    }
}