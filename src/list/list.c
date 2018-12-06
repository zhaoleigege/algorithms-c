/**
 * 文件 list 创建于 2017/10/17
 * @author 赵磊
 * @version 1.0
 */

#include <stdlib.h>
#include <printf.h>
#include "../../inlclude/list/list.h"

struct _list_ele {
  void *data;                                              //链表中节点的数据
  struct _list_ele *next;                                       //指向下一个节点的指针
};

struct _list {
  int size;                                                     //链表大小

  struct _list_ele *head;                                               //链表头节点
  struct _list_ele *tail;                                               //链表尾节点

  int c_index;                                                   //链表当前指针索引
  struct _list_ele *current;                                            //链表当前指针

  node_free ele_free;                               //链表节点释放方法
  int (*match)(const void *v1, const void *v2);
};

/**
 * 链表的初始化
 * @param list 链表对象
 * @param ele_free  链表节点释放方法
 * @return 创建是否成功
 */
bool list_init(List **list, void (*ele_free)(void *), int (*match)(const void *v1, const void *v2)) {
  *list = malloc(sizeof(struct _list));
  if ((*list) == NULL)
    return false;

  if (match != NULL)
    (*list)->match = match;

  (*list)->size = 0;
  (*list)->head = NULL;
  (*list)->tail = NULL;
  (*list)->c_index = 0;
  (*list)->current = (*list)->head;
  (*list)->ele_free = ele_free;
  (*list)->match = NULL;

  return true;
}

/**
 * 链表对象销毁
 * @param list 链表对象
 */
void list_destroy(List **list) {
  if (*list == NULL)
    return;

  while ((*list)->size > 0)
    list_remove(*list, 0, NULL);

  (*list)->head = NULL;
  (*list)->tail = NULL;
  (*list)->current = NULL;
  (*list)->ele_free = NULL;
  (*list)->match = NULL;

  free(*list);
  *list = NULL;
}

/**
 * 从头部插入数据
 * @param list 链表对象
 * @param ele 节点指针
 * @return
 */
static void _list_insert_head(List *list, struct _list_ele *ele) {
  if (list->head == NULL)
    list->tail = ele;
  else
    ele->next = list->head;

  list->head = ele;
  list->c_index = 0;
  list->current = list->head;
}

/**
 * 从尾部插入数据
 * @param list 链表对象
 * @param ele 节点指针
 * @return
 */
static void _list_insert_tail(List *list, struct _list_ele *ele) {
  list->tail->next = ele;
  list->tail = ele;
}

/**
 * 插入一个节点
 * @param list 链表对象
 * @param index 插入的位置，索引不得小于0
 * @param data 插入的数据
 * @return 插入是否成功
 */
bool list_insert(List *list, int index, void *data) {
  if (index < 0)
    return false;

  if (index > list->size)
    index = list->size;

  struct _list_ele *ele = malloc(sizeof(struct _list_ele));
  if (ele == NULL)
    return false;

  ele->data = data;
  ele->next = NULL;

  if (index == 0) {
    _list_insert_head(list, ele);
  } else if (index == list->size) {
    _list_insert_tail(list, ele);
  } else {
    if (list->c_index >= index) {
      list->current = list->head;
      list->c_index = 0;
    }

    while (list->c_index < index - 1) {
      list->current = list->current->next;
      list->c_index++;
    }

    ele->next = list->current->next;
    list->current->next = ele;

    list->current = ele;
    list->c_index++;
  }

  list->size++;

  return true;
}

/**
 * 删除头节点的数据
 * @param list 链表对象
 * @param data 用来承载删除的数据的引用
 * @return
 */
static bool _list_remove_first(List *list, void **data) {
  struct _list_ele *old_ele = list->head;
  if (data != NULL)
    *data = old_ele->data;
  else {
    if (list->ele_free != NULL)
      list->ele_free(old_ele->data);
  }
  list->head = old_ele->next;

  free(old_ele);
  list->size--;
  if (list->head == NULL) {
    list->tail = NULL;
    list->c_index = 0;
    list->current = list->head;
  } else {
    if (list->c_index > 0)
      list->c_index--;
    else
      list->current = list->head;
  }

  return true;
}

