/**
 * 文件 clist 创建于 2017/10/20
 * @author 赵磊
 * @version 1.0
 */

#include <stdlib.h>
#include "../../inlclude/list/clist.h"

struct _clist_ele {
  void *data;
  struct _clist_ele *next;
};

struct _clist {
  int size;

  struct _clist_ele *tail;

  struct _clist_ele *current;
  int c_index;

  void (*ele_free)(void *ele);
};

bool clist_init(CList **list, void (*ele_free)(void *ele)) {
  if (ele_free == NULL)
    return false;

  *list = malloc(sizeof(struct _clist));
  if (*list == NULL)
    return false;

  (*list)->size = 0;
  (*list)->tail = NULL;
  (*list)->current = NULL;
  (*list)->c_index = -1;

  (*list)->ele_free = ele_free;

  return true;
}

void clist_destroy(CList **list) {
  while ((*list)->size > 0)
    clist_remove(*list, 0, NULL);

  (*list)->tail = NULL;
  (*list)->current = NULL;
  (*list)->ele_free = NULL;

  free(*list);
  *list = NULL;
}

static void mcur_pointer(CList *list, int index) {
  if (list->c_index == index)
    return;

  if (index == list->size - 1) {
    list->current = list->tail;
    list->c_index = list->size - 1;
    return;
  }

  if (index < list->c_index) {
    list->current = list->tail->next;
    list->c_index = 0;
  }

  while (list->c_index < index) {
    list->current = list->current->next;
    list->c_index = list->c_index + 1;
  }
}

bool clist_insert(CList *list, int index, void *data) {
  if (index < 0)
    return false;

  if (index > list->size)
    index = list->size;

  struct _clist_ele *node = malloc(sizeof(struct _clist_ele));
  if (node == NULL)
    return false;

  node->data = data;

  if (list->tail == NULL) {
    list->tail = node;
    list->tail->next = node;
    list->current = node;
    list->c_index = 0;
  } else {
    mcur_pointer(list, (index - 1) % list->size);
    node->next = list->current->next;
    list->current->next = node;

    if (index == list->size)
      list->tail = node;

    if (list->c_index < list->size / 2) {
      list->c_index++;
      list->current = list->current->next;
    }
  }

  list->size++;

  return true;
}

bool clist_remove(CList *list, int index, void **data) {
  if (list->size <= 0 || index < 0 || index > list->size - 1)
    return false;

  struct _clist_ele *node;

  if (list->size == 1) {
    node = list->tail;

    list->current = NULL;
    list->c_index = -1;
    list->tail = NULL;
  } else {
    mcur_pointer(list, (index - 1 + list->size) % list->size);
    node = list->current->next;
    list->current->next = node->next;

    if (index == list->size - 1) {
      list->tail = list->current;
    } else if (list->c_index == list->size - 1) {
      list->c_index--;
    }
  }

  if (data == NULL) {
    list->ele_free(node->data);
  } else {
    *data = node->data;
  }

  node->next = NULL;
  free(node);

  list->size--;

  return true;
}

bool clist_get(CList *list, int index, void **data) {
  if (list->size <= 0 || index < 0 || index > list->size - 1)
    return false;

  mcur_pointer(list, index);
  *data = list->current->data;

  return true;
}

int clist_next_data(CList *list, void **data) {
  list->current = list->current->next;
  list->c_index = (list->c_index + 1) % list->size;

  *data = list->current->data;
  return list->c_index;
}

bool clist_update(CList *list, int index, void *data) {
  if (list->size <= 0 || index < 0 || index > list->size - 1)
    return false;

  mcur_pointer(list, index);

  if (list->current->data != data) {
    list->ele_free(list->current->data);
    list->current->data = data;
  }

  return true;
}

int clist_find(CList *list, void *data, int (*match)(const void *, const void *)) {
  if (data == NULL)
    return -1;

  struct _clist_ele *temp = list->current;
  do {
    if (!match(data, list->current->data))
      return list->c_index;

    list->c_index = (list->c_index + 1) % list->size;
    list->current = list->current->next;
  } while (temp != list->current);

  return -1;
}

//static struct _clist_ele *reverse(struct _clist_ele *node, struct _clist_ele *head) {
//  if (node->next == head)
//    return node;
//
//  struct _clist_ele *temp = reverse(node->next, head);
//  temp->next = node;
//  node->next = NULL;
//
//  return node;
//}

//void clist_reverse(CList *list) {
//  if (list->size <= 1)
//    return;
//
//  list->current = reverse(list->tail->next, list->tail->next);
//  list->current->next = list->tail;
//  list->tail = list->current;
//  list->c_index = list->size - 1;
//}

static struct _clist_ele *reverse(CList *list) {
  struct _clist_ele *tail = list->tail;
  struct _clist_ele *head = tail->next;
  struct _clist_ele *temp;
  do {
    temp = head->next;
    head->next = tail;
    tail = head;
    head = temp;
  } while (head != list->tail);

  return tail;
}

void clist_reverse(CList *list) {
  if (list->size <= 1)
    return;

  list->current = list->tail->next;
  list->tail->next = reverse(list);
  list->tail = list->current;
  list->c_index = list->size - 1;
}

int clist_size(CList *list) {
  return list->size;
}