/**
 * 文件 heap 创建于 2017/11/7
 * @author 赵磊
 * @version 1.0
 */

#include <stdlib.h>
#include "../../inlclude/heap/heap.h"

#define DEFAULT_LENGTH 4

struct entry {
  heapKey key;
  heapValue value;
};

struct heap_ {
  unsigned length;
  struct entry **entrys;

  compare_func compare;
  key_free kfree;
  value_free vfree;

  unsigned size;
};

static inline void free_entry(Heap *heap, unsigned index) {
  if (heap->kfree != NULL && heap->entrys[index]->key != NULL)
    heap->kfree(heap->entrys[index]->key);

  if (heap->vfree != NULL && heap->entrys[index]->value != NULL)
    heap->vfree(heap->entrys[index]->value);

  heap->entrys[index]->key = NULL;
  heap->entrys[index]->value = NULL;

  free(heap->entrys[index]);
  heap->entrys[index] = NULL;
}

bool heap_init(Heap **heap, unsigned length, compare_func key_cmp, key_free kfree, value_free vfree) {
  if (key_cmp == NULL)
    return false;

  *heap = malloc(sizeof(struct heap_));
  if ((*heap) == NULL)
    return false;

  if (length < DEFAULT_LENGTH)
    length = DEFAULT_LENGTH;

  (*heap)->length = length;
  if (((*heap)->entrys = malloc(length * sizeof(struct entry *))) == NULL) {
    free(*heap);
    return false;
  }

  (*heap)->compare = key_cmp;
  (*heap)->kfree = kfree;
  (*heap)->vfree = vfree;
  (*heap)->size = 0;

  return true;
}

void heap_destroy(Heap **heap) {
  for (unsigned i = 1; i <= (*heap)->size; i++)
    free_entry(*heap, i);

  free((*heap)->entrys);
  (*heap)->entrys = NULL;
  (*heap)->compare = NULL;
  (*heap)->kfree = NULL;
  (*heap)->vfree = NULL;

  free(*heap);
  *heap = NULL;
}

static inline void exchange(struct entry **array, unsigned index1, unsigned index2) {
  struct entry *temp = array[index1];
  array[index1] = array[index2];
  array[index2] = temp;
}

static bool resize(Heap *heap, unsigned length) {
  struct entry **temp = realloc(heap->entrys, length * sizeof(struct entry *));
  if (temp == NULL)
    return false;

  heap->entrys = temp;
  heap->length = length;

  return true;
}

static inline void swim(Heap *heap) {
  unsigned index = heap->size;

  while (index > 1 && heap->compare(heap->entrys[index]->key, heap->entrys[index / 2]->key) > 0) {
    exchange(heap->entrys, index, index / 2);
    index = index / 2;
  }
}

bool heap_insert(Heap *heap, heapKey key, heapValue value) {
  if (key == NULL)
    return false;

  if (heap->size + 1 >= heap->length)
    if (!resize(heap, (heap->length << 1) + 1))
      return false;

  struct entry *data = malloc(sizeof(struct entry));
  if (data == NULL)
    return false;

  data->key = key;
  data->value = value;

  heap->entrys[++heap->size] = data;
  swim(heap);

  return true;
}

static inline void sink(Heap *heap) {
  unsigned index = 1;

  while (2 * index <= heap->size) {
    unsigned j = index * 2;

    if (j + 1 <= heap->size && heap->compare(heap->entrys[j + 1]->key, heap->entrys[j]->key) > 0)
      j++;
    if (heap->compare(heap->entrys[index]->key, heap->entrys[j]->key) > 0)
      break;

    exchange(heap->entrys, index, j);
    index = j;
  }
}

void heap_extract(Heap *heap, heapKey *key, heapValue *value) {
  if (heap->size < 1)
    return;

  if (key != NULL) {
    *key = heap->entrys[1]->key;
    heap->entrys[1]->key = NULL;
  }

  if (value != NULL) {
    *value = heap->entrys[1]->value;
    heap->entrys[1]->value = NULL;
  }

  exchange(heap->entrys, 1, heap->size);
  free_entry(heap, heap->size--);

  if ((heap->length >> 2) > heap->size)
    resize(heap, (heap->length >> 1) + 1);

  sink(heap);
}

void heap_traverse(Heap *heap, void (*action)(heapKey key, heapValue value)) {
  for (unsigned i = 1; i <= heap->size; i++)
    action(heap->entrys[i]->key, heap->entrys[i]->value);
}

unsigned heap_size(Heap *heap) {
  return heap->size;
}