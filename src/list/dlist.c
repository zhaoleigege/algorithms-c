/**
 * 文件 dlist 创建于 2017/10/19
 * @author 赵磊
 * @version 1.0
 */

#include <stdlib.h>
#include "../../inlclude/list/dlist.h"

struct _dlist_ele {
  DListValue data;
  struct _dlist_ele *prev;
  struct _dlist_ele *next;
};

struct _dlist {
  int size;

  struct _dlist_ele *head;
  struct _dlist_ele *tail;

  struct _dlist_ele *current;
  int c_index;

  void (*ele_free)(DListValue data);
};

/**
 * 双链表的初始化
 * @param list 链表对象
 * @param ele_free 释放链表节点的方法
 * @return  初始化是否成功
 */
bool dlist_init(DList **list, void(*ele_free)(DListValue data)) {
  if (ele_free == NULL)
    return false;

  if (((*list) = malloc(sizeof(struct _dlist))) == NULL)
    return false;

  (*list)->size = 0;
  (*list)->head = NULL;
  (*list)->tail = NULL;

  (*list)->current = (*list)->head;
  (*list)->c_index = 0;

  (*list)->ele_free = ele_free;

  return true;
}

/**
 * 双链表的销毁
 * @param list 链表对象
 */
void dlist_destroy(DList **list) {
  while ((*list)->size > 0)
    dlist_remove(*list, 0, NULL);

  (*list)->head = NULL;
  (*list)->tail = NULL;
  (*list)->current = NULL;
  (*list)->ele_free = NULL;

  free(*list);
  *list = NULL;
}

static void dlist_mforward(DList *list, int index) {
  while (list->c_index < index) {
    list->c_index++;
    list->current = list->current->next;
  }
}

static void dlist_mbackward(DList *list, int index) {
  while (list->c_index > index) {
    list->c_index--;
    list->current = list->current->prev;
  }
}

static void mcur_pointer(DList *list, int index) {
  if (index <= list->c_index) {
    if (2 * index >= list->c_index) {
      dlist_mbackward(list, index);
    } else {
      list->c_index = 0;
      list->current = list->head;
      dlist_mforward(list, index);
    }
  } else {
    if (2 * index < (list->c_index + list->size)) {
      dlist_mforward(list, index);
    } else {
      list->c_index = list->size - 1;
      list->current = list->tail;
      dlist_mbackward(list, index);
    }
  }
}

static void chg_cur_pointer(DList *list) {
  int gap = list->c_index - list->size / 2;
  if (gap < 0) {
    list->current = list->current->next;
    list->c_index++;
  } else if (gap > 0) {
    list->current = list->current->prev;
    list->c_index--;
  }
}

static void dlist_insert_head(DList *list, struct _dlist_ele *ele) {
  struct _dlist_ele *node = list->head;
  ele->next = node;
  list->head = ele;

  if (node == NULL) {
    list->tail = ele;
    list->current = list->head;
  } else {
    chg_cur_pointer(list);
    node->prev = ele;
  }
}

static void dlist_insert_tail(DList *list, struct _dlist_ele *ele) {
  ele->prev = list->tail;
  list->tail->next = ele;
  list->tail = ele;

  chg_cur_pointer(list);
}

/**
 * 双链表的插入
 * @param list  链表对象
 * @param index 插入的位置(下标从0开始)
 * @param data 插入的内容
 * @return 插入是否成功
 */
bool dlist_insert(DList *list, int index, DListValue data) {
  if (index < 0)
    return false;

  if (index > list->size)
    index = list->size;

  struct _dlist_ele *node = malloc(sizeof(struct _dlist_ele));
  if (node == NULL)
    return false;

  node->data = data;
  node->prev = NULL;
  node->next = NULL;

  if (index == 0) {
    dlist_insert_head(list, node);
  } else if (index == list->size) {
    dlist_insert_tail(list, node);
  } else {
    mcur_pointer(list, index);

    node->prev = list->current->prev;
    node->next = list->current;
    list->current->prev->next = node;
    list->current->prev = node;
    list->current = node;
  }

  list->size++;

  return true;
}

