/**
 * 文件 queue 创建于 2017/10/19
 * @author 赵磊
 * @version 1.0
 */



#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
typedef void *QueueValue;

typedef struct _queue Queue;

bool queue_init(Queue **queue, void (*ele_free)(QueueValue data));

void queue_destroy(Queue **queue);

bool queue_enqueue(Queue *queue, QueueValue data);

bool queue_dequeue(Queue *queue, QueueValue *data);

QueueValue queue_peek(Queue *queue);

int queue_size(Queue *queue);

#endif
