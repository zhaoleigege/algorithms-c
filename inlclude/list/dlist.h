/**
 * 文件 dlist 创建于 2017/10/19
 * @author 赵磊
 * @version 1.0
 */

#include <stdbool.h>
#ifndef DLIST_H
#define DLIST_H

typedef struct _dlist DList;

typedef void *DListValue;

bool dlist_init(DList **list, void(*ele_free)(DListValue data));

void dlist_destroy(DList **list);

bool dlist_insert(DList *list, int index, DListValue data);

bool dlist_remove(DList *list, int index, DListValue *data);

bool dlist_get(DList *list, int index, DListValue *data);

bool dlist_update(DList *list, int index, DListValue data);

int dlist_find(DList *list, DListValue data, int (*match)(const void *, const void *));

void dlist_reverse(DList *list);

int dlist_size(DList *list);

#endif
