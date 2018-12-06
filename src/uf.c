#include <stdbool.h>
#include <stdlib.h>
#include "../inlclude/uf.h"
/**
 * 文件 uf 创建于 2017/11/12
 * @author 赵磊
 * @version 1.0
 */

struct uf_ {
  int *id;
  int *sz;

  int size;
  int count;
};

bool uf_init(UF **uf, int size) {
  if (size <= 0)
    return false;

  *uf = malloc(sizeof(struct uf_));
  if ((*uf) == NULL)
    return false;

  (*uf)->count = size;
  (*uf)->size = size;

  (*uf)->id = malloc(size * sizeof(int));
  (*uf)->sz = malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
    (*uf)->id[i] = i;

  for (int i = 0; i < size; i++)
    (*uf)->sz[i] = 1;

  return true;
}

static inline int uf_find(UF *uf, int p) {
  int temp = p;

  while (p != uf->id[p]) p = uf->id[p];

  while (uf->id[temp] != p) {
    int inner = uf->id[temp];
    uf->id[temp] = p;
    temp = inner;
  }

  return p;
}

bool uf_connected(UF *uf, int p, int q) {
  return uf_find(uf, p) == uf_find(uf, q);
}

void uf_union(UF *uf, int p, int q) {
  int pIndex = uf_find(uf, p);
  int qIndex = uf_find(uf, q);

  if (qIndex == pIndex)
    return;

  if (uf->sz[pIndex] > uf->sz[qIndex]) {
    uf->id[qIndex] = pIndex;
    uf->sz[pIndex] += uf->sz[qIndex];
  } else {
    uf->id[pIndex] = qIndex;
    uf->sz[qIndex] += uf->sz[pIndex];
  }

  uf->count--;
}

int uf_count(UF *uf) {
  return uf->count;
}

void uf_destroy(UF **uf) {
  free((*uf)->id);
  free((*uf)->sz);

  free(*uf);
  *uf = NULL;
}

void uf_traverse(UF *uf, void (*action)(int index, int count)) {
  for (int i = 0; i < uf->size; i++)
    action(i, uf->id[i]);
}

