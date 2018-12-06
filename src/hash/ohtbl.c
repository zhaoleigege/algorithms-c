#include <stdlib.h>
#include "../../inlclude/hash/ohtbl.h"
#include "../../inlclude/list/list.h"
/**
 * 文件 ohtbl 创建于 2017/10/27
 * @author 赵磊
 * @version 1.0
 */

struct OHTbl_ {
  unsigned int size;
  unsigned int capacity;

  unsigned int (*hash)(const void *key);
  int (*match)(const void *k1, const void *k2);
  void (*key_free)(void *key);
  void (*value_free)(void *value);

  void *vacated;
  void **keys;
  void **values;
};

#define CAPACITY 8

/*设置一个标识位，表示这个元素的指向的内容已经删除*/
static char vacated;

bool ohtbl_init(
    OHTbl **ohtbl, unsigned int capacity                                     //m代表表中槽位的个数
    , unsigned int (*hash)(const void *key)                         //hash分别代表哈希函数
    , int (*match)(const void *k1, const void *k2)                  //match用来判断两个健是否相等
    , void (*key_free)(void *key)                                   //用来释放健的方法
    , void (*value_free)(void *value)                               //用来释放值的方法
) {
  if (hash == NULL || match == NULL || key_free == NULL || value_free == NULL)
    return false;

  *ohtbl = malloc(sizeof(struct OHTbl_));
  if ((*ohtbl) == NULL)
    return false;

  (*ohtbl)->capacity = capacity > CAPACITY ? capacity : CAPACITY;
  (*ohtbl)->size = 0;

  (*ohtbl)->hash = hash;
  (*ohtbl)->match = match;
  (*ohtbl)->key_free = key_free;
  (*ohtbl)->value_free = value_free;

  (*ohtbl)->vacated = &vacated;

  if (((*ohtbl)->keys = calloc((*ohtbl)->capacity, sizeof(void *))) == NULL) {
    free(*ohtbl);
    *ohtbl = NULL;
    return false;
  }

  if (((*ohtbl)->values = malloc((*ohtbl)->capacity * sizeof(void *))) == NULL) {
    free((*ohtbl)->keys);
    free(*ohtbl);
    *ohtbl = NULL;
    return false;
  }

  return true;
}

static void ohtbl_inner_remove(OHTbl *ohTbl, int index, void **value) {
  if (value != NULL) {
    *value = ohTbl->values[index];
  } else {
    ohTbl->value_free(ohTbl->values[index]);
  }

  ohTbl->key_free(ohTbl->keys[index]);
  ohTbl->keys[index] = ohTbl->vacated;

  ohTbl->values[index] = NULL;

  ohTbl->size--;
}

void ohtbl_destroy(OHTbl **ohtbl) {
  for (int i = 0; i < (*ohtbl)->capacity; i++) {
    if ((*ohtbl)->keys[i] != NULL && (*ohtbl)->keys[i] != (*ohtbl)->vacated)
      ohtbl_inner_remove(*ohtbl, i, NULL);
  }

  free((*ohtbl)->keys);
  free((*ohtbl)->values);

  (*ohtbl)->key_free = NULL;
  (*ohtbl)->value_free = NULL;
  (*ohtbl)->match = NULL;
  (*ohtbl)->hash = NULL;

  free(*ohtbl);
  *ohtbl = NULL;
}

static void ohtbl_copy_from_list(OHTbl *ohTbl, List *klist, List *vlist) {
  int size = list_size(klist);
  void *key;
  void *value;

  while (size > 0) {
    list_remove(klist, --size, &key);
    list_remove(vlist, size, &value);

    ohtbl_insert(ohTbl, key, value);
  }

  list_destroy(&klist);
  list_destroy(&vlist);
}

static bool ohtbl_resize(OHTbl *ohTbl, unsigned int capacity) {

  List *klist;
  List *vlist;

  list_init(&klist, free, NULL);
  list_init(&vlist, free, NULL);

  for (int i = 0; i < ohTbl->capacity; i++) {
    if (ohTbl->keys[i] != NULL && ohTbl->keys[i] != ohTbl->vacated) {
      list_insert(klist, 0, ohTbl->keys[i]);
      list_insert(vlist, 0, ohTbl->values[i]);
      ohTbl->keys[i] = NULL;
      ohTbl->values[i] = NULL;
    }
  }

  ohTbl->size = 0;

  void **temp = NULL;

  temp = realloc(ohTbl->keys, capacity * sizeof(void *));
  if (temp == NULL) {
    ohtbl_copy_from_list(ohTbl, klist, vlist);
    return false;
  } else {
    for (int i = 0; i < capacity; i++)
      temp[i] = NULL;

    ohTbl->keys = temp;
  }

  temp = realloc(ohTbl->values, capacity * sizeof(void *));
  if (temp == NULL) {
    ohtbl_copy_from_list(ohTbl, klist, vlist);
    return false;
  } else {
    for (int i = 0; i < capacity; i++)
      temp[i] = NULL;

    ohTbl->values = temp;
  }

  ohTbl->capacity = capacity;
  ohtbl_copy_from_list(ohTbl, klist, vlist);

  return true;
}

static int inner_look_up(OHTbl *ohTbl, void *key) {
  int position = ohTbl->hash(key) % ohTbl->capacity;
  while (ohTbl->keys[position] != NULL) {
    if (!ohTbl->match(key, ohTbl->keys[position]))
      return position;

    position++;
  }

  return -1;
}

static int ohtabl_insert_index(OHTbl *ohTbl, void *key) {
  int position = ohTbl->hash(key) % ohTbl->capacity;
  while (ohTbl->keys[position] != NULL && ohTbl->keys[position] != ohTbl->vacated) {
    if (!ohTbl->match(key, ohTbl->keys[position]))
      break;

    position = (position + 1) % ohTbl->capacity;
  }

  return position;
}

bool ohtbl_insert(OHTbl *ohTbl, void *key, void *value) {
  if (ohTbl->size > (ohTbl->capacity >> 1)) {
    if (!ohtbl_resize(ohTbl, (ohTbl->capacity << 1) + 1))
      if (ohTbl->size >= ohTbl->capacity)
        return false;
  }

  int index = ohtabl_insert_index(ohTbl, key);
  if (ohTbl->keys[index] == NULL) {
    ohTbl->size++;
  } else {
    ohTbl->key_free(ohTbl->keys[index]);
    ohTbl->value_free(ohTbl->values[index]);
  }

  ohTbl->keys[index] = key;
  ohTbl->values[index] = value;

  return true;
}

bool ohtbl_remove(OHTbl *ohTbl, void *key, void **value) {
  int index = inner_look_up(ohTbl, key);
  if (index == -1)
    return false;

  ohtbl_inner_remove(ohTbl, index, value);

  if (ohTbl->size <= (ohTbl->capacity >> 3))
    ohtbl_resize(ohTbl, ohTbl->capacity >> 1);

  return true;
}

bool ohtbl_lookup(OHTbl *ohTbl, void *key, void **value) {
  int index = inner_look_up(ohTbl, key);
  if (index == -1)
    return false;

  if (value != NULL)
    *value = ohTbl->values[index];

  return true;
}

unsigned int ohtbl_size(const OHTbl *ohTbl) {
  return ohTbl->size;
}

void ohtbl_foreach(const OHTbl *ohTbl, void (*action)(const void *key, const void *value)) {
  for (int i = 0; i < ohTbl->capacity; i++) {
    if (ohTbl->keys[i] != NULL && ohTbl->keys[i] != ohTbl->vacated)
      action(ohTbl->keys[i], ohTbl->values[i]);
  }
}
