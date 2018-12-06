/**
 * 文件 set 创建于 2017/10/20
 * @author 赵磊
 * @version 1.0
 */

#include <stdlib.h>
#include "../../inlclude/list/set.h"
#include "../../inlclude/list/list.h"

bool set_init(Set *set, int (*match)(const void *v1, const void *v2), void (*ele_free)(void *data)) {
  if (ele_free == NULL || match == NULL)
    return false;

  if (list_init(&set, ele_free, match))
    return true;
  else
    return false;
}

bool set_is_member(Set *set, const void *data) {
  return list_find(set, data, NULL) != -1;
}

bool set_insert(Set *set, const void *data) {
  if (set_is_member(set, data))
    return false;

  return list_insert(set, 0, data);
}

bool set_remove(Set *set, void **data) {
  int index = list_find(set, *data, NULL);

  if (index == -1)
    return false;

  return list_remove(set, index, NULL);
}

bool set_union(Set *setu, const Set *set1, const Set *set2) {
//  list_init(&setu, )
}
