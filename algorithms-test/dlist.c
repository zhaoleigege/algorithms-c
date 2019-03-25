/**
 * 文件 dlist 创建于 2017/10/19
 * @author 赵磊
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "../inlclude/list/dlist.h"
#include "../inlclude/compare.h"

#define LENGTH 100000000

int main(void) {
  DList *list;
  dlist_init(&list, free);
  int *data;

  for (int i = 0; i < LENGTH; i++) {
    data = malloc(sizeof(int));
    *data = i * 2;
    dlist_insert(list, i, data);
  }

  data = malloc(sizeof(int));
  *data = 20;
  dlist_insert(list, 3, data);

  data = malloc(sizeof(int));
  *data = 30;
  dlist_insert(list, 2, data);

  dlist_remove(list, 1, (DListValue *) &data);
  printf("remove-> %d\n", *data);

  *data = 8;
  int index = dlist_find(list, data, int_compare);
  printf("find-> %d\n", index);
  dlist_update(list, index, data);

  dlist_reverse(list);

  for (int i = 0; i < dlist_size(list); i++) {
    dlist_get(list, i, (DListValue *) &data);
    printf("data-> %d\n", *data);
  }

  dlist_destroy(&list);
  return 0;
}