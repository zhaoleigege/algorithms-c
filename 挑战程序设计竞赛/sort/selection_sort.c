/* 选择排序算法，包含下标为start但不包含end的数组 */
void selection_sort(int array[], int start, int end)
{
    for (int i = start; i < end; i++)
    {
        int index = i;
        for (int j = i; j < end; j++)
        {
            if (array[j] < array[index])
                index = j;
        }
        int temp = array[index];
        array[index] = array[i];
        array[i] = temp;
    }
}

void sort(int array[], int start, int end)
{
    selection_sort(array, start, end);
}