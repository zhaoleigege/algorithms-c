/**
 * 文件 list 创建于 2017/10/17
 * @author 赵磊
 * @version 1.0
 */

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef void (*node_free)(void *data);

typedef struct _list List;

bool list_init(List **list, void (*ele_free)(void *), int (*match)(const void *v1, const void *v2));

void list_destroy(List **list);

bool list_insert(List *list, int index, void *data);

bool list_remove(List *list, int index, void **data);

bool list_get(List *list, int index, void **data);

bool list_update(List *list, int index, void *data);

int list_find(List *list, const void *data, int (*match)(const void *, const void *));

void list_reverse(List *list);

int list_size(List *list);

node_free get_ele_free(List *list);

#endif
