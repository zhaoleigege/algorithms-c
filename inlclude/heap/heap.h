/**
 * 文件 heap 创建于 2017/11/7
 * @author 赵磊
 * @version 1.0
 */

#ifndef HEAP_H
#define HEAP_H
#include <stdbool.h>
#include "../util.h"

typedef struct heap_ Heap;
typedef void *heapKey;
typedef void *heapValue;

bool heap_init(Heap **heap, unsigned length, compare_func key_cmp, key_free kfree, value_free vfree);

void heap_destroy(Heap **heap);

bool heap_insert(Heap *heap, heapKey key, heapValue value);

void heap_extract(Heap *heap, heapKey *key, heapValue *value);

void heap_traverse(Heap *heap, void (*action)(heapKey key, heapValue value));

unsigned heap_size(Heap *heap);

#endif
