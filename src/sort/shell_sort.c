/* 希尔排序，序列数组（9 * 4^i - 9 * 2^i + 1）和（2^(i + 1) * (2^(i + 2) - 3) + 1） */
/* 1, 5, 19, 41, 109 */
void shell_sort(int array[], int start, int end)
{
    int step_array[] = {1, 5};
    int step_len = 2;

    for (int i = step_len - 1; i >= 0; i--)
    {
        int step = step_array[i];
        for (int j = start + step; j < end; j += 1)
        {
            int curValue = array[j];
            int index = j - step;

            for (; index >= start; index -= step)
            {
                if (curValue < array[index])
                {
                    array[index + step] = array[index];
                }
            }

            array[index + step] = curValue;
        }
    }
}

