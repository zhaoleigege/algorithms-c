/* 希尔排序，序列数组（9 * 4^i - 9 * 2^i + 1）和（2^(i + 1) * (2^(i + 2) - 3) + 1） */
/* 1, 5, 19, 41, 109 */
/** 实现1
void shell_sort(int array[], int start, int end)
{
    int step_array[] = {1, 5};
    int step_len = 2;

    for (int i = step_len - 1; i >= 0; i--)
    {
        int step = step_array[i];
        for (int j = start + step; j < end; j += 1)
        {
            int index = j;
            for (int k = j - step; k >= start; k -= step)
            {
                if (array[k] > array[j])
                {
                    index -= step;
                }
                else
                {
                    break;
                }
            }

            int curValue = array[j];
            for (int k = j; k > index; k -= step)
                array[k] = array[k - step];

            array[index] = curValue;
        }
    }
}
*/
/* 实现2 */
void shell_sort(int array[], int start, int end){
    int step_array[] = {1, 5};
    int step_len = 2;

    for(int i = step_len - 1; i >= 0; i--){
        int step = step_array[i];

        for(int j = start + step; j < end; j++){
            int index = j;
            int curValue = array[j];
            while(index - step >= start && array[index - step] > curValue){
                array[index] = array[index - step];
                index -=step; 
            }

            array[index] = curValue;
        }
    }
}

void sort(int array[], int start, int end)
{
    shell_sort(array, start, end);
}