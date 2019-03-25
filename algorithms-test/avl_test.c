/**
 * 文件 avl_test 创建于 2017/11/3
 * @author 赵磊
 * @version 1.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inlclude/tree/avltree.h"
#include "../inlclude/compare.h"

#define LENGTH 10

static void avl_traverse(AvlTreeEntry *entry) {
  printf("key-> %d, value-> %d\n", *(int *) entry->key, *(int *) entry->value);
}

int main(void) {
  AvlTree *tree;
  AvlTreeEntry entry[LENGTH + 2];
  int *key, *value;

  AvlTree_init(&tree, free, free, int_compare);

  srand((unsigned) time(NULL));

  for (int i = 0; i < LENGTH; i++) {
    key = malloc(sizeof(int));
    value = malloc(sizeof(int));

    *key = rand() % 10;
    *value = i;

    printf("key-> %d, value-> %d\n", *key, *value);

    entry[i].key = key;
    entry[i].value = value;

    AvlTree_insert(tree, &entry[i]);
  }

  AvlTree_delete(tree, key, NULL);

  key = malloc(sizeof(int));
  value = malloc(sizeof(int));
  *key = -1;
  *value = 100;
  entry[LENGTH].key = key;
  entry[LENGTH].value = value;
  AvlTree_insert(tree, &entry[LENGTH]);

  printf("key-> %d, value-> %d\n", *key, *value);

  printf("============================\n");

  AvlTree_traverse(tree, avl_traverse);

}