/**
 * 删除一个节点
 * @param list 链表对象
 * @param index 删除的索引
 * @param data 用来承载删除的数据的引用
 * @return 是否删除成功
 */
bool list_remove(List *list, int index, void **data) {
  if (list->size <= 0 || index < 0 || index >= list->size)
    return false;

  if (index == 0)
    return _list_remove_first(list, data);

  if (index <= list->c_index) {
    list->current = list->head;
    list->c_index = 0;
  }

  while (list->c_index < index - 1) {
    list->current = list->current->next;
    list->c_index++;
  }

  struct _list_ele *old_ele = list->current->next;

  if (data != NULL)
    *data = old_ele->data;
  else {
    if (list->ele_free != NULL)
      list->ele_free(old_ele->data);
  }

  list->current->next = old_ele->next;

  free(old_ele);
  if (list->current->next == NULL)
    list->tail = list->current;

  list->size--;
  return true;
}

/**
 * 查找对应索引的节点对象
 * @param list 链表对象
 * @param index 索引
 * @return 找到的节点对象
 */
static struct _list_ele *_index_ele(List *list, int index) {
  if (index < 0 || index >= list->size)
    return NULL;

  if (list->c_index > index) {
    list->c_index = 0;
    list->current = list->head;
  }

  while (list->c_index < index) {
    list->c_index++;
    list->current = list->current->next;
  }

  return list->current;
}

/**
 * 返回给定索引的节点数据
 * @param list 链表对象
 * @param index 索引
 * @param data 用来承载返回数据的对象引用
 * @return  该索引是否存在
 */
bool list_get(List *list, int index, void **data) {
  struct _list_ele *ele = _index_ele(list, index);
  if (ele == NULL)
    return false;

  *data = ele->data;
  return true;
}

/**
 * 更新对应索引的节点数据
 * @param list
 * @param index
 * @param data
 * @return
 */
bool list_update(List *list, int index, void *data) {
  struct _list_ele *ele = _index_ele(list, index);
  if (ele == NULL)
    return false;

  if (ele->data == data) {
    ele->data = data;
  } else {
    list->ele_free(ele->data);
    ele->data = data;
  }
  return true;
}

/**
 * 从头开始找第一个遇到的对应数据的索引
 * @param list 链表对象
 * @param data 要找的数据对象
 * @param match 判断对象相等的函数
 * @return 找到的索引下标
 */
int list_find(List *list, const void *data, int (*match)(const void *, const void *)) {
  if (list->size <= 0)
    return -1;

  if (match == NULL) {
    if (list->match != NULL)
      match = list->match;
    else
      return -1;
  }

  if (!match(list->head->data, data))
    return 0;

  list->c_index = 0;
  list->current = list->head;

  while (list->current->next != NULL) {
    if (!match(list->current->next->data, data))
      return list->c_index + 1;

    list->current = list->current->next;
    list->c_index++;
  }

  return -1;
}

static struct _list_ele *_reverse(struct _list_ele *head) {
  if (head->next == NULL)
    return head;

  struct _list_ele *temp = _reverse(head->next);

  temp->next = head;
  head->next = NULL;
  return head;
}

/**
 * 链表反转
 * @param list 链表对象
 */
void list_reverse(List *list) {
  struct _list_ele *temp = list->tail;
  list->tail = _reverse(list->head);
  list->head = temp;
  list->current = list->head;
  list->c_index = 0;
}

/**
 * 返回链表的大小
 * @param list 链表对象
 * @return
 */
int list_size(List *list) {
  return list->size;
}

node_free get_ele_free(List *list) {
  node_free *address = (node_free *) ((char *) list + __offsetof(List, ele_free));
  printf("address-> %p\n", *address);
  return list->ele_free;
}