/**
 * 文件 heap_test 创建于 2017/11/7
 * @author 赵磊
 * @version 1.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inlclude/heap/heap.h"
#include "../inlclude/compare.h"

#define LENGTH 15
#define STRING_LENGTH 140

void foreach(heapKey key, heapValue value) {
  printf("key-> %d, value-> %s\n", *(int *) key, (char *) value);
}

int main(void) {
  Heap *heap;

  int *key;
  char *string;

  heap_init(&heap, LENGTH, int_compare, free, free);

  srand((unsigned) time(NULL));

  for (int i = 0; i < LENGTH; i++) {
    key = malloc(sizeof(int));
    string = malloc(STRING_LENGTH * sizeof(char));
    *key = rand() % 100;
    printf("print: ");
    scanf("%s", string);
    heap_insert(heap, key, string);
    printf("num-> %d, string->%s \n", *key, string);
  }

  for(int i = 0; i < LENGTH; i++) {
    heap_extract(heap, (void **) &key, (void **) &string);

    printf("max->%d value->%s\n", *key, string);

    free(key);
    free(string);
  }

  heap_traverse(heap, foreach);

  heap_destroy(&heap);
}