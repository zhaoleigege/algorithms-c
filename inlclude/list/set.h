/**
 * 文件 set 创建于 2017/10/20
 * @author 赵磊
 * @version 1.0
 */

#include "list.h"
#ifndef SET_H
#define SET_H

typedef List Set;

bool set_init(Set *set, int (*match)(const void *v1, const void *v2), void (*ele_free)(void *data));

#define set_destroy list_destroy

bool set_insert(Set *set, const void *data);

bool set_remove(Set *set, void **data);

bool set_union(Set *setu, const Set *set1, const Set *set2);

bool set_intersection(Set *seti, const Set *set1, const Set *set2);

bool set_difference(Set *setd, const Set *set1, const Set *set2);

bool set_is_member(Set *set, const void *data);

bool set_is_subset(const Set *set1, const Set *set2);

bool set_is_equal(const Set *set1, const Set *set2);

int set_size(Set *set);

#endif
