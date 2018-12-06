#include <stdio.h>
#include <stdlib.h>
#define MAX 100
typedef struct{
    int* page; //物理页框数组
    int status; //物理页框是否占满
    int size;   //物理页框大小
    int occupy; //物理页框已用大小
    int counts; //物理页框中断次数
}pageBlock;

int arrayContains(int* array, int start, int length, int value){
    for(int i = start; i < length; i++)
        if(array[i] == value)
            return i;

    return -1;
}

int longTimeNoUse(int* array, int srcLength, int* dataArray, int start, int length){
    int data, distance;
    distance = -1;
    data = -1;

    for(int i = 0; i < srcLength; i++){
        int valDistance = arrayContains(dataArray, start, length, array[i]);
        if(valDistance == -1){
            return array[i];
        }
        if(distance < valDistance){
            distance = valDistance;
            data = array[i];
        }
    }

    return data;
}

int main(void)
{
    int size, data[MAX], dataLength, temp;
    pageBlock* block = NULL;
    block = malloc(sizeof(pageBlock*));

    printf("输入物理页框的大小: ");
    scanf("%d", &size);

//物理页框初始化
    block->page = malloc(sizeof(int) * size);
    block->status = 0;
    block->size = size;
    block->occupy = 0;
    block->counts = 0;

    dataLength = 0;
    printf("输入全部的页面号引用串:(输入q结束) \n");
    while (scanf("%d", &temp) > 0)
    {
        data[dataLength++] = temp;
    }

    for(int i = 0; i < dataLength; i++){
        if(!block->status){
            if(arrayContains(block->page, 0, i, data[i]) == -1){
                block->page[block->occupy++] = data[i];
                if(block->occupy == block->size)
                    block->status = 1;
            }
        }else{
            if(arrayContains(block->page, 0, size, data[i]) == -1){
                int value = longTimeNoUse(block->page, size, data, i, dataLength);
                if(value == -1){
                    block->page[0] = data[i];
                }else{
                    block->page[arrayContains(block->page, 0, size, value)] = data[i];
                }
                block->counts++;
            }
        }
        printf("第%d次页面置换，现在物理页框的数据为: \n", i + 1);
        for(int j = 0; j < block->occupy; j++)
            printf("%d ", block->page[j]);
            printf("\n");
    }

    printf("页面全部置换完成.\n引发的缺页中断次数为%d次。\n", block->counts);
}