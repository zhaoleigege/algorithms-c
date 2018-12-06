/**
 * 文件 queue 创建于 2017/10/19
 * @author 赵磊
 * @version 1.0
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../../inlclude/list/queue.h"

struct _queue_ele {
  QueueValue data;
  struct _queue_ele *next;
};

struct _queue {
  int size;

  struct _queue_ele *head;
  struct _queue_ele *tail;

  void (*ele_free)(QueueValue data);
};

bool queue_init(Queue **queue, void (*ele_free)(QueueValue data)) {
  if (ele_free == NULL)
    return false;

  *queue = malloc(sizeof(struct _queue));
  if (*queue == NULL)
    return false;

  (*queue)->size = 0;
  (*queue)->head = NULL;
  (*queue)->tail = NULL;
  (*queue)->ele_free = ele_free;

  return true;
}

void queue_destroy(Queue **queue) {
  while ((*queue)->size > 0)
    queue_dequeue(*queue, NULL);

  (*queue)->tail = NULL;
  (*queue)->head = NULL;
  (*queue)->ele_free = NULL;
  free(*queue);
}

bool queue_enqueue(Queue *queue, QueueValue data) {
  struct _queue_ele *node = malloc(sizeof(struct _queue_ele));
  if (node == NULL)
    return false;

  node->data = data;
  node->next = NULL;

  if (queue->tail == NULL) {
    queue->tail = node;
    queue->head = node;
  } else {
    queue->tail->next = node;
    queue->tail = node;
  }

  queue->size++;

  return true;
}

bool queue_dequeue(Queue *queue, QueueValue *data) {
  if (queue->size <= 0)
    return false;

  struct _queue_ele *node = queue->head;
  queue->head = node->next;

  if (queue->head == NULL)
    queue->tail = NULL;

  queue->size--;

  if (data == NULL)
    queue->ele_free(node->data);
  else
    *data = node->data;

  free(node);

  return true;
}

QueueValue queue_peek(Queue *queue) {
  if (queue->size <= 0)
    return NULL;

  return queue->head->data;
}

int queue_size(Queue *queue) {
  return queue->size;
}