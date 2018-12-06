/**
 * 文件 testQueue 创建于 2017/10/19
 * @author 赵磊
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "../inlclude/list/queue.h"

#define LENGTH 100

int main(void) {
  Queue *queue;
  int *data;

  queue_init(&queue, free);

  for (int i = 0; i < LENGTH; i++) {
    data = malloc(sizeof(int));
    *data = i + 1;
    queue_enqueue(queue, data);
  }

  printf("peek-> %d\n", *(int *) queue_peek(queue));

  printf("size-> %d\n", queue_size(queue));

  for (int i = 0; i < LENGTH; i++) {
    queue_dequeue(queue, (QueueValue *) &data);
    printf("data-> %d\n", *data);
    free(data);
  }

  queue_destroy(&queue);

  return 0;
}