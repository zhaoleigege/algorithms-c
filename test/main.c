#include <stdio.h>
#include "../list/list.h"

void show(element ele) {
  printf("%d\n", ele);
}

int main() {
  list array_list;
  init(&array_list);
  for (int i = 0; i < 15; i++)
    add(array_list, i);

  insert(array_list, size(array_list), 34);

  element ele;
  get(array_list, 9, &ele);
  printf("get-> %d\n", ele);

  delete(array_list, 5);

  printf("size->> %d\n", size(array_list));

  int a_size = size(array_list);
  for (int i = 0; i < a_size; i++) {
    element ele = i * 2;
    update(array_list, i, ele);
  }

  for_each(array_list, show);
  clean(array_list);
  printf("是否为空: %d\n", empty(array_list));
  destroy(&array_list);

  return 0;
}