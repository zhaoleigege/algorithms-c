/**
 * 文件 bitree_test 创建于 2017/10/31
 * @author 赵磊
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inlclude/tree/bitree.h"
#include "../inlclude/compare.h"

#define LENGTH 5

struct message {
  int key;
  char msg[140];
};

static void traverse(void *data) {
  struct message *msg = data;
  printf("key-> %d, message-> %s\n", msg->key, msg->msg);
}

static int msg_compare(const void *v1, const void *v2) {
  int value1 = ((struct message *) v1)->key;
  int value2 = ((struct message *) v2)->key;
  return int_compare(&value1, &value2);
}

int main(void) {
  srand((unsigned) time(NULL));

  BiTree *tree;
  struct message *m;

  bitree_init(&tree, free, msg_compare);

  for (int i = 0; i < LENGTH; i++) {
    m = malloc(sizeof(struct message));
//    m->key = rand() % 100;
    m->key = (i + 2) % 5;
    printf("输入一系列字符串吧: ");
    scanf("%s", m->msg);
    bitree_insert(tree, m);
  }

  printf("二叉树大小-> %d\n", bitree_size(tree));

  printf("===================================\n");
  m = malloc(sizeof(struct message));
  int index = 3;
  m->key = 2;
//   index= bitree_look_up(tree, (void **) &m, sizeof(struct message));
  bitree_get(tree, index, m, sizeof(struct message));
//  bitree_delete(tree, (void **) &m);
  traverse(m);
  free(m);
  printf("index-> %d\n===================================\n", index);

  printf("二叉树大小-> %d\n", bitree_size(tree));

  bitree_traverse(tree, traverse);

  bitree_destroy(&tree);

  return 0;
}