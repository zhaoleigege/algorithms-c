/**
 * 文件 testList 创建于 2017/10/17
 * @author 赵磊
 * @version 1.0
 */
#include <stdio.h>
#include <stdlib.h>
#include "../inlclude/list/list.h"
#include "../inlclude/compare.h"

#define LENGTH 10

int main(void) {
  int *data;
  List *list;
  list_init(&list, free, NULL);
  printf("free地址-> %p\n", free);
  printf("ele_free地址-> %p\n", get_ele_free(list));
  for (int i = 0; i < LENGTH; i++) {
    if ((data = malloc(sizeof(int))) == NULL)
      return 1;

    *data = i + 1;
    list_insert(list, i, data);
  }

  if ((data = malloc(sizeof(int))) == NULL)
    return 1;

  *data = 10;
  list_insert(list, 0, data);

  list_reverse(list);

  list_remove(list, 5, (void **) &data);
  printf("删除的数据-> %d\n", *data);
  free(data);

  list_get(list, 2, (void **) &data);
  data = malloc(sizeof(int));
  *data = 100;
  list_update(list, 2, data);

  printf("找到的索引-> %d\n", list_find(list, data, int_compare));

  for (int i = 0; i < list_size(list); i++) {
    list_get(list, i, (void **) &data);
    printf("第%d个元素 -> %d\n", i + 1, *data);
  }

  list_destroy(&list);

  return 0;
}

