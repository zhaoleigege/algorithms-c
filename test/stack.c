/**
 * 文件 stack 创建于 2017/10/15
 * @author 赵磊
 * @version 1.0
 */

#include <stdio.h>
#include "../list/stack.h"

int main(void) {
  stack s_linked;
  s_init(&s_linked);
  s_push(s_linked, 10);
  s_push(s_linked, 9);
  s_push(s_linked, 8);

  element  ele;
  s_peek(s_linked, &ele);
  printf("peek-> %d\n", ele);

  while (s_size(s_linked)) {
    element ele;
    s_pop(s_linked, &ele);
    printf("element-> %d\n", ele);
  }

  printf("size-> %d\n", s_size(s_linked));

  s_destroy(&s_linked);
  return 0;
}