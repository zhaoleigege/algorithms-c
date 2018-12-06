/**
 * 文件 list_hash 创建于 2017/10/20
 * @author 赵磊
 * @version 1.0
 */

#include <stdlib.h>

#include "../../inlclude/list/list.h"
#include "../../inlclude/hash/list_hash.h"

struct CHTbl_ {
  int buckets;

  unsigned int (*h)(const void *key);
  int (*match)(const void *key1, const void *key2);
  void (*destroy)(void *data);

  unsigned int size;
  List **table;
};

bool chtbl_init(CHTbl **htbl,
                int buckets,
                unsigned int (*h)(const void *key),
                int (*match)(const void *key1, const void *key2),
                void (*destroy)(void *data)
) {
  if (h == NULL || match == NULL)
    return false;

  if ((*htbl = malloc(sizeof(struct CHTbl_))) == NULL)
    return false;

  (*htbl)->size = 0;
  (*htbl)->buckets = buckets;
  (*htbl)->table = malloc(buckets * sizeof(List *));
  for (int i = 0; i < (*htbl)->buckets; i++)
    list_init(&(*htbl)->table[i], destroy, match);

  (*htbl)->h = h;
  (*htbl)->match = match;
  (*htbl)->destroy = destroy;

  return true;
}

void chtbl_destroy(CHTbl **htbl) {
  for (int i = 0; i < (*htbl)->buckets; i++)
    list_destroy(&(*htbl)->table[i]);

  free((*htbl)->table);

  free(*htbl);
  *htbl = NULL;
}

bool chtbl_insert(CHTbl *htbl, void *data) {
  if (chtbl_lookup(htbl, &data) != -1)
    return false;

  int bucket = htbl->h(data) % htbl->buckets;
  list_insert(htbl->table[bucket], 0, data);

  htbl->size++;

  return true;
}

bool chtbl_remove(CHTbl *htbl, void **data) {
  int index = chtbl_lookup(htbl, (void **) &data);
  if (index == -1)
    return false;

  htbl->size--;

  return list_remove(htbl->table[htbl->h(*data) % htbl->buckets], index, NULL);
}

int chtbl_lookup(const CHTbl *htbl, void **data) {
  int bucket = htbl->h(*data) % htbl->buckets;

  return list_find(htbl->table[bucket], *data, htbl->match);
}

unsigned int chtbl_size(const CHTbl *htbl) {
  return htbl->size;
}

void chtbl_foreach(const CHTbl *htbl, void (*action)(void *data)) {
  for (int i = 0; i < htbl->buckets; i++) {
    for (int j = 0; j < list_size(htbl->table[i]); j++) {
      void *ele;
      list_get(htbl->table[i], j, &ele);
      action(ele);
    }
  }
}