/**
 * 文件 queue 创建于 2017/10/15
 * @author 赵磊
 * @version 1.0
 */
#include <stdio.h>
#include "../list/queue.h"

int main(void) {
  queue q_linked;
  q_init(&q_linked);
  q_add(q_linked, 10);
  q_add(q_linked, 9);
  q_add(q_linked, 8);
  q_add(q_linked, 7);

  while (q_size(q_linked) > 0) {
    element ele;
    q_poll(q_linked, &ele);
    printf("element-> %d\n", ele);
  }

  q_destroy(&q_linked);

  return 0;
}
