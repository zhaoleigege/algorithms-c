/**
 * 文件 clist 创建于 2017/10/20
 * @author 赵磊
 * @version 1.0
 */

#include <stdbool.h>
#ifndef CLIST_H
#define CLIST_H

typedef struct _clist CList;

bool clist_init(CList **list, void (*ele_free)(void *ele));

void clist_destroy(CList **list);

bool clist_insert(CList *list, int index, void *data);

bool clist_remove(CList *list, int index, void **data);

bool clist_get(CList *list, int index, void **data);

int clist_next_data(CList *list, void **data);

bool clist_update(CList *list, int index, void *data);

int clist_find(CList *list, void *data, int (*match)(const void *, const void *));

void clist_reverse(CList *list);

int clist_size(CList *list);

#endif