/**
 * 双链表删除一个节点
 * @param list 链表对象
 * @param index 删除的索引(从0到size-1)
 * @param data 删除的节点中数据引用
 * @return 返回删除是否成功
 */
bool dlist_remove(DList *list, int index, DListValue *data) {
  if (index < 0 || index >= list->size)
    return false;

  struct _dlist_ele *node;

  if (index == 0) {
    node = list->head;

    list->head = node->next;
    if (list->head == NULL) {
      list->tail = NULL;
      list->current = list->head;
      list->c_index = 0;
    } else {
      list->head->prev = NULL;
      if (list->c_index > 0)
        list->c_index--;
      else
        list->current = list->head;
    }
  } else if (index == list->size - 1) {
    node = list->tail;

    list->tail = node->prev;
    list->tail->next = NULL;

    if (list->current == node) {
      list->current = list->tail;
      list->c_index--;
    }
  } else {
    mcur_pointer(list, index);

    list->current->next->prev = list->current->prev;
    list->current->prev->next = list->current->next;

    node = list->current;
    list->current = node->prev;
    list->c_index--;
  }

  node->prev = NULL;
  node->next = NULL;

  if (data != NULL)
    *data = node->data;
  else
    list->ele_free(node->data);

  free(node);

  list->size--;

  return true;
}

/**
 * 获得指定索引的节点数据
 * @param list  链表对象
 * @param index 索引
 * @param data 存储数据的对象引用
 * @return 是否找到数据
 */
bool dlist_get(DList *list, int index, DListValue *data) {
  if (index < 0 || index > list->size - 1)
    return false;

  mcur_pointer(list, index);
  *data = list->current->data;

  return true;
}

bool dlist_update(DList *list, int index, DListValue data) {
  if (index < 0 || index > list->size - 1)
    return false;

  mcur_pointer(list, index);
  if (list->current->data == data)
    list->current->data = data;
  else {
    list->ele_free(list->current->data);
    list->current->data = data;
  }

  return true;
}

int dlist_find(DList *list, DListValue data, int (*match)(const void *, const void *)) {
  int times = 0;
  while (list->current != NULL && times++ < list->size) {
    if (!match(data, list->current->data))
      return list->c_index;

    list->c_index++;
    list->current = list->current->next;
  }

  list->c_index = 0;
  list->current = list->head;
  while (times++ < list->size) {
    if (!match(data, list->current->data))
      return list->c_index;

    list->c_index++;
    list->current = list->current->next;
  }

  return -1;
}

/*
static struct _dlist_ele *reverse(struct _dlist_ele *head) {
  if (head->next == NULL) {
    return head;
  }

  struct _dlist_ele *node = reverse(head->next);
  node->prev = NULL;
  node->next = head;
  head->prev = node;
  head->next = NULL;

  return head;
}

void dlist_reverse(DList *list) {
  if (list->size <= 0)
    return;

  list->current = list->tail;
  list->tail = reverse(list->head);

  list->head = list->current;
  list->current = list->head;
  list->c_index = 0;
}
*/

static void tail_reverse(struct _dlist_ele *head, struct _dlist_ele *tail) {

  while (head != NULL) {
    struct _dlist_ele *temp = head->prev;

    tail->next = head;
    head->prev = tail;
    head->next = NULL;

    tail = head;
    head = temp;
  }

//  if (head == NULL)
//    return;
//
//  struct _dlist_ele *temp = head->prev;
//
//  tail->next = head;
//  head->prev = tail;
//  head->next = NULL;
//
//  tail_reverse(temp, head);
}

void dlist_reverse(DList *list) {
  if (list->size <= 1)
    return;

  tail_reverse(list->tail->prev, list->tail);
  list->tail->prev = NULL;
  list->current = list->tail;
  list->c_index = 0;
  list->tail = list->head;
  list->head = list->current;
}

int dlist_size(DList *list) {
  return list->size;
}
